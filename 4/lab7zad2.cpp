#include <stdio.h>
#include <omp.h>

#define MAX_VAL 1000

int main() {
    long int rows = 500, columns = 500;
    long int columns_rows_MAX = 200; // Próg dla wersji b)
    double start_t, end_t;
    long int i, j;

    double *u = new double[columns];
    double *v = new double[rows];
    double **A = new double*[rows];
    for (i = 0; i < rows; i++) A[i] = new double[columns];

    // Inicjalizacja
    for (i = 0; i < columns; i++) {
        u[i] = (double)(i / 1000.0f);
        for (j = 0; j < rows; j++) A[j][i] = (double)(i * j / 1000.0f);
    }

    start_t = omp_get_wtime();

    #pragma omp parallel for private(j) if(rows * columns > columns_rows_MAX) [cite: 37]
    for (i = 0; i < rows; i++) {
        v[i] = 0.0f;
        for (j = 0; j < columns; j++) {
            v[i] += A[i][j] * u[j];
        }
    }

    end_t = omp_get_wtime();
    printf("Mnozenie zakonczone w czasie: %f s\n", end_t - start_t);

    // Czyszczenie pamięci
    delete[] u; delete[] v;
    for (i = 0; i < rows; i++) delete[] A[i];
    delete[] A;
    return 0;
}
