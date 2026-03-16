#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 10

#define OFFSET_MASK 4095
#define PAGES 8
#define OFFSET_BITS 12
#define PAGE_SIZE 4096

int page_table[PAGES] = {6, 4, 3, 7, 0, 1, 2, 5};

void Lab_3a(void){
    FILE *file = fopen("labaddr.txt", "r");
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file) != NULL){
        // printf("%s", buffer);
        int addr = atoi(buffer);
        int page = addr >> OFFSET_BITS;
        int offset = addr & OFFSET_MASK;
        int physical_addr = (page_table[page] << OFFSET_BITS) | offset;
        printf("Virtual addr is %d: Page# = %d & Offset = %d. Physical addr = %d\n", addr, page, offset, physical_addr);
    }
    fclose(file);
}

int main(void){
    Lab_3a();
    return 0;
}