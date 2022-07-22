#include "./s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
    char *str_1 = (char *)str1;
    char *str_2 = (char *)str2;
    s21_size_t i, j, k = 0;
    for (i = 0; str_1[i] && !k; i++) {
        for (j = 0; str_2[j]; j++) {
            if (str_1[i] == str_2[j]) {
                k = 1;
                i--;
                break;
            }
        }
    }
    return i;
}
