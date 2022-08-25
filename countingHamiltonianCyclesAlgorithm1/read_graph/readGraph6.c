#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readGraph6.h"
#include "../bitset.h"

// Unsafe because no defined behaviour if character = 0. ctz and clz work with 32 bit numbers.
#define unsafePrev(character, current) (__builtin_ctz(character) - current >= 0 ? -1 : current -__builtin_clz((character) << (32 - current)) - 1)

#define prev(character,current) (character ? unsafePrev(character,current) : -1)

int getNumberOfVertices(const char * graphString) {
	if(strlen(graphString) == 0){
        printf("Error: String is empty.\n");
        abort();
    }
    else if((graphString[0] < 63 || graphString[0] > 126) && graphString[0] != '>') {
    	printf("Error: Invalid start of graphstring.\n");
    	abort();
    }

	int index = 0;
	if (graphString[index] == '>') { // Skip >>graph6<< header.
		index += 10;
	}

	if(graphString[index] < 126) { // 0 <= n <= 62
		return (int) graphString[index] - 63;
	}

	else if(graphString[++index] < 126) { 
		int number = 0;
		for(int i = 2; i >= 0; i--) {
			number |= (graphString[index++] - 63) << i*6;
		}
		return number;
	}

	else if (graphString[++index] < 126) {
		int number = 0;
		for (int i = 5; i >= 0; i--) {
			number |= (graphString[index++] - 63) << i*6;
		}
		return number;
	}

	else {
		printf("Error: Format only works for graphs up to 68719476735 vertices.\n");
		abort();
	}
}

void loadGraph(const char * graphString, int numberOfVertices, bitset adjacencyList[]) { //Support only up to 128 vertices. (Select right option in bitset.h! Lower is faster.)
	int startIndex = 0;
	if (graphString[startIndex] == '>') { // Skip >>graph6<< header.
		startIndex += 10;
	}
	if (numberOfVertices <= 62) {
		startIndex += 1;
	}
	else if (numberOfVertices <= MAXVERTICES) {
		startIndex += 4;
	}
	else {
		printf("Error: Program can only handle graphs with %d vertices or fewer.\n",MAXVERTICES);
		abort();
	}

	for (int vertex = 0; vertex < numberOfVertices; vertex++) { //Initialize adjacencyList.
		adjacencyList[vertex] = EMPTY;
	}

	int currentVertex = 1;
	int sum = 0; 
	for (int index = startIndex; graphString[index] != '\n'; index++) {
		int i;
		for (i = prev(graphString[index] - 63, 6); i != -1; i = prev(graphString[index] - 63, i)) {
			while(5-i+(index-startIndex)*6 - sum >= 0) {
				sum += currentVertex;
				currentVertex++;
			}
			sum -= --currentVertex;
			int neighbour = 5-i+(index - startIndex)*6 - sum;
			add(adjacencyList[currentVertex], neighbour);
			add(adjacencyList[neighbour], currentVertex);
		}
	}

}

// int main() {
// 	char *graphString = NULL;
// 	size_t size;
// 	if (getline(&graphString, &size, stdin) == -1) {
// 		printf("Something went wrong reading stdin.\n");
// 	}
// 	int nVertices = getNumberOfVertices(graphString);
// 	printf("Number of vertices: %d\n", nVertices);
// 	bitset adjacencyList[nVertices];
// 	loadGraph(graphString,nVertices,adjacencyList);
// 	for(int i = 0; i < nVertices; i++) {
// 		printf("Neighbours of %d: %lu\n", i,adjacencyList[i]);
// 	}

// 	return 0;
// }