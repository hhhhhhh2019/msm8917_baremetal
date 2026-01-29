.section .text
.global _start
.type _start@function

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


start:
    ldr x30, =_stack_top
    mov sp, x30

    bl main

    wfe
    b start
