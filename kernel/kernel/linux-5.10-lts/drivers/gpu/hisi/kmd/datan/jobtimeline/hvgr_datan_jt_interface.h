/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2022-2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_DATAN_JT_INTERFACE_H
#define HVGR_DATAN_JT_INTERFACE_H

#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/atomic.h>

#include "hvgr_ioctl_datan.h"
struct hvgr_ctx;
struct hvgr_device;

#define HVGR_DATAN_TIMLINE_ENABLE           1U
#define HVGR_DATAN_TIMLINE_DISABLE          0U
#define JOB_TIMELINE_MAX_BUF_LEN            4096U
#define HVGR_SVD_TOKEN_START                0x1000U
#define HVGR_SVD_TOKEN_END                  0x2000U
#define HVGR_INVALID_CHAIN_ID               0xFFFFFFFFU
#define hvgr_datan_jt_get_chain_id(value)   ((value) >> 4)
#define hvgr_datan_jt_get_sub_q(value)      (((value) >> 1) & 0x7)

struct hvgr_jt_ring_buffer_control {
	uint16_t read_idx;
	uint16_t write_idx;
	bool is_full;
	struct hvgr_datan_timeline_entry timeline_entry[JOB_TIMELINE_MAX_BUF_LEN];
	uint64_t system_cnt_base;
	uint64_t system_time_base;
};

/*
 * @brief when the data write into the ting buffer, we need to update the write idx.
 */
void hvgr_datan_jt_update_ring_buffer_write_idx(struct hvgr_jt_ring_buffer_control *ring_buf_control);

/*
 * @brief Get an available entry for read or write.
 */
struct hvgr_datan_timeline_entry *hvgr_datan_jt_get_available_entry(
	struct hvgr_jt_ring_buffer_control *ring_buf_control);

/*
 * @brief cal the soc time stamp. only v200 has body.
 */
void hvgr_datan_jt_calc_soc_freq(struct hvgr_device *gdev);

/*
 * @brief when the tools call ioctrl and config the ring buffer control.
 */
int hvgr_datan_jt_config_status(struct hvgr_ctx *ctx, union kmd_ioctl_timeline_set *timeline_set);

/*
 * @brief when the hvgr node probe, this func will be called.
 */
void hvgr_datan_jt_probe(struct hvgr_device *gdev);

/*
 * @brief copy some ring buffers to user space.
 */
long hvgr_datan_jt_copy_entries_to_user(struct hvgr_ctx *ctx, union kmd_ioctl_timeline_get *timeline_get);

/*
 * @brief all ths job timeline ioctl come in here.
 */
long hvgr_datan_jt_ioctl(struct hvgr_ctx * const ctx, unsigned int cmd, unsigned long arg);

/*
 * @brief inner function.
 */
struct hvgr_mem_area *hvgr_datan_jt_mem_allocate(struct hvgr_ctx *ctx,
	uint64_t property, uint64_t attribute, uint64_t bytes);

/*
 * @brief When the fd has closed, we need to free the memmory.
 */
long hvgr_datan_jt_chain_buffer_term(struct hvgr_ctx * const ctx);

/*
 * @brief when the hvgr node closed, free the buffer
 */
void hvgr_datan_jt_buffers_free(struct hvgr_ctx * const ctx);

/*
 * @brief when the job timeline is enabled, call this to print the info. [debug func]
 */
void hvgr_datan_jt_debug_dump(struct hvgr_ctx *ctx, struct hvgr_datan_timeline_entry *entry);

void hvgr_datan_jt_config_entry_irq(struct hvgr_device * const gdev, bool enable);

#endif /* HVGR_DATAN_JT_INTERFACE_H END */

