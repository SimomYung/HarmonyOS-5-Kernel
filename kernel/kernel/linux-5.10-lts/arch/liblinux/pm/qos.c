// SPDX-License-Identifier: GPL-2.0-only
#include <linux/platform_drivers/platform_qos.h>
#include <linux/export.h>

#ifdef CONFIG_DEVFREQ_GOV_PLATFORM_QOS
void platform_qos_add_request_memory_latency(struct platform_qos_request *req, s32 value)
{
	platform_qos_add_request(req, PLATFORM_QOS_MEMORY_LATENCY, value);
}
EXPORT_SYMBOL(platform_qos_add_request_memory_latency);
#endif
