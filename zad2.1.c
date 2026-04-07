#include <omp.h>
#include <stdio.h>

int main(void) {
    // Pobranie i wypisanie maksymalnej dostępnej liczby wątków
    printf("Liczba dostepnych watkow: %d\n", omp_get_max_threads());


    #pragma omp parallel num_threads(4)
    {
        int id = omp_get_thread_num();     //  ID
        int n = omp_get_num_threads();    // wątki XDDDD
        
        printf("[Watek %d/%d] Czesc z watku!\n", id, n);
    }

    printf("Koniec bloku rownoleglego.\n");
    return 0;
}