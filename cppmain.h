#ifndef INTERFACE_H
#define INTERFACE_H

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <NTL/ZZ_pXFactoring.h>
#include <NTL/ZZ_pEX.h>
#include <gmp.h>
#include <sstream>
#include <fstream>
#include <pari/pari.h>
#include <math.h> 

// Macros
#ifndef MY_EXTERNAL_DEFINE
#define FASTPRECISION 4
#define THREADS 31576
#endif

typedef enum mode
{
    FULL_FACTOR_N = 0,
    FILE_INPUT = 1,
};

typedef struct {
    GEN **a;
    GEN *x;
    GEN *q;
    GEN *n;

    int *results;
} c_arg_package;

// APIs
int td(NTL::ZZ n, long bound);
int step5(NTL::ZZ n, long r, long bound);
void ask_for_mode();

#endif