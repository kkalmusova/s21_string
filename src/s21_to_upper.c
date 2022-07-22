#include "./s21_string.h"

void *s21_to_upper(const char *str) {
    char *str1 = (char *)str;
    char *new_str = s21_NULL;
    if (str1 != s21_NULL) {
        int n = s21_strlen(str1);
        new_str = calloc(n + 1, sizeof(char));
        if (new_str != s21_NULL) {
            int i, difference = 32;
            for (i = 0; str1[i]; i++) {
                int lower_case = str1[i] >= 97 && str1[i] <= 122;
                if (lower_case) {
                    new_str[i] = str1[i] - difference;
                } else {
                    new_str[i] = str1[i];
                }
            }
        }
    }
    return new_str;
}
