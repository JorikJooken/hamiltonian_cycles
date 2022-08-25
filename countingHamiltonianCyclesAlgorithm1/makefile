compiler=gcc
flags=-std=gnu11 -march=native -Wall -Wno-missing-braces -O3

all: countHamiltonianCycles

countHamiltonianCycles: countHamiltonianCycles.c read_graph/readGraph6.c hamiltonicityMethods.c bitset.h
	$(compiler) -o countHamiltonianCycles countHamiltonianCycles.c read_graph/readGraph6.c hamiltonicityMethods.c $(flags)

clean:
	rm countHamiltonianCycles

