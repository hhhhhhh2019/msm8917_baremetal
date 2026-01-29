ENTRY(_start)

SECTIONS {
    . = 0x80000000;

    .text : {
        *(.text._start)
        *(.text*)
    }

    __data_end = .;

    . = ALIGN(8);
    _stack_top = .;
}
