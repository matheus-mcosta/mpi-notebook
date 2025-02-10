#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int rank, size;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // MPI_Bcast: Broadcast a value from rank 0 to all processes.
  int bcast_data;
  if (rank == 0)
    bcast_data = 100;
  MPI_Bcast(&bcast_data, 1, MPI_INT, 0, MPI_COMM_WORLD);
  printf("[Collective] Rank %d received bcast_data: %d\n", rank, bcast_data);
  sleep(1);

  // MPI_Scatter: Scatter an array of integers from rank 0.
  int *scatter_sendbuf = NULL;
  int scatter_recvbuf;
  if (rank == 0) {
    scatter_sendbuf = malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++) {
      scatter_sendbuf[i] = (i + 1) * 10;
    }
  }
  MPI_Scatter(scatter_sendbuf, 1, MPI_INT, &scatter_recvbuf, 1, MPI_INT, 0,
              MPI_COMM_WORLD);
  printf("[Collective] Rank %d received scatter_recvbuf: %d\n", rank,
         scatter_recvbuf);
  sleep(1);

  // MPI_Gather: Gather a single integer from each process to rank 0.
  int send_val = rank;
  int *gather_recvbuf = NULL;
  if (rank == 0) {
    gather_recvbuf = malloc(sizeof(int) * size);
  }
  MPI_Gather(&send_val, 1, MPI_INT, gather_recvbuf, 1, MPI_INT, 0,
             MPI_COMM_WORLD);
  if (rank == 0) {
    printf("[Collective] Rank 0 gathered values: ");
    for (int i = 0; i < size; i++) {
      printf("%d ", gather_recvbuf[i]);
    }
    printf("\n");
  }
  sleep(1);

  // MPI_Reduce: Sum the values from each process, result goes to rank 0.
  int reduce_sum;
  MPI_Reduce(&send_val, &reduce_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    printf("[Collective] Sum of ranks (reduce): %d\n", reduce_sum);
  }
  sleep(1);

  // MPI_Allreduce: Sum the values from each process. All processes receive the
  // result.
  int allreduce_sum;
  MPI_Allreduce(&send_val, &allreduce_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  printf("[Collective] Rank %d has allreduce sum: %d\n", rank, allreduce_sum);
  sleep(1);

  // MPI_Alltoall: Exchange data between all processes.
  int *alltoall_send = malloc(sizeof(int) * size);
  int *alltoall_recv = malloc(sizeof(int) * size);
  for (int i = 0; i < size; i++) {
    alltoall_send[i] = rank * 100 + i;
  }
  MPI_Alltoall(alltoall_send, 1, MPI_INT, alltoall_recv, 1, MPI_INT,
               MPI_COMM_WORLD);
  printf("[Collective] Rank %d received alltoall data: ", rank);
  for (int i = 0; i < size; i++) {
    printf("%d ", alltoall_recv[i]);
  }
  printf("\n");

  // Cleanup
  free(alltoall_send);
  free(alltoall_recv);
  if (rank == 0) {
    free(scatter_sendbuf);
    free(gather_recvbuf);
  }

  MPI_Finalize();
  return 0;
}