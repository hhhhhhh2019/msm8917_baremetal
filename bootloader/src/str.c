#include "str.h"

u64 strlen(char* s) {
    u64 len = 0;
    while (*(s++)) {len++;}
    return len;
}
