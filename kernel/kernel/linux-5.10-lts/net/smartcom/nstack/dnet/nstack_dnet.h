/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Author: songqiubin tongshaojun
 *
 * dnet network scheduler, used to reduce burst flow delay when flood flow is
 * in parallel
 */
#ifndef __NSTACK_DNET_H__
#define __NSTACK_DNET_H__

#define pr_fmt(fmt) "[nstack_dnet]%s: " fmt, __func__

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include <linux/spinlock.h>
#include <linux/hrtimer.h>
#include <linux/workqueue.h>

#ifdef DNET_DEBUG_FTRACE
#define dnet_printk1(fmt, lvl, ...) trace_printk("[nstack_dnet]"fmt, ##__VA_ARGS__)
#else
#define dnet_printk1(fmt, lvl, ...) printk(lvl pr_fmt(fmt), ##__VA_ARGS__)
#endif

static bool dnet_debug_enable __read_mostly = 0;
#define dnet_debug(fmt, ...)	\
	do {	\
		if (unlikely(dnet_debug_enable)) \
			dnet_printk1(fmt, KERN_INFO, ##__VA_ARGS__); \
	} while (0)

#define dnet_info(fmt, ...)	\
	do {	\
		dnet_printk1(fmt, KERN_INFO, ##__VA_ARGS__); \
	} while (0)

enum {
	TCA_DNET_SCHED_RIPPLE_UNSPEC,
	TCA_DNET_SCHED_RIPPLE_CMD,
	TCA_DNET_SCHED_RIPPLE_BURST_MARK,
	TCA_DNET_SCHED_RIPPLE_FLOOD_MARK,
	TCA_DNET_SCHED_RIPPLE_BURST_MIN_INTERVAL,
	TCA_DNET_SCHED_RIPPLE_BURST_INTERVAL,
	TCA_DNET_SCHED_RIPPLE_FLOOD_CLOSE_TIME,
	__TCA_DNET_SCHED_RIPPLE_MAX,
};

#define TCA_DNET_SCHED_RIPPLE_ENABLE TCA_DNET_SCHED_RIPPLE_CMD
#define TCA_DNET_SCHED_RIPPLE_MAX (__TCA_DNET_SCHED_RIPPLE_MAX - 1)

enum {
	TCA_DNET_SCHED_UNSPEC,
	TCA_DNET_SCHED_MARK,
	TCA_DNET_SCHED_RIPPLE,
	__TCA_DNET_SCHED_MAX,
};

#define TCA_DNET_SCHED_MAX (__TCA_DNET_SCHED_MAX - 1)

enum {
	TC_DNET_SCHED_CMD_UNSPEC = 0x00,
	TC_DNET_SCHED_CMD_ADD = 0x01,
	TC_DNET_SCHED_CMD_CHANGE = 0x02,
	TC_DNET_SCHED_CMD_DELETE = 0x03,
};

#define DNET_RIPPLE_STATE_UNSPEC     0
#define DNET_RIPPLE_STATE_BURST_SEND 1
#define DNET_RIPPLE_STATE_FLOOD_SEND 2
#define DNET_RIPPLE_STATE_FLOOD_STOP 3
#define DNET_RIPPLE_STATE_FLOOD_RATE_LIMITED 4

#define DNET_MAX_QDISC 2
#define DNET_RIPPLE_FLOOD_OPEN_STEP 4000
#define DNET_BURST_TX_ALIVE (10 * 1024)
#define DNET_FLOOD_KICK_SEND 32000u
#define DNET_US_TO_NS 1000

#define FLOOD_CLOSE_TIME_MIN 4000000
#define FLOOD_CLOSE_TIME_MAX 10000000
#define MIN_BURST_PKT_LEN 1000 /* bytes */

#define DNET_SHIFT_2X 1
#define DNET_SHIFT_4X 2
#define DNET_SHIFT_8X 3
#define DNET_SHIFT_16X 4
#define DNET_SHIFT_32X 5

#define ENCODE_TO_QDISC_TIME_MIN 10000000
#define ENCODE_TO_QDISC_TIME_AVG 13000000
#define ENCODE_TO_QDISC_TIME_MAX 17000000

#define ENCODE_STAMP_CHK_FAIL_MAX 30

#define DNET_BURST_ATTACH (1u << 0)
#define DNET_FLOOD_ATTACH (1u << 1)
#define DNET_BOTH_ATTACH (DNET_BURST_ATTACH | DNET_FLOOD_ATTACH)

struct dnet_ripple {
	u32 burst_mark;
	u32 flood_mark;
	u32 burst_min_interval; /* ns */
	u32 burst_interval; /* ns, burst frame interval */
	u32 flood_close_time; /* ns, FLOOD_CLOSE_TIME_MIN ~ FLOOD_CLOSE_TIME_MAX */
};

struct dnet_ripple_state {
	spinlock_t lock;
	struct dnet_ripple ripple; /* store the configs form use space */
	struct Qdisc *flood_qdisc;
	struct work_struct worker; /* used to wake up flood_qdisc on the Specified CPU */
	struct hrtimer flood_timer; /* used to wake up flood_qdisc in future */
	u64 s_encode_intv; /* smoothed encode interval, 8 times the actual rate */
	u32 burst_tx_bytes; /* the burst flow bytes send in a frame interval */
	u32 flood_tx_bytes; /* the flood flow bytes send in a frame interval */
	u32 s_rate; /* smoothed bytes per burst frame can send, 8 times the actual rate */
	u32 encode_to_qdisc_time; /* ns, ENCODE_TO_QDISC_TIME_MIN ~ ENCODE_TO_QDISC_TIME_MAX */

	u64 burst_start_stamp; /* abs, ns, the timestamp when the lastest burst frame start send */
	u32 flood_reopen_time; /* avoid flood qdisc stoped for a very long time */
	u32 flood_kick_tx_bytes; /* the flood flow bytes send in a flood_kick_stamp */
	u64 flood_kick_stamp; /* the time interval of send flood packets */
	u64 close_encode_stamp; /* the encode_stamp trigger the flood stop */
	u64 close_stamp_offset; /* the flood stop stamp minus the encode_stamp */
	u64 latest_encode_stamp; /* the latest encode_stamp */

	u32 qdisc_attach; /* the qdisc attached, DNET_BURST_ATTACH and DNET_FLOOD_ATTACH */
	int flood_cpu; /* the flood send cpu, set when flood stoped */
	u8 state; /* DNET_RIPPLE_STATE_, Initialized as DNET_RIPPLE_STATE_UNSPEC */
	bool ripple_enabled; /* ripple enabled, can changed by user at runtime.
			      * if disabled, the dnet qdisc is same as the pfifo qdisc.
			      */
	bool flood_open; /* flood send is open? Initialized as true */
	volatile bool flood_kicked; /* flood send should restart? Initialized as false */
	bool use_venc_time; /* use video encode time to predict the send of next burst frame */
	u8 encode_chk_fail; /* the times of check encode timestamp fail, if it exceed ENCODE_STAMP_CHK_FAIL_MAX,
			     * use_venc_time will change to false and fallback to control without encode timestamp.
			     */
};

struct dnet_sched {
	u32 mark; /* should be one of burst_mark and flood_mark */
};

#ifdef CONFIG_HI_VCODEC_VENC_HIVNA
extern void venc_get_encode_time(u64 *prev, u64 *cur);
#endif

#endif /* __NSTACK_DNET_H__ */
