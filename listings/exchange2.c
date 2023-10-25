#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
   int rank, n_ranks;
   int numbers = 5;
   int send_message[numbers];
   int recv_message[numbers];
   MPI_Status status;

   // First call MPI_Init
   MPI_Init(&argc, &argv);

   // Get my rank and the number of ranks
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);

   // Generate numbers to send
   int sign=-1;
   if (rank == 0) sign =1; 
   for( int i=0; i<numbers; i++){
     send_message[i] = sign * (i+1);
   }
   for (int i = 0; i<numbers; i++)
     printf("%d ",send_message[i]);
   printf("\n");
   
   if( rank == 0 ){
      // Rank 0 will send first
      MPI_Send(send_message, numbers, MPI_INT, 1, 0, MPI_COMM_WORLD);
   }

   if( rank == 1 ){
      // Rank 1 will receive it's message before sending
      MPI_Recv(recv_message, numbers, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      printf("Message received by rank %d \n", rank);
      for (int i = 0; i<numbers; i++)
	printf("%d--%d ",rank,recv_message[i]);
      printf("\n");
   }

   if( rank == 1 ){
      // Now rank 1 is free to send
      MPI_Send(send_message, numbers, MPI_INT, 0, 0, MPI_COMM_WORLD);
   }

   if( rank == 0 ){
      // And rank 0 will receive the message
      MPI_Recv(recv_message, numbers, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
      printf("Message received by rank %d \n", rank);
      for (int i = 0; i<numbers; i++)
	printf("%d--%d ",rank,recv_message[i]);
      printf("\n");
      
   }

   // Call finalize at the end
   return MPI_Finalize();
}

