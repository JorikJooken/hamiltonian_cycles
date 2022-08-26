# hamiltonian_cycles
This repository contains code and data related to the paper "Few hamiltonian cycles in graphs with one or two vertex degrees".

The repository contains several folders, which will be described below. In this explanation, we will assume that linux is being used. Each repository that contains code also contains a makefile that can be executed by using "make" in the terminal. All of the algorithms use the standard input and standard output/standard error stream and they expect the graphs from the input to be in graph6 format.


## CODE
### countingHamiltonianCyclesAlgorithm1
This folder contains an algorithm ("countHamiltonianCycles") that can be used to count the number of hamiltonian cycles of a graph. This algorithm was taken from XXX (and some small modifications were made). The file "07_4_3.g6" contains a list of all connected 4-regular graphs on 7 vertices (there are 2 such graphs). One can count the number of hamiltonian cycles for each graph in this file by using the following command in a terminal:

```bash
./countHamiltonianCycles -a < 07_4_3.g6
```

The expected output is then:
```bash
F}oxw
24
F}hXw
23
```

This indicates the the graph "F}oxw" has 24 hamiltonian cycles and the graph "F}hXw" has 23 hamiltonian cycles.

It is also possible to specify an upper bound using the -u flag such that the algorithm will stop counting past this upper bound. For example, if we use:
```bash
./countHamiltonianCycles -a -u 24 < 07_4_3.g6
```

the expected output is:

```bash
F}oxw
>=24
F}hXw
23
```

### countingHamiltonianCyclesAlgorithm2
This folder contains another algorithm ("countHamiltonianCyclesHeldKarp") for counting the number of hamiltonian cycles based on the Held-Karp algorithm. The algorithm can be invoked as follows:

```bash
./countHamiltonianCyclesHeldKarp < 07_4_3.g6
```

and will produce the output:
```bash
F}oxw
24
F}hXw
23
```

The algorithm from this section tends to be faster for dense graphs, whereas the algorithm from the previous section tends to be faster for sparse graphs.

### makeMinList
This folder contains an algorithm ("minPicker") that is useful to process the output of the two previous algorithms. It will read the graphs and the number of hamiltonian cycles in these graphs and it will output the minimum number of hamiltonian cycles, together with a list of all graphs from the input that have this number of hamiltonian cycles.

The following command:
```bash
./../countingHamiltonianCyclesAlgorithm1/countHamiltonianCycles -a < ../countingHamiltonianCyclesAlgorithm1/07_4_3.g6 | ./minPicker
```

will yield the following output:
```bash
23
F}hXw
```

### hIndependentDominatingSet
This folder contains an algorithm ("findGHPairs") that can be used to find pairs (G,h), where G is an undirected simple graph and h is a hamiltonian cycle of the complement of G, such that there does not exist a vertex set S which is both dominating relative to G and independent relative to h. It is known that the graph obtained by adding h to G contains at least two hamiltonian cycles. This folder also contains the file "all_3_regular_graphs_10_vertices.g6" which contains all 3 regular graphs on 10 vertices.

The algorithm can be invoked as follows:
```bash
./findGHPairs -a < all_3_regular_graphs_10_vertices.g6
```

and it will output all graphs G such that a hamiltonian cycle h can be added which does not have any G-indepdent h-dominating vertex set. In the example above, there are 110 such (G,h) pairs.

If one uses the verbose ("-v") flag:
```bash
./findGHPairs -a -v < all_3_regular_graphs_10_vertices.g6
```

the algorithm will also output a list of all minimal dominating sets of G, a hamiltonian cycle h, a graph6-description of G and an adjacency-matrix representation of the graph obtained by adding h to G.

### allNonAdjacentPairsHamiltonianPath
This folder contains an algorithm ("nonAdjacentVerticesHamiltonianPathChecker") that can be used to filter all graphs for which there exists a pair of non-adjacent vertices with no hamiltonian path between them from a given list of graphs. This folder also contains the file "3_regular_3_hc_10_vertices.g6", which contains a list of all 3-regular graphs on 10 vertices which have precisely 3 hamiltonian cycles.

If the following command is used:
```bash
./nonAdjacentVerticesHamiltonianPathChecker < 3_regular_3_hc_10_vertices.g6
```

The output will be empty, since there are no such graphs.  The standard error will contain:
```bash
There were 0 graphs for which there exists a non-adjacent pair of vertices with no hamiltonian path between them
Checked 3 graphs in 0.000310 seconds.
```
 
### connectivity2Filter
This folder contains a simple algorithm ("connectivity2Filter") that can be used to filter out all graphs with vertex connectivity 2 from a list of graphs. The algorithm can be invoked as follows on the list of all (not necessarily connected) 3-regular graphs on 10 vertices (there are 21 such graphs):

```bash
./connectivity2Filter < all_3_regular_graphs_10_vertices.g6
```

and it will output all graphs from this list with vertex connectivity 2 (there are 4 of them):
```bash
I?bEHow[?
I?`bM_we?
ICQRD_kQ_
ICQRD_iR?
```

### nearlyKRegularFilter
This folder contains an algorithm ("nearlyKRegularFilter") that can be used to filter nearly K-regular graphs from a given list of graphs. A nearly k-regular graph is a graph in which every vertex has degree k, except for two vertices with degree less than k. These two vertices will be called the terminals of the nearly k-regular graph. There are 4 flags that the algorithm expects:
1) the degree K
2) a 0/1-flag indicating whether there is an edge between the terminals
3) the smallest degree of a terminal
4) the largest degree of a terminal

More precisely, the program expects K, connected_flag, min_degree and max_degree as arguments.
The program receives a list of graphs as input (one graph on each line) and produces as output those graphs for which there are precisely 2 vertices with degree less than K and these nodes are either linked with an edge or not (depending on the connected_flag). One of these 2 vertices should have degree min_degree, while the other one has degree max_degree.

The folder also contains a file "all_connected_graphs_6_vertices.g6", which contains a list of all connected graphs on 6 vertices (there are 112 of them). If the following command is invoked:

```bash
./nearlyKRegularFilter 4 1 3 3 < all_connected_graphs_6_vertices.g6
```

the expected output will be a list of all nearly 4-regular graphs on 6 vertices such that there is an edge between the terminals and the terminals have degree 3. There is one such graph:
```bash
E]yw
```

## CERTIFICATES
### graphCertificates
This folder contains the certificates that can be used to independently verify the claims that were made in the paper (e.g. graph X has Y hamiltonian cycles). This folder contains the following subfolders:
1) h_n_k: this subfolder contains a list of all hamiltonian k-regular graphs on n vertices having h_n_k hamiltonian cycles (for the values of n and k mentioned in the paper).
2) h_n_superindex_2_k: this subfolder contains a list of all hamiltonian k-regular graphs on n vertices with vertex connectivity 2 having h_n_superindex_2_k hamiltonian cycles (for the values of n and k mentioned in the paper).
3) graphHamiltonianCyclePairs: this subfolder contains a list of graphs G such that there is a pair (G,h), where G is an undirected simple graph and h is a hamiltonian cycle of the complement of G, such that there does not exist a vertex set S which is both dominating relative to G and independent relative to h. By adding h to G, we constructed (infinite families of) 5- and 6-regular graphs G with a hamiltonian cycle h that does not a vertex set S which is dominating relative to G-h and independent relative to h. 

The lists of the graphs G-h (i.e. 3- and 4-regular graphs) can be found in "5-regular/graphListsKMinus2Regular" ("6-regular/graphListsKMinus2Regular"), whereas a list of all minimal dominating sets of G, a hamiltonian cycle h, a graph6-description of G and an adjacency-matrix representation of the graph obtained by adding h to G can be found in "5-regular/graphsCyclesAndMinimalDominatingSets" ("6-regular/graphsCyclesAndMinimalDominatingSets").
