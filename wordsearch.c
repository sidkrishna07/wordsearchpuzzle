#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printPuzzle(char** arr, int size);
void searchPuzzle(char** arr, char* word, int size);
void printPath(int **path, int bSize);
void toUpperWord(char* word);
int searchWord(char **arr, int **path, int x, int y, char *word, int index, int size, int count);

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", *argv);
        return 2;
    }
    int i, j, bSize;
    FILE *fptr;

    fptr = fopen(*(argv + 1), "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    fscanf(fptr, "%d\n", &bSize);

    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize; j++) {
            fscanf(fptr, " %c", *(block + i) + j);
        }
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);
    toUpperWord(word);

    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block, bSize);

    searchPuzzle(block, word, bSize);

    for (i = 0; i < bSize; i++) {
        free(*(block + i));
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
    int pathFound = 0;

    int **path = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        *(path + i) = (int*)malloc(size * sizeof(int));
        memset(*(path + i), 0, size * sizeof(int));
    }

    printf("\n");

    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            if (searchWord(arr, path, x, y, word, 0, size, 1)) {
                pathFound = 1;
            }
        }
    }

    if(pathFound){
        printf("Word found!\n");
        printf("Printing the search path:\n");
        printPath(path, size);

    } else {
        printf("Word not found.\n");
    }

    for (int i = 0; i < size; i++) {
        free(*(path + i));
    }
    free(path);
}

int searchWord(char **arr, int **path, int x, int y, char *word, int index, int size, int count) {
    if (x < 0 || y < 0 || x >= size || y >= size) return 0;

    if (index == strlen(word)) return 1;

    char gridChar = *(*(arr + x) + y);
    char wordChar = *(word + index);
    if (gridChar >= 'a' && gridChar <= 'z') gridChar -= 32;
    if (wordChar >= 'a' && wordChar <= 'z') wordChar -= 32;
    if (gridChar != wordChar) return 0;

    *(*(path + x) + y) = (*(*(path + x) + y) * 10) + count;

    int (*directions)[2] = (int (*)[2])malloc(8 * sizeof(int[2]));
    *(*(directions + 0) + 0) = -1; *(*(directions + 0) + 1) = 0;
    *(*(directions + 1) + 0) = 1; *(*(directions + 1) + 1) = 0;
    *(*(directions + 2) + 0) = 0; *(*(directions + 2) + 1) = -1;
    *(*(directions + 3) + 0) = 0; *(*(directions + 3) + 1) = 1;
    *(*(directions + 4) + 0) = -1; *(*(directions + 4) + 1) = -1;
    *(*(directions + 5) + 0) = -1; *(*(directions + 5) + 1) = 1;
    *(*(directions + 6) + 0) = 1; *(*(directions + 6) + 1) = -1;
    *(*(directions + 7) + 0) = 1; *(*(directions + 7) + 1) = 1;

    for (int d = 0; d < 8; d++) {
        if (searchWord(arr, path, x + *(*(directions + d) + 0), y + *(*(directions + d) + 1), word, index + 1, size, count + 1)) {
            return 1;
        }
    }

    *(*(path + x) + y) = 0;
    return 0;
}

void printPath(int **path, int bSize) {
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%-5d", *(*(path + i) + j));
        }
        printf("\n");
    }
}