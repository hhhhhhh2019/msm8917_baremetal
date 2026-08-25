#include "graphics/fb.h"

const u32 fb_width  = CONFIG_FRAMEBUFFER_WIDTH,
          fb_height = CONFIG_FRAMEBUFFER_HEIGHT;

color_t* fb;

void fb_draw_at(void* address) {
    fb = address;
}

void fb_pixel_set(u32 x, u32 y, color_t color) {
    fb[y * fb_width + x] = color;
}

color_t fb_pixel_get(u32 x, u32 y) {
    return fb[y * fb_width + x];
}

void fb_flush() {
    u8* p = (void*)fb;
    for (u64 i = 0; i < fb_width * fb_height * 3; i += 64) {
        asm volatile("dc cvac, %0" ::"r"(p + i));
    }
    asm volatile("dsb sy");
}
