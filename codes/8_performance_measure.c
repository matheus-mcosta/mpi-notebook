#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  MPI_Barrier(MPI_COMM_WORLD);
  double start = MPI_Wtime();
  MPI_Barrier(MPI_COMM_WORLD);
  sleep(1);
  double end = MPI_Wtime();
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    printf("[Performance] Barrier took %f seconds\n", end - start);
  }
  MPI_Finalize();
  return 0;
}
