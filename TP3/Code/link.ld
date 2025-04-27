SECTIONS
{
  . = 0x7c00;         /* El código empieza en la dirección 0x7C00 (sector de booteo) */

  .text :
  {
    *(.text)          /* Incluir toda la sección de código .text */
  }

  .data :
  {
    *(.data)          /* Incluir la sección de datos si hubiera */
  }

  .bss :
  {
    *(.bss)           /* Reservar espacio para variables no inicializadas */
  }
}

