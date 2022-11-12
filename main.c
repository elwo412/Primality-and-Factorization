#include "main.h"
using namespace std;

bool initprimetest(unsigned int n)
{
	if (n < 2) return false;
	if (n < 4) return true;
	if (n % 2 == 0) return false;

	const unsigned int iMax = (int)sqrt(n) + 1;
	unsigned int i;
	for (i = 3; i <= iMax; i += 2)
		if (n % i == 0)
			return false;

	return true;
}

/**
 * General, polynomial-time, deterministic, and unconditional 
 * primality test based upon Agrawal's PRIMES is in P article
 *
 * @param {int} Number to perform aks primality test on.
 * @returns {int} Returns [0,1] indicating whether n is prime.
 */
int aks(unsigned int n) {
	GEN x = stoi(n);
	GEN bound = gel(stoi(log2(n)), 1);

	//check if number is perfect power
	if (Z_isanypower(x, &bound)) {
	    return COMPOSITE;
	}

	return -1;
}
 
int main(int argc, char* argv[])
{
	pari_init(8000000,500000);
	char n_str[1000];
    FILE* fp = fopen(argv[1], "r");
    clock_t t_start = clock();
    while (fscanf(fp, "%s", n_str) != EOF) {
	    printf("\n%s: aks input\n", n_str);
	    GEN n = strtoi(n_str);
	    printf("OUTPUT: %d\n", initprimetest(atoi(n_str)));
	    printf("AKS OUTPUT: %d\n", aks(atoi(n_str)));
	}
    printf("Total Elapsed Time: %f\n", 
	   (double)(clock() - t_start) / (double)CLOCKS_PER_SEC);
    fclose(fp);
    pari_close();
	return 0;
}