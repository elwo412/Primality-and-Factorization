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

/////QS IMPLEMENTATION
//helpful functions for QS (may implement using NTL instead)
/*
GEN gprimepi_upper_bound(GEN x) as primepi_upper_bound, returns a t_REAL
long Z_issmooth(GEN n, ulong lim) returns 1 if all the prime factors of the t_INT n are less or equal to lim.
(maybe for testing) GEN Z_factor(GEN n) factors the t_INT n. The “primes” in the factorization are actually strong pseudoprimes.
GEN F2Ms_colelim(GEN M, long nbrow) returns some subset of the columns of M as a t_VECSMALL of indices, selected such that the dimension of the kernel of the matrix is preserved. The subset is not guaranteed to be minimal.
GEN FpMs_FpCs_solve_safe(GEN M, GEN B, long nbrow, GEN p) as above, but in the event that p is not a prime and an impossible division occurs, return NULL.
GEN FpM_deplin(GEN x, GEN p) returns a nontrivial kernel vector, or NULL if none exist.
GEN FpM_inv(GEN x, GEN p) returns a left inverse of x (the inverse if x is square), or NULL if x is not invertible.
GEN bezout(GEN a, GEN b, GEN *u, GEN *v), returns the GCD d of t_INTs a and b and sets u,v to the Bezout coefficients such that au + bv = d.
*/

GEN generateFactorBase(GEN n, GEN b){
	long b_bound = itos(b);
	GEN factorbase = vectrunc_init(b_bound);

	int count = 1;

	GEN a = stoi(1);

	while(cmpii(a,b) < 1) {

		// Add a to factor base if n is a quadratic residue modulo a.
		if (isprime(a) && kronecker(n, a) == 1){
			vectrunc_append(factorbase, a);
			count++;
			if (count > b_bound) break;
		}
			

		a = gaddgs(a, 1);
	}
	//pari_printf("%Ps\n", factorbase);

	return factorbase;

}

void tonelliShanks(GEN n, GEN p, t_GEN_PAIR *pair){
	if (cmpii(p, gen_2) == 0){
		pair->first = n;
		pair->second = n;
		return;
	}

	// Define Q2^S = p - 1.
	GEN Q = gsubgs(p, 1);
	GEN S = gen_0;
	while (gequal0(gmod(Q, gen_2))) {
	    Q = gdiv(Q, gen_2);
	    S = gadd(S, gen_1);
	}

	// Define z as the first quadratic non-residue modulo p.
	GEN z = gen_2;
	while (kronecker(z, p) != -1)
	    z = gadd(z, gen_1);

	// Initialize c, R, t and M.
	GEN c = Fp_pow(z, Q, p);            // c = z^Q         (mod p)
	GEN R = Fp_pow(n, gdiv((gadd(Q, gen_1)),gen_2), p);  // R = n^((Q+1)/2) (mod p)
	GEN t = Fp_pow(n, Q, p);            // t = n^Q         (mod p)
	GEN M = gcopy(S);

	// Invariant: R^2 = nt (mod p)
	while (!gequal1(gmod(t,p))) {
		ltop = avma;
	    // Find lowest 0 < i < M such that t^2^i = 1 (mod p).
	    GEN i = gen_1;
	    GEN n_i = powgi(gen_2, i);
	    while (!gequal1(Fp_pow(t, n_i, p))){
	        i = gaddgs(i, 1);
	        n_i = powgi(gen_2, i);
	    }

	   // Set b = c^2^(M - i - 1)
	    GEN im1 = gsub(M,i);
	    GEN m_subi = gsub(im1, gen_1);
	    GEN pow2msubi = powgi(gen_2, m_subi);
	    GEN b = Fp_pow(c, pow2msubi, p);

	    // Update c, R, t and M.
	    R = gmod(gmul(R, b), p);     // R = Rb (mod p)
	    t = gmod(gmul(t, gmul(b,b)), p); // t = tb^2
	    c = gmod(gmul(b, b), p);      // c = b^2 (mod p)
	    M = i;

	    // Invariant: R^2 = nt (mod p)
	}

	pair->first = R;
	pair->second = gsub(p,R);

	return;
}

void QS(){
	GEN loglogn_val = glog(log2n_val, DEFAULTPREC);
	GEN sqrtn = mpfloor(gsqrt(n, DEFAULTPREC));

	//smoothness bound (some minimum bound + ...)
	GEN B = gadd(stoi(300), mpceil(gsqrt(gmul(loglogn_val,log2n_val) , DEFAULTPREC)));

	// (step 1) data collection
	GEN factorBase = generateFactorBase(n, B);

	// (step 2) compute t_vec shanks method
	long length_factorbase = lg(factorBase)-1;
	t_GEN_PAIR indicies[length_factorbase];
	
	for (int i = 0; i < length_factorbase; i++){
		GEN p = gel(factorBase, i+1);
		GEN nmod_p = gmod(n, p);

		tonelliShanks(nmod_p, p, &indicies[i]);

		indicies[i].first = gmod(gadd(gmod(gsub(indicies[i].first, sqrtn),p),p),p);
		indicies[i].second = gmod(gadd(gmod(gsub(indicies[i].second, sqrtn),p),p),p);
		//pari_printf("%Ps %Ps\n", indicies[i].first, indicies[i].second);

	}

	// (step 3) SIEVING
	int interval_length = 65535;
	GEN intervalStart = gen_0;
	GEN intervalEnd = stoi(interval_length);
	GEN smooth = vectrunc_init(itos(B) + 25);
	GEN smooth_factorizations = vectrunc_init(itos(B) + 25);
	GEN log_approximations = zerovec(interval_length);  // Approx. 2-logarithms of a^2 - N.


	// Rough log estimates instead of full approximations.
	float log_estimate = 0;
	int n_log_estimate = 1;
	while (lg(smooth) < lg(factorBase) + 20) {
	    
	    // (step 3.1)
	    // Generate log approximations of Q = (a + sqrt(N))^2 - N in the current interval.
		GEN a = gadd(intervalStart,gen_1);
	    for (int i = 1; i < interval_length; ++i) {
	    	if (cmpsi(n_log_estimate, a) < 1) {
                GEN Q = gsub(gmul(gadd(sqrtn, a),gadd(sqrtn, a)), n);
                log_estimate = dbllog2r(Q);    // ~log_2(Q)
                n_log_estimate = n_log_estimate * 1.8 + 1;
            }
	        gel(log_approximations, i+1) = dbltor(log_estimate);
	        gaddz(gen_1, a, a);
	    }

	    
	    //(step 3.2)
	    //Sieve for numbers in the sequence that factor completely over the factor base.
	    for (int i = 0; i < lg(factorBase)-1; ++i) {
	        GEN p = gel(factorBase, i+1);
	        GEN logp = gdiv(glog(p, DEFAULTPREC), constlog2(DEFAULTPREC));
	        

	        // Sieve first sequence.
	        while (cmpii(indicies[i].first, intervalEnd) == -1) {
	        	ltop = avma;
	            gel(log_approximations, 1+itos(gsub(indicies[i].first, intervalStart))) = subrr(gel(log_approximations, 1+itos(gsub(indicies[i].first, intervalStart))), logp);
	            indicies[i].first = gadd(indicies[i].first,p);
	            gerepileall(ltop, 3, &p, &logp, &indicies[i].first);
	        }

	        if (cmpii(p, gen_2) == 0)
	            continue; // a^2 = N (mod 2) only has one root.

	        // Sieve second sequence.
	        while (cmpii(indicies[i].second, intervalEnd) == -1) {
	        	ltop = avma;
	            gel(log_approximations, 1+itos(gsub(indicies[i].second, intervalStart))) = subrr(gel(log_approximations, 1+itos(gsub(indicies[i].second, intervalStart))), logp);
	            indicies[i].second = gadd(indicies[i].second,p);
	            gerepileall(ltop, 3, &p, &logp, &indicies[i].second);
	        }
	    }
	    //printf("ITER1\n");

	     
	    // (step 3.3)
	    a = intervalStart;
	    //GEN bound_ap = gdiv(glog(gel(factorBase,lg(factorBase)-1), DEFAULTPREC), constlog2(DEFAULTPREC));
	    for (int i = 0; i < interval_length; ++i) {
	        if (1) { //gcmp(gel(log_approximations,i+1), bound_ap) == -1 //<---logapproximation must be off
	        	//printf("HERE1\n");
	            GEN Q = gsub(gmul(gadd(sqrtn, a),gadd(sqrtn, a)), n);
	            GEN factors = vectrunc_init(100);
	            //printf("HERE2\n");

	            // For each factor p in the factor base.
	            for (int j = 0; j < lg(factorBase)-1; ++j) {
	                // Repeatedly divide Q by p until it's not possible anymore.
	                GEN p = gel(factorBase, j+1);
	                while (dvdii(Q, p)) {
	                	//pari_printf("%Ps\n", Q);
	                	//pari_printf("%Ps\n", p);
	                    Q = diviiexact(Q, p);
	                    vectrunc_append(factors, stoi(j)); // The j:th factor base number was a factor.
	                    
	                }
	            }
	            if (gequal1(Q)) {
	            	//printf("ADDING ELEMENT\n");
	                // Q really was B-smooth, so save its factors and the corresponding a.
	                vectrunc_append(smooth_factorizations, factors); //need to combine vectors
	                vectrunc_append(smooth, a);
	            }
	            if (lg(smooth) >= lg(factorBase) + 20)
	                break; // We have enough smooth numbers, so stop factoring.
	        }
	        //printf("HERE0\n");
	        a = gadd(gen_1, a);
	    }
	    //raise(SIGUSR1);

	    // Move on to next interval.	    
	    intervalStart = gaddgs(intervalStart, interval_length);
	    intervalEnd = gaddgs(intervalEnd, interval_length);
	    //printf("Next Interval\n");
	}
	//pari_printf("%Ps\n", smooth);
	//pari_printf("%Ps\n", smooth_factorizations);
	printf("ENOUGH FACTORS!!!\n");

	// (step 4)
	// Construct a binary matrix (gave up here)
	printf("factorBase: %d | smof: %d\n", lg(factorBase), lg(smooth_factorizations));
	GEN matrix =  zeromatcopy(lg(factorBase), lg(smooth_factorizations)+1);
	for (int i=0; i < lg(smooth_factorizations); ++i){
		for (int j=0; j < lg(gel(smooth_factorizations, i+1)); ++j){
			gmael(matrix, i+1, j+1) = stoi(1);
		}
	}
	printf("GOOD\n");
	pari_printf("%Ps\n", matrix);

}

