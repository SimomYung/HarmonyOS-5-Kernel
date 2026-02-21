SECTIONS
{
  .preinit_array    :
  {
    __preinit_array_start = .;
    KEEP (*(.preinit_array));
    __preinit_array_end = .;
    KEEP (*(.data_to_prevent_optimize));
  }
  .pac_init_section : {
    *(.pac_init_section)
  } :text
}