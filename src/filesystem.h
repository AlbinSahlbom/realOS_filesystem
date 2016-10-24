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

	int SetCurrentDirByPath(node *currentNode, std::vector<std::string> dirs, unsigned int index);//cd
public:
    FileSystem(int nbrOfBlocks);
    ~FileSystem();

    /* These commands needs to implemented
     *
     * However, you are free to change the parameterlist and specify your own returntype for each function below.
     */

    /* This function creates a file in the filesystem */
	int CheckKidsMakeFile(node *currentNode, std::vector<std::string> dirs, unsigned int index, int blockNr);
    int CreateFile(std::string filePath);
    
    /* Creates a folder in the filesystem */
    int MakeDirectory(std::string dirPath);

	/* Copy file to a selected folder*/

    /* Removes a file in the filesystem */
    // removeFile(...);

    /* Removes a folder in the filesystem */
    int RemoveFolder(std::string dirPath, std::string currentDir);

    /* Function will move the current location to a specified location in the filesystem */
    int GoToDirectory(std::string dirPath);//cd

    /* This function will get all the files and folders in the specified folder */
    int ListDir(std::string dirPath);



    /* Add your own member-functions if needed */
    int CheckKidsMakeDir(node *currentNode, std::vector<std::string> dirs, unsigned int index);
	int CheckKidsListDir(node *currentNode, std::vector<std::string> dirs, unsigned int index);
	std::vector<std::string> ConvertDirPathToVector(std::string dirPath);
};

#endif // FILESYSTEM_H
