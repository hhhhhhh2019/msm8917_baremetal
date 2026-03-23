#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "stdint.h"

extern u64 vector_table;

struct registers {
    u64 x0;
    u64 x1;
    u64 x2;
    u64 x3;
    u64 x4;
    u64 x5;
    u64 x6;
    u64 x7;
    u64 x8;
    u64 x9;
    u64 x10;
    u64 x11;
    u64 x12;
    u64 x13;
    u64 x14;
    u64 x15;
    u64 x16;
    u64 x17;
    u64 x18;
    u64 x19;
    u64 x20;
    u64 x21;
    u64 x22;
    u64 x23;
    u64 x24;
    u64 x25;
    u64 x26;
    u64 x27;
    u64 x28;
    u64 x29;
    u64 x30;
    u64 sp;
    u64 elr_el1;
};

typedef void (*irq_handler)(u32 irq, struct registers*);

// implemented in src/head.S
void set_vbar(u64 addr);
u64 get_vbar();

static inline void set_vector_table(u64* addr) {
    set_vbar((u64)addr);
}

u8 qgic_get_cpumask();
void qgic_dist_init();
void qgic_cpu_init();
void gic_unmask_interrupt(u32 vector);
void int_sync_handler(u64 esr, u64 elr, u64 spsr, u64 far);
void int_irq_handler(struct registers*);
void int_fiq_handler();
void int_serror_handler();

void set_irq_handler(u32 irq, irq_handler handler);

#endif // INTERRUPTS_H_
