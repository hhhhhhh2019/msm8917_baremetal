#include "boot/mmu.h"
#include "devices/gic.h"
#include "devices/qtimer.h"
#include "graphics/fb.h"
#include "graphics/log.h"
#include "interrupts/interrupts.h"

void timer_handler(u32 irq, struct registers*) {
    start_timer(1000);
    putc('t');
    fb_flush();
}

void hang();

void main() {
    // раскомментировать, если текст не выводится
    // TODO: починить bss либо поянть, почему caret сам не обнуляется
    caret_move(0, 0);

    fb_draw_at((void*)0x90001000ULL);

    puts("start\n");

    /* for (u32 level = 0; level < 4; level++) { */
    /*     asm volatile("MSR CSSELR_EL1, %0" :: "r"(level)); */
    /*     u32 ccsidr; */
    /*     asm volatile("mrs %0, CCSIDR_EL1" : "=r"(ccsidr)); */
    /*     printf("cache: %u: %lx\n", level, ccsidr); */
    /* } */

    /* hang(); */

    /* u64 el; */
    /* asm volatile("mrs %0, CurrentEL" : "=r"(el)); */
    /* printf("CurrentEL: %lu\n", (el >> 2) & 3); */

    /* u64 midr; */
    /* asm volatile("mrs %0, MIDR_EL1" : "=r"(midr)); */
    /* printf("MIDR: %lx\n", midr); */

    /* hang(); */

    set_vector_table(&vector_table);
    gic_init();

    /*
     D or bit 9 - when it’s set to 1, debug exceptions are masked;
     A or bit 8 - when it’s set to 1, SError is masked;
     I or bit 7 - when it’s set to 1, IRQs are masked;
     F or bit 6 - when it’s set to 1, FIQs are masked.
     */
    asm volatile("msr daifclr, #15" ::: "memory");

#ifdef CONFIG_MMU
    mmu_init();

    puts("mmu finished\n");
#endif

    gic_unmask_interrupt(QTIMER_IRQ);
    set_irq_handler(QTIMER_IRQ, timer_handler);

    start_timer(1000);

    while (1) {
        asm volatile("wfe");
    }

    hang();
}
