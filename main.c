#include "main.h"
using namespace std;

GEN GEN2;
GEN GEN3;
GEN GEN5;

GEN log2n_val;

pari_sp ltop, top;

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
int aks(GEN n) {
	GEN bound = gel(n, 1);

	//check if number is perfect power (step 1)
	if (Z_isanypower(n, &bound)) {
	    return COMPOSITE;
	}

	// (step 2) note: if r and n are not coprime, then skip this r
	GEN r = smallestR(n);

	// (step 3) //only relevant when n <  5690034
	if (divisibility_check(n, r)) return COMPOSITE;

	// (step 4)
	if (gcmp(n,r) < 1) return COMPOSITE;
	
	//(step 5) Can run equalities, 'a' in the range described, in parallel!!
	//Note: all equalities over range must be true, otherwise composite
	log2n_val

	return -1;
}

int divisibility_check(GEN n, GEN r) {
	GEN bound = gmin(gsubgs(n, 1), r);
	GEN a = GEN2;
	while (cmpii(a, bound) < 1){
		if (dvdii(n,a)) return COMPOSITE;
		a = gaddgs(a, 1);
	}
	return 0;
}

GEN smallestR(GEN n){
	log2n_val = gdiv(glog(n, DEFAULTPREC), constlog2(DEFAULTPREC));

	GEN r = mpceil(gpowgs(log2n_val,2));
	GEN k = GEN2;
	GEN max_k = mpfloor(gpow(log2n_val, GEN2, DEFAULTPREC));
	GEN max_r = gmax(GEN3, mpceil(gpow(log2n_val, GEN5, DEFAULTPREC)));

	GEN modd;

	r = gaddgs(r, 1);

	while (cmpii(k, max_k) < 1){
		if (gequal1(ggcd(n, r))){
			modd = gmodulo(n, r);
			k = znorder(modd, NULL);
		}
		//pari_printf("k=%Ps\n", k);
		r = gaddgs(r, 1);
		//if (cmpii(r, max_r) == 1) break; // <-- is this needed??
	}
	if (cmpii(r, max_r) == 1) { printf("Error: r > log2(n)^5\n"); raise(SIGINT);}
	r = gsubgs(r, 1);

	return r;
}

//ulong tridiv_bound(GEN n) returns the trial division bound used by Z_factor(n). !!Useful!!
 
int main(int argc, char* argv[])
{
	pari_init(7000000000, 10000000000);
	top = avma;
	GEN2 = stoi(2);
	GEN3 = stoi(3);
	GEN5 = stoi(5);
	char n_str[1000];
    FILE* fp = fopen(argv[1], "r");
    clock_t t_start = clock();
    while (fscanf(fp, "%s", n_str) != EOF) {
	    GEN n = strtoi(n_str);
	    printf("%s: %d\n", n_str, aks(n));
	    //n = stoi(89);
	    //printf("Fully proven answer: %d\n", isprime(n));
	}
    printf("Total Elapsed Time: %f\n", (double)(clock() - t_start) / (double)CLOCKS_PER_SEC);
    fclose(fp);
    pari_close();
	return 0;
}
