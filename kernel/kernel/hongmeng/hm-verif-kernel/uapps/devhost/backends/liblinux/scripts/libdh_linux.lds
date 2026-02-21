INCLUDE ksymtab.lds

SECTIONS
{
	.liblinux.init.text BLOCK(CONSTANT(COMMONPAGESIZE)) : {
		*(.liblinux.init.text)
	}

	.liblinux.text BLOCK(CONSTANT(COMMONPAGESIZE)) : {
		*(.liblinux.text)
	}

	/* will be removed */
	.text.lock : {
		__lock_text_start = DEFINED(__lock_text_start) ? __lock_text_start : .;
		*(.spinlock.text)
		__lock_text_end = DEFINED(__lock_text_end) ? __lock_text_end : .;
	}
}
INSERT AFTER .text;

SECTIONS
{
	. = ALIGN((1 << 12));
	.data..percpu : AT(ADDR(.data..percpu) - 0) {
		__per_cpu_load = .;
		__per_cpu_start = .;
		__per_cpu_user_mapped_start = .;
		*(.data..percpu..first)
		. = ALIGN((1 << 7));
		*(.data..percpu..user_mapped)
		*(.data..percpu..user_mapped..shared_aligned)
		. = ALIGN((1 << 12));
		*(.data..percpu..user_mapped..page_aligned)
		__per_cpu_user_mapped_end = .;
		. = ALIGN((1 << 12));
		*(.data..percpu..page_aligned)
		. = ALIGN((1 << 7));
		*(.data..percpu..read_mostly)
		. = ALIGN((1 << 7));
		*(.data..percpu)
		*(.data..percpu..shared_aligned)
		__per_cpu_end = .;
	}

	.init.data : {
		__initcall_start = .;
		*(.initcallearly.init)
		__initcall0_start = .; *(.initcall0.init) *(.initcall0s.init)
		__initcall1_start = .; *(.initcall1.init) *(.initcall1s.init)
		__initcall2_start = .; *(.initcall2.init) *(.initcall2s.init)
		__initcall3_start = .; *(.initcall3.init) *(.initcall3s.init)
		__initcall4_start = .; *(.initcall4.init) *(.initcall4s.init)
		__initcall5_start = .; *(.initcall5.init) *(.initcall5s.init)
		__initcallrootfs_start = .; *(.initcallrootfs.init) *(.initcallrootfss.init)
		__initcall6_start = .; *(.initcall6.init) *(.initcall6s.init)
		__initcall7_start = .; *(.initcall7.init) *(.initcall7s.init)
		__initcall_end = .;
		__con_initcall_start = .;
		*(.con_initcall.init)
		__con_initcall_end = .;

		/* CLK_OF_TABLES() */
		. = ALIGN(8);
		__clk_of_table = .;
		*(__clk_of_table)
		*(__clk_of_table_end)

		/* IOMMU_OF_TABLES() */
		. = ALIGN(8);
		__iommu_of_table = .;
		*(__iommu_of_table)
		*(__iommu_of_table_end)

		/* Support kernel parameter */
		. = ALIGN(16);
		__setup_start = .;
		*(.init.setup)
		__setup_end = .;
	}

	.extralib.init.data : {
		__extralib_initcall_start = .;
		*(.extralib.init.data)
		__extralib_initcall_end = .;
	}
}
INSERT AFTER .data;

SECTIONS
{
	/* Built-in module parameters */
	__param : {
		__start___param = .;
		*(__param)
		__stop___param = .;
	}

	/* Built-in module versions. */
	__modver : {
		__start___modver = .;
		*(__modver)
		__stop___modver = .;
	}
}
INSERT BEFORE .data;

SECTIONS
{
	PROVIDE (_sdata = __data_start);
}
INSERT BEFORE .data;

SECTIONS
{
	PROVIDE (__start_rodata = .);
}
INSERT BEFORE .rodata;

SECTIONS
{
	.builtin_fw : {
		__start_builtin_fw = .;
		*(.builtin_fw)
		__end_builtin_fw = .;
	}
	PROVIDE (__end_rodata = .);
}
INSERT AFTER .rodata1;

SECTIONS
{
	PROVIDE (__liblinux_stext = .);
	PROVIDE (__liblinux_etext = __etext);
	PROVIDE (__liblinux_head = SEGMENT_START("text-segment", 0) + SIZEOF_HEADERS);
	PROVIDE (__liblinux_end = _end);
}
INSERT AFTER .text;

SECTIONS
{
	.liblinux.modules.rsvd BLOCK(CONSTANT(COMMONPAGESIZE)) (NOLOAD) : {
		*(.liblinux.modules.*)
	}
}
INSERT BEFORE .bss;

SECTIONS
{
	PROVIDE (__bss_stop = __bss_end__);
}
INSERT AFTER .bss;
