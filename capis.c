#include "cmain.h"

GEN n, r;

long smallestR(char *n_str, long *log2n_buffer){
	n = strtoi(n_str);
	GEN log2n_val = gdiv(glog(n, DEFAULTPREC), constlog2(DEFAULTPREC));

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

	*log2n_buffer = itos(bound);
	long smallestr = itos(r);

	return smallestr;
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

