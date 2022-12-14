#include "cmain.h"

GEN n, r;
GEN log2n_val;

pari_sp ltop, top;

static unsigned long long max_thread = 0;

GEN smallestR(GEN n){ 
	log2n_val = gdiv(glog(n, DEFAULTPREC), constlog2(DEFAULTPREC));

	r = mpceil(gpowgs(log2n_val,2));
	GEN k = gen_2;
	GEN max_k = mpfloor(gpow(log2n_val, gen_2, DEFAULTPREC));
	GEN max_r = gmax(stoi(3), mpceil(gpow(log2n_val, stoi(5), DEFAULTPREC)));

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

	GEN bound = mpfloor(gmul(gsqrt(eulerphi(r), DEFAULTPREC),log2n_val));

	return r;
}

int aks(char *n_str) {
	ltop = avma;
	n = strtoi(n_str);
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
	if (gcmp(n,r) < 1) return PRIME; // <-- seems to fail for small primes

	//clean up stack
	gerepileall(ltop, 3, &n, &r, &log2n_val);
	
	//(step 5) Can run equalities, 'a' in the range described, in parallel!!
	//Note: all equalities over range must be true, otherwise composite
	return equal_polys(n,r);
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
	//printf("\n--- %d\n", res);
	if (!res)
		F = stoi(COMPOSITE);
	else F = stoi(PRIME);
	//printf("PASSED\n");
	//free(arg_package);
	pari_thread_close();
	return (void *)F;
}


int equal_polys(GEN n, GEN r){
	GEN bound = mpfloor(gmul(gsqrt(eulerphi(r), DEFAULTPREC),log2n_val));
	GEN a = stoi(1);
	GEN x = pol_x(0);

	GEN q = gsub(FpX_powu(x, itos(r), n), stoi(1));
	//q = FpX_get_red(q, n);

	GEN return_results = gen_0;

	if (cmpii(n, stoi(1000000)) == 1){
		//threaded approach
		max_thread = 16;
		int count = 0;
		int resultsarray[itos(bound)];
		pthread_t _compute_tid[max_thread];
		struct pari_thread _pari_t[max_thread];
		GEN *arg_package;
		//pthread_mutex_init(&mutex_, NULL);
		while (cmpii(a, bound) < 1){
			for (int i = 0; i < max_thread; ++i) { //max_thread
			    arg_package = (GEN *)malloc(sizeof(GEN)*4);
			    arg_package[0] = a;
			    arg_package[1] = q;
			    arg_package[2] = x;
			    arg_package[3] = n;
			    a = gaddgs(a, 1);
			    pari_thread_alloc(&_pari_t[i],	80000000, (GEN)arg_package);
			    if (pthread_create(&_compute_tid[i], NULL, &compute_equality, (void*)&_pari_t[i])){
			    	raise(SIGUSR1);
			    }
			}

			for (int i = 0; i < max_thread; ++i) {
				GEN F1;
			    pthread_join(_compute_tid[i],(void*)&F1);
			    resultsarray[i+count*max_thread] = itos(F1);
			    pari_thread_free(&_pari_t[i]);

			    return_results = gaddgs(return_results,  resultsarray[i+count*max_thread]);
			}
			free(arg_package);
			count++;
		}

		if (return_results == 0) return COMPOSITE;

		
	}else{ //clock_t t_start = clock();
		while (cmpii(a, bound) < 1){
			GEN p, p_2;
			p = FpXQ_pow(gadd(x, a), n, q, n); //As defined in paper
			p_2 = gadd(FpXQ_pow(x, n, q, n), a); //As defined in paper
			
			// Check if p and p_2 are equal
			int res = gequal(p, p_2);
			if (!res)
				return COMPOSITE;
			a = gaddgs(a, 1);
		}
		//printf("Step 5 Time: %f | ", (double)(clock() - t_start) / (double)CLOCKS_PER_SEC);
	}
	
	return PRIME;
}

int divisibility_check() {
	GEN bound = gmin(gsubgs(n, 1), r);
	GEN a = gen_2;
	while (cmpii(a, bound) < 1){
		if (dvdii(n,a)) return COMPOSITE;
		a = gaddgs(a, 1);
	}
	return 0;
}

void print_test(){
	printf("WORKING\n");
}

