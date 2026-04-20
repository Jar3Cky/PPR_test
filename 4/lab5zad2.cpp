#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size != 12) {
        printf("Program wymaga dokładnie 12 procesów (4x3).\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int dims[2] = {4, 3};      // wymiary: 4 poziomo, 3 pionowo [cite: 591]
    int periods[2] = {1, 0};   // periodyczny poziom (true), niep. pion (false) [cite: 473, 592]
    int reorder = 1;           // zezwól na zmianę rang [cite: 593]
    MPI_Comm cart_comm;

    // Utworzenie komunikatora kartezjańskiego [cite: 595]
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cart_comm);

    int cart_rank;
    MPI_Comm_rank(cart_comm, &cart_rank);

    int coords[2];
    // Pobranie współrzędnych procesu [cite: 597]
    MPI_Cart_coords(cart_comm, cart_rank, 2, coords);

    int left, right, up, down;
    // Znalezienie sąsiadów: wymiar 0 (poziom), wymiar 1 (pion) [cite: 598, 599]
    MPI_Cart_shift(cart_comm, 0, 1, &left, &right);
    MPI_Cart_shift(cart_comm, 1, 1, &up, &down);

    printf("Proces P: %d, moje wspolrzedne to %d %d\n", cart_rank, coords[0], coords[1]);
    printf("Proces P: %d Moi sasiedzi to: prawo %d, lewo %d, gora %d, dol %d\n", 
           cart_rank, right, left, up, down);

    MPI_Comm_free(&cart_comm);
    MPI_Finalize();
    return 0;
}
