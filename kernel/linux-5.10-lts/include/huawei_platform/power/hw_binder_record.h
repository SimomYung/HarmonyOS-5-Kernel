/*
* hw_binder_record.h
*
* This file use to record binder info, eg: timestamp
*
* Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
*
* This software is licensed under the terms of the GNU General Public
* License version 2, as published by the Free Software Foundation, and
* may be copied, distributed, and modified under those terms.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*/

#ifndef _HW_BINDER_RECORD_H
#define _HW_BINDER_RECORD_H

#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/time.h>
#include <linux/sched/clock.h>

extern uint8_t binder_record_switch;

#define BINDER_RECORD_MAX_NUM 100
#define BINDER_RECORD_CUR_TIMESTAMP (local_clock() / NSEC_PER_MSEC)
#define BINDER_RECORD_ENABLE (likely(binder_record_switch == 1))

struct binder_record_info {
	int32_t called_pid;
	u64 timestamp; /* timstamp of the last binder communication */
};

struct binder_record_list {
	u64 cur_timestamp; /* current timestamp, ms */
	uint32_t num;
	struct binder_record_info node[BINDER_RECORD_MAX_NUM];
};

struct binder_timestamp {
	int32_t pid;
	u64 last_timestamp; /* last binder timstamp, ms */
	u64 cur_timestamp;  /* current timestamp, ms */
};

/* for ioctl cmd */
#define BINDER_RECORD_GET_TIMESTAMP _IOR('b', 1, struct binder_timestamp)
#define BINDER_RECORD_GET_ALL _IOR('b', 2, struct binder_record_list)
#define BINDER_RECORD_REMOVE_PID _IOW('b', 3, int32_t)
#define BINDER_RECORD_REMOVE_ALL _IO('b', 4)
#define BINDER_RECORD_PRINT_ALL _IO('b', 5)
#define BINDER_RECORD_SET_MIN_TIME_DIFF _IOW('b', 6, uint32_t)
#define BINDER_RECORD_SET_SWITCH _IOW('b', 7, uint8_t)
#define BINDER_RECORD_CHANGE_NODE_ATTR _IO('b', 8)

void binder_record_add_timestamp(const int pid, const u64 timestamp);
void binder_record_remove_pid(const int pid);

#endif /* _HW_BINDER_RECORD_H */