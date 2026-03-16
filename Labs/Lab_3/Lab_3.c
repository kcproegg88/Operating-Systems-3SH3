#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 10

int main(void){
    FILE *file = fopen("labaddr.txt", "r");
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file) != NULL){
        printf("%s", buffer);
    }
    printf("Hello, World!\n");
    fclose(file);
    return 0;
}