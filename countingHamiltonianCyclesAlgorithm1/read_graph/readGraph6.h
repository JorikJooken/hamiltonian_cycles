#ifndef READ_GRAPH6
#define READ_GRAPH6

#include "../bitset.h"

void loadGraph(const char * graphString, int numberOfVertices, bitset adjacencyList[]);
int getNumberOfVertices(const char * graphString);

#endif