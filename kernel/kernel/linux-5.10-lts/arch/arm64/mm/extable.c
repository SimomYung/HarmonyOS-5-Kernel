// SPDX-License-Identifier: GPL-2.0
/*
 * Based on arch/arm/mm/extable.c
 */

#include <linux/extable.h>
#include <linux/uaccess.h>
#include <asm/pointer_auth_misc.h>

int fixup_exception(struct pt_regs *regs)
{
	const struct exception_table_entry *fixup;
	u64 pc;

	fixup = search_exception_tables(instruction_pointer(regs));
	if (!fixup)
		return 0;

	if (in_bpf_jit(regs))
		return arm64_bpf_fixup_exception(fixup, regs);

	pc = (unsigned long)&fixup->fixup + fixup->fixup;
#ifdef CONFIG_ARM64_PTR_AUTH_DATA_FIELD
	set_exception_context_register(regs, REGS_PC, pc);
#else
	regs->pc = pc;
#endif
	return 1;
}
