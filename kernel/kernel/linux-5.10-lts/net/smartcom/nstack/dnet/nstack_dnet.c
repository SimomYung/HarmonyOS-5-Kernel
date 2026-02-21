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
#ifndef CONFIG_PRODUCT_PC_OHOS
#include "nstack_dnet.h"
#include <net/pkt_sched.h>
#include <securec.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/udp.h>

static int use_venc_time = 1;
module_param(use_venc_time, int, S_IRUGO);

static struct dnet_ripple_state dnet;

/* is a<=x<=b ? */
static inline bool between(u64 x, u64 a, u64 b)
{
	return b - a >= x - a;
}

static inline void dnet_set_state(u8 state)
{
	if (dnet.state != state) {
		dnet_debug("state form %hhu to %hhu\n", dnet.state, state);
		dnet.state = state;
	}
}

static inline void start_flood_timer(u32 delay)
{
	hrtimer_start(&dnet.flood_timer, ns_to_ktime(ktime_get_ns() + delay), HRTIMER_MODE_ABS);
	dnet_debug("delay %u\n", delay);
}

#ifdef CONFIG_HI_VCODEC_VENC_HIVNA
#define PREV 0
#define LATEST 1
#define FLOOD_CLOSE_NODE (LATEST + 1)

static void dnet_venc_check_fallback(u64 now)
{
	/* no recent encode info? */
	if (now > (dnet.latest_encode_stamp + (dnet.ripple.burst_interval << DNET_SHIFT_8X))) {
		dnet_info("set use_venc_time false for encode_stamp\n");
		dnet.use_venc_time = false;
		return;
	}

	/* there is other encode user? */
	if ((dnet.s_encode_intv >> DNET_SHIFT_8X) <
	    (dnet.ripple.burst_interval - (dnet.ripple.burst_interval >> DNET_SHIFT_4X))) {
		dnet.encode_chk_fail++;
		if (dnet.encode_chk_fail > ENCODE_STAMP_CHK_FAIL_MAX) {
			dnet_info("set use_venc_time false for encode_chk_fail\n");
			dnet.use_venc_time = false;
		}
	} else {
		dnet.encode_chk_fail = 0;
	}
}

static void dnet_venc_time_update(u64 now)
{
	int i;
	u64 encode_time[FLOOD_CLOSE_NODE];
	u64 intv;

	venc_get_encode_time(&encode_time[PREV], &encode_time[LATEST]);
	if (dnet.state != DNET_RIPPLE_STATE_FLOOD_STOP)
		dnet_info("now %llu PREV %llu %llu~%llu LATEST %llu %llu~%llu\n", now,
			encode_time[PREV], encode_time[PREV] + dnet.close_stamp_offset,
			encode_time[PREV] + dnet.close_stamp_offset + FLOOD_CLOSE_TIME_MAX,
			encode_time[LATEST], encode_time[LATEST] + dnet.close_stamp_offset,
			encode_time[LATEST] + dnet.close_stamp_offset + FLOOD_CLOSE_TIME_MAX);
	for (i = 0; i < FLOOD_CLOSE_NODE; i++) {
		u32 encode_to_qdisc_time;
		if (encode_time[i] <= dnet.latest_encode_stamp)
			continue;

		intv = encode_time[i] - dnet.latest_encode_stamp;
		if (intv < (dnet.ripple.burst_interval << DNET_SHIFT_2X)) {
			if (dnet.s_encode_intv)
				dnet.s_encode_intv = dnet.s_encode_intv - (dnet.s_encode_intv >> DNET_SHIFT_8X) + intv;
			else
				dnet.s_encode_intv = intv << DNET_SHIFT_8X;
		}
		dnet.latest_encode_stamp = encode_time[i];

		if (now < encode_time[i])
			continue;

		encode_to_qdisc_time = now - encode_time[i];
		if (between(encode_to_qdisc_time, ENCODE_TO_QDISC_TIME_MIN, ENCODE_TO_QDISC_TIME_MAX)) {
			dnet.encode_to_qdisc_time = dnet.encode_to_qdisc_time -
				(dnet.encode_to_qdisc_time >> DNET_SHIFT_8X) + encode_to_qdisc_time;
			dnet.close_stamp_offset = (dnet.encode_to_qdisc_time >> DNET_SHIFT_8X) -
				dnet.ripple.flood_close_time;
		}
		dnet_debug("encode_time %llu encode_intv %llu encode_to_qdisc_time %u\n",
			encode_time[i], dnet.s_encode_intv >> DNET_SHIFT_8X, encode_to_qdisc_time);
	}
	dnet_venc_check_fallback(now);
}

static void dnet_use_venc_time_init(const struct dnet_sched *q)
{
	dnet.use_venc_time = !!use_venc_time;
	if (dnet.use_venc_time && q->mark == dnet.ripple.burst_mark) {
		dnet.encode_to_qdisc_time = ENCODE_TO_QDISC_TIME_AVG << DNET_SHIFT_8X;
		dnet.close_encode_stamp = 0;
		dnet.close_stamp_offset = ENCODE_TO_QDISC_TIME_AVG - FLOOD_CLOSE_TIME_MIN;
		dnet.latest_encode_stamp = 0;
	}
}

static bool flood_need_stop(void)
{
	int i;
	u64 now = ktime_get_ns();
	u64 encode_time[FLOOD_CLOSE_NODE];

	venc_get_encode_time(&encode_time[PREV], &encode_time[LATEST]);
	for (i = 0; i < FLOOD_CLOSE_NODE; i++) {
		u64 close_start_stamp;
		u64 close_end_stamp;
		if (encode_time[i] <= dnet.close_encode_stamp)
			continue;

		close_start_stamp = encode_time[i] + dnet.close_stamp_offset;
		close_end_stamp = close_start_stamp + FLOOD_CLOSE_TIME_MAX;
		if (between(now, close_start_stamp, close_end_stamp)) {
			dnet.close_encode_stamp = encode_time[i];
			dnet.flood_cpu = smp_processor_id();
			dnet_debug("flood stop now %llu close_start %llu close_end %llu\n",
				now, close_start_stamp, close_end_stamp);
			dnet_set_state(DNET_RIPPLE_STATE_FLOOD_STOP);
			start_flood_timer(close_end_stamp - now);
			return true;
		}
	}

	return false;
}
#else /* CONFIG_HI_VCODEC_VENC_HIVNA */
static void dnet_venc_time_update(u64 now)
{
	(void)now;
}

static void dnet_use_venc_time_init(const struct dnet_sched *q)
{
	(void)q;
	dnet.use_venc_time = false;
}

static bool flood_need_stop(void)
{
	return false;
}
#endif /* CONFIG_HI_VCODEC_VENC_HIVNA */

int dnet_qdisc_config(struct Qdisc *sch, const struct dnet_sched *q)
{
	if (q->mark == dnet.ripple.flood_mark) {
		if (dnet.qdisc_attach & DNET_FLOOD_ATTACH)
			return -EALREADY;
		dnet.qdisc_attach |= DNET_FLOOD_ATTACH;
		dnet.flood_qdisc = sch;
	} else if (q->mark == dnet.ripple.burst_mark) {
		if (dnet.qdisc_attach & DNET_BURST_ATTACH)
			return -EALREADY;
		dnet.burst_start_stamp = 0;
		dnet.s_encode_intv = 0;
		dnet.qdisc_attach |= DNET_BURST_ATTACH;
	} else {
		return -EINVAL;
	}

	dnet_use_venc_time_init(q);
	return 0;
}

static int dnet_config(struct Qdisc *sch, const struct dnet_sched *q, bool qdisc_add, u32 cmd,
	const struct dnet_ripple *new_ripple)
{
	spin_lock(&dnet.lock);
	if (!dnet.ripple.burst_mark)
		dnet.ripple.burst_mark = new_ripple->burst_mark;
	if (!dnet.ripple.flood_mark)
		dnet.ripple.flood_mark = new_ripple->flood_mark;
	dnet.ripple.burst_min_interval = new_ripple->burst_min_interval;
	dnet.ripple.burst_interval = new_ripple->burst_interval;
	dnet.ripple.flood_close_time = new_ripple->flood_close_time;
	dnet.flood_reopen_time = (new_ripple->burst_interval << DNET_SHIFT_2X) +
		(new_ripple->burst_interval >> DNET_SHIFT_2X);

	if (qdisc_add) {
		int err = dnet_qdisc_config(sch, q);
		if (err) {
			spin_unlock(&dnet.lock);
			dnet_info("dnet_qdisc_config return err %d\n", err);
			return err;
		}
	}

	if (cmd == TC_DNET_SCHED_CMD_ADD && !dnet.ripple_enabled) {
		dnet_set_state(DNET_RIPPLE_STATE_UNSPEC);
		dnet.flood_open = true;
		dnet.flood_kicked = false;
		dnet.s_rate = 0;
		dnet.encode_chk_fail = 0;
		dnet.ripple_enabled = true;
	} else if (cmd == TC_DNET_SCHED_CMD_DELETE) {
		dnet.ripple_enabled = false;
	}
	spin_unlock(&dnet.lock);
	dnet_info("cmd %d burst_mark %u flood_mark %u flood_close_time %u burst_min_interval %u burst_interval %u\n",
		cmd, new_ripple->burst_mark, new_ripple->flood_mark, new_ripple->flood_close_time,
		new_ripple->burst_min_interval, new_ripple->burst_interval);
	return 0;
}

static int new_ripple_check(const struct dnet_sched *q, const struct dnet_ripple *new_ripple)
{
	if (new_ripple->burst_min_interval > new_ripple->burst_interval)
		return -EINVAL;
	if (new_ripple->burst_min_interval == 0)
		return -EINVAL;
	if (new_ripple->burst_interval == 0)
		return -EINVAL;
	if (new_ripple->burst_mark == new_ripple->flood_mark)
		return -EINVAL;
	if (new_ripple->flood_close_time > FLOOD_CLOSE_TIME_MAX)
		return -EINVAL;
	if (q->mark != new_ripple->burst_mark && q->mark != new_ripple->flood_mark)
		return -EINVAL;
	return 0;
}

static int parse_ripple(struct Qdisc *sch, const struct nlattr *ripple, u32 *cmd, struct dnet_ripple *new_ripple)
{
	struct dnet_sched *q = qdisc_priv(sch);
	const struct nla_policy policy[TCA_DNET_SCHED_RIPPLE_MAX + 1] = {
		[TCA_DNET_SCHED_RIPPLE_CMD] = {.type = NLA_U32},
		[TCA_DNET_SCHED_RIPPLE_BURST_MARK] = {.type = NLA_U32},
		[TCA_DNET_SCHED_RIPPLE_FLOOD_MARK] = {.type = NLA_U32},
		[TCA_DNET_SCHED_RIPPLE_BURST_MIN_INTERVAL] = {.type = NLA_U32},
		[TCA_DNET_SCHED_RIPPLE_BURST_INTERVAL] = {.type = NLA_U32},
		[TCA_DNET_SCHED_RIPPLE_FLOOD_CLOSE_TIME] = {.type = NLA_U32},
	};

	struct nlattr *tb[TCA_DNET_SCHED_RIPPLE_MAX + 1] = {NULL};
	int err = nla_parse_nested_deprecated(tb, TCA_DNET_SCHED_RIPPLE_MAX, ripple, policy, NULL);
	if (err < 0)
		return err;

	if (!tb[TCA_DNET_SCHED_RIPPLE_CMD])
		return -EINVAL;

	*cmd = (u32)nla_get_s32(tb[TCA_DNET_SCHED_RIPPLE_CMD]);
	if (*cmd == TC_DNET_SCHED_CMD_DELETE)
		return 0;
	/* TC_DNET_SCHED_CMD_DELETE is not supported */
	if (*cmd != TC_DNET_SCHED_CMD_ADD && *cmd != TC_DNET_SCHED_CMD_CHANGE)
		return -EINVAL;

	if (!tb[TCA_DNET_SCHED_RIPPLE_BURST_MARK] || !tb[TCA_DNET_SCHED_RIPPLE_FLOOD_MARK] ||
	    !tb[TCA_DNET_SCHED_RIPPLE_BURST_MIN_INTERVAL] || !tb[TCA_DNET_SCHED_RIPPLE_BURST_INTERVAL] ||
	    !tb[TCA_DNET_SCHED_RIPPLE_FLOOD_CLOSE_TIME])
		return -EINVAL;

	new_ripple->burst_mark = nla_get_u32(tb[TCA_DNET_SCHED_RIPPLE_BURST_MARK]);
	new_ripple->flood_mark = nla_get_u32(tb[TCA_DNET_SCHED_RIPPLE_FLOOD_MARK]);
	new_ripple->burst_min_interval = nla_get_u32(tb[TCA_DNET_SCHED_RIPPLE_BURST_MIN_INTERVAL]) * DNET_US_TO_NS;
	new_ripple->burst_interval = nla_get_u32(tb[TCA_DNET_SCHED_RIPPLE_BURST_INTERVAL]) * DNET_US_TO_NS;
	new_ripple->flood_close_time = nla_get_u32(tb[TCA_DNET_SCHED_RIPPLE_FLOOD_CLOSE_TIME]) * DNET_US_TO_NS;

	err = new_ripple_check(q, new_ripple);
	if (err)
		return err;

	return 0;
}

static int dnet_change(struct Qdisc *sch, struct nlattr *opt, struct netlink_ext_ack *extack)
{
	struct dnet_sched *q = qdisc_priv(sch);
	const struct nla_policy policy[TCA_DNET_SCHED_MAX + 1] = {
		[TCA_DNET_SCHED_MARK] = {.type = NLA_S32},
		[TCA_DNET_SCHED_RIPPLE] = {.type = NLA_NESTED},
	};
	struct nlattr *tb[TCA_DNET_SCHED_MAX + 1] = {NULL};
	int err;

	(void)extack;
	if (!opt)
		return -EINVAL;

	err = nla_parse_nested_deprecated(tb, TCA_DNET_SCHED_MAX, opt, policy, NULL);
	if (err < 0) {
		dnet_info("nla_parse_nested_deprecated return err %d\n", err);
		return err;
	}

	if (tb[TCA_DNET_SCHED_RIPPLE]) {
		u32 cmd;
		struct dnet_ripple new_ripple;

		err = parse_ripple(sch, tb[TCA_DNET_SCHED_RIPPLE], &cmd, &new_ripple);
		if (err) {
			dnet_info("parse_ripple err %d\n", err);
			return err;
		}
		err = dnet_config(sch, q, false, cmd, &new_ripple);
	}

	return err;
}

static int dump_ripple(struct sk_buff *skb)
{
	struct nlattr *nest;
	struct dnet_ripple *ripple = &dnet.ripple;

	spin_lock_bh(&dnet.lock);
	nest = nla_nest_start(skb, TCA_DNET_SCHED_RIPPLE);
	if (!nest) {
		spin_unlock_bh(&dnet.lock);
		return -1;
	}

	if (nla_put_u32(skb, TCA_DNET_SCHED_RIPPLE_ENABLE, dnet.ripple_enabled))
		goto error;

	if (nla_put_u32(skb, TCA_DNET_SCHED_RIPPLE_BURST_MARK, ripple->burst_mark))
		goto error;

	if (nla_put_u32(skb, TCA_DNET_SCHED_RIPPLE_FLOOD_MARK, ripple->flood_mark))
		goto error;

	if (nla_put_u32(skb, TCA_DNET_SCHED_RIPPLE_BURST_MIN_INTERVAL, ripple->burst_min_interval / DNET_US_TO_NS))
		goto error;

	if (nla_put_u32(skb, TCA_DNET_SCHED_RIPPLE_BURST_INTERVAL, ripple->burst_interval / DNET_US_TO_NS))
		goto error;

	if (nla_put_u32(skb, TCA_DNET_SCHED_RIPPLE_FLOOD_CLOSE_TIME, ripple->flood_close_time / DNET_US_TO_NS))
		goto error;

	nla_nest_end(skb, nest);
	spin_unlock_bh(&dnet.lock);
	return 0;

error:
	nla_nest_cancel(skb, nest);
	spin_unlock_bh(&dnet.lock);
	return -1;
}

static int dnet_dump(struct Qdisc *sch, struct sk_buff *skb)
{
	struct nlattr *nest;
	struct dnet_sched *q = qdisc_priv(sch);

	nest = nla_nest_start(skb, TCA_OPTIONS);
	if (!nest) {
		goto start_error;
	}

	if (nla_put_s32(skb, TCA_DNET_SCHED_MARK, q->mark)) {
		goto options_error;
	}

	if (dump_ripple(skb) < 0) {
		goto options_error;
	}

	return nla_nest_end(skb, nest);

options_error:
	nla_nest_cancel(skb, nest);
start_error:
	return -ENOSPC;
}

static void flood_queue_schedule(void)
{
	struct Qdisc *flood_root = NULL;

	rcu_read_lock();
	flood_root = rcu_dereference(dnet.flood_qdisc->dev_queue->qdisc);
	if (flood_root) {
		dnet_debug("timer wake flood\n");
		__netif_schedule(flood_root);
	} else {
		dnet_debug("flood_root not found\n");
	}
	rcu_read_unlock();
}

static enum hrtimer_restart flood_timer_kick(struct hrtimer *timer)
{
	if (!dnet.flood_qdisc)
		return HRTIMER_NORESTART;

	dnet.flood_kicked = true;
	smp_mb();

	flood_queue_schedule();
	return HRTIMER_NORESTART;
}

static void flood_worker_kick(struct work_struct *work)
{
	(void)work;
	flood_queue_schedule();
}

static int dnet_init(struct Qdisc *sch, struct nlattr *opt, struct netlink_ext_ack *extack)
{
	int err;
	struct dnet_sched *q = qdisc_priv(sch);
	const struct nla_policy policy[TCA_DNET_SCHED_MAX + 1] = {
		[TCA_DNET_SCHED_MARK] = {.type = NLA_S32},
		[TCA_DNET_SCHED_RIPPLE] = {.type = NLA_NESTED},
	};
	struct nlattr *tb[TCA_DNET_SCHED_MAX + 1] = {NULL};

	(void)extack;
	if (!opt) {
		dnet_info("opt NULL\n");
		return -EINVAL;
	}

	err = nla_parse_nested_deprecated(tb, TCA_DNET_SCHED_MAX, opt, policy, NULL);
	if (err < 0) {
		dnet_info("nla_parse_nested_deprecated return err %d\n", err);
		return err;
	}

	if (tb[TCA_DNET_SCHED_MARK]) {
		q->mark = (u32)nla_get_s32(tb[TCA_DNET_SCHED_MARK]);
		dnet_info("mark %u\n", q->mark);
	} else {
		dnet_info("mark not set\n");
		return -EINVAL;
	}

	if (tb[TCA_DNET_SCHED_RIPPLE]) {
		u32 cmd;
		struct dnet_ripple new_ripple;

		err = parse_ripple(sch, tb[TCA_DNET_SCHED_RIPPLE], &cmd, &new_ripple);
		if (err) {
			dnet_info("parse_ripple err %d\n", err);
			return err;
		}
		err = dnet_config(sch, q, true, cmd, &new_ripple);
	} else {
		dnet_info("ripple not set\n");
		return -EINVAL;
	}
	return err;
}

static void dnet_destroy(struct Qdisc *sch)
{
	struct dnet_sched *q = qdisc_priv(sch);
	struct dnet_ripple *ripple = &dnet.ripple;

	if (q->mark == ripple->flood_mark) {
		cancel_work_sync(&dnet.worker);
		hrtimer_cancel(&dnet.flood_timer);
	}

	spin_lock(&dnet.lock);
	if (q->mark == ripple->flood_mark) {
		dnet.flood_qdisc = NULL;
		ripple->flood_mark = 0;
		dnet.qdisc_attach &= ~DNET_FLOOD_ATTACH;
	} else if (q->mark == ripple->burst_mark) {
		ripple->burst_mark = 0;
		dnet.qdisc_attach &= ~DNET_BURST_ATTACH;
	}

	dnet.ripple_enabled = false;
	spin_unlock(&dnet.lock);
}

static void dnet_reset(struct Qdisc *sch)
{
	struct dnet_sched *q = qdisc_priv(sch);
	struct dnet_ripple *ripple = &dnet.ripple;

	spin_lock(&dnet.lock);
	if (q->mark == ripple->flood_mark)
		hrtimer_try_to_cancel(&dnet.flood_timer);
	spin_unlock(&dnet.lock);

	qdisc_reset_queue(sch);
}

static struct sk_buff *dnet_peek(struct Qdisc *sch)
{
	struct dnet_sched *q = qdisc_priv(sch);
	struct dnet_ripple *ripple = &dnet.ripple;

	if (dnet.ripple_enabled && (q->mark == ripple->flood_mark) && !dnet.flood_open)
		return NULL;
	return sch->q.head;
}

static inline bool is_burst_pkt(const struct sk_buff *skb)
{
	if (qdisc_pkt_len(skb) < MIN_BURST_PKT_LEN)
		return false;

	return true;
}

static bool is_skb_burst_fin(const struct sk_buff *skb)
{
	struct iphdr *iph;
	if (skb->protocol != htons(ETH_P_IP))
		return false;
	iph = ip_hdr(skb);
	if (iph->protocol != IPPROTO_UDP)
		return false;

	if (ntohs(iph->tot_len) != (sizeof(struct iphdr) + sizeof(struct udphdr)))
		return false;

	if (qdisc_pkt_len(skb) < skb->len)
		return false;
	return true;
}

static void dnet_burst_stop(void)
{
	struct Qdisc *flood_qdisc;

	hrtimer_try_to_cancel(&dnet.flood_timer);
	dnet.flood_open = true;
	dnet.flood_kicked = false;

	if (dnet.use_venc_time) {
		dnet_set_state(DNET_RIPPLE_STATE_FLOOD_SEND);
	} else {
		dnet_debug("qdisc: burst_tx_bytes %u\n", dnet.burst_tx_bytes);
		if (dnet.state != DNET_RIPPLE_STATE_BURST_SEND) {
			dnet_debug("qdisc: state %hhu\n", dnet.state);
			return;
		}

		if ((dnet.burst_tx_bytes > DNET_BURST_TX_ALIVE) && (dnet.qdisc_attach == DNET_BOTH_ATTACH))
			dnet_set_state(DNET_RIPPLE_STATE_FLOOD_SEND);
		else
			dnet_set_state(DNET_RIPPLE_STATE_UNSPEC);
	}

	flood_qdisc = dnet.flood_qdisc;
	if (!flood_qdisc) {
		dnet_debug("flood_qdisc not found\n");
		return;
	}

	if (flood_qdisc->q.qlen) {
		if (dnet.flood_cpu == smp_processor_id())
			flood_queue_schedule();
		else
			schedule_work_on(dnet.flood_cpu, &dnet.worker);
	}
}

static u64 dnet_timer_kick_intv_get(void)
{
	u64 intv;
	struct dnet_ripple *ripple = &dnet.ripple;

	/* avoid divide by zero */
	if (dnet.s_rate < (1 << DNET_SHIFT_16X))
		return FLOOD_CLOSE_TIME_MIN;

	/* the normal rate is send (s_burst >> DNET_SHIFT_8X) in every frame(avg 16700000ns),
	 * we use the half rate, every timer tick send 32000Bytes.
	 */
	intv = (u64)DNET_FLOOD_KICK_SEND * ripple->burst_interval / (dnet.s_rate >> DNET_SHIFT_16X);
	dnet_debug("qdisc: s_rate %u intv %llu\n", dnet.s_rate, intv);
	return intv;
}

static void dnet_flood_send(const struct sk_buff *skb)
{
	u64 now = ktime_get_ns();

	if (dnet.state == DNET_RIPPLE_STATE_FLOOD_SEND) {
		if (now > (dnet.burst_start_stamp + dnet.ripple.burst_interval - dnet.ripple.flood_close_time)) {
			u64 rate_limit_stamp;
			dnet_debug("flood close flood_close_time %u flood_tx_bytes %u\n",
				dnet.ripple.flood_close_time, dnet.flood_tx_bytes);
			dnet.flood_open = false;
			dnet_set_state(DNET_RIPPLE_STATE_FLOOD_STOP);
			dnet.flood_cpu = smp_processor_id();
			rate_limit_stamp = dnet.burst_start_stamp + dnet.ripple.burst_interval +
				dnet.ripple.burst_min_interval;
			if (rate_limit_stamp > now && (rate_limit_stamp - now) > dnet.ripple.burst_min_interval)
				start_flood_timer(rate_limit_stamp - now);
			else
				start_flood_timer(dnet.ripple.burst_min_interval);
		}
	} else if (dnet.state == DNET_RIPPLE_STATE_FLOOD_RATE_LIMITED) {
		u32 skb_len = 0;
		if (skb) {
			skb_len = qdisc_pkt_len(skb);
			dnet.flood_kick_tx_bytes += skb_len;
		}
		if (now > (dnet.burst_start_stamp + dnet.flood_reopen_time)) {
			dnet_debug("burst intv too long\n");
			dnet_set_state(DNET_RIPPLE_STATE_UNSPEC);
		} else if (dnet.flood_kick_tx_bytes > DNET_FLOOD_KICK_SEND) {
			u64 intv = dnet_timer_kick_intv_get() * dnet.flood_kick_tx_bytes / DNET_FLOOD_KICK_SEND;
			if (now < (dnet.flood_kick_stamp + intv)) {
				dnet.flood_open = false;
				dnet.flood_cpu = smp_processor_id();
				start_flood_timer(dnet.flood_kick_stamp + intv - now);
			} else {
				dnet.flood_kick_stamp = now;
				dnet.flood_kick_tx_bytes = 0;
				dnet_debug("conitune the new tick\n");
			}
		}
	}
}

static struct sk_buff *dnet_flood_dequeue(struct Qdisc *sch)
{
	struct sk_buff *skb = NULL;
	if (!dnet.use_venc_time) {
		if (dnet.flood_kicked) {
			dnet.flood_kicked = false;
			if (!dnet.flood_open && (dnet.state == DNET_RIPPLE_STATE_FLOOD_RATE_LIMITED ||
			    dnet.state == DNET_RIPPLE_STATE_FLOOD_STOP)) {
				dnet.flood_open = true;
				dnet.flood_kick_stamp = ktime_get_ns();
				dnet.flood_kick_tx_bytes = 0;
				dnet_set_state(DNET_RIPPLE_STATE_FLOOD_RATE_LIMITED);
			}
		}

		if (dnet.flood_open) {
			skb = qdisc_dequeue_head(sch);
			if (skb)
				dnet.flood_tx_bytes += qdisc_pkt_len(skb);

			dnet_flood_send(skb);
		}
	} else {
		if (dnet.flood_kicked) {
			dnet.flood_kicked = false;
			dnet.flood_open = true;
			dnet_set_state(DNET_RIPPLE_STATE_UNSPEC);
		}

		if (dnet.flood_open)
			dnet.flood_open = !flood_need_stop();

		if (dnet.flood_open)
			skb = qdisc_dequeue_head(sch);
	}

	return skb;
}

static struct sk_buff *dnet_dequeue(struct Qdisc *sch)
{
	struct dnet_sched *q = qdisc_priv(sch);
	struct dnet_ripple *ripple = &dnet.ripple;
	struct sk_buff *skb = NULL;

	spin_lock(&dnet.lock);
	if (!dnet.ripple_enabled)
		goto skip;

	if (q->mark == ripple->burst_mark) {
		skb = qdisc_dequeue_head(sch);
		if (skb) {
			if (is_skb_burst_fin(skb))
				dnet_burst_stop();
			else if (!dnet.use_venc_time)
				dnet.burst_tx_bytes += qdisc_pkt_len(skb);
		}
	} else if (q->mark == ripple->flood_mark && (dnet.qdisc_attach == DNET_BOTH_ATTACH)) {
		skb = dnet_flood_dequeue(sch);
	} else {
skip:
		skb = qdisc_dequeue_head(sch);
	}

	spin_unlock(&dnet.lock);
	return skb;
}

static void dnet_control(struct Qdisc *sch, struct dnet_ripple *ripple, u64 now)
{
	u64 diff_time;

	if (dnet.use_venc_time) {
		dnet_venc_time_update(now);
	} else {
		if (dnet.s_rate)
			dnet.s_rate = dnet.s_rate - (dnet.s_rate >> DNET_SHIFT_8X) + dnet.flood_tx_bytes;
		else
			dnet.s_rate = dnet.flood_tx_bytes << DNET_SHIFT_8X;
		dnet.flood_tx_bytes = 0;
	}

	if (dnet.burst_start_stamp)
		diff_time = now - dnet.burst_start_stamp;
	else
		diff_time = 0;
	dnet.burst_tx_bytes = 0;
	dnet_info("state %hhu diff_time %llu encode_to_qdisc_time %u flood_close_time %u\n",
		  dnet.state, diff_time, dnet.encode_to_qdisc_time >> DNET_SHIFT_8X, ripple->flood_close_time);
}

static void dnet_burst_start(struct Qdisc *sch, u64 now)
{
	struct dnet_ripple *ripple = &dnet.ripple;

	dnet_control(sch, ripple, now);
	dnet.burst_start_stamp = now;
	if (dnet.qdisc_attach != DNET_BOTH_ATTACH) {
		dnet_set_state(DNET_RIPPLE_STATE_UNSPEC);
		dnet.flood_open = true;
		return;
	}
	dnet_set_state(DNET_RIPPLE_STATE_BURST_SEND);
	dnet.flood_open = false;
	start_flood_timer(dnet.ripple.burst_min_interval);
	dnet.flood_kicked = false;
}

static int dnet_enqueue(struct sk_buff *skb, struct Qdisc *sch, struct sk_buff **to_free)
{
	struct dnet_sched *q = qdisc_priv(sch);
	struct dnet_ripple *ripple = &dnet.ripple;

	spin_lock(&dnet.lock);
	if (!dnet.ripple_enabled)
		goto skip;
	if ((dnet.state != DNET_RIPPLE_STATE_BURST_SEND) &&
	    (q->mark == ripple->burst_mark) && is_burst_pkt(skb)) {
		u64 now = ktime_get_ns();
		if (now - dnet.burst_start_stamp > ripple->burst_min_interval)
			dnet_burst_start(sch, now);
	} else if ((q->mark == ripple->flood_mark) && !dnet.flood_open) {
		u64 now = ktime_get_ns();
		if ((!dnet.use_venc_time && (now - dnet.burst_start_stamp > dnet.flood_reopen_time)) ||
		    (dnet.use_venc_time && (now - dnet.close_encode_stamp > dnet.flood_reopen_time))) {
			dnet_info("burst interval too long\n");
			dnet_set_state(DNET_RIPPLE_STATE_UNSPEC);
			dnet.flood_open = true;
		}
	}

skip:
	spin_unlock(&dnet.lock);

	if (sch->q.qlen < qdisc_dev(sch)->tx_queue_len)
		return qdisc_enqueue_tail(skb, sch);

	return qdisc_drop(skb, sch, to_free);
}

static struct Qdisc_ops dnet_qdisc_ops __read_mostly = {
	.id = "nstack_dnet",
	.priv_size = sizeof(struct dnet_sched),
	.init = dnet_init,
	.change = dnet_change,
	.reset = dnet_reset,
	.destroy = dnet_destroy,
	.peek = dnet_peek,
	.dequeue = dnet_dequeue,
	.enqueue = dnet_enqueue,
	.dump = dnet_dump,
	.owner = THIS_MODULE,
};

static int __init dnet_module_init(void)
{
	dnet_info("dnet version 2.0\n");
	(void)memset_s(&dnet, sizeof(dnet), 0, sizeof(dnet));
	spin_lock_init(&dnet.lock);

	hrtimer_init(&dnet.flood_timer, CLOCK_MONOTONIC, HRTIMER_MODE_ABS_PINNED);
	INIT_WORK(&dnet.worker, flood_worker_kick);
	dnet.flood_timer.function = flood_timer_kick;
	dnet.qdisc_attach = 0;
	dnet.state = DNET_RIPPLE_STATE_UNSPEC;
	dnet.ripple_enabled = false;
	dnet.flood_open = true;
	dnet.flood_cpu = 0;

	return register_qdisc(&dnet_qdisc_ops);
}

static void __exit dnet_module_exit(void)
{
	cancel_work_sync(&dnet.worker);
	hrtimer_cancel(&dnet.flood_timer);
	unregister_qdisc(&dnet_qdisc_ops);
}

module_init(dnet_module_init);
module_exit(dnet_module_exit);
MODULE_LICENSE("GPL");
MODULE_VERSION("2.0");
MODULE_AUTHOR("nStack linux kernel dnet developers");
MODULE_DESCRIPTION("The dnet network scheduler");
#endif
