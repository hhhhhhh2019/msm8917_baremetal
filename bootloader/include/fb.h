#ifndef FB_H_
#define FB_H_

#include "utils.h"

void fb_init();
void fb_init_addres(void* address);
void fb_set_pixel(u32 x, u32 y, u8 r, u8 g, u8 b);
void fb_get_pixel(u32 x, u32 y, u8* r, u8* g, u8* b);
void fb_blit(u32 X, u32 Y, u32 image_width, u32 image_height, u8* image);
void fb_draw_char(u32 x, u32 y, u8 ch);
void fb_put_char(u8 ch);
void fb_put_str(u8* str);
void fb_flush();

#endif // FB_H_
