// SPDX-License-Identifier: GPL-2.0
#include <linux/cpu.h>

/*
 * override symbols in drivers/base/cpu.c
 * initialize cpu as EMPTY subsys without any sysfs entries
 */
void __override cpu_dev_init(void)
{
	static const struct attribute_group *dummy_attr_groups[] = { NULL };

	/*
	 * move cpu subsys of ldk to other place to avoid conflict
	 * with outside world
	 */
	cpu_subsys.name = "ldk-cpu";

	if (subsys_system_register(&cpu_subsys, dummy_attr_groups))
		panic("Failed to register CPU subsystem");
}
