OUTPUT_FORMAT(elf64-x86-64)

ENTRY(_start)

SECTIONS
{
    . = 0xffffffff80000000;
    phys = .;

    .text BLOCK(4K) : ALIGN(4K) {
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K) {
        *(.bss)
        *(COMMON)
    }

    end = .;

    /DISCARD/ : {
		*(.eh_frame)
        *(.eh_frame)
        *(.note .note.*)
		*(.note.gnu.build-id)
    }
}