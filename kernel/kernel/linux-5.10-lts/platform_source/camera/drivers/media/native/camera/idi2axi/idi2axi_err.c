/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <soc_media2_crg_interface.h>
#include <soc_idi2axi_interface.h>
#include "idi2axi_internal.h"
#include "idi2axi_err.h"
#include "reg_helper.h"
#include <securec.h>
#include <linux/types.h>

#define IDI2AXI_ERR_TYPE_MAX_NUM 10   // register num, no more than 12

static uint32_t err_reg_offset[IDI2AXI_ERR_TYPE_MAX_NUM] = {
    SOC_IDI2AXI_STAT0_ADDR(0),
    SOC_IDI2AXI_STAT1_ADDR(0),
    SOC_IDI2AXI_STAT2_ADDR(0),
    SOC_IDI2AXI_STAT3_ADDR(0),
    SOC_IDI2AXI_STAT4_ADDR(0),
    SOC_IDI2AXI_STAT5_ADDR(0),
    SOC_IDI2AXI_STAT6_ADDR(0),
    SOC_IDI2AXI_STAT7_ADDR(0),
    SOC_IDI2AXI_STAT8_ADDR(0),
    SOC_IDI2AXI_STAT9_ADDR(0),
};

int read_err_info(struct idi2axi_channel *ich, idi2axi_err_info *m)
{
    if (!ich || !ich->i2a_base_info || !ich->i2a_base_info->i2a_base || !m) {
        pr_err("%s input param is error", __func__);
        return -1;
    }
    int i ;
    uint32_t registers[IDI2AXI_ERR_TYPE_MAX_NUM] = {0};
    void __iomem *i2a_base = ich->i2a_base_info->i2a_base;
    for (i = 0; i < IDI2AXI_ERR_TYPE_MAX_NUM; i++) {
        registers[i] = read_reg(i2a_base + err_reg_offset[i]);
        m->info[i] = registers[i];
        pr_info("%s SOC_IDI2AXI_STAT%d_ADDR value is %#x", __func__, i, registers[i]);
    }
    return 0;
}
