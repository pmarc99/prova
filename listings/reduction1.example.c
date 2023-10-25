#include <stdio.h>
#include <mpi.h>
double vsum( double * vector, int N ){
   double sum = 0;
   for( int i=0; i<N; i++){
      sum += vector[i];
   }
   return sum;
}

double vmax( double * vector, int N ){
   double max = 0;
   for( int i=0; i<N; i++){
      if( vector[i] > max ){
         max = vector[i];
      }
   }
   return max;
}


int main(int argc, char** argv) {
   int n_numbers = 1024;
   int rank;
   double vector[n_numbers];
   double sum, max;
   double my_first_number;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   // Each rank will have n_numbers numbers,
   // starting from where the previous left off
   my_first_number = n_numbers*rank;
   // Generate a vector
   for( int i=0; i<n_numbers; i++){
      vector[i] = my_first_number + i;
   }
   //Find the sum and print
   sum = vsum( vector, n_numbers );
   printf("The sum of the numbers is %f\n", sum);
   //Find the maximum and print
   max = vmax( vector, n_numbers );
   printf("The largest number is %f\n", max);
   // Call finalize at the end
   return MPI_Finalize();
}
