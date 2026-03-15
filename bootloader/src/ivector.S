.global vector_table

.global set_vbar
.type set_vbar@function
.global get_vbar
.type set_vbar@function

.section .text

set_vbar:
    msr VBAR_EL1, x0
    isb
    ret

get_vbar:
    mrs x0, VBAR_EL1
    ret

.macro int_enter
    sub sp, sp, #16 * 17
    stp  x0,  x1, [sp, #16 * 0]
    stp  x2,  x3, [sp, #16 * 1]
    stp  x4,  x5, [sp, #16 * 2]
    stp  x6,  x7, [sp, #16 * 3]
    stp  x8,  x9, [sp, #16 * 4]
    stp x10, x11, [sp, #16 * 5]
    stp x12, x13, [sp, #16 * 6]
    stp x14, x15, [sp, #16 * 7]
    stp x16, x17, [sp, #16 * 8]
    stp x18, x19, [sp, #16 * 9]
    stp x20, x21, [sp, #16 * 10]
    stp x22, x23, [sp, #16 * 11]
    stp x24, x25, [sp, #16 * 12]
    stp x26, x27, [sp, #16 * 13]
    stp x28, x29, [sp, #16 * 14]
    str x30,      [sp, #16 * 15]

    ;; mrs x0, elr_el1
    ;; mrs x1, spsr_el1
    ;; stp x0, x1, [sp, #16 * 16]
.endm

.macro int_exit
    ;; ldp x0, x1, [sp, #16 * 16]
    ;; msr elr_el1, x0
    ;; msr spsr_el1, x1

    ldp  x0,  x1, [sp, #16 * 0]
    ldp  x2,  x3, [sp, #16 * 1]
    ldp  x4,  x5, [sp, #16 * 2]
    ldp  x6,  x7, [sp, #16 * 3]
    ldp  x8,  x9, [sp, #16 * 4]
    ldp x10, x11, [sp, #16 * 5]
    ldp x12, x13, [sp, #16 * 6]
    ldp x14, x15, [sp, #16 * 7]
    ldp x16, x17, [sp, #16 * 8]
    ldp x18, x19, [sp, #16 * 9]
    ldp x20, x21, [sp, #16 * 10]
    ldp x22, x23, [sp, #16 * 11]
    ldp x24, x25, [sp, #16 * 12]
    ldp x26, x27, [sp, #16 * 13]
    ldp x28, x29, [sp, #16 * 14]
    ldr x30,      [sp, #16 * 15]
    add sp, sp, #16 * 17

    eret
.endm

sync_handler:
    int_enter

    mrs x0, esr_el1
    mrs x1, elr_el1
    mrs x2, spsr_el1
    mrs x3, far_el1
    bl int_sync_handler

    int_exit

irq_handler:
    int_enter

    mov x0, sp
    bl int_irq_handler

    int_exit

fiq_handler:
    int_enter

    bl int_fiq_handler

    int_exit

serror_handler:
    int_enter

    bl int_serror_handler

    int_exit

unimplemented_sync_handler:
    mov x0, xzr
    ldr x1, =unimplemented_sync
    bl log
    b edl_reboot

unimplemented_irq_handler:
    mov x0, xzr
    ldr x1, =unimplemented_irq
    bl log
    b edl_reboot

unimplemented_fiq_handler:
    mov x0, xzr
    ldr x1, =unimplemented_fiq
    bl log
    b edl_reboot

unimplemented_serror_handler:
    mov x0, xzr
    ldr x1, =unimplemented_serror
    bl log
    b edl_reboot

.balign 2048
vector_table:
    // sync, irq, fiq, serror

    // Current EL with SP0
    .org vector_table + 0x000; b unimplemented_sync_handler
    .org vector_table + 0x080; b unimplemented_irq_handler
    .org vector_table + 0x100; b unimplemented_fiq_handler
    .org vector_table + 0x180; b unimplemented_serror_handler

    // Current EL with SPx (Kernel Modes)
    .org vector_table + 0x200; b sync_handler
    .org vector_table + 0x280; b irq_handler
    .org vector_table + 0x300; b unimplemented_fiq_handler
    .org vector_table + 0x380; b unimplemented_serror_handler

    // Lower EL (User Mode - Future Proofing)
    .org vector_table + 0x400; b unimplemented_sync_handler
    .org vector_table + 0x480; b unimplemented_irq_handler
    .org vector_table + 0x500; b unimplemented_fiq_handler
    .org vector_table + 0x580; b unimplemented_serror_handler

    .org vector_table + 0x600; b unimplemented_sync_handler
    .org vector_table + 0x680; b unimplemented_irq_handler
    .org vector_table + 0x700; b unimplemented_fiq_handler
    .org vector_table + 0x780; b unimplemented_serror_handler

.section .rodata
unimplemented_sync:
    .ascii "unimplemented sync exception\0"
unimplemented_irq:
    .ascii "unimplemented irq\0"
unimplemented_fiq:
    .ascii "unimplemented fiq\0"
unimplemented_serror:
    .ascii "unimplemented serror exception\0"
