/**
 *  This header file contains functions that have to do with checking
 *  hamiltonicity properties of graphs (K1-Hamiltonicity, K2-Hamiltonicity).
 * */

#ifndef HAM_METHODS 
#define HAM_METHODS

#include "bitset.h"

/**
 *  Returns a boolean indicating whether or not the specified path can be
 *  extended to a Hamiltonian cycle in the specified graph. The path is
 *  represented by a bitset containing its elements, two ints representing
 *  the last and first element and an int representing the current length.
 *  This is enough to check whether the given path can become a Hamiltonian
 *  cycle, but does not allow for the retrieval of said path.
 *  
 *  @param  adjacencyList   An array of bitsets representing the adjacency
 *   list of the graph. We check whether the path can be extended to a
 *   Hamiltonian cycle in this graph.
 *  @param  path    A bitset containing all nodes that are present in the
 *   path. It will be checked for this path whether it can be extended to a
 *   Hamiltonian cycle. For correctness of the method, it is required that
 *   the path exists in the given graph.
 *  @param  lastElemOfPath  An int representing the last vertex in the path.
 *   This is necessary, since the bitset path does not give any information
 *   on the order of the path.
 *  @param  firstElemOfPath An int representing the first vertex in the path.
 *   This is necessary since the bitset path does not give any information on
 *   the order of the path.
 *  @param  numberOfVertices    The number of vertices in the graph.
 *  @param  pathLength  The length of the current path.
 * 
 *  @return Boolean representing whether the given path can be extended to a
 *   Hamiltonian cycle.
 * */
bool canBeHamiltonian(bitset adjacencyList[], bitset remainingVertices, int
lastElemOfPath, int firstElemOfPath, int numberOfVertices, int pathLength);

bool canBeHamiltonianPrintCycle(bitset adjacencyList[], bitset
remainingVertices, int pathList[], int lastElemOfPath, int firstElemOfPath,
int numberOfVertices, int pathLength, int* numberOfHamiltonianCycles, bool
allCyclesFlag, int upperBound);

/**
 *  Returns a boolean indicating whether the given graph is Hamiltonian or
 *  not. It does so by checking whether all paths of the form a, 0, b, where
 *  a, b are neighbours of zero and a > b, can be extended to some
 *  Hamiltonian cycle.
 * 
 *  @param  adjacencyList   An array of bitsets representing the adjacency
 *   list of the given graph.
 *  @param  numberOfVertices    The number of vertices in the graph.
 *  @param  allCyclesFlag   Boolean indication whether the all-cycles option
 *   was passed.
 * 
 *  @return Returns true when the graph is Hamiltonian and false otherwise.
 * */
bool isHamiltonian(bitset adjacencyList[], int numberOfVertices, bool
allCyclesFlag, int upperBound);

bool isHamiltonianWithEdgeCounts(bitset adjacencyList[], int numberOfVertices, bool
allCyclesFlag, int upperBound, int* edgeCounts);

bool exclusionIsHamiltonian(bitset adjacencyList[], int totalNumberOfVertices,
int newNumberOfVertices, bitset excludedVertices, bool printPathFlag);


/**
 *  Returns a boolean indicating whether the graph is K1-hamiltonian, i.e.
 *  deleting any copy of K1 (a single vertex), yields a Hamiltonian graph for
 *  every vertex. It does so by, for every vertex of the graph, calling
 *  exclusionIsHamiltonian, where the bitset of excludedVertices consists of
 *  that vertex.
 * 
 *  @param  adjacencyList   Array of bitsets representing the adjacency list
 *   of the original graph.
 *  @param  numberOfVertices The number of vertices in the original graph.
 * 
 *  @return True if the graph is K1-hamiltonian.
 * */
bool isK1Hamiltonian(bitset adjacencyList[], int numberOfVertices, bool verboseFlag);

/**
 *  Returns a boolean indicating whether the graph is K2-hamiltonian, i.e.
 *  deleting any copy of K2 (a pair of adjacent vertices), yields a
 *  Hamiltonian graph for all such copies. It does so by for such copy in the
 *  graph calling exclusionIsHamiltonian, where the bitset of
 *  excludedVertices consists of those two adjacent vertices corresponding to
 *  the copy.
 * 
 *  @param  adjacencyList   Array of bitsets representing the adjacency list
 *   of the original graph.
 *  @param  numberOfVertices The number of vertices in the original graph.
 * 
 *  @return True if the graph is K2-hamiltonian.
 * */
bool isK2Hamiltonian(bitset adjacencyList[], int numberOfVertices, bool
verboseFlag);

#endif
