#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int color = rank % 2;
    MPI_Comm subcomm;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &subcomm);
    int subrank, subsize;
    MPI_Comm_rank(subcomm, &subrank);
    MPI_Comm_size(subcomm, &subsize);
    printf("[Subcomm] Global rank %d in group %d, subrank %d (subsize %d)\n", rank, color, subrank, subsize);
    MPI_Comm_free(&subcomm);
    MPI_Finalize();
    return 0;
}
