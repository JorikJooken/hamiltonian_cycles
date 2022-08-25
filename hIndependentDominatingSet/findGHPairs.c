#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <time.h>
#include "bitset.h"
#include "hamiltonicityMethods.h"
#include "read_graph/readGraph6.h"

bitset minimal_dominating_set;

void generateMinimalDominatingSets(int idx, bitset adjacencyList[], int nVertices, Array *a)
{
    bool some_child_ok=false;
    forEach(el2,minimal_dominating_set)
    {
        removeElement(minimal_dominating_set, el2);
        // check if child is ok
        bitset union_of_neighbors=EMPTY;
        forEach(el, minimal_dominating_set)
        {
            union_of_neighbors=(union_of_neighbors|adjacencyList[el]);
        }
        // current set is dominating
        if((union_of_neighbors&complement(minimal_dominating_set,nVertices))==complement(minimal_dominating_set,nVertices))
        {
            some_child_ok=true;
            if(el2 >= idx) generateMinimalDominatingSets(el2+1,adjacencyList,nVertices,a);
        }
        add(minimal_dominating_set,el2);
    }
    if(!some_child_ok)
    {
        insertArray(a,minimal_dominating_set);
    }
}

int main(int argc, char ** argv) {


	bool verboseFlag = false;
	bool allCyclesFlag = true;
	int nSplits = -1;
	int iteration = -1;
	int opt;
	while (1) {
    	int option_index = 0;
    	static struct option long_options[] = 
    	{
        	{"assume-non-hamiltonian",  no_argument, NULL,  'n'},
        	{"K1-hamiltonian",  		no_argument, NULL,  '1'},
        	{"K2-hamiltonian",  		no_argument, NULL,  '2'},
        	{"quiet", 					no_argument, NULL,  'q'},
        	{"verbose", 				no_argument, NULL, 	'v'},
        	{"all-cycles",				no_argument, NULL,	'a'},
        	{"split", 					required_argument, NULL, 's'},
        	{"iteration",				required_argument, NULL, 'i'},
            {"upper-bound",             required_argument, NULL, 'u'},
        	{NULL,      				0, 			 NULL,    0}
    	};

    	opt = getopt_long(argc, argv, "vafs:i:", long_options, &option_index);
    	if (opt == -1) break;
		switch(opt) {
			case 0:
        		fprintf(stderr,"long option %s", long_options[option_index].name);
        	if (optarg)
           		fprintf(stderr," with arg %s", optarg);
        	fprintf(stderr,"\n");
        	break;
			case 'v':
				verboseFlag = true;
				break;
			case 'a':
				allCyclesFlag = true;
				break;
			case 's':
            	nSplits = (int) strtol(optarg, (char **)NULL, 10);
            	break;
            case 'i':
            	iteration = (int) strtol(optarg, (char **)NULL, 10);
            	break;
			case '?':
        		fprintf(stderr,"Error: Unknown option: %c\n", optopt);
        		abort();
      		case ':':
       			fprintf(stderr,"Error: Missing arg for %c\n", optopt);
        		abort();
		}
	}

	if (nSplits == -1) {
		nSplits = 1;
	}
	
	if (iteration == -1) {
		iteration = 0;
	}
	

	if(nSplits <= iteration) {
		fprintf(stderr, "Error: The iteration cannot be higher or equal to the numbers of splits. Iterations start at zero.\n");
		abort();
	}



	unsigned long long int counter = 0;
	unsigned long long int total = 0;
    unsigned long long int numValidGraphCyclePairs=0;

	clock_t start = clock();

	char * graphString = NULL;
	size_t size;
	while(getline(&graphString, &size, stdin) != -1) {
		if (total++ % nSplits != iteration) {
			continue;
		}
		int nVertices = getNumberOfVertices(graphString);
		bitset adjacencyList[nVertices];
		loadGraph(graphString, nVertices, adjacencyList);
		counter++;
		if(verboseFlag) 
			fprintf(stderr, "Looking at: %s", graphString);

            minimal_dominating_set=complement(EMPTY,nVertices);
            Array a;
            initArray(&a, 1);  // initially 1 element
            generateMinimalDominatingSets(0,adjacencyList,nVertices,&a);
            // go over all hamiltonian cycles of complement graph and check if all minimal dominating sets can be destroyed
            for(int i=0; i<nVertices; i++)
            {
                adjacencyList[i]=difference(complement(adjacencyList[i],nVertices),singleton(i));
            }
            isHamiltonianCIndependentVersion(adjacencyList, nVertices, allCyclesFlag, 1e9, &a, &numValidGraphCyclePairs, graphString,verboseFlag);
            if(verboseFlag) fprintf(stderr,"Checked graph: %s\n",graphString);
            freeArray(&a);
	}
	clock_t end = clock();
	double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    
	fprintf(stderr,"\rChecked %lld graphs in %f seconds: ", counter, time_spent);
    fprintf(stderr,"\nFound %lld valid graph cycle pairs\n", numValidGraphCyclePairs);

	return 0;
}
