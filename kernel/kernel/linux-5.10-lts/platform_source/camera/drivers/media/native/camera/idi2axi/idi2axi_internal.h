/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi internal header,
 *              for common driver and platform drivers
 *
 * This file is released under the GPLv2.
 */
#ifndef IDI2AXI_INTERNAL_H
#define IDI2AXI_INTERNAL_H
#include <linux/completion.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/pm_wakeup.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <media/v4l2-subdev.h>
#include "idi2axi_intf.h"


struct idi2axi_device;

enum i2a_buf_flag_e {
	I2A_BUF_FL_VALID = 0,
	I2A_BUF_FL_FLUSHED = 1,
};

struct buf_node {
	struct list_head entry;
	unsigned int fd;
	unsigned int frame_num;
	ktime_t eof_ktime;
	uint64_t buf_addr;    // iova, for idi2axi is PA/IPA
	unsigned int flag;      // buf status valid flush
	unsigned int port;
	uint64_t base_addr;
};

struct i2a_dev_base_t {
	void __iomem *i2a_base;
	uint32_t      timeout;
	unsigned int id;
	struct device *dev;
};

typedef void (*channel_eof_cb)(struct i2a_dev_base_t *, struct buf_node *, void *);

struct idi2axi_channel {
	const char *requestor;
	struct i2a_dev_base_t *i2a_base_info;
	struct completion flush_done;
	unsigned int vc; // 0~3 corresponding to channel registers
	unsigned int dt;
	unsigned int buf_offset;

	unsigned int flags;
#define I2A_CH_FL_ATTACHED  0x01
#define I2A_CH_FL_STREAMING 0x02
#define I2A_CH_FL_POWERED   0x04
#define I2A_CH_FL_TIMEOUT   0x08

	spinlock_t lock; // lock buf_list, and cb ptr
	channel_eof_cb eof_cb;
	void *cb_data;
	struct list_head buf_list;
	struct list_head done_list;
};

#define i2a_is_streaming(ch) ((ch)->flags & I2A_CH_FL_STREAMING)
#define i2a_is_attached(ch)  ((ch)->flags & I2A_CH_FL_ATTACHED)
#define i2a_is_powered(ch)  ((ch)->flags & I2A_CH_FL_POWERED)
#define i2a_is_timeout(ch)  ((ch)->flags & I2A_CH_FL_TIMEOUT)

struct _tag_i2a_intf;
struct idi2axi_device {
	struct mutex mtx; // for members in struct
	struct device *dev;
	struct v4l2_subdev subdev;
	struct _tag_i2a_intf *intf;
	struct mutex lock; // for v4l2_subdev-->video_device-->lock
};

struct _tag_i2a_vtbl;
typedef struct _tag_i2a_intf {
	struct _tag_i2a_vtbl *vtbl;
} i2a_intf_t;


typedef struct _tag_i2a_vtbl {
	char const *(*get_name)(i2a_intf_t *i);
	int (*config)(i2a_intf_t *i, void *cfg);
	int (*on_msg)(i2a_intf_t *i, void *msg);
	int (*ack_msg)(i2a_intf_t *i, void *msg);
	int (*open)(i2a_intf_t *i);
	int (*close)(i2a_intf_t *i);
	int (*get_err_info)(i2a_intf_t *i, void *msg);
} i2a_vtbl_t;

int32_t idi2axi_register(struct platform_device *pdev, i2a_intf_t *intf);
void idi2axi_notify_event(struct device *dev);

#endif /* end of include guard: IDI2AXI_INTERNAL_H */
