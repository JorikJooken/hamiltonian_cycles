#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include <time.h>
#include "bitset.h"
#include "hamiltonicityMethods.h"
#include "read_graph/readGraph6.h"

// assumes all nodes have the same degree K, except for 2 nodes which have degree less than K.
// the program will find these 2 nodes and determine the number of hamiltonian paths between them.
int main(int argc, char ** argv) {
	bool assumeNonHamFlag = false;
	bool K1flag = false;
	bool K2flag = false;
	bool quietFlag = false;
	bool verboseFlag = false;
	bool allCyclesFlag = false;
	int nSplits = -1;
	int iteration = -1;
    int upperBound=(int)1e9;
	int opt;
    long long numGraphsNoHamPathPair=0;
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

    	opt = getopt_long(argc, argv, "n12qvas:i:u:", long_options, &option_index);
    	if (opt == -1) break;
		switch(opt) {
			case 0:
        		fprintf(stderr,"long option %s", long_options[option_index].name);
        	if (optarg)
           		fprintf(stderr," with arg %s", optarg);
        	fprintf(stderr,"\n");
        	break;
			case '1':
				K1flag = true;
				break;
			case '2':
				K2flag = true;
				break;
			case 'n':
				assumeNonHamFlag = true;
				break;
			case 'q':
				quietFlag = true;
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

	if (K1flag && K2flag) {
		fprintf(stderr,"Error: Do not use these flags simultaneously.\n");
		abort();
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
	unsigned long long int nonHamiltonianCounter = 0;
	unsigned long long int K1Counter = 0;
	unsigned long long int K2Counter = 0;



	clock_t start = clock();

	char * graphString = NULL;
	size_t size;
	while(getline(&graphString, &size, stdin) != -1) {
		if (total++ % nSplits != iteration) {
			continue;
		}
		int nVertices = getNumberOfVertices(graphString);
		bitset adjacencyList[nVertices+1]; // warning: might be incorrect in case nVertices is close to MAXVERTICES
		loadGraph(graphString, nVertices, adjacencyList);
        
        adjacencyList[nVertices]=EMPTY;
        nVertices+=1;

		counter++;
		if(verboseFlag) 
			fprintf(stderr, "Looking at: %s", graphString);

		bool isHam;
		if (assumeNonHamFlag) {
			isHam = false;
		}
		else {
            for(int i=0; i<nVertices-1; i++)
            {
                bool br=false;
                for(int j=i+1; j<nVertices-1; j++)
                {
                    if(adjacencyList[i]&(1LL<<j)) continue;
                    add(adjacencyList[nVertices-1], i);
		            add(adjacencyList[i], nVertices-1);
                    add(adjacencyList[nVertices-1], j);
		            add(adjacencyList[j], nVertices-1);
                    isHam = isHamiltonian(adjacencyList, nVertices, allCyclesFlag, upperBound);
                    if(!isHam)
                    {
                        numGraphsNoHamPathPair++;
                        printf("%s\n",graphString);
                        printf("no ham path between %d and %d\n",i,j);
                        br=true;
                        break;
                    }
                    removeElement(adjacencyList[nVertices-1], i);
		            removeElement(adjacencyList[i], nVertices-1);
                    removeElement(adjacencyList[nVertices-1], j);
		            removeElement(adjacencyList[j], nVertices-1);
                }
                if(br) break;
            }
		}
	}
	clock_t end = clock();
	double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    fprintf(stderr,"\rThere were %lld graphs for which there exists a non-adjacent pair of vertices with no hamiltonian path between them\n", numGraphsNoHamPathPair);
	fprintf(stderr,"\rChecked %lld graphs in %f seconds: ", counter, time_spent);

    /*
	if (!assumeNonHamFlag) {
		fprintf(stderr,"%lld are non-Hamiltonian, ", nonHamiltonianCounter);
	}
	if (K1flag && !assumeNonHamFlag) {
		fprintf(stderr,"%lld are hypohamiltonian, ", K1Counter);
	}
	if (K1flag && assumeNonHamFlag) {
		fprintf(stderr,"%lld are K1-Hamiltonian, ", K1Counter);
	}
	if (K2flag && !assumeNonHamFlag) {
		fprintf(stderr,"%lld are K2-hypohamiltonian, ", K2Counter);
	}
	if (K2flag && assumeNonHamFlag) {
		fprintf(stderr,"%lld are K2-Hamiltonian, ", K2Counter);
	}*/
	fprintf(stderr,"\b\b.\n");

	return 0;
}
