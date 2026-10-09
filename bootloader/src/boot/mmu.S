    .section .text

    .global mmu_init
    .type mmu_init@function
mmu_init:
    ldr x0, =0x44ff00
    msr mair_el1, x0

    ldr x0, =tt
    msr ttbr0_el1, x0
    msr ttbr1_el1, x0

    /*
    38    TBI1  = 0       Top byte used in the address calculation.
    37    TBI0  = 0       Top byte used in the address calculation.
    36    AS    = 1       ASID 16-bit.
    35    res0  = 0
    34:32 IPS   = 000     32 bits, 4GB.
    31:30 TG1   = 10      4KB
    29:28 SH1   = 10      Outer shareable.
    27:26 ORGN1 = 01      Normal memory, Outer Write-Back Write-Allocate Cacheable.
    25:24 IRGN1 = 01      Normal memory, Inner Write-Back Write-Allocate Cacheable.
    23    EPD1  = 0
    22    AS    = 0       TTBR0_EL1.ASID defines the ASID.
    21:16 T1SZ  = 011001  25
    15:14 TG0   = 00      4KB
    13:12 SH0   = 10      Outer shareable.
    11:10 ORGN0 = 10      Normal memory, Outer Write-Through Cacheable.
     9:8  IRGN0 = 10      Normal memory, Inner Write-Through Cacheable.
     7    EPD0  = 0
     6    res0  = 0
     5:0  T0SZ  = 011001  25
    */
    ldr x0, =0b001000010100101000110010010101000011001
    msr tcr_el1, x0

    tlbi vmalle1
    dsb sy
    isb

    mrs x0, sctlr_el1
    ldr x1, =(1<<2) | (1<<12)
    orr x0, x0, #1
    bic x0, x0, x1
    msr sctlr_el1, x0
    isb 

    br x30

    .section .data
    .balign 4096
tt:                                         // L1, по 1 ГБ
    .quad 0x00000000 | 0x401 | (3 << 53)      // 0..1GB: Device, XN
    .quad 0
    .quad tt_ram + 3                          // 0x80000000..0xBFFFFFFF -> L2
    .quad 0
    .space 4096 - 32

    .balign 4096
tt_ram:                                     // L2, блоки по 2 МБ
    .quad 0x80000000 | 0x705                  // код, данные, стек (исполняемый)
    .space 8 * 127                            // индексы 1..127 пустые
    .quad 0x90000000 | 0x609 | (3 << 53)      // fb: Normal NC, XN
    .quad 0x90200000 | 0x609 | (3 << 53)
    .space 4096 - 8 - 8*127 - 16
