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
 * The bandwidth estimator from tcp:
 *    send_rate = #pkts_delivered/(last_snd_time - first_snd_time)
 *    ack_rate  = #pkts_delivered/(last_ack_time - first_ack_time)
 *    bw = min(send_rate, ack_rate)
 */

#include "mintp.h"
#include "mintp_congestion.h"
#include "mintp_output.h"
#include "mintp_rate.h"

/* Snapshot the current delivery information in the skb, to generate
 * a rate sample later when the skb is (s)acked in mtp_rate_skb_delivered().
 */
void mtp_rate_skb_sent(struct sock *sk, struct sk_buff *skb)
{
	struct mtp_sock *msk = mtp_sk(sk);

	if (!msk->packets_out) {
		u64 tstamp_us = mtp_skb_timestamp_us(skb);

		msk->first_tx_mstamp  = tstamp_us;
		msk->delivered_mstamp = tstamp_us;
	}

	MTP_SKB_CB(skb)->tx.first_tx_mstamp	= msk->first_tx_mstamp;
	MTP_SKB_CB(skb)->tx.delivered_mstamp	= msk->delivered_mstamp;
	MTP_SKB_CB(skb)->tx.delivered		= msk->delivered;
	MTP_SKB_CB(skb)->tx.is_app_limited	= msk->app_limited ? 1 : 0;
}

/* When an skb is sacked or acked, we fill in the rate sample with the (prior)
 * delivery information when the skb was last transmitted.
 *
 * If an ACK (s)acks multiple skbs (e.g., stretched-acks), this function is
 * called multiple times. We favor the information from the most recently
 * sent skb, i.e., the skb with the highest prior_delivered count.
 */
void mtp_rate_skb_delivered(struct sock *sk, struct sk_buff *skb,
			    struct mtp_rate_sample *rs)
{
	struct mtp_sock *msk = mtp_sk(sk);
	struct mtp_skb_cb *scb = MTP_SKB_CB(skb);

	if (!scb->tx.delivered_mstamp)
		return;

	if (!rs->prior_delivered || after(scb->tx.delivered, rs->prior_delivered)) {
		rs->prior_delivered  = scb->tx.delivered;
		rs->prior_mstamp     = scb->tx.delivered_mstamp;
		rs->is_app_limited   = scb->tx.is_app_limited;
		rs->is_retrans	     = scb->sacked & MTPCB_RETRANS;

		/* Record send time of most recently ACKed packet: */
		msk->first_tx_mstamp  = mtp_skb_timestamp_us(skb);
		/* Find the duration of the "send phase" of this window: */
		rs->interval_us = mtp_stamp_us_delta(msk->first_tx_mstamp, scb->tx.first_tx_mstamp);
	}
	/* Mark off the skb delivered once it's sacked to avoid being
	 * used again when it's cumulatively acked. For acked packets
	 * we don't need to reset since it'll be freed soon.
	 */
	if (scb->sacked & MTPCB_SACKED_ACKED)
		scb->tx.delivered_mstamp = 0;
}

static u64 mtp_compute_delivery_rate(const struct mtp_sock *msk)
{
	u32 rate = READ_ONCE(msk->rate_delivered);
	u32 intv = READ_ONCE(msk->rate_interval_us);
	u64 rate64 = 0;

	if (rate && intv) {
		rate64 = (u64)rate * msk->mss * USEC_PER_SEC;
		do_div(rate64, intv);
	}
	return rate64;
}

static void mtp_update_msq_limit(struct sock *sk)
{
	struct mtp_sock *msk = mtp_sk(sk);
	unsigned long msq_limit_add = 0;
	u32 limit_add_extra_shift;
	u64 cur_interval = mtp_stamp_us_delta(msk->cur_mstamp, msk->last_rate_sample_mstamp);
	u32 pacing_shift = READ_ONCE(sk->sk_pacing_shift);
	int msk_msq_limit = atomic_read(&msk->msq_limit);
	int msk_msq_limit_old = msk_msq_limit;
	u64 last_send_rate = pacing_shift >= MSQ_LIMIT_EMA_SHIFT ?
		(u64)msk_msq_limit << (pacing_shift - MSQ_LIMIT_EMA_SHIFT) : msk->stats.send_rate;

	msk->last_rate_sample_mstamp = msk->cur_mstamp;
	if (msk->stats.send_rate > last_send_rate) {
		limit_add_extra_shift = MSQ_LIMIT_EMA_SHIFT;
		/* new_msq_limit = (1/8)*old_msq_limit + 7 * send_rate>>sk_pacing_shift */
		msk_msq_limit = msk_msq_limit >> MSQ_LIMIT_EMA_SHIFT;
		msq_limit_add = (msk->stats.send_rate >> (pacing_shift - MSQ_LIMIT_EMA_SHIFT)) -
			(msk->stats.send_rate >> pacing_shift);
	} else {
		limit_add_extra_shift = 1;
		if (cur_interval < MIN_RATE_SAMPLE_INTERVAL ||
			mtp_stamp_us_delta(msk->cur_mstamp, READ_ONCE(msk->rate_stat_ignore_mstamp)) <
			(msk->srtt_us >>MTP_SRTT_SHIFT)) {
			msq_limit_add = 0;
		} else {
			/* new_msq_limit = (7/8)*old_msq_limit + send_rate>>sk_pacing_shift */
			msk_msq_limit -= (msk_msq_limit >> MSQ_LIMIT_EMA_SHIFT);
			msq_limit_add = msk->stats.send_rate >> pacing_shift;
		}
	}
	/* in Previous half slow-start, add extra msq limit. */
	if ((msk->snd_cwnd < (msk->snd_ssthresh >> 1)) &&
	    (u32)(MTP_JIFFIERS32 - msk->snd_cwnd_stamp) < msk->rtx.rto)
			msq_limit_add += (msq_limit_add >> limit_add_extra_shift);

	msk_msq_limit += msq_limit_add;
	/* only update when msq_limit is unchanged during the caculation */
	(void)atomic_cmpxchg(&msk->msq_limit, msk_msq_limit_old, msk_msq_limit);

	mtp_sk_debug(msk, "delivered %u cwnd %u ssthresh %u send_rate %lu msq_limit %u pacing_rate %u\n",
		msk->delivered, msk->snd_cwnd, msk->snd_ssthresh, msk->stats.send_rate, msk_msq_limit,
		sk->sk_pacing_rate >> READ_ONCE(sk->sk_pacing_shift));
}

/* Update the connection delivery information and generate a rate sample. */
void mtp_rate_gen(struct sock *sk, u32 delivered, struct mtp_rate_sample *rs)
{
	struct mtp_sock *msk = mtp_sk(sk);
	u32 snd_us, ack_us;

	/* Clear app limited if bubble is acked and gone. */
	if (msk->app_limited && after(msk->delivered, msk->app_limited))
		msk->app_limited = 0;

	if (delivered)
		msk->delivered_mstamp = msk->cur_mstamp;

	if (!rs->prior_mstamp) {
		rs->delivered = -1;
		rs->interval_us = -1;
		return;
	}
	rs->delivered = msk->delivered - rs->prior_delivered;

	snd_us = rs->interval_us; /* send phase */
	ack_us = mtp_stamp_us_delta(msk->cur_mstamp, rs->prior_mstamp); /* ack phase */
	rs->interval_us = max(snd_us, ack_us);

	if (unlikely(rs->interval_us < mtp_min_rtt(msk))) {
		if (!rs->is_retrans)
			mtp_debug("mtp rate: %ld %d %u %u\n",
				 rs->interval_us, rs->delivered,
				 msk->ca_state, mtp_min_rtt(msk));
		rs->interval_us = -1;
		return;
	}

	/* Record the last non-app-limited or the highest app-limited bw */
	if (!rs->is_app_limited ||
	    ((u64)rs->delivered * msk->rate_interval_us >=
	     (u64)msk->rate_delivered * rs->interval_us)) {
		msk->rate_delivered = rs->delivered;
		msk->rate_interval_us = rs->interval_us;
		msk->rate_app_limited = rs->is_app_limited;
		msk->stats.send_rate = mtp_compute_delivery_rate(msk);

		mtp_update_msq_limit(sk);
	}
}

/* If a gap is detected between sends, mark the socket application-limited. */
void mtp_rate_check_app_limited(struct sock *sk)
{
	struct mtp_sock *msk = mtp_sk(sk);

	if (msk->write_seq - msk->snd_nxt < msk->mss && /* We have less than one packet to send. */
	    sk_wmem_alloc_get(sk) < SKB_TRUESIZE(1) && /* Nothing in sending host's qdisc queues or NIC tx queue. */
	    mtp_packets_in_flight(msk) < msk->snd_cwnd && /* We are not limited by CWND. */
	    msk->lost_out <= msk->retrans_out) /* All lost packets have been retransmitted. */
		msk->app_limited = (msk->delivered + mtp_packets_in_flight(msk)) ? : 1;
}
