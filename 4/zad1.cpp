#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <mpi.h>
#include <algorithm>

using namespace std;

// Funkcja sortowania bąbelkowego [cite: 12]
void bubblesort(int *v, int n) {
    for (int i = n - 2; i >= 0; i--) {
        for (int j = 0; j <= i; j++) {
            if (v[j] > v[j + 1]) {
                swap(v[j], v[j + 1]);
            }
        }
    }
}

// Funkcja scalająca [cite: 35]
int* merge(int *v1, int n1, int *v2, int n2) {
    int *res = (int*)malloc((n1 + n2) * sizeof(int));
    int i = 0, j = 0, k = 0;
    while (i < n1 && j < n2) {
        if (v1[i] <= v2[j]) res[k++] = v1[i++];
        else res[k++] = v2[j++];
    }
    while (i < n1) res[k++] = v1[i++];
    while (j < n2) res[k++] = v2[j++];
    return res;
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv); // [cite: 14]
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Rozmiar n (np. 3 ostatnie cyfry albumu) [cite: 8, 13]
    int n = 123; 
    int *data = NULL; // [cite: 14]
    int chunk_size;

    // Obliczenia wstępne proces 0 [cite: 15, 16]
    chunk_size = (n + size - 1) / size; // Zaokrąglenie w górę
    int total_padded_size = chunk_size * size;

    if (rank == 0) {
        data = (int*)malloc(total_padded_size * sizeof(int)); // [cite: 18]
        srand(time(NULL));
        
        printf("Tablica nieposortowana:\n");
        for (int i = 0; i < total_padded_size; i++) {
            if (i < n) data[i] = rand() % 1001; // [cite: 18]
            else data[i] = 1001; // Dopełnienie wartościami spoza zakresu [cite: 17]
            printf("%d ", data[i]);
        }
        printf("\n\n");
    }

    MPI_Barrier(MPI_COMM_WORLD); // [cite: 20]
    double start_time = MPI_Wtime(); // [cite: 20]

    // Rozgłoszenie rozmiaru i podział danych [cite: 21, 25]
    MPI_Bcast(&total_padded_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    int *chunk = (int*)malloc(chunk_size * sizeof(int)); // [cite: 24]
    MPI_Scatter(data, chunk_size, MPI_INT, chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        free(data); // [cite: 26]
        data = NULL;
    }

    // Sortowanie lokalne [cite: 27, 28]
    bubblesort(chunk, chunk_size);
    int current_s = chunk_size;

    // Etapowe scalanie (Tree Merge) [cite: 37, 38, 89]
    for (int Krok = 1; Krok < size; Krok *= 2) {
        // Warunek 1: wysyłanie [cite: 90, 91]
        if (rank % (2 * Krok) != 0) {
            int partner_left = rank - Krok;
            MPI_Send(&current_s, 1, MPI_INT, partner_left, 0, MPI_COMM_WORLD);
            MPI_Send(chunk, current_s, MPI_INT, partner_left, 0, MPI_COMM_WORLD);
            break;
        }
        // Warunek 2: odbieranie [cite: 92]
        if (rank + Krok < size) {
            int remote_s;
            MPI_Recv(&remote_s, 1, MPI_INT, rank + Krok, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // [cite: 93, 95]
            int *received_data = (int*)malloc(remote_s * sizeof(int)); // [cite: 94]
            MPI_Recv(received_data, remote_s, MPI_INT, rank + Krok, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            int *temp_merged = merge(chunk, current_s, received_data, remote_s); // [cite: 96]
            
            free(chunk); // [cite: 97]
            free(received_data);
            chunk = temp_merged;
            current_s += remote_s;
        }
    }

    double end_time = MPI_Wtime(); // [cite: 99]

    // Finalne wypisanie wyników przez proces ROOT [cite: 100]
    if (rank == 0) {
        printf("Liczba procesów: %d\n", size); // [cite: 103]
        printf("Czas obliczeniowy: %f s\n", end_time - start_time); // [cite: 104]
        printf("Rozmiar odebranej tablicy: %d\n", current_s); // [cite: 101]
        printf("Posortowana tablica:\n");
        for (int i = 0; i < current_s; i++) {
            printf("%d ", chunk[i]); // [cite: 102]
        }
        printf("\n");
    }

    free(chunk);
    MPI_Finalize(); // [cite: 105]
    return 0;
}
