#ifndef UTILS_H_
#define UTILS_H_

typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
typedef long i64; // надейтесь на это
typedef unsigned long u64;
typedef u64 size_t;

#define NULL 0

#define bits(n, e, s) (((n) >> (s)) & (1 << ((e) - (s) + 1)) - 1)

#endif // UTILS_H_
