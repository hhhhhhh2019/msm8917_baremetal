#ifndef MMU_H_
#define MMU_H_

#include "stdint.h"

// Регистр MAIR_EL1 мы настроили так(да, нейронка, а вы пытались сами официальную документацию arm читать?)
// Индекс 0 = Device-nGnRnE (0x00)
// Индекс 1 = Normal Memory Inner/Outer Write-Back (0xFF)
#define ATTR_INDEX_DEVICE   0
#define ATTR_INDEX_NORMAL   1

#define PT_VALID (1 << 0)
#define PT_ACCESS (1 << 10)

typedef u64 tlb_table_t[512] __attribute__((aligned(4096)));

void mmu_init();

#endif // MMU_H_
