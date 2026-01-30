#include "log.h"
#include "stdint.h"
#include <stdarg.h>

static volatile char* debug_str = (char*)0x90000000;
#define DEBUG_BUF_SIZE 0x8000

#define MSG_END \
    *(debug_str++) = 0x00; \
    *(debug_str++) = 0xa; \
    *(debug_str++) = 0xd;

static char hex_chars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

void clear_debug_buffer() {
    for (volatile u64* i = (void*)debug_str;
         (void*)i - (void*)debug_str < DEBUG_BUF_SIZE;
         i += 1) {
        *i = 0;
    }
}

void log_u32(u32 n) {
    if (n == 0) {
        *(debug_str++) = '0';
        return;
    }

    u32 r = n;
    u32 digits = 0;
    while (r != 0) { digits++; r /= 10; }

    debug_str += digits;

    for (u32 i = 1; i <= digits; i++) {
        *(debug_str - i) = n % 10 + '0';
        n /= 10;
    }
}

void log_hex32(u32 n) {
    if (n == 0) {
        *(debug_str++) = '0';
        return;
    }

    u32 r = n;
    u32 digits = 0;
    while (r != 0) { digits++; r /= 16; }

    debug_str += digits;

    for (u32 i = 1; i <= digits; i++) {
        *(debug_str - i) = hex_chars[n % 16];
        n /= 16;
    }
}

void log(enum LogLevel level, char* msg) {
    *(debug_str++) = level;

    while (*msg) { *(debug_str++) = *(msg++); }

    MSG_END
}

void logf(enum LogLevel level, char* fmt, ...) {
    *(debug_str++) = level;

    va_list args;
    va_start(args, fmt);
    for (; *fmt; fmt++) {
        if (*fmt != '%') {
            *(debug_str++) = *fmt;
            continue;
        }

        fmt++;

        switch (*fmt) {
        /* case 'd': */
        /*     log_i32(va_arg(args, u32)); */
        /*     break; */
        case 'z':
            log_u32(va_arg(args, u32));
            break;
        /* case 'l': */
        /*     log_i64(va_arg(args, u32)); */
        /*     break; */
        /* case 'u': */
        /*     log_u64(va_arg(args, u32)); */
        /*     break; */
        case 'x':
            log_hex32(va_arg(args, u32));
            break;
        }
    }
    va_end(args);

    MSG_END
}
