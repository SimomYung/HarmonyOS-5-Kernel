/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2022. All rights reserved.
 * Author: Huawei System Security Lab
 * Create: Thur Oct 19 11:15:05 2023
 */
IS_ENABLE_HKIP = 1;

OUTPUT_FORMAT("elf64-littleaarch64", "elf64-bigaarch64",
	      "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)

PHDRS {
	phdr         PT_PHDR PHDRS   FLAGS(4);
	interp       PT_INTERP       FLAGS(4);
        hkipinfo     0x60000008      FLAGS(4);
        text         PT_LOAD         FILEHDR PHDRS FLAGS(5);
        rodata       PT_LOAD         FLAGS(4);
        roafterdata  PT_LOAD         FLAGS(6);
        data         PT_LOAD         FLAGS(6);
        dynamic      PT_DYNAMIC      FLAGS(6);
        tls	     PT_TLS          FLAGS(4);
	gnu_eh_frame PT_GNU_EH_FRAME FLAGS(4);
        gnu_stack    PT_GNU_STACK    FLAGS(6);
        gnu_relro    0x6474e552      FLAGS(4);
}

SECTIONS
{
  /* Read-only sections, merged into text segment: */
  PROVIDE (__executable_start = SEGMENT_START("text-segment", 0)); . = SEGMENT_START("text-segment", 0) + SIZEOF_HEADERS;
  .interp         : { *(.interp) } :interp :text
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
    . = ALIGN(4096);
  } :text =0x1f2003d5
  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);
  .rodata         : { *(.rodata .rodata.* .gnu.linkonce.r.*) } :rodata
  .rodata1        : { *(.rodata1) } :rodata
  .eh_frame_hdr   : { *(.eh_frame_hdr) *(.eh_frame_entry .eh_frame_entry.*) } :gnu_eh_frame :rodata
  .eh_frame       : ONLY_IF_RO { KEEP (*(.eh_frame)) *(.eh_frame.*) } :rodata
  .gcc_except_table   : ONLY_IF_RO { *(.gcc_except_table .gcc_except_table.*) } :rodata
  .gnu_extab   : ONLY_IF_RO { *(.gnu_extab*) } :rodata
  .exception_ranges   : ONLY_IF_RO { *(.exception_ranges*) } :rodata

  /* define hkipinfo section to provide addr */
  .hkipinfo :
  {
    QUAD(IS_ENABLE_HKIP)
    QUAD(__start_ro_after_init)
    QUAD(__end_ro_after_init)
    . = ALIGN(4096);
  } :hkipinfo :rodata

  /* define ro_after_init section and provide __ro_after_init_start and _ero_after_init*/
  . = ALIGN(4096);
  __start_ro_after_init = .;
  .data.ro_after_init :
  {
    *(.ro_after_init .ro_after_init.*)
    *(.ro_after_init_hdlr) *(.ro_after_init_hdlr.*)
  } :roafterdata
  . = ALIGN(4096);
  __end_ro_after_init  = .;
  /* ro_after_init section defined */

  /* Adjust the address for the data segment. */
  . = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));
  /* Exception handling  */
  .eh_frame       : ONLY_IF_RW { KEEP (*(.eh_frame)) *(.eh_frame.*) } :data
  .gnu_extab      : ONLY_IF_RW { *(.gnu_extab) } :data
  .gcc_except_table   : ONLY_IF_RW { *(.gcc_except_table .gcc_except_table.*) } :data
  .exception_ranges   : ONLY_IF_RW { *(.exception_ranges*) } :data

  /* Thread Local Storage sections  */
  . = ALIGN(4096);
  .tdata	  :
   {
     PROVIDE_HIDDEN (__tdata_start = .);
     *(.tdata .tdata.* .gnu.linkonce.td.*)
   } :gnu_relro :data :tls
  .tbss		  : { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) } :data :tls
  .preinit_array    :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } :gnu_relro :data
  .init_array    :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  } :gnu_relro :data
  .fini_array    :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
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
  .got            :
  { *(.got.plt) *(.igot.plt) *(.got) *(.igot)
    . = ALIGN(4096);
  } :gnu_relro :data
  . = DATA_SEGMENT_RELRO_END (0, .);
  .data           :
  {
    PROVIDE (__data_start = .);
    *(.data .data.* .gnu.linkonce.d.*)
    SORT(CONSTRUCTORS)
  } :data

  .libdh.extensions : {
		__libdh_extpoint_start = .;
		*(.libdh.extensionpoints)
		__libdh_extpoint_end = .;
	} :data
	.libdh.extensions : {
		__libdh_plugin_start = .;
		*(.libdh.plugins)
		__libdh_plugin_end = .;
	} :data

  .data1          : { *(.data1) } :data
  _edata = .; PROVIDE (edata = .);
  . = .;
  __bss_start = .;
  __bss_start__ = .;
  .bss            :
  {
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*)
   *(COMMON)
   /* Align here to ensure that the .bss section occupies space up to
      _end.  Align after .bss to ensure correct alignment even if the
      .bss section disappears because there are no input sections.
      FIXME: Why do we need it? When there is no .bss section, we do not
      pad the .data section.  */
   . = ALIGN(. != 0 ? 64 / 8 : 1);
  }
  _bss_end__ = .; __bss_end__ = .;
  . = ALIGN(64 / 8);
  . = SEGMENT_START("ldata-segment", .);
  . = ALIGN(64 / 8);
  __end__ = .;
  _end = .; PROVIDE (end = .);
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
