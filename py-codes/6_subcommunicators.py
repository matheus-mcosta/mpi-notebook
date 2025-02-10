from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

# Each process selects a color based on even/odd rank
color = rank % 2

# Split the communicator into subcommunicators based on the color.
subcomm = comm.Split(color, rank)

subrank = subcomm.Get_rank()
subsize = subcomm.Get_size()

print(f"[Subcomm] Global rank {rank} in group {color}, subrank {subrank} (subsize {subsize})")

subcomm.Free()
MPI.Finalize()