#include <stdio.h>
#include "fs_indexed.h"

int main(){
    printf("Initializing File System...\n");
    initFS();

    printf("\nInitial Free Blocks:\n");
    printFreeBlocks();

    // Test allocation
    printf("\nAllocating 2 blocks...\n");
    Block b1 = allocateFreeBlock();
    Block b2 = allocateFreeBlock();

    printFreeBlocks();

    // Test return
    printf("\nReturning blocks...\n");
    returnFreeBlock(b1.blockNumber);
    returnFreeBlock(b2.blockNumber);

    printFreeBlocks();


    createFile("alpha.txt", 3000);
    createFile("beta.txt", 5000);

    listFiles();
    printFreeBlocks();

    deleteFile("alpha.txt");

    listFiles();
    printFreeBlocks();

    return 0;
}