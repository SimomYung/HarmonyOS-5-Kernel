/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: UDK PCI framework interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 22 17:24:26 2021
 */
#ifndef __UDK_PCI_H__
#define __UDK_PCI_H__

enum udk_pci_mem_type {
	UDK_PCI_MEM_IO = 0,		/* 32 bit io memory */
	UDK_PCI_MEM_NORMAL,		/* 32/64 bit non-prefetchable normal memory */
	UDK_PCI_MEM_NORMAL_PREF,	/* 32 bit prefetchable normal memory */
	UDK_PCI_MEM_NORMAL_PREF64,	/* 64 bit prefetchable normal memory */
	UDK_PCI_MEM_TYPE_NR,
};

struct udk_pci_ctx {
	unsigned char bus_id;
	struct udk_pci_host *host;
};

struct udk_pci_slot {
	unsigned char id;
	unsigned char func;
};

struct udk_pci_ops {
	int (*read)(const struct udk_pci_ctx *ctx, const struct udk_pci_slot *slot,
		    unsigned int offset, unsigned int len, unsigned int *val);
	int (*write)(const struct udk_pci_ctx *ctx, const struct udk_pci_slot *slot,
		     unsigned int offset, unsigned int len, unsigned int val);
};

struct udk_pci_mem {
	unsigned long long cpu_addr;
	unsigned long long pci_addr;
	unsigned long long size;
};

struct udk_pci_host {
	unsigned char init_bus_id;
	struct udk_pci_mem mem_space[(int)UDK_PCI_MEM_TYPE_NR];
	void *private;
	struct udk_pci_ops ops;
};

#ifdef CONFIG_UDK_PCI

int udk_pci_host_register(struct udk_pci_host *host);
int udk_pci_host_unregister(const struct udk_pci_host *host);

#else

#include <udk/errno.h>
#include <libstrict/strict.h>

static inline int udk_pci_host_register(struct udk_pci_host *host)
{
	UNUSED(host);
	return -ENOSYS;
}

static inline int udk_pci_host_unregister(const struct udk_pci_host *host)
{
	UNUSED(host);
	return -ENOSYS;
}

#endif /* CONFIG_UDK_PCI */

#endif /* __UDK_PCI_H__ */
