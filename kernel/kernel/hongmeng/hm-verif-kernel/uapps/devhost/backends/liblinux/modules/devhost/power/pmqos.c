/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Define interface of pmqos
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 16 15:00:45 UTC 2023
 */

#ifdef CONFIG_PLATFORM_QOS
#include <linux/platform_drivers/platform_qos.h>
#endif
#include <linux/pm_qos.h>
#include <linux/version.h>
#include <devhost/pm.h>
#include "devhost.h"
#include "linux_device.h"
#include "devhost.h"
#include "pmqos.h"
#ifdef CONFIG_VENDOR_HOOKS
#include <trace/hooks/liblinux.h>
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0) && !defined(CONFIG_PLATFORM_QOS)
/*
 * It is an adaptation to LDK 510 version without platform_qos support.
 */
enum {
	PM_QOS_RESERVED = 0,
	PM_QOS_CPU_DMA_LATENCY,
	PM_QOS_NETWORK_LATENCY,
	PM_QOS_NETWORK_THROUGHPUT,
	PM_QOS_MEMORY_BANDWIDTH,

	/* insert new class ID */
	PM_QOS_NUM_CLASSES,
};

static int pm_qos_add_notifier(int pm_qos_class, struct notifier_block *notifier)
{
	UNUSED(pm_qos_class);
	UNUSED(notifier);
	return 0;
}

static int pm_qos_request(int pm_qos_class)
{
	UNUSED(pm_qos_class);
	return PM_QOS_DEFAULT_VALUE;
}
#endif /* LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0) && !CONFIG_PLAFORM_QOS */

#define PM_QOS_ID_DEFAULT (-1)

#ifdef CONFIG_PLATFORM_QOS
#define LDK_QOS(type)	PLATFORM_QOS_##type
#else
#define LDK_QOS(type)	PM_QOS_##type
#endif /* CONFIG_PLATFORM_QOS */
#define HM_QOS(type)	PMQOS_TYPE_##type

/*
 * If you want to modify this enum, you need to modify pmqos type enum in
 * hm-verif-kernel/ulibs/include/libhmpm/pmqos.h,which corresponds to each other.
 * Currently, we cannot guarantee the mapping. Please check it.
 */
enum hm_pmqos_type {
	HM_QOS(CPU_DMA_LATENCY),
	HM_QOS(MEMORY_BANDWIDTH),
	HM_QOS(MEMORY_LATENCY),
	HM_QOS(MEMORY_THROUGHPUT),
	HM_QOS(MEMORY_THROUGHPUT_UP_THRESHOLD),
	HM_QOS(L1BUS_LATENCY),
	HM_QOS(NR),
};

struct pmqos_notifier_block {
	struct notifier_block nb;
	int id;
	int type;
	int hm_type;
};

#if (defined(CONFIG_PLATFORM_QOS) && defined(CONFIG_VENDOR_HOOKS) && defined(CONFIG_CPU_IDLE))
static void hook_ldk_vh_pm_qos_notify(void *args, struct pm_qos_constraints *c, int value)
{
	int ret;
	static int cpu_lat_id = PM_QOS_ID_DEFAULT;

	struct pm_qos_request req = {
		.qos = c,
	};
	if (!cpu_latency_qos_request_active(&req)) {
		return;
	}
	if (cpu_lat_id == PM_QOS_ID_DEFAULT) {
		ret = devhost_pmqos_request_add(HM_QOS(CPU_DMA_LATENCY), PM_QOS_DEFAULT_VALUE, &cpu_lat_id);
		if (ret < 0) {
			pr_err("liblinux: devhost_pmqos_request_add failed, type=%d, ret=%d\n",
			       HM_QOS(CPU_DMA_LATENCY), ret);
			return;
		}
	}
	ret = devhost_pmqos_request_update(HM_QOS(CPU_DMA_LATENCY), cpu_lat_id, value);
	if (ret < 0) {
		pr_err("pmqos update constraint value failed: %d, %d, %d\n",
			ret, HM_QOS(CPU_DMA_LATENCY), cpu_lat_id);
	}
}
#endif /* CONFIG_PLATFORM_QOS && CONFIG_VENDOR_HOOKS && CONFIG_CPU_IDLE */

static int pmqos_handle_notify(struct notifier_block *nb, unsigned long value, void *v)
{
	int id, ret, pmqos_type;
	struct pmqos_notifier_block *pmqos_nb = NULL;
	UNUSED(v);

	pmqos_nb = container_of(nb, struct pmqos_notifier_block, nb);
	id = pmqos_nb->id;
	pmqos_type = pmqos_nb->hm_type;
	/*
	 * pmqos value is int, "unsigned long" is only a notifier
	 * general interface
	 */
	ret = devhost_pmqos_request_update(pmqos_type, id, (int)value);
	if (ret < 0) {
		pr_err("pmqos update constraint value failed: %d, %d, %d\n",
			ret, pmqos_type, id);
		return ret;
	}

	return 0;
}

static struct pmqos_notifier_block pmqos_nbs[] = {
#ifndef CONFIG_PLATFORM_QOS
	{
		.id = PM_QOS_ID_DEFAULT,
		.type = LDK_QOS(CPU_DMA_LATENCY),
		.hm_type = HM_QOS(CPU_DMA_LATENCY),
		.nb.notifier_call = pmqos_handle_notify,
	},
	{
		.id = PM_QOS_ID_DEFAULT,
		.type = LDK_QOS(MEMORY_BANDWIDTH),
		.hm_type = HM_QOS(MEMORY_BANDWIDTH),
		.nb.notifier_call = pmqos_handle_notify,
	},
#endif /* !CONFIG_PLATFORM_QOS */
#if (defined(CONFIG_DEVFREQ_GOV_PM_QOS) || defined(CONFIG_DEVFREQ_GOV_PLATFORM_QOS))
	{
		.id = PM_QOS_ID_DEFAULT,
		.type = LDK_QOS(MEMORY_LATENCY),
		.hm_type = HM_QOS(MEMORY_LATENCY),
		.nb.notifier_call = pmqos_handle_notify,
	},
	{
		.id = PM_QOS_ID_DEFAULT,
		.type = LDK_QOS(MEMORY_THROUGHPUT),
		.hm_type = HM_QOS(MEMORY_THROUGHPUT),
		.nb.notifier_call = pmqos_handle_notify,
	},
	{
		.id = PM_QOS_ID_DEFAULT,
		.type = LDK_QOS(MEMORY_THROUGHPUT_UP_THRESHOLD),
		.hm_type = HM_QOS(MEMORY_THROUGHPUT_UP_THRESHOLD),
		.nb.notifier_call = pmqos_handle_notify,
	},
#if (defined(CONFIG_DEVFREQ_L1BUS_LATENCY) || defined(CONFIG_DEVFREQ_L1BUS_LATENCY_PLATFORM))
	{
		.id = PM_QOS_ID_DEFAULT,
		.type = LDK_QOS(L1BUS_LATENCY),
		.hm_type = HM_QOS(L1BUS_LATENCY),
		.nb.notifier_call = pmqos_handle_notify,
	},
#endif /* CONFIG_DEVFREQ_L1BUS_LATENCY || CONFIG_DEVFREQ_L1BUS_LATENCY_PLATFORM */
#endif /* CONFIG_DEVFREQ_GOV_PM_QOS || CONFIG_DEVFREQ_GOV_PLATFORM_QOS */
};

static int get_qos_request_value(struct pmqos_notifier_block pmqos_nb)
{
	int value = PM_QOS_DEFAULT_VALUE;

	#ifdef CONFIG_PLATFORM_QOS
		value = platform_qos_request(pmqos_nb.type);
	#else
		value = pm_qos_request(pmqos_nb.type);
	#endif

	return value;
}

int devhost_pmqos_init(void)
{
	int i, id, ret, pmqos_type;
	int nr = sizeof(pmqos_nbs) / sizeof(struct pmqos_notifier_block);

	for (i = 0; i < nr; i++) {
		pmqos_type = pmqos_nbs[i].hm_type;
		ret = devhost_pmqos_request_add(pmqos_type, get_qos_request_value(pmqos_nbs[i]), &id);
		if (ret < 0) {
			pr_err("liblinux: devhost_pmqos_request_add failed, type=%d, ret=%d\n",
			       pmqos_type, ret);
			return ret;
		}
		pmqos_nbs[i].id = id;
#ifdef CONFIG_PLATFORM_QOS
		platform_qos_add_notifier(pmqos_nbs[i].type, &pmqos_nbs[i].nb);
#else
		pm_qos_add_notifier(pmqos_nbs[i].type, &pmqos_nbs[i].nb);
#endif
	}

#if (defined(CONFIG_PLATFORM_QOS) && defined(CONFIG_VENDOR_HOOKS) && defined(CONFIG_CPU_IDLE))
	ret = register_trace_ldk_vh_pm_qos_notify(hook_ldk_vh_pm_qos_notify, NULL);
	if (ret) {
		pr_warn("hooks, liblinux: register_trace_ldk_vh_pm_qos_notify failed, ret=%d\n", ret);
	}
#endif /* CONFIG_PLATFORM_QOS && CONFIG_VENDOR_HOOKS && CONFIG_CPU_IDLE */

	return 0;
}
