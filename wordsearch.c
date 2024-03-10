#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int bSize;//size of the puzzle block
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
void printPath(int **path, int bSize);
void toUpperWord(char* word);
void toUpperPuzzle(char** arr);
int** copyPath(int** path, int size);
void addPath(int*** allPaths, int** pathCopy, int* allPathsCount, int* allPathsSize);
int searchWord(char **arr, int **path, int x, int y, char *word, int index, int size, int count);
int*** allPaths;
int allPathsCount = 0;
// Main function, DO NOT MODIFY
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
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
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);
    void toUpperPuzzle(char** arr);
    int** copyPath(int** path, int size);
    void addPath(int*** allPaths, int** pathCopy, int* allPathsCount, int* allPathsSize);
    printf("Enter the word to search: ");
    scanf("%s", word);
    void toUpperWord(char* word);
// Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
// Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    return 0;
}

void printPuzzle(char** arr){
     int size = bSize; // Use the global variable bSize as the size

    // Print the puzzle
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%c ", *(*(arr + i) + j));
        }
        printf("\n");
    }
}

    // Print the puzzle
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%c ", *(*(arr + i) + j));
        }
        printf("\n");
    }

void toUpperWord(char* word) {
    while (*word) {
        if (*word >= 'a' && *word <= 'z') *word -= 32;
        word++;
    }
}
void toUpperPuzzle(char** arr) {
    int size = bSize; // Use the global variable bSize as the size

    // Convert puzzle grid to uppercase
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (*(*(arr + i) + j) >= 'a' && *(*(arr + i) + j) <= 'z') *(*(arr + i) + j) -= 32;
        }
    }
}
    
    // Convert puzzle grid to uppercase
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (*(*(arr + i) + j) >= 'a' && *(*(arr + i) + j) <= 'z') *(*(arr + i) + j) -= 32;
    }
}


void searchPuzzle(char **arr, char* word) {
    toUpperWord(word);
    int size = bSize; // Use the global variable bSize as the size
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

int** copyPath(int** path, int size) {
    // Allocate memory for the new 2D array
    int** pathCopy = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        *(pathCopy + i) = (int*)malloc(size * sizeof(int));
        // Copy the contents of the path into the new array
        for (int j = 0; j < size; j++) {
            *(*(pathCopy + i) + j) = *(*(path + i) + j);
        }
    }
    return pathCopy;
}

void addPath(int*** allPaths, int** pathCopy, int* allPathsCount, int* allPathsSize) {
    // If allPaths is full, reallocate more memory
    if (*allPathsCount == *allPathsSize) {
        *allPathsSize *= 2;
        *allPaths = (int**)realloc(*allPaths, *allPathsSize * sizeof(int**));
    }

    // Add pathCopy to allPaths and increment allPathsCount
    (*allPaths)[(*allPathsCount)++] = *pathCopy;
}

int searchWord(char **arr, int **path, int x, int y, char *word, int index, int size, int count) {
    // If we're out of bounds or the current cell is already part of the path
    if (x < 0 || y < 0 || x >= bSize || y >= bSize || *(*(path + x) + y) != 0) return 0;

    // If the current character doesn't match
    char gridChar = *(*(arr + x) + y);
    char wordChar = *(word + index);
    if (gridChar != wordChar) return 0;

    // Mark the current cell in the path
    *(*(path + x) + y) = count;

    // If we've reached the end of the word, store the path and return 1
    if (word[index + 1] == '\0') {
        // Create a copy of the path and add it to allPaths
        int **pathCopy = copyPath(path, size);
        *allPaths[allPathsCount++] = *pathCopy;
        return 1;
    }

    // Define all 8 possible directions to search: up, down, left, right, and diagonals
    int directions[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    // Try to find the rest of the word in all directions from (x, y)
    for (int d = 0; d < 8; d++) {
        searchWord(arr, path, x + directions[d][0], y + directions[d][1], word, index + 1, size, count + 1);
    }

    // If we didn't find the rest of the word, remove the current cell from the path
    *(*(path + x) + y) = 0;
    
    return 0;
}

void printPath(int **path, int bSize) {
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%5d", *(*(path + i) + j)); // Changed to print five spaces
        }
        printf("\n");
    }
}