#include "spmi.h"
#include "log.h"
#include "utils.h"
#include "gpio.h"
#include "qtimer.h"
#include "interrupts.h"

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
    while (1);

    // судя по логам, выполнение доходит даже до сюда

    /* log(LOG_INFO, "edl_reboot(): MPM2_MPM_PS_HOLD done"); */
}

u32 tick;

void timer_handler(u32 irq, struct registers *regs) {
    logf(LOG_INFO, "x0: %X", regs->x0);
    logf(LOG_INFO, "x1: %X", regs->x1);
    /* logf(LOG_INFO, "x2: %X", regs->x2); */
    /* logf(LOG_INFO, "x3: %X", regs->x3); */
    /* logf(LOG_INFO, "x4: %X", regs->x4); */
    /* logf(LOG_INFO, "x5: %X", regs->x5); */
    /* logf(LOG_INFO, "x6: %X", regs->x6); */
    /* logf(LOG_INFO, "x7: %X", regs->x7); */
    /* logf(LOG_INFO, "x8: %X", regs->x8); */
    /* logf(LOG_INFO, "x9: %X", regs->x9); */
    /* logf(LOG_INFO, "x10: %X", regs->x10); */
    /* logf(LOG_INFO, "x11: %X", regs->x11); */
    /* logf(LOG_INFO, "x12: %X", regs->x12); */
    /* logf(LOG_INFO, "x13: %X", regs->x13); */
    /* logf(LOG_INFO, "x14: %X", regs->x14); */
    /* logf(LOG_INFO, "x15: %X", regs->x15); */
    /* logf(LOG_INFO, "x16: %X", regs->x16); */
    /* logf(LOG_INFO, "x17: %X", regs->x17); */
    /* logf(LOG_INFO, "x18: %X", regs->x18); */
    /* logf(LOG_INFO, "x19: %X", regs->x19); */
    /* logf(LOG_INFO, "x20: %X", regs->x20); */
    /* logf(LOG_INFO, "x21: %X", regs->x21); */
    /* logf(LOG_INFO, "x22: %X", regs->x22); */
    /* logf(LOG_INFO, "x23: %X", regs->x23); */
    /* logf(LOG_INFO, "x24: %X", regs->x24); */
    /* logf(LOG_INFO, "x25: %X", regs->x25); */
    /* logf(LOG_INFO, "x26: %X", regs->x26); */
    /* logf(LOG_INFO, "x27: %X", regs->x27); */
    /* logf(LOG_INFO, "x28: %X", regs->x28); */
    /* logf(LOG_INFO, "x29: %X", regs->x29); */
    /* logf(LOG_INFO, "x30: %X", regs->x30); */
    /* logf(LOG_INFO, "xzr: %X", regs->xzr); */

    if (tick == 3) {
        edl_reboot();
        while (1);
    }
    tick++;

    start_timer(100);

    /* if (tick % 2 == 1) */
    /*     tlmm_cfg(93, GPIO_NO_PULL, GPIO_FUNC_GPIO, GPIO_2MA, GPIO_ENABLE); */
    /* else */
    /*     tlmm_cfg(93, GPIO_NO_PULL, GPIO_FUNC_GPIO, GPIO_2MA, GPIO_DISABLE); */

    /* edl_reboot(); */
}

void foo();

void main() {
    clear_debug_buffer();
    tick = 0;

    log(LOG_INFO, "hello from main");
    logf(LOG_INFO, "vector_table: %X", &vector_table);

    set_vector_table(&vector_table);
    qgic_dist_init();
    qgic_cpu_init();

    set_irq_handler(289, &timer_handler);

    /*
     D or bit 9 - when it’s set to 1, debug exceptions are masked;
     A or bit 8 - when it’s set to 1, SError is masked;
     I or bit 7 - when it’s set to 1, IRQs are masked;
     F or bit 6 - when it’s set to 1, FIQs are masked.
     */
    /* asm volatile("msr daifset, #15" ::: "memory"); */
    asm volatile("msr daifclr, #15" ::: "memory");

    start_timer(100);

    foo();

    log(LOG_INFO, "reboot");

    edl_reboot();
}
