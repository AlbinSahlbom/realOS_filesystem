#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include <vector>
#include <iostream>
#include <string>

class FileSystem
{
private:
    MemBlockDevice mMemblockDevice;
	int* allBlockNbrs;
	int nbrOfBlocks;
	int TakeFirstFreeBlockNbr();
    // Here you can add your own data structures
    struct node
    {
        std::string directoryName;//Dirname or file name
        unsigned int nbrOfKids;
        node** kids;
        node* parent;
        int blockNr;    //-5 if dir otherwhise 0-249 for blocks
	} root;
	node * currentDirectory;

    void CreateNewNode(std::string dirName, node* parent, int blockNr);
	std::vector<std::string> ConvertDirPathToVector(std::string dirPath);

	int SetCurrentDirByPath(node *currentNode, std::vector<std::string> &dirs, unsigned int index);//cd
	int CheckKidsFindBlockNr(node *currentNode, std::vector<std::string> &dirs, unsigned int index);//returns the blocknumber of a file

	int GetDirectory(node *&currentNode, std::vector<std::string> &dirs, unsigned int index);
	
public:
    FileSystem(int nbrOfBlocks);
    ~FileSystem();

    int mkdir(std::string dirPath);
	int create(std::string filePath, std::string fileContent);
	int cat(std::string filePath, std::string &fileContent);
	int cd(std::string dirPath);
	int ls(std::string dirPath, std::string &dirContent);
	
	
};

#endif // FILESYSTEM_H
