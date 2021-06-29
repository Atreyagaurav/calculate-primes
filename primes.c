#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "primes.h"

void init(){
  char *filename,*dirname;
  FILE *fp;
  int n = 0;
  filename = get_cache_file_name();
  if (access(filename, F_OK) == -1){
    fprintf(stderr,"WARNING\tCache file (%s) doesn't exist. creating\n"
	   "Use generate_cache(length) function to generate cache. "
	   "Most functions are not usable without it.\n",filename);
    dirname = get_cache_file_dir();
    if(access(dirname,F_OK) == -1){
      fprintf(stderr,"WARNING\tData Directory (%s) doesn't exist. Creating\n",dirname);
      mkdir(dirname,0700);
      free(dirname);
    }
    fp = fopen(filename,"wb");
    fwrite(&n,sizeof(int),1,fp);
    fclose(fp);
  }
  free(filename);
  return;
  }

char* get_cache_file_name(){
  /* since it uses malloc, the name must be cleared */
  char *path;
  path = malloc(strlen(getenv("HOME")) + strlen(PRIME_FILE) + 2);
  sprintf(path,"%s/%s",getenv("HOME"), PRIME_FILE);
  return path;
}

char* get_cache_file_dir(){
  char *path;
  path = malloc(strlen(getenv("HOME")) + strlen(DATA_DIR) + 2);
  sprintf(path,"%s/%s",getenv("HOME"), DATA_DIR);
  return path;
}


FILE* open_primes_file(char * mode){
  FILE * fp;
  char* name;
  name = get_cache_file_name();
  fp = fopen(name, mode);
  free(name);
  return fp;
}

void calc_primes(int *nums, int n) {
  int i, j, flag, num;
  i = 1;
  *nums = 2;
  num = 3;
  while (i <= n) {
    flag = 1;
    for (j = 0; j < i; j++) {
      if ((*(nums + j)) * (*(nums + j)) > num) {
        break;
      }
      if (num % (*(nums + j)) == 0) {
        flag = 0;
        break;
      }
    }
    if (flag) {
      *(nums + i++) = num;
    }
    num += 2;
  }
}


int generate_cache(int n) {
  /* in case it is called after someone deleted the files */
  init();
  int *nums;
  FILE *fp;
  nums = malloc(n * sizeof(int));
  calc_primes(nums, n);
  fp = open_primes_file("wb");
  fwrite(&n, sizeof(int), 1, fp);
  fwrite(nums, sizeof(int), n, fp);
  fclose(fp);
  free(nums);
  return 1;
}

int cache_length(){
  init();
  int len;
  FILE *fp;
  fp = open_primes_file("rb");
  fread(&len, sizeof(int), 1, fp);
  fclose(fp);
  return len;
}

int nth_prime(int n) {
  int val, len;
  FILE *fp;
  fp = open_primes_file("rb");
  fread(&len, sizeof(int), 1, fp);
  if (len <= n) {
    return 0;
  }
  fseek(fp, n * sizeof(int), SEEK_CUR);
  fread(&val, sizeof(int), 1, fp);
  fclose(fp);
  return val;
}

int next_prime(int n) {
  int val, ith, len, i;
  FILE *fp;
  val = 0;
  fp = open_primes_file("rb");
  fread(&len, sizeof(int), 1, fp);
  for(i=0; i < len; i++){
    fread(&ith, sizeof(int), 1, fp);
    if (ith > n){
      val = ith;
      break;
    }
  }
  fclose(fp);
  return val;
}

int previous_prime(int n) {
  int val, prev, ith, len, i;
  FILE *fp;
  val = 0;
  prev = 0;
  fp = open_primes_file("rb");
  fread(&len, sizeof(int), 1, fp);
  for(i=0; i < len; i++){
    fread(&ith, sizeof(int), 1, fp);
    if (ith >= n){
      val = prev;
      break;
    }
    prev = ith;
  }
  fclose(fp);
  return val;
}

int fill_primes(int *nums, int n) {
  FILE *fp;
  int len;
  fp = open_primes_file("rb");
  fread(&len, sizeof(int), 1, fp);
  if (len < n) {
    return 0;
  }
  fread(nums, sizeof(int), n, fp);
  fclose(fp);
  return 1;
}

int is_prime(int num){
  FILE * fp;
  int i, len;
  fp = open_primes_file("rb");
  fread(&len, sizeof(int), 1, fp);
  int* nums;
  nums = malloc(len*sizeof(int));
  fill_primes(nums,len);
  for (i=0; i < len; i++){
    if (num == *(nums + i)){
      return 1;
    }else if (num < *(nums +i)){
      return 0;
    }
  }
  return -1;
}

int smallest_prime_factor(int num){
  int i, len, fac;
  int* nums;
  len = cache_length();
  nums = malloc(len*sizeof(int));
  fill_primes(nums,len);
  for (i=0; i<len; i++){
    if ((num % nums[i]) == 0){
      fac = nums[i];
      free(nums);
      return fac;
    }
  }
  free(nums);
  return -1;
}

void prime_factors(int num, int** fac_list, int* num_factors){
  int f,n=0;
  int *_fac_list = NULL;
  while(num != 1){
    f = smallest_prime_factor(num);
    _fac_list = realloc(_fac_list, (n+1)*sizeof(int));
    _fac_list[n] = f;
    num = (int)(num / f);
    n++;
  }
  *num_factors = n;
  *fac_list = _fac_list;
  return;
}

/* void factors(int num,  int** fac_list, int* num_factors){ */
/*   int i, n=0; */
/*   int *_fac_list = NULL; */
/*   for(i=1; i<=num; i++){ */
/*     if (num % i == 0){ */
/*     _fac_list = realloc(_fac_list, (n+1)*sizeof(int)); */
/*     _fac_list[n] = i; */
/*     n++; */
/*     } */
/*   } */
/*   *num_factors = n; */
/*   *fac_list = _fac_list; */
/*   return; */
/* } */

void print_arr(int *arr, int len){
  int i;
  for(i=0; i<len; i++){
    printf("%d, ", *(arr+i));
  }
  printf("\n");
}


void factors(int num,  int** fac_list, int* num_factors){
  int i, n=1, *pf, pfn, count=0, last=0, last_pos=0;
  prime_factors(num, &pf, &pfn);
  int *_pf_count = malloc(pfn * sizeof(int));
  for(i=0; i<pfn; i++){
    if (*(pf+i) == last){
      count++;
    }else{
      *(_pf_count+last_pos) = count;
      last_pos = i;
      n *= (count + 1);
      last = *(pf+i);
      count = 1;
    }
  }
  *(_pf_count+last_pos) = count;
  n *= (count + 1);
  int *_fac_list =  malloc(n*sizeof(int));
  for (i=0; i<n; i++){
    *(_fac_list + i) = 1;
  }
  
  int prod;
  int j, k, l;
  int batch, size, repeat=1;
  for(i=0; i<pfn;){
    size = *(_pf_count + i)+1;
    batch = n / (size * repeat);
    /* printf("b=%d, s=%d, r=%d\n", batch, size, repeat); */
    for(j=0; j<batch; j++){
      prod = *(pf+i);
      for(k=1; k < size; k++){
	for (l=0; l<repeat; l++){
	  *(_fac_list + (j * size + k)*repeat +l) *= prod;
	}
	/* print_arr(_fac_list, n); */
	prod *= *(pf+i);
      }
    }
    repeat *= size;
    i += *(_pf_count + i);
  }
  /* int *_fac_list =  malloc(n*sizeof(int)); */
  /* n=0; */
  /* for(i=1; i<=num; i++){ */
  /*   if (num % i == 0){ */
  /*     _fac_list[n] = i; */
  /*     n++; */
  /*   } */
  /* } */
  *num_factors = n;
  *fac_list = _fac_list;
  return;
}



/* int main(int argc, char *argv[]) { */
/*   int * fac; */
/*   int l; */
/*   int i; */
/*   factors(360, &fac, &l); */
/*   printf("factors\n"); */
/*   for (i=0;i<l;i++){ */
/*     printf("%d,",fac[i]); */
/*   } */
/*   printf("\n"); */
/*   return 0; */
/* } */
