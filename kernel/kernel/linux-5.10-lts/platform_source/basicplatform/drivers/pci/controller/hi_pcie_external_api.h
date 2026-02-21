/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe external api.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_API_H
#define HI_PCIE_API_H

#include <linux/types.h>
#include <linux/notifier.h>
#include <linux/pci.h>

#define PCIE_PORT_ID0		0
#define MPS_DEFAULT		0

struct hi_pcie_event_handler {
	u32 event;
	void *data;
	struct notifier_block notify;
	void (*fn)(struct hi_pcie_event_handler *);
};

#ifdef CONFIG_HIPCIE_CORE
int hi_pcie_startup(u32 cdx);
void hi_pcie_idle_vote(u32 cdx, u32 pdx, bool pro);
int hi_pcie_register_event_handler(u32 cdx, u32 pdx, struct hi_pcie_event_handler *handler);
int hi_pcie_unregister_event_handler(u32 cdx, u32 pdx, struct hi_pcie_event_handler *handler);
int hi_pcie_register_power_callback(u32 cdx, u32 pdx,
	int (*poweron_callback)(void *data),
	int (*poweroff_callback)(void *data), void *data);
int hi_pcie_pm_ctrl(u32 cdx, int power_ops);
int hi_pcie_lp_ctrl(u32 cdx, bool enable);
int hi_pcie_remove_device(u32 cdx, u32 pdx);
int hi_pcie_rescan_device(u32 cdx, u32 pdx);
int hi_pcie_set_speed(u32 cdx, u32 pdx, u16 speed);
int hi_pcie_cfg_l0sl1(u32 cdx, u32 aspm);
int hi_pcie_cfg_l1ss(u32 cdx, u32 aspm);
int hi_pcie_power_ctrl(u32 cdx, u32 power_flag);
int hi_pcie_core_setup(u32 cdx);
int hi_pcie_establish_link(u32 cdx);
int hi_pcie_cfg_mps(struct pci_dev *target_dev, u32 cdx, u32 pdx, u32 mps);
#else
static inline int hi_pcie_startup(u32 cdx)
{
	return 0;
}

static inline void hi_pcie_idle_vote(u32 cdx, u32 pdx, bool pro)
{
	return;
}

static inline int hi_pcie_register_event_handler(u32 cdx, u32 pdx, struct hi_pcie_event_handler *handler)
{
	return 0;
}

static inline int hi_pcie_unregister_event_handler(u32 cdx, u32 pdx, struct hi_pcie_event_handler *handler)
{
	return 0;
}

static inline int hi_pcie_register_power_callback(u32 cdx, u32 pdx,
	int (*poweron_callback)(void *data),
	int (*poweroff_callback)(void *data), void *data)
{
	return 0;
}
static inline int hi_pcie_pm_ctrl(u32 cdx, int power_ops)
{
	return 0;
}

static inline int hi_pcie_lp_ctrl(u32 cdx, bool enable)
{
	return 0;
}

static inline int hi_pcie_remove_device(u32 cdx, u32 pdx)
{
	return 0;
}

static inline int hi_pcie_rescan_device(u32 cdx, u32 pdx)
{
	return 0;
}

static inline int int hi_pcie_set_speed(u32 cdx, u32 pdx, u16 speed)
{
	return 0;
}

int hi_pcie_cfg_l0sl1(u32 cdx, u32 aspm)
{
	return 0;
}

int hi_pcie_cfg_l1ss(u32 cdx, u32 aspm)
{
	return 0;
}

int hi_pcie_power_ctrl(u32 cdx, u32 power_flag)
{
	return 0;
}

int hi_pcie_core_setup(u32 cdx)
{
	return 0;
}

int hi_pcie_establish_link(u32 cdx)
{
	return 0;
}

int hi_pcie_cfg_mps(struct pci_dev *target_dev, u32 cdx, u32 pdx, u32 mps)
{
	return 0;
} 
#endif

#ifdef CONFIG_HIPCIE_SLT
int hi_pcie_slt_hook_register(u32 cdx, u32 pdx, u32 device_type, int (*init)(void *),
			   int (*prepare)(void *), int (*shutdown)(void *),
			   int (*startup)(void *),
			   int (*data_transfer)(void *, u32, u32));
#endif

#endif
