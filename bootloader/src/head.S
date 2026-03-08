.section .text
.global _start
.type _start@function

.global vector_table

.global set_vbar
.type set_vbar@function
.global get_vbar
.type set_vbar@function

_start:
    b start
    nop

    // text offset
    .quad 0x80000000

    // image size
    .quad __data_end - _start

    // flags
    .quad 0

    // reserved
    .quad 0, 0, 0

    // magic
    .ascii "ARM\x64"

    // reserved
    .word 0

set_vbar:
    msr VBAR_EL1, x0
    isb
    ret

get_vbar:
    mrs x0, VBAR_EL1
    ret

start:
    ldr x30, =_stack_top
    mov sp, x30

    bl main

    wfe
    b start

sync_handler:
    sub sp, sp, #16*8
    stp x0, x1, [sp, #0]
    stp x2, x3, [sp, #16]
    stp x4, x5, [sp, #32]
    stp x6, x7, [sp, #48]
    stp x8, x9, [sp, #64]
    stp x10, x11, [sp, #80]
    stp x12, x13, [sp, #96]
    stp x14, x15, [sp, #112]
    mrs x0, esr_el1
    mrs x1, elr_el1
    mrs x2, spsr_el1
    mrs x3, far_el1
    bl synchronous
    ldp x0, x1, [sp, #0]
    ldp x2, x3, [sp, #16]
    ldp x4, x5, [sp, #32]
    ldp x6, x7, [sp, #48]
    ldp x8, x9, [sp, #64]
    ldp x10, x11, [sp, #80]
    ldp x12, x13, [sp, #96]
    ldp x14, x15, [sp, #112]
    add sp, sp, #16*8
    eret
irq_handler:
    sub sp, sp, #16*8
    stp x0, x1, [sp, #0]
    stp x2, x3, [sp, #16]
    stp x4, x5, [sp, #32]
    stp x6, x7, [sp, #48]
    stp x8, x9, [sp, #64]
    stp x10, x11, [sp, #80]
    stp x12, x13, [sp, #96]
    stp x14, x15, [sp, #112]
    ;; mrs x0, ICC_IAR1_EL1
    bl irq
    ;; msr ICC_EOIR1_EL1, x0
    ldp x0, x1, [sp, #0]
    ldp x2, x3, [sp, #16]
    ldp x4, x5, [sp, #32]
    ldp x6, x7, [sp, #48]
    ldp x8, x9, [sp, #64]
    ldp x10, x11, [sp, #80]
    ldp x12, x13, [sp, #96]
    ldp x14, x15, [sp, #112]
    add sp, sp, #16*8
    eret
fiq_handler:
    sub sp, sp, #16*8
    stp x0, x1, [sp, #0]
    stp x2, x3, [sp, #16]
    stp x4, x5, [sp, #32]
    stp x6, x7, [sp, #48]
    stp x8, x9, [sp, #64]
    stp x10, x11, [sp, #80]
    stp x12, x13, [sp, #96]
    stp x14, x15, [sp, #112]
    bl fiq
    ldp x0, x1, [sp, #0]
    ldp x2, x3, [sp, #16]
    ldp x4, x5, [sp, #32]
    ldp x6, x7, [sp, #48]
    ldp x8, x9, [sp, #64]
    ldp x10, x11, [sp, #80]
    ldp x12, x13, [sp, #96]
    ldp x14, x15, [sp, #112]
    add sp, sp, #16*8
    eret
serror_handler:
    sub sp, sp, #16*8
    stp x0, x1, [sp, #0]
    stp x2, x3, [sp, #16]
    stp x4, x5, [sp, #32]
    stp x6, x7, [sp, #48]
    stp x8, x9, [sp, #64]
    stp x10, x11, [sp, #80]
    stp x12, x13, [sp, #96]
    stp x14, x15, [sp, #112]
    bl serror
    ldp x0, x1, [sp, #0]
    ldp x2, x3, [sp, #16]
    ldp x4, x5, [sp, #32]
    ldp x6, x7, [sp, #48]
    ldp x8, x9, [sp, #64]
    ldp x10, x11, [sp, #80]
    ldp x12, x13, [sp, #96]
    ldp x14, x15, [sp, #112]
    add sp, sp, #16*8
    eret

.balign 128
vector_table:
    b sync_handler
.balign 128
    b irq_handler
.balign 128
    b fiq_handler
.balign 128
    b serror_handler
.balign 128
    b sync_handler
.balign 128
    b irq_handler
.balign 128
    b fiq_handler
.balign 128
    b serror_handler
.balign 128
    b sync_handler
.balign 128
    b irq_handler
.balign 128
    b fiq_handler
.balign 128
    b serror_handler
.balign 128
    b sync_handler
.balign 128
    b irq_handler
.balign 128
    b fiq_handler
.balign 128
    b serror_handler
