#include "graphics/log.h"
#include "graphics/charset.h"
#include "graphics/fb.h"
#include "string.h"
#include <stdarg.h>

u32 caret_x = 0, caret_y = 0;

void caret_move(u32 x, u32 y) {
    caret_x = x, caret_y = y;
}

void putc(char ch) {
    switch (ch) {
    case '\n':
        caret_y++;
    case '\r':
        caret_x = 0;
        break;
    default:
        {
            u32 X = caret_x * char_width;
            u32 Y = caret_y * char_height;

            for (u32 y = 0; y < char_height; y++) {
                for (u32 x = 0; x < char_width; x++) {
                    fb_pixel_set(X + x, Y + y, (color_t){100, 100, 100});
                    u8 c = char_bitmaps[ch * char_width * char_height
                                        + y * char_width + x];
                    fb_pixel_set(X + x, Y + y, (color_t){c, c, c});
                }
            }

            caret_x++;

            if (caret_x == fb_width / char_width) {
                caret_x = 0;
                caret_y++;
            }
        }
    }
}

void puts(const char* s) {
    while (*s)
        putc(*s++);
}

void putsn(const char* s, size_t n) {
    if (n == 0)
        puts(s);

    for (; n > 0; n--)
        putc(*s++);
}

size_t numlen(u64 num, u32 base) {
    size_t result = 0;

    do {
        num /= base;
        result++;
    } while (num);

    return result;
}

char digits[] = "0123456789abcdef";
void put_num(u64 num, u32 base, size_t len) {
    u64 pow = 1;
    for (; len > 1; len--)
        pow *= base;

    while (pow) {
        putc(digits[num / pow % base]);
        pow /= base;
    }
}

#define FLAG_ZEROPAD   (1U << 0U)
#define FLAG_LEFT      (1U << 1U)
#define FLAG_PLUS      (1U << 2U)
#define FLAG_SPACE     (1U << 3U)
#define FLAG_HASH      (1U << 4U)
#define FLAG_UPPERCASE (1U << 5U)
#define FLAG_CHAR      (1U << 6U)
#define FLAG_SHORT     (1U << 7U)
#define FLAG_LONG      (1U << 8U)
#define FLAG_PRECISION (1U << 9U)
#define FLAG_ADAPT_EXP (1U << 10U)

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt != '%') {
            putc(*fmt++);
            continue;
        }
        fmt++;

        //%[flags][width][.precision][length]

        // flags
        u32 flags = 0;
        u8 cont;
        do {
            switch (*fmt) {
            case '0':
                flags |= FLAG_ZEROPAD;
                fmt++;
                cont = 1;
                break;
            case '-':
                flags |= FLAG_LEFT;
                fmt++;
                cont = 1;
                break;
            case '+':
                flags |= FLAG_PLUS;
                fmt++;
                cont = 1;
                break;
            case ' ':
                flags |= FLAG_SPACE;
                fmt++;
                cont = 1;
                break;
            case '#':
                flags |= FLAG_HASH;
                fmt++;
                cont = 1;
                break;
            default:
                cont = 0;
                break;
            }
        } while (cont);

        // width
        u32 width = 0U;
        if (is_digit(*fmt)) {
            width = atoi(fmt, &fmt);
        } else if (*fmt == '*') {
            const i32 w = va_arg(args, i32);
            if (w < 0) {
                flags |= FLAG_LEFT; // reverse padding
                width = (unsigned int)-w;
            } else {
                width = (unsigned int)w;
            }
            fmt++;
        }

        // precision
        i32 precision = 0U;
        if (*fmt == '.') {
            flags |= FLAG_PRECISION;
            fmt++;
            if (is_digit(*fmt)) {
                precision = atoi(fmt, &fmt);
            } else if (*fmt == '*') {
                const int prec = (i32)va_arg(args, i32);
                precision      = prec > 0 ? (unsigned int)prec : 0U;
                fmt++;
            }
        }

        // length
        switch (*fmt) {
        case 'l':
            flags |= FLAG_LONG;
            fmt++;
            break;
        case 'h':
            flags |= FLAG_SHORT;
            fmt++;
            if (*fmt == 'h') {
                flags |= FLAG_CHAR;
                fmt++;
            }
            break;
        case 'z':
            flags |= FLAG_LONG;
            fmt++;
            break;
        default:
            break;
        }

        // specifier
        switch (*fmt) {
        case 0:
            fmt--;
            break;
        case '%':
            putc('%');
            break;
        case 'c':
            {
                char c     = va_arg(args, int);
                size_t len = 1;

                if (flags & FLAG_PRECISION) {
                    len = (len < precision) ? len : precision;
                }

                if (!(flags & FLAG_LEFT)) {
                    for (size_t i = len; i < width; i++)
                        putc(' ');
                }

                putc(c);

                if (flags & FLAG_LEFT) {
                    for (size_t i = len; i < width; i++)
                        putc(' ');
                }
                break;
            }
        case 's':
            {
                const char* s = va_arg(args, const char*);
                size_t len    = strlen(s);

                if (flags & FLAG_PRECISION) {
                    len = (len < precision) ? len : precision;
                }

                if (!(flags & FLAG_LEFT)) {
                    for (size_t i = len; i < width; i++)
                        putc(' ');
                }

                putsn(s, len);

                if (flags & FLAG_LEFT) {
                    for (size_t i = len; i < width; i++)
                        putc(' ');
                }

                break;
            }
        case 'i':
        case 'd':
        case 'u':
        case 'o':
        case 'b':
        case 'x':
        case 'X':
            {
                u32 base = 10;

                if (*fmt == 'x' || *fmt == 'X')
                    base = 16;

                if (*fmt == 'o')
                    base = 8;

                if (*fmt == 'b')
                    base = 2;

                // TODO: alternative form(0x123 for hex, etc) support(hash)

                union {
                    u64 usgn;
                    i64 sign;
                } value;

                if (flags & FLAG_LONG) {
                    if (*fmt == 'i' || *fmt == 'd')
                        value.sign = va_arg(args, i64);
                    else
                        value.usgn = va_arg(args, u64);
                } else {
                    if (*fmt == 'i' || *fmt == 'd')
                        value.sign = (i32)va_arg(args, i32);
                    else
                        value.usgn = (u32)va_arg(args, u32);
                }

                bool negative = value.sign < 0;

                if (negative && (*fmt == 'i' || *fmt == 'd'))
                    value.sign = -value.sign;

                size_t len = numlen(value.usgn, base);

                if (negative && (*fmt == 'i' || *fmt == 'd'))
                    len--;

                if (!(flags & FLAG_LEFT)) {
                    for (size_t i = len; i < width; i++)
                        putc(flags & FLAG_ZEROPAD ? '0' : ' ');
                }

                if ((*fmt == 'i' || *fmt == 'd') && negative) {
                    putc('-');
                }

                put_num(value.usgn, base, len);

                if (flags & FLAG_LEFT) {
                    for (size_t i = len; i < width; i++)
                        putc(' ');
                }

                break;
            }
        }

        fmt++;
    }

    va_end(args);
}
