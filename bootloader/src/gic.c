#include "gic.h"
#include "interrupts.h"
#include "log.h"
#include "stdint.h"
#include "utils.h"

void gic_unmask_interrupt(u32 vector) {
    u32 *reg = (u32*)GICD_ENABLE_SET(vector / 32);
    *reg |= 1 << (vector % 32);
}

void gic_init() {
    writeu32(GICD_CTLR, 0);

    u32 gicd_typer = readu32(GICD_TYPER);
    u32 ints_count = 32 * (bits(gicd_typer, 4, 0) + 1);
    u32 cpus_count = bits(gicd_typer, 7, 5) + 1;

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
}
