// SPDX-License-Identifier: GPL-2.0-only
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/string.h>
#include <asm/bootparam.h>

const struct boot_param param_all[] __section(".fixup.boot_param") = {
	{1, "fw_devlink", "off", 0},
};

extern const struct obs_kernel_param __setup_start[], __setup_end[];
extern const struct boot_param __fixup_boot_param_start[], __fixup_boot_param_end[];

static const struct boot_param *boot_param_of(const char *name, int is_early)
{
	const struct boot_param *p;

	for (p = __fixup_boot_param_start; p < __fixup_boot_param_end; p++) {
		if (p->is_early == is_early) {
			if (strcmp(name, p->name) == 0) {
				return p;
			}
		}
	}
	return NULL;
}

static void liblinux_set_early_param(void)
{
	const struct obs_kernel_param *p;
	const struct boot_param *params;

	for (p = __setup_start; p < __setup_end; p++) {
		params = boot_param_of(p->str, 1);
		if (params == NULL) {
			continue;
		}
		if (p->setup_func((char *)params->val) != 0) {
			if (params->should_panic) {
				panic("set value failed for '%s'\n", p->str);
			} else {
				pr_warn("set value failed for '%s'\n", p->str);
			}
		} else {
			pr_info("set value succeed for '%s'\n", p->str);
		}
	}
}

static void liblinux_set_normal_param(void)
{
	const struct kernel_param *p;
	const struct boot_param *params;

	for (p = __start___param; p < __stop___param; p++) {
		params = boot_param_of(p->name, 0);
		if (params == NULL) {
			continue;
		}
		if (p->ops->set(params->val, p) != 0) {
			if (params->should_panic) {
				panic("set value failed for '%s'\n", p->name);
			} else {
				pr_warn("set value failed for '%s'\n", p->name);
			}
		} else {
			pr_info("set value succeed for '%s'\n", p->name);
		}
	}
}

void liblinux_set_param(void)
{
	liblinux_set_early_param();
	liblinux_set_normal_param();
}
