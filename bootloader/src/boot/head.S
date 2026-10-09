    .section .text.head

    .global _start
    .type _start@function
_start:
    b start
    nop

    // text offset
    .quad 0x80000

    // image size
    .quad __data_end - _start

    // flags
    .quad 2

    // reserved
    .quad 0, 0, 0

    // magic
    .ascii "ARM\x64"

    // reserved
    .word 0

start:
    ldr x0, =__stack_top
    mov sp, x0

    ldr x0, =__bss_start
    ldr x1, =__bss_end

bss_clear:
    cmp x0, x1
    b.hs run_main

    str xzr, [x0], #8
    b bss_clear

run_main:
    bl main

    .global hang
    .type hang@function
hang:
    wfe
    b hang
