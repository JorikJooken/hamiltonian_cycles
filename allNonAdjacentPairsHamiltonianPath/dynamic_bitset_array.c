#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <inttypes.h>
#include <stdlib.h>
#include "bitset.h"
#include "dynamic_bitset_array.h"

void initArray(Array *a, size_t initialSize) {
  a->array = malloc(initialSize * sizeof(bitset));
  a->used = 0;
  a->size = initialSize;
}

void insertArray(Array *a, bitset element) {
  // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size 
  if (a->used == a->size) {
    a->size *= 2;
    a->array = realloc(a->array, a->size * sizeof(bitset));
  }
  a->array[a->used++] = element;
}

void freeArray(Array *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}
