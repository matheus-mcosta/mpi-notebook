#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size < 2) {
    if (rank == 0)
      fprintf(stderr, "Requires at least 2 processes\n");
    MPI_Finalize();
    return 1;
  }
  if (rank == 0) {
    char msg[] = "Hello from 0";
    MPI_Send(msg, sizeof(msg), MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    printf("[Blocking] Process 0 sent message.\n");
  } else if (rank == 1) {
    char msg[50];
    MPI_Recv(msg, 50, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("[Blocking] Process 1 received: %s\n", msg);
  }
  MPI_Finalize();
  return 0;
}
