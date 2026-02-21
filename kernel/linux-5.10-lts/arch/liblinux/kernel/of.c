// SPDX-License-Identifier: GPL-2.0-only
#include <linux/device.h>
#include <linux/export.h>
#include <linux/notifier.h>
#include <linux/of.h>
#include <linux/of_clk.h>
#include <linux/of_fdt.h>
#include <linux/proc_fs.h>

#include "of_private.h"

/* notify-chain for early fdt */
static BLOCKING_NOTIFIER_HEAD(of_earlyfdt_chain);

int of_register_earlyfdt_notifier(struct notifier_block *nb)
{
	if (of_have_populated_dt()) {
		return -EINVAL;
	}
	return blocking_notifier_chain_register(&of_earlyfdt_chain, nb);
}
EXPORT_SYMBOL_GPL(of_register_earlyfdt_notifier);

int __init liblinux_setup_fdt(void *dt_virt)
{
	struct device_node *np;
	int ret;

	if (of_have_populated_dt()) {
		return -EBUSY;
	}

	if (!dt_virt || !early_init_dt_scan(dt_virt)) {
		pr_crit("liblinux: invalid device tree blob.\n");
		while (true)
			cpu_relax();
	}

	/* include __unflatten_dt() and of_alis_scan() */
	unflatten_device_tree();

	/* re-do sysfs works in of_core_init() */
	mutex_lock(&of_mutex);
	for_each_of_allnodes(np)
		__of_attach_node_sysfs(np);
	mutex_unlock(&of_mutex);
	proc_symlink("device-tree", NULL, "/sys/firmware/devicetree/base");

	/* call earlyfdt notifier */
	ret = blocking_notifier_call_chain(&of_earlyfdt_chain, 0, NULL);
	if (ret < 0) {
		return ret;
	}

	/* of misc init */
	of_clk_init(NULL);

	return 0;
}
EXPORT_SYMBOL_GPL(liblinux_setup_fdt);
