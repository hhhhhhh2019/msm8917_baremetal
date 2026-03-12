#include "gic.h"
#include "interrupts.h"
#include "log.h"
#include "stdint.h"
#include "utils.h"

void gic_unmask_interrupt(u32 vector) {
    volatile u8* priority = (void*)GICD_PRIORITY;
    volatile u8* targets = (void*)GICD_TARGET;
    volatile u8* enables = (void*)GICD_ENABLE_SET;
    volatile u8* enablec = (void*)GICD_ENABLE_SET;

    priority[vector] = 0xA0;
    targets[vector] = 0x01;
    enables[vector / 8] |= (1 << (vector % 8));
}

void gic_init() {
    writeu32(GICD_CTLR, 0);

    volatile u8* priority = (void*)GICD_PRIORITY;
    volatile u8* targets = (void*)GICD_TARGET;
    volatile u8* enables = (void*)GICD_ENABLE_SET;
    volatile u8* enablec = (void*)GICD_ENABLE_SET;

    writeu32(GICD_CTLR, 1);
    writeu32(GICC_PMR, 0xff);
    writeu32(GICC_CTLR, 1);
}
