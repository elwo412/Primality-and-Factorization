#include <stdio.h>
#include <pthread.h>
#include <pari/pari.h>

#define NUM_THREADS 4

// Thread function that checks if a number is composite
void* divisibility_check(void* arg)
{
    // Get the input arguments
    GEN* input = (GEN*)arg;
    GEN n = input[0];
    GEN r = input[1];

    // Compute the minimum of (n - 1) and r
    GEN bound = gmin(gsubgs(n, 1), r);

    // Start with a = 2 and check for divisibility
    GEN a = gen_2;
    while (cmpii(a, bound) < 1) {
        if (dvdii(n, a)) {
            printf("The number is composite.\n");
            return NULL;
        }
        a = gaddgs(a, 1);
    }

    printf("The number is not composite.\n");
    return NULL;
}

int main()
{
    pari_init(100000000, 2); // Initialize the pari/gp library

    pthread_t threads[NUM_THREADS];

    // Create the input arguments for each thread
    GEN input[NUM_THREADS][2] = {
        {stoi(17), stoi(20)},
        {stoi(18), stoi(20)},
        {stoi(19), stoi(20)},
        {stoi(20), stoi(20)},
    };

    // Create and start the threads
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, divisibility_check, (void*)input[i]);
    }

    // Wait for the threads to finish
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pari_close(); // Clean up the pari/gp library
    return 0;
}
 