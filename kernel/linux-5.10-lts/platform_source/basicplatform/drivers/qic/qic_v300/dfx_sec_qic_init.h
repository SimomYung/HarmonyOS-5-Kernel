/*
 * QIC V300 init functions head file.
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef __DFX_SEC_QIC_INIT_H
#define __DFX_SEC_QIC_INIT_H
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include "dfx_sec_qic.h"

#define REG_ADDR_INVALID    0x0
struct qic_tb_info {
	char *tb_name;
	u32  mmidx_num;
	u32  *start_addr_head;
};

struct qic_access_info {
	u32 ib_num;
	char **ib_name;
	u32 tb_num;
	struct qic_tb_info *tb_info;
};

struct qic_bus_info {
	u32 bus_id;
	char *bus_name;
	struct qic_access_info *access_info;
};

struct qic_mid_info {
	u32 start;
	u32 end;
	char *mid_name;
};

struct qic_em_info {
	u32 qic_bus_id;
	u32 em_base_addr;
	void __iomem *em_intr_addr;
	u32 em_intr_mask;
	char *em_name;
};

struct qic_sub_intr_info {
	u32 qic_sub_intr_index;
	u32 qic_em_num;
	struct qic_em_info *em_info;
};

struct qic_chk_reg {
	void __iomem *clk_reg;
	void __iomem *rst_reg;
	u32 clk_addr;
	u32 clk_mask;
	u32 rst_addr;
	u32 rst_mask;
};

struct dfx_sec_qic_device {
	u32 irq_number;
	u32 irq_reg_num;
	struct qic_src_reg *irq_reg;
	struct qic_chk_reg *irq_chk;
	bool qic_ath;
	u32 sub_intr_number;
	struct qic_sub_intr_info *sub_intr_info;
	u32 bus_info_num;
	struct qic_bus_info *bus_info;
	u32 mid_info_num;
	struct qic_mid_info *mid_info;
};

int dfx_sec_qic_common_init(const struct device_node *np, struct platform_device *pdev);
void dfx_sec_qic_free_source(struct dfx_sec_qic_device *qic_dev);
#endif
