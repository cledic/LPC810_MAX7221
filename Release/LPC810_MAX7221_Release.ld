/*
 * GENERATED FILE - DO NOT EDIT
 * (C) Code Red Technologies Ltd, 2008-2013
 * Generated linker script file for LPC810
 * Created from generic_c.ld (vLPCXpresso v5.2 (4 [Build 2122] [2013-04-29] ))
 * By LPCXpresso v5.2.4 [Build 2122] [2013-04-29]  on Thu Aug 22 07:21:00 CEST 2013
 */


INCLUDE "LPC810_MAX7221_Release_lib.ld"
INCLUDE "LPC810_MAX7221_Release_mem.ld"

ENTRY(ResetISR)

SECTIONS
{

    /* MAIN TEXT SECTION */    
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        
        /* Global Section Table */
        . = ALIGN(4) ;
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data)) ;
        LONG(  SIZEOF(.data));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */
        

        *(.after_vectors*)
        
        /* Code Read Protect data */
        . = 0x000002FC ;
        PROVIDE(__CRP_WORD_START__ = .) ;
        KEEP(*(.crp))
        PROVIDE(__CRP_WORD_END__ = .) ;
        ASSERT(!(__CRP_WORD_START__ == __CRP_WORD_END__), "Linker CRP Enabled, but no CRP_WORD provided within application");
        /* End of Code Read Protect */
        
    } >MFlash4
    
    .text : ALIGN(4)    
    {
         *(.text*)
        *(.rodata .rodata.*)
        . = ALIGN(4);
        
    } > MFlash4

    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this. 
     */
    .ARM.extab : ALIGN(4)
    {
    	*(.ARM.extab* .gnu.linkonce.armextab.*)
    } > MFlash4
    __exidx_start = .;
    
    .ARM.exidx : ALIGN(4)
    {
    	*(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > MFlash4
    __exidx_end = .;
    
    _etext = .;
        
    
    /* MAIN DATA SECTION */
    
    /* Default MTB section */
    .mtb_buffer_default (NOLOAD) :
    {
       KEEP(*(.mtb*))
    } > RamLoc1 

    .uninit_RESERVED : ALIGN(4)
    {
        KEEP(*(.bss.$RESERVED*))
        . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > RamLoc1

	
	/* Main DATA section (RamLoc1) */
	.data : ALIGN(4)
	{
	   FILL(0xff)
	   _data = . ;
	   *(vtable)
	   *(.data*)
	   . = ALIGN(4) ;
	   _edata = . ;
	} > RamLoc1 AT>MFlash4


    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(end = .);
    } > RamLoc1
        
    
    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        *(.noinit*) 
         . = ALIGN(4) ;
        _end_noinit = .;
    } > RamLoc1
    
    PROVIDE(_pvHeapStart = .);
    PROVIDE(_vStackTop = __top_RamLoc1 - 0);
}
