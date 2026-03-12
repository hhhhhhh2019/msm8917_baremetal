.section .text.head
.global _start
.type _start@function

.global vector_table

.global set_vbar
.type set_vbar@function
.global get_vbar
.type set_vbar@function

.global sync_handler
.global irq_handler
.global fiq_handler
.global serror_handler

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
    ldr x0, =_stack_top
    mov sp, x0

    // ldr x0, =_stack_top
    // msr SP_EL1, x0
    // isb

    // mov x0, #1
    // msr SPSel, x0
    // isb

    bl main

    wfe
    b start

sync_handler:
    msr daifset, #15

    sub sp, sp, #320
    stp x0, x1, [sp, #0]
    stp x2, x3, [sp, #16]
    stp x4, x5, [sp, #32]
    stp x6, x7, [sp, #48]
    stp x8, x9, [sp, #64]
    stp x10, x11, [sp, #80]
    stp x12, x13, [sp, #96]
    stp x14, x15, [sp, #112]
    stp x16, x17, [sp, #128]
    stp x18, x19, [sp, #144]
    stp x20, x21, [sp, #160]
    stp x22, x23, [sp, #176]
    stp x24, x25, [sp, #192]
    stp x26, x27, [sp, #224]
    stp x28, x29, [sp, #256]
    stp x30, xzr, [sp, #304]

    mrs x0, esr_el1
    mrs x1, elr_el1
    mrs x2, spsr_el1
    mrs x3, far_el1
    bl int_sync_handler

    ldp x0, x1, [sp, #0]
    ldp x2, x3, [sp, #16]
    ldp x4, x5, [sp, #32]
    ldp x6, x7, [sp, #48]
    ldp x8, x9, [sp, #64]
    ldp x10, x11, [sp, #80]
    ldp x12, x13, [sp, #96]
    ldp x14, x15, [sp, #112]
    ldp x16, x17, [sp, #128]
    ldp x18, x19, [sp, #144]
    ldp x20, x21, [sp, #160]
    ldp x22, x23, [sp, #176]
    ldp x24, x25, [sp, #192]
    ldp x26, x27, [sp, #224]
    ldp x28, x29, [sp, #256]
    ldp x30, xzr, [sp, #304]
    add sp, sp, #320

    eret

irq_handler:
    msr daifset, #15

    sub sp, sp, #320
    stp x0, x1, [sp, #0]
    stp x2, x3, [sp, #16]
    stp x4, x5, [sp, #32]
    stp x6, x7, [sp, #48]
    stp x8, x9, [sp, #64]
    stp x10, x11, [sp, #80]
    stp x12, x13, [sp, #96]
    stp x14, x15, [sp, #112]
    stp x16, x17, [sp, #128]
    stp x18, x19, [sp, #144]
    stp x20, x21, [sp, #160]
    stp x22, x23, [sp, #176]
    stp x24, x25, [sp, #192]
    stp x26, x27, [sp, #224]
    stp x28, x29, [sp, #256]
    stp x30, xzr, [sp, #304]

    //mrs x0, ICC_IAR1_EL1
    mov x0, sp
    bl int_irq_handler
    //msr ICC_EOIR1_EL1, x0

    ldp x0, x1, [sp, #0]
    ldp x2, x3, [sp, #16]
    ldp x4, x5, [sp, #32]
    ldp x6, x7, [sp, #48]
    ldp x8, x9, [sp, #64]
    ldp x10, x11, [sp, #80]
    ldp x12, x13, [sp, #96]
    ldp x14, x15, [sp, #112]
    ldp x16, x17, [sp, #128]
    ldp x18, x19, [sp, #144]
    ldp x20, x21, [sp, #160]
    ldp x22, x23, [sp, #176]
    ldp x24, x25, [sp, #192]
    ldp x26, x27, [sp, #224]
    ldp x28, x29, [sp, #256]
    ldp x30, xzr, [sp, #304]
    add sp, sp, #320

    msr daifclr, #15
    eret

fiq_handler:
    sub sp, sp, #320
    stp x0, x1, [sp, #0]
    stp x2, x3, [sp, #16]
    stp x4, x5, [sp, #32]
    stp x6, x7, [sp, #48]
    stp x8, x9, [sp, #64]
    stp x10, x11, [sp, #80]
    stp x12, x13, [sp, #96]
    stp x14, x15, [sp, #112]
    stp x16, x17, [sp, #128]
    stp x18, x19, [sp, #144]
    stp x20, x21, [sp, #160]
    stp x22, x23, [sp, #176]
    stp x24, x25, [sp, #192]
    stp x26, x27, [sp, #224]
    stp x28, x29, [sp, #256]
    stp x30, xzr, [sp, #304]

    bl int_fiq_handler

    ldp x0, x1, [sp, #0]
    ldp x2, x3, [sp, #16]
    ldp x4, x5, [sp, #32]
    ldp x6, x7, [sp, #48]
    ldp x8, x9, [sp, #64]
    ldp x10, x11, [sp, #80]
    ldp x12, x13, [sp, #96]
    ldp x14, x15, [sp, #112]
    ldp x16, x17, [sp, #128]
    ldp x18, x19, [sp, #144]
    ldp x20, x21, [sp, #160]
    ldp x22, x23, [sp, #176]
    ldp x24, x25, [sp, #192]
    ldp x26, x27, [sp, #224]
    ldp x28, x29, [sp, #256]
    ldp x30, xzr, [sp, #304]
    add sp, sp, #320
    eret

serror_handler:
    sub sp, sp, #320
    stp x0, x1, [sp, #0]
    stp x2, x3, [sp, #16]
    stp x4, x5, [sp, #32]
    stp x6, x7, [sp, #48]
    stp x8, x9, [sp, #64]
    stp x10, x11, [sp, #80]
    stp x12, x13, [sp, #96]
    stp x14, x15, [sp, #112]
    stp x16, x17, [sp, #128]
    stp x18, x19, [sp, #144]
    stp x20, x21, [sp, #160]
    stp x22, x23, [sp, #176]
    stp x24, x25, [sp, #192]
    stp x26, x27, [sp, #224]
    stp x28, x29, [sp, #256]
    stp x30, xzr, [sp, #304]

    bl int_serror_handler

    ldp x0, x1, [sp, #0]
    ldp x2, x3, [sp, #16]
    ldp x4, x5, [sp, #32]
    ldp x6, x7, [sp, #48]
    ldp x8, x9, [sp, #64]
    ldp x10, x11, [sp, #80]
    ldp x12, x13, [sp, #96]
    ldp x14, x15, [sp, #112]
    ldp x16, x17, [sp, #128]
    ldp x18, x19, [sp, #144]
    ldp x20, x21, [sp, #160]
    ldp x22, x23, [sp, #176]
    ldp x24, x25, [sp, #192]
    ldp x26, x27, [sp, #224]
    ldp x28, x29, [sp, #256]
    ldp x30, xzr, [sp, #304]
    add sp, sp, #320
    eret

.balign 2048
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

.global foo
.type foo@function
foo:
    mov x0, xzr
    mov x1, 1
loop:
    add x0, x0, x1
    b loop
