#include "cppmain.h"
extern "C" {
#include "cmain.h"
}

using namespace std;
using namespace NTL;

int current_mode;
Mat<long> p_factors;
int x_dim;
const int y_dim = 2;

NTL_CLIENT

int td(ZZ n, long bound){
   //trial division
   PrimeSeq s;  // generates primes in sequence quickly
   long p;

   p = s.next();  // first prime is always 2
   while (p && p < bound-1) {
      if ((n % p) == 0) return COMPOSITE;
      p = s.next();  
   }
   return PRIME;
}

long td_factorization(ZZ n, long bound){
   //trial division
   PrimeSeq s;  // generates primes in sequence quickly
   long p;
   x_dim = 0;
   p_factors = Mat<long>();

   p = s.next();  // first prime is always 2
   while (p && p < bound) {
      int c = 1;
      while ((n % p) == 0){
         if (c == 1){ x_dim++; p_factors.SetDims(x_dim, y_dim);} 
         p_factors[x_dim-1].put(0,p);
         p_factors[x_dim-1].put(1,c);
         c++; //ha!
         n = n / p;
      }
      p = s.next();  
   }
   return p;
}

void ask_for_mode(string *buffer){
   int option;
   string n;
   cout << "FINAL PROJECT\n-----------\nOptions: 1. Run routine on single number N\n2. Run routine on file input\n\n" << endl << "Option (1|2): ";
   cin >> option;
   if (option == 2){
      current_mode = FILE_INPUT;
      cout << "\x1B[2J\x1B[H";
      cout << "File Name: ";
      cin >> n;
      cout << "\x1B[2J\x1B[H";
   } else {
      current_mode = FULL_FACTOR_N;
      cout << "\x1B[2J\x1B[H";
      cout << "Input N: ";
      cin >> n;
   }

   *buffer = n;
}

int main(int argc, char* argv[])
{
   string *inputstring = (string *)malloc(sizeof(string));
   if (!argv[1]) ask_for_mode(inputstring);
   else current_mode = FILE_INPUT;
   if (current_mode == FILE_INPUT){
      pari_init(7000000000, 10000000000);
      char n_str[1000];
      FILE *fp;
      if (!argv[1]) FILE* fp = fopen(inputstring->c_str(), "r");
      else fp = fopen(argv[1], "r");
      print_test();
      clock_t t_start = clock();
      long ln;
      while (fscanf(fp, "%s", n_str) != EOF) {
         ZZ n = conv<ZZ>(n_str);
         if (n < 1000) {ln = strtol(n_str,NULL,10); cout << "TD| " << n << ": " << ( td(n, ln)  ? "COMPOSITE" : "PRIME" ) << endl; }
         else
            cout << "AKS| " << n << ": " << (aks(n_str) ? "COMPOSITE" : "PRIME" ) << endl;
      }
      printf("Total Elapsed Time: %f\n", (double)(clock() - t_start) / (double)CLOCKS_PER_SEC);
      pari_close();
   } else {
      pari_init(7000000000, 10000000000);
      char n_str[1000];
      strcpy(n_str, inputstring->c_str());
      clock_t t_start = clock();
      int aks_result = aks(n_str);
      cout << "AKS| " << *inputstring << ": " << (aks_result ? "COMPOSITE" : "PRIME" ) << endl;
      if (aks_result == COMPOSITE){
         long ln = strtol(n_str,NULL,10);
         ZZ n = conv<ZZ>(n_str);
         cout << "Factorization| " << *inputstring << ": " << td_factorization(n, ln) << endl;
         cout << p_factors;
      }
      printf("\n\nTotal Elapsed Time: %f\n", (double)(clock() - t_start) / (double)CLOCKS_PER_SEC);
      
      pari_close();
   }

   free(inputstring);
   
   return 0;
}