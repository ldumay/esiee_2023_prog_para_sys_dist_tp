# include <mpi.h>
# include <stdio.h>
# include <stdlib.h>

# define MPI_TAG_VALUE 123456
	
void run( const int myid,const int nprocs) {
   if ( myid == 0) {
      
      int* valtx;

     // Allocate memory for the array
      valtx = (int*) malloc(nprocs * sizeof(int));

      // Initialize the array
      for (int i = 1; i < nprocs; i++) {
         valtx[i] = i*10;
      }
      
      for (int dest = 1; dest < nprocs; dest++) {
         MPI_Send(&valtx[dest] ,1, MPI_INT ,dest, MPI_TAG_VALUE, MPI_COMM_WORLD);
      }
   }

   if ( myid != 0) {
      int valrx;
      MPI_Status status;
      MPI_Recv(&valrx ,1, MPI_INT,0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      printf("proc %d received the source %d with the value %d\n",myid,status.MPI_SOURCE,valrx);
   }
}

int main(int argc , char *argv []) {
   int myid;
   int nprocs;
   MPI_Init(&argc, &argv);
  
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
   MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
   run(myid,nprocs);

   MPI_Finalize();
   return 0;
}

