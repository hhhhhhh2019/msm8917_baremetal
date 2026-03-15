#include "gic.h"
#include "interrupts.h"
#include "log.h"
#include "stdint.h"
#include "utils.h"

void gic_unmask_interrupt(u32 vector) {
    volatile u8* priority = (void*)GICD_PRIORITY;
    volatile u8* targets = (void*)GICD_TARGET;
    volatile u8* enables = (void*)GICD_ENABLE_SET;
    volatile u8* enablec = (void*)GICD_ENABLE_CLR;

    priority[vector] = 0xA0;
    targets[vector] = 0x01;
    enables[vector / 8] |= (1 << (vector % 8));
}

void gic_init() {
    writeu32(GICD_CTLR, 0);

    logf(LOG_INFO, "gic iidr: %x", GICD_BASE + 8);

    for (u32 i = 0; i < 128 / 8; i += 4)
        writeu32(GICD_ENABLE_CLR + i, 0xffffffff);

    writeu32(GICD_CTLR, 1);

    log(LOG_INFO, "gic initialized");
}
