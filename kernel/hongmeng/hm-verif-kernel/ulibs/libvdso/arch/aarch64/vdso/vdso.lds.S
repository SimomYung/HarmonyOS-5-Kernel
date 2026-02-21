/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 14 19:24:38 2019
 */
#include <hmasm/page.h>

#ifdef __ILP32__
OUTPUT_FORMAT("elf32-littleaarch64",
	      "elf32-bigaarch64",
	      "elf32-littleaarch64")
OUTPUT_ARCH(aarch64:ilp32)
#else
OUTPUT_FORMAT("elf64-littleaarch64",
	      "elf64-bigaarch64",
	      "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
#endif

ENTRY(vdso_sigentry)

SECTIONS
{
	PROVIDE(_vdso_data = . - __PAGE_SIZE);
	. = 0 + SIZEOF_HEADERS;

	.gnu.hash	: { *(.gnu.hash) }		:text
	.hash		: { *(.hash) }			:text
	.dynstr		: { *(.dynstr) }		:text
	.dynsym		: { *(.dynsym) }		:text

	. = ALIGN(16);

	.eh_frame_hdr   : { *(.eh_frame_hdr) *(.eh_frame_entry .eh_frame_entry.*) } :ehframe :text

	.eh_frame	: { KEEP (*(.eh_frame)) }	:text

	.text		: { *(.text*) }			:text

	.rodata		: { *(.rodata*) }		:text

	_end = .;
	PROVIDE(end = .);

	. = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));
	.dynamic	: { *(.dynamic) }		:dynamic	:text
	. = DATA_SEGMENT_RELRO_END(0, .);
	. = DATA_SEGMENT_END (.);

	/DISCARD/ : {
	*(.note.GNU-stack)
	*(.data .data.* .gnu.linkonce.d.* .sdata*)
	*(.bss .sbss .dynbss .dynsbss)
	*(.plt)
	}
}
PHDRS
{
 text PT_LOAD FILEHDR PHDRS FLAGS(5);
 dynamic PT_DYNAMIC FLAGS(4);
 note PT_NOTE FLAGS(4);
 gnu_stack PT_GNU_STACK FLAGS (6);
 gnu_relro 0x6474e552 FLAGS(4);
 ehframe PT_GNU_EH_FRAME FLAGS(4);
}
VERSION
{
	/*
	 * The version(LINUX_2.6.39) is fixed and compatible with linux vdso aarch64.
	 * Reference: https://man7.org/linux/man-pages/man7/vdso.7.html
	 * Some app would depends on the fixed version such as golang.
	 * Reference: https://go.googlesource.com/go/+/refs/tags/go1.21.4/src/runtime/vdso_linux_arm64.go#14
	 */
	LINUX_2.6.39 {
	global:
		__kernel_clock_gettime;
		__kernel_gettimeofday;
		__kernel_gettimeudata;
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
		__kernel_rt_sigreturn;
		vdso_get_data;
		safe_copy;
		safe_copy_align;
		safe_copy_nopf;
	local: *;
	};
}
