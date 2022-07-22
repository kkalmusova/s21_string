#include "./s21_string.h"

void *s21_memset(void *str, int c, s21_size_t n) {
    char ch = (char)c;
    char * str_s = (char*)str;
    while (n--) {
        *str_s++ = ch;
    }
    return str;
}
