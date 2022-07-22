#include "./s21_string.h"

void *s21_memmove(void *dest, const void *src, s21_size_t n) {
    char *dest_s = (char*)dest;
    char *src_s = (char*)src;
    if (dest_s < src_s) {
        while (n--)
            *dest_s++ = *src_s++;
    } else {
        char *lasts = src_s + (n-1);
        char *lastd = dest_s + (n-1);
        while (n--)
            *lastd-- = *lasts--;
    }
    return dest;
}
