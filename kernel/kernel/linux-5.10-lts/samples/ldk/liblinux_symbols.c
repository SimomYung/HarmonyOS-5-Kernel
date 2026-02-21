// SPDX-License-Identifier: GPL-2.0
#include <linux/of.h>
#include <linux/irq.h>
#include <linux/tick.h>
#include <linux/delay.h>
#include <linux/genhd.h>
#include <linux/compat.h>
#include <linux/libfdt.h>
#include <linux/of_clk.h>
#include <linux/reboot.h>
#include <linux/device.h>
#include <linux/hardirq.h>
#include <linux/irqdesc.h>
#include <linux/kallsyms.h>
#include <linux/notifier.h>
#include <linux/netdevice.h>
#include <linux/writeback.h>
#include <linux/syscore_ops.h>
#include <linux/timekeeping.h>
#include <linux/platform_drivers/platform_qos.h>
#include <linux/dma-map-ops.h>
#include <linux/cred.h>
#include <linux/alarmtimer.h>
#include <linux/blkdev.h>

#include <net/sock.h>
#include <net/compat.h>
#include <cpu_netlink/cpu_netlink.h>
#include <of_private.h>

/* extra linux symbols */
#ifdef CONFIG_BLOCK
EXPORT_SYMBOL(block_class);
#endif
EXPORT_SYMBOL(calibrate_delay);
EXPORT_SYMBOL(machine_power_off);
EXPORT_SYMBOL(machine_restart);
EXPORT_SYMBOL_GPL(device_get_devnode);
EXPORT_SYMBOL(reboot_notifier_list);
EXPORT_SYMBOL(syscore_shutdown);
EXPORT_SYMBOL(irq_enter);
EXPORT_SYMBOL(irq_exit);
EXPORT_SYMBOL(dma_alloc_contiguous);
EXPORT_SYMBOL(dma_free_contiguous);

/* used in transfs.ko/transfs.c */
EXPORT_SYMBOL(wakeup_flusher_threads);

#ifdef CONFIG_OF
/* used in of.ko, for clk subsystem init */
EXPORT_SYMBOL(of_clk_init);
EXPORT_SYMBOL(of_alias_scan);
#endif

/* used in devhost.ko/virqchip.c */
EXPORT_SYMBOL(__handle_domain_irq);

/* export libfdt APIs */
EXPORT_SYMBOL(fdt_create);
EXPORT_SYMBOL(fdt_add_reservemap_entry);
EXPORT_SYMBOL(fdt_finish_reservemap);
EXPORT_SYMBOL(fdt_begin_node);
EXPORT_SYMBOL(fdt_property);
EXPORT_SYMBOL(fdt_end_node);
EXPORT_SYMBOL(fdt_finish);

/* used in devhost.ko/power.c */
EXPORT_SYMBOL(tick_nohz_tick_stopped);
EXPORT_SYMBOL(tick_nohz_idle_stop_tick);
EXPORT_SYMBOL(tick_nohz_idle_enter);
EXPORT_SYMBOL(tick_nohz_idle_exit);

/*
 * export OF_DYNAMIC APIs
 * provide __weak symbols for backends not support OF_DYNAMIC
 */
struct property * __weak
__of_prop_dup(const struct property *prop, gfp_t allocflags)
{
	return NULL;
}
EXPORT_SYMBOL(__of_prop_dup);

struct device_node * __weak
__of_node_dup(const struct device_node *np, const char *full_name)
{
	return NULL;
}
EXPORT_SYMBOL(__of_node_dup);

/* ldk-5.10 symbols */
EXPORT_SYMBOL(kallsyms_lookup_name);

#ifdef CONFIG_NET
EXPORT_SYMBOL(dev_ioctl);
#endif

#ifdef CONFIG_LIBLINUX_NET_STACK
EXPORT_SYMBOL_GPL(get_compat_msghdr);
EXPORT_SYMBOL_GPL(cmsghdr_from_user_compat_to_kern);
EXPORT_SYMBOL(dev_ifconf);
EXPORT_SYMBOL(sock_getsockopt);
#ifdef CONFIG_NET_NS
EXPORT_SYMBOL(copy_net_ns);
#endif
#endif

/* used in iaware_notify.ko/iaware_notify.c */
EXPORT_SYMBOL_GPL(send_to_user);

#ifdef CONFIG_PLATFORM_QOS
EXPORT_SYMBOL(platform_qos_request);
#endif

#ifdef CONFIG_LTO
EXPORT_SYMBOL(get_liblinux_thread_offset);
EXPORT_SYMBOL(get_liblinux_thread_key);
#else
EXPORT_SYMBOL(liblinux_thread_offset);
EXPORT_SYMBOL(liblinux_thread_key);
#endif

/* used in cred.h */
EXPORT_SYMBOL(init_cred);

/* used in rtc.ko/virt_rtc.c  */
EXPORT_SYMBOL_GPL(set_power_on_alarm);

/* export mas blk APIs */
EXPORT_SYMBOL(mas_bkops_hm_nofity);
