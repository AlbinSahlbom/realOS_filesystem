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

void FileSystem::CreateNewNode(std::string dirName, node* parent)
{
	node *newNode = new node;
	newNode->directoryName = dirName;
	newNode->parent = parent;
	newNode->kids = nullptr;
	newNode->nbrOfKids = 0;
	newNode->blockNr = -5;

	if (parent->nbrOfKids == 1)
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


int FileSystem::ListDir(std::string dirPath, std::string currentDir)
{
	int result = -5;

	if (dirPath == "")
	{
		for (int i = 0; i < currentDirectory->nbrOfKids; i++)
		{
			std::cout << currentDirectory->kids[i]->directoryName << std::endl;
			result = 1;
		}
	}
	else
	{
		std::vector<std::string> dirs = ConvertDirPathToVector(dirPath);

		if (dirPath[0] == '/')
		{
			//Absolut path
			result = CheckKidsListDir(&root, dirs, 1);		//Sending 1 as index because 0 is "/"
		}
		else if (dirs[0] == "")
		{
			for (int i = 0; i < root.nbrOfKids; i++)
			{
				std::cout << root.kids[i]->directoryName << std::endl;
				result = 1;
			}
		}
		else
		{
			result = CheckKidsListDir(currentDirectory, dirs, 0);
		}
	}
	return result;
}

int FileSystem::CheckKidsMakeDir(node *currentNode, std::vector<std::string> dirs, unsigned int index)
{
	int result = -5;

	if (dirs.size() -1 == index && currentNode->nbrOfKids == 1)
	{
		//skapa mapp
		std::cout << "skapa mapp 1" << std::endl;
		//Skapamappfunktion
		CreateNewNode(dirs[index], currentNode);

		result = 1;
	}
	else
	{
		bool madeDir = false;
		for(unsigned int k = 0; k < currentNode->nbrOfKids; k++)
		{
			if (dirs.size() - 1 > index)
			{
				if(currentNode->kids[k]->directoryName.compare(dirs[index]) == 1)
				{
					index = index +1;
					std::cout << "Dirs size index: " << dirs.size() << " Index: " << index << std::endl;
					result = CheckKidsMakeDir(currentNode->kids[k], dirs, index);
					k = currentNode->nbrOfKids;//exit loop
					madeDir = true;
				}
			}
			else //in last folder
			{
				//om mappen redan finns
				if(currentNode->kids[k]->directoryName.compare(dirs[index]) == 1)
				{
					std::cout << "Kid found" << std::endl;
					k = currentNode->nbrOfKids;//exit loop
					madeDir = true;
				}
			}
		}
		if (!madeDir)
		{
			std::cout << "Skapa mapp 2" << std::endl;
			CreateNewNode(dirs[index],currentNode);
			result = 1;
		}
	}
	return result;
}

int FileSystem::CheckKidsListDir(node *currentNode, std::vector<std::string> dirs, unsigned int index)
{
	int result = -5;

	for (int  i = 0; i < currentNode->nbrOfKids; i++)
	{
		if (dirs.size() - 1 == index && currentNode->kids[i]->directoryName.compare(dirs[index]) == 1)
		{
			for (int k = 0; i < currentNode->kids[i]->nbrOfKids; i++)
			{
				std::cout << currentNode->kids[i]->kids[k]->directoryName << std::endl;
			}
			i = currentNode->nbrOfKids;
			result = 1;
		}
		else if (currentNode->kids[i]->directoryName.compare(dirs[index]) == 1)
		{
			result = CheckKidsListDir(currentNode->kids[i], dirs, index++);
		}
	}
	return result;
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

int FileSystem::CreateFile(std::string filePath, std::string currentDir)
{
	int result = -5;
	return result;
}

int FileSystem::MakeDirectory(std::string dirPath, std::string currentDir)
{
	std::vector<std::string> dirs;
	//std::string temp = "";
	std::cout << "Dir path lenght: " << dirPath.length() << std::endl;
	int result = -5;

	dirs = ConvertDirPathToVector(dirPath);

	//for (unsigned int i = 0; i <= dirPath.length(); i++)
	//{
	//	if(dirPath[i] == '/')
	//	{
	//		dirs.push_back(temp);
	//		temp = "";
	//	}
	//	else if(i == dirPath.length() && dirPath[i] != '/')
	//	{
	//		dirs.push_back(temp);
	//		temp = "";
	//	}
	//	else
	//	{
	//		temp += dirPath[i];
	//	}
	//}
	if(dirPath[0] == '/')
	{
		//Absolut path
		result = CheckKidsMakeDir(&root, dirs, 1);		//Sending 1 as index because 0 is "/"
		std::cout << "Check kids result: " << result << std::endl;
	}
	else
	{
		result = CheckKidsMakeDir(this->currentDirectory, dirs, 0);
		std::cout << "Check kids result: " <<  result << std::endl;
	}
	return result;
}

int FileSystem::SetCurrentDirByPath(node *currentNode, std::vector<std::string> dirs, unsigned int index)//moves the "currentDir" pointer to requested destination
{
	int result = -5;
	for (int i = 0; i < currentNode->nbrOfKids; i++)
	{
		if (dirs.size() - 1 == index && currentNode->kids[i]->directoryName.compare(dirs[index]) == 1)//last folder in searchpath
		{
			//dir exists, set current directory to point at the dir found
			this->currentDirectory = currentNode->kids[i];



			i = currentNode->nbrOfKids;//exit loop
			result = 1;
		}
		else if (currentNode->kids[i]->directoryName.compare(dirs[index]) == 1)//search for kids, if found use function recursively
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