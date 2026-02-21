/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Netlink route operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan  3 17:10:16 2019
 */
#include <netinet/if_ether.h>
#include <libstrict/strict.h>
#include <libhmsync/raw_rwlock.h>

#include "hm_ifaddrs.h"
#include "hm_netlink.h"
#include "hm_neighbour.h"

#include "ux_netdev.h"
#include "ux_netlink_core.h"
#include "ux_netlink_proto.h"
#include "ux_netlink_route.h"

#define IFA_FOREVER	0xFFFFFFFFU
typedef struct ux_netlink_route_link {
	int family;
	int type;
	int (*exec)(ux_sock_t *sk, struct nlmsghdr *nlh);
	int (*dump)(ux_sock_t *sk, const struct nlmsghdr *nlh);
} ux_rtnl_link_t;

static DLIST_HEAD(ux_link_ops_list);
static struct raw_rwlock ux_link_ops_lock = RAW_RDLOCK_INITIALIZER;

static ux_rtnl_link_ops_t *link_ops_get(const char *type)
{
	ux_rtnl_link_ops_t *ops = NULL;

	(void)raw_rwlock_wrlock(&ux_link_ops_lock);
	if (dlist_empty(&ux_link_ops_list)) {
		(void)raw_rwlock_unlock(&ux_link_ops_lock);
		return NULL;
	}

	dlist_for_each_entry(ops, &ux_link_ops_list, ux_rtnl_link_ops_t, node) {
		if (strcmp(ops->type, type) == 0) {
			(void)raw_rwlock_unlock(&ux_link_ops_lock);
			return ops;
		}
	}
	(void)raw_rwlock_unlock(&ux_link_ops_lock);
	return NULL;
}

void ux_rtnl_link_ops_register(ux_rtnl_link_ops_t *ops)
{
	(void)raw_rwlock_wrlock(&ux_link_ops_lock);
	dlist_insert(&ux_link_ops_list, &ops->node);
	(void)raw_rwlock_unlock(&ux_link_ops_lock);
}

void ux_rtnl_link_ops_unregister(ux_rtnl_link_ops_t *ops)
{
	(void)raw_rwlock_wrlock(&ux_link_ops_lock);
	dlist_delete(&ops->node);
	(void)raw_rwlock_unlock(&ux_link_ops_lock);
}

typedef struct ux_rtnl_dump_args {
	ux_pktbuf_t *pkt;
	uint32_t seq;
	uint32_t port;
} ux_rtnl_dump_args_t;

static ux_netlink_ops_t netlink_ops;
void ux_netlink_register_ops(const ux_netlink_ops_t *ops)
{
	if (ops != NULL) {
		NOFAIL(memcpy_s(&netlink_ops, sizeof(netlink_ops),
				ops, sizeof(netlink_ops)));
	}
}

static int link_dump_item(ux_pktbuf_t *pkt, const ux_netdev_t *dev, struct nlmsghdr *rnlh)
{
	unsigned char dev_addr[ETH_ALEN] = {0, 0, 0, 0, 0, 0};
	unsigned char brd_addr[ETH_ALEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	int dev_mtu = 0;
	int mindex;
	int ret;

	ret = E_HM_MSGSIZE;
	mindex = ux_netdev_get_master(dev);
	if (mindex > 0) {
		if (ux_netlink_attr_put_u32(pkt, IFLA_LINK, (uint32_t)mindex)) {
			return ret;
		}
	}

	(void)ux_netdev_get_mac(dev->name, dev_addr, ETH_ALEN);
	if (ux_netlink_attr_put(pkt, IFLA_ADDRESS, dev_addr, ETH_ALEN)) {
		return ret;
	}
	if (ux_netlink_attr_put(pkt, IFLA_BROADCAST, brd_addr, ETH_ALEN)) {
		return ret;
	}
	if (ux_netlink_attr_put(pkt, IFLA_IFNAME, dev->name, IFNAMSIZ)) {
		return ret;
	}

	(void)ux_netdev_get_mtu(dev->name, &dev_mtu);
	if (ux_netlink_attr_put_u32(pkt, IFLA_MTU, (uint32_t)dev_mtu)) {
		return ret;
	}
	rnlh->nlmsg_len = (uint32_t)((char *)pkt->tail - (char *)rnlh);
	return 0;
}

static int ux_rtnl_link_dump_item(const ux_netdev_t *dev, void *data)
{
	ux_rtnl_dump_args_t *args = (ux_rtnl_dump_args_t*)data;
	ux_pktbuf_t *pkt = args->pkt;
	struct ifinfomsg *ifm = NULL;
	struct nlmsghdr *rnlh = NULL;
	short ifi_flags = 0;
	int ret;

	rnlh = ux_pktbuf_push(pkt, NLMSG_SPACE(sizeof(struct ifinfomsg)));
	if (rnlh == NULL) {
		return E_HM_NOMEM;
	}

	rnlh->nlmsg_type = RTM_NEWLINK;
	rnlh->nlmsg_len = NLMSG_SPACE(sizeof(struct ifinfomsg));
	rnlh->nlmsg_flags = 0;
	rnlh->nlmsg_pid = args->port;
	rnlh->nlmsg_seq = args->seq;

	ifm = ux_nlmsg_data(rnlh);
	ifm->ifi_family = AF_INET;
	ifm->__ifi_pad = 0;
	ifm->ifi_type = dev->type;
	ifm->ifi_index = dev->index;
	(void)ux_netdev_get_flags(dev->name, &ifi_flags);
	ifm->ifi_flags = (unsigned)(unsigned short)ifi_flags;
	ifm->ifi_change = 0;

	ret = link_dump_item(pkt, dev, rnlh);
	if (ret != 0) {
		pkt->tail = (void *)rnlh;
		pkt->len = (unsigned int)((char *)pkt->tail - (char *)pkt->data);
	}
	return ret;
}

static int ux_rtnl_link_get(ux_sock_t *sk, struct nlmsghdr *nlh)
{
	UNUSED(sk, nlh);
	return 0;
}

static void attrlist_get(struct rtattr *attr, int rta_len, uint32_t max,
			struct rtattr *attrlist[])
{
	uint32_t attrlist_len = (unsigned int)(max + 1) * (unsigned int)sizeof(struct rtattr *);
	struct rtattr *rta = attr;
	NOFAIL(memset_s(attrlist, attrlist_len, 0, attrlist_len));
	while (RTA_OK(rta, rta_len)) {
		if (rta->rta_type <= max && rta->rta_type != 0) {
			attrlist[rta->rta_type] = rta;
		}
		rta = RTA_NEXT(rta, rta_len);
	}
	if (rta_len != 0) {
		net_debug(HMNET_NETLINK, "deficit=%d, rta_len=%hu\n", rta_len, rta->rta_len);
	}
	return;
}

static void ux_netlink_attrlist_get(const struct nlmsghdr *nlh,
				   size_t hdrlen, uint32_t max, struct rtattr *attrlist[])
{
	unsigned char *data = NLMSG_DATA(nlh);
	struct rtattr *rta = (struct rtattr *)(uintptr_t)(data + NLMSG_ALIGN(hdrlen));
	int rta_len = (int)NLMSG_PAYLOAD(nlh, hdrlen);

	attrlist_get(rta, rta_len, max, attrlist);
	return;
}

static ux_rtnl_link_ops_t *get_ops_by_lkattr(struct rtattr *lkattr)
{
	struct rtattr *lklist[IFLA_INFO_MAX + 1];
	char type[MAX_TYPE_LEN + 1] = {0};

	if (lkattr == NULL) {
		return NULL;
	}
	attrlist_get(RTA_DATA(lkattr), (int)RTA_PAYLOAD(lkattr), IFLA_INFO_MAX, lklist);
	if (lklist[IFLA_INFO_KIND] != NULL) {
		if (ux_netlink_attr_get((uintptr_t)type, MAX_TYPE_LEN, lklist[IFLA_INFO_KIND]) != 0) {
			return NULL;
		}
	}

	return link_ops_get(type);
}

static void set_master(ux_netdev_t *dev, const struct rtattr *master_attr)
{
	ux_netdev_t *master_dev = NULL;
	uint32_t mindex = 0;

	if (ux_netlink_attr_get_u32(&mindex, master_attr) != 0) {
		return;
	}

	master_dev = ux_netdev_get_by_index((int)mindex);
	if (master_dev == NULL) {
		return;
	}
	if (master_dev->ops && master_dev->ops->ndo_add_slave) {
		master_dev->ops->ndo_add_slave(master_dev, dev);
	}
	ux_netdev_put(master_dev);
}

static int ux_rtnl_link_add(ux_sock_t *sk, struct nlmsghdr *nlh)
{
	struct ifinfomsg *ifm = NLMSG_DATA(nlh);
	struct rtattr *attrlist[IFLA_MAX + 1];
	ux_netdev_t *dev = NULL;
	ux_rtnl_link_ops_t *ops = NULL;
	char ifname[IFNAMSIZ];
	int ret = 0;

	ux_netlink_attrlist_get(nlh, sizeof(*ifm), IFLA_MAX, attrlist);

	if (ifm->ifi_index > 0) {
		dev = ux_netdev_get_by_index(ifm->ifi_index);
	} else if (attrlist[IFLA_IFNAME] != NULL) {
		NOFAIL(memset_s(ifname, IFNAMSIZ, 0, IFNAMSIZ));
		ret = ux_netlink_attr_get((uintptr_t)ifname, IFNAMSIZ, attrlist[IFLA_IFNAME]);
		if (ret != E_HM_OK) {
			return ret;
		}
		dev = ux_netdev_get_by_name(ifname);
	}

	if (dev == NULL) {
		ops = get_ops_by_lkattr(attrlist[IFLA_LINKINFO]);
		if (ops != NULL && ops->addlink != NULL) {
			ux_rtnl_lock();
			ops->addlink(ifname);
			ux_rtnl_unlock();
		} else {
			ux_netlink_error(sk, nlh, -EOPNOTSUPP);
			return E_HM_OPNOTSUPP;
		}
	} else {
		if ((nlh->nlmsg_flags & NLM_F_EXCL) != 0U) {
			ux_netdev_put(dev);
			ux_netlink_error(sk, nlh, -EEXIST);
			return E_HM_POSIX_EXIST;
		}
		if (attrlist[IFLA_MASTER] != NULL) {
			ux_rtnl_lock();
			set_master(dev, attrlist[IFLA_MASTER]);
			ux_rtnl_unlock();
		}
		ux_netdev_put(dev);
	}

	ux_netlink_error(sk, nlh, 0);

	return 0;
}

static int ux_rtnl_get_addr_from_attrlist(struct sockaddr_in *sockaddr, struct rtattr **attrlist)
{
	int ret = E_HM_OK;
	uint32_t addr = 0;

	if (attrlist[IFA_ADDRESS] == NULL) {
		attrlist[IFA_ADDRESS] = attrlist[IFA_LOCAL];
	}

	if (attrlist[IFA_ADDRESS] != NULL) {
		ret = ux_netlink_attr_get_u32(&addr, attrlist[IFA_ADDRESS]);
		if (ret != E_HM_OK) {
			return ret;
		}
	}

	sockaddr->sin_addr.s_addr = addr;

	return ret;
}

static int ux_rtnl_addr_add(ux_sock_t *sk, struct nlmsghdr *nlh)
{
	struct ifaddrmsg *ifa = NLMSG_DATA(nlh);
	struct rtattr *attrlist[IFA_MAX + 1];
	ux_netdev_t *dev = NULL;
	int ret = E_HM_OK;
	struct sockaddr_in sockaddr = {0};

	ux_netlink_attrlist_get(nlh, sizeof(*ifa), IFA_MAX, attrlist);

	if (ifa->ifa_prefixlen > PREFIX_MAX_LEN ||
	    attrlist[IFA_LOCAL] == NULL ||
	    ifa->ifa_index > INT_MAX) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		dev = ux_netdev_get_by_index((int)ifa->ifa_index);
		if (dev == NULL) {
			ret = E_HM_NODEV;
		}
	}

	if (ret == E_HM_OK) {
		ret = ux_rtnl_get_addr_from_attrlist(&sockaddr, attrlist);
		if (ret != E_HM_OK) {
			ret = E_HM_POSIX_FAULT;
		}
	}

	if (ret == E_HM_OK) {
		if (dev->priv_ops != NULL && dev->priv_ops->set_ipaddr != NULL) {
			ux_rtnl_lock();
			ret = dev->priv_ops->set_ipaddr(dev, (struct sockaddr *)&sockaddr, dev->name,
							prelen_to_mask(ifa->ifa_prefixlen), true);
			ux_rtnl_unlock();
		} else {
			ret = E_HM_OPNOTSUPP;
		}
	}

	if (dev != NULL) {
		ux_netdev_put(dev);
	}

	ux_netlink_error(sk, nlh, -hmerrno2posix(ret));

	return 0;
}

static int ux_rtnl_addr_del(ux_sock_t *sk, struct nlmsghdr *nlh)
{
	struct ifaddrmsg *ifa = NLMSG_DATA(nlh);
	struct rtattr *attrlist[IFA_MAX + 1];
	ux_netdev_t *dev = NULL;
	struct sockaddr_in sockaddr = {0};
	int ret = E_HM_OK;

	ux_netlink_attrlist_get(nlh, sizeof(*ifa), IFA_MAX, attrlist);

	if (ifa->ifa_prefixlen > PREFIX_MAX_LEN ||
	    attrlist[IFA_LOCAL] == NULL ||
	    ifa->ifa_index > INT_MAX) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		dev = ux_netdev_get_by_index((int)ifa->ifa_index);
		if (dev == NULL) {
			ret = E_HM_NODEV;
		}
	}

	if (ret == E_HM_OK) {
		ret = ux_rtnl_get_addr_from_attrlist(&sockaddr, attrlist);
		if (ret != E_HM_OK) {
			ret = E_HM_POSIX_FAULT;
		}
	}

	if (ret == E_HM_OK) {
		if (dev->priv_ops != NULL && dev->priv_ops->del_ipaddr != NULL) {
			ux_rtnl_lock();
			ret = dev->priv_ops->del_ipaddr(dev, (struct sockaddr *)&sockaddr, prelen_to_mask(ifa->ifa_prefixlen));
			ux_rtnl_unlock();
		} else {
			ret = E_HM_OPNOTSUPP;
		}
	}

	if (dev != NULL) {
		ux_netdev_put(dev);
	}

	ux_netlink_error(sk, nlh, -hmerrno2posix(ret));

	return 0;
}

static int ux_rtnl_link_del(ux_sock_t *sk, struct nlmsghdr *nlh)
{
	struct ifinfomsg *ifm = NLMSG_DATA(nlh);
	struct rtattr *attrlist[IFLA_MAX + 1];
	ux_netdev_t *dev = NULL;

	ux_netlink_attrlist_get(nlh, sizeof(*ifm), IFLA_MAX, attrlist);

	if (ifm->ifi_index > 0) {
		dev = ux_netdev_get_by_index(ifm->ifi_index);
	} else if (attrlist[IFLA_IFNAME] != NULL) {
		if ((RTA_PAYLOAD(attrlist[IFLA_IFNAME]) == 0UL) ||
		    RTA_PAYLOAD(attrlist[IFLA_IFNAME]) > IFNAMSIZ) {
			return E_HM_INVAL;
		}
		char ifname[IFNAMSIZ];
		NOFAIL(memset_s(ifname, IFNAMSIZ, 0, IFNAMSIZ));
		NOFAIL(memcpy_s((void *)ifname, IFNAMSIZ,
				RTA_DATA(attrlist[IFLA_IFNAME]),
				(size_t)(unsigned int)RTA_PAYLOAD(attrlist[IFLA_IFNAME])));
		dev = ux_netdev_get_by_name(ifname);
	} else {
		return E_HM_INVAL;
	}

	if (dev == NULL) {
		return E_HM_NODEV;
	}

	if (dev->ops != NULL && dev->ops->ndo_dellink != NULL) {
		ux_rtnl_lock();
		dev->ops->ndo_dellink(dev);
		ux_rtnl_unlock();
	} else {
		ux_netdev_put(dev);
		return E_HM_OPNOTSUPP;
	}

	ux_netdev_put(dev);
	ux_netlink_error(sk, nlh, 0);

	return 0;
}

static int ux_rtnl_link_dump(ux_sock_t *sk, const struct nlmsghdr *nlh)
{
	ux_netlink_sock_t *nsk = netlink_sock(sk);
	ux_rtnl_dump_args_t args;
	ux_pktbuf_t *pkt = NULL;
	int ret;

	pkt = ux_pktbuf_alloc(NETLINK_PAYLOAD_MAX);
	if (pkt == NULL) {
		return E_HM_NOMEM;
	}

	args.pkt = pkt;
	args.seq = nlh->nlmsg_seq;
	args.port = nsk->port;
	ux_rtnl_lock();
	ret = ux_netdev_for_each_device(ux_rtnl_link_dump_item, &args);
	ux_rtnl_unlock();
	if (ret != 0) {
		net_info(HMNET_NETLINK, "ux_rtnl_link_dump_item in ux_rtnl_link_dump failed\n");
	}

	/* should not hold sk lock */
	ret = ux_netlink_recv_data(sk, pkt, false);
	if (ret != 0) {
		ux_pktbuf_free(pkt);
		return ret;
	}
	return ux_netlink_dump_done(sk, args.seq);
}

static int ux_rtnl_route_get(ux_sock_t *sk, struct nlmsghdr *nlh)
{
	UNUSED(sk, nlh);
	return 0;
}

static int route_dump_item(ux_pktbuf_t *pkt, const rt_entry_t *rt, struct nlmsghdr *rnlh)
{
	int ret;

	ret = E_HM_MSGSIZE;
	if (rt->dst != 0) {
		if (ux_netlink_attr_put_u32(pkt, RTA_DST, rt->dst)) {
			return ret;
		}
	}

	int oifindex = 0;
	if (ux_netdev_get_index_by_name(rt->oifname, &oifindex) != 0) {
		return ret;
	}
	if (ux_netlink_attr_put_u32(pkt, RTA_OIF, (uint32_t)oifindex)) {
		return ret;
	}

	if (rt->gw != 0) {
		if (ux_netlink_attr_put_u32(pkt, RTA_GATEWAY, rt->gw)) {
			return ret;
		}
	}

	if (rt->metric != 0) {
		if (ux_netlink_attr_put_u32(pkt, RTA_PRIORITY, rt->metric)) {
			return ret;
		}
	}

	if (rt->presrc != 0 && rt->dst != 0) {
		if (ux_netlink_attr_put_u32(pkt, RTA_PREFSRC, rt->presrc)) {
			return ret;
		}
	}

	rnlh->nlmsg_len = (uint32_t)((char *)pkt->tail - (char *)rnlh);
	return 0;
}

static int ux_rtnl_route_dump_item(const rt_entry_t *rt, ux_rtnl_dump_args_t *args)
{
	ux_pktbuf_t *pkt = args->pkt;
	struct rtmsg *r = NULL;
	struct nlmsghdr *rnlh = NULL;
	int ret;

	if (rt->dst == 0 && rt->gw == 0) {
		return 0;
	}
	rnlh = ux_pktbuf_push(pkt, NLMSG_SPACE(sizeof(struct rtmsg)));
	if (rnlh == NULL) {
		return E_HM_NOMEM;
	}

	rnlh->nlmsg_type = RTM_NEWROUTE;
	rnlh->nlmsg_len = NLMSG_SPACE(sizeof(struct rtmsg));
	rnlh->nlmsg_flags = 0;
	rnlh->nlmsg_pid = args->port;
	rnlh->nlmsg_seq = args->seq;

	r = ux_nlmsg_data(rnlh);
	r->rtm_family = AF_INET;
	r->rtm_dst_len = rt->dst_len; /* 0: default; ipv4 32 */
	r->rtm_src_len = 0;
	r->rtm_tos = 0;
	/* Routing table id, RT_TABLE_XXX in hm_netlink.h */
	r->rtm_table = RT_TABLE_MAIN;
	r->rtm_protocol = 0;  /* Routing protocol */
	r->rtm_scope = 0; /* RT_SCOPE_XXX in hm_netlink.h */
	if (rt->type == RTN_UNSPEC) {
		r->rtm_type = RTN_UNICAST; /* RTN_XXX in hm_netlink.h */
	}
	r->rtm_flags = 0; /* RTNH_F_XXX in hm_netlink.h */

	ret = route_dump_item(pkt, rt, rnlh);
	if (ret != 0) {
		pkt->tail = (void *)rnlh;
		pkt->len = (unsigned int)((char *)pkt->tail - (char *)pkt->data);
	}

	return ret;
}

static int get_route_buf(void **buf, size_t *len)
{
	if (netlink_ops.get_route_info == NULL) {
		return -1;
	}
	if (netlink_ops.get_route_info(buf, len) != 0) {
		return -1;
	}
	if (*len < sizeof(rt_entry_t)) {
		if (netlink_ops.release_route_info != NULL) {
			netlink_ops.release_route_info();
		} else if (*buf != NULL) {
			free(*buf);
			*buf = NULL;
		}
		return -1;
	}
	return 0;
}

static int route_dump(ux_sock_t *sk, const struct nlmsghdr *nlh,
		      ux_rtnl_dump_args_t *args, const size_t *plen, void *buf)
{
	ux_pktbuf_t *pkt = NULL;
	rt_entry_t *rt = NULL;
	size_t len = *plen;
	size_t rdlen = 0;
	int ret;

	while (len > rdlen) {
		if ((len - rdlen) < sizeof(rt_entry_t)) {
			break;
		}
		rt = (rt_entry_t *)(uintptr_t)((char *)buf + rdlen);
		while (ux_rtnl_route_dump_item(rt, args) != 0) {
			ret = ux_netlink_recv_data(sk, pkt, false);
			if (ret != 0) {
				ux_pktbuf_free(args->pkt);
				return ret;
			}
			pkt = ux_pktbuf_alloc(NETLINK_PAYLOAD_MAX);
			if (pkt == NULL) {
				return E_HM_NOMEM;
			}
			args->pkt = pkt;
			args->seq = nlh->nlmsg_seq;
		}
		rdlen += sizeof(rt_entry_t);
	}
	return 0;
}

static int ux_rtnl_route_dump(ux_sock_t *sk, const struct nlmsghdr *nlh)
{
	ux_rtnl_dump_args_t args;
	ux_pktbuf_t *pkt = NULL;
	ux_netlink_sock_t *nsk = netlink_sock(sk);
	void *buf = NULL;
	size_t len = 0;
	int ret;

	pkt = ux_pktbuf_alloc(NETLINK_PAYLOAD_MAX);
	if (pkt == NULL) {
		return E_HM_NOMEM;
	}

	args.pkt = pkt;
	args.seq = nlh->nlmsg_seq;
	args.port = nsk->port;

	ux_rtnl_lock();
	ret = get_route_buf(&buf, &len);
	ux_rtnl_unlock();
	if (ret != 0) {
		ux_pktbuf_free(pkt);
		return ux_netlink_dump_done(sk, args.seq);
	}

	ret = route_dump(sk, nlh, &args, &len, buf);
	if (netlink_ops.release_route_info != NULL) {
		netlink_ops.release_route_info();
	} else if (buf != NULL) {
		free(buf);
	}
	if (ret != 0) {
		return ret;
	}
	/* should not hold sk lock */
	ret = ux_netlink_recv_data(sk, pkt, false);
	if (ret != 0) {
		ux_pktbuf_free(pkt);
		return ret;
	}
	return ux_netlink_dump_done(sk, args.seq);
}

static int ux_rtnl_neigh_exec(ux_sock_t *sk, struct nlmsghdr *nlh)
{
	uint32_t dst = 0;
	ux_netdev_t *dev = NULL;
	struct ndmsg *ndm = NLMSG_DATA(nlh);
	struct rtattr *attrlist[NDA_MAX + 1];
	int ret = 0;

	UNUSED(sk);
	if (ndm->ndm_ifindex != 0) {
		dev = ux_netdev_get_by_index(ndm->ndm_ifindex);
		if (dev == NULL) {
			ret = E_HM_INVAL;
			goto out;
		}
	}

	ux_netlink_attrlist_get(nlh, sizeof(*ndm), NDA_MAX, attrlist);
	if (attrlist[NDA_DST] == NULL) {
		ret = E_HM_INVAL;
		goto out;
	}

	ret = ux_netlink_attr_get_u32(&dst, attrlist[NDA_DST]);
	if (ret != E_HM_OK) {
		goto out;
	}
	if (netlink_ops.del_neigh_info != NULL) {
		ux_rtnl_lock();
		netlink_ops.del_neigh_info(dst, (dev ? dev->name : NULL),
					   ndm->ndm_state);
		ux_rtnl_unlock();
	} else {
		ret = E_HM_OPNOTSUPP;
		goto out;
	}

out:
	if (dev != NULL) {
		ux_netdev_put(dev);
	}
	return ret;
}

static int neigh_dump_item(ux_pktbuf_t *pkt, const nd_entry_t *nd,
			   struct nlmsghdr *rnlh)
{
	int ret;

	ret = E_HM_MSGSIZE;
	if (ux_netlink_attr_put_u32(pkt, NDA_DST, nd->dst)) {
		return ret;
	}
	if (ux_netlink_attr_put(pkt, NDA_LLADDR, nd->lladdr, ETH_ALEN)) {
		return ret;
	}
	rnlh->nlmsg_len = (uint32_t)((char *)pkt->tail - (char *)rnlh);
	return 0;
}

static int ux_rtnl_neigh_dump_item(const nd_entry_t *nd, void *data)
{
	ux_rtnl_dump_args_t *args = (ux_rtnl_dump_args_t*)data;
	ux_pktbuf_t *pkt = args->pkt;
	struct ndmsg *r = NULL;
	struct nlmsghdr *rnlh = NULL;
	int ret;

	rnlh = ux_pktbuf_push(pkt, NLMSG_SPACE(sizeof(struct ndmsg)));
	if (rnlh == NULL) {
		return E_HM_NOMEM;
	}

	rnlh->nlmsg_type = RTM_NEWNEIGH;
	rnlh->nlmsg_len = NLMSG_SPACE(sizeof(struct ndmsg));
	rnlh->nlmsg_flags = 0;
	rnlh->nlmsg_pid = args->port;
	rnlh->nlmsg_seq = args->seq;

	r = ux_nlmsg_data(rnlh);
	r->ndm_family = AF_INET;
	(void)ux_netdev_get_index_by_name(nd->ifname, &r->ndm_ifindex);

	r->ndm_pad1 = 0;
	r->ndm_pad2 = 0;
	r->ndm_state = nd->state; /* NUD_xxx in hm_neighbour.h */
	r->ndm_type = 0;
	r->ndm_flags = nd->flags; /* NTF_xxx in hm_neighbour.h */

	ret = neigh_dump_item(pkt, nd, rnlh);
	if (ret != 0) {
		pkt->tail = (void *)rnlh;
		pkt->len = (unsigned int)((char *)pkt->tail - (char *)pkt->data);
	}

	return ret;
}

static int get_neigh_buf(void **buf, size_t *len)
{
	if (netlink_ops.get_neigh_info == NULL) {
		return -1;
	}
	if (netlink_ops.get_neigh_info(buf, len) != 0) {
		return -1;
	}
	if (*len < sizeof(nd_entry_t)) {
		if (netlink_ops.release_neigh_info != NULL) {
			netlink_ops.release_neigh_info();
		} else if (*buf != NULL) {
			free(*buf);
			*buf = NULL;
		}
		return -1;
	}
	return 0;
}

static int neigh_dump(ux_sock_t *sk, const struct nlmsghdr *nlh,
		      ux_rtnl_dump_args_t *args, const size_t *plen,
		      void *buf)
{
	ux_pktbuf_t *pkt = NULL;
	nd_entry_t *nd = NULL;
	size_t len = *plen;
	size_t rdlen = 0;
	int ret = 0;

	while (len > rdlen) {
		if ((len - rdlen) < sizeof(nd_entry_t)) {
			break;
		}
		nd = (nd_entry_t *)(uintptr_t)((char *)buf + rdlen);
		while (ux_rtnl_neigh_dump_item(nd, args) != 0) {
			ret = ux_netlink_recv_data(sk, args->pkt, false);
			if (ret != 0) {
				ux_pktbuf_free(args->pkt);
				return ret;
			}
			pkt = ux_pktbuf_alloc(NETLINK_PAYLOAD_MAX);
			if (pkt == NULL) {
				return E_HM_NOMEM;
			}
			args->pkt = pkt;
			args->seq = nlh->nlmsg_seq;
		}
		rdlen += sizeof(nd_entry_t);
	}
	return ret;
}

static int ux_rtnl_neigh_dump(ux_sock_t *sk, const struct nlmsghdr *nlh)
{
	ux_netlink_sock_t *nsk = netlink_sock(sk);
	ux_rtnl_dump_args_t args;
	ux_pktbuf_t *pkt = NULL;
	void *buf = NULL;
	size_t len = 0;
	int ret;

	pkt = ux_pktbuf_alloc(NETLINK_PAYLOAD_MAX);
	if (pkt == NULL) {
		return E_HM_NOMEM;
	}

	args.pkt = pkt;
	args.seq = nlh->nlmsg_seq;
	args.port = nsk->port;

	ux_rtnl_lock();
	ret = get_neigh_buf(&buf, &len);
	ux_rtnl_unlock();
	if (ret != 0) {
		ux_pktbuf_free(pkt);
		return ux_netlink_dump_done(sk, args.seq);
	}

	ret = neigh_dump(sk, nlh, &args, &len, buf);
	if (netlink_ops.release_neigh_info != NULL) {
		netlink_ops.release_neigh_info();
	} else if (buf != NULL) {
		free(buf);
	}

	if (ret != 0) {
		return ret;
	}
	/* should not hold sk lock */
	ret = ux_netlink_recv_data(sk, pkt, false);
	if (ret != 0) {
		ux_pktbuf_free(pkt);
		return ret;
	}
	return ux_netlink_dump_done(sk, args.seq);
}

static int ux_rtnl_addr_get(ux_sock_t *sk, struct nlmsghdr *nlh)
{
	UNUSED(sk, nlh);

	return 0;
}

static int addr_dump_item(ux_pktbuf_t *pkt, struct ifaddrmsg *ifa,
	const struct ifaddrs_storage_internal *ifp, struct nlmsghdr *rnlh)
{
	uint32_t ip, mask, broad;
	struct ifa_cacheinfo ci;
	int ret = E_HM_MSGSIZE;

	ip = ((struct sockaddr_in *)&(ifp->ifa_addr))->sin_addr.s_addr;
	if (ip != 0) {
		if (ux_netlink_attr_put_u32(pkt, IFA_ADDRESS, ip)) {
			return ret;
		}
	}

	mask = ((struct sockaddr_in *)&(ifp->ifa_netmask))->sin_addr.s_addr;
	if (mask != 0) {
		ifa->ifa_prefixlen = mask_to_prelen(mask);
		if (ux_netlink_attr_put_u32(pkt, IFA_MULTICAST, mask)) {
			return ret;
		}
	}

	/* IFA_BROADCAST should not be put if ip addr is not configured */
	broad = (ip & mask) | (~mask);
	if (ip != 0 && broad != 0 && strcmp(ifp->name, "lo") != 0) {
		if (ux_netlink_attr_put_u32(pkt, IFA_BROADCAST, broad)) {
			return ret;
		}
	}

	NOFAIL(memset_s(&ci, sizeof(ci), 0, sizeof(struct ifa_cacheinfo)));
	ci.ifa_valid = IFA_FOREVER;
	ci.ifa_prefered = IFA_FOREVER;
	if (ux_netlink_attr_put(pkt, IFA_CACHEINFO, &ci, sizeof(ci))) {
		return ret;
	}

	if (ux_netlink_attr_put(pkt, IFA_LABEL, ifp->name, IFNAMSIZ)) {
		return ret;
	}

	rnlh->nlmsg_len = (uint32_t)((char *)pkt->tail - (char *)rnlh);
	return 0;
}

static int __ux_rtnl_addr_dump_item(const ux_netdev_t *dev,
		const struct ifaddrs_storage_internal *ifp, void *data)
{
	ux_rtnl_dump_args_t *args = (ux_rtnl_dump_args_t*)data;
	ux_pktbuf_t *pkt = args->pkt;
	struct ifaddrmsg *ifa = NULL;
	struct nlmsghdr *rnlh = NULL;
	int ret;

	rnlh = ux_pktbuf_push(pkt, NLMSG_SPACE(sizeof(struct ifaddrmsg)));
	if (rnlh == NULL) {
		return E_HM_NOMEM;
	}

	rnlh->nlmsg_type = RTM_NEWADDR;
	rnlh->nlmsg_len = NLMSG_SPACE(sizeof(struct ifaddrmsg));
	rnlh->nlmsg_flags = 0;
	rnlh->nlmsg_pid = args->port;
	rnlh->nlmsg_seq = args->seq;

	ifa = ux_nlmsg_data(rnlh);
	ifa->ifa_family = AF_INET;
	ifa->ifa_flags = 0;
	if (strcmp(ifp->name, "lo") == 0) {
		ifa->ifa_scope = RT_SCOPE_HOST;
	} else {
		ifa->ifa_scope = RT_SCOPE_UNIVERSE;
	}
	ifa->ifa_index = (unsigned)dev->index;

	ret = addr_dump_item(pkt, ifa, ifp, rnlh);
	if (ret != 0) {
		pkt->tail = (void *)rnlh;
		pkt->len = (unsigned int)((char *)pkt->tail - (char *)pkt->data);
	}
	return ret;
}

static int ux_rtnl_getaddrs(ux_netdev_t *dev, char *buf, int size)
{
	int cnt = 0;

	if (dev->priv_ops != NULL &&
	    dev->priv_ops->getifaddrs != NULL) {
		(void)dev->priv_ops->getifaddrs(dev, buf, size, &cnt);
	}

	return cnt;
}

static int ux_rtnl_addr_dump_item(const ux_netdev_t *dev, void *data)
{
	int i;
	int ifcnt;
	char *buf = NULL;
	size_t size = UX_NETDEV_MAXADDR * sizeof (struct ifaddrs_storage_internal);
	struct ifaddrs_storage_internal *ifp = NULL;
	int ret = 0;

	buf = malloc(size);
	if (buf == NULL) {
		return E_HM_NOMEM;
	}

	ifcnt = ux_rtnl_getaddrs((ux_netdev_t *)dev, buf, (int)size);
	ifcnt = ifcnt > UX_NETDEV_MAXADDR ? UX_NETDEV_MAXADDR : ifcnt;
	ifp = (struct ifaddrs_storage_internal *)(void *)buf;

	for (i = 0; i < ifcnt; i++, ifp++) {
		ret = __ux_rtnl_addr_dump_item(dev, ifp, data);
		if (ret != 0) {
			break;
		}
	}

	free(buf);
	return ret;
}

static int ux_rtnl_addr_dump(ux_sock_t *sk, const struct nlmsghdr *nlh)
{
	ux_netlink_sock_t *nsk = netlink_sock(sk);
	ux_rtnl_dump_args_t args;
	ux_pktbuf_t *pkt = NULL;
	int ret;

	pkt = ux_pktbuf_alloc(NETLINK_PAYLOAD_MAX);
	if (pkt == NULL) {
		return E_HM_NOMEM;
	}

	args.pkt = pkt;
	args.seq = nlh->nlmsg_seq;
	args.port = nsk->port;
	ux_rtnl_lock();
	ret = ux_netdev_for_each_device(ux_rtnl_addr_dump_item, &args);
	ux_rtnl_unlock();
	if (ret != 0) {
		net_info(HMNET_NETLINK, "ux_rtnl_addr_dump_item in ux_rtnl_addr_dump failed\n");
	}

	/* should not hold sk lock */
	ret = ux_netlink_recv_data(sk, pkt, false);
	if (ret != 0) {
		ux_pktbuf_free(pkt);
		return ret;
	}

	return ux_netlink_dump_done(sk, args.seq);
}

static ux_rtnl_link_t ux_rtnl_link_ops[] = {
	{
		.family = PF_UNSPEC,
		.type = RTM_GETLINK,
		.exec = ux_rtnl_link_get,
		.dump = ux_rtnl_link_dump,
	},
	{
		.family = PF_UNSPEC,
		.type = RTM_NEWLINK,
		.exec = ux_rtnl_link_add,
		.dump = ux_rtnl_link_dump,
	},
	{
		.family = PF_UNSPEC,
		.type = RTM_DELLINK,
		.exec = ux_rtnl_link_del,
		.dump = ux_rtnl_link_dump,
	},
	{
		.family = PF_UNSPEC,
		.type = RTM_GETROUTE,
		.exec = ux_rtnl_route_get,
		.dump = ux_rtnl_route_dump,
	},
	{
		.family = PF_UNSPEC,
		.type = RTM_GETNEIGH,
		.exec = ux_rtnl_neigh_exec,
		.dump = ux_rtnl_neigh_dump,
	},
	{
		.family = PF_UNSPEC,
		.type = RTM_DELNEIGH,
		.exec = ux_rtnl_neigh_exec,
		.dump = ux_rtnl_neigh_dump,
	},
	{
		.family = PF_UNSPEC,
		.type = RTM_NEWADDR,
		.exec = ux_rtnl_addr_add,
		.dump = ux_rtnl_addr_dump,
	},
	{
		.family = PF_UNSPEC,
		.type = RTM_GETADDR,
		.exec = ux_rtnl_addr_get,
		.dump = ux_rtnl_addr_dump,
	},
	{
		.family = PF_UNSPEC,
		.type = RTM_DELADDR,
		.exec = ux_rtnl_addr_del,
		.dump = ux_rtnl_addr_dump,
	},
};

static ux_rtnl_link_t *ux_rtnl_lookup_link_ops(int family, int type)
{
	ux_rtnl_link_t *link = NULL;
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(ux_rtnl_link_ops); i++) {
		link = &ux_rtnl_link_ops[i];
		if (link->type == type &&
		    (link->family == family || link->family == PF_UNSPEC)) {
			return link;
		}
	}

	return NULL;
}

static void ux_netlink_route_handle_msg(ux_sock_t *sk, const ux_pktbuf_t *pkt,
					struct nlmsghdr *nlh)
{
	struct rtgenmsg *rtmsg = NULL;
	ux_rtnl_link_t *link = NULL;
	int family, type, ret;

	UNUSED(pkt);
	/* route message must have at least sizeof(struct rtgenmsg) length */
	if (nlh->nlmsg_len < NLMSG_LENGTH(sizeof(struct rtgenmsg))) {
		ret = E_HM_INVAL;
		goto err;
	}

	rtmsg = ux_nlmsg_data(nlh);
	family = (int)rtmsg->rtgen_family;
	type = (int)nlh->nlmsg_type;

	link = ux_rtnl_lookup_link_ops(family, type);
	if (link == NULL) {
		ret = E_HM_POSIX_NOENT;
		goto err;
	}

	if ((nlh->nlmsg_flags & NLM_F_DUMP) == NLM_F_DUMP) {
		ret = link->dump(sk, nlh);
	} else {
		ret = link->exec(sk, nlh);
	}

err:
	if ((ret != 0) || ((nlh->nlmsg_flags & NLM_F_ACK) != 0 && (nlh->nlmsg_flags & NLM_F_DUMP) == 0)) {
		ux_netlink_error(sk, nlh, -hmerrno2posix(ret));
	}
	return;
}

static int ux_netlink_route_rcv(ux_sock_t *sk, ux_pktbuf_t *pkt)
{
	return ux_netlink_rcv(sk, pkt, ux_netlink_route_handle_msg);
}

static ux_netlink_proto_t netlink_route = {
	.rcv = ux_netlink_route_rcv,
	.protocol = NETLINK_ROUTE,
	.is_dynamic = false,
	.refcnt = __RAW_REFCNT_NR_INITIALIZER(0),
};

int ux_netlink_route_init(void)
{
	int ret = 0;

	ret = ux_netlink_create_service(&netlink_route);
	if (ret != E_HM_OK) {
		net_error(HMNET_NETLINK, "create route service failed\n");
		return ret;
	}

	ret = ux_netlink_proto_register(&netlink_route);
	if (ret != E_HM_OK) {
		net_error(HMNET_NETLINK, "register netlink route failed\n");
		return ret;
	}

	return 0;
}

void ux_netlink_route_exit(void)
{
	ux_netlink_proto_unregister(&netlink_route);
}
