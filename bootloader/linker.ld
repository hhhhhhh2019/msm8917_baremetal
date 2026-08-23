ENTRY(_start)

SECTIONS {
    . = 0x80000000;

    .text : {
        * (.text.head)
        * (.text)
    }

    .data : {
        * (.data)
    }

    .rodata : {
        * (.rodata)
    }

    __data_end = .;

    . = ALIGN(8);
    __bss_start = .;
    .bss : {
        * (.bss)
        * (COMMON)
    }
    . = ALIGN(8);
    __bss_end = .;

    . = ALIGN(16);
    . = . + 0x4000;
    __stack_top = .;
}
