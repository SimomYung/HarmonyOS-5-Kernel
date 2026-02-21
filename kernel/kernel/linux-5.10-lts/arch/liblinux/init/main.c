// SPDX-License-Identifier: GPL-2.0-only
/*
 * liblinux init main
 */

#include <linux/init.h>
#include <linux/kallsyms.h>
#include <asm/sections.h>
#include <linux/printk.h>

#ifdef CONFIG_ARM64_PTR_AUTH_FWD_CFI
#include <asm/pointer_auth_misc.h>
#endif

#include <trace/hooks/liblinux.h>

static const char * const g_initcall_blacklist[] = {
	"ion_system_contig_heap_create",
	"cpu_hotplug_pm_sync_init",
};

static
void hook_initcall_blacklist(void *data, initcall_t fn, int *ret)
{
	char fn_name[KSYM_SYMBOL_LEN];
	unsigned long addr;
	int i;

#ifdef CONFIG_ARM64_PTR_AUTH_FWD_CFI
	addr = (unsigned long)pauth_autia(dereference_function_descriptor(fn),
		__builtin_get_modifier_bytype(initcall_t));
#else
	addr = (unsigned long) dereference_function_descriptor(fn);
#endif
	if (addr == NULL) {
		printk("%s add is null\n", __func__);
		return;
	}
	sprint_symbol_no_offset(fn_name, addr);

	/*
	 * fn will be "function_name [module_name]" where [module_name] is not
	 * displayed for built-in init functions.  Strip off the [module_name].
	 */
	strreplace(fn_name, ' ', '\0');

	for (i = 0; i < ARRAY_SIZE(g_initcall_blacklist); i++) {
		if (strcmp(fn_name, g_initcall_blacklist[i]) == 0) {
			*ret = true;
			printk("%s fn_name:%s\n", __func__, fn_name);
			return;
		}
	}
}
INIT_VENDOR_HOOK(ldk_rvh_initcall_blacklist, hook_initcall_blacklist);
