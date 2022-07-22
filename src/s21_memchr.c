#include "./s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
    char *str1 = (char*)str;
    char ch = (char)c;
    char *p = s21_NULL;
    while (n--) {
        if (*str1 == ch) {
            p = (void*)str1;
            break;
        }
        str1++;
    }
    return p;
}
