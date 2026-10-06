ENTRY(_start)

SECTIONS
{
  . = 0x40080000;


.text :
{
  *(.text)
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
}
