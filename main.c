#include <iostream>
 
#include <pari/pari.h>
#include <cmath>
 
using namespace std;

int option = -1; //EDIT HERE TO SEE SOLUTIONS

int simpleprimetest(GEN *num){
	pari_printf("Pari Num: %Ps\n", *num);
	cmpis()

	return 1;
}
 
int main(int argc, char* argv[])
{
	pari_init(8000000,500000);
	char n_str[1000];
    FILE* fp = fopen(argv[1], "r");
    clock_t t_start = clock();
    while (fscanf(fp, "%s", n_str) != EOF) {
	    printf("%s: aks output\n", n_str);
	    GEN n = strtoi(n_str);
	    simpleprimetest(&n);
	}
    printf("Total Elapsed Time: %f\n", 
	   (double)(clock() - t_start) / (double)CLOCKS_PER_SEC);
    fclose(fp);
    pari_close();
	return 0;
}