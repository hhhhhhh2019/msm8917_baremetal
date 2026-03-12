ENTRY(_start)

SECTIONS {
    . = 0x80080000;

    .text : {
        *(.text.head)
        *(.text*)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    __data_end = .;

    .bss : {
        _bss_start = .;
        *(.bss)
        _bss_end = .;
    }

    . = ALIGN(16);
    . = . + 0x1000;
    _stack_top = .;
}
