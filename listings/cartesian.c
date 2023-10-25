#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define NDIM      2
#define NX_GLOB   16
#define NY_GLOB   8

#define VERSION   1


double **Allocate_2DdblArray(int, int);
int    **Allocate_2DintArray(int, int);
void   Show_2DdblArray(double **, int, int, const char *);
void   Show_2DintArray(int **, int, int, const char *);


/* ********************************************************************* */
double **Allocate_2DdblArray(int nx, int ny)
/*
 * Allocate memory for a double precision array with
 * nx rows and ny columns
 *********************************************************************** */
{
  int i,j;
  double **buf;

  buf    = (double **)malloc (nx*sizeof(double *));
  buf[0] = (double *) malloc (nx*ny*sizeof(double)); 
  for (j = 1; j < nx; j++) buf[j] = buf[j-1] + ny;

  return buf;
}
/* ********************************************************************* */
int **Allocate_2DintArray(int nx, int ny)
/*
 * Allocate memory for an integer-type array with
 * nx rows and ny columns
 *********************************************************************** */
{
  int i,j;
  int **buf;

  buf    = (int **)malloc (nx*sizeof(int *));
  buf[0] = (int *) malloc (nx*ny*sizeof(int)); 
  for (j = 1; j < nx; j++) buf[j] = buf[j-1] + ny;

  return buf;
}


/* ********************************************************************* */
void Show_2DdblArray(double **A, int nx, int ny, const char *string)
/*
 *********************************************************************** */
{
  int i, j;

  printf ("%s\n",string);
  printf ("------------------------------\n");
  for (i = 0; i < nx; i++) {
    for (j = 0; j < ny; j++) {
      printf ("%8.2f  ", A[i][j]);
    }
    printf ("\n");
  }
  printf ("------------------------------\n");
}
/* ********************************************************************* */
void Show_2DintArray(int **A, int nx, int ny, const char *string)
/*
 *********************************************************************** */
{
  int i, j;

  printf ("%s\n",string);
  for (j = 0; j < ny; j++) printf ("-----");
  printf ("\n");

  for (i = 0; i < nx; i++) {
    for (j = 0; j < ny; j++) {
      printf ("%03d  ", A[i][j]);
    }
    printf ("\n");
  }

  for (j = 0; j < ny; j++) printf ("-----");
  printf ("\n");
}











int main(int argc, char ** argv)
{
  int i, j, rank, size;
  int nx, ny;
  int nprocs[NDIM];
  int gsizes[NDIM];
  int lsizes[NDIM];
  int periods[NDIM] = {0,0};
  int coords[NDIM];
  int start[NDIM];
  double **A;
  MPI_Datatype subarr_type;
  MPI_Comm MPI_COMM_CART;
  char fname[] = "arr2D.bin";

/* --------------------------------------------------------
   0. Initialize the MPI execution environment
   -------------------------------------------------------- */

  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  MPI_Comm_size (MPI_COMM_WORLD, &size);

/* --------------------------------------------------------
   1. Create a 2D domain decomposition
   -------------------------------------------------------- */
  
  /* -- 1a. Attempt to find a maximally cubic decomposition -- */

  nprocs[0] = (int)sqrt(size);
  nprocs[1] = size/nprocs[0];
  if (nprocs[0]*nprocs[1] != size){
    if (rank == 0) printf ("! Cannot decompose\n");
    MPI_Finalize();
    return 1;
  }

  /* -- 1b. Create communicator -- */

  MPI_Cart_create(MPI_COMM_WORLD, NDIM, nprocs, periods, 0, &MPI_COMM_CART);
  MPI_Cart_get(MPI_COMM_CART, NDIM, nprocs, periods, coords);

  gsizes[0] = NX_GLOB;
  gsizes[1] = NY_GLOB;

  lsizes[0] = nx = NX_GLOB/nprocs[0];
  lsizes[1] = ny = NY_GLOB/nprocs[1];

  if (rank == 0){
    printf ("Domain decomposed in %d X %d procs\n",nprocs[0],nprocs[1]);
    printf ("Local grid size = %d X %d\n",lsizes[0], lsizes[1]);
  }

/* --------------------------------------------------------
   2. Allocate memory and fill 2D array on local domain
   -------------------------------------------------------- */

  A = Allocate_2DdblArray(ny,nx);
  for (j = 0; j < ny; j++) {
  for (i = 0; i < nx; i++) {
    A[j][i] = rank;
  }}

/* --------------------------------------------------------
   3. Create new datatypes
   -------------------------------------------------------- */

  start[0] = coords[0]*lsizes[0];
  start[1] = coords[1]*lsizes[1];

  MPI_Type_create_subarray (NDIM, gsizes, lsizes, start, MPI_ORDER_FORTRAN,
                            MPI_DOUBLE, &subarr_type);
  MPI_Type_commit (&subarr_type);

/* --------------------------------------------------------
   4. Open file for writing
   -------------------------------------------------------- */

  MPI_File_delete(fname, MPI_INFO_NULL);

  MPI_File fh;
  MPI_Status status;

  MPI_File_open(MPI_COMM_CART, fname, 
                MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
  MPI_File_set_view(fh, 0, MPI_DOUBLE, subarr_type, "native", MPI_INFO_NULL);
  MPI_File_write_all(fh, A[0], nx*ny, MPI_DOUBLE, MPI_STATUS_IGNORE);
  MPI_File_close(&fh);

  MPI_Type_free(&subarr_type);
  MPI_Finalize();
  return 0;
}

