#include <stdio.h>
#include <omp.h>
#define NRA 10
#define NCA 5
#define NCB 3
int main() {
int i, j, k, tid, nthreads, chunk = 3;
double a[NRA][NCA], b[NCA][NCB], c[NRA][NCB];
double start = omp_get_wtime();
#pragma omp parallel private(i, j, k, tid) shared(a, b, c, nthreads, chunk)
{
tid = omp_get_thread_num();
if (tid == 0) {
nthreads = omp_get_num_threads();
printf("Liczba watkow: %d\n", nthreads);
printf("Inicjalizacja macierzy...\n");
}
#pragma omp for schedule(static, chunk)
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
#pragma omp for schedule(static, chunk)
for (i = 0; i < NRA; i++) {
printf("Watek %d wyliczyl wiersz %d\n", tid, i);
for (j = 0; j < NCB; j++) {
for (k = 0; k < NCA; k++) {
c[i][j] += a[i][k] * b[k][j];
}
}
}
}
printf("Macierz wynikowa:\n");
for (i = 0; i < NRA; i++) {
for (j = 0; j < NCB; j++) printf("%6.1f ", c[i][j]);
printf("\n");
}
printf("Czas: %f s\n", omp_get_wtime() - start);
return 0;
}
