// SPDX-License-Identifier: GPL-2.0-or-later
/* Rewritten by Rusty Russell, on the backs of many others...
   Copyright (C) 2001 Rusty Russell, 2002 Rusty Russell IBM.

*/
#include <linux/ftrace.h>
#include <linux/memory.h>
#include <linux/extable.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/init.h>
#include <linux/kprobes.h>
#include <linux/filter.h>

#include <asm/sections.h>
#include <linux/uaccess.h>

/*
 * mutex protecting text section modification (dynamic code patching).
 * some users need to sleep (allocating memory...) while they hold this lock.
 *
 * Note: Also protects SMP-alternatives modification on x86.
 *
 * NOT exported to modules - patching kernel text is a really delicate matter.
 */
DEFINE_MUTEX(text_mutex);

extern struct exception_table_entry __start___ex_table[];
extern struct exception_table_entry __stop___ex_table[];

/* Cleared by build time tools if the table is already sorted. */
u32 __initdata __visible main_extable_sort_needed = 1;

/* Sort the kernel's built-in exception table */
void __init sort_main_extable(void)
{
	if (main_extable_sort_needed &&
	    &__stop___ex_table > &__start___ex_table) {
		pr_notice("Sorting __ex_table...\n");
		sort_extable(__start___ex_table, __stop___ex_table);
	}
}

/* Given an address, look for it in the kernel exception table */
const
struct exception_table_entry *search_kernel_exception_table(unsigned long addr)
{
	return search_extable(__start___ex_table,
			      __stop___ex_table - __start___ex_table, addr);
}

/* Given an address, look for it in the exception tables. */
const struct exception_table_entry *search_exception_tables(unsigned long addr)
{
	const struct exception_table_entry *e;

	e = search_kernel_exception_table(addr);
	if (!e)
		e = search_module_extables(addr);
	if (!e)
		e = search_bpf_extables(addr);
	return e;
}
