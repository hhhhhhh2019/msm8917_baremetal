#ifndef MMU_H_
#define MMU_H_

#define SCTLR_MMU_ENABLE (1ULL << 0)
#define SCTLR_DCACHE     (1ULL << 2)
#define SCTLR_ICACHE     (1ULL << 12)

#define TCR_T0SZ(n) (n << 0)

#define TCR_INNER0_NON_CACHEABLE (0b00ULL << 8)
#define TCR_INNER0_BACK_ALLOC    (0b01ULL << 8)
#define TCR_INNER0_THROUGH       (0b10ULL << 8)
#define TCR_INNER0_BACK_NO_ALLOC (0b11ULL << 8)

#define TCR_OUTER0_NON_CACHEABLE (0b00ULL << 10)
#define TCR_OUTER0_BACK_ALLOC    (0b01ULL << 10)
#define TCR_OUTER0_THROUGH       (0b10ULL << 10)
#define TCR_OUTER0_BACK_NO_ALLOC (0b11ULL << 10)

#define TCR_NON_SHARE0   (0b00ULL << 12)
#define TCR_OUTER_SHARE0 (0b10ULL << 12)
#define TCR_INNER_SHARE0 (0b11ULL << 12)

#define TCR_GRANULE0_4KB  (0b00ULL << 14)
#define TCR_GRANULE0_64KB (0b01ULL << 14)

#define TCR_T1SZ(n) (n << 16)

#define TCR_ASID0 (0ULL << 22)
#define TCR_ASID1 (1ULL << 22)

#define TCR_INNER1_NON_CACHEABLE (0b00ULL << 24)
#define TCR_INNER1_BACK_ALLOC    (0b01ULL << 24)
#define TCR_INNER1_THROUGH       (0b10ULL << 24)
#define TCR_INNER1_BACK_NO_ALLOC (0b11ULL << 24)

#define TCR_OUTER1_NON_CACHEABLE (0b00ULL << 26)
#define TCR_OUTER1_BACK_ALLOC    (0b01ULL << 26)
#define TCR_OUTER1_THROUGH       (0b10ULL << 26)
#define TCR_OUTER1_BACK_NO_ALLOC (0b11ULL << 26)

#define TCR_NON_SHARE1   (0b00ULL << 28)
#define TCR_OUTER_SHARE1 (0b10ULL << 28)
#define TCR_INNER_SHARE1 (0b11ULL << 28)

#define TCR_GRANULE1_4KB  (0b10ULL << 30)
#define TCR_GRANULE1_64KB (0b11ULL << 30)

#define TCR_IPS_32bits (0b000ULL << 32)
#define TCR_IPS_36bits (0b001ULL << 32)
#define TCR_IPS_40bits (0b010ULL << 32)

#define TCR_ASID_8bit  (0ULL << 36)
#define TCR_ASID_16bit (1ULL << 36)

#define TLB_TABLE 0b11ULL
#define TLB_BLOCK 0b01ULL
#define TLB_ATTR(n) (n << 2)
#define TLB_ACCESS(bool) (bool << 10)

void mmu_init();

#endif // MMU_H_
