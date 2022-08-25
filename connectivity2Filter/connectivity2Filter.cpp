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
	for (int index = startIndex; index<(int)graphString.size(); index++) {
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

int n;
long long adjacencyList[MAXVERTICES];

// checks whether the graph is connected when two vertices are omitted.
bool is_connected(int omit1, int omit2)
{
    vector<bool> visited(n,false);
    int start=0;
    while(start==omit1 || start==omit2) start++;
    queue<int> q;
    q.push(start);
    visited[start]=true;
    while(!q.empty())
    {
        int now=q.front();
        q.pop();
        traverse_through_neighbors_nest1(adjacencyList[now],neigh)
        {
            if(neigh==omit1 || neigh==omit2) continue;
            if(visited[neigh]) continue;
            visited[neigh]=true;
            q.push(neigh);
        }
    }
    for(int i=0; i<n; i++)
    {
        if(i==omit1 || i==omit2) continue;
        if(!visited[i]) return false;
    }
    return true;
}

int num_connectivity_zero=0;
int num_connectivity_one=0;
int num_connectivity_two=0;
int num_connectivity_at_least_three=0;

bool is_two_vertex_connected()
{
    if(!is_connected(-1,-1))
    {
        num_connectivity_zero++;
        return false; // graph has connectivity 0
    }
    for(int i=0; i<n; i++)
    {
        if(!is_connected(-1,i))
        {
            num_connectivity_one++;
            return false; // graph has connectivity 1
        }
    }
    for(int i=0; i<n; i++)
    {
        for(int j=i+1; j<n; j++)
        {
            if(!is_connected(i,j))
            {
                num_connectivity_two++;
                return true; // graph has connectivity 2
            }
        }
    }
    num_connectivity_at_least_three++;
    return false; // graph has connectivity >= 3
}

// reads graphs and outputs those that have vertex connectivity precisely equal to 2.
// the connectivity algorithm is the most straightforward algorithm that uses the definition of vertex connectivity.
// more efficient algorithms exist, using Tarjan for finding articulation points and again Tarjan for finding biconnected components.
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    long long nb_graphs_read_from_input=0;
    long long nb_two_connected_graphs=0;
    string line;
    while(getline(cin,line))
    {
        //line+="\n";
        nb_graphs_read_from_input++;
        n = getNumberOfVertices(line);
        loadGraph(line,n,adjacencyList);
        if(is_two_vertex_connected())
        {
            nb_two_connected_graphs++;
            printf("%s\n", line.c_str());
        }
    }
    cerr << nb_graphs_read_from_input << " graphs were read from input and: " << endl;
    cerr << num_connectivity_zero << " of these have connectivity 0." << endl;
    cerr << num_connectivity_one << " of these have connectivity 1." << endl;
    cerr << num_connectivity_two << " of these have connectivity 2." << endl;
    cerr << num_connectivity_at_least_three << " of these have connectivity 3 or more." << endl;
    return 0;
}
