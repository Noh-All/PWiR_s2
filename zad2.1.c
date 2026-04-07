#include <omp.h>
#include <stdio.h>

int main(void) {
    // Pobranie i wypisanie maksymalnej dostepnej liczby watkow
    printf("Liczba dostepnych watkow: %d\n\n", omp_get_max_threads());

    // Tablica z liczba watkow do przetestowania (wymagane w zadaniu 1)
    int test_threads[] = {2, 4, 8};

    // Petla automatycznie uruchamia blok dla 2, 4 i 8 watkow
    for(int i = 0; i < 3; i++) {
        int threads = test_threads[i];
        printf("=== Test dla %d watkow ===\n", threads);
        
        // Ustawienie liczby watkow dla biezacego przebiegu
        omp_set_num_threads(threads);

        // Blok rownolegly (bez twardego num_threads w dyrektywie!)
        #pragma omp parallel
        {
            int id = omp_get_thread_num();     // ID
            int n = omp_get_num_threads();     // watki
            
            printf("[Watek %d/%d] Czesc z watku!\n", id, n);
        }
        printf("Koniec bloku rownoleglego dla %d watkow.\n\n", threads);
    }

    return 0;
}