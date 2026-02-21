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
 * MinTP socket options.
 */

#include "mintp_conn.h"
#include "mintp_tls.h"
#include "mintp_output.h"
#include "mintp_congestion.h"
#include "mintp_opt.h"

static int mtp_copy_int_from_sockptr(sockptr_t optval, unsigned int optlen, int *val)
{
	if (optlen >= sizeof(int)) {
		if (copy_from_sockptr(val, optval, sizeof(int)))
			return -EFAULT;
	} else if (optlen >= sizeof(char)) {
		unsigned char ucval;

		if (copy_from_sockptr(&ucval, optval, sizeof(ucval)))
			return -EFAULT;
		*val = (int) ucval;
	} else {
		return -EINVAL;
	}

	return 0;
}

static int mtp_setsockopt_tos(struct sock *sk, sockptr_t optval, unsigned int optlen)
{
	struct mtp_sock *msk = mtp_sk(sk);
	int ret;
	int val;

	ret = mtp_copy_int_from_sockptr(optval, optlen, &val);
	if (ret)
		return ret;

	if (msk->tos != val) {
		mtp_sk_info(msk, "set tos to %d\n", val);
		msk->tos = (__u8)val;
		sk->sk_priority = rt_tos2priority(val);
	}

	return 0;
}

static int mtp_setsockopt_keepidle(struct sock *sk, sockptr_t optval, unsigned int optlen)
{
	struct mtp_sock *msk = mtp_sk(sk);
	int ret;
	int val;

	ret = mtp_copy_int_from_sockptr(optval, optlen, &val);
	if (ret)
		return ret;

	if (val < 1 || val > MAX_MTP_KEEPIDLE) {
		mtp_sk_info(msk, "keepalive inval %d\n", val);
		return -EINVAL;
	}
	msk->keepalive_time = val * HZ;
	mtp_sk_info(msk, "set keepalive_time %u\n", msk->keepalive_time);
	if (sock_flag(sk, SOCK_KEEPOPEN) &&
	    !((1 << sk->sk_state) &
	      (TCPF_CLOSE | TCPF_LISTEN))) {
		u32 elapsed = MTP_JIFFIERS32 - msk->rcv_tstamp;
		if (msk->keepalive_time > elapsed)
			elapsed = msk->keepalive_time - elapsed;
		else
			elapsed = 0;
		sk_reset_timer(sk, &sk->sk_timer, jiffies + elapsed);
	}

	return 0;
}

static int mtp_setsockopt_max_msg_size(struct sock *sk, sockptr_t optval, unsigned int optlen)
{
	struct mtp_sock *msk = mtp_sk(sk);
	int ret;
	int val;

	ret = mtp_copy_int_from_sockptr(optval, optlen, &val);
	if (ret)
		return ret;

	if (val < 1) {
		mtp_sk_err(msk, "max_msg_size inval %d\n", val);
		return -EINVAL;
	}
	if (sk->sk_state == TCP_CLOSE) {
		if (sk->sk_sndbuf < (val << (MTP_MSG_SHIFT + 1))) {
			sk->sk_sndbuf = val << (MTP_MSG_SHIFT + 1);
			sk->sk_rcvbuf = sk->sk_sndbuf << 1;
			mtp_sk_info(msk, "set sk_sndbuf %u sk_rcvbuf %u\n",
				sk->sk_sndbuf, sk->sk_rcvbuf);
		}
		msk->max_msg_size = (u32)val << MTP_MSG_SHIFT;
		mtp_sk_info(msk, "set max_msg_size %u\n", msk->max_msg_size);
	} else {
		mtp_sk_err(msk, "max_msg_size on state %u\n", sk->sk_state);
		return -EPERM;
	}
	return 0;
}

static int mtp_setsockopt_d2d(struct sock *sk, sockptr_t optval, unsigned int optlen)
{
	struct mtp_sock *msk = mtp_sk(sk);
	int ret;
	int val;

	ret = mtp_copy_int_from_sockptr(optval, optlen, &val);
	if (ret)
		return ret;

	if (msk->src_port) {
		if (val && !msk->d2d) {
			ret = mtp_d2d_enable(msk);
			if (!ret)
				msk->d2d = true;
		} else if (!val && msk->d2d) {
			mtp_d2d_disable(msk);
			msk->d2d = false;
		}
	} else {
		ret = -ENODEV;
	}
	mtp_sk_info(msk, "dev_enable ret %d\n", ret);
	return ret;
}

static int mtp_setsockopt_cpu_id(struct sock *sk, sockptr_t optval, unsigned int optlen)
{
	struct mtp_sock *msk = mtp_sk(sk);
	int ret;
	int val;
	int cpu;
	int old_cpu;

	ret = mtp_copy_int_from_sockptr(optval, optlen, &val);
	if (ret)
		return ret;

	if ((unsigned int)val >= nr_cpu_ids || !cpu_online(val)) {
		mtp_sk_err(msk, "cpu_id %d not online\n", val);
		return -EINVAL;
	}

	cpu = val;
	old_cpu = msk->cpu_id;
	if (old_cpu == -1 && cpu != -1) {
		msk->cpu_id = cpu;
		mtp_sk_info(msk, "cpu id change from %d to %d\n", old_cpu, cpu);
		return 0;
	}
	mtp_sk_info(msk, "already bind cpu\n");
	return -EALREADY;
}

static int mtp_setsockopt_qdisc(struct sock *sk, sockptr_t optval, unsigned int optlen)
{
	struct mtp_sock *msk = mtp_sk(sk);
	int ret;
	int val;

	ret = mtp_copy_int_from_sockptr(optval, optlen, &val);
	if (ret)
		return ret;

	mtp_sk_info(msk, "enable MTP_QDISC %d\n", val);
	msk->xmit = (val ? dev_queue_xmit : mtp_direct_xmit);
	return 0;
}

static int mtp_setsockopt_cong_config(struct sock *sk, sockptr_t optval, unsigned int optlen)
{
	struct mtp_sock *msk = mtp_sk(sk);
	struct mtp_cong_config cfg;

	if (optlen < sizeof(msk->cong_cfg))
		return -EINVAL;

	if (sk->sk_state != TCP_CLOSE && sk->sk_state != TCP_LISTEN)
		return -ENOPROTOOPT;

	if (copy_from_sockptr(&cfg, optval, sizeof(cfg)))
		return -EFAULT;

	if (cfg.init_cwnd > (MTP_SND_CWND_CLAMP >> 1))
		return -EINVAL;
	if (cfg.cong_avoid_ratio > MTP_CONG_AVOID_RATIO_MAX)
		return -EINVAL;
	msk->cong_cfg = cfg;
	mtp_sk_info(msk, "congestion init_cwnd %u cong_avoid_ratio %u\n", msk->cong_cfg.init_cwnd,
		msk->cong_cfg.cong_avoid_ratio);
	return 0;
}

static int mtp_setsockopt_sendpage_msg_size(struct sock *sk, sockptr_t optval, unsigned int optlen)
{
	struct mtp_sock *msk = mtp_sk(sk);
	int ret;
	int val;

	if (sk->sk_state != TCP_ESTABLISHED)
		return -ENOTCONN;

	ret = mtp_copy_int_from_sockptr(optval, optlen, &val);
	if (ret)
		return ret;

	if ((val < (int)TLS_MAX_PAYLOAD_SIZE) || (val > (msk->max_msg_size >> 1)))
		return -EINVAL;

	mtp_sk_info(msk, "set sendpage_msg_size to %d\n", val);
	msk->sendpage_msg_size = val;
	return 0;
}

typedef int (*mtp_setsockopt_hander)(struct sock *sk, sockptr_t optval, unsigned int optlen);

static mtp_setsockopt_hander setsockopt_handle[] = {
	mtp_setsockopt_tos, /* MTP_TOS */
	mtp_setsockopt_keepidle, /* MTP_KEEPIDLE */
	mtp_setsockopt_max_msg_size, /* MTP_MAX_MSG_SIZE */
	mtp_setsockopt_d2d, /* MTP_D2D */
	NULL, /* MTP_STATS */
	mtp_setsockopt_cpu_id, /* MTP_CPU_ID */
	mtp_setsockopt_qdisc, /* MTP_QDISC */
	mtp_setsockopt_cong_config, /* MTP_CONG_CONFIG */
	mtp_setsockopt_sendpage_msg_size, /* MTP_SENDPAGE_MSG_SIZE */
};

int mtp_setsockopt(struct sock *sk, int level, int optname, sockptr_t optval, unsigned int optlen)
{
	int err = 0;
	struct mtp_sock *msk = (struct mtp_sock *)sk;
	mtp_setsockopt_hander handle;

	if (unlikely(level == SOL_TLS))
		return mtp_setsockopt_tls(sk, level, optname, optval, optlen);
	if (level != SOL_MTP)
		return -ENOPROTOOPT;

	if (optname <= 0 || optname > ARRAY_SIZE(setsockopt_handle)) {
		mtp_sk_info(msk, "optname %d not supported\n", optname);
		return -ENOPROTOOPT;
	}

	lock_sock(sk);
	handle = setsockopt_handle[optname - 1];
	if (handle)
		err = handle(sk, optval, optlen);
	else
		err = -ENOPROTOOPT;
	release_sock(sk);
	mtp_sk_debug(msk, "setsockopt %d ret %d\n", optname, err);
	return err;
}

static int mtp_copy_int_to_user(char __user *optval, int __user *optlen, int val, int len)
{
	if (len < sizeof(int) && len > 0 && val >= 0 && val <= 255) {
		unsigned char ucval = (unsigned char)val;
		len = 1;
		if (put_user(len, optlen))
			return -EFAULT;
		if (copy_to_user(optval, &ucval, 1))
			return -EFAULT;
	} else {
		len = min_t(unsigned int, sizeof(int), len);
		if (put_user(len, optlen))
			return -EFAULT;
		if (copy_to_user(optval, &val, len))
			return -EFAULT;
	}
	return 0;
}

static int mtp_getsockopt_tos(struct sock *sk, char __user *optval, int __user *optlen)
{
	int val, len;
	struct mtp_sock *msk = (struct mtp_sock *)sk;

	if (get_user(len, optlen))
		return -EFAULT;
	len = min_t(int, len, sizeof(int));
	if (len <= 0)
		return -EINVAL;

	val = msk->tos;
	return mtp_copy_int_to_user(optval, optlen, val, len);
}

static int mtp_getsockopt_keepidle(struct sock *sk, char __user *optval, int __user *optlen)
{
	int val, len;
	struct mtp_sock *msk = (struct mtp_sock *)sk;

	if (get_user(len, optlen))
		return -EFAULT;
	if (len < (int)sizeof(int))
		return -EINVAL;
	len = sizeof(int);
	val = msk->keepalive_time / HZ;
	return mtp_copy_int_to_user(optval, optlen, val, len);
}

static int mtp_getsockopt_max_msg_size(struct sock *sk, char __user *optval, int __user *optlen)
{
	int val, len;
	struct mtp_sock *msk = (struct mtp_sock *)sk;

	if (get_user(len, optlen))
		return -EFAULT;
	if (len < (int)sizeof(int))
		return -EINVAL;
	len = sizeof(int);
	val = msk->max_msg_size >> MTP_MSG_SHIFT;
	return mtp_copy_int_to_user(optval, optlen, val, len);
}

static int mtp_getsockopt_d2d(struct sock *sk, char __user *optval, int __user *optlen)
{
	int val, len;
	struct mtp_sock *msk = (struct mtp_sock *)sk;

	if (get_user(len, optlen))
		return -EFAULT;
	len = min_t(int, len, sizeof(int));
	if (len <= 0)
		return -EINVAL;

	val = msk->d2d;
	return mtp_copy_int_to_user(optval, optlen, val, len);
}

static int mtp_getsockopt_stats(struct sock *sk, char __user *optval, int __user *optlen)
{
	struct mtp_sock *msk = (struct mtp_sock *)sk;
	struct mtp_sock_stat stats = msk->stats;
	int len;

	if (get_user(len, optlen))
		return -EFAULT;
	if (len <= 0)
		return -EINVAL;
	len = min_t(int, sizeof(stats), len);
	if (put_user(len, optlen))
		return -EFAULT;

	if (len < sizeof(msk->stats))
		mtp_sk_err(msk, "len %d should be %zd\n", len, sizeof(msk->stats));
	stats.cwnd = msk->snd_cwnd;
	stats.srtt = msk->srtt_us >> 3; /* divided by 8 */
	stats.limit_output_bytes = msk->msq_limit;
	if (copy_to_user(optval, &stats, len))
		return -EFAULT;
	return 0;
}

static int mtp_getsockopt_cpu_id(struct sock *sk, char __user *optval, int __user *optlen)
{
	int val, len;
	struct mtp_sock *msk = (struct mtp_sock *)sk;

	if (get_user(len, optlen))
		return -EFAULT;
	len = min_t(int, len, sizeof(int));
	if (len <= 0)
		return -EINVAL;

	val = msk->cpu_id;
	return mtp_copy_int_to_user(optval, optlen, val, len);
}

static int mtp_getsockopt_qdisc(struct sock *sk, char __user *optval, int __user *optlen)
{
	int val, len;
	struct mtp_sock *msk = (struct mtp_sock *)sk;

	if (get_user(len, optlen))
		return -EFAULT;
	len = min_t(int, len, sizeof(int));
	if (len <= 0)
		return -EINVAL;

	val = (msk->xmit == dev_queue_xmit);
	return mtp_copy_int_to_user(optval, optlen, val, len);
}

static int mtp_getsockopt_cong_config(struct sock *sk, char __user *optval, int __user *optlen)
{
	struct mtp_sock *msk = (struct mtp_sock *)sk;
	struct mtp_cong_config cfg = msk->cong_cfg;
	int len;

	if (get_user(len, optlen))
		return -EFAULT;
	if (len <= 0)
		return -EINVAL;
	len = min_t(int, sizeof(cfg), len);
	if (put_user(len, optlen))
		return -EFAULT;

	if (len < sizeof(cfg))
		mtp_sk_err(msk, "len %zd should be %zd\n", len, sizeof(cfg));
	if (copy_to_user(optval, &cfg, len))
		return -EFAULT;
	return 0;
}

static int mtp_getsockopt_sendpage_msg_size(struct sock *sk, char __user *optval, int __user *optlen)
{
	int val, len;
	struct mtp_sock *msk = (struct mtp_sock *)sk;

	if (get_user(len, optlen))
		return -EFAULT;
	len = min_t(int, len, sizeof(int));
	if (len <= 0)
		return -EINVAL;

	val = (int)msk->sendpage_msg_size;
	return mtp_copy_int_to_user(optval, optlen, val, len);
}

typedef int (*mtp_getsockopt_hander)(struct sock *sk, char __user *optval, int __user *optlen);

static mtp_getsockopt_hander getsockopt_handle[] = {
	mtp_getsockopt_tos, /* MTP_TOS */
	mtp_getsockopt_keepidle, /* MTP_KEEPIDLE */
	mtp_getsockopt_max_msg_size, /* MTP_MAX_MSG_SIZE */
	mtp_getsockopt_d2d, /* MTP_D2D */
	mtp_getsockopt_stats, /* MTP_STATS */
	mtp_getsockopt_cpu_id, /* MTP_CPU_ID */
	mtp_getsockopt_qdisc, /* MTP_QDISC */
	mtp_getsockopt_cong_config, /* MTP_CONG_CONFIG */
	mtp_getsockopt_sendpage_msg_size, /* MTP_SENDPAGE_MSG_SIZE */
};

int mtp_getsockopt(struct sock *sk, int level, int optname, char __user *optval,
			int __user *optlen)
{
	int err;
	mtp_getsockopt_hander handle;

	if (unlikely(level == SOL_TLS))
		return mtp_getsockopt_tls(sk, optname, optval, optlen);

	if (level != SOL_MTP)
		return -ENOPROTOOPT;

	if (optname <= 0 || optname > ARRAY_SIZE(getsockopt_handle)) {
		mtp_sk_info(mtp_sk(sk), "optname %d not supported\n", optname);
		return -ENOPROTOOPT;
	}

	lock_sock(sk);
	handle = getsockopt_handle[optname - 1];
	if (handle)
		err = handle(sk, optval, optlen);
	else
		err = -ENOPROTOOPT;
	release_sock(sk);
	return err;
}
