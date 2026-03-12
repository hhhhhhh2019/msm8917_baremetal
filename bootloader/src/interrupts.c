#include "interrupts.h"
#include "gic.h"
#include "log.h"
#include "utils.h"

static irq_handler irq_handlers[512];

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

void gic_unmask_interrupt(unsigned int vector) {
    u32 reg = GIC_DIST_ENABLE_SET + (vector / 32) * 4;
    u32 bit = 1 << (vector & 31);

    writeu32(reg, bit);
}

void int_sync_handler(u64 esr, u64 elr, u64 spsr, u64 far) {
    logf(LOG_INFO, "synchronous: esr: %x elr: %x spsr: %x far: %x", esr, elr, spsr, far);
    edl_reboot();
}

void int_irq_handler(struct registers* regs) {
    u32 iar = readu32(GIC_CPU_INTACK);
    u32 irq = iar & 0x3ff;
    logf(LOG_INFO, "irq: %x, iar: %x", irq, iar);

    irq_handlers[irq](irq, regs);

    writeu32(GIC_CPU_EOI, iar);
}

void int_fiq_handler() {
    logf(LOG_INFO, "fiq");
    edl_reboot();
}

void int_serror_handler() {
    logf(LOG_INFO, "serror");
    edl_reboot();
}

void set_irq_handler(u32 irq, irq_handler handler) {
    if (irq > sizeof(irq_handlers) / sizeof(irq_handler)) {
        logf(LOG_INFO, "too big irq number: %d", irq);
        return;
    }

    irq_handlers[irq] = handler;
}
