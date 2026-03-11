#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "stdint.h"

extern u64 vector_table;

typedef void (*irq_handler)(u32 irq);

// implemented in src/head.S
void set_vbar(u64 addr);
u64 get_vbar();

static void set_vector_table(u64* addr) {
    set_vbar((u64)addr);
}

u8 qgic_get_cpumask();
void qgic_dist_init();
void qgic_cpu_init();
void gic_unmask_interrupt(u32 vector);
void int_sync_handler(u64 esr, u64 elr, u64 spsr, u64 far);
void int_irq_handler();
void int_fiq_handler();
void int_serror_handler();

void set_irq_handler(u32 irq, irq_handler handler);

#endif // INTERRUPTS_H_
