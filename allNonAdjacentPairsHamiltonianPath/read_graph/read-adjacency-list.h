#ifndef READ_ADJACENCY_LIST
#define READ_ADJACENCY_LIST

#include "../bitset.h"

/**
 *  Returns an integer representing the total number of vertices present in the graph corresponding to the given file.
 * 
 *  @param  filename    A string with a path pointing to the HoG adjacency list file (usually ending in .lst).
 * 
 *  @return Returns the number of vertices in the graph.
 * */
int countNumberOfVerticesInFile(const char * filename);

/**
 *  Loads the graphs into an array of bitsets. This method also relabels the nodes to let them start at 0, i.e. the node 
 *  labeled n in the HoG adjacency list file format is n-1 in this representation. From here on out we use our labeling.
 *  The bitset at index i represents the neighbours of node i. 
 * 
 *  @param  filename    A string with a path pointing to the HoG adjacency list file (usually ending in .lst). Correct behaviour
 *                      is not guaranteed unless MAXLINELENGTH is at least the length of the longest line present in the file.
 *  @param  adjacencyList   The (empty) bitset array in which the adjacency list will be loaded. This array needs to contain
 *                          numberOfVertices bitsets, each of which corresponds to a node in the graph.  
 *  @param  numberOfVertices    The number of vertices in the graph.
 * */
void loadAdjacencyList(const char * filename, bitset adjacencyList[], int numberOfVertices);

#endif