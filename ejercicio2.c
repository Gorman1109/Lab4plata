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
