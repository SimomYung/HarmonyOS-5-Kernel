/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Exported liblinux APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 07 11:50:28 2020
 */
#include <hmkernel/types.h>
#include <hmkernel/compiler.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhmsrv_key/key.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_timer.h>

#include <liblinux/pal.h>
#include <lnxbase/lnxbase.h>
#include <devhost/log.h>
#include "libdh_linux.h"

/* extra linux symbols */
EXPORT_SYMBOL(block_class);
EXPORT_SYMBOL(calibrate_delay);
EXPORT_SYMBOL(machine_power_off);
EXPORT_SYMBOL(machine_restart);
EXPORT_SYMBOL_GPL(device_get_devnode);
EXPORT_SYMBOL(reboot_notifier_list);
EXPORT_SYMBOL(syscore_shutdown);
EXPORT_SYMBOL(irq_enter);
EXPORT_SYMBOL(irq_exit);

/* used in transfs.ko/transfs.c */
EXPORT_SYMBOL(wakeup_flusher_threads);

/* used in devhost.ko/dma.c */
EXPORT_SYMBOL(__dma_map_area);
EXPORT_SYMBOL(__dma_unmap_area);
EXPORT_SYMBOL(__dma_flush_area);

/* weak symbol for liblinux-4.4, which use the old name __dma_flush_range() */
void __attribute__((weak))
__dma_flush_area(const void *start, size_t size)
{
	liblinux_pal_flush_cache_range(LIBLINUX_PAL_DCACHE_FLUSH_RANGE,
				       (unsigned long)(uintptr_t)start,
				       (unsigned long)((uintptr_t)start + size));
}

/* weak symbols for liblinux arm32 nop cache model */
void __attribute__((weak)) __dma_map_area(const void *start, size_t size, int dir)
{
	UNUSED(start);
	UNUSED(size);
	UNUSED(dir);
}
void __attribute__((weak)) __dma_unmap_area(const void *start, size_t size, int dir)
{
	UNUSED(start);
	UNUSED(size);
	UNUSED(dir);
}

/* used in of.ko, for clk subsystem init */
EXPORT_SYMBOL(of_clk_init);

/* used in devhost.ko/virqchip.c */
EXPORT_SYMBOL(__handle_domain_irq);
/* used in devhost.ko/kvic.c */
EXPORT_SYMBOL(irq_domain_free_fwnode);
EXPORT_SYMBOL(irq_domain_reset_irq_data);
EXPORT_SYMBOL(irq_domain_get_irq_data);

EXPORT_SYMBOL(of_alias_scan);

EXPORT_SYMBOL(compat_alloc_user_space);
void *compat_alloc_user_space(unsigned long len)
{
	if (len == 0) {
		return NULL;
	}

	return malloc((size_t)len);
}

EXPORT_SYMBOL(strerror);

struct timespec64 {
	long long tv_sec;
	long tv_nsec;
};
int __getnstimeofday64(struct timespec64 *ts64)
{
	int ret;
	struct timespec ts;

	if (ts64 == NULL) {
		return -EINVAL;
	}

	ret = hm_clock_gettime(CLOCK_REALTIME, &ts);
	if (ret) {
		return -hmerrno2posix(ret);
	}
	ts64->tv_sec = (long long)ts.tv_sec;
	ts64->tv_nsec = ts.tv_nsec;
	return 0;
}
EXPORT_SYMBOL(__getnstimeofday64);

int do_settimeofday64(const struct timespec64 *ts64)
{
	struct timespec ts;

	if (ts64 == NULL) {
		return -EINVAL;
	}

	ts.tv_sec = (long)ts64->tv_sec;
	ts.tv_nsec = ts64->tv_nsec;
	return -hmerrno2posix(hm_clock_settime(CLOCK_REALTIME, &ts));
}
EXPORT_SYMBOL(do_settimeofday64);

void getrawmonotonic64(struct timespec64 *ts64)
{
	int ret;
	struct timespec ts;

	if (ts64 == NULL) {
		return;
	}

	ret = hm_clock_gettime(CLOCK_MONOTONIC, &ts);
	if (ret) {
		dh_error("failed to get momotonic time, err=%s\n",
			 hmstrerror(ret));
		ts64->tv_sec = 0;
		ts64->tv_nsec = 0;
		return;
	}
	ts64->tv_sec = ts.tv_sec;
	ts64->tv_nsec = ts.tv_nsec;
}
EXPORT_SYMBOL(getrawmonotonic64);

/* timekeeping */
void read_persistent_clock64(struct timespec64 *ts64)
{
	struct timespec ts;
	int ret;

	mem_zero_s(ts);
	ret = hm_clock_gettime(CLOCK_REALTIME, &ts);
	if (ret < 0) {
		dh_warn("get CLOCK_REALTIME failed: %s\n", hmstrerror(ret));
	}
	ts64->tv_sec = (long long)ts.tv_sec;
	ts64->tv_nsec = ts.tv_nsec;
}

void read_boot_clock64(struct timespec64 *ts64)
{
	struct timespec ts;
	int ret;

	mem_zero_s(ts);
	ret = hm_clock_gettime(CLOCK_BOOTTIME, &ts);
	if (ret < 0) {
		dh_warn("get CLOCK_BOOTTIME failed: %s\n", hmstrerror(ret));
	}
	ts64->tv_sec = (long long)ts.tv_sec;
	ts64->tv_nsec = ts.tv_nsec;
}

/* export libfdt APIs */
EXPORT_SYMBOL(fdt_create);
EXPORT_SYMBOL(fdt_add_reservemap_entry);
EXPORT_SYMBOL(fdt_finish_reservemap);
EXPORT_SYMBOL(fdt_begin_node);
EXPORT_SYMBOL(fdt_property);
EXPORT_SYMBOL(fdt_end_node);
EXPORT_SYMBOL(fdt_finish);

/*
 * export OF_DYNAMIC APIs
 * provide __weak symbols for backends not support OF_DYNAMIC
 */
__attribute__((weak))
struct property *
__of_prop_dup(const struct property *prop, gfp_t allocflags)
{
	UNUSED(prop, allocflags);
	return NULL;
}
EXPORT_SYMBOL(__of_prop_dup);

__attribute__((weak))
struct device_node *
__of_node_dup(const struct device_node *np, const char *fmt, ...)
{
	UNUSED(np, fmt);
	return NULL;
}
EXPORT_SYMBOL(__of_node_dup);

EXPORT_SYMBOL(liblinux_pal_preempt_init);
EXPORT_SYMBOL(liblinux_pal_preempt_resched);
EXPORT_SYMBOL(liblinux_pal_preempt_enable);
EXPORT_SYMBOL(liblinux_pal_preempt_disable);
EXPORT_SYMBOL(liblinux_pal_process_set_slv);
EXPORT_SYMBOL(liblinux_pal_thread_set_slv);
EXPORT_SYMBOL(liblinux_pal_processor_id);
EXPORT_SYMBOL(liblinux_pal_local_irq_enable);
EXPORT_SYMBOL(liblinux_pal_local_irq_disable);
EXPORT_SYMBOL(liblinux_pal_local_irq_save);
EXPORT_SYMBOL(liblinux_pal_local_irq_restore);
EXPORT_SYMBOL(liblinux_pal_local_save_flags);
EXPORT_SYMBOL(liblinux_pal_irq_set_affinity_hint);

EXPORT_SYMBOL(lnxbase_default_clocksource);
EXPORT_SYMBOL(dh_post_dev_populate_call);

long liblinux_key_request(const char *type, const char *name,
			 const char *callout_info, int dest_keyring)
{
#ifdef CONFIG_KEYMGR
	return hm_key_request(type, name, callout_info, dest_keyring);
#else
	UNUSED(type, name, callout_info, dest_keyring);
	hm_info("not support key request\n");
	return -ENOTSUP;
#endif
}
EXPORT_SYMBOL(liblinux_key_request);

long liblinux_key_read(unsigned long id, unsigned long buf, unsigned long size)
{
#ifdef CONFIG_KEYMGR
	return hm_key_control(KEY_CONTROL_READ, id, buf, size, 0);
#else
	UNUSED(id, buf, size);
	hm_info("not support key control\n");
	return -ENOTSUP;
#endif
}
EXPORT_SYMBOL(liblinux_key_read);
EXPORT_SYMBOL(liblinux_dfx_vmalloc_hook);
