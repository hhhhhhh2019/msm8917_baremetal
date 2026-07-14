#include "mmu.h"
#include "fb.h"

tlb_table_t tlb_table_kernel = {0};

#define PTE_VALID          (1ULL << 0)
#define PTE_BLOCK          (0ULL << 1)
#define PTE_TABLE          (1ULL << 1)
#define PTE_AF             (1ULL << 10)
#define PTE_INNER_SH       (3ULL << 8)

#define ATTR_INDEX_DEVICE  0
#define ATTR_INDEX_NORMAL  1

#define MMIO_FLAGS         (PTE_VALID | PTE_BLOCK | (ATTR_INDEX_DEVICE << 2) | PTE_AF)
#define RAM_FLAGS          (PTE_VALID | PTE_BLOCK | (ATTR_INDEX_NORMAL << 2) | PTE_AF | PTE_INNER_SH)

__attribute__((noinline))
void loop();

void mmu_init() {
    tlb_table_kernel[0] = 0x00000000ULL | MMIO_FLAGS;
    tlb_table_kernel[1] = 0x20000000ULL | MMIO_FLAGS;
    tlb_table_kernel[2] = 0x40000000ULL | MMIO_FLAGS;
    tlb_table_kernel[3] = 0x60000000ULL | MMIO_FLAGS;

    tlb_table_kernel[4] = 0x80000000ULL | RAM_FLAGS;
    tlb_table_kernel[5] = 0xA0000000ULL | RAM_FLAGS;
    tlb_table_kernel[6] = 0xC0000000ULL | RAM_FLAGS;
    tlb_table_kernel[7] = 0x80000000ULL | RAM_FLAGS;

    asm volatile("dsb sy" ::: "memory");

    fb_put_char('1');

    // attr0 = 0b00000000 device memory, nGnRnE
    // attr1 = 0b11111111 normal memory, Inner Write-back non-transient
    asm volatile("msr MAIR_EL1, %0" :: "r"(0xff00));

    fb_put_char('2');

    // [36] = 1 16 bit ASID
    // [34:32] = 000 Intermediate Physical Address Size 32 bits, 4GB
    // [31:30] = 11 TTBR1_EL1 granule size 64KB
    // [29:28] = 10 TTBR1_EL1 Outer shareable
    // [27:26] = 01 TTBR1_EL1 Outer Write-Back Write-Allocate Cacheable
    // [25:24] = 01 TTBR1_EL1 Inner Write-Back Write-Allocate Cacheable.
    // [23] = 0 Perform translation table walk using TTBR1_EL1
    // [22] = 0 TTBR0_EL1.ASID defines the ASID
    // [21:16] = 011000 Size offset of the memory region addressed by TTBR1_EL1.
    // [15:14] = 01 TTBR0_EL1 granule size = 64KB
    // [13:12] = 10 TTBT_EL1 Outer shareable
    // [11:10] = 01 TTBR0_EL1 Outer Write-Back Write-Allocate Cacheable
    // [9:8] = 01 TTBR0_EL1 Inner Write-Back Write-Allocate Cacheable.
    // [7] = 0 Perform translation table walk using TTBR0_EL1
    // [5:0] = 011000 Size offset of the memory region addressed by TTBR0_EL1.
    asm volatile("msr TCR_EL1, %0" :: "r"(0b1000011100101000110000110010100011000));

    fb_put_char('3');

    asm volatile("msr TTBR0_EL1, %0" :: "r"(tlb_table_kernel));
    asm volatile("msr TTBR1_EL1, %0" :: "r"(tlb_table_kernel));

    fb_put_char('4');

    asm volatile(
        "ic ialluis     \n"
        "dsb sy         \n"
        "tlbi vmalle1is \n"
        "dsb sy         \n"
        "isb            \n"
    );

    fb_put_char('5');

    u64 sctrl;

    asm volatile("mrs %0, SCTLR_EL1" : "=r"(sctrl));

    fb_put_char('6');

    sctrl |= (1 <<  0) | // MMU
             (0 <<  2) | // Cache
             (0 << 12);  // ICache

    sctrl &= ~(1 << 28);
    sctrl &= ~(1 << 29);

    fb_put_char('7');

    asm volatile("msr SCTLR_EL1, %0\n"
                 "isb"
                 :: "r"(sctrl));

    fb_put_char('8');

    u64 current_sp;
    asm volatile("mov %0, sp" : "=r"(current_sp));

    asm volatile("dc ivac, %0" :: "r"(current_sp));
    asm volatile("dc ivac, %0" :: "r"(current_sp + 64));
    asm volatile("dc ivac, %0" :: "r"(current_sp - 64));

    asm volatile("dsb sy\n"
                 "isb");

    loop();

    for (int a = 0; a < 10; a++) {
        fb_put_char('a');
        for (volatile u32 i = 0; i < 1000000; i++);
    }
}

__attribute__((noinline))
void loop() {
    u8* fb = (u8*)(0x90001000ULL - 0x80000000ULL + 0xE0000000ULL);

    for (u32 x = 0; x < 100; x++) {
        for (u32 y = 0; y < 100; y++) {
            u32 offset = (x + y * 720) * 3;
            fb[offset + 0] = 100;
            fb[offset + 1] = 200;
            fb[offset + 2] = 250;
        }
    }
}
