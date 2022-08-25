#include <bits/stdc++.h>

// Unsafe because no defined behaviour if character = 0. ctz and clz work with 32 bit numbers.
#define unsafePrev(character, current) (__builtin_ctz(character) - current >= 0 ? -1 : current -__builtin_clz((character) << (32 - current)) - 1)

#define prev(character,current) (character ? unsafePrev(character,current) : -1)

using namespace std;

const int nb_bits=32;
#define MAXVERTICES 32

int getNumberOfVertices(string graphString) 
{
	if(graphString.size() == 0){
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

void loadGraph(string graphString, int numberOfVertices, long long adjacencyList[]) {
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
		adjacencyList[vertex] = 0;
	}

	int currentVertex = 1;
	int sum = 0; 
	for (int index = startIndex; index<graphString.size(); index++) {
		int i;
		for (i = prev(graphString[index] - 63, 6); i != -1; i = prev(graphString[index] - 63, i)) {
			while(5-i+(index-startIndex)*6 - sum >= 0) {
				sum += currentVertex;
				currentVertex++;
			}
			sum -= --currentVertex;
			int neighbour = 5-i+(index - startIndex)*6 - sum;
            adjacencyList[currentVertex]|=(1LL<<neighbour);
            adjacencyList[neighbour]|=(1LL<<currentVertex);
		}
	}
}

#define left_shifted(n,x) ((n)<<(x))
#define set_bit(n,i) ((n)|=left_shifted(1LL,i))
#define count_bits(n) (__builtin_popcountll((n)))
//#define index_rightmost_bit(n) (count_bits((n)^(n-1))-1)
#define index_rightmost_bit(n) (__builtin_ctzll(n))
#define rightmost_bit_removed(n) ((n)&((n)-1))
#define traverse_through_neighbors_nest1(x,neighbor) for(long long (neighbor)=index_rightmost_bit((x)),DESTROYED_MASK1=(x); (DESTROYED_MASK1)!=0; (DESTROYED_MASK1)=rightmost_bit_removed((DESTROYED_MASK1)),(neighbor)=index_rightmost_bit((DESTROYED_MASK1)))
#define traverse_through_neighbors_nest2(x,neighbor) for(long long (neighbor)=index_rightmost_bit((x)),DESTROYED_MASK2=(x); (DESTROYED_MASK2)!=0; (DESTROYED_MASK2)=rightmost_bit_removed((DESTROYED_MASK2)),(neighbor)=index_rightmost_bit((DESTROYED_MASK2)))

int n=-1;
long long adjacencyList[MAXVERTICES];

bool satisfies_conditions(int K, int connected_flag, int min_degree, int max_degree)
{
    int start=-2;
    int finish=-1;
    for(int i=0; i<n; i++)
    {
        int deg=count_bits(adjacencyList[i]);
        if(deg>K) return false;
        else if(deg==K) continue;
        else
        {
            if(start>=0) return false;
            start=finish;
            finish=i;
        }
    }
    if(start<0 || finish<0) return false;
    int deg1=count_bits(adjacencyList[start]);
    int deg2=count_bits(adjacencyList[finish]);
    if(min_degree != min(deg1,deg2) || max_degree != max(deg1,deg2)) return false;
    int are_connected=0;
    if(adjacencyList[start]&left_shifted(1,finish)) are_connected=1;
    return are_connected==connected_flag;
}

// expects K, connected_flag, min_degree and max_degree as arguments
// the program receives a list of graphs as input (one graph on each line)
// and produces as output those graphs for which there are precisely 2 nodes with degree less than K
// and these nodes are either linked with an edge or not (depending on the connected_flag).
// one of these nodes should have degree min_degree, while the other one has degree max_degree

int main(int argc, char *argv[])
{
    if(argc != 5)
    {
        cerr << "Wrong number of arguments." << endl;
    }
    int K=atoi(argv[1]);
    int connected_flag=atoi(argv[2]);
    int min_degree=atoi(argv[3]);
    int max_degree=atoi(argv[4]);

    ios::sync_with_stdio(false);
    cin.tie(0);
    long long nb_graphs_read_from_input=0;
    long long passed_filter=0;
    string line;
    while(getline(cin,line))
    {
        //line+="\n";
        nb_graphs_read_from_input++;
        n = getNumberOfVertices(line);
        loadGraph(line,n,adjacencyList);
        bool correct=satisfies_conditions(K,connected_flag,min_degree,max_degree);
        if(correct)
        {
            printf("%s\n", line.c_str());
            passed_filter++;
        }
    }
    cerr << nb_graphs_read_from_input << " graphs were read from input for the parameters n, K, connected_flag, min_degree, max_degree: " << n << " " << K << " " << connected_flag << " " <<  min_degree << " " << max_degree << endl;
    cerr << passed_filter << " graphs passed the filter for the parameters n, K, connected_flag, min_degree, max_degree: " << n << " " << K << " " << connected_flag << " " <<  min_degree << " " << max_degree << endl;
    return 0;
}
