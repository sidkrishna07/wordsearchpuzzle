#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

void printPuzzle(char** arr, int size);
void searchPuzzle(char** arr, char* word, int size);
void toUpperWord(char* word);
int searchWord(char **arr, char *word, int x, int y, int dx, int dy, int size, int **path); 
void printPath(int **path, int bSize);

// Main function, DO NOT MODIFY
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j, bSize;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);

    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        block[i] = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize; j++) {
            fscanf(fptr, " %c", &block[i][j]);
        }
    }
    fclose(fptr);

    // Ask user for the word to search
    printf("Enter the word to search: ");
    scanf("%s", word);
    toUpperWord(word); // Convert the word to uppercase before searching

    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block, bSize);

    // Call searchPuzzle to find the word in the puzzle
    searchPuzzle(block, word, bSize);

    // Free memory
    for (i = 0; i < bSize; i++) {
        free(block[i]);
    }
    free(block);
    free(word);

    return 0;
}

// Function to print the puzzle
void printPuzzle(char** arr, int bSize) {
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%c ", arr[i][j]);
        }
        printf("\n");
    }
}

// Function to convert string to uppercase
void toUpperWord(char* word) {
    while (*word) {
        *word = toupper((unsigned char)*word);
        word++;
    }
}




void searchPuzzle(char **arr, char *word, int bSize) {
    int **path = (int **)malloc(bSize * sizeof(int *));
    for (int i = 0; i < bSize; i++) {
        path[i] = (int *)calloc(bSize, sizeof(int));
    }

    int found = 0;

    for (int i = 0; i < bSize && !found; i++) {
        for (int j = 0; j < bSize && !found; j++) {
            for (int dx = -1; dx <= 1 && !found; dx++) {
                for (int dy = -1; dy <= 1 && !found; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    if (searchWord(arr, word, i, j, dx, dy, bSize, path)) {
                        found = 1;
                        break;
                    }
                }
            }
        }
    }

    if(found) {
        printf("Word '%s' found!\n", word);
        printPath(path, bSize);
    } else {
        printf("Word '%s' not found.\n", word);
    }

    for (int i = 0; i < bSize; i++) {
        free(path[i]);
    }
    free(path);
}

int searchWord(char **arr, char *word, int x, int y, int dx, int dy, int size, int **path) {
    int len = strlen(word);
    for (int i = 0; i < len; i++) {
        int newX = x + i * dx;
        int newY = y + i * dy;
        if (newX < 0 || newY < 0 || newX >= size || newY >= size || toupper(arr[newX][newY]) != toupper(word[i])) {
            return 0;
        }
        path[newX][newY] = i + 1;
    }
    return 1;
}

void printPath(int **path, int bSize) {
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            if (path[i][j] != 0) {
                printf("%2d ", path[i][j]);
            } else {
                printf(" . ");
            }
        }
        printf("\n");
    }
}