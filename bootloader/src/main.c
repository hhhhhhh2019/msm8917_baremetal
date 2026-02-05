#include "spmi.h"
#include "log.h"
#include "utils.h"
#include "gpio.h"

#define __asmeq(x, y)  ".ifnc " x "," y " ; .err ; .endif\n\t"

void edl_reboot() {
    register u64 r0 __asm__("x0") = 2181039362;
    register u64 r1 __asm__("x1") = 2;
    register u64 r2 __asm__("x2") = 0x193D100;
    register u64 r3 __asm__("x3") = 1;
    register u64 r4 __asm__("x4") = 0;
    register u64 r5 __asm__("x5") = 0;
    register u64 r6 __asm__("x6") = 0;

    do {
        __asm__ volatile(
            __asmeq("%0", "x0")
            __asmeq("%1", "x1")
            __asmeq("%2", "x2")
            __asmeq("%3", "x3")
            __asmeq("%4", "x0")
            __asmeq("%5", "x1")
            __asmeq("%6", "x2")
            __asmeq("%7", "x3")
            __asmeq("%8", "x4")
            __asmeq("%9", "x5")
            __asmeq("%10", "x6")
            "smc    #0\n"
            : "=r"(r0), "=r"(r1), "=r"(r2), "=r"(r3)
            : "r"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5), "r"(r6));
    } while (r0 == 1);


    /* log(LOG_INFO, "edl_reboot(): smc done"); */

    pmic_reg_write(0, 8, 87, 0);

    /* log(LOG_INFO, "edl_reboot(): PMIC_WD_RESET_S2_CTL2 done"); */

    pmic_reg_write(0, 8, 91, 0);
    pmic_reg_write(2, 8, 91, 0);

    // TODO: better delay
    for (volatile int i = 0; i < 1000000; i++);

    pmic_reg_write(0, 8, 90, 1);
    pmic_reg_write(2, 8, 90, 1);

    pmic_reg_write(0, 8, 91, 1 << 7);
    pmic_reg_write(2, 8, 91, 1 << 7);

    /* log(LOG_INFO, "edl_reboot(): pmic_reset_configure done"); */

    writeu32(0x004AB000, 0);

    // судя по логам, выполнение доходит даже до сюда

    /* log(LOG_INFO, "edl_reboot(): MPM2_MPM_PS_HOLD done"); */
}

#define MIPI_DSI_BASE               (0x1A98000)
#define MIPI_DSI0_BASE              MIPI_DSI_BASE
#define MIPI_DSI1_BASE              (0x1A96000)
#define DSI0_PHY_BASE               (0x1A98500)
#define DSI1_PHY_BASE               (0x1A96400)
#define DSI0_PLL_BASE               (0x1A98300)
#define DSI1_PLL_BASE               (0x1A96A00)
#define DSI0_REGULATOR_BASE         (0x1A98780)
#define DSI1_REGULATOR_BASE         DSI0_REGULATOR_BASE
#define MDP_BASE                    (0x1A00000)
#define REG_MDP(off)                (MDP_BASE + (off))

#define MDP_VP_0_VIG_0_BASE                     REG_MDP(0x5000)
#define MDP_VP_0_RGB_0_BASE                     REG_MDP(0x15000)
#define MDP_VP_0_DMA_0_BASE                     REG_MDP(0x25000)

#define PIPE_SSPP_SRC0_ADDR                     0x14
#define PIPE_SSPP_SRC_YSTRIDE                   0x24
#define PIPE_SSPP_SRC_IMG_SIZE                  0x04
#define PIPE_SSPP_SRC_SIZE                      0x00
#define PIPE_SSPP_SRC_OUT_SIZE                  0x0C
#define PIPE_SSPP_SRC_XY                        0x08
#define PIPE_SSPP_OUT_XY                        0x10
#define PIPE_SSPP_SRC_FORMAT                    0x30
#define PIPE_SSPP_SRC_UNPACK_PATTERN            0x34
#define PIPE_SSPP_SRC_OP_MODE                   0x38
#define REQPRIORITY_FIFO_WATERMARK0        	0x50
#define REQPRIORITY_FIFO_WATERMARK1        	0x54
#define REQPRIORITY_FIFO_WATERMARK2        	0x58
#define PIPE_SW_PIXEL_EXT_C0_REQ                0x108
#define PIPE_SW_PIXEL_EXT_C1C2_REQ              0x118
#define PIPE_SW_PIXEL_EXT_C3_REQ                0x128
#define PIPE_COMP0_3_PHASE_STEP_X               0x210
#define PIPE_COMP0_3_PHASE_STEP_Y               0x214
#define PIPE_COMP1_2_PHASE_STEP_X               0x218
#define PIPE_COMP1_2_PHASE_STEP_Y               0x21c

#define PIPE_SRC_SIZE       PIPE_SSPP_SRC_SIZE
#define PIPE_SRC_IMG_SIZE   PIPE_SSPP_SRC_IMG_SIZE
#define PIPE_SRC_XY         PIPE_SSPP_SRC_XY
#define PIPE_SRC_OUT_SIZE   PIPE_SSPP_SRC_OUT_SIZE
#define PIPE_OUT_XY         PIPE_SSPP_OUT_XY
#define PIPE_SRC0_ADDR      PIPE_SSPP_SRC0_ADDR
#define PIPE_SRC_YSTRIDE    PIPE_SSPP_SRC_YSTRIDE
#define PIPE_SRC_FORMAT     PIPE_SSPP_SRC_FORMAT

#define MDP_CTL_0_BASE                          REG_MDP(0x2000)
#define CTL_LAYER_0                             0x00
#define CTL_LAYER_1                             0x04
#define CTL_TOP                                 0x14
#define CTL_FLUSH                               0x18
#define CTL_START                               0x1C


void main() {
    clear_debug_buffer();

    log(LOG_INFO, "hello from main");

    u64 current_el;
    asm("mrs %0, CurrentEL" : "=r"(current_el));
    logf(LOG_INFO, "current EL: %z", current_el >> 2);

    logf(LOG_INFO, "VIG_0: %x", readu32(MDP_VP_0_VIG_0_BASE + PIPE_SRC0_ADDR));
    logf(LOG_INFO, "RGB_0: %x", readu32(MDP_VP_0_RGB_0_BASE + PIPE_SRC0_ADDR));
    logf(LOG_INFO, "DMA_0: %x", readu32(MDP_VP_0_DMA_0_BASE + PIPE_SRC0_ADDR));

    u32 src_size = readu32(MDP_VP_0_RGB_0_BASE + PIPE_SRC_SIZE);
    u32 img_size = readu32(MDP_VP_0_RGB_0_BASE + PIPE_SRC_IMG_SIZE);
    u32 src_xy = readu32(MDP_VP_0_RGB_0_BASE + PIPE_SRC_XY);
    u32 out_size = readu32(MDP_VP_0_RGB_0_BASE + PIPE_SRC_OUT_SIZE);
    u32 out_xy = readu32(MDP_VP_0_RGB_0_BASE + PIPE_OUT_XY);
    u32 stride = readu32(MDP_VP_0_RGB_0_BASE + PIPE_SRC_YSTRIDE);
    u32 format = readu32(MDP_VP_0_RGB_0_BASE + PIPE_SRC_FORMAT);
    u32 width = img_size & 0xffff;
    u32 height = (img_size >> 16) & 0xffff;

    logf(LOG_INFO, "src_size: %d", src_size);
    logf(LOG_INFO, "img_size: %d", img_size);
    logf(LOG_INFO, "src_xy: %d", src_xy);
    logf(LOG_INFO, "out_size: %d", out_size);
    logf(LOG_INFO, "out_xy: %d", out_xy);
    logf(LOG_INFO, "stride: %d", stride);
    logf(LOG_INFO, "format: %d", format);
    logf(LOG_INFO, "width: %d", width);
    logf(LOG_INFO, "height: %d", height);

    u32* base = (u32*)(u64)readu32(MDP_VP_0_RGB_0_BASE + PIPE_SRC0_ADDR);

    for (u32 x = 100; x < 200; x++) {
        for (u32 y = 100; y < 200; y++) {
            *(base + x + y * (2160 / 4)) = 0xffffffff;
        }
    }

    writeu32(MDP_CTL_0_BASE + CTL_START, 1);

    for (volatile u32 i = 0; i < 30000000; i++);

    edl_reboot();

    while (1);
}
