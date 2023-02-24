# include <mpi.h>
# include <stdio.h>

# define MPI_TAG_VALUE 123456
	
void run( const int myid,const int nprocs) {
   if ( myid == 0) {
      int valtx;

      valtx = 3785;

      for (int source = 1; source < nprocs; source++) {
         MPI_Send(&valtx ,1, MPI_INT ,source, MPI_TAG_VALUE, MPI_COMM_WORLD);
         valtx++;
      }
   }

   if ( myid != 0) {
      int valrx;
      MPI_Status status;
      MPI_Recv(&valrx ,1, MPI_INT,0, MPI_TAG_VALUE, MPI_COMM_WORLD, &status);
      printf("proc %d received the value %d\n",myid,valrx); 
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

