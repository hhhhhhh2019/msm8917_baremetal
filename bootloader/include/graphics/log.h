#ifndef LOG_H_
#define LOG_H_

#include "utils.h"

extern u32 caret_x, caret_y;

void caret_move(u32 x, u32 y);
void putc(char);
void puts(const char*);
void putsn(const char* s, size_t n);
void printf(const char* format, ...);

#endif // LOG_H_
