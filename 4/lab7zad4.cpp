#include <stdio.h>
#include <omp.h>

#define NRA 10
#define NCA 5
#define NCB 3

int main() {
    int i, j, k, tid, nthreads, chunk = 3; [cite: 124]
    double a[NRA][NCA], b[NCA][NCB], c[NRA][NCB];
    double start = omp_get_wtime();

    #pragma omp parallel private(i, j, k, tid) shared(a, b, c, nthreads, chunk) [cite: 125, 126]
    {
        tid = omp_get_thread_num();
        if (tid == 0) {
            nthreads = omp_get_num_threads();
            printf("Mnozenie macierzy z uzyciem %d watkow\n", nthreads); [cite: 127]
            printf("Inicjalizacja macierzy...\n");
        }

        // Zrównoleglona inicjalizacja [cite: 129]
        #pragma omp for schedule(static, chunk) [cite: 130, 131]
        for (i = 0; i < NRA; i++) {
            for (j = 0; j < NCA; j++) a[i][j] = i + j;
        }
        #pragma omp for schedule(static, chunk)
        for (i = 0; i < NCA; i++) {
            for (j = 0; j < NCB; j++) b[i][j] = i * j;
        }
        #pragma omp for schedule(static, chunk)
        for (i = 0; i < NRA; i++) {
            for (j = 0; j < NCB; j++) c[i][j] = 0;
        }

        // Zrównoleglone mnożenie [cite: 132]
        #pragma omp for schedule(static, chunk)
        for (i = 0; i < NRA; i++) {
            printf("Watek nr %d wylicza wiersz %d\n", tid, i); [cite: 133]
            for (j = 0; j < NCB; j++) {
                for (k = 0; k < NCA; k++) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }

    printf("Macierz wynikowa:\n"); [cite: 136]
    for (i = 0; i < NRA; i++) {
        for (j = 0; j < NCB; j++) printf("%6.1f ", c[i][j]);
        printf("\n");
    }
    printf("Czas: %f s\n", omp_get_wtime() - start); [cite: 137]

    return 0;
}
