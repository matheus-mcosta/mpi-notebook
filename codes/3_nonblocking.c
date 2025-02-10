#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        if (rank == 0) fprintf(stderr, "Requires at least 2 processes\n");
        MPI_Finalize();
        return 1;
    }
    MPI_Request req;
    if (rank == 0) {
        int data = 123;
        MPI_Isend(&data, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &req);
        printf("[Nonblocking] Process 0 initiated Isend with data %d\n", data);
        sleep(1);
        MPI_Wait(&req, MPI_STATUS_IGNORE);
        printf("[Nonblocking] Process 0 completed Isend\n");
    } else if (rank == 1) {
        int data = 0;
        MPI_Irecv(&data, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &req);
        printf("[Nonblocking] Process 1 initiated Irecv\n");
        sleep(1);
        MPI_Wait(&req, MPI_STATUS_IGNORE);
        printf("[Nonblocking] Process 1 received data %d\n", data);
    }
    MPI_Finalize();
    return 0;
}
