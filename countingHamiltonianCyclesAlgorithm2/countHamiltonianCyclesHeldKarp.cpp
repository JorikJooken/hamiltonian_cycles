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

map< pair<long long, int>, long long> even_path_length;
map< pair<long long, int>, long long> odd_path_length;

long long get_num_hamiltonian_cycles()
{
    even_path_length.clear();
    odd_path_length.clear();
    int start=0;
    traverse_through_neighbors_nest1(adjacencyList[start],neigh)
    {
        odd_path_length[make_pair(1LL<<neigh,neigh)]=1;
    }

    // recursive case : extend current paths
    for(int length=2; length<=n-1; length++)
    {
        if(length&1)
        {
            for(auto it=even_path_length.begin(); it!=even_path_length.end(); it++)
            {
                pair<long long, int> p=it->first;
                long long current_bitset=p.first;
                int current_last=p.second;
                traverse_through_neighbors_nest1(adjacencyList[current_last],neigh)
                {
                    if(((1LL<<neigh)&current_bitset)>0 || neigh==start) continue;
                    long long new_bitset=(current_bitset|(1LL<<neigh));
                    odd_path_length[make_pair(new_bitset,neigh)]+=it->second;
                }
            }
            even_path_length.clear();
        }
        else
        {
            for(auto it=odd_path_length.begin(); it!=odd_path_length.end(); it++)
            {
                pair<long long, int> p=it->first;
                long long current_bitset=p.first;
                int current_last=p.second;
                traverse_through_neighbors_nest1(adjacencyList[current_last],neigh)
                {
                    if(((1LL<<neigh)&current_bitset)>0 || neigh==start) continue;
                    long long new_bitset=(current_bitset|(1LL<<neigh));
                    even_path_length[make_pair(new_bitset,neigh)]+=it->second;
                }
            }
            odd_path_length.clear();
        }
    }


    // retrieve answer
    long long all_visited_bitset=(((1LL<<n)-1)^(1LL<<start));
    long long num_cycles=0;
    if(n&1)
    {
        traverse_through_neighbors_nest1(adjacencyList[start],finish)
        {
            num_cycles+=even_path_length[make_pair(all_visited_bitset,finish)];
        }
    }
    else
    {
        traverse_through_neighbors_nest1(adjacencyList[start],finish)
        {
            num_cycles+=odd_path_length[make_pair(all_visited_bitset,finish)];
        }
    }
    return num_cycles;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    long long nb_graphs_read_from_input=0;
    string line;
    while(getline(cin,line))
    {
        nb_graphs_read_from_input++;
        n = getNumberOfVertices(line);
        loadGraph(line,n,adjacencyList);
        long long num_hamiltonian_cycles=get_num_hamiltonian_cycles();
        printf("%s\n%lld\n", line.c_str(),num_hamiltonian_cycles/2); // 0123 is considered the same as 0321, so divide by 2
    }
    cerr << nb_graphs_read_from_input << " graphs were read from input" << endl;;
    return 0;
}
