# python
from mpi4py import MPI
import time

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

# Synchronize processes.
comm.Barrier()
start = MPI.Wtime()

comm.Barrier()
time.sleep(1)  # Simulate work

end = MPI.Wtime()

if rank == 0:
    print(f"[Performance] Barrier took {end - start:.6f} seconds")

MPI.Finalize()