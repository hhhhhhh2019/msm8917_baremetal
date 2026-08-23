#ifndef FB_H_
#define FB_H_

#include "utils.h"

typedef struct __attribute__((packed)) {
    u8 b, g, r;
} color_t;

extern const u32 fb_width, fb_height;

void fb_draw_at(void* address);
void fb_pixel_set(u32 x, u32 y, color_t);
color_t fb_pixel_get(u32 x, u32 y);
// void fb_blit_image(
// u32 x, u32 y, rbg_t* image, u32 ix, u32 iy, u32 iw, u32 ih);
void fb_flush();

#endif // FB_H_
