#ifndef MMU_H_
#define MMU_H_

#include "stdint.h"

typedef u64 tlb_table_t[8192] __attribute__((aligned(65536)));

void mmu_init();

#endif // MMU_H_
