# python
from mpi4py import MPI

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()

# Create a 2D Cartesian grid communicator.
dims = MPI.Compute_dims(size, 2)
periods = [True, True]
reorder = True
cart_comm = comm.Create_cart(dims, periods=periods, reorder=reorder)

cart_rank = cart_comm.Get_rank()
coords = cart_comm.Get_coords(cart_rank)

print(f"[Cartesian] Global rank {rank} has coords: {coords}")

cart_comm.Free()
MPI.Finalize()