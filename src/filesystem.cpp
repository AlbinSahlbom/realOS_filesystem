#include "filesystem.h"

FileSystem::FileSystem(int nbrOfBlocks) {
	this->root.parent = nullptr;
	this->root.kids = nullptr;
	this->root.blockNr = -5;
	this->root.nbrOfKids = 0;
	this->root.directoryName = "/";

	this->currentDirectory = &this->root;

	this->allBlockNbrs = new int[nbrOfBlocks];
	this->nbrOfBlocks = nbrOfBlocks;
	for (int i = 0; i < nbrOfBlocks; i++)
	{
		this->allBlockNbrs[i] = i;
	}
}

FileSystem::~FileSystem() {
	delete[] this->allBlockNbrs;
}

int FileSystem::TakeFirstFreeBlockNbr()
{
	int firstFreeBlock = -5;
	for (int i = 0; i < this->nbrOfBlocks; i++)
	{
		if (this->allBlockNbrs[i] == i)
		{
			firstFreeBlock = i;
			this->allBlockNbrs[i] = -5;//use number
			i = this->nbrOfBlocks;//break loop
		}
	}
	return firstFreeBlock;
}

void FileSystem::CreateNewNode(std::string dirName, node* parent, int blockNr)
{
	node *newNode = new node;
	newNode->directoryName = dirName;
	newNode->parent = parent;
	newNode->kids = nullptr;
	newNode->nbrOfKids = 0;
	newNode->blockNr = blockNr;

	if (parent->nbrOfKids == 0)
	{
		parent->kids = new node*[1];
		parent->kids[0] = newNode;
		parent->nbrOfKids = 1;
	}
	else
	{
		node ** temp = new node*[parent->nbrOfKids + 1];
		for (int i = 0; i < parent->nbrOfKids; ++i)
		{
			temp[i] = parent->kids[i];
		}
		temp[parent->nbrOfKids] = newNode;
		parent->kids = temp;
		parent->nbrOfKids++;
		temp = nullptr;

	}
	for (int i = 0; i < parent->nbrOfKids; i++)
		std::cout << parent->kids[i]->directoryName << std::endl;
	std::cout << parent->nbrOfKids << std::endl;
	return;
}


std::vector<std::string> FileSystem::ConvertDirPathToVector(std::string dirPath)
{
	std::vector<std::string> dirs;
	std::string temp = "";
	int asd = dirPath.length();
	for (unsigned int i = 0; i <= dirPath.length(); i++)
	{

		if (dirPath[i] == '/')
		{
			dirs.push_back(temp);
			temp = "";
		}
		else if (i == dirPath.length() && dirPath[i] != '/')
		{
			dirs.push_back(temp);
			temp = "";
		}
		else
		{
			temp += dirPath[i];
		}
	}
	return dirs;
}

int FileSystem::GetDirectory(node *&currentNode, std::vector<std::string> &dirs, unsigned int index)
{
	int res = -1;

	if (dirs.size() - 1 == index && currentNode->nbrOfKids == 0)//currentNode is last forlder, no kids found -> result == 1
	{
		res = 1;
	}
	else
	{
		for (unsigned int i = 0; i < currentNode->nbrOfKids; i++)//for each kid in current node
		{
			if (dirs.size() - 1 > index) 
			{
				if (currentNode->kids[i]->directoryName.compare(dirs[index]) == 0 && currentNode->kids[i]->blockNr == -5)//if directory in path, and is actually an directory and not a file
				{
					index++;
					currentNode = currentNode->kids[i];
					this->GetDirectory(currentNode, dirs, index);
					res = 1;
					i = currentNode->nbrOfKids;
					
				}
			}
			else//in last folder
			{
				if (currentNode->kids[i]->directoryName.compare(dirs[index]) == 0)//if in last folder, make sure requested name don't exist
				{
					res = -1;
					i = currentNode->nbrOfKids;
				}
				else
				{
					res = 1;
				}
			}
		}
	}

	return res;
}
int FileSystem::mkdir(std::string dirPath)
{
	int res = -1;
	std::vector<std::string> dirs = ConvertDirPathToVector(dirPath);
	node* currentNode;

	if (dirPath[0] == '/')
	{
		currentNode = &root;
		res = this->GetDirectory(currentNode, dirs, 1);//1 since first index is '/' for absolut path
	}
	else
	{
		currentNode = currentDirectory;
		res = this->GetDirectory(currentNode, dirs, 0);
	}

	if (res == 1)
	{
		CreateNewNode(dirs[dirs.size() -1], currentNode, -5);
	}

	return res;
}
int FileSystem::create(std::string filePath, std::string fileContent)
{
	int res = -1;
	int blockNr = this->TakeFirstFreeBlockNbr();
	std::vector<std::string> dirs = ConvertDirPathToVector(filePath);
	node* currentNode;

	if (filePath[0] == '/')
	{
		currentNode = &root;
		res = this->GetDirectory(currentNode, dirs, 1);//1 since first index is '/' for absolut path
	}
	else
	{
		currentNode = currentDirectory;
		res = this->GetDirectory(currentNode, dirs, 0);
	}

	if (res == 1)
	{
		CreateNewNode(dirs[dirs.size() - 1], currentNode, blockNr);
		this->mMemblockDevice.writeBlock(blockNr, fileContent);
	}
	

	return res;
}

int FileSystem::CheckKidsFindBlockNr(node *currentNode, std::vector<std::string> &dirs, unsigned int index)
{
	int blockNr = -1;

	if (dirs.size() - 1 == index && currentNode->nbrOfKids == 0)
	{
		blockNr = -1;
	}
	else
	{
		for (unsigned int i = 0; i < currentNode->nbrOfKids; i++)
		{
			if (dirs.size() - 1 > index)
			{
				if (currentNode->kids[i]->directoryName.compare(dirs[index]) == 0 && currentNode->kids[i]->blockNr == -5)
				{
					index++;
					blockNr = CheckKidsFindBlockNr(currentNode->kids[i], dirs, index);
					i = currentNode->nbrOfKids;//exit loop
				}
			}
			else //in last folder
			{
				if (currentNode->kids[i]->directoryName.compare(dirs[index]) == 0 && currentNode->kids[i]->blockNr > -1)
				{
					blockNr = currentNode->kids[i]->blockNr;
					i = currentNode->nbrOfKids;//exit loop
				}
				else
				{
					blockNr = -1;
				}
			}
		}
	}

	return blockNr;
}
int FileSystem::cat(std::string filePath, std::string &fileContent)
{
	int blockNr = -1;

	std::vector<std::string> dirs = ConvertDirPathToVector(filePath);
	node* currentNode;

	if (filePath[0] == '/')
	{
		blockNr = this->CheckKidsFindBlockNr(&this->root, dirs, 1);
	}
	else
	{
		blockNr = this->CheckKidsFindBlockNr(this->currentDirectory, dirs, 0);
	}

	if (blockNr != -1)
	{
		fileContent = this->mMemblockDevice.readBlock(blockNr).toString();
		
		return 1;
	}
	else
	{
		fileContent = "File not found";

		return -1;
	}
}

int FileSystem::SetCurrentDirByPath(node *currentNode, std::vector<std::string> &dirs, unsigned int index)//moves the "currentDir" pointer to requested destination
{
	int result = -1;
	for (int i = 0; i < currentNode->nbrOfKids; i++)
	{
		if (dirs.size() - 1 == index && currentNode->kids[i]->directoryName.compare(dirs[index]) == 0 && currentNode->kids[i]->blockNr == -5)//last folder in searchpath
		{
			//dir exists, set current directory to point at the dir found
			this->currentDirectory = currentNode->kids[i];
			i = currentNode->nbrOfKids;//exit loop
			result = 1;
		}
		else if (currentNode->kids[i]->directoryName.compare(dirs[index]) == 0 && currentNode->kids[i]->blockNr == -5)//search for kids, if found use function recursively
		{
			result = SetCurrentDirByPath(currentNode->kids[i], dirs, ++index);
		}
	}
	return result;
}
int FileSystem::cd(std::string dirPath)
{
	int res = -1;
	std::vector<std::string> dirs;
	
	dirs = ConvertDirPathToVector(dirPath);

	if (dirPath[0] == '/')//absolute path
	{
		if (dirPath.length() == 1)//just go to root
		{
			this->currentDirectory = &this->root;
			res = 1;
		}
		else//else reccusion
		{
			res = SetCurrentDirByPath(&root, dirs, 1);
		}
	}
	else
	{
		res = SetCurrentDirByPath(this->currentDirectory, dirs, 0);
	}

	return res;


}

int FileSystem::ls(std::string dirPath, std::string &dirContents)
{
	int res = -1;
	dirContents = "";

	int stopper = 0;

	if (dirPath == "")//currrent dir
	{
		if (this->currentDirectory->nbrOfKids == 0)
		{
			dirContents = "Current directory is empty";
		}
		else
		{
			for (int i = 0; i < this->currentDirectory->nbrOfKids; i++)
			{
				dirContents += this->currentDirectory->kids[i]->directoryName + " ";
			}
		}
		res = 1;
	}
	else
	{
		std::vector<std::string> dirs = ConvertDirPathToVector(dirPath);
		node* currentNode;

		if (dirPath[0] == '/')
		{
			currentNode = &root;
			res = this->GetDirectory(currentNode, dirs, 1);//1 since first index is '/' for absolut path
		}
		else
		{
			currentNode = currentDirectory;
			res = this->GetDirectory(currentNode, dirs, 0);
		}

		if (res == 1)
		{
			if (currentNode->nbrOfKids == 0)
			{
				dirContents = "Current directory is empty";
			}
			else
			{
				for (int i = 0; i < currentNode->nbrOfKids; i++)
				{
					dirContents += currentNode->kids[i]->directoryName + " ";
				}
			}
		}
	}

	return res;
}
