#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int n;                      // total number of intervals
  int i;                      // loop variable
  int rank, size;             // MPI process rank and number of processes
  double h;                   // width of each interval
  double x;                   // sample point in the interval
  double local_sum = 0.0, pi; // partial sum and final result

  // Initialize MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Process 0 reads the command-line argument.
  if (rank == 0) {
    if (argc != 2) {
      fprintf(stderr, "Usage: %s <num_intervals>\n", argv[0]);
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
    n = atoll(argv[1]);
  }
  double t_start = MPI_Wtime();

  // Broadcast the number of intervals to all processes
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // Compute the width of each interval
  h = 1.0 / n;

  /*
   * Each process computes its portion of the sum.
   * Process 'rank' handles the intervals:
   *   i = rank, rank + size, rank + 2*size, ...
   *
   * The approximation to the integral for each interval is:
   *   (1/n) * (4 / (1 + x*x))   where x = i/n.
   */
  for (i = rank; i < n; i += size) {
    x = i * h;
    local_sum += 4.0 / (1.0 + x * x);
  }
  local_sum = local_sum * h;

  // Use MPI_Reduce to sum up all the partial results into pi (only on rank 0)
  MPI_Reduce(&local_sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  double t_end = MPI_Wtime();
  if (rank == 0) {
    printf("Calculated Pi = %.16f\n", pi);
    printf("Computation Time: %.10f seconds\n", t_end - t_start);
    double expected_pi = 3.141592653589793;
    double error = fabs(pi - expected_pi);
    printf("Error: %.8e\n", error);
  }

  MPI_Finalize();
  return 0;
}