#ifndef UTILS_H_
#define UTILS_H_

#include "stdint.h"

// TODO: find better way to cast int to void*
#define readi8(a)  (*((volatile i8*)(a)))
#define readu8(a)  (*((volatile u8*)(a)))
#define readi16(a) (*((volatile i16*)(a)))
#define readu16(a) (*((volatile u16*)(a)))
#define readi32(a) (*((volatile i32*)(a)))
#define readu32(a) (*((volatile u32*)(u64)(a)))
#define readi64(a) (*((volatile i64*)(a)))
#define readu64(a) (*((volatile u64*)(a)))

#define writei8(a, v)  (*((volatile i8*)(a))  = (v))
#define writeu8(a, v)  (*((volatile u8*)(a))  = (v))
#define writei16(a, v) (*((volatile i16*)(a)) = (v))
#define writeu16(a, v) (*((volatile u16*)(a)) = (v))
#define writei32(a, v) (*((volatile i32*)(a)) = (v))
#define writeu32(a, v) (*((volatile u32*)(u64)(a)) = (v))
#define writei64(a, v) (*((volatile i64*)(a)) = (v))
#define writeu64(a, v) (*((volatile u64*)(a)) = (v))

#define bits(n, e, s) (((n) >> (s)) & (1 << ((e) - (s) + 1)) - 1)

void edl_reboot();
void* memcpy(void* dest, const void* src, size_t n);

#endif // UTILS_H_
