#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funkcja scalająca dwie posortowane podtablice
void merge(int *tab, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) L[i] = tab[l + i];
    for (int j = 0; j < n2; j++) R[j] = tab[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) tab[k++] = L[i++];
        else tab[k++] = R[j++];
    }
    while (i < n1) tab[k++] = L[i++];
    while (j < n2) tab[k++] = R[j++];

    free(L);
    free(R);
}

// Główna funkcja sortująca
void mergeSort(int *tab, int l, int r, int cutoff) {
    if (l < r) {
        // Implementacja progu (cutoff) [cite: 476]
        if ((r - l) < cutoff) {
            // Sortowanie sekwencyjne dla małych fragmentów
            for (int i = l + 1; i <= r; i++) {
                int klucz = tab[i];
                int j = i - 1;
                while (j >= l && tab[j] > klucz) {
                    tab[j + 1] = tab[j];
                    j--;
                }
                tab[j + 1] = klucz;
            }
            return;
        }

        int m = l + (r - l) / 2;

        // Tworzenie zadań dla lewej i prawej strony [cite: 475]
        #pragma omp task shared(tab) firstprivate(l, m, cutoff)
        mergeSort(tab, l, m, cutoff);

        #pragma omp task shared(tab) firstprivate(m, r, cutoff)
        mergeSort(tab, m + 1, r, cutoff);

        // Oczekiwanie na zakończenie obu zadań potomnych 
        #pragma omp taskwait
        merge(tab, l, m, r);
    }
}

int main() {
    int N = 1000000; // 10^6 elementów [cite: 477]
    int *tab = malloc(N * sizeof(int));
    int progi[] = {100, 1000, 10000}; // Eksperymenty z progiem [cite: 478]

    for (int p = 0; p < 3; p++) {
        int cutoff = progi[p];
        // Inicjalizacja tablicy losowymi danymi
        for (int i = 0; i < N; i++) tab[i] = rand() % 100000;

        double start = omp_get_wtime();
        
        // Wzorzec: parallel -> single -> task [cite: 132, 521]
        #pragma omp parallel
        {
            #pragma omp single
            {
                mergeSort(tab, 0, N - 1, cutoff);
            }
        }
        
        double stop = omp_get_wtime();
        printf("Cutoff: %d | Czas: %.4f s\n", cutoff, stop - start);
    }

    free(tab);
    return 0;
}