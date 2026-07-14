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
#include "mmu.h"

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

    /* log(LOG_INFO, "edl_reboot(): smc done"); */

    pmic_reg_write(0, 8, 87, 0);

    /* log(LOG_INFO, "edl_reboot(): PMIC_WD_RESET_S2_CTL2 done"); */

    pmic_reg_write(0, 8, 91, 0);
    pmic_reg_write(2, 8, 91, 0);

    // TODO: better delay
    for (volatile int i = 0; i < 2000000; i++);

    pmic_reg_write(0, 8, 90, 1);
    pmic_reg_write(2, 8, 90, 1);

    /* pmic_reg_write(0, 8, 91, 1 << 7); */
    /* pmic_reg_write(2, 8, 91, 1 << 7); */

    // наделал какую-то дичь, но теперь оно стабильно перезагружается
    __asm__ volatile ("isb");
    __asm__ volatile ("dsb sy");

    writeu32(0x193d100, 1);
    writeu32(0x004AB000, 0);

    /* log(LOG_INFO, "edl_reboot(): pmic_reset_configure done"); */

    writeu32(0x004AB000, 0);

    while (1);

    // судя по логам, выполнение доходит даже до сюда

    /* log(LOG_INFO, "edl_reboot(): MPM2_MPM_PS_HOLD done"); */
}

u32 tick;

void timer_handler(u32 irq, struct registers *regs) {
    edl_reboot();
    /* tick++; */
    /* /\* logf(LOG_INFO, "tick: %d", tick); *\/ */

    /* #define delay 1000 */

    /* if (tick == 10*1000/delay) { */
    /*     edl_reboot(); */
    /*     while (1); */
    /* } */

    /* start_timer(delay); */

    /* schedule(regs); */
}

void task(void* userdata) {
    /* u8* fb = (u8*)0x90001000; */

    /* for (u32 y = 0; y < 1280; y++) { */
    /*     for (u32 x = 0; x < 720; x++) { */
    /*         u32 offset = (x + y * 720) * 3; */
    /*         fb[offset + 1] += 73; */
    /*         fb[offset + 1] += 73; */
    /*         fb[offset + 2] += 73; */
    /*     } */
    /* } */
}

void main() {
    tick = 0;

    fb_init();
    fb_init_addres((void*)0x90001000);

    u64 current_el;
    asm("mrs %0, CurrentEL" : "=r"(current_el));
    current_el = (current_el >> 2) & 0x3;

    fb_put_char('0' + current_el);

    for (u32 a = 0; a < 5; a++) {
        for (volatile u32 i = 0; i < 1000; i++);
        fb_put_char('!');
    }

    mmu_init();

    /* set_vector_table(&vector_table); */
    /* gic_init(); */

    /*
     D or bit 9 - when it’s set to 1, debug exceptions are masked;
     A or bit 8 - when it’s set to 1, SError is masked;
     I or bit 7 - when it’s set to 1, IRQs are masked;
     F or bit 6 - when it’s set to 1, FIQs are masked.
     */
    /* asm volatile("msr daifset, #15" ::: "memory"); */
    /* asm volatile("msr daifclr, #15" ::: "memory"); */

    while (1) {
        fb_put_char('b');
        for (volatile u32 i = 0; i < 1000; i++);
    }

    log(LOG_INFO, "reboot");

    edl_reboot();
}
