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


<<<<<<< HEAD
int FileSystem::CreateImageCd(std::string fileName)
{
	int result = -1;


	return result;
}

int FileSystem::RestoreImageCd(std::string fileName)
{
	int result = -1;


	return result;
}

int FileSystem::Format(std::string &currentDir)
{
	currentDirectory = &this->root;
	currentDir = this->root.directoryName;
	int result = -5;
	for (int i = 0; i < 250; i++)
	{
		this->allNodes[i].blockNr = i;
		this->allNodes[i].directoryName = "";
		this->allNodes[i].kids = nullptr;
		this->allNodes[i].nbrOfKids = 0;
		this->allNodes[i].parent = nullptr;
	}

	/*Delete the whole filesystem*/
	result = DeleteNode(&root);

	return result;
}

int FileSystem::ListDir(std::string dirPath, std::string currentDir)
=======
std::vector<std::string> FileSystem::ConvertDirPathToVector(std::string dirPath)
>>>>>>> 4ad6813f4147094e0d9c7af4082e79c2d7260dc9
{
	std::vector<std::string> dirs;
	std::string temp = "";
	int asd = dirPath.length();
	for (unsigned int i = 0; i <= dirPath.length(); i++)
	{
<<<<<<< HEAD
		for (int i = 0; i < currentDirectory->nbrOfKids; i++)
		{
			std::cout << currentDirectory->kids[i]->directoryName << std::endl;
			result = 1;
=======

		if (dirPath[i] == '/')
		{
			dirs.push_back(temp);
			temp = "";
>>>>>>> 4ad6813f4147094e0d9c7af4082e79c2d7260dc9
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
<<<<<<< HEAD
				std::cout << root.kids[i]->directoryName << std::endl;
				result = 1;
			}
		}
		else
		{
			result = CheckKidsListDir(currentDirectory, dirs, 0);
=======
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
>>>>>>> 4ad6813f4147094e0d9c7af4082e79c2d7260dc9
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

<<<<<<< HEAD
		result = 1;
=======
	if (dirs.size() - 1 == index && currentNode->nbrOfKids == 0)
	{
		blockNr = -1;
>>>>>>> 4ad6813f4147094e0d9c7af4082e79c2d7260dc9
	}
	else
	{
		for (unsigned int k = 0; k < currentNode->nbrOfKids; k++)
		{
			if (dirs.size() - 1 > index)
			{
				if (currentNode->kids[k]->directoryName.compare(dirs[index]) == 0)
				{
					index++;
					blockNr = CheckKidsFindBlockNr(currentNode->kids[k], dirs, index);
					k = currentNode->nbrOfKids;//exit loop
				}
			}
			else //in last folder
			{
				if (currentNode->kids[k]->directoryName.compare(dirs[index]) == 0)
				{
					blockNr = currentNode->kids[k]->blockNr;
					k = currentNode->nbrOfKids;//exit loop
				}
				else
				{
					blockNr = -1;
				}
			}
		}
<<<<<<< HEAD
		if (!madeDir)
		{
			std::cout << "Skapa mapp 2" << std::endl;
			CreateNewNode(dirs[index],currentNode);
			result = 1;
		}
=======
>>>>>>> 4ad6813f4147094e0d9c7af4082e79c2d7260dc9
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
<<<<<<< HEAD
			for (int k = 0; i < currentNode->kids[i]->nbrOfKids; i++)
			{
				std::cout << currentNode->kids[i]->kids[k]->directoryName << std::endl;
			}
			i = currentNode->nbrOfKids;
=======
			//dir exists, set current directory to point at the dir found
			this->currentDirectory = currentNode->kids[i];
			i = currentNode->nbrOfKids;//exit loop
>>>>>>> 4ad6813f4147094e0d9c7af4082e79c2d7260dc9
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
<<<<<<< HEAD
		result = CheckKidsMakeDir(this->currentDirectory, dirs, 0);
		std::cout << "Check kids result: " <<  result << std::endl;
	}
	return result;
}

int FileSystem::rmFile(std::string fileToRemove)
{
	int result = -5;
	for (int i = 0; i < currentDirectory->nbrOfKids; i++)
	{
		if (currentDirectory->kids[i]->directoryName == fileToRemove)
		{
			result = RemoveFile(currentDirectory, i, currentDirectory->kids[i]->blockNr);
			i = currentDirectory->nbrOfKids;
		}
	}

	return result;
}

int FileSystem::rmDir(std::string dirToRemove)
{
	int result = -5;
	for (int i = 0; i < currentDirectory->nbrOfKids; i++)
	{
		if (currentDirectory->kids[i]->directoryName == dirToRemove && currentDirectory->kids[i]->blockNr == -5)
		{
			result = RemoveFolder(currentDirectory, i);
			i = currentDirectory->nbrOfKids;
		}
	}
	return result;
}

int FileSystem::RemoveFile(node *currentNode, int kidNbr, int fileBlock)
{
	int result = -5;
	result = DeleteFileBlock(currentNode->blockNr);
	delete currentNode->kids[kidNbr];
	for (int i = kidNbr; i < currentNode->nbrOfKids-1; i++)
	{
		currentNode->kids[i] = currentNode->kids[i + 1];
	}
	currentNode->kids[currentNode->nbrOfKids - 1] = nullptr;
	currentNode->nbrOfKids--;
	return result;
}

int FileSystem::RemoveFolder(node *currentNode, int kidNbr)
{
	int result = 1;
	delete currentNode->kids[kidNbr];
	for (int i = kidNbr; i < currentNode->nbrOfKids-1; i++)
	{
		node *temp = currentNode->kids[i];
		node *temp2 = currentNode->kids[i + 1];
		currentNode->kids[i] = currentNode->kids[i + 1];
	}
	currentNode->kids[currentNode->nbrOfKids - 1] = nullptr;
	currentNode->nbrOfKids--;
	return result;
}

int FileSystem::SetCurrentDirByPath(node *currentNode, std::vector<std::string> dirs, unsigned int index)//moves the "currentDir" pointer to requested destination
{
	int result = -5;
	for (int i = 0; i < currentNode->nbrOfKids; i++)
	{
		if (dirs.size() - 1 == index && currentNode->kids[i]->directoryName.compare(dirs[index]) == 0)//last folder in searchpath
		{
			//dir exists, set current directory to point at the dir found
			this->currentDirectory = currentNode->kids[i];



			i = currentNode->nbrOfKids;//exit loop
			result = 1;
		}
		else if (currentNode->kids[i]->directoryName.compare(dirs[index]) == 0)//search for kids, if found use function recursively
		{
			result = SetCurrentDirByPath(currentNode->kids[i], dirs, ++index);
		}
	}
	return result;
}

int FileSystem::GoToDirectory(std::string dirPath, std::string &currentWorkDir)	//cd
{
	std::vector<std::string> dirs;

	std::cout << "CD called" << std::endl;
	int result = -5;

	dirs = ConvertDirPathToVector(dirPath);


	if (dirPath[0] == '/')
	{
		//Absolut path
		result = SetCurrentDirByPath(&root, dirs, 1);		//Sending 1 as index because 0 is "/"
		if (result == 1)
		{
			currentWorkDir = "/" + dirPath + "/";
		}
		std::cout << "Absolute path: " << result << std::endl;
	}
	else
	{
		result = SetCurrentDirByPath(this->currentDirectory, dirs, 0);
		if (result == 1)
		{
			currentWorkDir += dirPath + "/";
		}
		std::cout << "Path: " << result << std::endl;
	}

	return result;
}

int FileSystem::DeleteNode(node *nodeToBeDeleted)
{

	int result = -5;
	for (int i = 0; i < nodeToBeDeleted->nbrOfKids; i++)
	{
		if (nodeToBeDeleted->kids[i]->blockNr != -5)
		{
			result = RemoveFile(nodeToBeDeleted, i, nodeToBeDeleted->kids[i]->blockNr);
			i--;
		}
		else if (nodeToBeDeleted->kids[i]->nbrOfKids == 0)
		{
			result = RemoveFolder(nodeToBeDeleted, i);
			i--;
		}
		else
		{
			result = DeleteNode(nodeToBeDeleted->kids[i]);
			i--;
		}
	}
	return result;
}

int FileSystem::DeleteFileBlock(int fileBlockToDelete)
{
	int result = -5;
	result = mMemblockDevice.DeleteBlock(fileBlockToDelete);
	return result;
=======
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
>>>>>>> 4ad6813f4147094e0d9c7af4082e79c2d7260dc9
}
