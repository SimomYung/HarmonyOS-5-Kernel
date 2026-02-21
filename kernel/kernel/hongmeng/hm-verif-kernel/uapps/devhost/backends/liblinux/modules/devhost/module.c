/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: devhost module
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 12 15:30:27 2021
 */
#include <linux/rculist.h>
#include <linux/module.h>
#include <linux/version.h>

#include <lnxbase/lnxbase.h>

#include "devhost.h"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 0)
# define mod_core_base(mod)	((mod)->core_layout.base)
# define mod_core_size(mod)	((mod)->core_layout.size)
# define mod_init_base(mod)	((mod)->init_layout.base)
# define mod_init_size(mod)	((mod)->init_layout.size)
#else
# define mod_core_base(mod)	((mod)->module_core)
# define mod_core_size(mod)	((mod)->core_size)
# define mod_init_base(mod)	((mod)->module_init)
# define mod_init_size(mod)	((mod)->init_size)
#endif

static int __fill_mod_syminfo(struct module *mod, bool fill_init)
{
	unsigned long strtab_len;
	struct liblinux_mod_syminfo si;
	int ret;

	/*
	 * The sysmgr require the exact strtab len for query strtab,
	 * but it's a little hard to get it from `struct module`.
	 * The `core_size - offset_of(strtab)` should be large enough
	 * to cover whole strtab, we use it instead of real strtab len.
	 */
	strtab_len = mod_core_size(mod) - ((unsigned long)mod->core_kallsyms.strtab - \
					   (unsigned long)mod_core_base(mod));

	/* fill core symbols */
	si.symtab = (const void *)mod->core_kallsyms.symtab;
	si.symtab_len = mod->core_kallsyms.num_symtab * sizeof(Elf_Sym);
	si.strtab = mod->core_kallsyms.strtab;
	si.strtab_len = (unsigned int)strtab_len;
	si.module_core = mod_core_base(mod);
	si.core_size = mod_core_size(mod);
	ret = lnxbase_fill_mod_symbol(mod->name, &si);
	if (ret < 0) {
		pr_err("load core syminfo for module %s failed, ret=%d\n",
		       mod->name, ret);
		return ret;
	}

	/* fill init symbols */
	if (fill_init) {
		si.symtab = (const void *)mod->kallsyms->symtab;
		si.symtab_len = mod->kallsyms->num_symtab * sizeof(Elf_Sym);
		si.strtab = mod->kallsyms->strtab;
		si.strtab_len = (unsigned int)strtab_len;
		si.module_core = mod_init_base(mod);
		si.core_size = mod_init_size(mod);
		ret = lnxbase_fill_mod_symbol(mod->name, &si);
		if (ret < 0) {
			pr_err("load init syminfo for module %s failed, ret=%d\n",
			       mod->name, ret);
			lnxbase_drop_mod_symbol(mod_core_base(mod));
			return ret;
		}
	}
	return 0;
}

static int
__mod_syminfo_cb(struct notifier_block *this, unsigned long event, void *ptr)
{
	struct module *mod = (struct module *)ptr;
	int ret = NOTIFY_DONE;

	switch (event) {
	case MODULE_STATE_COMING:
		/* fill mod symbol for dumpstack */
		ret = __fill_mod_syminfo(mod, true);
		ret = (ret < 0) ? notifier_from_errno(ret) : NOTIFY_OK;
		break;
	case MODULE_STATE_LIVE:
		/* kill init symbol after module is alive */
		lnxbase_drop_mod_symbol(mod_init_base(mod));
		break;
	case MODULE_STATE_GOING:
		/* kill core symbol */
		lnxbase_drop_mod_symbol(mod_core_base(mod));
		/*
		 * after MODULE_STATE_LIVE, the MOD_INIT_BASE will be NULL,
		 * and init symbol has been killed already
		 */
		if (mod_init_base(mod) != NULL) {
			lnxbase_drop_mod_symbol(mod_init_base(mod));
		}
		break;
	default:
		/* nothing to do */
		break;
	}

	return ret;
}

static struct notifier_block mod_syminfo_nb = {
	.notifier_call = __mod_syminfo_cb,
};

int __init devhost_module_init(void)
{
	struct module *mod = NULL;
	int ret;

	/* foreach loaded modules */
	list_for_each_entry_rcu(mod, THIS_MODULE->list.prev, list) {
		/* skip symbol module without module core */
		if (mod_core_base(mod) == NULL) {
			continue;
		}
		/* fill syminfo for it */
		ret = __fill_mod_syminfo(mod, false);
		if (ret < 0) {
			pr_err("load core syminfo for module %s failed, ret=%d\n",
			       mod->name, ret);
			return ret;
		}
	}

	/* register module notifier for futher module syminfo */
	ret = register_module_notifier(&mod_syminfo_nb);
	if (ret < 0) {
		pr_err("register module syminfo support failed, ret=%d\n", ret);
		return ret;
	}

	return 0;
}
