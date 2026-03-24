#include "utils.h"
#include "log.h"

void *memcpy(void* dest, const void* src, size_t n) {
    logf(LOG_INFO, "memcpy: %d %X <- %X", n, dest, src);

    for (const u8* p = src; p - (u8*)src < n; p++)
        *(u8*)(dest++) = *p;

    return dest-n;
}
