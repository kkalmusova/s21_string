#include "./s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
    char *str_1 = (char *)str1;
    char *str_2 = (char *)str2;
    char *p = s21_NULL;
    s21_size_t i, j, k = 0;
    for (i = 0; str_1[i] && !k; i++) {
        for (j = 0; str_2[j]; j++) {
            if (str_1[i] == str_2[j]) {
                p = &str_1[i];
                k = 1;
                break;
            }
        }
    }
    return p;
}
