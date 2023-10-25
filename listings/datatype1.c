#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


#define ROWDIM 4
#define COLDIM 7
int main(int argc, char ** argv)
{
  int rank, size;
  int A[ROWDIM][COLDIM], row[COLDIM], col[ROWDIM];
  MPI_Datatype row_type, col_type;
  // Initialize MPI environment here
  MPI_Init(&argc, &argv);                                                     
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);                                       
  MPI_Comm_size(MPI_COMM_WORLD, &size); 

  MPI_Type_contiguous (COLDIM, MPI_INT, &row_type); // Create row_type
  MPI_Type_vector (ROWDIM, 1, COLDIM, MPI_INT, &col_type); // Create col_type
  MPI_Type_commit (&row_type);
  MPI_Type_commit (&col_type);

  if (rank == 0){ // rank 0 inits the matrix

    printf("A[][]= \n");
    for (int i = 0; i < ROWDIM; i++) {
      for (int j = 0; j < COLDIM; j++) {
	A[i][j] = 1 + j + COLDIM*i;
	printf("%2d  ", A[i][j]);
      }
      printf("\n");
    }
  }
  // rank 0 sends one row and one column to rank 1
  // rank 1 receives
  int irow = 2; // Index of the row we want to send
  int jcol = 1; // Index of the col we want to send
  if (rank == 0){
    printf ("Sending row = %d, col = %d\n",irow, jcol);
    MPI_Ssend (&(A[irow][0]), 1, row_type, 1, 10, MPI_COMM_WORLD);
    MPI_Ssend (&(A[0][jcol]), 1, col_type, 1, 11, MPI_COMM_WORLD);
  }else{
    MPI_Recv (row, COLDIM, MPI_INT, 0, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv (col, ROWDIM, MPI_INT, 0, 11, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf ("Row = ");
    for (int j = 0; j < COLDIM; j++)
      printf ("%2d  ", row[j]);
    printf ("\n");
    printf ("Column = ");
    for (int i = 0; i < ROWDIM; i++)
      printf ("%2d ", col[i]);
    printf ("\n");
  }
  MPI_Type_free(&row_type);
  MPI_Type_free(&col_type);
  MPI_Finalize();
}
