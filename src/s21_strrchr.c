#include "./s21_string.h"

char *s21_strrchr(const char *str, int c) {
    char ch = (char)c;
    s21_size_t len = s21_strlen(str);
    char *p = s21_NULL;
    for (int i = len; i >= 0; i--) {
        if (str[i] == ch) {
            p = ((char *)str) + i;
            break;
        }
    }
    return p;
}
