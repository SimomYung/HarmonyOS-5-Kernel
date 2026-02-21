/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_DATAN_CT_H
#define HVGR_DATAN_CT_H

#include <linux/atomic.h>
#include <linux/wait.h>
#include <linux/timer.h>
#include <linux/rtc.h>
#include <linux/list.h>
#include <linux/poll.h>
#include <linux/vmalloc.h>
#include <linux/kref.h>

#include "hvgr_datan_ct_time.h"
#include "hvgr_ioctl_datan.h"
#include "hvgr_datan_ct_config.h"

#define CT_DUMP_MIN_INTERVAL             70000ULL /* ns */
#define CT_DUMP_MAX_INTERVAL 			 3000000000ULL /* ns */
#define RING_BUFFER_MAX_SIZE             512
#define CT_DUMP_BY_NS                    1000000000ULL /* ns */
#define GROUP_SIZE_BY_UINT               64 /* every group has 64 uint */
#define CT_OTHER_GROUP_NUMS              (4U) /* except the gpc, other module nums */
#define HEADER_SIZE_BY_UINT              4  /* beginning of every group 4 uint called header */
#define BODY_SIZE_BY_UINT                60 /* rest of every group 60 uint called body */
#define CT_ONCE_DUMP_GROUP_NUM           8  /* fcm | mmu | btc | bvhg | gpc0 | gpc1 | gpc2 | gpc3 */
#define HEADER_SIZE_BY_BYTES             16 /* 4 * 4 */
#define GROUP_SIZE_BY_BYTES              256 /* 64 * 4 */
#define BITMASK_OFFSET_IN_HEADER         8
#define CT_DUMP_GROUP_ALL                (~0u)
#define DUMP_REG_SIZE_V350			 	 3
#define BODY_SIZE_FOR_F					 12
#define BODY_SIZE_FOR_FF				 28
#define BODY_SIZE_FOR_FFF				 44
#define HEADER_OFFSET_MASK               2
#define MASK_F							 15
#define MASK_FF							 255
#define MASK_FFF						 4095
#define MASK_FFFF 						 65535
#define CT_HEADER_MASK                   2
#define GROUPS_PER_MASK_BIT              4
#define MIN_VALID_INTERVAl				 50ULL /* ns */

#define datan_unused(x)                  ((void)(x))

struct hvgr_ctx;
struct hvgr_device;
struct hvgr_mem_area;

enum {
	CT_FCM_INDEX,
	CT_MMU_INDEX,
	CT_BTC_INDEX,
	CT_BVH_INDEX,
	CT_GPC_INDEX,
	CT_MAX_COUNT
};

#define CT_ENABLED    1
#define CT_CLOSED     0

struct hvgr_ct_cfg {
	u32 bitmap[CT_MAX_COUNT];
	u32 set_id;
	u32 ring_num;
};

struct hvgr_ct_context {
	struct mutex lock;
	struct hvgr_device *gdev;
	struct hvgr_ctx *ctx;

	u64 gpu_va;
	struct hvgr_mem_area *area;
	void *cpu_va;
	void *accu_cpu_va;
	u64 last_dump_time;
	uint32_t as_id;

	size_t dump_size;
	u32 bitmap[CT_MAX_COUNT];
	u32 set_id;

	bool is_need_update_config;
	struct workqueue_struct *wq;

	u32 client_num;
	struct list_head work_clients;
	atomic_t pm_ref_cnt;
};

struct hvgr_ct_dev {
	struct hvgr_ct_context *ct_ctx;
};

struct hvgr_ct_client {
	struct hvgr_ct_context *ct_ctx;
	struct list_head list;
	size_t dump_size;
	u32 bitmap[CT_MAX_COUNT];
	u32 set_id;
	void __user *legacy_buffer;
	void *old_buffer;
	void *new_buffer;
	u64 dump_time;
	u32 dump_interval;
	wait_queue_head_t waitq;

	bool is_config_valid;
	bool qwork_allow;

	struct hrtimer client_hrtimer;
	volatile bool timer_running_flag;
	volatile bool timer_should_run;
	struct work_struct ct_worker;
	volatile bool need_waiting_wq;
	spinlock_t timer_lock;
	spinlock_t qwork_lock;
	spinlock_t flag_lock;

	struct hvgr_ctx *ctx;
	struct hvgr_ct_queue_ctrl *kmd_ctrl;
	struct hvgr_ct_queue_user_ctrl *user_ctrl;
	struct hvgr_ct_queue_data_ctrl *data_ctrl;
	struct hvgr_mem_area *kmd_ctrl_area;
	struct hvgr_mem_area *user_ctrl_area;
	struct hvgr_mem_area *data_ctrl_area;
	uint32_t ring_num;
};

struct hvgr_ct_single_dump_context {
	struct mutex ct_client_lock;
	struct hvgr_ct_client *single_dump_client;
};

void hvgr_api_ct_probe(struct hvgr_device * const gdev);
void hvgr_api_ct_term(struct hvgr_device * const gdev);

void hvgr_api_ct_collect_and_disable(struct hvgr_device *gdev, bool dump_ct);
int hvgr_api_ct_enable_and_go_on(struct hvgr_device *gdev);

long hvgr_api_ct_alloc(struct hvgr_ctx *ctx, union hvgr_ioctl_ct_alloc *const para);

int hvgr_api_dump_reg(struct hvgr_ctx *ctx, struct hvgr_ioctl_dump_reg *reg_dump);

int hvgr_api_ct_enable(struct hvgr_ctx *ctx, struct hvgr_ioctl_ct_single_dump_enable *enable);
int hvgr_api_ct_dump(struct hvgr_ctx *ctx);

int hvgr_api_ct_close_ctx(struct hvgr_ctx *ctx);
void hvgr_api_ct_open_ctx(struct hvgr_ctx *ctx);

uint32_t hvgr_api_ct_get_dump_size(void);
void hvgr_ct_detach_client(struct hvgr_ct_client *client);
u64 hvgr_ct_get_dump_timestamp(void);

void hvgr_ct_start_cycle_client_timer(struct hvgr_ct_client *client);

unsigned int hvgr_ct_data_poll(struct file *filp, poll_table *wait);
long hvgr_ct_data_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
int hvgr_ct_data_release(struct inode *inode, struct file *filp);

#endif /* HVGR_DATAN_CT_H */
