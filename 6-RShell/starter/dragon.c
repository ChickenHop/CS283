#include <stdio.h>
#include <stdlib.h>
// EXTRA CREDIT - print the drexel dragon from the readme.md
extern void print_dragon(){
  FILE *file = fopen("./dragon.txt", "r");
    if (file == NULL) {
        printf("Error opening file");
        exit(-1);
    }

    char buffer[1024];   
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }
    printf("\n");
    fclose(file);
}
