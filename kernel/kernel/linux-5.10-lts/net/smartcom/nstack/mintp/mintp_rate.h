/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
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
 * Author: songqiubin
 *
 * MinTP layer2 Reliable Transmission Protocol.
 * Part of the code refers to the Linux tcpip stack
 */

#ifndef __MINTP_RATE_H__
#define __MINTP_RATE_H__

#include "mintp.h"

#define MIN_RATE_SAMPLE_INTERVAL 1000 /* in us */
#define MSQ_LIMIT_EMA_SHIFT 3
#define MAX_BURST_INTERVAL 500 /* in us */

struct mtp_rate_sample {
	u64 prior_mstamp; /* starting timestamp for interval */
	u32 prior_delivered; /* msk->delivered at "prior_mstamp" */
	s32 delivered; /* number of packets delivered over interval */
	long interval_us; /* time for msk->delivered to incr "delivered" */
	bool is_app_limited; /* is sample from packet with bubble in pipe? */
	bool is_retrans; /* is sample from retransmission? */
};

void mtp_rate_skb_sent(struct sock *sk, struct sk_buff *skb);
void mtp_rate_skb_delivered(struct sock *sk, struct sk_buff *skb, struct mtp_rate_sample *rs);
void mtp_rate_gen(struct sock *sk, u32 delivered, struct mtp_rate_sample *rs);
void mtp_rate_check_app_limited(struct sock *sk);

#endif /* __MINTP_RATE_H__ */
