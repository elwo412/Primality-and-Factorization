#ifndef INTERFACE_H
#define INTERFACE_H

//#include <stdio.h>
//#include <stdlib.h>
//#include <iostream>
#include <pari/pari.h>
//#include <cmath>

// Macros
#ifndef MY_EXTERNAL_DEFINE
#define FASTPRECISION 4
#define THREADS 31576
#endif


// Allocation type
typedef enum n_type
{
    COMPOSITE = 1,
    PRIME = 0,
};

typedef struct {
    GEN **a;
    GEN *x;
    GEN *q;
    GEN *n;

    int *results;
} c_arg_package;

typedef struct {
    int *t;
} simple_struct;

// APIs
int aks(GEN n);
GEN smallestR(GEN n);
int divisibility_check(GEN n, GEN r);
int step5_rename(GEN n, GEN r);
int main(int argc, char* argv[]);
void* inc_thread_nr(void* arg);
void _maxthread();

#endif