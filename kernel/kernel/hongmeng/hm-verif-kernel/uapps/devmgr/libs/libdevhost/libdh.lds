/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2022. All rights reserved.
 * Author: Huawei System Security Lab
 * Create: Tue Aug 20 08:38:32 2024
 */

OUTPUT_FORMAT("elf64-littleaarch64", "elf64-bigaarch64",
	      "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)

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
  .note.gnu.build-id  : { *(.note.gnu.build-id) } :text
  .hash           : { *(.hash) } :text
  .gnu.hash       : { *(.gnu.hash) } :text
  .dynsym         : { *(.dynsym) } :text
  .dynstr         : { *(.dynstr) } :text
  .gnu.version    : { *(.gnu.version) } :text
  .gnu.version_d  : { *(.gnu.version_d) } :text
  .gnu.version_r  : { *(.gnu.version_r) } :text
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
  .rodata1        : { *(.rodata1) } :text
  .eh_frame_hdr   : { *(.eh_frame_hdr) *(.eh_frame_entry .eh_frame_entry.*) } :text
  .eh_frame       : ONLY_IF_RO { KEEP (*(.eh_frame)) *(.eh_frame.*) } :text
  .gcc_except_table   : ONLY_IF_RO { *(.gcc_except_table .gcc_except_table.*) } :text
  .gnu_extab   : ONLY_IF_RO { *(.gnu_extab*) } :text
  .exception_ranges   : ONLY_IF_RO { *(.exception_ranges*) }

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
    *(.ro_after_init_hdlr) *(.ro_after_init_hdlr.*)
    . = ALIGN(4096);
    __end_ro_after_init  = .;
  } :roafterdata
  /* ro_after_init section defined */

  /* Adjust the address for the data segment. */
  . = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));
  /* Exception handling  */
  .eh_frame       : ONLY_IF_RW { KEEP (*(.eh_frame)) *(.eh_frame.*) } :data
  .gnu_extab      : ONLY_IF_RW { *(.gnu_extab) } :data
  .gcc_except_table   : ONLY_IF_RW { *(.gcc_except_table .gcc_except_table.*) } :data
  .exception_ranges   : ONLY_IF_RW { *(.exception_ranges*) } :data

  /* Thread Local Storage sections  */
  .tdata	  :
   {
     *(.tdata .tdata.* .gnu.linkonce.td.*)
   } :gnu_relro :data :tls
  .tbss		  : { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) } :data :tls
  .preinit_array    :
  {
    KEEP (*(.preinit_array))
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
  .ctors          :
  {
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  }
  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  }
  .jcr            : { KEEP (*(.jcr)) }
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
  .data1          : { *(.data1) } :data
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

  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  .gnu.build.attributes : { *(.gnu.build.attributes .gnu.build.attributes.*) }
  /* DWARF debug sections. */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  .debug_pubtypes 0 : { *(.debug_pubtypes) }
  .debug_ranges   0 : { *(.debug_ranges) }
  .debug_macro    0 : { *(.debug_macro) }
  .debug_addr     0 : { *(.debug_addr) }
  .ARM.attributes 0 : { KEEP (*(.ARM.attributes)) KEEP (*(.gnu.attributes)) }
  .note.gnu.arm.ident 0 : { KEEP (*(.note.gnu.arm.ident)) }
  /DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) *(.gnu.lto_*) }
}
