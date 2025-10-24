#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Universidad de Costa Rica, Adrian Mendez
// Laboratorio #4 programacion bajo plataformas abiertas


#define MAX_WORD 1024

/* Convierte una cadena a minusculas (devuelve puntero a buffer dinámico).
   Caller debe liberar el resultado con free(). */

//Facilita comparaciones insensibles a mayusculas sin modificar las cadenas originales
char *str_to_lower_alloc(const char *s) {
    size_t n = strlen(s);
    char *out = malloc(n + 1);
    if (!out) return NULL;
    for (size_t i = 0; i < n; ++i) out[i] = (char)tolower((unsigned char)s[i]);
    out[n] = '\0';
    return out;
}

/* Construye nombre de archivo de salida: input -> input_replaced (si tiene .ext, lo preserva)
   Devuelve puntero dinámico que debe liberarse. */
char *make_output_filename(const char *inname) {
    const char *suffix = "_replaced";
    size_t len = strlen(inname);
    /* buscar ultima '.' despues del último '/' para mantener extension si existe */
    const char *last_slash = strrchr(inname, '/');
    const char *p = strrchr(inname, '.');
    if (p && (!last_slash || p > last_slash)) {
        /* hay extensión */
        size_t base_len = p - inname;
        size_t ext_len = strlen(p);
        char *out = malloc(base_len + strlen(suffix) + ext_len + 1); //utilizamos malloc para liberar la memoria y si malloc falla devuelve NULL
        if (!out) return NULL;
        memcpy(out, inname, base_len);
        out[base_len] = '\0';
        strcat(out, suffix);
        strcat(out, p); /* agrega extension */
        return out;
    } else {
        /* no extension detectable */
        char *out = malloc(len + strlen(suffix) + 1);
        if (!out) return NULL;
        strcpy(out, inname);
        strcat(out, suffix);
        return out;
    }
}

/* Compara dos palabras insensible a mayusculas.
   Devuelve 1 si son iguales, 0 si no. */
int equals_insensitive(const char *a, const char *b) {
    char *la = str_to_lower_alloc(a);
    char *lb = str_to_lower_alloc(b);
    if (!la || !lb) {
        free(la); free(lb);
        return 0; /* en error de malloc, tratar como no iguales */
    }
    int res = (strcmp(la, lb) == 0);
    free(la); free(lb);
    return res;
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s archivo_entrada palabra_a_buscar palabra_reemplazo\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *infile = argv[1];
    const char *search = argv[2];
    const char *replace = argv[3];

    if (strlen(search) == 0) {
        fprintf(stderr, "La palabra a buscar no puede ser vacia.\n");
        return EXIT_FAILURE;
    }

    FILE *fin = fopen(infile, "r");
    if (!fin) {
        perror("Error al abrir archivo de entrada");
        return EXIT_FAILURE;
    }

    char *outname = make_output_filename(infile);
    if (!outname) {
        fprintf(stderr, "Error al crear nombre de archivo de salida\n");
        fclose(fin);
        return EXIT_FAILURE;
    }

    FILE *fout = fopen(outname, "w");
    if (!fout) {
        perror("Error al abrir archivo de salida");
        free(outname);
        fclose(fin);
        return EXIT_FAILURE;
    }
   
   
   
    /* Procesamos carácter por caracter. Construimos "palabras" con caracteres alfanumericos
       y consideramos cualquier otro caracter como separador/puntuacion. Preservamos los
       separadores exactamente igual en la salida. */
    int c;
    char wordbuf[MAX_WORD];
    size_t wlen = 0;

    while ((c = fgetc(fin)) != EOF) {
        if (isalnum((unsigned char)c)) {
            /* acumular en palabra */
            if (wlen + 1 < MAX_WORD) {
                wordbuf[wlen++] = (char)c;
            } else {
                /* palabra excesivamente larga: escribir lo acumulado y resetear */
                wordbuf[wlen] = '\0';
                /* comparar y escribir */
                if (equals_insensitive(wordbuf, search)) {
                    fputs(replace, fout);
                } else {
                    fputs(wordbuf, fout);
                }
                wlen = 0;
                /* continuar acumulando this char as first char */
                wordbuf[wlen++] = (char)c;
            }
        } else {
            /* fin de palabra (si existia): procesar */
            if (wlen > 0) {
                wordbuf[wlen] = '\0';
                if (equals_insensitive(wordbuf, search)) {
                    fputs(replace, fout);
                } else {
                    fputs(wordbuf, fout);
                }
                wlen = 0;
            }
            /* escribir el separador/puntuacion tal cual */
            fputc(c, fout);
        }
    }

    /* Si el archivo termina con una palabra sin separador al final */
    if (wlen > 0) {
        wordbuf[wlen] = '\0';
        if (equals_insensitive(wordbuf, search)) {
            fputs(replace, fout);
        } else {
            fputs(wordbuf, fout);
        }
    }

    /* cerrar y liberar */
    fclose(fin);
    fclose(fout);
    free(outname);

    printf("Archivo procesado. Salida escrita en: %s\n", outname ? outname : "(nombre liberado)");
    /* Nota: outname ya fue liberado; se deja mensaje informativo anterior a free en caso de error. */
    return EXIT_SUCCESS;
}
