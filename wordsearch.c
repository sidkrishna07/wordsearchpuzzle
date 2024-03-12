#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printPuzzle(char** arr, int size);
void searchPuzzle(char** arr, char* word, int size);
void printPath(int **path, int bSize);
void toUpperWord(char* word);
int searchWord(char **arr, int **path, int x, int y, char *word, int index, int size, int count);

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

void printPuzzle(char** arr, int bSize) {
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%c ", *(*(arr + i) + j));
        }
        printf("\n");
    }
}

void toUpperWord(char* word) {
    while (*word) {
        if (*word >= 'a' && *word <= 'z') *word -= 32;
        word++;
    }
}

void searchPuzzle(char **arr, char* word, int size) {
    int found = 0;

    // Create a 2D array to store the path
    int **path = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        path[i] = (int*)malloc(size * sizeof(int));
        memset(path[i], 0, size * sizeof(int));
    }

    printf("\n");

    // Iterate through each cell in the grid as a starting point
    for (int x = 0; x < size && !found; x++) {
        for (int y = 0; y < size && !found; y++) {
            // Try to find the word starting from (x, y)
            if (searchWord(arr, path, x, y, word, 0, size, 1)) {
                found = 1;
                printf("Word found!\n"); // Changed wording
                printf("Printing the search path:\n"); // Added line
                printPath(path, size);
                break; // Found the word, no need to search further
            }
        }
    }

    if (!found) {
        printf("Word not found.\n"); // Changed wording
    }

    // Free the path array
    for (int i = 0; i < size; i++) {
        free(path[i]);
    }
    free(path);
}

int searchWord(char **arr, int **path, int x, int y, char *word, int index, int size, int count) {
    // If we're out of bounds
    if (x < 0 || y < 0 || x >= size || y >= size) return 0;

    // If we've found the whole word
    if (index == strlen(word)) return 1;

    // If the current character doesn't match
    char gridChar = *(*(arr + x) + y);
    char wordChar = *(word + index);
    if (gridChar >= 'a' && gridChar <= 'z') gridChar -= 32;
    if (wordChar >= 'a' && wordChar <= 'z') wordChar -= 32;
    if (gridChar != wordChar) return 0;


    *(*(path + x) + y) = (*(*(path + x) + y) * 10) + count;

    // Define all 8 possible directions to search: up, down, left, right, and diagonals
    int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    // Try to find the rest of the word in all directions from (x, y)
    for (int d = 0; d < 8; d++) {
        if (searchWord(arr, path, x + directions[d][0], y + directions[d][1], word, index + 1, size, count + 1)) {
            // If we found the rest of the word, return 1
            return 1;
        }
    }

    // If we didn't find the rest of the word, unmark the current cell and remove it from the path
    *(*(path + x) + y) = 0;
    return 0;
}

void printPath(int **path, int bSize) {
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%d     ", *(*(path + i) + j)); // Changed to print five spaces
        }
        printf("\n");
    }
}