#include "./s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
    unsigned char *str1_s = (unsigned char *)str1;
    unsigned char *str2_s = (unsigned char *)str2;
    int res = 0;
    for (s21_size_t i = 0; i < n; i++, str1_s++, str2_s++) {
        res = *str1_s - *str2_s;
        if (*str1_s != *str2_s)
            break;
    }
    return res;
}
