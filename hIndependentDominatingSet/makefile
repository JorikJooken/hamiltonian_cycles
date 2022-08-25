compiler=gcc
flags=-std=gnu11 -march=native -Wall -Wno-missing-braces -O3

all: findGHPairs

findGHPairs: findGHPairs.c read_graph/readGraph6.c hamiltonicityMethods.c dynamic_bitset_array.c bitset.h
	$(compiler) -o findGHPairs findGHPairs.c read_graph/readGraph6.c dynamic_bitset_array.c hamiltonicityMethods.c $(flags)


clean:
	rm findGHPairs

