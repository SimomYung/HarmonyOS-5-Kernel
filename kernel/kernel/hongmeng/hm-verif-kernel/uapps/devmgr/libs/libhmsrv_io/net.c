/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: I/O service library of net device operation
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 22 13:58:38 2019
 */
#include <unistd.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>

#include <libhmsrv_io/io_net_sring.h>
#include <libhmlog/hmlog.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>
#include <libstrict/strict.h>

#include <libsysif/devmgr/api.h>
#include <libsysif/devhost/api.h>

#include <libhmsrv_io/net.h>
#include "internal.h"

struct dh_net_channel_entry {
	struct dlist_node head;
	uint32_t dh_cnode_idx;

	struct io_uring *rx_uring;
	raw_thread_t rx_thread;
	bool rx_thread_running;

	struct hmsrv_net_config config;
};

static struct raw_mutex g_net_chan_lock = RAW_MUTEX_INITIALIZER;
static DLIST_HEAD(g_net_chan_list);

static struct dh_net_channel_entry *find_dh_channel(unsigned int cnode_idx)
{
	struct dh_net_channel_entry *dh = NULL;
	dlist_for_each_entry(dh, &g_net_chan_list, struct dh_net_channel_entry, head) {
		if (dh->dh_cnode_idx == cnode_idx) {
			return dh;
		}
	}
	return NULL;
}

static void insert_net_dh_channel(struct dh_net_channel_entry *dh)
{
	dlist_insert_tail(&g_net_chan_list, &dh->head);
}

int hmsrv_io_net_init(int ifindex, struct hmsrv_io_ctx *ctx, unsigned int flags)
{
	return hmsrv_io_net_open(ifindex, ctx, flags);
}

int hmsrv_io_net_open(int ifindex, struct hmsrv_io_ctx *ctx, unsigned int flags)
{
	struct sysarg_cnode_statrref stat;
	rref_t dh_ap_rref;
	int ret;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	IS_FILLED(&stat, sizeof(stat), "init by sysfast_cnode_stat_rref()");

	/* require rref for devhost ap */
	dh_ap_rref = hmsrv_io_require();
	if (IS_REF_ERR(dh_ap_rref)) {
		return REF_TO_ERR(dh_ap_rref);
	}
	ret = actvcapcall_drvcall_devmgr_net_open(g_devmgr_ap_rref,
						  ifindex,
						  dh_ap_rref);
	if (ret < 0) {
		(void)hm_ucap_reject(0, dh_ap_rref);
		return ret;
	}

	ret = sysfast_cnode_stat_rref(&stat, dh_ap_rref, CNODE_STATRREF_SRC_CNODE_IDX);
	if (ret < 0) {
		(void)hm_ucap_reject(0, dh_ap_rref);
		return ret;
	}

	ctx->ifindex = ifindex;
	ctx->dh_ap_rref = dh_ap_rref;
	ctx->dh_cnode_idx = stat.src_cnode_idx;
	ctx->flags = flags;

	return E_HM_OK;
}

int hmsrv_io_net_destroy(struct hmsrv_io_ctx *ctx)
{
	return hmsrv_io_net_close(ctx);
}

int hmsrv_io_net_close(struct hmsrv_io_ctx *ctx)
{
	int ret;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	if (IS_REF_ERR(ctx->dh_ap_rref) || (ctx->ifindex == -1)) {
		hm_warn("hmsrv_io: try to destroy invalid ctx: ifindex=%d\n", ctx->ifindex);
		return E_HM_OK;
	}

	ret = hm_ucap_reject(0, ctx->dh_ap_rref);
	if (ret < 0) {
		hm_warn("hmsrv_io: reject dh_ap_rref failed: %s\n", hmstrerror(ret));
	}
	ctx->ifindex = -1;
	ctx->dh_ap_rref = ERR_TO_REF(E_HM_INVAL);

	return E_HM_OK;
}

int hmsrv_io_net_status(const struct hmsrv_io_ctx *ctx, struct netdev_status *status_out)
{
	struct __actvret_drvcall_devhost_net_status pret;
	int ret;

	if (ctx == NULL || status_out == NULL) {
		return E_HM_INVAL;
	}

	mem_zero_s(pret);
	ret = actvcapcall_drvcall_devhost_net_status(ctx->dh_ap_rref, ctx->ifindex, &pret);
	if (ret < 0) {
		return ret;
	}

	mem_assign_s(*status_out, pret.status);

	return E_HM_OK;
}

int hmsrv_io_net_ioctl(const struct hmsrv_io_ctx *ctx, int request, unsigned long args, bool is_fwd)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}
	return actvxactcapcall_drvcall_devhost_net_ioctl(
						     is_fwd, /* isfwd */
						     true, /* allow refwd */
						     ctx->dh_ap_rref,
						     ctx->ifindex,
						     request, args);
}

int hmsrv_io_net_send(const struct hmsrv_io_ctx *ctx,
		      unsigned int offset, unsigned int size,
		      unsigned int payload_len, unsigned long flags)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}
	return actvcapcall_drvcall_devhost_net_send(ctx->dh_ap_rref,
			ctx->ifindex, offset, size, payload_len, flags);
}

int hmsrv_io_net_ready(void)
{
	return actvxactcapcall_drvcall_devmgr_net_hmnetd_ready(false, true, g_devmgr_ap_rref);
}

static void *handler_devhost_net_recv(void *arg)
{
	BUG_ON(arg == NULL);
	struct dh_net_channel_entry *dh = (struct dh_net_channel_entry *)arg;
	struct io_uring *uring = (struct io_uring *)dh->rx_uring;
	struct uring_net_sqe sqe;
	struct hmsrv_io_net_pkt pkt;
	char thread_name[HMCAP_THREAD_NAME_SIZE];
	int ret;

	ret = snprintf_s(thread_name, HMCAP_THREAD_NAME_SIZE,
			 HMCAP_THREAD_NAME_SIZE - 1,
			 "netdrx-%u", dh->dh_cnode_idx);
	if (ret < 0) {
		hm_error("constructed thread_name failed, ret %d\n", ret);
		return NULL;
	}
	ret = hm_thread_setname(0, thread_name, NULL);
	if (ret < 0) {
		hm_error("set thread name failed, ret %d\n", ret);
		return NULL;
	}
	while (dh->rx_thread_running) {
		ret = io_uring_wait_netsqe(uring, &sqe);
		if (ret < 0) {
			if (ret == E_HM_NOMSG) {
				break;
			}
			continue;
		}
		if (sqe.base.opcode != URING_OP_NETRX) {
			ret = E_HM_INVAL;
		} else {
			pkt.devno = sqe.net.linux_devno;
			pkt.pool_offset = sqe.net.pool_offset;
			pkt.shm_len = 0;
			pkt.rx_len = sqe.net.data_len;
			pkt.offset = sqe.net.data_offset;
			ret = dh->config.ns_input(&pkt);
		}
		(void)io_uring_complete_netsqe(uring, &sqe, ret);
	}
	return NULL;
}

static int __netstack_net_channel_setup(const struct hmsrv_io_ctx *ctx, const struct hmsrv_net_config *config)
{
	struct dh_net_channel_entry *net_chan = NULL;
	struct io_uring *uring = NULL;
	int ret;

	net_chan = malloc(sizeof(*net_chan));
	if (net_chan == NULL) {
		return E_HM_NOMEM;
	}
	mem_zero_b(net_chan, sizeof(*net_chan));
	net_chan->config = *config;
	net_chan->dh_cnode_idx = ctx->dh_cnode_idx;
	net_chan->rx_thread_running = false;
	net_chan->rx_uring = NULL;

	if (net_chan->config.rx_uring_shm_id == 0) {
		insert_net_dh_channel(net_chan);
		return E_HM_OK;
	}

	uring = io_uring_import(net_chan->config.rx_uring_shm_id, ctx->dh_cnode_idx);
	if (uring == NULL) {
		free(net_chan);
		return E_HM_RESFAULT;
	}
	net_chan->rx_thread_running = true;
	net_chan->rx_uring = uring;
	ret = raw_thread_create(&net_chan->rx_thread, NULL, handler_devhost_net_recv, net_chan);
	if (ret != E_HM_OK) {
		io_uring_destroy(uring);
		free(net_chan);
		return ret;
	}

	insert_net_dh_channel(net_chan);
	return E_HM_OK;
}

int hmsrv_io_net_prepare(const struct hmsrv_io_ctx *ctx,
			 struct hmsrv_net_config *config)
{
	struct __actvret_drvcall_devhost_net_prepare prepare_ret;
	struct dh_net_channel_entry *net_chan = NULL;
	int ret;

	if (ctx == NULL || config == NULL) {
		return E_HM_INVAL;
	}

	raw_mutex_lock(&g_net_chan_lock);
	net_chan = find_dh_channel(ctx->dh_cnode_idx);
	if (net_chan != NULL) {
		*config = net_chan->config;
		raw_mutex_unlock(&g_net_chan_lock);
		return E_HM_OK;
	}

	ret = actvxactcapcall_drvcall_devhost_net_prepare(false, /* not fwd */
							  true, /* allow refwd */
							  ctx->dh_ap_rref,
							  &prepare_ret);
	if (ret < 0) {
		raw_mutex_unlock(&g_net_chan_lock);
		return ret;
	}

	config->pool_size = prepare_ret.pool_size;
	config->unit_size = prepare_ret.unit_size;
	config->pool_shm_id = prepare_ret.shm_id;
	config->rx_uring_shm_id = prepare_ret.rx_uring_shm_id;
	ret = __netstack_net_channel_setup(ctx, config);
	raw_mutex_unlock(&g_net_chan_lock);
	return ret;
}

int hmsrv_io_netlink_send(rref_t dh_ap, const void *msg, size_t msg_size,
			  int protocol, unsigned int src_portid)
{
	if ((msg == NULL) || (msg_size == 0)) {
		return E_HM_INVAL;
	}
	return actvcapcall_drvcall_devhost_net_netlink_recv(dh_ap, protocol, src_portid,
							    (unsigned long)(uintptr_t)msg, (unsigned int)msg_size);
}
