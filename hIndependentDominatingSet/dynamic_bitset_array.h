#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <inttypes.h>
#include <stdlib.h>
#include "bitset.h"

typedef struct {
  bitset *array;
  size_t used;
  size_t size;
} Array;

void initArray(Array *a, size_t initialSize);
void insertArray(Array *a, bitset element);
void freeArray(Array *a);
