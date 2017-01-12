#include "filesystem.h"

FileSystem::FileSystem(int nbrOfBlocks) {
	this->root.parent = nullptr;
	this->root.kids = nullptr;
	this->root.blockNbr = -5;
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

	if (this->root.nbrOfKids > 0)
		this->Format(this->root.directoryName);

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

void FileSystem::CreateNewNode(std::string dirName, node* parent, int blockNbr)
{
	node *newNode = new node;
	newNode->directoryName = dirName;
	newNode->parent = parent;
	newNode->kids = nullptr;
	newNode->nbrOfKids = 0;
	newNode->blockNbr = blockNbr;

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

	return;
}

std::vector<std::string> FileSystem::ConvertDirPathToVector(std::string dirPath)
{
	std::vector<std::string> dirs;
	std::string temp = "";
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
				if (currentNode->kids[i]->directoryName.compare(dirs[index]) == 0 && currentNode->kids[i]->blockNbr == -5)//if directory in path, and is actually an directory and not a file
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
		CreateNewNode(dirs[dirs.size() - 1], currentNode, -5);
	}

	return res;
}

int FileSystem::create(std::string filePath, std::string fileContent)
{
	int res = -1;
	int blockNbr = this->TakeFirstFreeBlockNbr();
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
		CreateNewNode(dirs[dirs.size() - 1], currentNode, blockNbr);
		this->mMemblockDevice.writeBlock(blockNbr, fileContent);
	}

	return res;
}

int FileSystem::CheckKidsFindBlockNbr(node *currentNode, std::vector<std::string> &dirs, unsigned int index)
{
	int blockNbr = -1;

	if (dirs.size() - 1 == index && currentNode->nbrOfKids == 0)
	{
		blockNbr = -1;
	}
	else
	{
		for (unsigned int i = 0; i < currentNode->nbrOfKids; i++)
		{
			if (dirs.size() - 1 > index)
			{
				if (currentNode->kids[i]->directoryName.compare(dirs[index]) == 0 && currentNode->kids[i]->blockNbr == -5)
				{
					index++;
					blockNbr = CheckKidsFindBlockNbr(currentNode->kids[i], dirs, index);
					i = currentNode->nbrOfKids;//exit loop
				}
			}
			else //in last folder
			{
				if (currentNode->kids[i]->directoryName.compare(dirs[index]) == 0 && currentNode->kids[i]->blockNbr > -1)
				{
					blockNbr = currentNode->kids[i]->blockNbr;
					i = currentNode->nbrOfKids;//exit loop
				}
				else
				{
					blockNbr = -1;
				}
			}
		}
	}

	return blockNbr;
}

int FileSystem::cat(std::string filePath, std::string &fileContent)
{
	int blockNbr = -1;

	std::vector<std::string> dirs = ConvertDirPathToVector(filePath);

	if (filePath[0] == '/')
	{
		blockNbr = this->CheckKidsFindBlockNbr(&this->root, dirs, 1);
	}
	else
	{
		blockNbr = this->CheckKidsFindBlockNbr(this->currentDirectory, dirs, 0);
	}

	if (blockNbr != -1)
	{
		fileContent = this->mMemblockDevice.readBlock(blockNbr).toString();

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
		if (dirs.size() - 1 == index && currentNode->kids[i]->directoryName.compare(dirs[index]) == 0 && currentNode->kids[i]->blockNbr == -5)//last folder in searchpath
		{
			//dir exists, set current directory to point at the dir found
			this->currentDirectory = currentNode->kids[i];
			i = currentNode->nbrOfKids;//exit loop
			result = 1;
		}
		else if (currentNode->kids[i]->directoryName.compare(dirs[index]) == 0 && currentNode->kids[i]->blockNbr == -5)//search for kids, if found use function recursively
		{
			result = SetCurrentDirByPath(currentNode->kids[i], dirs, ++index);
		}
	}
	return result;
}

int FileSystem::cd(std::string dirPath, std::string &currentDir)
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
			node *temp = currentDirectory;
			currentDirectory = &root;
			res = SetCurrentDirByPath(currentDirectory, dirs, 1);
			if (res == -1)
			{
				currentDirectory = temp;
			}
		}
	}
	else
	{
		res = SetCurrentDirByPath(this->currentDirectory, dirs, 0);
	}
	if (res == 1 && dirPath[0] == '/')
	{
		currentDir = dirPath;
	}
	else if (res == 1 && dirPath[0] != '/')
	{
		if (currentDir.compare("/") == 0)
			currentDir += dirPath;
		else
			currentDir += '/' + dirPath;
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

int FileSystem::CreateImageCd(std::string fileName)
{
	int result = -1;

	std::ofstream imageFile;
	imageFile.open(fileName);

	if (imageFile.is_open())
	{
		result = 0;
		/* The root, special case so we do that first. The root is always the same, the onely thing that changes is nbrOfKids. */
		imageFile << root.nbrOfKids << "\n";
		for (int i = 0; i < root.nbrOfKids; i++)
		{
			result = SaveTree(imageFile, root.kids[i]);
		}

		imageFile.close();
	}
	return result;
}

int FileSystem::RestoreImageCd(std::string fileName, std::string &currentDir)
{
	int result = -1;

	std::ifstream imageFile(fileName);
	if (imageFile.is_open())
	{
		Format(currentDir);
		result = 0;
		std::string tempLine;
		int howManyLoops = 0;
		std::getline(imageFile, tempLine);
		howManyLoops = std::stoi(tempLine);
		for (int i = 0; i < howManyLoops; i++)
		{
			LoadTree(imageFile, &root, i);
		}
	}
	return result;
}

int FileSystem::Format(std::string &currentDir)
{
	currentDirectory = &this->root;
	currentDir = this->root.directoryName;
	int result = -5;

	/*Delete the whole filesystem*/
	result = DeleteNode(&root);

	return result;
}

int FileSystem::DeleteNode(node *nodeToBeDeleted)
{

	int result = -5;

	for (int i = 0; i < nodeToBeDeleted->nbrOfKids; i++)
	{
		if (nodeToBeDeleted->kids[i]->blockNbr != -5)
		{
			result = RemoveFile(nodeToBeDeleted, i, nodeToBeDeleted->kids[i]->blockNbr);

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
	if (nodeToBeDeleted->nbrOfKids == 0)
	{
		delete[] nodeToBeDeleted->kids;
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
			result = RemoveFile(currentDirectory, i, currentDirectory->kids[i]->blockNbr);
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
		if (currentDirectory->kids[i]->directoryName == dirToRemove && currentDirectory->kids[i]->blockNbr == -5)
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
	this->allBlockNbrs[fileBlock] = fileBlock;

	delete currentNode->kids[kidNbr];

	for (int i = kidNbr; i < currentNode->nbrOfKids - 1; i++)
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
	for (int i = kidNbr; i < currentNode->nbrOfKids - 1; i++)
	{
		currentNode->kids[i] = currentNode->kids[i + 1];
	}
	currentNode->kids[currentNode->nbrOfKids - 1] = nullptr;
	currentNode->nbrOfKids--;
	return result;
}

int FileSystem::SaveTree(std::ofstream &imageFile, node *currentNode)
{
	int result = 0;
	imageFile << currentNode->directoryName << "\n";
	imageFile << currentNode->blockNbr << "\n";
	if (currentNode->blockNbr != -5)
	{
		std::string tempString;
		tempString = this->mMemblockDevice.readBlock(currentNode->blockNbr).toString();
		imageFile << tempString << "\n";
	}
	else
		imageFile << currentNode->nbrOfKids << "\n";

	for (int i = 0; i < currentNode->nbrOfKids; i++)
	{
		result = SaveTree(imageFile, currentNode->kids[i]);
	}
	return result;
}

void FileSystem::LoadTree(std::ifstream &imageFile, node *parentNode, int index)
{
	std::string tempName;
	std::string tempBlockNbr;
	int tempBlockNbrInt = 0;
	std::string tempFileContent;
	std::string tempNbrOfKids;

	std::getline(imageFile, tempName);
	std::getline(imageFile, tempBlockNbr);
	tempBlockNbrInt = std::stoi(tempBlockNbr);
	if (tempBlockNbrInt != -5)
	{
		std::getline(imageFile, tempFileContent);
		tempBlockNbrInt = TakeFirstFreeBlockNbr();
		CreateNewNode(tempName, parentNode, tempBlockNbrInt);
		this->mMemblockDevice.writeBlock(tempBlockNbrInt, tempFileContent);
	}
	else
	{
		int howManyLoops = 0;
		std::getline(imageFile, tempNbrOfKids);
		howManyLoops = std::stoi(tempNbrOfKids);
		CreateNewNode(tempName, parentNode, tempBlockNbrInt);
		for (int i = 0; i < howManyLoops; i++)
		{
			LoadTree(imageFile, parentNode->kids[index], i);
		}
	}
	return;
}

int FileSystem::cp(std::string fileName1, std::string fileName2)
{
	int res = -1;

	if (fileName1.compare(fileName2) == 0)
	{
		res = -1;
	}
	else
	{
		for (int i = 0; i < this->currentDirectory->nbrOfKids; i++)
		{
			if (this->currentDirectory->kids[i]->directoryName.compare(fileName1) == 0)
			{
				//file exist in current folder
				std::string fileContent;
				this->cat(fileName1, fileContent);
				this->create(fileName2, fileContent);
				res = 1;
				i = this->currentDirectory->nbrOfKids;//exit loop
			}
			else
			{
				//file doesnt exist in current folder
				res = -1;
			}
		}
	}


	return res;
}
