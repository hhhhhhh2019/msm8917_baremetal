#include "interrupts.h"
#include "gic.h"
#include "log.h"
#include "utils.h"

static irq_handler irq_handlers[512] = {0};

void int_sync_handler(u64 esr, u64 elr, u64 spsr, u64 far) {
    logf(LOG_INFO, "synchronous: esr: %x elr: %x spsr: %x far: %x", esr, elr, spsr, far);
    edl_reboot();
}

void int_irq_handler(struct registers* regs) {
    u32 iar = readu32(GICC_IAR);
    u32 irq = iar & 0x3ff;
    logf(LOG_INFO, "irq: %d, iar: %x", irq, iar);

    logf(LOG_INFO, "x0: %X", regs->x0);
    logf(LOG_INFO, "x1: %X", regs->x1);
    logf(LOG_INFO, "x2: %X", regs->x2);
    logf(LOG_INFO, "x3: %X", regs->x3);
    logf(LOG_INFO, "x4: %X", regs->x4);
    logf(LOG_INFO, "x5: %X", regs->x5);
    logf(LOG_INFO, "x6: %X", regs->x6);
    logf(LOG_INFO, "x7: %X", regs->x7);
    logf(LOG_INFO, "x8: %X", regs->x8);
    logf(LOG_INFO, "x9: %X", regs->x9);
    logf(LOG_INFO, "x10: %X", regs->x10);
    logf(LOG_INFO, "x11: %X", regs->x11);
    logf(LOG_INFO, "x12: %X", regs->x12);
    logf(LOG_INFO, "x13: %X", regs->x13);
    logf(LOG_INFO, "x14: %X", regs->x14);
    logf(LOG_INFO, "x15: %X", regs->x15);
    logf(LOG_INFO, "x16: %X", regs->x16);
    logf(LOG_INFO, "x17: %X", regs->x17);
    logf(LOG_INFO, "x18: %X", regs->x18);
    logf(LOG_INFO, "x19: %X", regs->x19);
    logf(LOG_INFO, "x20: %X", regs->x20);
    logf(LOG_INFO, "x21: %X", regs->x21);
    logf(LOG_INFO, "x22: %X", regs->x22);
    logf(LOG_INFO, "x23: %X", regs->x23);
    logf(LOG_INFO, "x24: %X", regs->x24);
    logf(LOG_INFO, "x25: %X", regs->x25);
    logf(LOG_INFO, "x26: %X", regs->x26);
    logf(LOG_INFO, "x27: %X", regs->x27);
    logf(LOG_INFO, "x28: %X", regs->x28);
    logf(LOG_INFO, "x29: %X", regs->x29);
    logf(LOG_INFO, "x30: %X", regs->x30);
    logf(LOG_INFO, "sp: %X", regs->sp);
    logf(LOG_INFO, "elr: %X", regs->elr);
    logf(LOG_INFO, "spsr: %X", regs->spsr);

    if (irq_handlers[irq] != 0)
        irq_handlers[irq](irq, regs);
    else
        logf(LOG_ERROR, "no handler for irq %d", irq);

    writeu32(GICC_EOIR, iar);
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
