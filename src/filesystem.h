#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

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
        int blockNbr;    //-5 if dir otherwhise 0-249 for blocks
    } root;
	node * currentDirectory;
	node allNodes[250];

	void CreateNewNode(std::string dirName, node* parent, int blockNbr);
	std::vector<std::string> ConvertDirPathToVector(std::string dirPath);

	int SetCurrentDirByPath(node *currentNode, std::vector<std::string> &dirs, unsigned int index);//cd
	int CheckKidsFindBlockNbr(node *currentNode, std::vector<std::string> &dirs, unsigned int index);//returns the blocknumber of a file

	int GetDirectory(node *&currentNode, std::vector<std::string> &dirs, unsigned int index);

	int DeleteNode(node *nodeToBeDeleted);
	int DeleteFileBlock(int fileBlockToDelete);
	int RemoveFolder(node *currentNode, int kidNbr);
	int RemoveFile(node *currentNode, int kidNbr, int fileBlock);
	int SaveTree(std::ofstream &imageFile, node *currentNode);
	void LoadTree(std::ifstream &imageFile, node *parentNode, int index);

public:
	FileSystem(int nbrOfBlocks);
    ~FileSystem();
	
	int CreateImageCd(std::string fileName);
	int RestoreImageCd(std::string fileName, std::string &currentDir);
	int Format(std::string &currentDir);
	int rmFile(std::string fileToRemove);
	int rmDir(std::string dirToRemove);

	int mkdir(std::string dirPath);
	int create(std::string filePath, std::string fileContent);
	int cat(std::string filePath, std::string &fileContent);
	int cd(std::string dirPath, std::string &currentDir);
	int ls(std::string dirPath, std::string &dirContent);
	int cp(std::string fileName1, std::string fileName2);
	

};

#endif // FILESYSTEM_H
