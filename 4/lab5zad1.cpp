#include <mpi.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Oblicz q jako pierwiastek z p [cite: 458]
    int q = (int)sqrt(world_size);
    // Oblicz numer wiersza jako klucz podziału [cite: 459, 465]
    int row_id = world_rank / q;

    MPI_Comm row_comm;
    // Podział komunikatora na podstawie numeru wiersza [cite: 417, 420]
    MPI_Comm_split(MPI_COMM_WORLD, row_id, world_rank, &row_comm);

    int row_rank;
    MPI_Comm_rank(row_comm, &row_rank);

    int test = 0;
    // Jeśli id w nowym komunikatorze == 0, test = numer wiersza [cite: 466]
    if (row_rank == 0) {
        test = row_id;
    }

    // Rozgłoś zmienną test wewnątrz nowego komunikatora [cite: 419, 467]
    MPI_Bcast(&test, 1, MPI_INT, 0, row_comm);

    // Wypisz wymagane informacje [cite: 419, 469]
    printf("ID globalne: %d, Wiersz: %d, ID wierszowe: %d, Test: %d\n", 
           world_rank, row_id, row_rank, test);

    MPI_Comm_free(&row_comm);
    MPI_Finalize();
    return 0;
}
