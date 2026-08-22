#include "config/mmu.h"
#include "utils.h"

#ifdef CONFIG_MMU
    #define MMIO_FLAGS (0b01 << 0) | (1 << 10) | (0 << 2)
    #define RAM_FLAGS  (0b01 << 0) | (1 << 10) | (1 << 2) | (0b11 << 8)

/* если верить gemini размер таблицы = размеру страницы (= 64КБ в моем случае)
 * 64*1024/8 = 8192 индеска в таблице -> 13 бит на индекс
 * TxSZ = 24 -> размер вирт. адреса = 40 бит
 * смещение на странице - 16 бит(64КБ)
 * индекс - 13 бит
 * 40 = 16 + 13 * 2
 * -> имеем два уровня таблиц, где самая верхная мапит сразу 512 МБ(64КБ * 8192)
 */

u64 tlb_kernel_table[8192] __attribute__((aligned(65536)));
#endif

void main() {
#ifdef CONFIG_MMU
    u8 attr0 =
        (0b0000 << 0) | (0b0000 << 4); // Device memory | Device-nGnRnE memory
    u8 attr1 = (0b1111 << 0)
             | (0b1111 << 4); // Normal Memory, Outer Write-back Non-transient |
                              // Normal Memory, Inner Write-back Non-transient
    asm volatile("msr MAIR_EL1, %0" ::"r"(attr0 | (attr1 << 8)));

    tlb_kernel_table[0] = 0x00000000 | MMIO_FLAGS;
    tlb_kernel_table[1] = 0x20000000 | MMIO_FLAGS;
    tlb_kernel_table[2] = 0x40000000 | MMIO_FLAGS;
    tlb_kernel_table[3] = 0x60000000 | MMIO_FLAGS;

    tlb_kernel_table[4] = 0x80000000 | RAM_FLAGS;
    tlb_kernel_table[5] = 0xa0000000 | RAM_FLAGS;
    tlb_kernel_table[6] = 0xc0000000 | RAM_FLAGS;
    tlb_kernel_table[7] = 0xe0000000 | RAM_FLAGS;

    asm volatile("msr TTBR0_EL1, %0" ::"r"(tlb_kernel_table));
    asm volatile("msr TTBR1_EL1, %0" ::"r"(tlb_kernel_table));

    u64 tcr = (24ULL << 0) |     // T0SZ
              (0b01ULL << 8) |   // Inner Write-Back Write-Allocate Cacheable
              (0b01ULL << 10) |  // Outer Write-Back Write-Allocate Cacheable
              (0b11ULL << 12) |  // Inner shareable
              (0b01ULL << 14) |  // TTBR0 granule size = 64KB
              (24ULL << 16) |    // T1ZS
              (0b01ULL << 24) |  // Inner Write-Back Write-Allocate Cacheable
              (0b01ULL << 26) |  // Outer Write-Back Write-Allocate Cacheable
              (0b11ULL << 28) |  // Inner shareable
              (0b11ULL << 30) |  // TTBR1 granule size = 64KB
              (0b000ULL << 32) | // 4GB IPS
              (1ULL << 36);      // 16bit ASID

    asm volatile("msr TCR_EL1, %0" ::"r"(tcr));

    asm volatile("ic ialluis     \n"
                 "dsb sy         \n"
                 "tlbi vmalle1is \n"
                 "dsb sy         \n"
                 "isb            \n");

    for (u64 i = 0; i < sizeof(tlb_kernel_table); i += 64) {
        asm volatile("dc civac, %0" ::"r"(((u64)tlb_kernel_table) + i));
    }
    asm volatile("dsb sy\nisb" ::
                     : "memory");

    // enable MMU
    u64 scr;
    asm volatile("mrs %0, SCTLR_EL1"
                 : "=r"(scr));
    scr |= (1 << 0) | // mmu
           (1 << 2) | // data cache
           (1 << 12); // instruction cache
    asm volatile("msr SCTLR_EL1, %0" ::"r"(scr));

    asm volatile("ic ialluis     \n"
                 "dsb sy         \n"
                 "tlbi vmalle1is \n"
                 "dsb sy         \n"
                 "isb            \n");
#endif

    while (1) {
        asm volatile("wfe");
    }
}
