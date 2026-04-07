#include <omp.h>
#include <stdio.h>
#include <math.h>

int main(void) {
    double wynik_A = 0.0;
    long long wynik_B = 1;
    double t_start, t_end, t_A, t_B, t_C;

    t_start = omp_get_wtime(); // Początek pomiaru czasu całkowitego [cite: 470, 537]

    // Uruchomienie sekcji równoległych [cite: 40, 458]
    // Zmieniamy num_threads(3) na num_threads(2), aby sprawdzić zachowanie programu [cite: 469]
    #pragma omp parallel sections num_threads(3) 
    {
        // Sekcja A: Suma szeregu 1/i^2 [cite: 459]
        #pragma omp section
        {
            double start = omp_get_wtime();
            for (int i = 1; i <= 10000000; i++) {
                wynik_A += 1.0 / (pow(i, 2));
            }
            t_A = omp_get_wtime() - start;
            printf("[Watek %d] Sekcja A zakonczona. Czas: %.4f s\n", omp_get_thread_num(), t_A);
        }

        // Sekcja B: Silnia 20 (iloczyn) [cite: 460]
        #pragma omp section
        {
            double start = omp_get_wtime();
            for (int i = 1; i <= 20; i++) {
                wynik_B *= i;
            }
            t_B = omp_get_wtime() - start;
            printf("[Watek %d] Sekcja B zakonczona. Czas: %.4f s\n", omp_get_thread_num(), t_B);
        }

        // Sekcja C: Informacje o wątku i czasie [cite: 461]
        #pragma omp section
        {
            double start = omp_get_wtime();
            // Małe opóźnienie dla demonstracji
            printf("[Watek %d] Sekcja C: Obsluguje wypisywanie danych.\n", omp_get_thread_num());
            t_C = omp_get_wtime() - start;
            printf("[Watek %d] Sekcja C zakonczona. Czas: %.4f s\n", omp_get_thread_num(), t_C);
        }
    }
    t_end = omp_get_wtime(); // Koniec pomiaru czasu całkowitego [cite: 470]

    // Wyświetlenie wyników końcowych
    printf("\n--- WYNIKI ---\n");
    printf("Suma (Sekcja A): %.10f\n", wynik_A);
    printf("Silnia 20 (Sekcja B): %lld\n", wynik_B);
    printf("Suma czasów sekcji: %.4f s\n", (t_A + t_B + t_C));
    printf("Calkowity czas bloku sections: %.4f s\n", (t_end - t_start));

    return 0;
}