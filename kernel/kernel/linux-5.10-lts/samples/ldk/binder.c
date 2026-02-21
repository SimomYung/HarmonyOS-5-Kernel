// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/init.h>

static uint32_t binder_debug_mask;
module_param_named(debug_mask, binder_debug_mask, uint, 0644);

static int __init binder_module_init(void)
{
	return 0;
}
static void __exit binder_module_exit(void)
{
}

MODULE_LICENSE("GPL");
module_init(binder_module_init);
module_exit(binder_module_exit);