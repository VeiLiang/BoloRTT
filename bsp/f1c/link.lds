OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{
	. = 0x80000000;
	__image_start = .;
	.bootloader :
	{		
		build/libcpu/start_gcc.o 
		build/bootloader/memcpy.o 
		build/bootloader/memset.o 
		build/bootloader/sys-uart.o 
		build/bootloader/sys-clock.o 
		build/bootloader/sys-dram.o 
		build/bootloader/sys-mmu.o 
		build/bootloader/sys-spi-flash.o 
		build/bootloader/sys-copyself.o 		
	}
	__bootloader_size = SIZEOF(.bootloader); 
	
	. = ALIGN(4);
	
    __text_start = .;
    .text :
    {	
		build/libcpu/context_gcc.o
        *(.text)
        *(.text.*)
	KEEP(*(.fini))

        /* section information for finsh shell */
        . = ALIGN(4);
        __fsymtab_start = .;
        KEEP(*(FSymTab))
        __fsymtab_end = .;
        . = ALIGN(4);
        __vsymtab_start = .;
        KEEP(*(VSymTab))
        __vsymtab_end = .;
        . = ALIGN(4);

        /* section information for modules */
        . = ALIGN(4);
        __rtmsymtab_start = .;
        KEEP(*(RTMSymTab))
        __rtmsymtab_end = .;

        /* section information for initialization */
        . = ALIGN(4);
        __rt_init_start = .;
        KEEP(*(SORT(.rti_fn*)))
        __rt_init_end = .;
    } =0
    __text_end = .;

    __rodata_start = .;
    .rodata   : { *(.rodata) *(.rodata.*) *(.eh_frame) }
    __rodata_end = .;

    . = ALIGN(4);
    .ctors :
    {
        PROVIDE(__ctors_start__ = .);
	*crtbegin.o(.ctors)
	*crtbegin?.o(.ctors)
	*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
	*(SORT(.ctors.*))
	*(.ctors)
        PROVIDE(__ctors_end__ = .);       
    }

    .dtors :
    {
        PROVIDE(__dtors_start__ = .);
        *crtbegin.o(.dtors)
 		*crtbegin?.o(.dtors)
 		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
 		*(SORT(.dtors.*))
 		*(.dtors)
        PROVIDE(__dtors_end__ = .);
    }

    . = ALIGN(8);
    __data_start = .;
    .data :
    {
        *(.data)
        *(.data.*)

		. = ALIGN(4);
		/* preinit data */
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);

		. = ALIGN(4);
		/* init data */
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);

		. = ALIGN(4);
		/* finit data */
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);

		KEEP(*(.jcr*))
		. = ALIGN(4);
		/* All data end */

        *(.gnu.linkonce.d*)
    }
    
	__data_end = .;
	
    __image_end = .;
	
	. = ALIGN(8);
    __bss_start = .;
    .bss       :
    {
    *(.bss)
    *(.bss.*)
    *(COMMON)
    . = ALIGN(4);
    }
    . = ALIGN(4);
    __bss_end = .;

    /* Stabs debugging sections.  */
    .stab 0 : { *(.stab) }
    .stabstr 0 : { *(.stabstr) }
    .stab.excl 0 : { *(.stab.excl) }
    .stab.exclstr 0 : { *(.stab.exclstr) }
    .stab.index 0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment 0 : { *(.comment) }

    _end = .;
}
