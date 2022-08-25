#ifndef BITSETCHOOSER
#define BITSETCHOOSER

#define USE_64_BIT

#ifdef USE_64_BIT
	#include "bitset64Vertices.h"
	#define MAXVERTICES 64

#elif defined(USE_128_BIT)
	#include "bitset128Vertices.h"
	#define MAXVERTICES 128

#elif defined(USE_128_BIT_ARRAY)
	#include "bitset128VerticesArray.h"
	#define MAXVERTICES 128

#elif defined(USE_192_BIT_ARRAY)
	#include "bitset192VerticesArray.h"
	#define MAXVERTICES 192

#elif defined(USE_256_BIT)
	#include "bitset256Vertices.h"
	#define MAXVERTICES 256

#elif defined(USE_256_BIT_ARRAY)
	#include "bitset256VerticesArray.h"
	#define MAXVERTICES 256

#endif

#endif
