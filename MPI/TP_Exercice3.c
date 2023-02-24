#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {

    int rank, size, next, prev, id;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    next = (rank + 1) % size;
    prev = (rank + size - 1) % size;
    id = rank;

    printf("Process %d sending value %d to process %d\n", rank, id, next);
    MPI_Send(&id, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    MPI_Recv(&id, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process %d received value %d from process %d\n", rank, id, prev);

    MPI_Finalize();
    return 0;
}