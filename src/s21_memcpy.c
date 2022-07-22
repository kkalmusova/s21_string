#include "./s21_string.h"

char *s21_memcpy(void *dest, const void *src, s21_size_t n) {
    char * dest_s = (char*)dest;
    char * src_s = (char*)src;
    while (n--) {
        *dest_s++ = *src_s++;
    }
    return dest;
}
