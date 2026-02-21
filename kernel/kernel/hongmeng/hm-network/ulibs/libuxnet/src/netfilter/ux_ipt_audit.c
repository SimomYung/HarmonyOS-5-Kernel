/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Audit iptable target impl file
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 17 10:01:02 2021
 */
#include <stdbool.h>
#include <lib/dlist.h>
#include <sys/socket.h>
#include <libhmaudit/audit.h>
#include <libhmaudit/audit_log.h>
#include <hongmeng/errno.h>
#include "ux_pktbuf.h"
#include "ux_iptables.h"
#include "ux_ipt_audit.h"
#include "ux_netdev.h"
#include "ux_pbuf.h"

#define AUDIT_ADDR_BIT_NUM 3
#define AUDIT_ADDR_BIT_SIZE 8
#define byte_of_high(addr) (((addr) & 0xff000000) >> 24)

static int audit_format_addr(struct audit_log *log, uint32_t addr, bool is_saddr)
{
	int ret;
	if (is_saddr) {
		ret = audit_log_fmt(log, " saddr=%d", byte_of_high(addr));
	} else {
		ret = audit_log_fmt(log, " daddr=%d", byte_of_high(addr));
	}
	for (int i = 0; i < AUDIT_ADDR_BIT_NUM && ret == E_HM_OK; i++) {
		addr <<= AUDIT_ADDR_BIT_SIZE;
		ret = audit_log_fmt(log, "%c%d", '.', byte_of_high(addr));
	}
	return ret;
}

static int audit_log_generate(uint32_t saddr, uint32_t daddr, int proto)
{
	int ret = E_HM_OK;
	struct audit_log *log = NULL;

	log = audit_log_alloc(HM_AUDIT_NETFILTER_PKT, TYPE_AUDIT_COMMON);
	if (log == NULL) {
		ret = E_HM_NOMEM;
	}
	if (ret == E_HM_OK) {
		ret = audit_format_addr(log, saddr, true);
	}
	if (ret == E_HM_OK) {
		ret = audit_format_addr(log, daddr, false);
	}
	if (ret == E_HM_OK) {
		ret = audit_log_fmt(log, " proto=%d", proto);
	}
	if (ret == E_HM_OK) {
		ret = audit_log_write(log);
	}
	free(log);
	return ret;
}

static unsigned int audit_tg(const ux_pktbuf_t *skb,
			     const struct ux_netdev *in,
			     const struct ux_netdev *out,
			     const struct ux_ipt_target *match,
			     const void *targetinfo)
{
	ux_pbuf_header_t *pbuf_hdr = NULL;
	int ret;

	do {
		/*
		 * The ctx flag is not used here to determine whether auditing
		 * is required, because the subject here is a data packet, not
		 * a process.
		 */
		if (audit_is_disabled()) {
			break;
		}
		pbuf_hdr = (ux_pbuf_header_t *)(skb->payload);
		if (pbuf_hdr == NULL) {
			break;
		}
		ret = audit_log_generate(pbuf_hdr->iphdr.saddr, pbuf_hdr->iphdr.daddr, pbuf_hdr->iphdr.protocol);
		if (ret != E_HM_OK) {
			net_error(HMNET_CORE, "generate audit log failed, ret=%s\n", hmstrerror(ret));
		}
	} while (false);

	return UX_IPT_CONTINUE;
}

static struct ux_ipt_target audit_tg_reg = {
	.name       = "AUDIT",
	.revision   = 0,
	.family     = AF_INET,
	.target      = audit_tg,
	.targetsize  = sizeof(struct ux_ipt_audit_info),
};

int init_audit_tg(void)
{
	ux_ipt_register_target(&audit_tg_reg);

	return 0;
}
