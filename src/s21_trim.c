#include "./s21_string.h"

void* s21_trim(const char *src, const char *trim_chars) {
    int start = src != s21_NULL && src && s21_strlen(src) != 0 ? 1 : 0;
    char *new_str = s21_NULL;
    if (start) {
        int src_len = s21_strlen(src);
        if (trim_chars && s21_strlen(trim_chars) > 0) {
            int ptr_l = 0;
            int ptr_r = src_len - 1;
            for (; s21_strchr(trim_chars, src[ptr_l]) && src[ptr_l]; ptr_l++) {}
            for (; s21_strchr(trim_chars, src[ptr_r]) && ptr_l < ptr_r; ptr_r--) {}
            new_str = calloc(ptr_r - ptr_l + 2, sizeof(char));
            if (new_str != s21_NULL) {
                for (int j = 0, i = ptr_l; i <= ptr_r; i++, j++) {
                    new_str[j] = src[i];
                }
            }
        } else {
            new_str = s21_trim(src, " \t\n\v\f\r");
        }
    } else if (src != s21_NULL) {
        new_str = calloc(1, sizeof(char));
    }
    return new_str;
}
