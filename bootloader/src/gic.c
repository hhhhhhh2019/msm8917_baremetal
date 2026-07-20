#include "gic.h"
#include "interrupts.h"
#include "log.h"
#include "stdint.h"
#include "utils.h"

void gic_unmask_interrupt(u32 vector) {
    /* volatile u8* priority = (void*)GICD_PRIORITY; */
    /* volatile u8* targets = (void*)GICD_TARGET; */
    /* volatile u8* enables = (void*)GICD_ENABLE_SET; */
    /* volatile u8* enablec = (void*)GICD_ENABLE_CLR; */

    /* priority[vector] = 0xA0; */
    /* targets[vector] = 0x01; */
    /* enables[vector / 8] |= (1 << (vector % 8)); */

    u32 *reg = (u32*)GICD_ENABLE_SET(vector / 32);
    *reg |= 1 << (vector % 32);
}

void gic_init() {
    /* writeu32(GICD_CTLR, 0); */

    /* logf(LOG_INFO, "gic iidr: %x", GICD_BASE + 8); */

    /* for (u32 i = 0; i < 128 / 4; i += 4) */
    /*     writeu32(GICD_ENABLE_CLR(0) + i, 0xffffffff); */

    /* writeu32(GICD_CTLR, 1); */

    /* log(LOG_INFO, "gic initialized"); */

    writeu32(GICD_CTLR, 0);

    u32 gicd_typer = readu32(GICD_TYPER);
    u32 ints_count = 32 * (bits(gicd_typer, 4, 0) + 1);
    u32 cpus_count = bits(gicd_typer, 7, 5) + 1;

    logf(LOG_INFO, "gicd typer: %x", gicd_typer);

    // disable all interrupts(1 bit - 1 interrupt)
    for (u32 i = 0; i < ints_count / 32; i++) {
        writeu32(GICD_ENABLE_CLR(i), 0xffffffff);
    }

    // set equal priority(1 interrupt - 8 bits)
    for (u32 i = 0; i < ints_count / 4; i++) {
        writeu32(GICD_PRIORITY(i), 0xa0a0a0a0);
    }

    // set target to cpu0
    for (u32 i = 8; i < ints_count / 4; i++) {
        writeu32(GICD_TARGET(i), 0x01010101);
    }

    writeu32(GICD_CTLR, 1);

    log(LOG_INFO, "gic initialized");
}
