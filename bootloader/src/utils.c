#include "utils.h"

void *memcpy(void* dest, const void* src, size_t n) {
    for (const u8* p = src; p - (u8*)src < n; p++)
        *(u8*)(dest++) = *p;

    return dest-n;
}
