ENTRY(_start)

SECTIONS
{
  . = 0x40080000;


.text :
{
  *(.text)
}

. = ALIGN(2048);
.vectors : 
{
  *(.vectors)
}

.data : 
{
  *(.data)
}
.rodata :
{
  *(.rodata*)
}

. = ALIGN(16);
stack_bottom = .;
. += 0x4000;
stack_top = .;
kernel_end = .;

.bss :
{
    bss_start = .;
    *(.bss)
    *(.bss.*)
    *(COMMON)
    bss_end = .;
}
}

