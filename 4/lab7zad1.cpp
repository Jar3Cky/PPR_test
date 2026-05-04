#include <stdio.h>
#include <omp.h>

#define VECLEN 100
#define VECLEN_MAX 50 // Próg opłacalności zrównoleglenia

int main() {
    int i, tid, nthreads;
    double Gsum = 0.0, Lsum;
    double *a = new double[VECLEN];
    double *b = new double[VECLEN];
    double start_t, end_t;

    for (i = 0; i < VECLEN; i++) {
        a[i] = 1.0 + i;
        b[i] = 1.1 * i;
    }

    start_t = omp_get_wtime(); [cite: 24]

    // Wersja b) z klauzulą if [cite: 8, 26]
    #pragma omp parallel if(VECLEN > VECLEN_MAX) private(i, tid, Lsum) shared(Gsum, a, b) [cite: 13, 26]
    {
        Lsum = 0.0; [cite: 14]
        tid = omp_get_thread_num(); [cite: 11]
        
        #pragma omp for reduction(+:Gsum) [cite: 15]
        for (i = 0; i < VECLEN; i++) {
            double prod = a[i] * b[i];
            Gsum += prod;
            Lsum += prod; [cite: 15]
        }
        
        printf("Watek %d wyliczyl sume lokalna Lsum: %f\n", tid, Lsum); [cite: 16]
    }

    end_t = omp_get_wtime(); [cite: 24]

    printf("Iloczyn skalarny wynosi: %f\n", Gsum); [cite: 20]
    printf("Czas realizacji: %f sekund\n", end_t - start_t); [cite: 24]

    delete[] a;
    delete[] b;
    return 0;
}
