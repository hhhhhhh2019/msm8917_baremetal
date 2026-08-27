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

flush_all_dcache:
    mrs     x0, clidr_el1
    and     x3, x0, #0x07000000
    lsr     x3, x3, #23
    cbz     x3, .Lfinished

    mov     x10, #0
    mov     x11, #0

.Lloop_level:
    add     x2, x10, x10, lsr #1
    lsr     x1, x0, x2
    and     x1, x1, #7
    cmp     x1, #2
    b.lt    .Lnext_level

    lsl     x1, x10, #1
    msr     csselr_el1, x1
    isb

    mrs     x1, ccsidr_el1
    
    and     x2, x1, #7
    add     x2, x2, #4

    ubfx    x3, x1, #3, #10
    clz     w4, w3

    ubfx    x5, x1, #13, #15
    clz     w6, w5

.Lloop_set:
    mov     x7, x3
.Lloop_way:
    lsl     x8, x10, #1
    lsl     x9, x7, x4
    orr     x8, x8, x9
    lsl     x9, x5, x2
    orr     x8, x8, x9

    //dc      cisw, x8
    dc      isw, x8

    subs    x7, x7, #1
    b.ge    .Lloop_way
    subs    x5, x5, #1
    b.ge    .Lloop_set

.Lnext_level:
    add     x10, x10, #1
    cmp     x3, x10, lsl #1
    b.gt    .Lloop_level

.Lfinished:
    mov     x0, #0
    msr     csselr_el1, x0
    dsb     sy
    isb

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
