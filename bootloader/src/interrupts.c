#include "interrupts.h"
#include "gic.h"
#include "log.h"
#include "utils.h"

static irq_handler irq_handlers[512];

void int_sync_handler(u64 esr, u64 elr, u64 spsr, u64 far) {
    logf(LOG_INFO, "synchronous: esr: %x elr: %x spsr: %x far: %x", esr, elr, spsr, far);
    edl_reboot();
}

void int_irq_handler(struct registers* regs) {
    u32 iar = readu32(GICC_IAR);
    u32 irq = iar & 0x3ff;
    logf(LOG_INFO, "irq: %x, iar: %x", irq, iar);

    irq_handlers[irq](irq, regs);

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
