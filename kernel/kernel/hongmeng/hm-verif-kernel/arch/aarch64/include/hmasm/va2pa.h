/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Provide va to pa conversion using AT instruction
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 13 13:31:13 2022
 */

#ifndef AARCH64_HMASM_VA2PA_H
#define AARCH64_HMASM_VA2PA_H

#include <hmkernel/compiler.h>
/* For read_sysreg */
#include <hmasm/processor.h>
#include <hmasm/barrier.h>
#include <hmasm/memory.h>
#include <hmasm/irqflags.h>

#define PAR_EL1_F_extract(x)	((x) & 0x1ul)

/* When F == 0, translation success */
#define PAR_EL1_SH_extract(x)		(((x) >> 7u) & 0x3ul)
#define PAR_EL1_NS_extract(x)		(((x) >> 9u) & 0x1ul)
/* 36 bits */
#define PAR_EL1_PA_47_12_extract(x)	(((x) >> 12u) & 0xffffffffful)
/* 4 bits */
#define PAR_EL1_PA_51_48_extract(x)	(((x) >> 48u) & 0xful)
/* 8 bits */
#define PAR_EL1_ATTR_extract(x)		(((x) >> 56u) & 0xfful)


/* When F == 1, translation fault */
#define PAR_EL1_FST_extract(x)	(((x) >> 1u) & 0x3ful)
#define PAR_EL1_PTW_extract(x)	(((x) >> 8u) & 0x1ul)
#define PAR_EL1_S_extract(x)	(((x) >> 9u) & 0x1ul)

/*
 * ARM64 support only 52 bits physical address, so use -1 to represent invalid
 * pa would be ok.
 */
#define ARCH_VA2PA_INVALID_PA	((paddr_t)(0xffffffffffffffff))

static inline paddr_t arch_va2pa(const void __user *va)
{
	u64 par;
	paddr_t pa;

#ifdef CONFIG_ENABLE_KERN_HANDLE_IRQ
	/* par_el1 is not saved & restored during kernel's pmu interrupt handling, so we need to manually
	 * disable & enable irq around at & par_el1 reading. Otherwise, when perf's sample handler overwrites
	 * this register, the normal va2pa operation's result would be corrupted.
	 */
	unsigned long irq = arch_local_irq_save();
#endif
	/*
	 * S1E1R: Performs stage 1 address translation, with permissions as if reading from the
	 * given virtual address from EL1.
	 */
	asm volatile ("at s1e1r, %[va]" :: [va] "r" (va));
	isb();
	par = read_sysreg("par_el1");

#ifdef CONFIG_ENABLE_KERN_HANDLE_IRQ
	arch_local_irq_restore(irq);
#endif

	if (PAR_EL1_F_extract(par) == 0) {
		pa = (PAR_EL1_PA_47_12_extract(par) << 12u) + (PAR_EL1_PA_51_48_extract(par) << 48u);
		pa += ptr_to_ulong(va) & 0xffful;
	} else {
		pa = ARCH_VA2PA_INVALID_PA;
	}

	return pa;
}

#endif
