#ifndef SRC_S21_STRING_H_
#define SRC_S21_STRING_H_

#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#define WIDE_CHAR_LEN 2048
#define LLONG_MIN_SIZE 19
#define ULLONG_MAX_SIZE 20

#define s21_NULL ((void *) 0)
typedef long unsigned s21_size_t;

s21_size_t s21_strlen(const char *str);
char *s21_strcat(char *dest, const char *src);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
int s21_strcmp(const char *str1, const char *str2);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strcpy(char *dest, const char *src);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
char *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memmove(void *dest, const void *src, s21_size_t n);
void *s21_memchr(const void *str, int c, s21_size_t n);
char *s21_strchr(const char *str, int c);
char *s21_strrchr(const char *str, int c);
s21_size_t s21_strcspn(const char *str1, const char *str2);
s21_size_t s21_strspn(const char *str1, const char *str2);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strerror(int errnum);
char *s21_strtok(char *str, const char *delim);

void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);


typedef struct format_struct {
    char* flag;
    char* width;
    char* precision;
    char* length;
    char* specifier;
} flags;
typedef struct {
    int f_plus;
    int f_minus;
    int f_space;
    int f_zero;
    int width;
    int precision;
    int l_h;
    int l_l;
    char specifier;
    int filler_spaces;
    int filler_zeros;
    int max_size;
    int is_signed;
    int total_var_len;
    int right_var_len;
    int is_precisioned;
    int llong_min;
    int ullong_max;
} token_struct;

int s21_sprintf(char* str, const char* format, ...);

// helper functions
int get_int_size(long long num);
void int_to_string(long long num, int size, char* str);
void insert(char *src, char *str);
void print_token(token_struct token_f);
char* parse_percent(token_struct token_f);

// parsers
char* get_next_token(const char* str, flags f);
token_struct get_token_flags(const char* token, flags f);
int s21_strtoi(const char *format, int len);

// d, i, u handlers
char* parse_d(long long var, token_struct token_f);
char* parse_d_insert_flag(long long var_int, char* ptr, token_struct token_f);
char* parse_d_insert_spaces(token_struct token_f, char* ptr);
char* parse_d_insert_var_str(char* ptr, char* var_str, int var_len);
char* parse_d_insert_zeros(token_struct token_f, char* ptr);
void set_token_sizes(token_struct* token_f, long long var);
char* parse_d_llong(token_struct token_f);

// c handlers
char* parse_c(void* var, token_struct* token_f);
char* parse_c_fill_spaces(token_struct token_f, char* ptr);
char* parse_lc(wchar_t var, token_struct token_f);

// s handlers
char* parse_s(const char* var_str, token_struct* token_f);
void set_parse_s_sizes(token_struct* token_f, char* var_str);
char* parse_ls(token_struct* token_f, wchar_t* arg);

// f handlers
char* parse_f(double arg, token_struct token_f);
void set_parse_f_sizes(token_struct* token_f, double arg);
int get_f_right_size(token_struct* token_f, double arg);


#endif  // SRC_S21_STRING_H_
