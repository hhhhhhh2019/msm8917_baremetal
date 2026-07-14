#include "mmu.h"
#include "fb.h"

u64 tlb_kernel_table[8192] __attribute__((aligned(65536)));

/* если верить gemini размер таблицы = размеру страницы (= 64КБ в моем случае)
 * 64*1024/8 = 8192 индеска в таблице -> 13 бит на индекс
 * TxSZ = 24 -> размер вирт. адреса = 40 бит
 * смещение на странице - 16 бит(64КБ)
 * индекс - 13 бит
 * 40 = 16 + 13 * 2
 * -> имеем два уровня таблиц, где самая верхная мапит сразу 512 МБ(64КБ * 8192)
 */

#define MMIO_FLAGS (0b01 << 0) | (1 << 10) | (0 << 2)
#define RAM_FLAGS  (0b01 << 0) | (1 << 10) | (1 << 2)

void mmu_init() {
    fb_put_char('1');

    u8 attr0 = (0b0000 << 0) | (0b0000 << 4); // Device memory, Device-nGnRnE memory
    u8 attr1 = (0b0100 << 0) | (0b0100 << 4); // Normal Memory, Outer Non-Cacheable, Normal memory, Inner Non-Cacheable
    asm volatile("msr MAIR_EL1, %0" :: "r"(attr0 | (attr1 << 8)));

    fb_put_char('2');

    tlb_kernel_table[0] = 0x00000000 | MMIO_FLAGS;
    tlb_kernel_table[1] = 0x20000000 | MMIO_FLAGS;
    tlb_kernel_table[2] = 0x40000000 | MMIO_FLAGS;
    tlb_kernel_table[3] = 0x60000000 | MMIO_FLAGS;

    tlb_kernel_table[4] = 0x80000000 | RAM_FLAGS;
    tlb_kernel_table[5] = 0xa0000000 | RAM_FLAGS;
    tlb_kernel_table[6] = 0xc0000000 | RAM_FLAGS;
    tlb_kernel_table[7] = 0xe0000000 | RAM_FLAGS;

    asm volatile("msr TTBR0_EL1, %0" :: "r"(tlb_kernel_table));
    asm volatile("msr TTBR1_EL1, %0" :: "r"(tlb_kernel_table));

    fb_put_char('3');

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

    fb_put_char('4');

    asm volatile("isb\ndsb sy");

    // enable MMU
    u64 scr;
    asm volatile("mrs %0, SCTLR_EL1" : "=r"(scr));
    scr |= (1 <<  0) | // mmu
           (0 <<  2) | // data cache
           (0 << 12);  // instruction cache
    asm volatile("msr SCTLR_EL1, %0" :: "r"(scr));

    asm volatile("isb \ndsb sy");

    fb_put_char('f');
}
