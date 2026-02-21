/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi common power interface
 *
 * This file is released under the GPLv2.
 */
#ifndef I2A_MGR_H
#define I2A_MGR_H
#include <linux/types.h>
#include <linux/device.h>
#include "idi2axi_internal.h"
#include "idi2axi_intf.h"

#define DQUEUE_TIMEOUT 200

struct req_node {
	struct list_head entry;
	unsigned int fd;
	unsigned int eof_cnt;
	unsigned int flags;
	unsigned int msg_id;
	unsigned int api_name;
};

struct done_node {
	struct list_head entry;
	unsigned int msg_id;
	unsigned int api_name;
	union {
		struct buf_node bn;
		int ret;
	} u;
};

typedef struct _tag_i2a_mgr_t {
	struct list_head req_list;
	struct list_head done_list;
	unsigned int each_sensor_channel_cnt;
	unsigned int all_work_ch_cnt;
	spinlock_t buf_lock;
	struct completion eof_done;
	unsigned int buf_size;
	bool close_dev;
}i2a_mgr_t;

void i2a_mgr_init(i2a_mgr_t *i2a_mgr);
int i2a_attach_channel(struct idi2axi_channel * channels, i2a_mgr_t *i2a_mgr, idi2axi_msg_attach_channel_req *ch_param);
int i2a_flush(struct device *dev, struct idi2axi_channel * channels, i2a_mgr_t *i2a_mgr, idi2axi_msg_t *msg);
int i2a_queue_buf(unsigned int i2a_id, struct idi2axi_channel * channels, i2a_mgr_t *i2a_mgr, idi2axi_msg_t *msg);
int i2a_dqueue_buf(struct i2a_dev_base_t *i2a_base_info, i2a_mgr_t *i2a_mgr, idi2axi_msg_t *msg);
int i2a_usecase_config(struct i2a_dev_base_t *i2a_base_info, i2a_mgr_t *i2a_mgr, idi2axi_msg_t *msg);
int i2a_is_all_channel_idle(struct idi2axi_channel * channels);
void i2a_clear(struct device *dev, struct idi2axi_channel * channels);
#endif /* end of include guard: I2A_MGR_H */
