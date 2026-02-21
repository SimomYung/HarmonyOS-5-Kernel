/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: platform idi2axi interface
 *
 * This file is released under the GPLv2.
 */
#ifndef PLAT_IDI2AXI_H
#define PLAT_IDI2AXI_H
#include <linux/types.h>

struct idi2axi_channel;
struct i2a_dev_base_t;

int plat_idi2axi_enable(struct i2a_dev_base_t *i2a_base_info);
int plat_idi2axi_disable(struct i2a_dev_base_t *i2a_base_info);

uint32_t plat_idi2axi_get_eof_channels(struct idi2axi_channel *ich);

void plat_idi2axi_push_buffer(struct idi2axi_channel *ich, uint64_t va);
uint64_t plat_idi2axi_pop_buffer(struct idi2axi_channel *ich);

int plat_idi2axi_channel_config(struct idi2axi_channel *ich);
void plat_idi2axi_channel_deconfig(struct idi2axi_channel *ich);
int plat_idi2axi_channel_working(struct idi2axi_channel *ich);
void disable_channel_base_irq(struct i2a_dev_base_t *i2a_base_info);
void plat_dump_status(struct i2a_dev_base_t *i2a_base_info);

#endif /* end of include guard: PLAT_IDI2AXI_H */
