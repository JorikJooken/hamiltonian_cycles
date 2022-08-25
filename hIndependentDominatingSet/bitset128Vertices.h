// FOR GRAPHS UP TO 128 VERTICES
#ifndef BITSET_MACROS
#define BITSET_MACROS
#define NOFBYTES 16

#include <stdint.h>

//  Bitset macros, we assume nodes are labeled 0,1,2,...
typedef uint64_t bitset __attribute__ ((vector_size (NOFBYTES)));

//  Returns an empty bitset. 
#define EMPTY (bitset) {0LL}

//  Returns a bitset containing only node.
#define singleton(node) (((node)/64) == 0 ? ((bitset) {1LL << (node)}) : \
											((bitset) {0LL,1LL << ((node)%64)}))

//  Returns the union of set1 and set2.
#define union(set1, set2) ((set1) | (set2))

//  Returns the intersection of set1 and set2.
#define intersection(set1, set2) ((set1) & (set2))

//  Adds node to set.
#define add(set, node) ((set) = union((set),singleton(node)))

//  Returns set1\set2 (set difference).
#define difference(set1, set2) ((set1) & ~(set2))

//  Removes node from set.
#define removeElement(set, node) ((set) = difference((set), singleton(node)))

//  Check if set is empty.
#define isEmpty(set) !((set)[0] | (set)[1]) 

//  Returns the size of the set.
#define size(set) (__builtin_popcountll((set)[0]) + \
				   __builtin_popcountll((set)[1]))


//	Loops over all elements of the set. 
#define forEach(element, set) for (int element = next((set), -1); (element) != -1; (element) = next((set), (element)))

//	Loops over all elements of the set starting from start (not included). 
#define forEachAfterIndex(element, set, start) for (int element = next((set), (start)); (element) != -1; (element) = next((set), (element)))

#define next(set, current)  (current < 63 ? safeNext0(set, current) : safeNext64(set, current))

#define safeNext0(set, current) (set[0] ? unsafeNext0(set, current) : safeNext64(set, 63))
#define safeNext64(set, current) (set[1] ? unsafeNext64(set, current) : -1)

#define unsafeNext0(set, current) (__builtin_clzll((set)[0]) + (current) >= 63 ? safeNext64(set, 63) : \
																				 (current) + __builtin_ctzll((set)[0] >> ((current) + 1)) + 1)
#define unsafeNext64(set, current) (__builtin_clzll((set)[1]) + ((current) - 64) >= 63 ? -1 : \
		 																				(current) + __builtin_ctzll((set)[1] >> ((current) - 64 + 1)) + 1)


//  Checks whether node is an element of set.
#define contains(set, node) (!isEmpty(intersection((set), singleton(node))))

//	Take the complement of a set in a universe consisting of sizeOfUniverse elements. 
//	E.g.: complement of {0,2} if there are only 4 elements is {1,3} and not 64-bit complement. 
#define complement(set, sizeOfUniverse) (sizeOfUniverse <= 64 ? (bitset) {~(set)[0] << (64-(sizeOfUniverse)) >> (64-(sizeOfUniverse)), (set)[1]} : \
																(bitset) {~(set)[0], ~(set)[1] << (64 - ((sizeOfUniverse) - 64)) >> (64-((sizeOfUniverse) - 64) )})

#endif