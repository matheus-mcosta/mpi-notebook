import time
from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

# MPI_Bcast: Broadcast from process 0 to all processes
if rank == 0:
    bcast_data = 100
else:
    bcast_data = None
bcast_data = comm.bcast(bcast_data, root=0)
print(f"[Collective] Rank {rank} received bcast_data: {bcast_data}")
time.sleep(1)

# MPI_Scatter: Scatter an array from process 0 to all processes
if rank == 0:
    scatter_data = [i * 10 for i in range(size)]
else:
    scatter_data = None
scatter_recv = comm.scatter(scatter_data, root=0)
print(f"[Collective] Rank {rank} received scatter data: {scatter_recv}")
time.sleep(1)

# MPI_Gather: Gather data from all processes to process 0
send_val = rank
gather_data = comm.gather(send_val, root=0)
if rank == 0:
    print("[Collective] Gathered data:", gather_data)
time.sleep(1)

# MPI_Reduce: Reduce data from all processes (sum) and send result to process 0
reduce_sum = comm.reduce(send_val, op=MPI.SUM, root=0)
if rank == 0:
    print("[Collective] Sum of ranks (reduce):", reduce_sum)
time.sleep(1)

# MPI_Allreduce: Reduce data (sum) and distribute the result to all processes
allreduce_sum = comm.allreduce(send_val, op=MPI.SUM)
print(f"[Collective] Rank {rank} has allreduce sum: {allreduce_sum}")
time.sleep(1)

# MPI_Alltoall: Exchange data between all processes
alltoall_send = [rank * 100 + i for i in range(size)]
alltoall_recv = comm.alltoall(alltoall_send)
print(f"[Collective] Rank {rank} received alltoall data: {alltoall_recv}")