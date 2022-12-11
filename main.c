#include "main.h"
using namespace std;

GEN GEN2;
GEN GEN3;
GEN GEN5;

GEN log2n_val;

pari_sp ltop, top;

static unsigned long long max_thread = 0;
pthread_mutex_t mutex_;

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
	ltop = avma;
	GEN bound = gel(n, 1);

	//check if number is perfect power (step 1)
	if (Z_isanypower(n, &bound)) {
	    return COMPOSITE;
	}

	// (step 2) note: if r and n are not coprime, then skip this r
	GEN r = smallestR(n);

	// (step 3)
	if (divisibility_check(n, r)) return COMPOSITE;

	// (step 4) //only relevant when n <  5690034
	//if (gcmp(n,r) < 1) return COMPOSITE; // <-- seems to fail for small primes

	//clean up stack
	gerepileall(ltop, 3, &n, &r, &log2n_val);
	
	//(step 5) Can run equalities, 'a' in the range described, in parallel!!
	//Note: all equalities over range must be true, otherwise composite
	return step5_rename(n,r);
}

void *compute_equality(void *args) {
	GEN F;
	GEN *N = (GEN *)pari_thread_start((struct pari_thread*) args);
	F = stoi(0);
	//pari_printf("%Ps \n", N[2]); //ISSUE IN THE STRUCT
	GEN p = FpXQ_pow(gadd(N[2], N[0]), N[3], N[1], N[3]);
	GEN p_2 = gadd(FpXQ_pow(N[2], N[3], N[1], N[3]), N[0]);
	//pthread_mutex_unlock(&mutex_);
	
	// Check if p and p_2 are equal
	int res = gequal(p, p_2);
	if (!res)
		F = stoi(COMPOSITE);
	else F = stoi(PRIME);
	//printf("PASSED\n");
	//free(arg_package);
	pari_thread_close();
	return (void *)F;
}


int step5_rename(GEN n, GEN r){
	GEN bound = mpfloor(gmul(gsqrt(eulerphi(r), DEFAULTPREC),log2n_val));
	GEN a = stoi(1);
	GEN x = pol_x(0);

	GEN q = gsub(FpX_powu(x, itos(r), n), stoi(1));
	GEN pp2 = NULL;
	GEN p_2 = FpXQ_pow(x, stoi(1), q, n);

	while (cmpii(a, bound) < 1){
		GEN offset = gen_1;
		GEN n_t = n;
		GEN p;
		p = FpXQ_pow(gadd(x, a), stoi(1), q, n);
		/*
		while (!gequal0(n_t)){
			if (gequal1(n_t)){
				//pari_printf("res %Ps\n", gsub(n, gpow(gen_2,mpfloor(log2n_val), DEFAULTPREC)));
            	p = FpXQ_mul(p,FpXQ_pow(gadd(x, a), gsub(n,offset), q, n), q, n);
            	if (!pp2) pp2 = FpXQ_mul(p_2,FpXQ_pow(x, gsub(n,offset), q, n), q, n);

            	p_2 = gadd(pp2, a);
            	break;
            	//p_2 = gadd(FpXQ_mul(p_2, x_t, q, n), a);
			}
            p = FpXQ_pow(p, gen_2, q, n); //RgXQ_powu(p, 2, q);
            if (!pp2) p_2 = FpXQ_pow(p_2, gen_2, q, n); //RgXQ_powu(p, 2, q);
            //p_2 = gadd(FpXQ_mul(x_t, x_t, q, n), a);
            //pari_printf("n_t: %Ps\n", n_t);
            //pari_printf("p: %Ps\n", p);
			n_t = mpshift(n_t, -1);
			offset = gmul(offset,gen_2);
			//pari_printf("n_t: %Ps\n", n_t);
		}*/
		//pari_printf("\np: %Ps\n", p);
		p = FpXQ_pow(gadd(x, a), n, q, n); //SUCCESSIVE SQUARING USING: FpXQ_mul?
		//pari_printf("p_2: %Ps\n", p_2);
		p_2 = gadd(FpXQ_pow(x_t, n, q, n), a);

		//raise(SIGINT);
		
		// Check if p and p_2 are equal
		int res = gequal(p, p_2);
		if (!res)
			return COMPOSITE;
		a = gaddgs(a, 1);
	}

	/*
	//threaded approach
	int count = 0;
	int resultsarray[itos(bound)];
	pthread_t _compute_tid[100];
	struct pari_thread _pari_t[100];

	//pthread_mutex_init(&mutex_, NULL);

	for (int i = 0; i < 2; ++i) { //max_thread
	    GEN *arg_package = (GEN *)malloc(sizeof(GEN)*4);
	    arg_package[0] = a;
	    arg_package[1] = q;
	    arg_package[2] = x;
	    arg_package[3] = n;
	    a = gaddgs(a, 1);
	    pari_thread_alloc(&_pari_t[i],	8000000000, (GEN)arg_package);
	    if (pthread_create(&_compute_tid[i], NULL, &compute_equality, (void*)&_pari_t[i])){
	    	raise(SIGUSR1);
	    }


	    //arg_package->results = &resultsarray[i];

	    //simple_struct *package = (simple_struct *)malloc(sizeof(simple_struct));
	    //package->t = (int *)malloc(sizeof(int));
	    //*package->t = i;
	    //if(pthread_create(&_compute_tid[i], NULL, compute_equality, arg_package)) {
	    //    free(arg_package);
	    //    raise(SIGUSR1);
	    //    //goto error_handler;
		//}
		//printf("CREATED THREAD\n");
	}
	for (int i = 0; i < 100; ++i) {
	    pthread_join(_compute_tid[i], NULL);
	    pari_thread_free(&_pari_t[i]);
	}
	//pthread_mutex_destroy(&mutex_);

	*/

	/* //MVP
	while (cmpii(a, bound) < 1){
		GEN p = FpXQ_pow(gadd(x, a), n, q, n); //SUCCESSIVE SQUARING USING: FpXQ_mul?
		GEN p_2 = gadd(FpXQ_pow(x, n, q, n), a);
		
		// Check if p and p_2 are equal
		int res = gequal(p, p_2);
		if (!res)
			return COMPOSITE;
		a = gaddgs(a, 1);
	}
	*/
	
	return PRIME;
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

//for finding max threads
void* inc_thread_nr(void* arg) {
    (void*)arg;
    pthread_mutex_lock(&mutex_);
    max_thread ++;
    pthread_mutex_unlock(&mutex_);
    sleep(300000);
}

void _maxthread(){
	int err;
	int cnt = 0;

	pthread_t pid[1000000];

	pthread_mutex_init(&mutex_, NULL);

	while (cnt < 1000000) {

	    err = pthread_create(&pid[cnt], NULL, inc_thread_nr, NULL);
	    if (err != 0) {
	        break;
	    }
	    cnt++;
	}

	pthread_join(pid[cnt], NULL);

	pthread_mutex_destroy(&mutex_);
}
 
int main(int argc, char* argv[])
{
	//_maxthread();
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
