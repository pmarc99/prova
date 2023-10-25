//first version: 


#include <mpi.h>
#include <stdio.h>
int main(int argc, char ** argv)
{

  /* Init MPI...*/
int i, rank;
int numbers= 5;
int send_buf[numbers], recv_buf[numbers];
MPI_Request req;
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

/* Create buffers of positive or negative numbers */
 int sign = -1;
 if (rank == 0) sign = 1;
 for (i = 0; i < numbers; i++)
  send_buf[i] = sign * (i+1);

/* first version: both processes Ssend and then Recv...*/
 if (rank == 0){
   MPI_Ssend(send_buf, numbers, MPI_INT, 1, 0, MPI_COMM_WORLD);
   MPI_Recv (recv_buf, numbers, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
 }

 if (rank == 1){
   MPI_Ssend(send_buf, numbers, MPI_INT, 0, 0, MPI_COMM_WORLD);
   MPI_Recv (recv_buf, numbers, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
 }

}
 
 /* This version has a deadlock ... */

 /**
  *   Proc 0
  *   
  *   MPI_Ssend() ---------->  ???? *MPI_Recv()-----
  *             \                  /
  *   Proc 1    -\----------------/
  *            /  +++++++++++++****+\               
  *   MPI_Ssend() ---------->  ???? *MPI_Recv()-----
  */

