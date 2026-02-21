/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description : information about lpcpu cpu info hide
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <asm/cputype.h>

static struct hide_info {
	unsigned int impl;
	unsigned int partnum;
	unsigned int hide_midr;
} g_hide[] = {
	{ ARM_CPU_IMP_HISI, 0xD05, 0x482FD022 },
	{ ARM_CPU_IMP_HISI, 0xD06, 0x482FD030 },
	{ ARM_CPU_IMP_HISI, 0xD23, 0x411FD461 },
	{ ARM_CPU_IMP_HISI, 0xD24, 0x482FD423 },
	{ ARM_CPU_IMP_HISI, 0xD43, 0x411FD0D0 },
#ifdef CONFIG_CPUID_MID_LIT_EXCLUSIVE
	{ ARM_CPU_IMP_HISI, 0xD03, 0x411FD0D0 },
	{ ARM_CPU_IMP_HISI, 0xD42, 0x410FD050 },
	{ ARM_CPU_IMP_HISI, 0xD43, 0x411FD0D0 },
#endif
	{ ARM_CPU_IMP_HISI, 0xD46, 0x482FD422 },
	{ ARM_CPU_IMP_HISI, 0xD47, 0x482FD423 },
#ifdef CONFIG_CPUID_BIG_MID_UNIQUE
	{ ARM_CPU_IMP_ARM, ARM_CPU_PART_CORTEX_A77, 0x484fd400 },
#endif
#ifdef CONFIG_CPUID_LIT_EXCLUSIVE
	{ ARM_CPU_IMP_ARM, ARM_CPU_PART_CORTEX_A55, 0x410fd032 },
#endif
};

u32 __attribute_const__ read_cpuid_id(void)
{
	u32 const midr = read_cpuid(MIDR_EL1);
	u32 const impl = MIDR_IMPLEMENTOR(midr);
	u32 const partnum = MIDR_PARTNUM(midr);
	u32 i;
	struct hide_info *p;

	for (p = g_hide, i = 0; i < sizeof(g_hide)/sizeof(g_hide[0]); i++, p++)
		if ((impl == p->impl) && (partnum == p->partnum))
			return p->hide_midr;
	return midr;
}
