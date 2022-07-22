#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "./s21_string.h"

int s21_sprintf(char* str, const char* format, ...) {
    int recorded = 0;
    // if !str then EXIT
    s21_memset(str, '\0', sizeof(str));
    va_list args, args2;
    va_start(args, format);
    va_copy(args2, args);

    char** tokens = s21_NULL;
    token_struct* tokens_f = s21_NULL;
    int tokens_count = 0;
    char* format_ = (char*) format;
    flags f = {
        .flag = "+- 0",
        .width = "0123456789",
        .precision = "0123456789",
        .length = "hl",
        .specifier = "cdifsu%"
    };
    char* ptr = str;

    while (*format_ != '\0') {
        if (*format_ != '%') {
            *ptr = *format_;
            ptr++;
            format_++;
            recorded++;
        } else {
            tokens_count++;
            char** tmp = realloc(tokens, sizeof(char**) * tokens_count);
            token_struct* tmp2 = realloc(tokens_f, sizeof(token_struct) * tokens_count);
            if (tmp == s21_NULL || tmp2 == s21_NULL) {
            } else {
                tokens = tmp;
                tokens_f = tmp2;
                tokens[tokens_count - 1] = get_next_token(format_, f);
                tokens_f[tokens_count - 1] = get_token_flags(tokens[tokens_count - 1], f);
                char* parsed = s21_NULL;
                if (tokens_f[tokens_count - 1].specifier == '%') {
                    parsed = parse_percent(tokens_f[tokens_count - 1]);
                    if (s21_strlen(parsed) > 1) {
                        recorded = -1;
                        s21_memset(str, '\0', sizeof(str));
                        free(parsed);
                        break;
                    }

                } else if (tokens_f[tokens_count - 1].specifier == 'd' ||
                            tokens_f[tokens_count - 1].specifier == 'i' ||
                            tokens_f[tokens_count - 1].specifier == 'u') {
                    unsigned long long arg2 = va_arg(args2, unsigned long long);
                    long long arg = tokens_f[tokens_count - 1].l_l ?
                        va_arg(args, long long) :
                        (long long)va_arg(args, int);
                    if (arg == LLONG_MIN) tokens_f[tokens_count - 1].llong_min = 1;
                    if (arg2 == ULLONG_MAX) tokens_f[tokens_count - 1].ullong_max = 1;
                    if (tokens_f[tokens_count - 1].specifier != 'u' &&
                        tokens_f[tokens_count - 1].l_h == 1) arg = (short) arg;
                    if (tokens_f[tokens_count - 1].specifier == 'u' &&
                        tokens_f[tokens_count - 1].l_h == 1) arg = (unsigned short) arg;

                    if (tokens_f[tokens_count - 1].specifier != 'u' &&
                        tokens_f[tokens_count - 1].l_l == 1) arg = (long) arg;
                    if (tokens_f[tokens_count - 1].specifier == 'u' &&
                        tokens_f[tokens_count - 1].l_h == 1) arg = (unsigned long) arg;

                    if (tokens_f[tokens_count - 1].specifier != 'u' &&
                        !tokens_f[tokens_count - 1].l_h &&
                        !tokens_f[tokens_count - 1].l_l) arg = (int) arg;
                    if (tokens_f[tokens_count - 1].specifier == 'u' &&
                        !tokens_f[tokens_count - 1].l_h &&
                        !tokens_f[tokens_count - 1].l_l) arg = (unsigned int) arg;

                    set_token_sizes(&(tokens_f[tokens_count - 1]), arg);
                    if (tokens_f[tokens_count - 1].llong_min ||
                        tokens_f[tokens_count - 1].ullong_max) {
                        parsed = parse_d_llong(tokens_f[tokens_count - 1]);
                    } else {
                        parsed = parse_d(arg, tokens_f[tokens_count - 1]);
                    }
                } else if (tokens_f[tokens_count - 1].specifier == 'c' &&
                        tokens_f[tokens_count - 1].l_l == 1) {
                    wchar_t arg = va_arg(args, wchar_t);
                    va_arg(args2, int);
                    set_token_sizes(&(tokens_f[tokens_count - 1]), 1);
                    parsed = parse_lc(arg, tokens_f[tokens_count - 1]);

                } else if (tokens_f[tokens_count - 1].specifier == 'c') {
                    char arg = va_arg(args, int);
                    va_arg(args2, int);
                    set_token_sizes(&(tokens_f[tokens_count - 1]), 1);
                    parsed = parse_c(&arg, &tokens_f[tokens_count - 1]);
                    if (arg == 0) {
                        *ptr = '\0';
                        insert(ptr, parsed);
                        ptr += s21_strlen(parsed);
                        recorded += tokens_f[tokens_count - 1].width ?
                            tokens_f[tokens_count - 1].width : 1;
                        free(parsed);
                        break;
                    }
                } else if (tokens_f[tokens_count - 1].specifier == 's' &&
                        tokens_f[tokens_count - 1].l_l == 1) {
                    wchar_t* arg = va_arg(args, wchar_t*);
                    va_arg(args2, char*);
                    set_token_sizes(&(tokens_f[tokens_count - 1]), 1);
                    parsed = parse_ls(&tokens_f[tokens_count - 1], arg);

                } else if (tokens_f[tokens_count - 1].specifier == 's') {
                    char* arg = va_arg(args, char*);
                    va_arg(args2, char*);
                    if (!arg) arg = "(null)";
                    set_parse_s_sizes(&tokens_f[tokens_count - 1], arg);
                    parsed = parse_s(arg, &tokens_f[tokens_count - 1]);

                } else if (tokens_f[tokens_count - 1].specifier == 'f') {
                    double arg = va_arg(args, double);
                    va_arg(args2, double);
                    set_parse_f_sizes(&tokens_f[tokens_count - 1], arg);
                    parsed = parse_f(arg, tokens_f[tokens_count - 1]);
                } else {
                    format++;  //
                }

                if (parsed != s21_NULL) {
                    insert(ptr, parsed);
                    ptr += s21_strlen(parsed);
                    format_ += s21_strlen(tokens[tokens_count - 1]) + 1;
                    recorded += (int)s21_strlen(parsed);
                    free(parsed);
                } else {
                    format++;
                    tokens_count--;
                }
            }
        }
    }
    *ptr = '\0';
    if (tokens_count > 0) {
        for (int t = 0; t < tokens_count; t++) {
            free(tokens[t]);
        }
        free(tokens);
        free(tokens_f);
    }
    va_end(args2);
    va_end(args);

    return recorded;
}

// 0 => 1, 10 => 2, -1 => 1
int get_int_size(long long num) {
    int size = 1;
    long long res = num < 0 ? -num/10 : num/10;
    while (res >= 1) {
        res /= 10;
        size++;
    }
    return size;
}

// 123 -> "  123". negative numbers handled in parse_d. mutates str
void int_to_string(long long num, int size, char* str) {
    long long res = num < 0 ? -num : num;
    int i = size - 1;
    if (size == 1) {
        str[0] = num + '0';
        str[1] = '\0';
    } else {
        str[size] = '\0';
    }
    while (res >= 1) {
        str[i] = (res % 10) + 48;
        res /= 10;
        i--;
    }
}

// should be freed
char* parse_f(double arg, token_struct token_f) {
    long double val_float = signbit(arg) ? (long double)-arg : (long double)arg;
    long long sign = signbit(arg) ? -1L : 1L;
    if (token_f.precision < 20) val_float += powl(10.0L, -(long double)token_f.precision) * .5L;
    long double left = 0.0L;
    long double right = modfl(val_float, &left);

    int left_len = get_int_size((long long)left);
    char* parsed = calloc(token_f.max_size + 1, sizeof(char));
    char* ptr = parsed;
    long double temp = 0.0L;
    if (token_f.f_minus) {
        if (token_f.is_signed) ptr = parse_d_insert_flag(sign, ptr, token_f);

        int_to_string(left, left_len, ptr);
        ptr += left_len;

        if (token_f.precision > 0) {
            *ptr = '.';
            ptr++;
        }

        for (int i = 0; i < token_f.precision; i++, ptr++) {
            if (token_f.precision >= 20 && i == token_f.precision - 1) right +=  0.05;
            right = modfl(right * 10.0L, &temp);

            *ptr = (int)temp + '0';
        }

        // for (int i = 0; i < token_f.precision - token_f.right_var_len; i++, ptr++) *ptr = '0';
        for (int i = 0; i < token_f.filler_spaces; i++, ptr++) *ptr = ' ';
    } else {
        if (token_f.f_zero) {
            if (token_f.is_signed) ptr = parse_d_insert_flag(sign, ptr, token_f);
            for (int i = 0; i < token_f.filler_spaces; i++, ptr++) *ptr = '0';
        } else {
            for (int i = 0; i < token_f.filler_spaces; i++, ptr++) *ptr = ' ';
            if (token_f.is_signed) ptr = parse_d_insert_flag(sign, ptr, token_f);
        }

        int_to_string(left, left_len, ptr);
        ptr += left_len;

        if (token_f.precision > 0) {
            *ptr = '.';
            ptr++;
        }

        for (int i = 0; i < token_f.precision; i++, ptr++) {
            if (token_f.precision >= 20 && i == token_f.precision - 1) right +=  0.05;
            right = modfl(right * 10.0L, &temp);

            *ptr = (int)temp + '0';
        }

        // for (int i = 0; i < token_f.precision - token_f.right_var_len; i++, ptr++) *ptr = '0';
    }

    return parsed;
}

// insert str into src. mutates src, src should be big enough
void insert(char *src, char *str) {
    // used strcat
    s21_memset(src, '\0', sizeof(src));   // pomenat mestami
    if (src == s21_NULL) src = "";  // "(null)" ne vivodili NADO
    int k = (int)s21_strlen(str);
    for (int i = s21_strlen(src) + k; i >= k; i--) {
        src[i] = src[i - k];
    }
    for (int i = 0; i < k; i++) {
        src[i] = str[i];
    }
}

// "text %5d text ..." -> "%5d" all extracted tokens should be freed.
char* get_next_token(const char* str, flags f) {
    static char* ptr = s21_NULL;
    ptr = (char*)str;
    char* token = s21_NULL;

    while (*ptr != '\0') {
        if (*ptr == '%') {
            ptr++;
            int len = s21_strcspn(ptr, f.specifier);
            if (len == (int)s21_strlen(ptr)) {
                break;
            }
            token = calloc((len + 2), sizeof(char));
            for (int i = 0; i < len + 1; i++) {
                if (s21_strchr(f.specifier, *ptr) != s21_NULL) {
                    token[i] = *ptr;
                    ptr++;
                    break;
                } else {
                    token[i] = *ptr;
                    ptr++;
                }
            }
            break;
        } else {
            ptr++;
        }
    }
    return token;
}

// "%+2d" -> { .plus=1, .width=2, .specifier='d' ...} va_arg is unknown yet
token_struct get_token_flags(const char* token, flags f) {
    int token_size = (int)s21_strlen(token);
    char* token_ = (char*)token;
    token_struct token_f = {
        .f_plus = 0,
        .f_minus = 0,
        .f_space = 0,
        .f_zero = 0,
        .width = 0,
        .precision = 1,
        .l_h = 0,
        .l_l = 0,
        .specifier = ' ',
        .filler_spaces = 0,
        .filler_zeros = 0,
        .max_size = 0,
        .is_signed = 0,
        .total_var_len = 0,
        .right_var_len = 0,
        .is_precisioned = 0,
        .llong_min = 0,
        .ullong_max = 0,
    };

    // get specifier
    token_f.specifier = *(token + token_size - 1);
    if (token_f.specifier == 's') token_f.precision = 0;
    if (token_f.specifier == 'f') token_f.precision = 6;

    // get flags
    int flags_len = (int)s21_strspn(token_, f.flag);
    if (flags_len) {
        if (s21_memchr(token_, '+', flags_len) != s21_NULL) token_f.f_plus = 1;
        if (s21_memchr(token_, '-', flags_len) != s21_NULL) token_f.f_minus = 1;
        if (s21_memchr(token_, ' ', flags_len) != s21_NULL) token_f.f_space = 1;
        if (s21_memchr(token_, '0', flags_len) != s21_NULL) token_f.f_zero = 1;
        token_ += flags_len;
    }

    // get width
    int width_len = (int)s21_strspn(token_, f.width);
    if (width_len) {
        if (width_len > 0) token_f.width = s21_strtoi(token_, width_len);
        token_ += width_len;
    }

    // get precision
    if (*token_ == '.') {
        token_++;
        int precision_len = (int)s21_strspn(token_, f.precision);
        if (precision_len > 0) token_f.precision = s21_strtoi(token_, precision_len);
        if (precision_len == 0) token_f.precision = 0;
        token_ += precision_len;
        token_f.is_precisioned = 1;
    }

    // get length
    int length_len = (int)s21_strspn(token_, f.length);
    if (length_len) {
        if (s21_memchr(token_, 'l', length_len) != s21_NULL) token_f.l_l = 1;
        if (s21_memchr(token_, 'h', length_len) != s21_NULL) token_f.l_h = 1;
    }
    // zero flag
    if ((token_f.specifier == 'd' || token_f.specifier == 'i' ||
        token_f.specifier == 'u') && (token_f.is_precisioned)) {
        token_f.f_zero = 0;
    }
    if (token_f.f_minus && token_f.specifier != 'f') token_f.f_zero = 0;
    if (token_f.f_space && token_f.specifier != 'f') token_f.f_zero = 0;
    return token_f;
}

// parsing 'd' and 'i' 1234 -> "1234". should be freed. token should be fully filled
char* parse_d(long long var, token_struct token_f) {
    long long var_int = var;

    int var_len = get_int_size(var_int);
    char* var_str = calloc(token_f.total_var_len - token_f.is_signed + 1, sizeof(char));
    int_to_string(var_int, token_f.total_var_len - token_f.is_signed, var_str);

    char* parsed_token = calloc(token_f.max_size + 2, sizeof (char));
    char* ptr = parsed_token;

    if (token_f.precision == 0 && var_int == 0 && token_f.width == 0) {
    } else {
        if (token_f.f_minus) {
            if (token_f.specifier != 'u') ptr = parse_d_insert_flag(var_int, ptr, token_f);
            ptr = parse_d_insert_zeros(token_f, ptr);
            ptr = parse_d_insert_var_str(ptr, var_str, var_len);
            if (var == 0 && token_f.precision == 0) *(ptr - 1) = ' ';
            parse_d_insert_spaces(token_f, ptr);
        } else {
            ptr = parse_d_insert_spaces(token_f, ptr);
            if (token_f.specifier != 'u') ptr = parse_d_insert_flag(var_int, ptr, token_f);
            ptr = parse_d_insert_zeros(token_f, ptr);
            ptr = parse_d_insert_var_str(ptr, var_str, var_len);
            if (var == 0 && token_f.precision == 0) *(ptr - 1) = ' ';
        }
    }

    free(var_str);
    return parsed_token;
}

// for llong_min, ullong_max
char* parse_d_llong(token_struct token_f) {
    int var_len = token_f.llong_min ? LLONG_MIN_SIZE : ULLONG_MAX_SIZE;
    char* var_str = token_f.llong_min ? "9223372036854775808" : "18446744073709551615";

    char* parsed_token = calloc(token_f.max_size + 2, sizeof (char));
    char* ptr = parsed_token;

    if (token_f.precision == 0 && token_f.width == 0) {
    } else {
        if (token_f.f_minus) {
            if (token_f.specifier != 'u') ptr =
                parse_d_insert_flag(token_f.ullong_max - token_f.llong_min, ptr, token_f);
            ptr = parse_d_insert_zeros(token_f, ptr);
            ptr = parse_d_insert_var_str(ptr, var_str, var_len);
            parse_d_insert_spaces(token_f, ptr);
        } else {
            ptr = parse_d_insert_spaces(token_f, ptr);
            if (token_f.specifier != 'u') ptr =
                parse_d_insert_flag(token_f.ullong_max - token_f.llong_min, ptr, token_f);
            ptr = parse_d_insert_zeros(token_f, ptr);
            parse_d_insert_var_str(ptr, var_str, var_len);
        }
    }

    return parsed_token;
}

// parse_d helper function - place flag at ptr, return ptr+1
char* parse_d_insert_flag(long long var_int, char* ptr, token_struct token_f) {
    if (signbit(var_int)) {
        *ptr = '-';
        ptr++;
    } else {
        if (token_f.f_plus) {
            *ptr = '+';
            ptr++;
        } else if (token_f.f_space) {
            *ptr = ' ';
            ptr++;
        }
    }
    return ptr;
}

// parse_d helper function - place var_str at ptr, return ptr+var_len
char* parse_d_insert_var_str(char* ptr, char* var_str, int var_len) {
    s21_strcpy(ptr, var_str);
    ptr += var_len;
    return ptr;
}

// parse_d helper function
char* parse_d_insert_spaces(token_struct token_f, char* ptr) {
    if (token_f.filler_spaces) {
        for (int k = 0; k < token_f.filler_spaces; k++) {
            *ptr = token_f.f_zero ? '0' : ' ';
            ptr++;
        }
    }
    return ptr;
}

// parse_d helper function
char* parse_d_insert_zeros(token_struct token_f, char* ptr) {
    if (token_f.filler_zeros) {
        for (int k = 0; k < token_f.filler_zeros; k++) {
            *ptr = '0';
            ptr++;
        }
    }
    return ptr;
}

// get rest of flags, when var is known already
void set_token_sizes(token_struct* token_f, long long var) {
    token_f->is_signed = ((signbit(var) || token_f->f_plus || token_f->f_space) &&
                        token_f->specifier != 'u') ? 1 : 0;
    if (token_f->llong_min) {
        token_f->total_var_len = token_f->is_signed + LLONG_MIN_SIZE;
    } else if (token_f->ullong_max) {
        token_f->total_var_len = token_f->is_signed + ULLONG_MAX_SIZE;
    } else {
        token_f->total_var_len = token_f->is_signed + get_int_size(var);
    }

    // width <= len <= precision
    if (token_f->width <= token_f->total_var_len &&
        token_f->total_var_len <= token_f->precision) {
        token_f->max_size = token_f->precision;
        token_f->filler_zeros = token_f->precision - token_f->total_var_len + token_f->is_signed;

    // precision <= width <= len
    } else if (token_f->precision <= token_f->width &&
        token_f->width <= token_f->total_var_len) {
        token_f->max_size = token_f->total_var_len;

    // len <= precision <= width
    } else if (token_f->total_var_len <= token_f->precision &&
        token_f->precision <= token_f->width) {
        token_f->max_size = token_f->width;
        token_f->filler_spaces = token_f->width - token_f->precision - token_f->is_signed;
        token_f->filler_zeros = token_f->precision - token_f->total_var_len + token_f->is_signed;

    // precision <= len <= width
    } else if (token_f->precision <= token_f->total_var_len &&
        token_f->total_var_len <= token_f->width) {
        token_f->max_size = token_f->width;
        token_f->filler_spaces = token_f->width - token_f->total_var_len;

    // len <= width <= precision
    } else if (token_f->total_var_len <= token_f->width &&
        token_f->width <= token_f->precision) {
        token_f->max_size = token_f->precision;
        token_f->filler_zeros = token_f->precision - token_f->total_var_len + token_f->is_signed;

    // width <= precision <= len
    } else if (token_f->width <= token_f->precision &&
        token_f->precision <= token_f->total_var_len) {
        token_f->max_size = token_f->total_var_len;
    }
}

// parsing 'c'.    'c' - > "    c". returned char should be freed.
char* parse_c(void* var, token_struct* token_f) {
    char var_char = *((char*)var);
    token_f->max_size = (token_f->width > 1) ? token_f->width : 1;
    char* parsed_token = calloc((token_f->max_size + 1), sizeof (char));
    char* ptr = parsed_token;

    if (token_f->f_minus) {
        *ptr = var_char;
        ptr++;
        parse_c_fill_spaces(*token_f, ptr);
    } else {
        ptr = parse_c_fill_spaces(*token_f, ptr);
        *ptr = var_char;
        // ptr++;
    }

    return parsed_token;
}

// wide string parser
char* parse_ls(token_struct* token_f, wchar_t* arg) {
    char tmp[WIDE_CHAR_LEN];
    s21_memset(tmp, '\0', sizeof tmp);
    char* parsed = (char*)calloc((s21_size_t)WIDE_CHAR_LEN, sizeof(char));

    wcstombs(tmp, arg, (s21_size_t)WIDE_CHAR_LEN);
    if (token_f->is_precisioned) tmp[token_f->precision] = '\0';

    token_f->total_var_len = (int)s21_strlen(tmp);
    token_f->filler_spaces = token_f->width - token_f->total_var_len;

    if (token_f->f_minus && token_f->filler_spaces > 0) {
        s21_strcpy(parsed, tmp);
        s21_memset(parsed + token_f->total_var_len, ' ', token_f->filler_spaces);
    } else if (token_f->filler_spaces > 0) {
        s21_memset(parsed, ' ', token_f->filler_spaces);
        s21_strcpy(parsed + token_f->filler_spaces, tmp);
    } else {
        s21_strcpy(parsed, tmp);
    }
    return parsed;
}

// wide char parser
char* parse_lc(wchar_t var, token_struct token_f) {
    char* parsed = calloc(WIDE_CHAR_LEN + 1, sizeof(wchar_t));
    if (!token_f.f_minus && token_f.width) {
        char tmp[WIDE_CHAR_LEN];
        s21_memset(tmp, '\0', WIDE_CHAR_LEN);
        if (var) wcstombs(tmp, &var, WIDE_CHAR_LEN - 1);
        for (int i = 0; i < token_f.width - (int)s21_strlen(tmp); i++) parsed[i] = ' ';
        s21_strcat(parsed + token_f.width - (int)s21_strlen(tmp), tmp);

    } else if (token_f.width) {
        if (var) wcstombs(parsed, &var, WIDE_CHAR_LEN - 1);
        for (int i = s21_strlen(parsed); i < token_f.width; i++) parsed[i] = ' ';

    } else {
        if (var) wcstombs(parsed, &var, WIDE_CHAR_LEN - 1);
    }
    return parsed;
}

// parse_c helper function
char* parse_c_fill_spaces(token_struct token_f, char* ptr) {
    if (token_f.width) {  // ?????
        for (int k = 0; k < token_f.max_size - 1; k++) {
            *ptr = ' ';
            ptr++;
        }
    }
    return ptr;
}

// parsing 's'.    "ss" - > "    ss". RETURNED STRING SHOULD BE FREED. sizes should be set already
char* parse_s(const char* var_str, token_struct* token_f) {
    char* parsed_token = calloc((token_f->max_size + 1), sizeof(char));
    char* ptr = parsed_token;
    // print_token(*token_f);
    if (token_f->f_minus) {
        if (token_f->filler_spaces > 0) {
            for (int i = 0; i < token_f->total_var_len; i++) {
                ptr[i] = (token_f->is_precisioned && token_f->precision == 0) ?
                         ' ' : var_str[i];
            }
            for (int i = token_f->total_var_len; i < token_f->max_size; i++) {
                ptr[i] = ' ';
            }
        } else {
            for (int i = 0; i < token_f->max_size; i++) {
                ptr[i] = (token_f->is_precisioned && token_f->precision == 0) ?
                         ' ' : var_str[i];
            }
        }
    } else {
        if (token_f->filler_spaces > 0) {
            for (int i = 0; i < token_f->filler_spaces; i++) {
                ptr[i] = ' ';
            }
            for (int i = token_f->filler_spaces; i < token_f->max_size; i++) {
                ptr[i] = (token_f->is_precisioned && token_f->precision == 0) ?
                         ' ' : var_str[i - token_f->filler_spaces];
            }
        } else {
            for (int i = 0; i < token_f->max_size; i++) {
                ptr[i] = (token_f->is_precisioned && token_f->precision == 0) ?
                        ' ' : var_str[i - token_f->filler_spaces];
            }
        }
    }
    return parsed_token;
}

// get rest of flags, when var is known already
void set_parse_s_sizes(token_struct* token_f, char* var_str) {
    int var_len = (int)s21_strlen(var_str);

    // width precision
    if (token_f->width && token_f->precision) {
        token_f->total_var_len = token_f->precision < var_len ? token_f->precision : var_len;
        token_f->max_size = (token_f->width > token_f->total_var_len) ?
                            token_f->width : token_f->total_var_len;
        token_f->filler_spaces = token_f->max_size - token_f->total_var_len;

    // precision = 0
    } else if (token_f->is_precisioned && token_f->precision == 0) {
        token_f->max_size = token_f->width;
        token_f->total_var_len = var_len;
        token_f->filler_spaces = token_f->max_size - token_f->total_var_len;

    // precision
    } else if (!token_f->width && token_f->precision) {
        token_f->max_size = token_f->precision < var_len ? token_f->precision : var_len;
        token_f->total_var_len = token_f->max_size;

    // width
    } else if (token_f->width && !token_f->precision) {
        token_f->max_size = token_f->width > var_len ? token_f->width : var_len;
        token_f->total_var_len = var_len;
        token_f->filler_spaces = token_f->max_size - token_f->total_var_len;

    // _
    } else if (!token_f->width && !token_f->precision) {
        token_f->max_size = var_len;
        token_f->total_var_len = var_len;
    }
}

// get rest of flags, when var is known already
void set_parse_f_sizes(token_struct* token_f, double arg) {
    token_f->right_var_len = get_f_right_size(token_f, arg);
    token_f->is_signed = (signbit(arg) || token_f->f_plus || token_f->f_space) ? 1 : 0;
    double val_float = signbit(arg) ? -arg : arg;
    val_float += pow(10, -token_f->precision - 1) * 5;
    double left = 0;
    modf(val_float, &left);
    token_f->total_var_len = (token_f->precision > 0) ?
                                get_int_size((long long)left) + token_f->precision + 1 :
                                get_int_size((long long)left);
    if (token_f->width > token_f->total_var_len) {
        token_f->max_size = token_f->width + token_f->precision + 1;
        token_f->filler_spaces = token_f->width - token_f->total_var_len - token_f->is_signed;
    } else {
        token_f->max_size = token_f->total_var_len + token_f->precision + 1;
    }
}

// get right part of float
int get_f_right_size(token_struct* token_f, double arg) {
    int counter = 0;
    long double left = 0.0L;
    long double right = modfl(arg, &left);  // .153151
    right *= powl(10.0L, (long double)token_f->precision);  // 153151

    for (int i = token_f->precision; i > 0; i--) {
        if ((long long)right % 10 != 0) {
            break;
        } else {
            right = (long long)right/10.0L;
            counter++;
        }
    }
    return token_f->precision - counter;
}

// parser helper function '8d' -> 8
int s21_strtoi(const char* format, int len) {
    int res = 0;
    char* ptr = (char*) format;
    for (int i = len - 1; i >= 0; i--) {
        res += ((int)ptr[i] - 48) * pow(10, len - 1 - i);
    }
    return res;
}

// %5% -> '    %'
char* parse_percent(token_struct token_f) {
    int size = token_f.width ? token_f.width + 1 : 2;
    char* parsed = calloc(size, sizeof(char));
    // if (!token_f.f_minus) {
        if (token_f.width > 1) {
            for (int i = 0; i < token_f.width - 1; i++) parsed[i] = ' ';
            parsed[token_f.width - 1] = '%';
        } else {
            parsed[0] = '%';
        }
    // }
    return parsed;
}
