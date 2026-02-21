/*
 * freq_autodown.h
 *
 * bus automatic frequency reduction
 *
 * Copyright (c) 2015-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __FREQ_AUTODOWN_H__
#define __FREQ_AUTODOWN_H__

#include <linux/device.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#define FREQ_AUTODOWN_DTS_ATTR_LEN 64
#define FREQ_AUTODOWN_DTS_REG_NUM  3
#define CLOCK_DISABLE_REG_OFFSET   0x04
#define CLOCK_STATE_REG_OFFSET     0x08
#define FREQ_AUTODOWN_CLK_NUM      3
#define INVALID_REG_ADDR           0xDEAD
#ifndef BIT
#define BIT(n)                     (1 << (n))
#endif
#define BIT_MSK(x)                 (BIT(x) << 16)

#define UNBYPASS_VALUE(n, bp_mask) (BIT_MSK(n) | (BIT(n) & (bp_mask)))
#define BYPASS_VALUE(n, bp_mask) (BIT_MSK(n) | (BIT(n) & (~(bp_mask))))

#define AUTOFS_DEBUG(fmt, ...)     pr_debug("[Info]:%s(%d): " fmt "\n", \
	__FUNCTION__, __LINE__, ##__VA_ARGS__)

enum enable_state {
	DISABLE,
	ENABLE,
};

enum open_state {
	CLOSE,
	OPEN,
};

enum freq_autodown_base_address {
	MEDIA_CRG_BASE_ADDR,
	CRG_PERI_BASE_ADDR,
	MEDIA2_CRG_BASE_ADDR,
	FREQ_AUTODOWN_BASEADDR_NUM,
};

struct freq_autodown_reg_info {
	u32 disable_bypass_reg;
	u32 disable_bypass_bit;
	u32 disable_bypass_addr_flag;
	u32 enable_clock_reg;
	u32 enable_clock_bit;
	u32 enable_clock_addr_flag;
	u32 bypass_low_mask;
};

struct freq_autodown_info {
	struct freq_autodown_reg_info clks_reg[FREQ_AUTODOWN_CLK_NUM];
	u32 clk_num;
};

struct freq_autodown_desc {
	struct resource *res;
	struct device *dev;
	void __iomem *regs;
	void __iomem *base_addr[FREQ_AUTODOWN_BASEADDR_NUM];
	struct freq_autodown_info *freq_info;
	unsigned int freq_autodown_num;
	unsigned int freq_autodown_baseaddress_num;
	unsigned int freq_autodown_state;
	char **freq_autodown_name;
	spinlock_t lock;
};

int freq_autodown_clk_set(const char *freq_name, u32 control_flag);

#endif

