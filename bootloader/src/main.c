#include "gic.h"
#include "spmi.h"
#include "log.h"
#include "utils.h"
#include "gpio.h"

#define __asmeq(x, y)  ".ifnc " x "," y " ; .err ; .endif\n\t"

#define QTIMER_FREQ 0x124f800

#define APPS_APCS_F0_QTMR_V1_BASE          0xb021000
#define QTMR_BASE                          APPS_APCS_F0_QTMR_V1_BASE

#define QTMR_V1_CNTPCT_LO                (0x00000000 + QTMR_BASE)
#define QTMR_V1_CNTPCT_HI                (0x00000004 + QTMR_BASE)
#define QTMR_V1_CNTFRQ                   (0x00000010 + QTMR_BASE)
#define QTMR_V1_CNTP_CVAL_LO             (0x00000020 + QTMR_BASE)
#define QTMR_V1_CNTP_CVAL_HI             (0x00000024 + QTMR_BASE)
#define QTMR_V1_CNTP_TVAL                (0x00000028 + QTMR_BASE)
#define QTMR_V1_CNTP_CTL                 (0x0000002C + QTMR_BASE)

#define QTMR_TIMER_CTRL_ENABLE          (1 << 0)
#define QTMR_TIMER_CTRL_INT_MASK        (1 << 1)

#define GIC_PPI_START                          16
#define GIC_SPI_START                          32

#define INT_QTMR_NON_SECURE_PHY_TIMER_EXP      (GIC_PPI_START + 3)
#define INT_QTMR_VIRTUAL_TIMER_EXP             (GIC_PPI_START + 4)

#define INT_QTMR_FRM_0_PHYSICAL_TIMER_EXP      (GIC_SPI_START + 257)

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

void set_vbar(u64 addr);
u64 get_vbar();

void set_vector_table(u64* addr) {
    /* u64* base = (void*)0x80000000; */
    /* u64 base = get_vbar(); */
    /* logf(LOG_INFO, "%X", base); */
    /* logf(LOG_INFO, "%X", addr); */
    set_vbar((u64)addr);
    /* addr = 0; */
    /* logf(LOG_INFO, "%X", addr); */
    /* addr = (void*)get_vbar(); */
    /* logf(LOG_INFO, "%X", addr); */
}

u8 qgic_get_cpumask() {
    for (u32 i = 0; i < 32; i += 4) {
        u32 mask = 0;
        mask = readu32(GIC_DIST_TARGET + i);
        mask |= mask >> 16;
        mask |= mask >> 8;
        if (mask)
            return mask;
    }
    return 0;
}

void qgic_dist_init() {
    u32 cpumask = qgic_get_cpumask();
    cpumask |= cpumask << 8;
    cpumask |= cpumask << 16;
    logf(LOG_INFO, "cpumask: %x", cpumask);

    u32 gic_ctrl = readu32(GIC_DIST_CTRL);
    logf(LOG_INFO, "gic ctrl: %x", gic_ctrl);

    writeu32(GIC_DIST_CTRL, 0);

    u32 num_irq = readu32(GIC_DIST_CTR) & 0x1f;
    logf(LOG_INFO, "num irq: %z", num_irq);

    for (u32 i = 0; i < num_irq; i += 4) {
        writeu32(GIC_DIST_TARGET + i, cpumask);
        /* logf(LOG_INFO, "target %z: %x", i, readu32(GIC_DIST_TARGET + i)); */
    }

    for (u32 i = 32; i < num_irq; i += 16)
      writeu32(GIC_DIST_CONFIG + i * 4 / 16, 0xffffffff);

    writeu32(GIC_DIST_CONFIG + 4, 0xffffffff);

    /* Set priority of all interrupts */

    /*
     * In bootloader we dont care about priority so
     * setting up equal priorities for all
     */
    for (u32 i = 0; i < num_irq; i += 4)
      writeu32(GIC_DIST_PRI + i, 0xa0a0a0a0);

    /* Disabling interrupts */
    for (u32 i = 0; i < num_irq; i += 32)
      writeu32(GIC_DIST_ENABLE_CLEAR + i * 4 / 32, 0xffffffff);

    writeu32(GIC_DIST_ENABLE_SET, 0x0000ffff);

    writeu32(GIC_DIST_CTRL, 1);
}

void qgic_cpu_init(void) {
    writeu32(GIC_CPU_PRIMASK, 0xf0);
    writeu32(GIC_CPU_CTRL, 1);
}

// void enable_interrupts()

void qtimer_disable() {
    u32 ctrl = readu32(QTMR_V1_CNTP_CTL);

    log(LOG_INFO, "disable qtimer");
    logf(LOG_INFO, "qtimer ctrl: %x", ctrl);

    ctrl &= ~QTMR_TIMER_CTRL_ENABLE;
    ctrl |= QTMR_TIMER_CTRL_INT_MASK;

    writeu32(QTMR_V1_CNTP_CTL, ctrl);

    asm("isb");
    // dsb
}

void qtimer_enable() {
    u32 ctrl = readu32(QTMR_V1_CNTP_CTL);

    log(LOG_INFO, "enable qtimer");
    logf(LOG_INFO, "qtimer ctrl: %x", ctrl);

    ctrl |= QTMR_TIMER_CTRL_ENABLE;
    ctrl &= ~QTMR_TIMER_CTRL_INT_MASK;

    writeu32(QTMR_V1_CNTP_CTL, ctrl);

    asm("isb");
    // dsb
}

void gic_unmask_interrupt(unsigned int vector) {
    u32 reg = GIC_DIST_ENABLE_SET + (vector / 32) * 4;
    u32 bit = 1 << (vector & 31);

    writeu32(reg, bit);
}

extern u64 vector_table;

void synchronous(u64 esr, u64 elr, u64 spsr, u64 far) {
    logf(LOG_INFO, "synchronous: esr: %x elr: %x spsr: %x far: %x", esr, elr, spsr, far);
    edl_reboot();
}

void start_timer();

u32 tick = 0;

void irq() {
    u32 iar = readu32(GIC_CPU_INTACK);
    logf(LOG_INFO, "irq: %x, tick: %d", iar & 0x3ff, tick);

    if (tick == 10) {
        edl_reboot();
        while (1);
    }
    tick++;

    start_timer();
    writeu32(GIC_CPU_EOI, iar);
    /* edl_reboot(); */
}

void fiq() {
    log(LOG_INFO, "fiq");
    edl_reboot();
}

void serror() {
    log(LOG_INFO, "serror");
    edl_reboot();
}

void start_timer() {
    qtimer_disable();
    u32 msecs_interval = 100000;
    u32 tick_count = msecs_interval * QTIMER_FREQ / 1000;
    writeu32(QTMR_V1_CNTP_TVAL, 10000000);
    qtimer_enable();
}

void main() {
    clear_debug_buffer();
    tick = 0;

    log(LOG_INFO, "hello from main");
    logf(LOG_INFO, "vector_table: %X", &vector_table);

    set_vector_table(&vector_table);
    qgic_dist_init();
    qgic_cpu_init();
    /* gic_unmask_interrupt(19); */

    start_timer();

    /*
     D or bit 9 - when it’s set to 1, debug exceptions are masked;
     A or bit 8 - when it’s set to 1, SError is masked;
     I or bit 7 - when it’s set to 1, IRQs are masked;
     F or bit 6 - when it’s set to 1, FIQs are masked.
     */
    asm volatile("msr daifset, #15" ::: "memory");
    asm volatile("msr daifclr, #15" ::: "memory");

    /* for (volatile u32 i = 0; i < 10000000; i++); */
    while (1);

    log(LOG_INFO, "reboot");

    edl_reboot();

    while (1);
}
