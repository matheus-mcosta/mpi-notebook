from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if size < 2:
    raise Exception("This example requires at least 2 processes.")

if rank == 0:
    data = np.array([1, 2, 3], dtype=np.int32)
    comm.Send([data, MPI.INT], dest=1, tag=0)
    print(f"[P2P] Process {rank} sent {data} to process 1")
elif rank == 1:
    data = np.empty(3, dtype=np.int32)
    comm.Recv([data, MPI.INT], source=0, tag=0)
    print(f"[P2P] Process {rank} received {data} from process 0")