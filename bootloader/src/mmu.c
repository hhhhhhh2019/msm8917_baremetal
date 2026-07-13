#include "mmu.h"

tlb_table_t tlb_table_kernel = {0};

void mmu_init() {
    asm volatile("msr MAIR_EL1, %0" :: "r"(192)); // 0b11000000

    tlb_table_kernel[0] = 0x00000000 | PT_VALID | PT_ACCESS | (ATTR_INDEX_DEVICE << 2);
    tlb_table_kernel[1] = 0x40000000 | PT_VALID | PT_ACCESS | (ATTR_INDEX_DEVICE << 2);
    tlb_table_kernel[2] = 0x80000000 | PT_VALID | PT_ACCESS | (ATTR_INDEX_NORMAL << 2);
    tlb_table_kernel[3] = 0xc0000000 | PT_VALID | PT_ACCESS | (ATTR_INDEX_NORMAL << 2);
}
