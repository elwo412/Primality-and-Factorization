#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pari/pari.h>
#include <cmath>

// Macros for memory allocator configuration
#ifndef MY_EXTERNAL_DEFINE
#define FASTPRECISION 4
#endif

// Allocation type
typedef enum n_type
{
    COMPOSITE = 1,
    PRIME = 0,
};

// APIs
bool initprimetest(unsigned int n);
int aks(GEN n);
GEN smallestR(GEN n);
int divisibility_check(GEN n, GEN r);
int main(int argc, char* argv[]);

#endif