#include "filesystem.h"

FileSystem::FileSystem() {
	this->currentDirectory.parent = nullptr;
	this->currentDirectory.kids = nullptr;
	this->currentDirectory.blockNr = -5;
	this->currentDirectory.nbrOfKids = 0;
	this->currentDirectory.directoryName = '/';
}

FileSystem::~FileSystem() {

}

void FileSystem::CreateNewNode(std::string dirName, node* parent)
{
	node *newNode = new node;
	newNode->directoryName = dirName;
	newNode->parent = parent;
	newNode->kids = nullptr;
	newNode->nbrOfKids = 0;
	newNode->blockNr = -5;

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


int FileSystem::CheckKids(node *currentNode, std::vector<std::string> dirs, unsigned int index)
{
	int result = -5;

	if (dirs.size() -1 == index && currentNode->nbrOfKids == 0)
	{
		//skapa mapp
		std::cout << "skapa mapp 1" << std::endl;
		//Skapamappfunktion
		CreateNewNode(dirs[index], currentNode);

		result = 0;
	}
	else
	{
		bool madeDir = false;
		for(unsigned int k = 0; k < currentNode->nbrOfKids; k++)
		{
			if (dirs.size() - 1 > index)
			{
				if(currentNode->kids[k]->directoryName.compare(dirs[index]) == 0)
				{
					index = index +1;
					std::cout << "Dirs size index: " << dirs.size() << " Index: " << index << std::endl;
					result = CheckKids(currentNode->kids[k], dirs, index);
					k = currentNode->nbrOfKids;//exit loop
					madeDir = true;
				}
			}
			else //in last folder
			{
				//om mappen redan finns
				if(currentNode->kids[k]->directoryName.compare(dirs[index]) == 0)
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
			result = 0;
		}
	}
	return result;
}

int FileSystem::MakeDirectory(std::string dirPath, std::string currentDir)
{
	std::vector<std::string> dirs;
	std::string temp = "";
	std::cout << "Dir path lenght: " << dirPath.length() << std::endl;
	int result = -5;

	for (unsigned int i = 0; i <= dirPath.length(); i++)
	{

		if(dirPath[i] == '/')
		{
			dirs.push_back(temp);
			temp = "";
		}
		else if(i == dirPath.length() && dirPath[i] != '/')
		{
			dirs.push_back(temp);
			temp = "";
		}
		else
		{
			temp += dirPath[i];
		}
	}
	if(dirPath[0]=='/')
	{
		//Absolut path
		result = CheckKids(&root, dirs, 1);
		std::cout << "Check kids result: " << result << std::endl;
	}
	else
	{
		result = CheckKids(&this->currentDirectory, dirs, 0);
		std::cout << "Check kids result: " <<  result << std::endl;
	}
	return result;
}
