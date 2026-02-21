/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe idle.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_IDLE_H
#define HI_PCIE_IDLE_H

#include <linux/types.h>
#include <linux/platform_device.h>

struct hi_pcie_idle_irq {
	u32		timeout_irq;
	u32		wake_irq;
};

struct hi_pcie_idle_port {
	u32		id;
	u32		idle_l1ss_cnt_en;
	u32		idle_l1ss_cnt_en_bitmask;
	u32		idle_l1ss_cnt_en_addr;
	u32		idle_l1ss_cnt_time_set;
	u32		idle_l1ss_cnt_time_set_addr;
	u32		idle_timeout_mask_bit;
	u32		idle_timeout_mask_bitmask;
	u32		idle_timeout_mask_addr;
	u32		idle_timeout_clr_bit;
	u32		idle_timeout_clr_bitmask;
	u32		idle_timeout_clr_addr;
	u32		idle_wake_mask_bit;
	u32		idle_wake_mask_bitmask;
	u32		idle_wake_mask_addr;
	u32		idle_wake_clr_bit;
	u32		idle_wake_clr_bitmask;
	u32		idle_wake_clr_addr;
	atomic_t	device_idle_vote;
	atomic_t	idle_state;
	struct		hi_pcie_idle_irq	irq;
	struct		hi_pcie				*pcie;
	struct		hi_pcie_port		*pcie_port;
};

struct hi_pcie_idle {
	void __iomem	*idle_reg_base;
	u32		host_id;
	u32		port_num;
	u32		idle_enable;
	atomic_t	host_idle_vote;
	spinlock_t	idle_lock;
	struct	hi_pcie_idle_port	port[2];
	void		(*startup)(struct hi_pcie_idle *idle);
	void		(*shutdown)(struct hi_pcie_idle *idle);
};

#ifdef CONFIG_HIPCIE_IDLE
int hi_pcie_idle_hook_register(struct platform_device *pdev);
void hi_pcie_idle_host_vote(struct hi_pcie_idle *idle, u32 id, u32 vote);
void hi_pcie_idle_device_vote(u32 cdx, u32 pdx, u32 vote);
static inline void hi_pcie_32k_idle_init(struct hi_pcie_idle *idle)
{
	if (idle && idle->startup)
		idle->startup(idle);
}
static inline void hi_pcie_32k_idle_exit(struct hi_pcie_idle *idle)
{
	if (idle && idle->shutdown)
		idle->shutdown(idle);
}
static inline unsigned long hi_pcie_idle_spin_lock(struct hi_pcie_idle *idle,
		unsigned long flags)
{
	if (idle)
		spin_lock_irqsave(&(idle->idle_lock), flags);
	return flags;
}
static inline void hi_pcie_idle_spin_unlock(struct hi_pcie_idle *idle,
		unsigned long flags)
{
	if (idle)
		spin_unlock_irqrestore(&(idle->idle_lock), flags);
}
void hi_pcie_idle_enable_irq(struct hi_pcie_idle *idle);
void hi_pcie_idle_disable_irq(struct hi_pcie_idle *idle);
void hi_pcie_idle_mask_int(struct hi_pcie_idle *idle);
void hi_pcie_idle_clear_irq(struct hi_pcie_idle *idle);
#else
static inline int hi_pcie_idle_hook_register(struct platform_device *dev)
{
	return 0;
}
static inline void hi_pcie_idle_host_vote(struct hi_pcie_idle *idle, u32 id, u32 vote)
{
	return;
}
static inline void hi_pcie_idle_device_vote(u32 cdx, u32 pdx, u32 vote)
{
	return;
}
static inline void hi_pcie_32k_idle_init(struct hi_pcie_idle *idle)
{
	return;
}
static inline void hi_pcie_32k_idle_exit(struct hi_pcie_idle *idle)
{
	return;
}
static inline unsigned long hi_pcie_idle_spin_lock(struct hi_pcie_idle *idle,
		unsigned long flags)
{
	return 0;
}
static inline void hi_pcie_idle_spin_unlock(struct hi_pcie_idle *idle,
		unsigned long flags)
{
	return;
}
static inline void hi_pcie_idle_enable_irq(struct hi_pcie_idle *idle)
{
	return;
}
static inline void hi_pcie_idle_disable_irq(struct hi_pcie_idle *idle)
{
	return;
}
static inline void hi_pcie_idle_mask_int(struct hi_pcie_idle *idle)
{
	return;
}
static inline void hi_pcie_idle_clear_irq(struct hi_pcie_idle *idle)
{
	return;
}
#endif
#endif

