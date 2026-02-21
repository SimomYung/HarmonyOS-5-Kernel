// SPDX-License-Identifier: GPL-2.0-only
#include <linux/init.h>
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/irq.h>
#include <linux/of_irq.h>

#include <trace/hooks/liblinux.h>
#include <liblinux/pal.h>

int __ref liblinux_register_irqchip_table(const struct of_device_id *table)
{
	if (system_state >= SYSTEM_RUNNING) {
		return -EINVAL;
	}
	of_irq_init(table);
	return 0;
}
EXPORT_SYMBOL(liblinux_register_irqchip_table);

static void hook_set_affinity_hint(void *args, struct irq_desc *desc, const struct cpumask *m, int *ret)
{
	unsigned int affinity_hint;
	struct irq_data *data;

	data = irq_desc_get_irq_data(desc);
	affinity_hint = (m != NULL) ? cpumask_bits(m)[0] : 0;
	*ret = liblinux_pal_irq_set_affinity_hint(data->hwirq, affinity_hint);
}

INIT_VENDOR_HOOK(ldk_rvh_set_affinity_hint, hook_set_affinity_hint);
