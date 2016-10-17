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
    // Here you can add your own data structures
    struct node
    {
        std::string directoryName;
        unsigned int nbrOfKids;
        node** kids;
        node* parent;
        int blockNr;    //-5 if dir otherwhise 0-249 for blocks
    } root, currentDirectory;

    void CreateNewNode(std::string dirName, node* parent);

    node FindDir(std::string path);
    node ChangeDir(std::string path);
public:
    FileSystem();
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
    // goToFolder(...);

    /* This function will get all the files and folders in the specified folder */
    // listDir(...);



    /* Add your own member-functions if needed */
    int CheckKids(node *currentNode, std::vector<std::string> dirs, unsigned int index);
};

#endif // FILESYSTEM_H
