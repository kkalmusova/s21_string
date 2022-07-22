#include "./s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
    char *str1 = (char *)haystack;
    char *str2 = (char *)needle;
    char *p = s21_NULL;
    if (!*str2) {
        p = (char *)str1;
    } else if (!*str1) {
        p = s21_NULL;
    } else if (*str1 != *str2) {
        p = s21_strstr(str1 + 1, str2);
    } else if (s21_strstr(str1 + 1, str2 + 1) == str1 + 1) {
        p = (char *)str1;
    } else {
        p =  s21_strstr(str1 + 1, str2);
    }
    return p;
}
