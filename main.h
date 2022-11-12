#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pari/pari.h>
#include <cmath>

// Macros for memory allocator configuration
#ifndef MY_EXTERNAL_DEFINE
#define MEMORY_SIZE 8 * 1024 * 1024
#define HEADER_SIZE 8
#define MIN_MEM_CHUNK_SIZE 512
#define N_OBJS_PER_SLAB 64
#endif

// Allocation type
typedef enum n_type
{
    COMPOSITE = 0,
    PRIME = 1,
};

// APIs
bool initprimetest(unsigned int n);
int aks(unsigned int n);
int main(int argc, char* argv[]);

#endif