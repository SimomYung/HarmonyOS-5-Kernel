// SPDX-License-Identifier: GPL-2.0
#include <linux/cpumask.h>

static struct cpumask default_activate_mask;

extern void liblinux_thread_get_default_affinity(unsigned long *mask);

void liblinux_get_cpus_allowed(struct cpumask *allowed_mask, struct cpumask *new_mask)
{
	int ret = 0;

	if (cpumask_empty(&default_activate_mask)) {
		liblinux_thread_get_default_affinity((unsigned long *)cpumask_bits(&default_activate_mask));
	}

	/*
	 * Non-percpu threads are expected to run in the default affinity mode to
	 * isolate some CPU resources. Percpu threads may run in the non-default
	 * affinity mode.
	 */
	ret = cpumask_and(allowed_mask, &default_activate_mask, new_mask);
	if (ret == 0) {
		if (cpumask_weight(new_mask) > 1U) {
			pr_info("set affinity 0x%lx for non-percpu thread\n", *cpumask_bits(new_mask));
		}
		cpumask_copy(allowed_mask, new_mask);
	}
}
