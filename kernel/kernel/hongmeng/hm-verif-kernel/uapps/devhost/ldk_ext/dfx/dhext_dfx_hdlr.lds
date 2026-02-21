SECTIONS
{
  .ro_after_init_hdlr :
  {
    __dfx_ro_after_init_hdlr_start = .;
    *(.ro_after_init_hdlr) *(.ro_after_init_hdlr.*)
    __dfx_ro_after_init_hdlr_end = .;
  } :roafterdata
}
INSERT AFTER .data;
