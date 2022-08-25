#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <time.h>
#include "bitset.h"
#include "hamiltonicityMethods.h"
#include "read_graph/readGraph6.h"

int main(int argc, char ** argv) {
	bool allCyclesFlag = false;
	int nSplits = -1;
	int iteration = -1;
    int upperBound=(int)1e9;
	int opt;
	while (1) {
    	int option_index = 0;
    	static struct option long_options[] = 
    	{
        	{"all-cycles",				no_argument, NULL,	'a'},
        	{"split", 					required_argument, NULL, 's'},
        	{"iteration",				required_argument, NULL, 'i'},
            {"upper-bound",             required_argument, NULL, 'u'},
        	{NULL,      				0, 			 NULL,    0}
    	};

    	opt = getopt_long(argc, argv, "as:i:u:", long_options, &option_index);
    	if (opt == -1) break;
		switch(opt) {
			case 0:
        		fprintf(stderr,"long option %s", long_options[option_index].name);
        	if (optarg)
           		fprintf(stderr," with arg %s", optarg);
        	fprintf(stderr,"\n");
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
            case 'u':
                upperBound = (int) strtol(optarg, (char **)NULL, 10);
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

        printf("%s", graphString);
        // This function will return true if and only if the graph encoded by adjacencyList is hamiltonian.
        // If allCyclesFlag is set to true, it will count and output the number of hamiltonian cycles of this graph.
        // The count will stop when a total of upperBound hamiltonian cycles have been encountered.
        // If upperBound hamiltonian cycles have been encountered, the output will be ">=upperBound".
		isHamiltonian(adjacencyList, nVertices, allCyclesFlag, upperBound);
		
	}
	clock_t end = clock();
	double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

	fprintf(stderr,"\rChecked %lld graphs in %f seconds: ", counter, time_spent);
	fprintf(stderr,"\b\b.\n");

	return 0;
}
