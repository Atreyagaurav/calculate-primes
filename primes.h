#include <stdlib.h>

// HOME + PRIME_FILE
#define DATA_DIR ".data"
#define PRIME_FILE DATA_DIR"/primes.dat"

#define MAX_PATH 100

void init();

char *get_cache_file_name();

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

void prime_factors(int num, int** fac_list, int* num_factors);

