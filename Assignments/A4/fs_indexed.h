#ifndef FS_INDEXED_H
#define FS_INDEXED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 1024
#define TOTAL_BLOCKS 64
#define MAXIMUM_FILES 10

typedef struct Block {
    unsigned char data[BLOCK_SIZE];
    int blockNumber;
} Block;

typedef struct Node {
    int blockNumber;
    struct Node* next;
} Node;

typedef struct FIB {
    int ID;
    char name[100];
    int file_size;
    int block_count;     // number of data blocks
    int index_block;     // block number of index block
    int used;            // 0 = available, 1 = occupied
} FIB;

typedef struct VCB {
    Block logical_blocks[TOTAL_BLOCKS];

    Node* free_head;
    Node* free_tail;
    int free_block_count;

    FIB files_created[MAXIMUM_FILES];
    int num_files_created;
} VCB;

/* global file system */
extern VCB volume_control_block;

/* required functions */
void initFS(void);
void createFile(char* filename, int size);
void deleteFile(char* filename);
void listFiles(void);
Block allocateFreeBlock(void);
void returnFreeBlock(int blockNumber);
void printFreeBlocks(void);

/* helper functions you should implement */
Node* addNode(int blockNumber);
int getFileInformationBlockID(void);
int findFileByName(const char* filename);

#endif