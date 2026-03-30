/** Assignment 3 3SH3
 * Krishna Chauhan and Fiona Douglas
 * March 21, 2026
 * Compile: gcc assignment3.c -o assignment3
 * Run: ./assignment3
*/


#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MEMORY_SIZE 65536 // size of logical address space
#define PHYSICAL_MEMORY_SIZE 65536/2

#define BUFFER_SIZE 10 // size of address read buffer

#define OFFSET_MASK 255
#define PAGES 256 // number of pages = memory size/page size
#define OFFSET_BITS 8
#define PAGE_SIZE 256 // size of each page
#define TLB_LENGTH 16

#define PHYSICAL_MEMORY_PAGES PAGES/2

struct TLBentry{
    // Entry in the TLB, stores page number in page table and corresponding frame in physical memory
    int page_number;
    int frame_number;

};

// Circular arrays
struct TLBentry TLB[TLB_LENGTH]; // Stores 16 most recetly accessed pages to save time searching for them
signed char phys_memory[PHYSICAL_MEMORY_SIZE]; // The physical memory of the system, stores 128 out of 256 possible pages

int TLB_write = 0; // Write positions for cicrular arrays
int phys_write = 0;

int page_table[PAGES]; // Table of all pages in virtual memory, index is page number and value is frame of physical memory
// If not in physical memory, value is -1

int search_TLB(int page){
    // Takes in a page number and searches the TLB to return the index if it is in the TLB
    // If not, returns -1
    int i = 0;

    for (i = 0; i<TLB_LENGTH; i++){
        if (TLB[i].page_number == page){
            return i;
        }
    }
    return -1;
}



void TLB_add(struct TLBentry entry){
    // Takes in a TLB entry and adds it to the TLB
    TLB[TLB_write] = entry;
    TLB_write++;
    TLB_write %= TLB_LENGTH;
}

void TLB_update(int page, int frame, int index){
    // Updates the TLB for when an entry is replaced by a new one 
    TLB[index].page_number = page;
    TLB[index].frame_number = frame;
}

void phys_memory_add(int page_num, void* virtual_ptr){
    // Takes in a page number and a pointer to its spot in virtual memory, adds that page to physical memory
    int i = 0;

    int notInTLB = 1; // 1 if page not in TLB
    for (i = 0; i<TLB_LENGTH; i++){
        if (TLB[i].frame_number == phys_write){
            TLB_update(page_num, phys_write, i); // If page is in the TLB, update it with new frame location
            notInTLB = 0;
        }
    }
    if (notInTLB){
        // Add page and frame data to TLB
        struct TLBentry temp;
        temp.page_number = page_num;
        temp.frame_number = phys_write;
        TLB_add(temp);
    }

    // Copy 1 page worth of data from the pointer and put it in physical memory
    memcpy(&phys_memory[phys_write*PAGE_SIZE], virtual_ptr, PAGE_SIZE);
    phys_write++;
    phys_write %= PHYSICAL_MEMORY_PAGES;
}





void initialize_TLB(){
    // Make all unushed TLB slots -1,-1
    int i;
    for (i = 0; i<TLB_LENGTH; i++){
        struct TLBentry t;
        t.frame_number = -1;
        t.page_number = -1;
        TLB_add(t);
    }
}

void initialize_page_table(){
    // Set all elements in page table to -1 to show they aren't in physical memory
    memset(page_table, -1, PAGES*sizeof(int));
}

void read_addresses(void){
    int addresses = 0; // Number of addresses in the text file
    int TLB_hits = 0; // Number of times the requested page was in the TLB
    int page_faults = 0; // Number of times requested page wasn't in physical memory

    signed char *mmapfptr;

    // Read logical addresses from text file
    FILE *file = fopen("addresses.txt", "r");
    int mmapfile_fd = open("BACKING_STORE.bin", O_RDONLY);
    char buffer[BUFFER_SIZE];
    mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);

    while (fgets(buffer, BUFFER_SIZE, file) != NULL){
        // For each address line in file:

        // printf("%s", buffer);
        int addr = atoi(buffer);
        int page = addr >> OFFSET_BITS;
        int offset = addr & OFFSET_MASK;
        // Calculate page and offset


        int frame = 0;

        //printf("Page: %d\n", page);

        // Check if page is in TLB
        int TLBpos = search_TLB(page);
        //printf("TLBpos: %d\n", TLBpos);

         if(TLBpos != -1){
            // If yes, get frame number from TLB
            frame = TLB[TLBpos].frame_number;
            TLB_hits++;
        }
        // If not, check page table to see if page is in physical memory
        else if (page_table[page] != -1){
            // In physical memory
            struct TLBentry temp;
            frame = page_table[page];
            temp.page_number = page;
            temp.frame_number = frame;
            TLB_add(temp); // Add page to TLB
        }            
        else{
            // Not in physical memory, therefore page fault
            page_faults++;
            int i;
            for (i = 0; i<PAGES; i++){
                // Go through page table and see if any pages point to frame about to be overwritten
                
                if (page_table[i] == phys_write){
                    // If so, update them to say not in physical memory
                    page_table[i] = -1;
                    break;
                }
            }
            //printf("Page fault\n");
            frame = phys_write; 
            page_table[page] = phys_write; // Add frame in physcial memory to page in page table
            phys_memory_add(page, mmapfptr + page*PAGE_SIZE); // Add page to physical memory
            
        }
        addresses++;
        int physical_addr = (page_table[page] << OFFSET_BITS) | offset;
        printf("Virutal Address:%d Physical addr = %d ", addr, physical_addr);
        printf("Value=%d\n", *(phys_memory + frame * PAGE_SIZE + offset));

    }
    printf("Total addresses = %d\n", addresses);
    printf("Page faults = %d\n", page_faults);
    printf("TLB hits = %d\n", TLB_hits);

    fclose(file);
    close(mmapfile_fd);
}

void read_virtual_memory(){
    // read the binary file and put its contents into a virtual memory buffer
    signed char *mmapfptr;
    int virtual_memory[MEMORY_SIZE];
    
    int mmapfile_fd = open("BACKING_STORE.bin", O_RDONLY);
    mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
    int i;
    for (i = 0; i < PAGES; i++){
        memcpy(&virtual_memory[i], mmapfptr + i * PAGE_SIZE, PAGE_SIZE);
    }
}



int main(void){

    initialize_TLB();
    initialize_page_table();

    read_virtual_memory();
    read_addresses();
    
    return 0;
}