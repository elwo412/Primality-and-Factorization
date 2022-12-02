#include "main.h"
using namespace std;

GEN GEN2;
GEN GEN3;
GEN GEN5;

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
	//findsmallestR()
	
	//(step 5) Can run equalities, 'a' in the range described, in parallel!!

	return -1;
}


int smallestR(GEN n){
	ltop = avma;
	GEN log2n_val = gdiv(glog(n, FASTPRECISION), constlog2(FASTPRECISION));
	GEN max_k = mpfloor(gpow(log2n_val, GEN2, FASTPRECISION));
	GEN max_r = gmax(GEN3, mpceil(gpow(log2n_val, GEN5, FASTPRECISION)));
	//pari_printf("max_k: %Ps | max_r: %Ps\n", max_k, max_r);
	int next_r = 1;
	int r, k;

	//printf("|> STACK SIZE: %d\n", (top-avma)/sizeof(long));
	
	// > Start r at log2(n)^2
	for (r = 2; next_r && cmpis(max_r, r) > 0; r++){
		pari_sp btop = avma;
		next_r = 0;
		for (k=1; !next_r && cmpis(max_k, k) >= 0; k++){
			pari_sp btop = avma;
			GEN k_t_INT = stoi(k);
			GEN k_bit_vector = binary_zv(k_t_INT); //(converts t_INT to bit vector)
			// GEN total;
			// iterate over each bit b_i in vector
			// 		if (b_i == 1) total *= n^(2^i)
			// #(finally...)
			// GEN modded_total = mod(total, r)
			next_r = equalis(Fp_pow(n, k_t_INT, stoi(r)), 1) || equalis(Fp_pow(n, k_t_INT, stoi(r)), 0) ;  //Fp_pow may be a huge time sink
			if (gc_needed(btop, 7))
			  avma = btop;
			//gmodgs(gpowgs(n, 200), 1429);
		}
		if (gc_needed(btop, 7))
	      avma = btop;
	}
	r--;

	//printf("Final R val: %d\n", r);
	avma = ltop;
	return r; //CHANGE (placeholder for now)
}

/* //UNGODLY SLOW
GEN smallestR(GEN n){
	ltop = avma;
	GEN lg2 = gdiv(glog(n, FASTPRECISION), glog(gen_2, FASTPRECISION));
	GEN max_k = gfloor(gsqr(lg2));
	GEN max_r = gmaxsg(3, gceil(gpowgs(lg2, 5)));
	GEN nextR = gen_1;
	GEN r = gen_2;
	GEN k = gen_1;
	{
	  pari_sp btop = avma;
	  GEN i = gen_0;
	  for (i = gen_2; gcmp(i, max_r) <= 0; i = gaddgs(i, 1))
	  {
	    r = i;
	    if (gequal0(nextR) || (gcmp(i, max_r) >= 0))
	      break;
	    nextR = gen_0;
	    {
	      pari_sp btop = avma;
	      GEN j = gen_0;
	      for (j = gen_2; gcmp(j, max_k) <= 0; j = gaddgs(j, 1))
	      {
	        k = j;
	        if (gequal1(nextR) || (gcmp(j, max_k) > 0))
	          break;
	        nextR = stoi(gequal1(gmod(gpow(n, j, FASTPRECISION), i)) || gequal0(gmod(gpow(n, j, FASTPRECISION), i)));
	        if (gc_needed(btop, 1))
	          gerepileall(btop, 3, &nextR, &k, &j);
	      }
	    }
	    if (gc_needed(btop, 1))
	      gerepileall(btop, 4, &nextR, &r, &k, &i);
	  }
	}
	r = gsubgs(r, 1);
	pari_printf("%Ps\n", r);
	gerepileall(ltop, 7, &n, &lg2, &max_k, &max_r, &nextR, &r, &k);
	return r;
} */

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
	    //printf("%s: %d\n", n_str, aks(n));
	    printf("\n%s: %d", n_str, smallestR(n));
	    //printf("Fully proven answer: %d\n", isprime(n));
	}
    printf("Total Elapsed Time: %f\n", (double)(clock() - t_start) / (double)CLOCKS_PER_SEC);
    fclose(fp);
    pari_close();
	return 0;
}
