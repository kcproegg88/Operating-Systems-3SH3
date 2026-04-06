#include "fs_indexed.h"

VCB volume_control_block;

Node* addNode(int blockNumber){
    Node* temp = (Node*)malloc(sizeof(Node));
    if (temp == NULL){
        return NULL;
    }

    temp->blockNumber = blockNumber;
    temp->next = NULL;
    return temp;
}

void initFS(){
    int i;

    // Initialize counters
    volume_control_block.free_head = NULL;
    volume_control_block.free_tail = NULL;
    volume_control_block.free_block_count = TOTAL_BLOCKS;
    volume_control_block.num_files_created = 0;

    // Initialize blocks + build free list
    for (i = 0; i < TOTAL_BLOCKS; i++){
        volume_control_block.logical_blocks[i].blockNumber = i;

        Node* newNode = addNode(i);

        if (volume_control_block.free_head == NULL){
            volume_control_block.free_head = newNode;
            volume_control_block.free_tail = newNode;
        }else{
            volume_control_block.free_tail->next = newNode;
            volume_control_block.free_tail = newNode;
        }
    }

    // Initialize FIB table
    for (i = 0; i < MAXIMUM_FILES; i++){
        volume_control_block.files_created[i].used = 0;
        volume_control_block.files_created[i].ID = i;
    }

    printf("Filesystem initialized with %d blocks of %d bytes each.\n", TOTAL_BLOCKS, BLOCK_SIZE);
}

Block allocateFreeBlock(){
    Block invalid;
    invalid.blockNumber = -1;

    if (volume_control_block.free_head == NULL)
        return invalid;

    Node* temp = volume_control_block.free_head;
    int blockNum = temp->blockNumber;

    volume_control_block.free_head = temp->next;

    if (volume_control_block.free_head == NULL)
        volume_control_block.free_tail = NULL;

    free(temp);

    volume_control_block.free_block_count--;

    return volume_control_block.logical_blocks[blockNum];
}

void returnFreeBlock(int blockNumber){
    Node* newNode = addNode(blockNumber);

    if (volume_control_block.free_tail == NULL){
        volume_control_block.free_head = newNode;
        volume_control_block.free_tail = newNode;
    }else{
        volume_control_block.free_tail->next = newNode;
        volume_control_block.free_tail = newNode;
    }

    volume_control_block.free_block_count++;
}

void printFreeBlocks(){
    printf("Free Blocks (%d): ", volume_control_block.free_block_count);

    Node* current = volume_control_block.free_head;

    while (current != NULL){
        printf("[%d] -> ", current->blockNumber);
        current = current->next;
    }

    printf("NULL\n");
}

int getFileInformationBlockID(){
    int i;

    for (i = 0; i < MAXIMUM_FILES; i++){
        if (volume_control_block.files_created[i].used == 0){
            volume_control_block.files_created[i].used = 1;
            return i;
        }
    }

    return -1;
}

void createFile(char* filename, int size){
    int fibID;
    int dataBlocksNeeded;
    int totalBlocksNeeded;
    int i;

    if (volume_control_block.num_files_created >= MAXIMUM_FILES){
        printf("Error: Maximum file limit reached\n");
        return;
    }

    for (i = 0; i < MAXIMUM_FILES; i++)
    {
        if (volume_control_block.files_created[i].used && strcmp(volume_control_block.files_created[i].name, filename) == 0)
        {
            printf("Error: File already exists\n");
            return;
        }
    }

    dataBlocksNeeded = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    totalBlocksNeeded = dataBlocksNeeded + 1;

    if (totalBlocksNeeded > volume_control_block.free_block_count)
    {
        printf("Error: Not enough free blocks\n");
        return;
    }

    fibID = getFileInformationBlockID();
    if (fibID == -1)
    {
        printf("Error: No available FIB entry\n");
        return;
    }

    Block indexBlock = allocateFreeBlock();

    if (indexBlock.blockNumber == -1)
    {
        printf("Error: Failed to allocate index block\n");
        volume_control_block.files_created[fibID].used = 0;
        return;
    }

    int* blockPointers = (int*) volume_control_block.logical_blocks[indexBlock.blockNumber].data;

    for (i = 0; i < dataBlocksNeeded; i++){
        Block b = allocateFreeBlock();

        if (b.blockNumber == -1){
            printf("Error: Allocation failed\n");

            // rollback already allocated blocks
            int j;
            for (j = 0; j < i; j++){
                returnFreeBlock(blockPointers[j]);
            }

            returnFreeBlock(indexBlock.blockNumber);
            volume_control_block.files_created[fibID].used = 0;
            return;
        }

        blockPointers[i] = b.blockNumber;
    }

    FIB* file = &volume_control_block.files_created[fibID];

    strcpy(file->name, filename);
    file->file_size = size;
    file->block_count = dataBlocksNeeded;
    file->index_block = indexBlock.blockNumber;
    file->used = 1;

    volume_control_block.num_files_created++;

    printf("File '%s' created with %d data blocks + 1 index block.\n", filename, dataBlocksNeeded);
}

void deleteFile(char* filename){
    int fileIndex;
    int i;

    fileIndex = -1;

    for (i = 0; i < MAXIMUM_FILES; i++){
        if (volume_control_block.files_created[i].used && strcmp(volume_control_block.files_created[i].name, filename) == 0){
            fileIndex = i;
            break;
        }
    }

    if (fileIndex == -1){
        printf("Error: File not found\n");
        return;
    }

    FIB* file = &volume_control_block.files_created[fileIndex];

    int indexBlock = file->index_block;

    int* blockPointers = (int*) volume_control_block.logical_blocks[indexBlock].data;

    for (i = 0; i < file->block_count; i++){
        returnFreeBlock(blockPointers[i]);
    }

    returnFreeBlock(indexBlock);

    file->used = 0;
    volume_control_block.num_files_created--;

    printf("File '%s' deleted.\n", filename);
}

void listFiles(void){
    int i;

    printf("Root Directory Listing (%d files):\n", volume_control_block.num_files_created);

    for (i = 0; i < MAXIMUM_FILES; i++){
        if (volume_control_block.files_created[i].used){
            FIB* file = &volume_control_block.files_created[i];

            int display_size = file->block_count * BLOCK_SIZE;

            printf("  %-10s | %6d bytes | %2d data blocks | FIBID=%d\n",
                file->name,
                display_size,
                file->block_count,
                file->ID);
        }
    }
}