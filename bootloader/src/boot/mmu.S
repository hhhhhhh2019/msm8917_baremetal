.section .data

.global tt
.align 12
tt: .zero 8*512

.section .rodata

msg_start: .asciz "mmu: start\n"
msg_mair: .asciz "mmu: mair configured\n"
msg_tables: .asciz "mmu: tables initizalied\n"
msg_ttbr: .asciz "mmu: ttbr initizalied\n"
msg_tcr: .asciz "mmu: tcr configured\n"
msg_sctlr: .asciz "mmu: sctlr configured\n"

.section .text

.global mmu_init
.type mmu_init@function
mmu_init:
    stp x29, x30, [sp, #-16]!

    ldr x0, =msg_start
    bl puts

    ldr x0, =0x44ff00
    msr mair_el1, x0

    ldr x0, =msg_mair
    bl puts

    ldr x1, =tt
    ldr x0, =0x00000401
    str x0, [x1, #0]
    ldr x0, =0x40000401
    str x0, [x1, #8]
    ldr x0, =0x80000405
    str x0, [x1, #16]
    ldr x0, =0xc0000405
    str x0, [x1, #24]

    ldr x0, =msg_tables
    bl puts

    ldr x0, =tt
    msr ttbr0_el1, x0

    ldr x0, =msg_ttbr
    bl puts

    ldr x0, =0x1000192519
    msr tcr_el1, x0

    ldr x0, =msg_tcr
    bl puts

    mrs x0, sctlr_el1

    ldr x1, =0x1005
    orr x0, x0, x1

    isb
    dsb sy
    TLBI VMALLE1
    IC IALLU
    isb
    dsb sy

    msr sctlr_el1, x0

    isb
    dsb sy

    nop
    nop

    ldr x0, =msg_sctlr
    bl printf

    bl fb_flush

    ldp x29, x30, [sp], #16

    blr x30
    // ret стабильно не работает
//ret
