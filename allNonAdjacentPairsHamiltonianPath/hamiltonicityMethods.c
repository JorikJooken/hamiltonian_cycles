#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <inttypes.h>
#include <stdlib.h>
#include "bitset.h"
#include "hamiltonicityMethods.h"

// bool reject(bitset adjacencyList[], bitset path, int lastElemOfPath, int firstElemOfPath, int numberOfVertices, int pathLength) {
//     bool shouldReject = isEmpty(difference(adjacencyList[firstElemOfPath], path));
//     // Check for all elements not yet visited whether they still have two neighbours to which they can connect.
//     bitset notInPath = (~path) << (64 - numberOfVertices) >> (64 - numberOfVertices);
//     for(int element = next(notInPath, -1); element != -1; element = next(notInPath,element)) {
//         bitset remainingOptionsForElem = difference(adjacencyList[element], path & (~singleton(firstElemOfPath)) & (~singleton(lastElemOfPath)));
//         shouldReject |= ((remainingOptionsForElem & (remainingOptionsForElem - 1)) == 0LL);
//     }
//     return shouldReject;
// }

// bool accept(bitset adjacencyList[], int lastElemOfPath, int firstElemOfPath, int numberOfVertices, int pathLength) {

//     // Check whether we have a Hamiltonian path already and whether this path is a cycle.
//     return ((pathLength == numberOfVertices) && contains(adjacencyList[firstElemOfPath], lastElemOfPath));
// }

// bool canBeHamiltonian2(bitset adjacencyList[], bitset path, int lastElemOfPath, int firstElemOfPath, int numberOfVertices, int pathLength) {
//     int prevVertex[64];
//     int lastCheckedNeighbour[64];
//     bitset neighboursNotInPathList[64];
//     prevVertex[lastElemOfPath] = firstElemOfPath;
//     lastCheckedNeighbour[firstElemOfPath] = 64;
//     neighboursNotInPathList[lastElemOfPath] = difference(adjacencyList[lastElemOfPath], path);
//     lastCheckedNeighbour[lastElemOfPath] = -1;
//     bool justBacktracked = false;

//     while(lastElemOfPath != firstElemOfPath) {


//         if(accept(adjacencyList, lastElemOfPath, firstElemOfPath, numberOfVertices, pathLength)) {
//             return true;
//         }

//         int nextVertex = next(neighboursNotInPathList[lastElemOfPath], lastCheckedNeighbour[lastElemOfPath]);

//         if((reject(adjacencyList, path, lastElemOfPath, firstElemOfPath, numberOfVertices, pathLength) && !justBacktracked )|| (nextVertex == -1)){
//             removeElement(path, lastElemOfPath);
//             pathLength--;
//             lastElemOfPath = prevVertex[lastElemOfPath];
//             justBacktracked = true;
//             continue;
//         }

//         prevVertex[nextVertex] = lastElemOfPath;
//         lastCheckedNeighbour[lastElemOfPath] = nextVertex;
//         lastElemOfPath = nextVertex;
//         add(path, lastElemOfPath);
//         pathLength++;
//         lastCheckedNeighbour[lastElemOfPath] = -1;
//         neighboursNotInPathList[lastElemOfPath] = difference(adjacencyList[lastElemOfPath], path);
//         justBacktracked = false;
//     }
//     return false;

// }

bool canBeHamiltonian(bitset adjacencyList[], bitset remainingVertices, int lastElemOfPath, int firstElemOfPath, int numberOfVertices, int pathLength) {
    // Check whether we have a Hamiltonian path already and whether this path is a cycle.
    if((pathLength == numberOfVertices) && contains(adjacencyList[firstElemOfPath], lastElemOfPath)) {
        return true;
    }

    // Check if cycle can still be closed with remaining vertices.
    if(isEmpty(intersection(adjacencyList[firstElemOfPath],remainingVertices))) return false;

    // Check for all elements not yet visited whether they still have two neighbours to which they can connect.
    bitset remainingWithFirstAndLast = union(remainingVertices, union(singleton(firstElemOfPath), singleton(lastElemOfPath)));
    forEach(vertex, remainingVertices) {

        //  Neighbours which either do not lie in the path, or which are one of its endpoints.
        bitset remainingNeighbours = intersection(adjacencyList[vertex], remainingWithFirstAndLast );

        //  If there is only one such neighbour or less, our path cannot be extended through this vertex into a Hamiltonian cycle.
        if(size(remainingNeighbours) < 2) return false;
    }


    // Create a bitset of the neighbours of the last element in the path which do not belong to the path. 
    // The path will be extended via these neighbours.
    bitset neighboursOfLastNotInPath = intersection(adjacencyList[lastElemOfPath], remainingVertices);
    forEach(neighbour, neighboursOfLastNotInPath) {

        //  Save the current last element of path. If an extension cannot be a Hamiltonian cycle, we need to try the
        //  other possible extensions for this path.
        int oldElemOfPath = lastElemOfPath;

        //  Extend the path with neighbour, which is a neighbour oldElemOfPath that does no belong to the path yet.
        removeElement(remainingVertices, neighbour);
        lastElemOfPath = neighbour; // Neighbour is the new last element.

        //  If this extension can become a Hamiltonian cycle, so can the current path.
        if (canBeHamiltonian(adjacencyList, remainingVertices, lastElemOfPath, firstElemOfPath, numberOfVertices, pathLength + 1)) {
            return true;
        }

        //  If we reach this part, the extension could not become a Hamiltonian cycle, hence we need to look again
        //  at the other possible extensions for our old path.
        add(remainingVertices, lastElemOfPath);
        lastElemOfPath = oldElemOfPath;
    }

    //  None of the possible extensions worked, so the path cannot be a Hamiltonian cycle.
    return false;
}

bool canBeHamiltonianPrintCycleCIndependentVersion(bitset adjacencyList[], bitset remainingVertices, int pathList[], int lastElemOfPath, int firstElemOfPath, int numberOfVertices, int pathLength, int* numberOfHamiltonianCycles, bool allCyclesFlag, int upperBound, Array *MISArr) {
    if((*numberOfHamiltonianCycles) >= upperBound)
    {
        return true;
    }
    // Check whether we have a Hamiltonian path already and whether this path is a cycle.
    if((pathLength == numberOfVertices) && contains(adjacencyList[firstElemOfPath], lastElemOfPath)) {
        /*fprintf(stderr,"Hamiltonian cycle: ");
        for(int i = 1; i < numberOfVertices; i++) {
            fprintf(stderr, "%d -> ", pathList[i]);
        }
        fprintf(stderr,"%d\n",pathList[0]);*/
    
        bool allCanBeDestroyed=true;
        for(int i=0; i<MISArr->used; i++)
        {
            bool currentMISCanBeDestroyed=false;
            for(int j=0; j<numberOfVertices; j++)
            {
                int currNode=pathList[j];
                int nextNode=pathList[(j+1)%numberOfVertices];
                if(contains(MISArr->array[i],currNode) && contains(MISArr->array[i],nextNode))
                {
                    currentMISCanBeDestroyed=true;
                    break;
                }
            }
            if(!currentMISCanBeDestroyed)
            {
                allCanBeDestroyed=false;
                break;
            }
        }
        if(allCanBeDestroyed)
        {
            fprintf(stderr,"all minimal dominating sets: \n");
            for(int i=0; i<MISArr->used; i++)
            {
                for(int j=0; j<numberOfVertices; j++)
                {
                    if(contains(MISArr->array[i],j)) fprintf(stderr,"%d ",j);
                }
                fprintf(stderr,"\n");
            }
            fprintf(stderr,"Hamiltonian cycle which destroys all minimal dominating sets: ");
            for(int i = 1; i < numberOfVertices; i++) {
                fprintf(stderr, "%d -> ", pathList[i]);
            }
            fprintf(stderr,"%d\n",pathList[0]);

            bool someEdgeRemovalAlsoWorks=false;
            int correct_xyz=-1;
            for(int xyz=0; xyz<numberOfVertices; xyz++)
            {
                bool allCanBeDestroyed2=true;
                for(int i=0; i<MISArr->used; i++)
                {
                    bool currentMISCanBeDestroyed=false;
                    for(int j=0; j<numberOfVertices; j++)
                    {
                        if(j==xyz) continue;
                        int currNode=pathList[j];
                        int nextNode=pathList[(j+1)%numberOfVertices];
                        if(contains(MISArr->array[i],currNode) && contains(MISArr->array[i],nextNode))
                        {
                            currentMISCanBeDestroyed=true;
                            break;
                        }
                    }
                    if(!currentMISCanBeDestroyed)
                    {
                        allCanBeDestroyed2=false;
                        break;
                    }
                }
                if(allCanBeDestroyed2)
                {
                    someEdgeRemovalAlsoWorks=true;
                    correct_xyz=xyz;
                    break;
                }
            }

            // take complement
            for(int i=0; i<numberOfVertices; i++)
            {
                adjacencyList[i]=difference(complement(adjacencyList[i],numberOfVertices),singleton(i));
            }
            // add hamiltonian cycle
            for(int j=0; j<numberOfVertices; j++)
            {
                int currNode=pathList[j];
                int nextNode=pathList[(j+1)%numberOfVertices];
                add(adjacencyList[currNode],nextNode);
                add(adjacencyList[nextNode],currNode);
            }
            fprintf(stderr,"n=%d\n",numberOfVertices);
            for(int i=0; i<numberOfVertices; i++)
            {
                for(int j=0; j<numberOfVertices; j++)
                {
                    if(contains(adjacencyList[i],j)) fprintf(stderr,"1");
                    else fprintf(stderr,"0");
                }
                fprintf(stderr,"\n");
            }
            if(someEdgeRemovalAlsoWorks)
            {
                fprintf(stderr,"Remove edge between vertices %d and %d\n",pathList[correct_xyz],pathList[(correct_xyz+1)%numberOfVertices]);
                printf("Remove edge between vertices %d and %d\n",pathList[correct_xyz],pathList[(correct_xyz+1)%numberOfVertices]);
                exit(0);
            }
            printf("n=%d\n",numberOfVertices);
            for(int i=0; i<numberOfVertices; i++)
            {
                for(int j=0; j<numberOfVertices; j++)
                {
                    if(contains(adjacencyList[i],j)) printf("1");
                    else printf("0");
                }
                printf("\n");
            }

            printf("all minimal dominating sets: \n");
            for(int i=0; i<MISArr->used; i++)
            {
                for(int j=0; j<numberOfVertices; j++)
                {
                    if(contains(MISArr->array[i],j)) printf("%d ",j);
                }
                printf("\n");
            }
            printf("Hamiltonian cycle which destroys all minimal dominating sets: ");
            for(int i = 1; i < numberOfVertices; i++) {
                printf("%d -> ", pathList[i]);
            }
            printf("%d\n",pathList[0]);

            // remove hamiltonian cycle
            for(int j=0; j<numberOfVertices; j++)
            {
                int currNode=pathList[j];
                int nextNode=pathList[(j+1)%numberOfVertices];
                removeElement(adjacencyList[currNode],nextNode);
                removeElement(adjacencyList[nextNode],currNode);
            }
            // take complement
            for(int i=0; i<numberOfVertices; i++)
            {
                adjacencyList[i]=difference(complement(adjacencyList[i],numberOfVertices),singleton(i));
            }
            //fprintf(stderr,"found one\n");
        }        
        (*numberOfHamiltonianCycles)++;
        return true;
    }

    // Check if cycle can still be closed with remaining vertices.
    if(isEmpty(intersection(adjacencyList[firstElemOfPath],remainingVertices))) return false;

    // Check for all elements not yet visited whether they still have two neighbours to which they can connect.
    bitset remainingWithFirstAndLast = union(remainingVertices, union(singleton(firstElemOfPath), singleton(lastElemOfPath)));
    forEach(vertex, remainingVertices) {

        //  Neighbours which either do not lie in the path, or which are one of its endpoints.
        bitset remainingNeighbours = intersection(adjacencyList[vertex], remainingWithFirstAndLast );

        //  If there is only one such neighbour or less, our path cannot be extended through this vertex into a Hamiltonian cycle.
        if(size(remainingNeighbours) < 2) return false;
    }


    // Create a bitset of the neighbours of the last element in the path which do not belong to the path. 
    // The path will be extended via these neighbours.
    bitset neighboursOfLastNotInPath = intersection(adjacencyList[lastElemOfPath], remainingVertices);
    forEach(neighbour, neighboursOfLastNotInPath) {

        //  Save the current last element of path. If an extension cannot be a Hamiltonian cycle, we need to try the
        //  other possible extensions for this path.
        int oldElemOfPath = lastElemOfPath;

        //  Extend the path with neighbour, which is a neighbour oldElemOfPath that does no belong to the path yet.
        removeElement(remainingVertices, neighbour);
        lastElemOfPath = neighbour; // Neighbour is the new last element.
        pathList[pathLength] = neighbour;

        //  If this extension can become a Hamiltonian cycle, so can the current path.
        if (canBeHamiltonianPrintCycleCIndependentVersion(adjacencyList, remainingVertices, pathList, lastElemOfPath, firstElemOfPath, numberOfVertices, pathLength + 1, numberOfHamiltonianCycles, allCyclesFlag,upperBound,MISArr)) {
            if(!allCyclesFlag)
                return true;
        }

        //  If we reach this part, the extension could not become a Hamiltonian cycle, hence we need to look again
        //  at the other possible extensions for our old path.
        add(remainingVertices, lastElemOfPath);
        lastElemOfPath = oldElemOfPath;
    }

    //  None of the possible extensions worked, so the path cannot be a Hamiltonian cycle.
    return false;
}

bool canBeHamiltonianPrintCycle(bitset adjacencyList[], bitset remainingVertices, int pathList[], int lastElemOfPath, int firstElemOfPath, int numberOfVertices, int pathLength, int* numberOfHamiltonianCycles, bool allCyclesFlag, int upperBound) {
    if((*numberOfHamiltonianCycles) >= upperBound)
    {
        return true;
    }
    // Check whether we have a Hamiltonian path already and whether this path is a cycle.
    if((pathLength == numberOfVertices) && contains(adjacencyList[firstElemOfPath], lastElemOfPath)) {
        /*fprintf(stderr,"Hamiltonian cycle: ");
        for(int i = 1; i < numberOfVertices; i++) {
            fprintf(stderr, "%d -> ", pathList[i]);
        }
        fprintf(stderr,"%d\n",pathList[0]);*/
        
        (*numberOfHamiltonianCycles)++;
        return true;
    }

    // Check if cycle can still be closed with remaining vertices.
    if(isEmpty(intersection(adjacencyList[firstElemOfPath],remainingVertices))) return false;

    // Check for all elements not yet visited whether they still have two neighbours to which they can connect.
    bitset remainingWithFirstAndLast = union(remainingVertices, union(singleton(firstElemOfPath), singleton(lastElemOfPath)));
    forEach(vertex, remainingVertices) {

        //  Neighbours which either do not lie in the path, or which are one of its endpoints.
        bitset remainingNeighbours = intersection(adjacencyList[vertex], remainingWithFirstAndLast );

        //  If there is only one such neighbour or less, our path cannot be extended through this vertex into a Hamiltonian cycle.
        if(size(remainingNeighbours) < 2) return false;
    }


    // Create a bitset of the neighbours of the last element in the path which do not belong to the path. 
    // The path will be extended via these neighbours.
    bitset neighboursOfLastNotInPath = intersection(adjacencyList[lastElemOfPath], remainingVertices);
    forEach(neighbour, neighboursOfLastNotInPath) {

        //  Save the current last element of path. If an extension cannot be a Hamiltonian cycle, we need to try the
        //  other possible extensions for this path.
        int oldElemOfPath = lastElemOfPath;

        //  Extend the path with neighbour, which is a neighbour oldElemOfPath that does no belong to the path yet.
        removeElement(remainingVertices, neighbour);
        lastElemOfPath = neighbour; // Neighbour is the new last element.
        pathList[pathLength] = neighbour;

        //  If this extension can become a Hamiltonian cycle, so can the current path.
        if (canBeHamiltonianPrintCycle(adjacencyList, remainingVertices, pathList, lastElemOfPath, firstElemOfPath, numberOfVertices, pathLength + 1, numberOfHamiltonianCycles, allCyclesFlag,upperBound)) {
            if(!allCyclesFlag || (*numberOfHamiltonianCycles) >= upperBound)
                return true;
        }

        //  If we reach this part, the extension could not become a Hamiltonian cycle, hence we need to look again
        //  at the other possible extensions for our old path.
        add(remainingVertices, lastElemOfPath);
        lastElemOfPath = oldElemOfPath;
    }

    //  None of the possible extensions worked, so the path cannot be a Hamiltonian cycle.
    return false;
}

bool canBeHamiltonianPrintCycleWithEdgeCounts(bitset adjacencyList[], bitset remainingVertices, int pathList[], int lastElemOfPath, int firstElemOfPath, int numberOfVertices, int pathLength, int* numberOfHamiltonianCycles, bool allCyclesFlag, int upperBound, int* edgeCounts) {
    if((*numberOfHamiltonianCycles) >= upperBound)
    {
        return true;
    }
    // Check whether we have a Hamiltonian path already and whether this path is a cycle.
    if((pathLength == numberOfVertices) && contains(adjacencyList[firstElemOfPath], lastElemOfPath)) {
        for(int i = 0; i < numberOfVertices; i++) {
            int from=pathList[i];
            int to=pathList[(i+1)%numberOfVertices];
            edgeCounts[from*numberOfVertices+to]++;
            edgeCounts[to*numberOfVertices+from]++;
        }
        (*numberOfHamiltonianCycles)++;
        return true;
    }

    // Check if cycle can still be closed with remaining vertices.
    if(isEmpty(intersection(adjacencyList[firstElemOfPath],remainingVertices))) return false;

    // Check for all elements not yet visited whether they still have two neighbours to which they can connect.
    bitset remainingWithFirstAndLast = union(remainingVertices, union(singleton(firstElemOfPath), singleton(lastElemOfPath)));
    forEach(vertex, remainingVertices) {

        //  Neighbours which either do not lie in the path, or which are one of its endpoints.
        bitset remainingNeighbours = intersection(adjacencyList[vertex], remainingWithFirstAndLast );

        //  If there is only one such neighbour or less, our path cannot be extended through this vertex into a Hamiltonian cycle.
        if(size(remainingNeighbours) < 2) return false;
    }


    // Create a bitset of the neighbours of the last element in the path which do not belong to the path. 
    // The path will be extended via these neighbours.
    bitset neighboursOfLastNotInPath = intersection(adjacencyList[lastElemOfPath], remainingVertices);
    forEach(neighbour, neighboursOfLastNotInPath) {

        //  Save the current last element of path. If an extension cannot be a Hamiltonian cycle, we need to try the
        //  other possible extensions for this path.
        int oldElemOfPath = lastElemOfPath;

        //  Extend the path with neighbour, which is a neighbour oldElemOfPath that does no belong to the path yet.
        removeElement(remainingVertices, neighbour);
        lastElemOfPath = neighbour; // Neighbour is the new last element.
        pathList[pathLength] = neighbour;

        //  If this extension can become a Hamiltonian cycle, so can the current path.
        if (canBeHamiltonianPrintCycleWithEdgeCounts(adjacencyList, remainingVertices, pathList, lastElemOfPath, firstElemOfPath, numberOfVertices, pathLength + 1, numberOfHamiltonianCycles, allCyclesFlag,upperBound,edgeCounts)) {
            if(!allCyclesFlag || (*numberOfHamiltonianCycles) >= upperBound)
                return true;
        }

        //  If we reach this part, the extension could not become a Hamiltonian cycle, hence we need to look again
        //  at the other possible extensions for our old path.
        add(remainingVertices, lastElemOfPath);
        lastElemOfPath = oldElemOfPath;
    }

    //  None of the possible extensions worked, so the path cannot be a Hamiltonian cycle.
    return false;
}

bool isHamiltonian(bitset adjacencyList[], int numberOfVertices, bool allCyclesFlag, int upperBound) {
    bool foundHamiltonianCycle = false;
    bool upperBoundReached=false;
    int numberOfHamiltonianCycles = 0;
    int leastNeighbours = size(adjacencyList[0]);
    int start = 0;
    int a;
    for(int i = 1; i < numberOfVertices; i++){
        if(leastNeighbours > (a = size(adjacencyList[i]))){
            leastNeighbours = a;
            start = i;
        }
    }
    // Loop over neighbours of 0 and for each neighbour loop over the neighbours of 0 that are of higher index.
    forEach(secondElemOfPath, adjacencyList[start]) {
        forEachAfterIndex(lastElemOfPath, adjacencyList[start], secondElemOfPath) {

            // Create path, lastElemOfPath, 0, secondElemOfPath. We have lastElemOfPath > secondElemOfPath, so that we eliminate the checking of paths which are mirrored.
            bitset path = singleton(start);
            add(path, lastElemOfPath);
            add(path, secondElemOfPath);
            bitset remainingVertices = complement(path, numberOfVertices);

            if(allCyclesFlag) {
                int pathList[numberOfVertices];
                pathList[0] = lastElemOfPath;
                pathList[1] = start;
                pathList[2] = secondElemOfPath;
                // Check if this path can be extended to some Hamiltonian cycle.
                canBeHamiltonianPrintCycle(adjacencyList, remainingVertices, pathList, secondElemOfPath, lastElemOfPath, numberOfVertices, 3, &numberOfHamiltonianCycles, allCyclesFlag,upperBound);
                if(numberOfHamiltonianCycles) {
                    foundHamiltonianCycle = true;
                }
                if(numberOfHamiltonianCycles>=upperBound)
                {
                    upperBoundReached=true;
                    break;
                }
            }
            else {
                // Check if this path can be extended to some Hamiltonian cycle.
                if (canBeHamiltonian(adjacencyList, remainingVertices, lastElemOfPath, secondElemOfPath, numberOfVertices, 3)) {
                    return true;
                }
            }
        }
        if(upperBoundReached)
        {
            break;
        }
    }
    /*if(allCyclesFlag) {
       fprintf(stderr,"There were %d Hamiltonian cycles in this graph.\n", numberOfHamiltonianCycles);
    }*/
    if(allCyclesFlag) {
       if(upperBoundReached)
       {
            printf(">=");
       }
       printf("%d\n", numberOfHamiltonianCycles);
    }
    return foundHamiltonianCycle;
} 

bool isHamiltonianWithCount(bitset adjacencyList[], int numberOfVertices, bool allCyclesFlag, int upperBound, int* numberOfHamiltonianCycles) {
    bool foundHamiltonianCycle = false;
    bool upperBoundReached=false;
    int leastNeighbours = size(adjacencyList[0]);
    int start = 0;
    int a;
    for(int i = 1; i < numberOfVertices; i++){
        if(leastNeighbours > (a = size(adjacencyList[i]))){
            leastNeighbours = a;
            start = i;
        }
    }

    // Loop over neighbours of 0 and for each neighbour loop over the neighbours of 0 that are of higher index.
    forEach(secondElemOfPath, adjacencyList[start]) {
        forEachAfterIndex(lastElemOfPath, adjacencyList[start], secondElemOfPath) {

            // Create path, lastElemOfPath, 0, secondElemOfPath. We have lastElemOfPath > secondElemOfPath, so that we eliminate the checking of paths which are mirrored.
            bitset path = singleton(start);
            add(path, lastElemOfPath);
            add(path, secondElemOfPath);
            bitset remainingVertices = complement(path, numberOfVertices);

            if(allCyclesFlag) {
                int pathList[numberOfVertices];
                pathList[0] = lastElemOfPath;
                pathList[1] = start;
                pathList[2] = secondElemOfPath;
                // Check if this path can be extended to some Hamiltonian cycle.
                canBeHamiltonianPrintCycle(adjacencyList, remainingVertices, pathList, secondElemOfPath, lastElemOfPath, numberOfVertices, 3, numberOfHamiltonianCycles, allCyclesFlag,upperBound);
                if((*numberOfHamiltonianCycles)) {
                    foundHamiltonianCycle = true;
                }
                if((*numberOfHamiltonianCycles)>=upperBound)
                {
                    upperBoundReached=true;
                    break;
                }
            }
            else {
                // Check if this path can be extended to some Hamiltonian cycle.
                if (canBeHamiltonian(adjacencyList, remainingVertices, lastElemOfPath, secondElemOfPath, numberOfVertices, 3)) {
                    return true;
                }
            }
        }
        if(upperBoundReached)
        {
            break;
        }
    }
    /*if(allCyclesFlag) {
       fprintf(stderr,"There were %d Hamiltonian cycles in this graph.\n", numberOfHamiltonianCycles);
    }*/
    if(allCyclesFlag) {
       /*if(upperBoundReached)
       {
            printf(">=");
       }
       printf("%d\n", numberOfHamiltonianCycles);*/
    }
    return foundHamiltonianCycle;
}
 
bool isHamiltonianCIndependentVersion(bitset adjacencyList[], int numberOfVertices, bool allCyclesFlag, int upperBound, Array *MISArr) {
    bool foundHamiltonianCycle = false;
    bool upperBoundReached=false;
    int numberOfHamiltonianCycles = 0;
    int leastNeighbours = size(adjacencyList[0]);
    int start = 0;
    int a;
    for(int i = 1; i < numberOfVertices; i++){
        if(leastNeighbours > (a = size(adjacencyList[i]))){
            leastNeighbours = a;
            start = i;
        }
    }

    // Loop over neighbours of 0 and for each neighbour loop over the neighbours of 0 that are of higher index.
    forEach(secondElemOfPath, adjacencyList[start]) {
        forEachAfterIndex(lastElemOfPath, adjacencyList[start], secondElemOfPath) {

            // Create path, lastElemOfPath, 0, secondElemOfPath. We have lastElemOfPath > secondElemOfPath, so that we eliminate the checking of paths which are mirrored.
            bitset path = singleton(start);
            add(path, lastElemOfPath);
            add(path, secondElemOfPath);
            bitset remainingVertices = complement(path, numberOfVertices);

            if(allCyclesFlag) {
                int pathList[numberOfVertices];
                pathList[0] = lastElemOfPath;
                pathList[1] = start;
                pathList[2] = secondElemOfPath;
                // Check if this path can be extended to some Hamiltonian cycle.
                canBeHamiltonianPrintCycleCIndependentVersion(adjacencyList, remainingVertices, pathList, secondElemOfPath, lastElemOfPath, numberOfVertices, 3, &numberOfHamiltonianCycles, allCyclesFlag,upperBound,MISArr);
                if(numberOfHamiltonianCycles) {
                    foundHamiltonianCycle = true;
                }
            }
            else {
                // Check if this path can be extended to some Hamiltonian cycle.
                if (canBeHamiltonian(adjacencyList, remainingVertices, lastElemOfPath, secondElemOfPath, numberOfVertices, 3)) {
                    return true;
                }
            }
        }
    }
    /*if(allCyclesFlag) {
       fprintf(stderr,"There were %d Hamiltonian cycles in this graph.\n", numberOfHamiltonianCycles);
    }*/
    return foundHamiltonianCycle;
} 

bool isHamiltonianWithEdgeCounts(bitset adjacencyList[], int numberOfVertices, bool allCyclesFlag, int upperBound, int* edgeCounts) {
    bool foundHamiltonianCycle = false;
    bool upperBoundReached=false;
    int numberOfHamiltonianCycles = 0;
    int leastNeighbours = size(adjacencyList[0]);
    int start = 0;
    int a;
    for(int i = 1; i < numberOfVertices; i++){
        if(leastNeighbours > (a = size(adjacencyList[i]))){
            leastNeighbours = a;
            start = i;
        }
    }

    // Loop over neighbours of 0 and for each neighbour loop over the neighbours of 0 that are of higher index.
    forEach(secondElemOfPath, adjacencyList[start]) {
        forEachAfterIndex(lastElemOfPath, adjacencyList[start], secondElemOfPath) {

            // Create path, lastElemOfPath, 0, secondElemOfPath. We have lastElemOfPath > secondElemOfPath, so that we eliminate the checking of paths which are mirrored.
            bitset path = singleton(start);
            add(path, lastElemOfPath);
            add(path, secondElemOfPath);
            bitset remainingVertices = complement(path, numberOfVertices);

            if(allCyclesFlag) {
                int pathList[numberOfVertices];
                pathList[0] = lastElemOfPath;
                pathList[1] = start;
                pathList[2] = secondElemOfPath;
                // Check if this path can be extended to some Hamiltonian cycle.
                canBeHamiltonianPrintCycleWithEdgeCounts(adjacencyList, remainingVertices, pathList, secondElemOfPath, lastElemOfPath, numberOfVertices, 3, &numberOfHamiltonianCycles, allCyclesFlag,upperBound,edgeCounts);
                if(numberOfHamiltonianCycles) {
                    foundHamiltonianCycle = true;
                }
                if(numberOfHamiltonianCycles>=upperBound)
                {
                    upperBoundReached=true;
                    break;
                }
            }
            else {
                // Check if this path can be extended to some Hamiltonian cycle.
                if (canBeHamiltonian(adjacencyList, remainingVertices, lastElemOfPath, secondElemOfPath, numberOfVertices, 3)) {
                    return true;
                }
            }
        }
        if(upperBoundReached)
        {
            break;
        }
    }
    /*if(allCyclesFlag) {
       fprintf(stderr,"There were %d Hamiltonian cycles in this graph.\n", numberOfHamiltonianCycles);
    }*/
    if(allCyclesFlag) {
        int smallest=(int)1e9;
        for(int i=0; i<numberOfVertices*numberOfVertices; i++)
        {
            if(edgeCounts[i]>0 && edgeCounts[i]<smallest)
            {
                smallest=edgeCounts[i];
            }
        }
       //printf("%d\n", smallest);
    }
    return foundHamiltonianCycle;
} 

bool exclusionIsHamiltonian(bitset adjacencyList[], int totalNumberOfVertices, int newNumberOfVertices, bitset excludedVertices, bool printPathFlag) {
    return false; // not necessary
}

bool isK1Hamiltonian(bitset adjacencyList[], int numberOfVertices, bool verboseFlag) {
    bool encounteredNonHamSubgraph = false;
    bitset excludedVertices;
    for (int i = 0; i < numberOfVertices; i++) {
        excludedVertices = singleton(i);
        if (!(exclusionIsHamiltonian(adjacencyList, numberOfVertices, numberOfVertices - 1, excludedVertices, false))) {
            if(verboseFlag){
                fprintf(stderr, "G - %d is not hamiltonian\n", i);
                encounteredNonHamSubgraph = true;
            }
            else {
                return false;
            }
        }
    }
    return !encounteredNonHamSubgraph;
}

bool isK2Hamiltonian(bitset adjacencyList[], int numberOfVertices, bool verboseFlag) {
    for (int i = 0; i < numberOfVertices; i++) { //Check whether all vertices have degree at least 3
        if (size(adjacencyList[i]) < 3) {
            return false;
        }
    }
    bool encounteredNonHamSubgraph = false;
    bitset excludedVertices;
    for (int i = 0; i < numberOfVertices; i++) {
        excludedVertices = singleton(i);
        // Loop over neighbours of i which are of higher index.
        forEachAfterIndex(neighbour, adjacencyList[i], i) {
            add(excludedVertices, neighbour);
            if(!(exclusionIsHamiltonian(adjacencyList, numberOfVertices, numberOfVertices - 2, excludedVertices, false))){
                if(verboseFlag) {
                    fprintf(stderr, "Not hamiltonian for edge %d %d\n",i, neighbour);
                    encounteredNonHamSubgraph = true;
                }
                else {
                    return false;
                }
            }
            removeElement(excludedVertices, neighbour);
        }
    }
    return !encounteredNonHamSubgraph;
}
