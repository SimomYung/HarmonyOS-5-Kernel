/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Netlink generic methods
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan  3 17:10:16 2019
 */

#include <libhmsync/raw_rwlock.h>
#include <libstrict/strict.h>
#include <libhmsrv_io/net.h>

#include "hm_netlink.h"
#include "ux_netlink.h"

#include "ux_netlink_core.h"
#include "ux_netlink_proto.h"
#include "ux_netlink_generic.h"
#include "ux_netlink_table.h"

static ux_netlink_generic_t *gen_families[GENL_FAMILY_MAX] = { 0 };
static struct raw_rwlock ux_netlink_genl_lock = RAW_RDLOCK_INITIALIZER;

void ux_netlink_generic_rdlock(void)
{
	(void)raw_rwlock_rdlock(&ux_netlink_genl_lock);
}

void ux_netlink_generic_wrlock(void)
{
	(void)raw_rwlock_wrlock(&ux_netlink_genl_lock);
}

void ux_netlink_generic_unlock(void)
{
	(void)raw_rwlock_unlock(&ux_netlink_genl_lock);
}

int ux_netlink_register_generic(ux_netlink_generic_t *family)
{
	uint16_t i;
	int ret = 0;

	if ((family->id != 0U) && (family->id < NLMSG_MIN_TYPE ||
	    family->id >= NLMSG_MIN_TYPE + GENL_FAMILY_MAX)) {
		net_debug(HMNET_NETLINK, "family id is out of range\n");
		return E_HM_INVAL;
	}

	ux_netlink_generic_wrlock();
	if (family->id != 0) {
		if (gen_families[family->id - NLMSG_MIN_TYPE] != NULL) {
			net_debug(HMNET_NETLINK, "family id is already exist\n");
			ret = E_HM_POSIX_EXIST;
			goto out_unlock;
		}
		gen_families[family->id - NLMSG_MIN_TYPE] = family;
	} else {
		for (i = (GENL_START_ALLOC - NLMSG_MIN_TYPE);
		     i < ARRAY_SIZE(gen_families); i++) {
			if (gen_families[i] == NULL) {
				gen_families[i] = family;
				family->id = i + NLMSG_MIN_TYPE;
				goto out_unlock;
			}
		}
		/* none empty entry exists */
		net_debug(HMNET_NETLINK, "none empty entry for the family to be registed\n");
		ret = E_HM_NOMEM;
	}

out_unlock:
	ux_netlink_generic_unlock();

	return ret;
}

void ux_netlink_unregister_generic(const ux_netlink_generic_t *family)
{
	if (family->id < NLMSG_MIN_TYPE ||
	    family->id >= NLMSG_MIN_TYPE + GENL_FAMILY_MAX) {
		return;
	}

	gen_families[family->id - NLMSG_MIN_TYPE] = NULL;
}

static uint32_t mcgrps_end = GENL_START_ALLOC;

static int check_netlink_name(const ux_reg_fam_info_t *info)
{
	unsigned int i;
	const char *famname = info->famname;
	const ux_genl_mcgrp_t *mcgrps = (ux_genl_mcgrp_t *)((char *)info + sizeof(ux_reg_fam_info_t));
	const char *grpname = NULL;

	if (famname[0] == '\0' || famname[GENL_NAMSIZ - 1] != '\0') {
		return E_HM_INVAL;
	}

	for (i = 0; i < info->mcgrps_cnt; i++) {
		grpname = mcgrps[i].name;
		if (grpname[0] == '\0' || grpname[GENL_NAMSIZ - 1] != '\0') {
			return E_HM_INVAL;
		}
	}

	return 0;
}

ux_netlink_generic_t *devhost_register_generic(const ux_reg_fam_info_t *info, uint32_t len, uref_t dh_ap_uref)
{
	ux_netlink_generic_t *ux_genl_fm = NULL;
	uint32_t mcgrps_cnt;
	uint32_t grpnames_len;
	int ret;

	if (info == NULL || len < sizeof(ux_reg_fam_info_t)) {
		return NULL;
	}

	mcgrps_cnt = info->mcgrps_cnt;
	grpnames_len = (unsigned int)sizeof(ux_genl_mcgrp_t) * mcgrps_cnt;

	if (len < (sizeof(ux_reg_fam_info_t) + grpnames_len) || mcgrps_cnt > MCGRPS_CNT_MAX) {
		return NULL;
	}

	ret = check_netlink_name(info);
	if (ret != E_HM_OK) {
		return NULL;
	}

	ux_genl_fm = calloc(1, sizeof(ux_netlink_generic_t) + grpnames_len);
	if (ux_genl_fm == NULL) {
		return NULL;
	}

	ux_genl_fm->grpnames_len = grpnames_len;
	NOFAIL(memcpy_s(ux_genl_fm->name, GENL_NAMSIZ, info->famname, GENL_NAMSIZ));
	ux_genl_fm->dh_ap_rref = hm_ucap_accept_grant(0, dh_ap_uref, 0);
	if (IS_REF_ERR(ux_genl_fm->dh_ap_rref)) {
		net_error(HMNET_CORE, "genl accept grant failed, err=%s\n",
			 hmstrerror(REF_TO_ERR(ux_genl_fm->dh_ap_rref)));
		free(ux_genl_fm);
		return NULL;
	}
	ux_genl_fm->mcgrps_cnt = mcgrps_cnt;

	ux_netlink_generic_wrlock();
	ux_genl_fm->mcgrps_offset = mcgrps_end;
	mcgrps_end += mcgrps_cnt;
	ux_netlink_generic_unlock();

	if (ux_genl_fm->grpnames_len != 0) {
		NOFAIL(memcpy_s(ux_genl_fm->grpnames, ux_genl_fm->grpnames_len,
				info->grpnames,
				grpnames_len));
	}
	ret = ux_netlink_register_generic(ux_genl_fm);
	if (ret == E_HM_OK) {
		return ux_genl_fm;
	}

	if (hm_ucap_reject(0, ux_genl_fm->dh_ap_rref) < 0) {
		net_warn(HMNET_CORE, "hm_ucap_reject failed\n");
	}
	free(ux_genl_fm);
	return NULL;
}

int devhost_unregister_generic(uint16_t familyid)
{
	ux_netlink_generic_t *ux_genl_fm = NULL;

	ux_netlink_generic_wrlock();
	ux_genl_fm = ux_genl_family_lookup_by_id(familyid);
	if (ux_genl_fm == NULL) {
		ux_netlink_generic_unlock();
		return E_HM_INVAL;
	}

	ux_netlink_unregister_generic(ux_genl_fm);
	ux_netlink_generic_unlock();

	if (ux_genl_fm->dh_ap_rref != 0) {
		(void)hm_ucap_reject(0, ux_genl_fm->dh_ap_rref);
		free(ux_genl_fm);
	}

	return 0;
}

int netlink_genl_unicast(unsigned int dst_port, const char *msg,
			 const size_t len)
{
	return ux_netlink_unicast(NETLINK_GENERIC, dst_port, msg, len, false);
}

int netlink_genl_broadcast(unsigned int dst_group, const char *msg,
			   const size_t len)
{
	return ux_netlink_broadcast(NETLINK_GENERIC, dst_group, msg, len);
}

/* the caller should hold lock */
ux_netlink_generic_t *ux_genl_family_lookup_by_id(uint16_t id)
{
	if (id < NLMSG_MIN_TYPE || id >= NLMSG_MIN_TYPE + GENL_FAMILY_MAX) {
		return NULL;
	}

	return gen_families[id - NLMSG_MIN_TYPE];
}

/* the caller should hold lock */
static ux_netlink_generic_t *ux_genl_family_lookup_by_name(const char *name)
{
	ux_netlink_generic_t *family;
	uint16_t i;

	for (i = 0; i < ARRAY_SIZE(gen_families); i++) {
		if (gen_families[i] == NULL) {
			continue;
		}

		family = gen_families[i];
		if (strcmp(family->name, name) == 0) {
			return family;
		}
	}

	return NULL;
}

static int file_grps_info(const ux_netlink_generic_t *family, ux_pktbuf_t *pkt)
{
	struct nlattr *nla_grps = NULL;
	unsigned int i;

	if (family->mcgrps_cnt == 0) {
		return 0;
	}

	nla_grps = (struct nlattr *)pkt->tail;
	if (ux_netlink_attr_put(pkt, CTRL_ATTR_MCAST_GROUPS, NULL, 0)) {
		return -1;
	}

	for (i = 0; i < family->mcgrps_cnt; i++) {
		struct nlattr *nest = (struct nlattr *)pkt->tail;
		if (ux_netlink_attr_put(pkt, (uint16_t)(i + 1), NULL, 0)) {
			return -1;
		}
		if (ux_netlink_attr_put_u32(pkt, CTRL_ATTR_MCAST_GRP_ID,
						    family->mcgrps_offset + i)) {
			return -1;
		}

		if (ux_netlink_attr_put(pkt, CTRL_ATTR_MCAST_GRP_NAME,
					family->grpnames[i].name,
					strlen(family->grpnames[i].name) + 1)) {
			return -1;
		}
		nest->nla_len = (uint16_t)((char *)pkt->tail - (char *)nest);
	}
	nla_grps->nla_len = (uint16_t)((char *)pkt->tail - (char *)nla_grps);
	return 0;
}

static int ux_genl_ctl_fill_family(const ux_netlink_generic_t *family,
				   ux_pktbuf_t *pkt, uint32_t seq,
				   uint32_t port, uint16_t flags)
{
	struct genlmsghdr *genlh = NULL;
	struct nlmsghdr *nlh = NULL;
	int ret;

	nlh = ux_pktbuf_push(pkt, NLMSG_SPACE(sizeof(struct genlmsghdr)));
	if (nlh == NULL) {
		return E_HM_NOMEM;
	}

	nlh->nlmsg_type = GENL_ID_CTRL;
	nlh->nlmsg_len = NLMSG_SPACE(sizeof(struct genlmsghdr));
	nlh->nlmsg_flags = flags;
	nlh->nlmsg_pid = port;
	nlh->nlmsg_seq = seq;

	genlh = ux_nlmsg_data(nlh);
	genlh->cmd = CTRL_CMD_NEWFAMILY;
	genlh->version = 0x02;
	genlh->reserved = 0;

	ret = E_HM_MSGSIZE;
	if (ux_netlink_attr_put(pkt, CTRL_ATTR_FAMILY_NAME, family->name,
				GENL_NAMSIZ)) {
		goto out;
	}

	if (ux_netlink_attr_put_u16(pkt, CTRL_ATTR_FAMILY_ID, family->id)) {
		goto out;
	}

	if (file_grps_info(family, pkt) != 0) {
		goto out;
	}

	nlh->nlmsg_len = (uint32_t)((char *)pkt->tail - (char *)nlh);

	return 0;

out:
	pkt->tail = (void *)nlh;
	pkt->len = (unsigned int)((char *)pkt->tail - (char *)pkt->data);

	return ret;
}

static int ux_genl_ctl_get_family(ux_sock_t *sk, const struct nlmsghdr *nlh,
				  void *attrs, uint32_t size)
{
	struct nlattr *attr = (struct nlattr *)attrs;
	ux_netlink_sock_t *nsk = netlink_sock(sk);
	ux_netlink_generic_t *family = NULL;
	ux_pktbuf_t *pkt = NULL;
	int ret;

	if (size < sizeof(struct nlattr) || size < attr->nla_len) {
		return E_HM_INVAL;
	}

	switch (attr->nla_type) {
	case CTRL_ATTR_FAMILY_ID: {
		uint16_t id = *(uint16_t *)(attr + 1);

		family = ux_genl_family_lookup_by_id(id);
		if (family == NULL) {
			return E_HM_POSIX_NOENT;
		}

		break;
	}
	case CTRL_ATTR_FAMILY_NAME: {
		char *name = (char *)(attr + 1);

		family = ux_genl_family_lookup_by_name(name);
		if (family == NULL) {
			return E_HM_POSIX_NOENT;
		}

		break;
	}
	default:
		return E_HM_INVAL;
	}

	/* get a vaild family */
	pkt = ux_pktbuf_alloc(NETLINK_PAYLOAD_MAX);
	if (pkt == NULL) {
		return E_HM_NOMEM;
	}

	ret = ux_genl_ctl_fill_family(family, pkt, nlh->nlmsg_seq, nsk->port, 0);
	if (ret != E_HM_OK) {
		ux_pktbuf_free(pkt);
		return ret;
	}

	/* should not hold sk lock */
	ret = ux_netlink_recv_data(sk, pkt, false);
	if (ret != 0) {
		ux_pktbuf_free(pkt);
		return ret;
	}

	return 0;
}

static int ux_genl_ctl_dump_family(ux_sock_t *sk, const struct nlmsghdr *nlh)
{
	ux_netlink_sock_t *nsk = netlink_sock(sk);
	ux_netlink_generic_t *family = NULL;
	ux_pktbuf_t *pkt = NULL;
	int ret;
	int i;

	pkt = ux_pktbuf_alloc(NETLINK_PAYLOAD_MAX);
	if (pkt == NULL) {
		return E_HM_NOMEM;
	}

	for (i = 0; i < GENL_FAMILY_MAX; i++) {
		family = gen_families[i];
		if (family == NULL) {
			continue;
		}

		ret = ux_genl_ctl_fill_family(family, pkt, nlh->nlmsg_seq,
					      nsk->port, NLM_F_MULTI);
		if (ret != E_HM_OK) {
			ux_pktbuf_free(pkt);
			return ret;
		}
	}

	/* should not hold sk lock */
	ret = ux_netlink_recv_data(sk, pkt, false);
	if (ret != 0) {
		ux_pktbuf_free(pkt);
		return ret;
	}

	return 0;
}

static const ux_netlink_generic_ops_t ux_genl_ctrl_ops[] = {
	{
		.cmd = CTRL_CMD_GETFAMILY,
		.exec = ux_genl_ctl_get_family,
		.dump = ux_genl_ctl_dump_family,
	}
};

static ux_netlink_generic_t ux_genl_ctrl = {
	.name = "nlctrl",
	.ops = ux_genl_ctrl_ops,
	.id = GENL_ID_CTRL,
	.ops_cnt = ARRAY_SIZE(ux_genl_ctrl_ops),
	.mcgrps_offset = 0,
	.mcgrps_cnt = 0,
	.dh_ap_rref = 0
};

static const ux_netlink_generic_ops_t *
ux_genl_family_lookup_cmd(const ux_netlink_generic_t *family, uint8_t cmd)
{
	const ux_netlink_generic_ops_t *ops;
	uint16_t i;

	for (i = 0; i < family->ops_cnt; i++) {
		ops = &family->ops[i];
		if (ops->cmd == cmd) {
			return ops;
		}
	}

	return NULL;
}

static int devhost_genl_send(uint16_t familyid, rref_t dh_ap_rref,
			     void *nlh, uint32_t nlhlen, unsigned int src_portid)
{
	UNUSED(familyid);
	int ret;
	ret = hmsrv_io_netlink_send(dh_ap_rref, nlh, nlhlen, NETLINK_GENERIC, src_portid);

	return ret;
}

static int ux_genl_family_handle_msg_m(const ux_sock_t *sk,
				       const ux_netlink_generic_t *family,
				       struct nlmsghdr *nlh)
{
	ux_netlink_sock_t *nsk = netlink_sock(sk);
	int ret;

	ret = devhost_genl_send(family->id, family->dh_ap_rref, nlh,
				nlh->nlmsg_len, nsk->port);
	if (ret != 0) {
		return ret;
	}

	return 0;
}

static int __call_dump(ux_sock_t *sk, const struct nlmsghdr *nlh,
		       const ux_netlink_generic_ops_t *ops)
{
	if (ops->dump == NULL) {
		return E_HM_OPNOTSUPP;
	}

	/* need del err in ux_netlink_dump_done */
	(void)ops->dump(sk, nlh);
	return ux_netlink_dump_done(sk, nlh->nlmsg_seq);
}

static int ux_genl_family_handle_msg(ux_sock_t *sk,
				     const ux_netlink_generic_t *family,
				     struct nlmsghdr *nlh)
{
	uint32_t hdr_size = NLMSG_LENGTH(sizeof(struct genlmsghdr));
	const ux_netlink_generic_ops_t *ops = NULL;
	struct genlmsghdr *genlmsg = NULL;

	if (nlh->nlmsg_len < hdr_size) {
		return E_HM_INVAL;
	}

	if (family->dh_ap_rref != 0) {
		return ux_genl_family_handle_msg_m(sk, family, nlh);
	}

	genlmsg = ux_nlmsg_data(nlh);
	ops = ux_genl_family_lookup_cmd(family, genlmsg->cmd);
	if (ops == NULL) {
		return E_HM_OPNOTSUPP;
	}

	if ((nlh->nlmsg_flags & NLM_F_DUMP) != 0) {
		return __call_dump(sk, nlh, ops);
	}

	if (ops->exec == NULL) {
		return E_HM_OPNOTSUPP;
	}

	return ops->exec(sk, nlh, (void *)(genlmsg + 1),
			 nlh->nlmsg_len - hdr_size);
}

static void ux_netlink_generic_handle_msg(ux_sock_t *sk, const ux_pktbuf_t *pkt,
					  struct nlmsghdr *nlh)
{
	ux_netlink_generic_t *family = NULL;
	int ret;

	UNUSED(pkt);
	ux_netlink_generic_rdlock();
	family = ux_genl_family_lookup_by_id(nlh->nlmsg_type);
	if (family == NULL) {
		ux_netlink_generic_unlock();
		net_debug(HMNET_NETLINK, "generic handle msg err = %s\n", hmstrerror(E_HM_POSIX_NOENT));
		return;
	}

	ret = ux_genl_family_handle_msg(sk, family, nlh);
	if (family->dh_ap_rref == 0) {
		if ((ret != 0) ||
		    ((((unsigned int)nlh->nlmsg_flags & NLM_F_ACK) != 0U) &&
		     (((unsigned int)nlh->nlmsg_flags & NLM_F_DUMP) == 0U))) {
			ux_netlink_error(sk, nlh, -hmerrno2posix(ret));
		}
	}
	ux_netlink_generic_unlock();
}

static int ux_netlink_generic_rcv(ux_sock_t *sk, ux_pktbuf_t *pkt)
{
	return ux_netlink_rcv(sk, pkt, ux_netlink_generic_handle_msg);
}

static ux_netlink_proto_t netlink_generic = {
	.rcv = ux_netlink_generic_rcv,
	.protocol = NETLINK_GENERIC,
	.is_dynamic = false,
	.refcnt = __RAW_REFCNT_NR_INITIALIZER(0),
};

int ux_netlink_generic_init(void)
{
	int ret;

	ret = ux_netlink_register_generic(&ux_genl_ctrl);
	if (ret != E_HM_OK) {
		net_error(HMNET_NETLINK, "netlink register generic ctrl failed\n");
		return ret;
	}

	ret = ux_netlink_create_service(&netlink_generic);
	if (ret != E_HM_OK) {
		net_error(HMNET_NETLINK, "create netlink generic service failed\n");
		return ret;
	}

	ret = ux_netlink_proto_register(&netlink_generic);
	if (ret != E_HM_OK) {
		net_error(HMNET_NETLINK, "register netlink generic failed\n");
		return ret;
	}

	return 0;
}

void ux_netlink_generic_exit(void)
{
	ux_netlink_proto_unregister(&netlink_generic);
}
