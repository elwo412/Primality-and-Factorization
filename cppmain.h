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

// Macros
#ifndef MY_EXTERNAL_DEFINE
#define FASTPRECISION 4
#define THREADS 31576
#endif

typedef struct {
    GEN **a;
    GEN *x;
    GEN *q;
    GEN *n;

    int *results;
} c_arg_package;

// APIs
int td(NTL::ZZ n, long bound);
int aks(NTL::ZZ n, char *n_pari);
int step5(NTL::ZZ n, long r, long bound);

#endif