#include <string.h>
#include <stdlib.h>

#define BLOCK_SIZE 1024
#define TOTAL_BLOCKS 64
#define MAXIMUM_FILES 10

typedef struct LinkedList {
    Block data;
    struct LinkedList *next;
} Node;

Node* addNode(Block b) {
    Node* temp;
    temp = (Node*)malloc(sizeof(struct LinkedList));
    temp->data = b; 
    temp->next = NULL;
    return temp;
}

Node* head = NULL;

typedef struct Block{
    unsigned char data[BLOCK_SIZE];
    int blockNumber;
};

typedef struct FIB{
    int ID;
    char name [100];
    int file_size;
    int block_count;
    Block * index_block;
};

typedef struct VCB{
    LinkedList free_blocks;
    Block logical_blocks [TOTAL_BLOCKS];
    int file_control_block_id;
    int file_control_block_status;
    FIB files_created [MAXIMUM_FILES];
    int num_files_created;
};

VCB volume_control_block;

void initFS();

void createFile(char* filename, int size);

void deleteFile(char* filename);

void listFiles();

Block allocateFreeBlock();

void returnFreeBlock();

void printFreeBlocks();