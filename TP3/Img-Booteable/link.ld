SECTIONS
{
  /* The BIOS loads the image at 0x7C00.
     We must tell the linker to place code there.
     Calculate the address: 0x7C00 + 0x0 */
  . = 0x7C00;
  .text :
  {
    _start = .;
    *(.text)
    /* Place the magic BIOS signature at offset 510: */
    . = 0x1FE;
    SHORT(0xAA55)
  }
}
