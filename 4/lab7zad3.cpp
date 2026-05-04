#include <stdio.h>
#include <omp.h>
#include <math.h>

void min_sub_tab(double *tab, long start_point, long n_points, double *sub_min) {
    double tmp = tab[start_point];
    for (long i = 0; i < n_points; i++) {
        if (tmp > tab[start_point + i]) tmp = tab[start_point + i];
    }
    *sub_min = tmp;
}

void min_tab_omp(double *tab, long n_points, double *tab_min) {
    int nthreads;
    double *min_locals;
    double global_min;

    #pragma omp parallel shared(nthreads, min_locals, n_points, tab) [cite: 56]
    {
        int id = omp_get_thread_num(); [cite: 58]
        
        #pragma omp single [cite: 57]
        {
            nthreads = omp_get_num_threads(); [cite: 57]
            min_locals = new double[nthreads]; [cite: 57]
            printf("Uruchomiono %d watkow\n", nthreads);
        }

        long ppt = (long)ceil((double)n_points / nthreads); [cite: 66]
        long sp = id * ppt; [cite: 64]
        
        // Korekta dla ostatniego wątku [cite: 64, 68]
        long current_ppt = ppt;
        if (sp + ppt > n_points) current_ppt = n_points - sp;
        if (sp >= n_points) current_ppt = 0;

        if (current_ppt > 0) {
            min_sub_tab(tab, sp, current_ppt, &min_locals[id]); [cite: 76]
        }
    } // Koniec parallel [cite: 77]

    min_sub_tab(min_locals, 0, nthreads, &global_min); [cite: 82]
    *tab_min = global_min; [cite: 83]
    delete[] min_locals;
}

int main() {
    long N = 7050200;
    double *t1 = new double[N], *t2 = new double[N], *t3 = new double[N];
    double res[3], start, end;

    for (long j = 0; j < N; j++) {
        t1[j] = N - j;
        t2[j] = (double)((j + 1.0) * (N - j) / 33.2);
        t3[j] = (double)(j * (N - j) * (2.0 * N - j) / 100.0);
    }

    start = omp_get_wtime();
    min_tab_omp(t1, N, &res[0]);
    min_tab_omp(t2, N, &res[1]);
    min_tab_omp(t3, N, &res[2]);
    end = omp_get_wtime();

    printf("Min1: %f, Min2: %f, Min3: %f\n", res[0], res[1], res[2]);
    printf("Czas: %f s\n", end - start);

    delete[] t1; delete[] t2; delete[] t3;
    return 0;
}
