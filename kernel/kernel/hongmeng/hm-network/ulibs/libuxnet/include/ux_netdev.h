/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for network netdev
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 28 13:34:09 2019
 */

#ifndef __LIBUXNET_UX_NETDEV_
#define __LIBUXNET_UX_NETDEV_

#include <sys/socket.h>
#include <net/if.h>
#include <net/route.h>

#include <lib/dlist.h>
#include <libhmsync/atomic.h>
#include <libhmshmalloc/shm_allocator.h>
#include <libhmsrv_io/net.h>
#include <libhmsync/raw_thread.h>

#include "ux_socket.h"
#include "ux_event.h"
#include "ux_netlog.h"
#include "ux_compat.h"

struct ux_netdev_ops;
struct ux_netdev_ops_priv;

#define UXDEV_PKT_STRUCT 0  // means pbuf or sbuf packet
#define UXDEV_PKT_RAW 1  // means raw packet
#define UXDEV_PKT_BUF_NOLINK 2  // means pbuf or sbuf packet without link header
#define UX_NETDEV_PRIV_DEFAULT	512
#define UX_NETDEV_MAXADDR 64 // means max number of addr for each dev
#define IFF_802_1Q_VLAN 1
#define IFF_BONDING  (1<<2)
#define ETHERNET_MTU	1500

typedef struct hmshm_mgr {
	struct share_mem_ctrl_info *netif_mpool;
	void *paddr;
	unsigned int pool_size;
	unsigned int unit_size;
} hmshm_mgr_t;

typedef struct ux_devhost {
	struct dlist_node head;
	uint32_t dh_cnode_idx;
	rref_t dh_ap_rref;
	hmshm_mgr_t shm;
	raw_atomic_int_t refcnt;
	raw_thread_t rx_thread;
	bool rx_thread_running;
} ux_devhost_t;

typedef struct ux_netdev {
	struct dlist_node head;
	struct dlist_node demux;

	char name[IFNAMSIZ];
	raw_atomic_int_t refcnt;

	struct ux_netdev *master;
	unsigned int flags;
	unsigned int priv_flags;
	/*
	 * _mtu: this member can only be visited by inner module.
	 * When other module want to get/set mtu, please use interface exported.
	 */
	int _mtu;

	int index;
	unsigned short type;

	uint64_t features;

	struct ux_netdev_ops *ops;
	struct ux_netdev_ops_priv *priv_ops;

	struct hmsrv_io_ctx ctx;
	struct hmshm_mgr *shmmgr;
	ux_devhost_t *devhost;
} ux_netdev_t;

typedef struct ux_netdev_ops {
	int (*ndo_init)(ux_netdev_t *dev);
	int (*ndo_xmit)(ux_netdev_t *dev, const void *data, int len, int flags);
	int (*ndo_set_mtu)(ux_netdev_t *dev, int mtu);
	int (*ndo_set_mac)(const ux_netdev_t *dev, const unsigned char *mac, int len);
	int (*ndo_ioctl)(ux_netdev_t *dev, int request, unsigned long args, bool is_fwd);
	int (*ndo_dump_stats)(ux_netdev_t *dev, void *buf, int size);
	int (*ndo_get_master)(const ux_netdev_t *dev);
	int (*ndo_dellink)(ux_netdev_t *dev);
	int (*ndo_add_slave)(const ux_netdev_t *master_dev, ux_netdev_t *slave_dev);
	int (*ndo_del_slave)(const ux_netdev_t *master_dev, ux_netdev_t *slave_dev);
} ux_netdev_ops_t;

typedef struct ux_netdev_ops_priv {
	int (*dump_stat)(ux_netdev_t *dev, void *buf, size_t len);
	int (*get_mac)(ux_netdev_t *dev, unsigned char *mac, size_t len);
	int (*getifaddrs)(ux_netdev_t *dev, char *buf, size_t size, int *cnt);
	int (*get_mtu)(ux_netdev_t *dev, int *mtu);
	void (*get_flags)(ux_netdev_t *dev, short int *flags, const char *name);
	void (*get_ipaddr)(ux_netdev_t *dev, struct sockaddr_in *sockaddr, const char *name);
	void (*get_netmask)(ux_netdev_t *dev, struct sockaddr_in *sockaddr, const char *name);
	int (*get_stats)(ux_netdev_t *dev, int file_bytes, char *temp);
	void (*get_dstaddr)(ux_netdev_t *dev, struct sockaddr_in *sockaddr);
	int (*get_ifconf)(ux_netdev_t *dev, void *ifr_dst, int ifr_len, int *size);

	/* notify protocol */
	int (*set_mac)(ux_netdev_t *dev, const unsigned char *mac, size_t len);
	int (*set_ipaddr)(ux_netdev_t *dev, struct sockaddr *addr, const char *name, unsigned int ipmask, bool netlink);
	int (*del_ipaddr)(ux_netdev_t *dev, struct sockaddr *addr, unsigned int ipmask);
	int (*set_netmask)(ux_netdev_t *dev, struct sockaddr *addr, const char *name);
	int (*set_dstaddr)(ux_netdev_t *dev, struct sockaddr *addr);
	int (*set_flags)(ux_netdev_t *dev, short int flags, const char *name);
	int (*set_mtu)(ux_netdev_t *dev, int mtu);
	int (*set_rxhwcsum)(ux_netdev_t *dev, int enable);

	int (*recv_zero_copy)(struct ux_netdev *dev, void *data, size_t len,
			      size_t offset);
	int (*recv)(struct ux_netdev *dev, const void *data, size_t len);
	int (*copy)(ux_netdev_t *dev, const void *from, void *to, size_t len,
		    size_t offset);
	void (*free)(ux_netdev_t *dev, const void *data);
	int (*add_header)(ux_netdev_t *dev, void *pkt, size_t len, const void *header, size_t header_len);
	int (*remove_header)(ux_netdev_t *dev, void *pkt, size_t len, void *saved_header, size_t header_len);

	/**
	 * operations for AF_PACKET, some device may need extra header when do
	 * packet capture, e.g loop interface, vsock interface
	 * @get_packet_hdr_len: if the packet type need extra header, return the header length,
	 * 			a zero header length won't cause set_packet_hdr call.
	 * @set_packet_hdr: set the real header data.
	 */
	size_t (*get_packet_hdr_len)(ux_netdev_t *dev, const void *data, size_t len, int flags);
	int (*set_packet_hdr)(ux_netdev_t *dev, void *hdr, const void *data,
			      size_t len, int flags);
} ux_netdev_ops_priv_t;

struct ux_packet_type {
	unsigned short type;	/* This is really htons(ether_type).	*/
	ux_netdev_t *dev;	/* NULL is wildcarded here		*/
	int (*func) (const void *, int, ux_netdev_t *, struct ux_packet_type *, int, int);
	void *af_packet_priv;
	struct dlist_node list;
};

typedef struct ux_copy_data_args {
	void *dst;
	size_t dstlen;
	const void *src;
	size_t count;
	int offset;
	int flags;
} ux_copy_data_args_t;

static inline void *ux_netdev_priv(const ux_netdev_t *dev)
{
	return (char *)dev + sizeof(ux_netdev_t);
}

static inline void *ux_netdev_priv_bond(const ux_netdev_t *dev)
{
	if (dev == NULL) {
		return NULL;
	}
	return (char *)dev + sizeof(ux_netdev_t) + UX_NETDEV_PRIV_DEFAULT;
}

static inline void *ux_netdev_priv_vlan(const ux_netdev_t *dev)
{
	return (char *)dev + sizeof(ux_netdev_t) + UX_NETDEV_PRIV_DEFAULT;
}

static inline ux_netdev_t *ux_netdev_from_priv(const void *priv)
{
	return (ux_netdev_t *)(void *)((char *)priv - sizeof(ux_netdev_t));
}

ux_netdev_t *ux_netdev_alloc(const char *name, size_t priv);
void ux_netdev_free(ux_netdev_t *dev);
void ux_dev_add_pt(struct ux_packet_type *pt);
void ux_dev_remove_pt(struct ux_packet_type *pt);
void ux_dev_queue_nit_comm(const void *data, const void *netif, int len, int flags);

int ux_netdev_register(ux_netdev_t *dev);
int ux_netdev_register_devhost(ux_netdev_t *dev, int ifindex);
void ux_netdev_unregister(ux_netdev_t *dev);

void ux_netdev_get(ux_netdev_t *dev);
void ux_netdev_put(ux_netdev_t *dev);

void ux_rtnl_lock(void);
void ux_rtnl_unlock(void);

ux_netdev_t *ux_netdev_get_by_name(const char *name);
ux_netdev_t *ux_netdev_get_by_index(int index);

int ux_netdev_xmit(ux_netdev_t *dev, const void *data, int len, int flags);
int ux_netdev_receive(ux_netdev_t *dev, const void *data, int len, int flags);
int ux_netdev_receive_zero_copy(ux_netdev_t *dev, void *data, int len, int offset, int flags);
int ux_netdev_copy_data(ux_netdev_t *dev, ux_copy_data_args_t *args);
void ux_netdev_free_data(ux_netdev_t *dev, const void *data);

void __ux_sync_macaddr_to_stack(ux_netdev_t *dev);
int __ux_netdev_get_mac(ux_netdev_t *dev, unsigned char *buf, int size);
int ux_netdev_get_mac(const char *name, unsigned char *buf, int size);
int ux_netdev_get_type(const char *name, unsigned short *type);
int __ux_netdev_set_mac(ux_netdev_t *dev, const unsigned char *buf, int size);
bool check_buf_zero(const unsigned char *buf, int size);
int ux_netdev_set_mac(const char *name, const unsigned char *buf, int size);
int ux_netdev_get_ipaddr(const char *name, struct sockaddr_in *sockaddr);
int ux_netdev_get_netmask(const char *name, struct sockaddr_in *sockaddr);
int ux_netdev_get_dstaddr(const char *name, struct sockaddr_in *sockaddr);
int ux_netdev_get_broadaddr(const char *name, struct sockaddr_in *sockaddr);
int ux_netdev_get_ifconf(struct ifconf *ifconf);
int ux_netdev_get_compat_ifconf(struct ifconf32 *ifconf32);
int ux_netdev_set_ipaddr(const char *name, struct sockaddr *addr);
int ux_netdev_set_netmask(const char *name, struct sockaddr *addr);
int ux_netdev_set_dstaddr(const char *name, struct sockaddr *addr);
void __ux_netdev_get_flags(ux_netdev_t *dev, short int *flags, const char *name);
int ux_netdev_get_flags(const char *name, short int *flags);
int __ux_netdev_set_flags(ux_netdev_t *dev, short int flags, const char *name);
int ux_netdev_set_flags(const char *name, short int flags);
int __ux_netdev_get_mtu(ux_netdev_t *dev, int *mtu);
int ux_netdev_get_mtu(const char *name, int *mtu);
int ux_netdev_update_features(ux_netdev_t *dev, uint64_t features);

int ux_netdev_get_master(const ux_netdev_t *dev);
int __ux_netdev_set_mtu(ux_netdev_t *dev, int mtu);
int ux_netdev_set_mtu(const char *name, int mtu);
int ux_netdev_set_updown(const char *name, int updown);
int ux_netdev_set_default(const char *name, int action, struct sockaddr *gw);
int ux_netdev_dump_stats(void *buf, int size);

int ux_netdev_get_index_by_name(const char *name, int *if_index);
int ux_netdev_get_name_by_index(int index, char *name);

int ux_netdev_getifaddrs(char *buf, int size);
int ux_get_netdev_info(void *rvaddr, size_t mpool_size);
unsigned int ux_get_netdev_nums(void);
int ux_netdev_ioctl_get(int cmd, struct ifreq *ifr);
int ux_netdev_ioctl(int cmd, struct ifreq *ifr);
int ux_netdev_compat_ioctl_get(int cmd, struct ifreq32 *ifr);
int ux_netdev_compat_ioctl(int cmd, struct ifreq32 *ifr);
int ux_netdev_ifr_copy_to_user(void *dst_ifr, struct ifreq *src_ifr, void **next_ifr);

int ux_netdev_for_each_device(int (*cb)(const ux_netdev_t *, void *), void *data);
int ux_netdev_set_route(struct rtentry *rt, int op);
int ux_route_ioctl(int cmd, struct rtentry *rt);

void ux_devhost_get(ux_devhost_t *dh);
void ux_devhost_put(ux_devhost_t *dh);
ux_devhost_t *ux_devhost_get_by_cnode(uint32_t cnode_idx,
				      unsigned int pool_size,
				      unsigned int unit_size,
				      uint64_t shm_id);
#endif
