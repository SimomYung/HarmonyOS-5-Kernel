/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 11 12:52:03 2024
 */
#include <hmasm/page.h>

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(vdso_sigentry)

#define ALIGN_PAGE	ALIGN(PAGE_SIZE)

SECTIONS
{
	PROVIDE(_vdso_data = . - __PAGE_SIZE);
	. = 0 + SIZEOF_HEADERS;

	.gnu.hash	: { *(.gnu.hash) }		:rodata
	.hash		: { *(.hash) }			:rodata
	.dynstr		: { *(.dynstr) }		:rodata
	.dynsym		: { *(.dynsym) }		:rodata

	. = ALIGN(16);

	.eh_frame	: { KEEP (*(.eh_frame)) }	:rodata

	.rodata		: { *(.rodata*) }		:rodata

	.text		: ALIGN_PAGE { *(.text*) }	:text

	.dynamic	: ALIGN_PAGE { *(.dynamic) }	:dynamic
	_end = .;
	PROVIDE(end = .);

	. = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));
	. = DATA_SEGMENT_RELRO_END(0, .);
	. = DATA_SEGMENT_END (.);

	/DISCARD/ : {
	*(.note.GNU-stack)
	*(.data .data.* .gnu.linkonce.d.* .sdata*)
	*(.bss .sbss .dynbss .dynsbss)
	*(.plt .got .igot)
	}
}
PHDRS
{
 rodata PT_LOAD FILEHDR PHDRS FLAGS(4);
 text PT_LOAD FLAGS(5);
 dynamic PT_DYNAMIC FLAGS(4);
 note PT_NOTE FLAGS(4);
 gnu_stack PT_GNU_STACK FLAGS (6);
 gnu_relro 0x6474e552 FLAGS(4);
}
VERSION
{
	/*
	 * The version(LINUX_2.6) is fixed and compatible with linux vdso arm.
	 * Reference: https://man7.org/linux/man-pages/man7/vdso.7.html
	 * Some app would depends on the fixed version such as golang.
	 */
	LINUX_2.6 {
	global:
		__vdso_clock_gettime;
	};
}

VERSION
{
	HongMeng_1.8.0 {
	global:
		_except_tag_start;
		_except_tag_end;
		_except_fixup;
		_except_tag_nopf_start;
		_except_tag_nopf_end;
		vdso_get_data;
		safe_copy;
		safe_copy_align;
		safe_copy_nopf;
	local: *;
	};
}
