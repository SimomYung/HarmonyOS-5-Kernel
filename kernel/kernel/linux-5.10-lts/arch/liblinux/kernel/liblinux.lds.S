/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * ld script to make liblinux dynamic library
 */

/*
 * Avoid including RO_AFTER_INIT_DATA twice. which cause the value of
 * __start___jump_label and __stop___jump_label is reassigned.
 */
#define RO_AFTER_INIT_DATA
#define RUNTIME_DISCARD_EXIT
#define RO_EXCEPTION_TABLE_ALIGN	8

#include <asm-generic/vmlinux.lds.h>
#include <asm/cache.h>
#include <asm/page.h>
#include <asm/thread_info.h>
#include <asm/memory.h>
#include <liblinux/kapi.h>

#define PT_GNU_RELRO	0x6474e552
#define PT_GNU_STACK	0x6474e551
#define PT_HKIP     0x60000008
#define IS_ENABLE_HKIP  1
#define MB(n)		(1024 * 1024 * (n))	/* for huge page mappings */

PHDRS
{
	text PT_LOAD FILEHDR PHDRS ;
	initdata PT_LOAD ;
	hkipinfo PT_HKIP FLAGS(4);
	roafterdata PT_LOAD FLAGS(6);
	data PT_LOAD ;
	rwdata PT_LOAD FLAGS(6);
	tls PT_TLS FLAGS(4);
	module_rsvd PT_LOAD FLAGS(0);
	dynamic PT_DYNAMIC ;
	note PT_NOTE ;
	stack PT_GNU_STACK FLAGS(6);
	relro PT_GNU_RELRO FLAGS(4);
}

SECTIONS
{
	. = SEGMENT_START("text-segment", 0) + SIZEOF_HEADERS;
	.note.gnu.build-id  : { *(.note.gnu.build-id) } : text : note
	.hash           : { *(.hash) } : text
	.gnu.hash       : { *(.gnu.hash) }
	.dynsym         : { *(.dynsym) }
	.dynstr         : { *(.dynstr) }
	.gnu.version    : { *(.gnu.version) }
	.gnu.version_d  : { *(.gnu.version_d) }
	.gnu.version_r  : { *(.gnu.version_r) }
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
		/* vmlinux generated rela sections */
		*(.rela_ftrace_eval_map)
		*(.rela_ftrace_events)
		*(.rela__*)
		*(.rela.con_initcall.init*)
		*(.rela.pci_fixup*)
		*(.rela.init.*)
		*(.rela.initcall*)
		*(.rela.exitcall*)
		*(.rela.ref.*)
		*(.rela.fixup.*)
		*(.rela.liblinux.*)
		*(.rela.lsm_info.*)
	}
	/* use a separate RELA section to ensure MUSL libc works properly */
	.rela.plt       :
	{
		*(.rela.plt)
		*(.rela.iplt)
	}
	.plt            : ALIGN(16) { *(.plt) *(.iplt) }

	.text : ALIGN(PAGE_SIZE) {
		_text = .;
		_stext = .;
		IRQENTRY_TEXT
		SOFTIRQENTRY_TEXT
		TEXT_TEXT
		SCHED_TEXT
		LOCK_TEXT
		KPROBES_TEXT
		LIBLINUX_TEXT
		*(.fixup)
		. = ALIGN(16);
	}
	. = ALIGN(PAGE_SIZE);
	_etext = .;

	.eh_frame : ONLY_IF_RO { KEEP (*(.eh_frame)) *(.eh_frame.*) }

	. = ALIGN(PAGE_SIZE);
	__init_begin = .;
	__inittext_begin = .;
	INIT_TEXT_SECTION(8)
	__exittext_begin = .;
	.exit.text : {
		EXIT_TEXT
	}
	__exittext_end = .;
	. = ALIGN(4);
	.altinstructions : {
		__alt_instructions = .;
		*(.altinstructions)
		. = ALIGN(PAGE_SIZE);
		__alt_instructions_end = .;
	}
	.altinstr_replacement : {
		*(.altinstr_replacement)
	}
	. = ALIGN(CONSTANT(COMMONPAGESIZE));
	__init_text_end = .;
	. = ALIGN(MB(2));
	__init_data_begin = .;
	INIT_DATA_SECTION(8) : initdata
	.extralib.init.data : {
		__vendor_hook_initcall_start = .;
		*(.liblinux.vh.init.data)
		__vendor_hook_initcall_end = .;
		__extralib_initcall_start = .;
		*(.extralib.init.data)
		__extralib_initcall_end = .;
	}
	.fixup.boot_param : {
		__fixup_boot_param_start = .;
		*(.fixup.boot_param)
		__fixup_boot_param_end = .;
	}
	PERCPU_SECTION(L1_CACHE_BYTES)
	. = ALIGN(PAGE_SIZE);
	__initdata_end = .;
	__init_end = .;

	PRMEM_SECTIONS
	/*
	 * The ".data.ro_after_init" section must be placed before DATA_SEGMENT_ALIGN,
	 * otherwise it will be included in RELRO segment which is NOT writable after
	 * relocation.
	 */
	.data.ro_after_init : {
		. = ALIGN(PAGE_SIZE);
		__start_ro_after_init = .;
		*(.data..ro_after_init .data.rel.ro.*)
		JUMP_TABLE_DATA
		STATIC_CALL_DATA
		. = ALIGN(PAGE_SIZE);
		__end_ro_after_init = .;
	} : roafterdata

	. = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));

	/* RELRO Segment Start */
	.eh_frame : ONLY_IF_RW { KEEP (*(.eh_frame)) *(.eh_frame.*) }
	. = ALIGN(MB(2));
	/* TLS Segment */
	.tdata		: { *(.tdata .tdata.* .gnu.linkonce.td.*) } : data : relro : tls
	.tbss		: { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) } : tls
	.init_array	: { KEEP (*(.init_array)) } : data : relro
	/* vmlinux's RO_DATA is relro */
	.data.rel.ro	: { *(.data.rel.ro.local* .gnu.linkonce.d.rel.ro.local.*) *(.data.rel.ro .data.rel.ro.* .gnu.linkonce.d.rel.ro.*) } : data : relro
	.dynamic	: { *(.dynamic) } : dynamic : data : relro
	.got		: { *(.got) *(.igot) } : data : relro
#ifdef CONFIG_HW_LOGGER
	. = ALIGN(32);
	__start_hwlog_tag = .;
	__start_hwlog_tag : {
		*(__hwlog_tag)
	} : data : relro
	. = ALIGN(CONSTANT(COMMONPAGESIZE));
	__stop_hwlog_tag = .;
#endif
	RO_DATA(PAGE_SIZE)
	. = ALIGN(PAGE_SIZE);
	. = DATA_SEGMENT_RELRO_END (0, .);

	.got.plt	: { *(.got.plt) *(.igot.plt) } : data

	.hkipinfo : {
		QUAD(IS_ENABLE_HKIP)
		QUAD(__start_ro_after_init)
		QUAD(__end_ro_after_init)
	} : hkipinfo : data

	_data = .;
	_sdata = .;
#ifndef CONFIG_DEBUG_KMEMLEAK
	. = ALIGN(MB(2));
#endif
	.data : { *(.data..read_mostly) } : rwdata
	RW_DATA(L1_CACHE_BYTES, PAGE_SIZE, THREAD_ALIGN)
	.data : { *(.data.rel) *(.data.rel.local) } : rwdata
	.data : {
		__liblinux_unimpl_entry_start = . ;
		*(.__liblinux_unimpl_entry)
		__liblinux_unimpl_entry_end = .;
	} : rwdata
	_edata = .;

	BSS_SECTION(0, 0, 0)

	_end = .;
	. = DATA_SEGMENT_END (.);

	/* for jiffies */
	.liblinux.self.jiffies : {
		PROVIDE(jiffies_64 = .);
#ifdef __ARMEB__
		jiffies = jiffies_64 + 4;
#else
		jiffies = jiffies_64;
#endif
	}

	/* for module rsvd */
	. = ALIGN(MB(2));
	.liblinux.modules.rsvd ALIGN(CONSTANT(COMMONPAGESIZE)) (NOLOAD) : {
		__liblinux_modules_pool = .;
		. = . + (1 << CONFIG_LIBLINUX_MODULES_RSVD_BITS);
		__liblinux_modules_pool_end = .;
	} : module_rsvd

	.head.text : { HEAD_TEXT }
	ASSERT(SIZEOF(.head.text) == 0, "Unexpected early init text (.head.text) detected!")

	DWARF_DEBUG
	ELF_DETAILS

	DISCARDS

	/DISCARD/ : {
		*(.rela.discard.*)
	}

	/* ARM need extra DISCARD */
	#ifdef CONFIG_ARM
	/DISCARD/ : {
		*(.ARM.exidx.exit.text)
		*(.ARM.extab.exit.text)
		*(.ARM.exidx.text.exit)
		*(.ARM.extab.text.exit)
	}
	#endif
}
