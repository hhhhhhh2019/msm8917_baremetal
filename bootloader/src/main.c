#include "spmi.h"
#include "log.h"
#include "task.h"
#include "utils.h"
#include "gpio.h"
#include "qtimer.h"
#include "interrupts.h"
#include "gic.h"

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
    tick++;
    logf(LOG_INFO, "tick: %d", tick);

    /* u8* fb = (u8*)0x90001000; */
    /* for (u32 y = 20; y < 40; y++) */
    /*     for (u32 x = 20; x < 40; x++) { */
    /*         u32 offset = (x + y * 720) * 3; */
    /*         fb[offset + 0] += 100; */
    /*   } */

    if (tick == 3) {
        edl_reboot();
        while (1);
    }

    start_timer(1000);

    schedule(regs);
}

void foo();

void main() {
    clear_debug_buffer();
    tick = 0;

    log(LOG_INFO, "hello from main");
    logf(LOG_INFO, "vector_table: %X", &vector_table);

    init_scheduler();

    tlmm_mode(93, GPIO_OUTPUT);

    set_vector_table(&vector_table);
    gic_init();

    // если включать все, то помимо таймера сыпет еще кучу прерываний
    /* for (int i = 0; i < 128; i++) */
    /*     gic_unmask_interrupt(i); */
    gic_unmask_interrupt(19);

    set_irq_handler(289, &timer_handler);

    /*
     D or bit 9 - when it’s set to 1, debug exceptions are masked;
     A or bit 8 - when it’s set to 1, SError is masked;
     I or bit 7 - when it’s set to 1, IRQs are masked;
     F or bit 6 - when it’s set to 1, FIQs are masked.
     */
    asm volatile("msr daifset, #15" ::: "memory");
    asm volatile("msr daifclr, #15" ::: "memory");

    start_timer(1);

    for (volatile u32 i = 0; i < 80; i++) {
        for (volatile u32 i = 0; i < 200000; i++);
        tlmm_cfg(93, GPIO_NO_PULL, GPIO_FUNC_GPIO, GPIO_2MA, GPIO_ENABLE);
        for (volatile u32 i = 0; i < 200000; i++);
        tlmm_cfg(93, GPIO_NO_PULL, GPIO_FUNC_GPIO, GPIO_2MA, GPIO_DISABLE);
    }
    /* for (volatile u32 i = 0; i < 10000000; i++); */

    log(LOG_INFO, "reboot");

    edl_reboot();
}
