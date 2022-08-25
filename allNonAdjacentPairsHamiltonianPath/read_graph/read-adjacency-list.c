
#include "read-adjacency-list.h"
#include "../bitset.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//  Needs to be at least the length of the longest line in the given file.
#define MAXLINELENGTH 1000

int countNumberOfVerticesInFile(const char * filename) {
    char line[MAXLINELENGTH];
    int lastVertex = 0;
    FILE *fp;
    fp = fopen( filename, "r");
    for (int j = 0;fgets(line, MAXLINELENGTH, fp) != NULL; j++){ //Loop ends if there are no more lines.
        char* token;
        token = strtok(line, ":");
        sscanf(token, "%d", &lastVertex); //Save the first integer found on the line.
        
    }
    fclose(fp);
    return lastVertex;
}

void loadAdjacencyList(const char * filename, bitset adjacencyList[], int numberOfVertices) {
    if (numberOfVertices > 64) {
        printf("This program can only handly graphs with 64 vertices or less.\n\n");
        abort();
    }
    FILE *fp;

    // Open the file corresponding to filename.
    fp = fopen( filename, "r");
    char line[MAXLINELENGTH];
    int value;

    // At the start of each loop line is the j+1th line.
    for (int j = 0;fgets(line, MAXLINELENGTH, fp) != NULL; j++){ 
        char* token;
        token = strtok(line, " ");
        adjacencyList[j] = EMPTY;
        for (int i = 0; (token = strtok(NULL," ")) != NULL; i++){
            if(MAXLINELENGTH - ((intptr_t) token - (intptr_t) & line) <= 4) {
                printf("Adjust the macro MAXLINELENGTH in read-adjacency-list.c and recompile.\n\n");
                abort();
            }
            sscanf(token, "%d", &value);
            add(adjacencyList[j], value - 1); // Node value is relabeled to value - 1.
        }
    }
    fclose(fp);
}
