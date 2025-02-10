# Basic MPI Examples for C and Python

This repository contains 2 Jupyter Notebooks, for C and Python, and all codes.

## C Code Examples

All MPI C examples are located in the `codes` folder.  
You can compile an example with:

```bash
mpicc -o ./codes/program_name codes/example.c
mpiexec -np 4 ./codes/program_name
```

## Jupyter Notebooks

- `c-mpi.ipynb`: Demonstrates MPI in C.
