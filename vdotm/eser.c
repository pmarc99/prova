#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ROWS 4
#define COLS 4

int main() {
    int matrix[ROWS][COLS];
    int vector[COLS];
    int result[ROWS];

    omp_set_num_threads(4);

    #pragma omp parallel
    	printf("sono il thread numero %d\n",omp_get_thread_num());


    // Inizializzazione della matrice e del vettore
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            matrix[i][j] = i + j;
        }
        result[i] = 0;
    }

    for (int j = 0; j < COLS; j++) {
        vector[j] = j;
    }

    // Moltiplicazione parallela della matrice per il vettore
    #pragma omp parallel for
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    // Stampa del risultato
    printf("Risultato:\n");
    for (int i = 0; i < ROWS; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");

    return 0;
}
