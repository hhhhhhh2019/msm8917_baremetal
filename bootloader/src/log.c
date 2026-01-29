#include "log.h"

static char* debug_str = (char*)0x90000000;

void log(enum LogLevel level, char* msg) {
    *(debug_str++) = level;

    while (*msg) { *(debug_str++) = *(msg++); }

    *(debug_str++) = 0x00;
    *(debug_str++) = 0xff;
    *(debug_str++) = 0xa;
}
