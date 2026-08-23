#include "string.h"
#include <stddef.h>

size_t strlen(const char* s) {
    size_t result = 0;
    while (*s++)
        result++;
    return result;
}

bool is_digit(char ch) {
    return '0' <= ch && ch <= '9';
}

i32 atoi(const char* s, const char** end_ptr) {
    i32 result = 0;
    i32 sign   = 1;

    if (*s == '-') {
        sign = -1;
        s++;
    }

    while (is_digit(*s)) {
        result = result * 10 + (*s++ - '0');
    }

    if (end_ptr != NULL) {
        *end_ptr = s;
    }

    return result * sign;
}

i64 atol(const char* s, const char** end_ptr) {
    i64 result = 0;
    i64 sign   = 1;

    if (*s == '-') {
        sign = -1;
        s++;
    }

    while (is_digit(*s)) {
        result = result * 10 + (*s++ - '0');
    }

    if (end_ptr != NULL) {
        *end_ptr = s;
    }

    return result * sign;
}
