#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[])
{
  int rank, size;
  MPI_Init(&argc, &argv); //init MPI environment
  //get rank and size of communicator
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  printf("Hello! I am rank # %d of %d processes\n", rank,size);

  MPI_Finalize(); //Terminate MPI execution env.
  exit(EXIT_SUCCESS);
}
