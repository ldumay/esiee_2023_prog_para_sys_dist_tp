# include <mpi.h>
# include <stdio.h>

# define MPI_TAG_VALUE 123456

void run( const int myid,const int nprocs) {
	if ( myid == 0) {
		int valtx = 3785;
		for (int source = 1; source < nprocs; source++) {
            float *donnees;
            donnees=(float*)malloc(10);
            
			MPI_Send(&valtx, 1, MPI_INT, source, MPI_TAG_VALUE+10, MPI_COMM_WORLD);
			//printf("proc %d sent the value %d to proc %d\n",myid,valtx,source);
			valtx++;
		}
	}

	if ( myid != 0) {
		int valrx;
		MPI_Status status;
		MPI_Recv(&valrx, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf("proc %d received the value %d\n",myid,valrx);
		//printf("proc %d received the tag %d\n",myid,status.MPI_TAG);
		printf("proc %d received the source %d\n",myid,status.MPI_SOURCE);
		//printf("proc %d received the error %d\n",myid,status.MPI_ERROR);
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
