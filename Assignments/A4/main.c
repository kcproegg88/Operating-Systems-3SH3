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


    printf("\nCreating files...\n");
    createFile("alpha.txt", 3000);
    createFile("beta.txt", 5000);

    printf("\nAfter creation:\n");
    listFiles();
    printFreeBlocks();

    printf("\nDeleting alpha.txt...\n");
    deleteFile("alpha.txt");

    printf("\nAfter deletion:\n");
    listFiles();
    printFreeBlocks();

    return 0;
}