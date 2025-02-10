from mpi4py import MPI
import numpy as np
import time

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if size < 2:
    raise Exception("This example requires at least 2 processes.")

if rank == 0:
    data = np.array([123], dtype=np.int32)
    # Simulate computation
    print(f"[Nonblocking] Process {rank} is sending {data[0]}")
    req = comm.Isend([data, MPI.INT], dest=1, tag=0)
    # Simulate computation
    print(f"[Nonblocking] Process {rank} is doing some computation")
    time.sleep(4)
    req.Wait()  # Ensure the send has completed
    print(f"[Nonblocking] Process {rank} completed send of {data[0]} after req.Wait")
elif rank == 1:
    data = np.empty(1, dtype=np.int32)
    req = comm.Irecv([data, MPI.INT], source=0, tag=0)
    # Simulate computation
    print(f"[Nonblocking] Process {rank} is doing some computation")
    time.sleep(2)
    req.Wait()  # Ensure the receive has completed
    print(f"[Nonblocking] Process {rank} completed receive with {data[0]} after req.Wait")