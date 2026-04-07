#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Dodatek dla Windowsa (MinGW), który nie ma wbudowanego rand_r ---
#ifdef _WIN32
int rand_r(unsigned int *seed) {
    *seed = *seed * 1103515245 + 12345;
    return (unsigned int)(*seed / 65536) % 32768;
}
#endif
// ---------------------------------------------------------------------

#define N 1000000

int main(void) {
    // Alokacja pamięci dla 1 000 000 elementów
    int *tab = (int *)malloc(N * sizeof(int));
    if (!tab) {
        printf("Błąd alokacji pamięci!\n");
        return 1;
    }

    // Wypełnienie tablicy z użyciem rand_r i private(seed)
    #pragma omp parallel
    {
        // Inicjalizacja unikalnego seeda dla każdego wątku
        unsigned int seed = omp_get_thread_num() + time(NULL); 
        
        #pragma omp for
        for (int i = 0; i < N; i++) {
            tab[i] = (rand_r(&seed) % 100) + 1; // losowe liczby z zakresu 1-100
        }
    }

    double t_start, t_stop;
    int licznik_seq = 0, licznik_crit = 0, licznik_red = 0;

    printf("=== Zadanie 3: Liczby podzielne przez 7 (N=%d) ===\n\n", N);

    // --- 1. Wersja sekwencyjna ---
    t_start = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        if (tab[i] % 7 == 0) {
            licznik_seq++;
        }
    }
    t_stop = omp_get_wtime();
    double t_seq = t_stop - t_start;
    printf("Wersja sekwencyjna:  Wynik = %d | Czas = %.6f s\n", licznik_seq, t_seq);

    // --- 2. Wersja rownolegla z sekcja krytyczna (critical) ---
    t_start = omp_get_wtime();
    #pragma omp parallel for num_threads(4)
    for (int i = 0; i < N; i++) {
        if (tab[i] % 7 == 0) {
            #pragma omp critical
            {
                licznik_crit++;
            }
        }
    }
    t_stop = omp_get_wtime();
    double t_crit = t_stop - t_start;
    printf("Wersja z critical:   Wynik = %d | Czas = %.6f s\n", licznik_crit, t_crit);

    // --- 3. Wersja rownolegla z redukcja (reduction) ---
    t_start = omp_get_wtime();
    #pragma omp parallel for reduction(+:licznik_red) num_threads(4)
    for (int i = 0; i < N; i++) {
        if (tab[i] % 7 == 0) {
            licznik_red++;
        }
    }
    t_stop = omp_get_wtime();
    double t_red = t_stop - t_start;
    printf("Wersja z reduction:  Wynik = %d | Czas = %.6f s\n", licznik_red, t_red);

    free(tab);
    return 0;
}