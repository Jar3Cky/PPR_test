#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size != 16) { // Przykładowo dla siatki 4x4 [cite: 783]
        if(world_rank == 0) printf("Wymagane 16 procesów.\n");
        MPI_Finalize(); return 0;
    }

    int val = 0;
    if (world_rank == 0) {
        srand(time(NULL));
        int matrix[16];
        for(int i=0; i<16; i++) matrix[i] = rand() % 11; // 0-10 [cite: 786]
        // Rozsyłanie elementów [cite: 610, 787]
        MPI_Scatter(matrix, 1, MPI_INT, &val, 1, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Scatter(NULL, 1, MPI_INT, &val, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // 1. Tworzenie topologii kartezjańskiej [cite: 790]
    int dims[2] = {4, 4};
    int periods[2] = {0, 0};
    MPI_Comm cart_comm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 1, &cart_comm);

    int coords[2];
    MPI_Cart_coords(cart_comm, world_rank, 2, coords);

    // 2. Podział na komunikatory KOLUMN (free_coords[1]=1 zachowuje pion) [cite: 796, 797]
    int free_cols[2] = {0, 1}; 
    MPI_Comm col_comm;
    MPI_Cart_sub(cart_comm, free_cols, &col_comm);

    int col_sum = 0;
    // Redukcja sumująca w kolumnie [cite: 800]
    MPI_Reduce(&val, &col_sum, 1, MPI_INT, MPI_SUM, 0, col_comm);

    int col_rank;
    MPI_Comm_rank(col_comm, &col_rank);
    if (col_rank == 0) {
        printf("Kolumna %d - Suma: %d (ID globalne: %d)\n", coords[0], col_sum, world_rank);
    }

    // 3. Podział na komunikatory WIERSZY (używając MPI_Comm_split wg coords[1]) [cite: 813]
    MPI_Comm row_comm;
    MPI_Comm_split(cart_comm, coords[1], world_rank, &row_comm);

    int row_sum = 0;
    MPI_Reduce(&val, &row_sum, 1, MPI_INT, MPI_SUM, 0, row_comm);

    int row_rank;
    MPI_Comm_rank(row_comm, &row_rank);
    if (row_rank == 0) {
        printf("Wiersz %d - Suma: %d (ID globalne: %d)\n", coords[1], row_sum, world_rank);
    }

    MPI_Comm_free(&col_comm);
    MPI_Comm_free(&row_comm);
    MPI_Comm_free(&cart_comm);
    MPI_Finalize();
    return 0;
}
