from mpi4py import MPI
import sys
import math

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

# Process 0 reads the number of intervals from the command line.
if rank == 0:
    if len(sys.argv) != 2:
        print("Usage: python3 9_final.py <number_of_intervals>")
        comm.Abort(1)
    try:
        n = int(sys.argv[1])
    except ValueError:
        print("Please provide an integer for number_of_intervals.")
        comm.Abort(2)
else:
    n = None

# Broadcast the number of intervals to all processes
n = comm.bcast(n, root=0)

h = 1.0 / n
local_sum = 0.0

t_start = MPI.Wtime()

# Each process computes its portion of the sum.
# Process 'rank' handles i = rank, rank+size, rank+2*size, ...
for i in range(rank, n, size):
    x = i * h
    local_sum += 4.0 / (1.0 + x * x)
local_sum *= h

# Reduce all partial sums into the final value of pi on process 0.
pi = comm.reduce(local_sum, op=MPI.SUM, root=0)
t_end = MPI.Wtime()

if rank == 0:
    error = abs(pi - math.pi)
    print(f"Calculated Pi = {pi:.16f}")
    print(f"Elapsed time = {t_end - t_start:.6f} seconds")
    print(f"Error = {error:.6e}")

MPI.Finalize()