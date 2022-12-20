![Platform] [![pariv]][pari-url] [![NTL]][ntl-url] [![GMP]][gmp-url] 

<!-- PROJECT LOGO -->
<br />
<div align="center">

  <h3 align="center">Primality testing and Factorization Final Project</h3>

  <p align="center">
    <a href="https://drive.google.com/file/d/16_xObW7kn6OI4o_ZNZQqleFgk97sVQ6r/view?usp=share_link">Full Demo</a>
    <a href="https://drive.google.com/file/d/1HiVDiCXncjqIyc-thjU8CRO-01mWbdE3/view?usp=share_link">Quick Demo</a>
  </p>
</div>

<!-- ABOUT THE PROJECT -->
## About The Project

The AKS primality test is a reliable method for determining whether a given number N is prime or composite. It is a polynomial time algorithm with a predictable and efficient runtime, and has been rigorously proven to be accurate in all cases. If N is found to be composite using the AKS primality test, the number is then factored into its prime factorization using a combination of trial division, fermat's method, and the quadratic sieve. Trial division is used for small prime factors, the quadratic sieve is used for larger prime factors, and fermat's method is used for extreme cases.

Trial division is used for finding small prime factors, where N is divided by all possible prime factors up to its square root. For larger prime factors, the quadratic sieve works by sieving for integers that can be factored using a predetermined factor base. It then sets up a matrix using these integers and their corresponding congruences, and solves the matrix using linear algebra techniques to find a congruence of squares modulo N. This congruence can then be used to obtain a factorization of N. Finally, for extreme cases where the other methods are not efficient, fermat's method is used. This involves using the fermat's little theorem to quickly check for potential prime factors that would otherwise be challenging to find by the previous methods. By using these methods in combination, the prime factorization of N can be accurately and efficiently determined.

**Features:**
* Partially Parrellized Primality Test
* Factorization of Composites using Trial Division and Fermat's Method

**To do:**
* Fully implement QS and verify its correctness

### Built With

C/C++, Pari GP, NTL, GMP, Gfx2 (not necessary)




<!-- GETTING STARTED -->
## Getting Started

<b>Prerequisite:</b> Build all dependencies from source from the links above

### Basic Usage

_Below are some examples on how to interface with the program._
<br />
<br />
1.0. Compile the program
   ```sh
   make clean; make cppversion
   ```
2.1. Run main routine
   ```sh
   ./main
   ```
2.2. Run against test file
   ```js
   ./main first1000.txt
   ```




<!-- MARKDOWN LINKS & IMAGES -->
[Platform]: https://img.shields.io/badge/Platform-linux--64-blue
[pariv]: https://img.shields.io/badge/PARIGP-v2.15.1-brightgreen
[pari-url]: https://pari.math.u-bordeaux.fr/download.html
[GMP]: https://img.shields.io/badge/GMP-v6.2.0-brightgreen
[gmp-url]: https://gmplib.org/#DOWNLOAD
[NTL]: https://img.shields.io/badge/NTL-v11.5.1-brightgreen
[ntl-url]: https://libntl.org/doc/tour-gmp.html
