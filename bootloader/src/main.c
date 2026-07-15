#include "spmi.h"
#include "log.h"
#include "stdint.h"
#include "task.h"
#include "utils.h"
#include "gpio.h"
#include "qtimer.h"
#include "interrupts.h"
#include "gic.h"
#include "fb.h"

#define __asmeq(x, y)  ".ifnc " x "," y " ; .err ; .endif\n\t"

void edl_reboot() {
    asm volatile("msr daifset, #15" ::: "memory");

    register u64 r0 __asm__("x0") = 2181039362;
    register u64 r1 __asm__("x1") = 2;
    register u64 r2 __asm__("x2") = 0x193D100;
    register u64 r3 __asm__("x3") = 1;
    register u64 r4 __asm__("x4") = 0;
    register u64 r5 __asm__("x5") = 0;
    register u64 r6 __asm__("x6") = 0;

    do {
        __asm__ volatile(
            __asmeq("%0", "x0")
            __asmeq("%1", "x1")
            __asmeq("%2", "x2")
            __asmeq("%3", "x3")
            __asmeq("%4", "x0")
            __asmeq("%5", "x1")
            __asmeq("%6", "x2")
            __asmeq("%7", "x3")
            __asmeq("%8", "x4")
            __asmeq("%9", "x5")
            __asmeq("%10", "x6")
            "smc    #0\n"
            : "=r"(r0), "=r"(r1), "=r"(r2), "=r"(r3)
            : "r"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5), "r"(r6));
    } while (r0 == 1);

    /* log(LOG_INFO, "edl_reboot(): smc done"); */

    pmic_reg_write(0, 8, 87, 0);

    /* log(LOG_INFO, "edl_reboot(): PMIC_WD_RESET_S2_CTL2 done"); */

    pmic_reg_write(0, 8, 91, 0);
    pmic_reg_write(2, 8, 91, 0);

    // TODO: better delay
    for (volatile int i = 0; i < 2000000; i++);

    pmic_reg_write(0, 8, 90, 1);
    pmic_reg_write(2, 8, 90, 1);

    /* pmic_reg_write(0, 8, 91, 1 << 7); */
    /* pmic_reg_write(2, 8, 91, 1 << 7); */

    // наделал какую-то дичь, но теперь оно стабильно перезагружается
    __asm__ volatile ("isb");
    __asm__ volatile ("dsb sy");

    writeu32(0x193d100, 1);
    writeu32(0x004AB000, 0);

    /* log(LOG_INFO, "edl_reboot(): pmic_reset_configure done"); */

    writeu32(0x004AB000, 0);

    while (1);

    // судя по логам, выполнение доходит даже до сюда

    /* log(LOG_INFO, "edl_reboot(): MPM2_MPM_PS_HOLD done"); */
}

#define MMIO_FLAGS (0b01 << 0) | (1 << 10) | (0 << 2)
#define RAM_FLAGS  (0b01 << 0) | (1 << 10) | (1 << 2)

u64 tlb_kernel_table[8192] __attribute__((aligned(65536)));

void main() {
    u8 attr0 = (0b0000 << 0) | (0b0000 << 4); // Device memory | Device-nGnRnE memory
    u8 attr1 = (0b0111 << 0) | (0b0111 << 4); // Normal Memory, Outer Write-back transient | Normal Memory, Inner Write-back transient
    asm volatile("msr MAIR_EL1, %0" :: "r"(attr0 | (attr1 << 8)));

    tlb_kernel_table[0] = 0x00000000 | MMIO_FLAGS;
    tlb_kernel_table[1] = 0x20000000 | MMIO_FLAGS;
    tlb_kernel_table[2] = 0x40000000 | MMIO_FLAGS;
    tlb_kernel_table[3] = 0x60000000 | MMIO_FLAGS;

    tlb_kernel_table[4] = 0x80000000 | RAM_FLAGS;
    tlb_kernel_table[5] = 0xa0000000 | RAM_FLAGS;
    tlb_kernel_table[6] = 0xc0000000 | RAM_FLAGS;
    tlb_kernel_table[7] = 0x90000000 | RAM_FLAGS;

    asm volatile("msr TTBR0_EL1, %0" :: "r"(tlb_kernel_table));
    asm volatile("msr TTBR1_EL1, %0" :: "r"(tlb_kernel_table));

    u64 tcr = (24ULL << 0) |    // T0SZ
              (0b00ULL << 8) |  // Inner non cachable
              (0b00ULL << 10) | // Outer non cachable
              (0b00ULL << 12) | // Non shareable
              (0b01ULL << 14) | // TTBR0 granule size = 64KB
              (24ULL << 16) | // T1ZS
              (0b00ULL << 24) | // Inner non cachable
              (0b00ULL << 26) | // Outer non cachable
              (0b00ULL << 28) | // Non shareable
              (0b11ULL << 30) | // TTBR1 granule size = 64KB
              (0b000ULL << 32) | // 4GB IPS
              (1ULL << 36);  // 16bit ASID

    asm volatile("msr TCR_EL1, %0" :: "r"(tcr));

    asm volatile(
        "ic ialluis     \n"
        "dsb sy         \n"
        "tlbi vmalle1is \n"
        "dsb sy         \n"
        "isb            \n"
    );

    // enable MMU
    u64 scr;
    asm volatile("mrs %0, SCTLR_EL1" : "=r"(scr));
    scr |= (1 <<  0) | // mmu
           (1 <<  2) | // data cache
           (1 << 12);  // instruction cache
    asm volatile("msr SCTLR_EL1, %0" :: "r"(scr));

    asm volatile("isb \ndsb sy");

    fb_init();
    fb_init_addres((void*)0x90001000);

    /* set_vector_table(&vector_table); */
    /* gic_init(); */

    /*
     D or bit 9 - when it’s set to 1, debug exceptions are masked;
     A or bit 8 - when it’s set to 1, SError is masked;
     I or bit 7 - when it’s set to 1, IRQs are masked;
     F or bit 6 - when it’s set to 1, FIQs are masked.
     */
    /* asm volatile("msr daifset, #15" ::: "memory"); */
    /* asm volatile("msr daifclr, #15" ::: "memory"); */

    log(LOG_INFO, "reboot");

    edl_reboot();
}
