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

#define GCC_BASE 0x1800000
#define GCC_SLEEP_CLK_OFFSET 0x21000
#define GCC_SLEEP_CLK_REG 0x45008
#define GCC_SLEEP_CLK_ENABLE (1 << 23)

void main() {
    clear_debug_buffer();

    log(LOG_INFO, "hello from main");

    u64 current_el;
    asm("mrs %0, CurrentEL" : "=r"(current_el));
    logf(LOG_INFO, "current EL: %z", current_el >> 2);

    logf(LOG_INFO, "i32 test: %d", -123);
    logf(LOG_INFO, "u32 test: %z", -123);
    logf(LOG_INFO, "i64 test: %l", -123l);
    logf(LOG_INFO, "u64 test: %u", -123l);
    logf(LOG_INFO, "hex32 test: %x", -123);
    logf(LOG_INFO, "hex64 test: %X", -123l);
    logf(LOG_INFO, "str test: %s %s", "hello", "world");


    edl_reboot();

    while (1);
}
