/* file primes.i */
%module primes

%{
#define SWIG_FILE_WITH_INIT
#include "primes.h"
%}

%inline %{
  /* Create any sort of [size] array */
  int *int_array(int size) {
    return (int *) malloc(size*sizeof(int));
  }
%}

void init();

%include "numpy.i"
%init %{
import_array();
init();
%}

%apply (int* ARGOUT_ARRAY1, int DIM1) {(int *nums, int n)}
%apply (int** ARGOUTVIEWM_ARRAY1, int* DIM1){(int** fac_list, int* num_factors)}

char* get_cache_file_name();

char* get_cache_file_dir();

void calc_primes(int *nums, int n);

int generate_cache(int n);

int cache_length();

int nth_prime(int n);

int next_prime(int n);

int previous_prime(int n);

int fill_primes(int *nums, int n);

int is_prime(int num);

int smallest_prime_factor(int num);

void prime_factors(int num, int **fac_list, int *num_factors);

void factors(int num, int** fac_list, int* num_factors);

