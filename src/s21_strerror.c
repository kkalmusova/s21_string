#include "./s21_errors.h"
#include "./s21_string.h"

char *digtostr(int errnum, char *buffer) {
    buffer += 11;
    *--buffer = 0;
    do {
        *--buffer = errnum % 10 + '0';
        errnum /= 10;
    } while (errnum != 0);
    return buffer;
}

char *s21_strerror(int errnum) {
    #ifdef __APPLE__
    char *p = s21_NULL;
    static const char *er_arr[] = S21_ERRLIST;
    static char error[100];
    if (errnum < ERSIZE && errnum >= 0) {
        if (errnum < (int)(ERSIZE)) {
            p = (char *)er_arr[errnum];
        }
    } else {
        s21_memset(error, '\0', s21_strlen(error));
        char text[100];
        s21_strcat(error, "Unknown error: ");
        if (errnum < 0) {
            s21_strcat(error, "-");
            errnum *= -1;
        }
        s21_strcat(error, digtostr(errnum, text));
    }
    #endif
    #ifdef __unix__
    char *p = s21_NULL;
    static char error[100];
    if (errnum < ERSIZE && errnum >= 0) {
        if (errnum < (int)(ERSIZE)) {
            p = S21_ERRLIST[errnum];
        }
    } else {
        s21_memset(error, '\0', s21_strlen(error));
        char text[100];
        s21_strcat(error, "Unknown error ");
        if (errnum < 0) {
            s21_strcat(error, "-");
            errnum *= -1;
        }
        s21_strcat(error, digtostr(errnum, text));
    }
    #endif
    return p == s21_NULL ? error : p;
}
