compiler=gcc
flags=-std=gnu11 -march=native -Wall -Wno-missing-braces -O3

all: nonAdjacentVerticesHamiltonianPathChecker

nonAdjacentVerticesHamiltonianPathChecker: nonAdjacentVerticesHamiltonianPathChecker.c read_graph/readGraph6.c hamiltonicityMethods.c bitset.h
	$(compiler) -o nonAdjacentVerticesHamiltonianPathChecker nonAdjacentVerticesHamiltonianPathChecker.c read_graph/readGraph6.c hamiltonicityMethods.c $(flags)

clean:
	rm nonAdjacentVerticesHamiltonianPathChecker
