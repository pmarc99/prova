#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define numbers 3

int main(){
    int rank,size;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Status status;

    int *sendbuf,*recvbuf;
    if ( NULL == (sendbuf = malloc( 10 * sizeof (int) ))) exit(1);       
    if ( NULL == (recvbuf = malloc( 10 * sizeof (int) ))) exit(1);       
   
    for ( int i = 0 ; i < numbers; i++) {
        sendbuf[i] = rank;
        recvbuf[i] = -100;
    }

    int next_rank, prev_rank;
    next_rank = (rank < size -1) ? rank + 1 : MPI_PROC_NULL;   
    prev_rank = (rank >    0   ) ? rank - 1 : MPI_PROC_NULL;   

    for ( int i = 0 ; i < numbers; i++)
    	printf("[Before] Send Rank: %d\tRecv Rank: %d\n",sendbuf[i],recvbuf[i]);
/*
       int MPI_Sendrecv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
            int dest, int sendtag, void *recvbuf, int recvcount,
            MPI_Datatype recvtype, int source, int recvtag,
            MPI_Comm comm, MPI_Status *status)
*/   
    MPI_Sendrecv( sendbuf, numbers, MPI_INT, next_rank, 0, recvbuf, numbers, MPI_INT, prev_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &status );

    for ( int i = 0 ; i < numbers; i++)
    	printf("[After] Send Rank: %d\tRecv Rank: %d\n",sendbuf[i],recvbuf[i]);

   
    free(sendbuf);
    free(recvbuf);

    MPI_Finalize();
    return 0;
}

