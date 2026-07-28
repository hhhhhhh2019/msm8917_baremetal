#include "spmi.h"
#include "log.h"
#include "stdint.h"
#include "task.h"
#include "utils.h"
#include "gpio.h"
#include "qtimer.h"
#include "interrupts.h"
#include "gic.h"
#include "fb.h"

#define __asmeq(x, y)  ".ifnc " x "," y " ; .err ; .endif\n\t"

void edl_reboot() {
    asm volatile("msr daifset, #15" ::: "memory");

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

    pmic_reg_write(0, 8, 87, 0);

    pmic_reg_write(0, 8, 91, 0);
    pmic_reg_write(2, 8, 91, 0);

    /* // TODO: better delay */
    /* for (volatile int i = 0; i < 2000000; i++); */

    pmic_reg_write(0, 8, 90, 1);
    pmic_reg_write(2, 8, 90, 1);

    /* pmic_reg_write(0, 8, 91, 1 << 7); */
    /* pmic_reg_write(2, 8, 91, 1 << 7); */

    // наделал какую-то дичь, но теперь оно стабильно перезагружается
    __asm__ volatile ("isb");
    __asm__ volatile ("dsb sy");

    writeu32(0x193d100, 1);
    writeu32(0x004AB000, 0);

    writeu32(0x004AB000, 0);

    __asm__ volatile ("isb");
    __asm__ volatile ("dsb sy");

    while (1) { asm volatile("wfi"); };
}

void fb_put_hex(u64 num, u32 chars);

void main() {
    fb_init();
    fb_init_addres((void*)0x90001000);

    pmic_arb_init();

    u32 status = 0;
    pmic_arb_read(0, 0x800, 1, (u8*)&status);
    fb_put_hex(status, 9);

    fb_put_char('\n');


    for (volatile u32 i = 0; i < 1000; i++) {
        u32 status = 0;
        pmic_arb_read(0, 0x810, 1, (u8*)&status);
        fb_put_hex(status, 9);
        /* fb_put_char((status & 3) + '0'); */
        for (volatile u32 j = 0; j < 10000; j++);
    }

    edl_reboot();
}
