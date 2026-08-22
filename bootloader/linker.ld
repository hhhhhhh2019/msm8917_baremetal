ENTRY(_start)

SECTIONS {
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

    __bss_start = ALIGN(16);
    .bss : {
        * (.bss)
        * (COMMON)
    }
    __bss_end = .;

    . = ALIGN(16);
    . = . + 0x1000;
    __stack_top = .;
}
