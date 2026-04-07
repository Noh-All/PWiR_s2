#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define N 16
#define POWTORZENIA 1000000

// Funkcja symulująca ciężkie obliczenia o zmiennym czasie
double wykonaj_obliczenia(int i, int typ_obciazenia) {
    double suma = 0.0;
    // Jeśli typ_obciazenia == 1, czas rośnie proporcjonalnie do i
    // Jeśli typ_obciazenia == 0, czas jest stały
    long limit = (typ_obciazenia == 1) ? (i * POWTORZENIA) : (8 * POWTORZENIA);
    
    for (long j = 0; j < limit; j++) {
        suma += sqrt(j);
    }
    return suma;
}

void testuj_strategie(int typ_obciazenia) {
    const char* nazwy[] = {"static", "static, 1", "dynamic, 1", "dynamic, 4", "guided"};
    
    printf("%-12s | %-10s\n", "Strategia", "Czas [s]");
    printf("---------------------------\n");

    for (int s = 0; s < 5; s++) {
        double start = omp_get_wtime();
        
        #pragma omp parallel num_threads(4)
        {
            // Wybór strategii w zależności od iteracji testu
            if (s == 0) {
                #pragma omp for schedule(static)
                for (int i = 1; i <= N; i++) wykonaj_obliczenia(i, typ_obciazenia);
            } else if (s == 1) {
                #pragma omp for schedule(static, 1)
                for (int i = 1; i <= N; i++) wykonaj_obliczenia(i, typ_obciazenia);
            } else if (s == 2) {
                #pragma omp for schedule(dynamic, 1)
                for (int i = 1; i <= N; i++) wykonaj_obliczenia(i, typ_obciazenia);
            } else if (s == 3) {
                #pragma omp for schedule(dynamic, 4)
                for (int i = 1; i <= N; i++) wykonaj_obliczenia(i, typ_obciazenia);
            } else if (s == 4) {
                #pragma omp for schedule(guided)
                for (int i = 1; i <= N; i++) wykonaj_obliczenia(i, typ_obciazenia);
            }
        }
        
        double stop = omp_get_wtime();
        printf("%-12s | %.4f\n", nazwy[s], stop - start);
    }
}

int main() {
    printf("### TEST 1: Obciazenie niejednorodne (czas rośnie z i) ###\n");
    testuj_strategie(1);
    
    printf("\n### TEST 2: Obciazenie jednorodne (staly czas) ###\n");
    testuj_strategie(0);
    
    return 0;
}