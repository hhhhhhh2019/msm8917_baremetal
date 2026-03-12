.section .text.head
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
    ldr x0, =_stack_top
    mov sp, x0

    ldr x0, =_bss_start
    ldr x1, =_bss_end

    cmp x0, x1
    b.eq run_main

clear_bss:
    str xzr, [x0], #8
    cmp x0, x1
    b.lt clear_bss

run_main:
    bl main

hang:
    wfe
    b hang


.global foo
.type foo@function
foo:
    mov x0, xzr
    mov x1, 1
loop:
    add x0, x0, x1
    b loop
