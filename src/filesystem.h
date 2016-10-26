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
	node allNodes[250];

	void CreateNewNode(std::string dirName, node* parent, int blockNr);
	std::vector<std::string> ConvertDirPathToVector(std::string dirPath);

	int SetCurrentDirByPath(node *currentNode, std::vector<std::string> &dirs, unsigned int index);//cd
	int CheckKidsFindBlockNr(node *currentNode, std::vector<std::string> &dirs, unsigned int index);//returns the blocknumber of a file

	int GetDirectory(node *&currentNode, std::vector<std::string> &dirs, unsigned int index);

	int DeleteNode(node *nodeToBeDeleted);
	int DeleteFileBlock(int fileBlockToDelete);
	int RemoveFolder(node *currentNode, int kidNbr);
	int RemoveFile(node *currentNode, int kidNbr, int fileBlock);
public:
	FileSystem(int nbrOfBlocks);
    ~FileSystem();
	
	int CreateImageCd(std::string fileName);
	int RestoreImageCd(std::string fileName);
	int Format(std::string &currentDir);
	int rmFile(std::string fileToRemove);
	int rmDir(std::string dirToRemove);

	int mkdir(std::string dirPath);
	int create(std::string filePath, std::string fileContent);
	int cat(std::string filePath, std::string &fileContent);
	int cd(std::string dirPath);
	int ls(std::string dirPath, std::string &dirContent);
};

#endif // FILESYSTEM_H
