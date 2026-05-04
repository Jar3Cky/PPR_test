#include <stdio.h>
#include <omp.h>
#define VECLEN 100
#define VECLEN_MAX 50
int main() {
int i, tid;
double Gsum = 0.0, Lsum;
double *a = new double[VECLEN];
double *b = new double[VECLEN];
double start_t, end_t;
for (i = 0; i < VECLEN; i++) {
a[i] = 1.0 + i;
b[i] = 1.1 * i;
}
start_t = omp_get_wtime();
#pragma omp parallel if(VECLEN > VECLEN_MAX) private(i, tid, Lsum) shared(Gsum, a, b)
{
Lsum = 0.0;
tid = omp_get_thread_num();
#pragma omp for reduction(+:Gsum)
for (i = 0; i < VECLEN; i++) {
double prod = a[i] * b[i];
Gsum += prod;
Lsum += prod;
}
printf("Watek %d wyliczyl sume lokalna Lsum: %f\n", tid, Lsum);
}
end_t = omp_get_wtime();
printf("Iloczyn skalarny wynosi: %f\n", Gsum);
printf("Czas realizacji: %f sekund\n", end_t - start_t);
delete[] a;
delete[] b;
return 0;
}
