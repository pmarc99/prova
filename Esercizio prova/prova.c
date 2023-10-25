#include <stdio.h>
#include <omp.h> 
#include <stdlib.h>


int main(int argc, char *argv[]){
#pragma omp parallel
{
 printf ("Parallel Region executed by %d thread\n",
 omp_get_thread_num());
 if (omp_get_thread_num() == 1)
 printf(" I’m thread %d and I’m different\n",
 omp_get_thread_num());
 } //end of parallel region

}