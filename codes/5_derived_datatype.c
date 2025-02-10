#include <mpi.h>
#include <stdio.h>
#include <stddef.h>

typedef struct {
    int id;
    double value;
    int data[3];
} Record;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        if (rank == 0) fprintf(stderr, "Requires at least 2 processes\n");
        MPI_Finalize();
        return 1;
    }

    MPI_Datatype record_type;
    int blocklengths[3] = {1, 1, 3};
    MPI_Aint offsets[3];
    offsets[0] = offsetof(Record, id);
    offsets[1] = offsetof(Record, value);
    offsets[2] = offsetof(Record, data);
    MPI_Datatype types[3] = {MPI_INT, MPI_DOUBLE, MPI_INT};
    MPI_Type_create_struct(3, blocklengths, offsets, types, &record_type);
    MPI_Type_commit(&record_type);

    if (rank == 0) {
        Record rec = {42, 3.14159, {7,8,9}};
        MPI_Send(&rec, 1, record_type, 1, 0, MPI_COMM_WORLD);
        printf("[Derived] Process 0 sent record: id=%d, value=%f\n", rec.id, rec.value);
    } else if (rank == 1) {
        Record rec;
        MPI_Recv(&rec, 1, record_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("[Derived] Process 1 received record: id=%d, value=%f\n", rec.id, rec.value);
    }
    MPI_Type_free(&record_type);
    MPI_Finalize();
    return 0;
}
