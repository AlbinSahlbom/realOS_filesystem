#include "memblockdevice.h"
#include <stdexcept>

MemBlockDevice::MemBlockDevice(int nrOfBlocks): BlockDevice(nrOfBlocks) {
    this->allBlockNbrs = new int[nrOfBlocks];
    for(int i = 0; i < nrOfBlocks; i++)
    {
        this->allBlockNbrs[i] = i;
    }
}

MemBlockDevice::MemBlockDevice(const MemBlockDevice &other) : BlockDevice(other) {

}

MemBlockDevice::~MemBlockDevice() {
    /* Implicit call to base-class destructor */
    delete [] this->allBlockNbrs;
}

MemBlockDevice& MemBlockDevice::operator=(const MemBlockDevice &other) {
    delete [] this->memBlocks;
    this->nrOfBlocks = other.nrOfBlocks;
    this->freePointer = other.freePointer;
    this->memBlocks = new Block[this->nrOfBlocks];

    for (int i = 0; i < this->nrOfBlocks; ++i)
        this->memBlocks[i] = other.memBlocks[i];

    return *this;
}

Block& MemBlockDevice::operator[](int index) const {
    if (index < 0 || index >= this->nrOfBlocks) {
        throw std::out_of_range("Illegal access\n");
    }
    else {
        return this->memBlocks[index];
    }
}

int MemBlockDevice::spaceLeft() const {
    /* Not yet implemented */
    return 0;
}

int MemBlockDevice::writeBlock(int blockNr, const std::vector<char> &vec) {
    int output = -1;    // Assume blockNr out-of-range
    blockNr = this->TakeFirstFreeBlockNbr();

    if (blockNr < this->nrOfBlocks && blockNr >= 0) {
        /* -2 = vec and block dont have same dimensions */
        /* 1 = success */
        output = this->memBlocks[blockNr].writeBlock(vec);
    }
    if(output == 1)
    {
        return blockNr;
    }
    /*else if(output == -2)
    {

    }*/
    return output;
}

int MemBlockDevice::writeBlock(int blockNr, const std::string &strBlock) {
    int output = -1;    // Assume blockNr out-of-range

    if (blockNr < this->nrOfBlocks && blockNr >= 0) {
        /* -2 = str-length and block dont have same dimensions */
        /* 1 = success */
        output = this->memBlocks[blockNr].writeBlock(strBlock);
    }
    return output;
}

int MemBlockDevice::writeBlock(int blockNr, const char cArr[]) {
    int output = -1;    // Assume blockNr out-of-range
    if (blockNr < this->nrOfBlocks && blockNr >= 0) {
        output = 1;
        // Underlying function writeBlock cannot check array-dimension.
        this->memBlocks[blockNr].writeBlock(cArr);
    }
    return output;
}

Block MemBlockDevice::readBlock(int blockNr) const {
    if (blockNr < 0 || blockNr >= this->nrOfBlocks)
        throw std::out_of_range("Block out of range");
    else {
        Block a(this->memBlocks[blockNr]);
        return a;
    }
}

/* Resets all the blocks */
void MemBlockDevice::reset() {
    for (int i = 0; i < this->nrOfBlocks; ++i) {
        this->memBlocks[i].reset('0');
    }
}

int MemBlockDevice::size() const {
    return this->nrOfBlocks;
}

int MemBlockDevice::DeleteBlock(int blockToDelete)
{
	delete &this->memBlocks[blockToDelete];
	return 1;
}

int MemBlockDevice::TakeFirstFreeBlockNbr()
{
    int firstFreeBlock = -5;
    for (int i = 0; i < this->nrOfBlocks; i++)
    {
        if (this->allBlockNbrs[i] == i)
        {
            firstFreeBlock = i;
            this->allBlockNbrs[i] = -5;
            i = this->nrOfBlocks;//break loop
        }
    }
    return firstFreeBlock;
}