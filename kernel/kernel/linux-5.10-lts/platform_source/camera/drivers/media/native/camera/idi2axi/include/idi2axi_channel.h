/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi device interface
 *
 * This file is released under the GPLv2.
 */
#ifndef IDI2AXI_H
#define IDI2AXI_H
#include <linux/ktime.h>
#include <linux/device.h>
#include "idi2axi_internal.h"

#define FLUSH_DONE_TIME 100

struct _channel_port_dt_buf_offset;

void i2a_channel_init(struct idi2axi_channel* channels, unsigned int max_ch_cnt, struct i2a_dev_base_t *i2a_base_info);
struct idi2axi_channel *idi2axi_request_channel(struct idi2axi_channel *channels,
		channel_eof_cb cb, void *cb_data, struct _channel_port_dt_buf_offset* param);
int idi2axi_channel_qbuf(struct idi2axi_channel *ch, struct buf_node *bn);
void idi2axi_channel_eof_handler(struct idi2axi_channel *ch);
int idi2axi_release_channel(struct idi2axi_channel *ch);
#endif /* end of include guard: IDI2AXI_H */
