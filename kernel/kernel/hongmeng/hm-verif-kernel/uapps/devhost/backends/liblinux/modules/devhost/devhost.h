/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 12 19:11:36 2019
 */
#ifndef __DEVHOST_DEVHOST_H__
#define __DEVHOST_DEVHOST_H__

#include <devhost/device.h>
#include <devhost/memory.h>
#include <linux/dma-mapping.h>
#include <linux/version.h>

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif

extern int pmu_disable(void);
extern int pmu_enable(void);
extern struct devhost_block_ops blkops;
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0) || defined(CONFIG_POSIX_TIMERS)
extern struct devhost_posix_clock_ops posix_clkops;
#endif

struct netlink_rcv_msg;
#ifdef CONFIG_NET
extern int devhost_net_init(void);
extern int devhost_net_stack_init(void);
extern int devhost_linux_netlink_recv(struct netlink_rcv_msg *args);
#else
static inline int devhost_net_init(void) { return 0; }
static inline int devhost_net_stack_init(void) { return 0; }
static inline int devhost_linux_netlink_recv(struct netlink_rcv_msg *args) { return -ENOSYS; }
#endif
extern void *liblinux_pal_alloc_dma_pool_ex(unsigned int *offset,
					    unsigned long long *paddr,
					    unsigned int size);

extern struct hm_driver_ops devhost_builtin_ops;
extern struct dma_map_ops dma_devhost_ops;
extern struct devhost_dev_pm_ops dev_pm_ops;

extern int devhost_module_init(void);
extern int vtimer_init(void);
extern int devhost_kvic_init(void);
extern int devhost_virqchip_init(void);
extern int vtimer_kevt_init(void);
extern bool is_vtimer_dying(unsigned int cpu);

extern int devhost_virqchip_add(struct fwnode_handle *fwnode, int irqctrl_id);
extern unsigned int devhost_kvic_create_mapping(unsigned int type);

#ifdef CONFIG_MDC_GODEL_HACK
extern int devhost_vitschip_add(struct fwnode_handle *fwnode, int irqctrl_id);
#endif
extern int devhost_vitschip_pmsi_add(struct fwnode_handle *handle, int irqctrl_id);

#ifdef CONFIG_DEVHOST_PM_PLUGIN
extern int devhost_power_init(void);
#else
static inline int devhost_power_init(void)
{
	return 0;
}
#endif

extern struct irq_domain *pci_msi_get_domain(struct pci_dev *dev);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
struct file *alloc_file(const struct path *path, int flags,
			const struct file_operations *fop);
#endif

#ifdef CONFIG_KEYS
int __init devhost_key_init(void);
#else
static inline int __init devhost_key_init(void)
{
	return 0;
}
#endif
extern long liblinux_key_request(const char *type, const char *name,
				 const char *callout_info, int dest_keyring);

extern long liblinux_key_read(unsigned long id, unsigned long buf, unsigned long size);
extern int devhost_postinit(int (*function)(void *arg), void *arg);
extern int devhost_net_fops_poll(void *ctx, void *pinfo, unsigned int *revents);
extern int devhost_poll_initwait(void *poll_info, void **pt);
extern int __init devhost_poll_init(void);
extern int devhost_transfs_fops_mmap(void *ctx, struct devhost_vma_info *vma_info);
extern int devhost_net_fops_mmap(void *ctx, struct devhost_vma_info *vma_info);

#endif /* __DEVHOST_DEVHOST_H__ */
