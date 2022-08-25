compiler=gcc
flags=-std=gnu11 -march=native -Wall -Wno-missing-braces -O3

all: cubic3HCHamPathChecker

cubic3HCHamPathChecker: cubic3HCHamPathChecker.c read_graph/readGraph6.c hamiltonicityMethods.c bitset.h
	$(compiler) -o cubic3HCHamPathChecker cubic3HCHamPathChecker.c read_graph/readGraph6.c hamiltonicityMethods.c $(flags)

clean:
	rm cubic3HCHamPathChecker
