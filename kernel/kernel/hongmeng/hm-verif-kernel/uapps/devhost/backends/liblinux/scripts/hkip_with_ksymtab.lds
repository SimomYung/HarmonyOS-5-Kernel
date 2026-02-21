/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2022. All rights reserved.
 * Author: Huawei System Security Lab
 * Create: Tue Aug 20 08:38:32 2024
 */

PHDRS {
  hkipinfo   0x60000008 FLAGS(4);
  text       PT_LOAD FILEHDR PHDRS FLAGS(5);
  roafterdata PT_LOAD FLAGS(6);
  data       PT_LOAD FLAGS(6);
  dynamic    PT_DYNAMIC FLAGS(6);
  tls	   PT_TLS  FLAGS(4);
  gnu_stack  PT_GNU_STACK FLAGS(6);
  gnu_relro  0x6474e552 FLAGS(4);
}

SECTIONS
{
  /* Read-only sections, merged into text segment: */
  . = SEGMENT_START("text-segment", 0) + SIZEOF_HEADERS;
  .gnu.hash       : { *(.gnu.hash) } :text
  .dynsym         : { *(.dynsym) } :text
  .dynstr         : { *(.dynstr) } :text
  .rela.dyn       :
    {
      *(.rela.init)
      *(.rela.text .rela.text.* .rela.gnu.linkonce.t.*)
      *(.rela.fini)
      *(.rela.rodata .rela.rodata.* .rela.gnu.linkonce.r.*)
      *(.rela.data .rela.data.* .rela.gnu.linkonce.d.*)
      *(.rela.tdata .rela.tdata.* .rela.gnu.linkonce.td.*)
      *(.rela.tbss .rela.tbss.* .rela.gnu.linkonce.tb.*)
      *(.rela.ctors)
      *(.rela.dtors)
      *(.rela.got)
      *(.rela.bss .rela.bss.* .rela.gnu.linkonce.b.*)
      *(.rela.ifunc)
    } :text
  .rela.plt       :
    {
      *(.rela.plt)
      *(.rela.iplt)
    } :text
  .init           :
  {
    KEEP (*(SORT_NONE(.init)))
  } :text =0x1f2003d5
  .plt            : ALIGN(16) { *(.plt) *(.iplt) } :text
  .text           :
  {
    *(.text.unlikely .text.*_unlikely .text.unlikely.*)
    *(.text.exit .text.exit.*)
    *(.text.startup .text.startup.*)
    *(.text.hot .text.hot.*)
    *(SORT(.text.sorted.*))
    *(.text .stub .text.* .gnu.linkonce.t.*)
    *(.gnu.warning)
  } :text =0x1f2003d5
  .fini           :
  {
    KEEP (*(SORT_NONE(.fini)))
  } :text =0x1f2003d5
  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);
  .rodata         : { *(.rodata .rodata.* .gnu.linkonce.r.*) } :text
  .eh_frame_hdr   : { *(.eh_frame_hdr) *(.eh_frame_entry .eh_frame_entry.*) } :text
  .eh_frame       : ONLY_IF_RO { KEEP (*(.eh_frame)) *(.eh_frame.*) } :text

  /* Kernel symbol table: strings */
  __ksymtab_strings : {
    *(__ksymtab_strings)
  } :text

  /* define hkipinfo section to provide addr */
  .hkipinfo :
  {
    QUAD(0)
    QUAD(__start_ro_after_init)
    QUAD(__end_ro_after_init)
  } :hkipinfo :text

  /* define ro_after_init section and provide __ro_after_init_start and _ero_after_init*/
  .data.ro_after_init :
  {
    . = ALIGN(4096);
    __start_ro_after_init = .;
    *(.ro_after_init .ro_after_init.*)
    __net_ro_after_init_hdlr_start = .;
    *(.ro_after_init_hdlr) *(.ro_after_init_hdlr.*)
    __net_ro_after_init_hdlr_end = .;
    . = ALIGN(4096);
    __end_ro_after_init  = .;
  } :roafterdata
  /* ro_after_init section defined */

  /* Kernel symbol table: Normal symbols */
  __ksymtab : {
    __start___ksymtab = .;
    KEEP(*(SORT(___ksymtab+*)))
    __stop___ksymtab = .;
  } :data
  /* Kernel symbol table: GPL-only symbols */
  __ksymtab_gpl : {
    __start___ksymtab_gpl = .;
    KEEP(*(SORT(___ksymtab_gpl+*)))
    __stop___ksymtab_gpl = .;
  } :data
  /* Kernel symbol table: GPL-future-only symbols */
  __ksymtab_gpl_future : {
    __start___ksymtab_gpl_future = .;
    KEEP(*(SORT(___ksymtab_gpl_future+*)))
    __stop___ksymtab_gpl_future = .;
  } :data
  /* Kernel symbol table: Normal symbols */
  __kcrctab : {
    __start___kcrctab = .;
    KEEP(*(SORT(___kcrctab+*)))
    __stop___kcrctab = .;
  } :data
  /* Kernel symbol table: GPL-only symbols */
  __kcrctab_gpl : {
    __start___kcrctab_gpl = .;
    KEEP(*(SORT(___kcrctab_gpl+*)))
    __stop___kcrctab_gpl = .;
  } :data
  /* Kernel symbol table: GPL-future-only symbols */
  __kcrctab_gpl_future : {
    __start___kcrctab_gpl_future = .;
    KEEP(*(SORT(___kcrctab_gpl_future+*)))
    __stop___kcrctab_gpl_future = .;
  } :data

  /* Adjust the address for the data segment. */
  . = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));
  .preinit_array    :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } :gnu_relro :data
  .init_array    :
  {
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
  } :gnu_relro :data
  .fini_array    :
  {
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
  } :gnu_relro :data
  .data.rel.ro : { *(.data.rel.ro.local* .gnu.linkonce.d.rel.ro.local.*) *(.data.rel.ro .data.rel.ro.* .gnu.linkonce.d.rel.ro.*) } :gnu_relro :data
  .dynamic        : { *(.dynamic) } :gnu_relro :dynamic :data
  .got            : { *(.got.plt) *(.igot.plt) *(.got) *(.igot) }  :gnu_relro :data
  . = DATA_SEGMENT_RELRO_END (0, .);

  .data           :
  {
    PROVIDE (__data_start = .);
    *(.data .data.* .gnu.linkonce.d.*)
    SORT(CONSTRUCTORS)
  } :data
  PROVIDE (_edata = .); PROVIDE (edata = .);
  . = .;
  PROVIDE (__bss_start = .);
  PROVIDE (__bss_start__ = .);
  .bss            :
  {
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*)
   *(COMMON)
   . = ALIGN(. != 0 ? 64 / 8 : 1);
  }
  PROVIDE (_bss_end__ = .); PROVIDE (__bss_end__ = .);
  . = ALIGN(64 / 8);
  . = SEGMENT_START("ldata-segment", .);
  . = ALIGN(64 / 8);
  PROVIDE (__end__ = .);
  PROVIDE (_end = .); PROVIDE (end = .);
  . = DATA_SEGMENT_END (.);
}
