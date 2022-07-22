#include "./s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
    char * res = dest;
    s21_size_t i, j;
    for (i = 0; dest[i] != '\0'; i++) {}
    for (j = 0; j < n && src[j] != '\0'; j++) {
        dest[i + j] = src[j];
    }
    dest[i+j] = '\0';
    return res;
}
