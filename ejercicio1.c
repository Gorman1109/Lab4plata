#include <stdio.h>
#include <stdlib.h>

/*
  Universidad de Costa Rica
  Ejercicio 1 - Adrian Mendez
*/

//funcion para imprimir la matriz en pantalla con filas y columnas, solo lee e imprime usando aitmetica de punteros 


void imp_matrix(int *mat, int rows, int cols) {
    int *p = mat; //creacion del puntero p que apunta al inicio de la matriz
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            // dirección lineal: p + r*cols + c
            printf("%4d ", *(p + r*cols + c));
        }
        putchar('\n');
    }
}

//Implementacion con bubble_sort_inplace para ordenar en la misma memoria arr, usando los punteros i y j

void bubble_sort_inplace(int *arr, int n) {
    int *end = arr + n; // puntero que apunta a un elemnto despues del ultimo
    int *i, *j; //punteros usados como indices
    for (i = arr; i < end - 1; ++i) {
        int swapped = 0;
        for (j = arr; j < end - (i - arr) - 1; ++j) {
            if (*j > *(j + 1)) {
                int tmp = *j;
                *j = *(j + 1);
                *(j + 1) = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break; // si no hubo intercambios en la pasada completa, el arreglo ya esta ordenado: salimos del bucle externo anticipadamente
    }
}
/* función que toma una matriz contigua (mat) con rows x cols,
   crea una copia lineal (opcionalmente) y ordena en su lugar */

void procesar_matriz(int *mat, int rows, int cols) {
    int total = rows * cols;

    printf("Matriz original (%dx%d):\n", rows, cols);
    imp_matrix(mat, rows, cols);
    putchar('\n');

    // Convertir virtualmente a arreglo 1D, usamos el mismo puntero 'mat' 
    int *arr = mat; /* puntero al arreglo unidimensional */

    // Ordenar usando BubbleSort in-place (sin arreglo auxiliar)
    bubble_sort_inplace(arr, total);

    // Mostrar el arreglo ordenado (lineal)
    printf("Arreglo ordenado (1D):\n");
    for (int k = 0; k < total; ++k) {
        printf("%d ", *(arr + k));
    }
    putchar('\n');

    // Reconstruir y mostrar la matriz con las mismas dimensiones 
    printf("\nMatriz reconstruida (%dx%d) despues de ordenar:\n", rows, cols);
    imp_matrix(mat, rows, cols);
    putchar('\n');
}

int main(void) {
    /* Creacion de las tres matrices que se almacenan en buffers dinámicos para que sean contiguas y trabajemos con punteros.*/

    /* Ejemplo 1: 2x3 */
    int rows1 = 2, cols1 = 3;
    int *mat1 = malloc(rows1 * cols1 * sizeof(int)); // aqui usamos malloc para garantizar memoria de forma contigua y permitir usar la misma funcion procesar_matriz para matrices de  distintas dimensiones

    if (!mat1) { perror("malloc"); return 1; }
    /* Llenado usando aritmética de punteros */
    int vals1[] = { 9, 2, 7, 4, 5, 1 };
    for (int k = 0; k < rows1 * cols1; ++k) {
        *(mat1 + k) = *(vals1 + k);
    }

    /* Ejemplo 2: 3x3 */
    int rows2 = 3, cols2 = 3;
    int *mat2 = malloc(rows2 * cols2 * sizeof(int));
    if (!mat2) { perror("malloc"); return 1; }
    int vals2[] = { 12, 3, 5, 8, 1, 10, 6, 7, 2 };
    for (int k = 0; k < rows2 * cols2; ++k) {
        *(mat2 + k) = *(vals2 + k);
    }

    /* Ejemplo 3: 4x2 */
    int rows3 = 4, cols3 = 2;
    int *mat3 = malloc(rows3 * cols3 * sizeof(int));
    if (!mat3) { perror("malloc"); return 1; }
    int vals3[] = { 20, 4, 3, 15, 9, 0, 7, 11 };
    for (int k = 0; k < rows3 * cols3; ++k) {
        *(mat3 + k) = *(vals3 + k);
    }

    /* Procesar y mostrar cada caso */
    printf("=== CASO 1 ===\n");
    procesar_matriz(mat1, rows1, cols1);

    printf("=== CASO 2 ===\n");
    procesar_matriz(mat2, rows2, cols2);

    printf("=== CASO 3 ===\n");
    procesar_matriz(mat3, rows3, cols3);

    /* Liberar memoria */
    free(mat1);
    free(mat2);
    free(mat3);

    return 0;
}
