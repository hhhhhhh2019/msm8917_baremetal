#ifndef UTILS_H_
#define UTILS_H_

#include "stdint.h"

// TODO: find better way to cast int to void*
#define readi8(a)  (*((i8*)(a)))
#define readu8(a)  (*((u8*)(a)))
#define readi16(a) (*((i16*)(a)))
#define readu16(a) (*((u16*)(a)))
#define readi32(a) (*((i32*)(a)))
#define readu32(a) (*((u32*)(u64)(a)))
#define readi64(a) (*((i64*)(a)))
#define readu64(a) (*((u64*)(a)))

#define writei8(a, v)  (*((i8*)(a))  = (v))
#define writeu8(a, v)  (*((u8*)(a))  = (v))
#define writei16(a, v) (*((i16*)(a)) = (v))
#define writeu16(a, v) (*((u16*)(a)) = (v))
#define writei32(a, v) (*((i32*)(a)) = (v))
#define writeu32(a, v) (*((u32*)(u64)(a)) = (v))
#define writei64(a, v) (*((i64*)(a)) = (v))
#define writeu64(a, v) (*((u64*)(a)) = (v))

#endif // UTILS_H_
