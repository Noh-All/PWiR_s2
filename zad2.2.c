#include <omp.h>
#include <stdio.h>

#define N 1000000

int main(void) {
    double t_start, t_stop;
    double t_seq, t_par;
    double suma_seq = 0.0;

    printf("=== Zadanie 2: Suma harmoniczna (N=%d) ===\n\n", N);

    // --- 1. Wersja sekwencyjna ---
    t_start = omp_get_wtime();
    for (long i = 1; i <= N; i++) {
        suma_seq += 1.0 / i;
    }
    t_stop = omp_get_wtime();
    t_seq = t_stop - t_start;
    
    printf("Wersja sekwencyjna:\n");
    printf("Wynik: %.6f | Czas: %.6f s\n\n", suma_seq, t_seq);

    // --- 2 i 3. Wersja rownolegla (pomiary dla 1, 2, 4 watkow) ---
    int watki[] = {1, 2, 4};
    
    printf("Wersja rownolegla (z klauzula reduction):\n");
    printf("Watki | Wynik      | Czas       | Przyspieszenie\n");
    printf("------+------------+------------+----------------\n");

    for (int j = 0; j < 3; j++) {
        int p = watki[j];
        double suma_par = 0.0;
        
        t_start = omp_get_wtime();
        
        // Dyrektywa zrownoleglajaca petle z bezpieczna redukcja
        #pragma omp parallel for reduction(+:suma_par) num_threads(p)
        for (long i = 1; i <= N; i++) {
            suma_par += 1.0 / i;
        }
        
        t_stop = omp_get_wtime();
        t_par = t_stop - t_start;
        
        double speedup = t_seq / t_par;
        printf("%5d | %10.6f | %8.6f s | %6.2fx\n", p, suma_par, t_par, speedup);
    }

    return 0;
}