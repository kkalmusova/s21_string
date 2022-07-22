#include "./s21_string.h"

s21_size_t s21_strspn(const char *str1, const char *str2) {
    char *str_1 = (char *)str1;
    char *str_2 = (char *)str2;
    s21_size_t i, j;
    for (i = 0; str_1[i]; i++) {
        int control = 0;
        for (j = 0; str_2[j]; j++) {
            if (str_1[i] == str_2[j]) {
                control = 1;
                break;
            }
        }
        if (control == 0) {
            break;
        }
    }
    return i;
}
