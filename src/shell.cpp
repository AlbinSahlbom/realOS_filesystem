#include <iostream>
#include <sstream>
#include <string>
#include "filesystem.h"

const int MAXCOMMANDS = 8;
const int NUMAVAILABLECOMMANDS = 16;

std::string availableCommands[NUMAVAILABLECOMMANDS] = {
    "quit","format","ls","create","cat","createImage","restoreImage",
    "rmFile","rmDir","cp","append","mv","mkdir","cd","pwd","help"
};

/* Takes usercommand from input and returns number of commands, commands are stored in strArr[] */
int ParseCommandString(const std::string &userCommand, std::string strArr[]);
int FindCommand(std::string &command);
void ClearCommandArr(std::string *commandArr);
std::string help();

int main(void) {
	const int nbrOfBlocks = 250;
    FileSystem* fileSystem = new FileSystem(nbrOfBlocks);		//Delete this when program quits

	std::string userCommand, commandArr[MAXCOMMANDS];
	std::string user = "user@DV1492";    // Change this if you want another user to be displayed
	std::string currentDir = "/";    // current directory, used for output

    bool bRun = true;

    do {
        std::cout << user << ":" << currentDir << "$ "<< std::endl;
        
        getline(std::cin, userCommand);

        int nrOfCommands = ParseCommandString(userCommand, commandArr);
        if (nrOfCommands > 0) {

            int cIndex = FindCommand(commandArr[0]);
            switch(cIndex) {
            case 0: // quit
                bRun = false;
                std::cout << "Exiting\n" << std::endl;
                break;
            case 1: // format				//Klar
				fileSystem->Format();
                break;
            case 2: // ls					//Klar
                std::cout << "Listing directory" << std::endl;
				if (fileSystem->ListDir(commandArr[1], currentDir) == -5)
				{
					std::cout << "ls: directory " << commandArr[1] << "not found or there are no files/directories in the map" << std::endl;
				}
				ClearCommandArr(commandArr);
                break;
            case 3: // create				//Klar
                break;
            case 4: // cat					//Klar
                break;
            case 5: // createImagecd		//Fixa
                break;
            case 6: // restoreImage			//Fixa
                break;
            case 7: // rmFile				//Klar
				fileSystem->rmFile(commandArr[1]);
                break;
			case 8:	// rmDir				//Klar
				fileSystem->rmDir(commandArr[1]);
				break;
            case 9: // cp					//Fixa
                break;
            case 10: // append				//Behover inte ha med
                break;
            case 11: // mv					//Behover inte ha med
                break;
            case 12: // mkdir				//Klar
			{
				std::cout << "Command: " << commandArr[0] << " with " << commandArr[1] << std::endl;
				if (fileSystem->MakeDirectory(commandArr[1], currentDir) == -5)
				{
					std::cout << "mkdir: cannot create directory " << commandArr[1] << ": No such file or directory\n" << std::endl;
				}
				ClearCommandArr(commandArr);
				break;
			}
            case 13: // cd					//Klar
			{
				std::cout << "Command: " << commandArr[0] << " with " << commandArr[1] << std::endl;
				if (fileSystem->GoToDirectory(commandArr[1], currentDir) == -5)
				{
					std::cout << "cd: cannot create directory " << commandArr[1] << ": No such file or directory\n" << std::endl;
					break;
				}
				ClearCommandArr(commandArr);
				break;
			}
            case 14: // pwd					//Klar
				std::cout << currentDir << std::endl;
				ClearCommandArr(commandArr);
                break;
            case 15: // help
                std::cout << help() << std::endl;
                break;
            default:
                std::cout << "Unknown command: " << commandArr[0] << std::endl;
            }
        }
    } while (bRun == true);

    return 0;
}

void ClearCommandArr(std::string *commandArr)
{
	for (int i = 0; i < MAXCOMMANDS; i++)
		commandArr[i] = "";
	return;
}

int ParseCommandString(const std::string &userCommand, std::string strArr[]) {
    
    std::stringstream ssin(userCommand);
    int counter = 0;

    while (ssin.good() && counter < MAXCOMMANDS) {
        ssin >> strArr[counter];
        counter++;
    }
    if (strArr[0] == "") {
        counter = 0;
    }
    return counter;
}
int FindCommand(std::string &command) {
    int index = -1;
    for (int i = 0; i < NUMAVAILABLECOMMANDS && index == -1; ++i) {

        if (command == availableCommands[i]) {
            index = i;
        }
    }
    return index;
}

std::string help() {
    std::string helpStr;
    helpStr += "OSD Disk Tool .oO Help Screen Oo.\n";
    helpStr += "-----------------------------------------------------------------------------------\n" ;
    helpStr += "* quit:                             Quit OSD Disk Tool\n";
    helpStr += "* format;                           Formats disk\n";
    helpStr += "* ls     <path>:                    Lists contents of <path>.\n";
    helpStr += "* create <path>:                    Creates a file and stores contents in <path>\n";
    helpStr += "* cat    <path>:                    Dumps contents of <file>.\n";
    helpStr += "* createImage  <real-file>:         Saves disk to <real-file>\n";
    helpStr += "* restoreImage <real-file>:         Reads <real-file> onto disk\n";
    helpStr += "* rm     <file>:                    Removes <file>\n";
    helpStr += "* cp     <source> <destination>:    Copy <source> to <destination>\n";
    helpStr += "* append <source> <destination>:    Appends contents of <source> to <destination>\n";
    helpStr += "* mv     <old-file> <new-file>:     Renames <old-file> to <new-file>\n";
    helpStr += "* mkdir  <directory>:               Creates a new directory called <directory>\n";
    helpStr += "* cd     <directory>:               Changes current working directory to <directory>\n";
    helpStr += "* pwd:                              Get current working directory\n";
    helpStr += "* help:                             Prints this help screen\n";
    return helpStr;
}
