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

