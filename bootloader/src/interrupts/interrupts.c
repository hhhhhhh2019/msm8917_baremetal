#include "interrupts/interrupts.h"
#include "devices/gic.h"
#include "graphics/fb.h"
#include "graphics/log.h"

static irq_handler irq_handlers[512] = {0};

void int_sync_handler(u64 esr, u64 elr, u64 spsr, u64 far) {
    printf("\nsynchronous: esr: %x elr: %x spsr: %x far: %x\n", esr, elr, spsr,
           far);
    fb_flush();
}

void int_irq_handler(struct registers* regs) {
    u32 iar = *GICC_IAR;
    u32 irq = iar & 0x3ff;

    /* asm volatile("nop"); */
    /* asm volatile("nop"); */
    /* asm volatile("nop"); */
    /* asm volatile("nop"); */
    /* asm volatile("nop"); */

    if (irq_handlers[irq] != 0) {
        /* asm volatile(".balign 64; nop" ::: "memory"); */
        irq_handlers[irq](irq, regs);
    } else {
        printf("\nno handler for irq %d\n", irq);
        fb_flush();
    }

    *GICC_EOIR = iar;
}

void int_fiq_handler() {
    printf("fiq");
    fb_flush();
}

void int_serror_handler() {
    printf("serror");
    fb_flush();
}

void set_irq_handler(u32 irq, irq_handler handler) {
    if (irq > sizeof(irq_handlers) / sizeof(irq_handler)) {
        printf("\ntoo big irq number: %d", irq);
        fb_flush();
        return;
    }

    irq_handlers[irq] = handler;
}
