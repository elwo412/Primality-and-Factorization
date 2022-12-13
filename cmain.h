#ifndef CMAIN_H
#define CMAIN_H

#include <pari/pari.h>
#include <math.h>

// Macros
#ifndef MY_EXTERNAL_DEFINE
#define FASTPRECISION 4
#define THREADS 31576
#endif

typedef enum n_type
{
    COMPOSITE = 1,
    PRIME = 0,
};

// APIs
long smallestR(char *n_str, long *log2n_buffer);
void print_test();
int divisibility_check();

#endif

