from mpi4py import MPI
import numpy as np

# Define a structured NumPy data type.
derived_dtype = np.dtype([
    ('id', np.int32),
    ('value',   np.float64),
    ('timestamp', np.float64)
])

# Compute block lengths, displacements (offsets), and MPI types.
blocklengths = [1, 1, 1]
offsets = [derived_dtype.fields[field][1] for field in derived_dtype.names]
types = [MPI.INT, MPI.DOUBLE, MPI.DOUBLE]

# Create the MPI datatype
derived_mpi_type = MPI.Datatype.Create_struct(blocklengths, offsets, types)
derived_mpi_type.Commit()

# Align it properly to match NumPy's structure size
aligned_derived_mpi_type = derived_mpi_type.Create_resized(0, derived_dtype.itemsize)
aligned_derived_mpi_type.Commit()

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

if size < 2:
    raise Exception("This example requires at least 2 processes.")

if rank == 0:
    sensor_data = np.array([
        (101, 23.45, 1618033.98),
        (102, 27.89, 1618034.98)
    ], dtype=derived_dtype)
    
    comm.Send([sensor_data, aligned_derived_mpi_type], dest=1, tag=42)
    print(f"[Derived] Process {rank} sent sensor data:")
    print(sensor_data)
    
elif rank == 1:
    sensor_data_recv = np.empty(2, dtype=derived_dtype)
    comm.Recv([sensor_data_recv, aligned_derived_mpi_type], source=0, tag=42)
    print(f"[Derived] Process {rank} received sensor data:")
    print(sensor_data_recv)

aligned_derived_mpi_type.Free()
derived_mpi_type.Free()
