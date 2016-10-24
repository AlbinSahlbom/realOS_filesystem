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
        std::string directoryName;
        unsigned int nbrOfKids;
        node** kids;
        node* parent;
        int blockNr;    //-5 if dir otherwhise 0-249 for blocks
    } root;
	node * currentDirectory;

    

public:
	FileSystem(int nbrOfBlocks);
    ~FileSystem();

    /* These commands needs to implemented
     *
     * However, you are free to change the parameterlist and specify your own returntype for each function below.
     */

    /* This function creates a file in the filesystem */
    int CreateFile(std::string filePath, std::string currentDir);
    
    /* Creates a folder in the filesystem */
    int MakeDirectory(std::string dirPath, std::string currentDir);

    /* Removes a file in the filesystem */
    // removeFile(...);

    /* Removes a folder in the filesystem */
    // removeFolder(...);

    /* Function will move the current location to a specified location in the filesystem */
	int GoToDirectory(std::string dirPath, std::string &currentWorkDir);//cd

    /* This function will get all the files and folders in the specified folder */
    int ListDir(std::string dirPath, std::string currentDir);

	/* This function will return the absolut path to the working directory*/


    /* Add your own member-functions if needed */
private:
	std::vector<std::string> ConvertDirPathToVector(std::string dirPath);							//Used by all
	void CreateNewNode(std::string dirName, node* parent);											//mkdir
    int CheckKidsMakeDir(node *currentNode, std::vector<std::string> dirs, unsigned int index);		//mkdir
	int CheckKidsListDir(node *currentNode, std::vector<std::string> dirs, unsigned int index);		//ls
	int SetCurrentDirByPath(node *currentNode, std::vector<std::string> dirs, unsigned int index);	//cd
};

#endif // FILESYSTEM_H
