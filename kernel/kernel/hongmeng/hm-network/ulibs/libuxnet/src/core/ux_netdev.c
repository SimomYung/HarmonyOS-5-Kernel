/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: interface for network device
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 28 13:34:09 2019
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>

#include <libhmsync/raw_rwlock.h>
#include <libhmlog/hmlog.h>
#include <libhmactv/actv.h>
#include <libstrict/strict.h>
#include <libsysif/utils.h>

#include "ux_netdev.h"
#include "ux_demux.h"
#include "libhwsecurec/securec.h"
#include "ux_netlog.h"
#include "hm_if_bonding.h"
#include "ux_vlan.h"
#include "ux_phy_mem.h"
#include "uev/ux_uev.h"

#define PER_NETDEV_SIZE 123
#define HEAD_MSG_SIZE 77
#define MULTICAST_MASK 0x01
#define MAC_ADDR_SIZE 6
#define MTU_MIN 68
#define MTU_MAX 65535
#define MAX_PER_NETDEV_INFO_SIZE 256
#define NETDEV_INFO_HEADER_SIZE 200
#define IFR_QLEN 1000
#define UX_NETDEV_NATIVE_INDEX_BASE 0x1000000
#define UX_NETDEV_NATIVE_NR  4096
#define MAX_NETDEV_NUM 512

STATIC_DEFINE_RAW_RWLOCK(ux_netdev_lock);
static DLIST_HEAD(ux_netdev_list);
STATIC_DEFINE_RAW_RWLOCK(ux_devhost_lock);
static DLIST_HEAD(ux_devhost_list);
static raw_atomic_int_t ux_netdev_index =
		RAW_ATOMIC_VAL_INIT(UX_NETDEV_NATIVE_INDEX_BASE);
static raw_atomic_int_t ux_netdev_num =
		RAW_ATOMIC_VAL_INIT(0);

STATIC_DEFINE_RAW_RWLOCK(ux_ptype_lock);
static DLIST_HEAD(ptype_all); /* Taps */
static int ux_netdev_nit = 0;

static struct raw_mutex ux_netdev_rtnl_lock = RAW_MUTEX_INITIALIZER;

#define devhost_for_each_entry(pos) \
	dlist_for_each_entry(pos, &ux_devhost_list, ux_devhost_t, head)

void ux_rtnl_lock(void)
{
	raw_mutex_lock(&ux_netdev_rtnl_lock);
}

void ux_rtnl_unlock(void)
{
	raw_mutex_unlock(&ux_netdev_rtnl_lock);
}

static ux_devhost_t *ux_nethost_alloc(void)
{
	ux_devhost_t *dh = NULL;

	dh = calloc(1, sizeof(ux_devhost_t));
	if (dh == NULL) {
		return NULL;
	}

	(void)raw_atomic_int_set(&dh->refcnt, 0);
	dlist_init(&dh->head);
	dh->rx_thread = NULL;
	dh->rx_thread_running = false;

	return dh;
}

static void __ux_devhost_register(ux_devhost_t *dh)
{
	(void)raw_atomic_int_inc(&dh->refcnt);
	/* the max number limited by os process number CONFIG_SYSMGR_MAX_PROCESSES */
	dlist_insert_tail(&ux_devhost_list, &dh->head);
}

static void ux_devhost_unregister(ux_devhost_t *dh)
{
	(void)raw_rwlock_wrlock(&ux_devhost_lock);
	dlist_delete(&dh->head);
	(void)raw_rwlock_unlock(&ux_devhost_lock);
}

void ux_devhost_get(ux_devhost_t *dh)
{
	(void)raw_atomic_int_inc(&dh->refcnt);
}

static ux_devhost_t *__ux_devhost_get_by_cnode(uint32_t cnode_idx)
{
	ux_devhost_t *dh = NULL;

	devhost_for_each_entry(dh) {
		if (dh->dh_cnode_idx == cnode_idx) {
			return dh;
		}
	}
	return NULL;
}

static int ux_devhost_mmap_shm(ux_devhost_t *dh,
			uint32_t cnode_idx,
			unsigned int pool_size,
			unsigned int unit_size,
			uint64_t shm_key)
{
	uint32_t flags =  MFLAG_MAP_SHARED | MFLAG_MAP_ANONYMOUS;
	void *rvaddr = NULL;
	int shm_id;

	UNUSED(unit_size);
	shm_id = hm_ashm_open_auth(shm_key, cnode_idx, pool_size, SHM_O_RDWR, NULL);
	if (shm_id < 0) {
		net_error(HMNET_CORE, "open shm failed: %s\n", hmstrerror(shm_id));
		return shm_id;
	}
	rvaddr = hm_mem_mmap(NULL, pool_size, PROT_READ | PROT_WRITE, flags, shm_id, 0);
	if (rvaddr == MAP_FAILED) {
		net_error(HMNET_CORE, "map shm failed, shm_id=%d\n", shm_id);
		(void)hm_ashm_close(shm_id);
		return E_HM_NOMEM;
	}
	(void)hm_ashm_close(shm_id);

	dh->shm.netif_mpool = rvaddr;
	return 0;
}

ux_devhost_t *ux_devhost_get_by_cnode(uint32_t cnode_idx,
				      unsigned int pool_size,
				      unsigned int unit_size,
				      uint64_t shm_id)
{
	ux_devhost_t *dh = NULL;
	int ret;

	(void)raw_rwlock_wrlock(&ux_devhost_lock);
	dh = __ux_devhost_get_by_cnode(cnode_idx);
	if (dh != NULL) {
		ux_devhost_get(dh);
		(void)raw_rwlock_unlock(&ux_devhost_lock);
		return dh;
	}

	dh = ux_nethost_alloc();
	if (dh == NULL) {
		(void)raw_rwlock_unlock(&ux_devhost_lock);
		return NULL;
	}

	if (cnode_idx == hm_ucap_self_cnode_idx()) {
		dh->shm.netif_mpool = (void *)(uintptr_t)shm_id;
	} else {
		ret = ux_devhost_mmap_shm(dh, cnode_idx, pool_size, unit_size, shm_id);
		if (ret != 0) {
			free(dh);
			dh = NULL;
			(void)raw_rwlock_unlock(&ux_devhost_lock);
			return NULL;
		}
	}

	dh->dh_cnode_idx = cnode_idx;
	dh->shm.pool_size = pool_size;
	dh->shm.unit_size = unit_size;

	__ux_devhost_register(dh);
	(void)raw_rwlock_unlock(&ux_devhost_lock);
	return dh;
}

static void ux_devhost_free(ux_devhost_t *dh)
{
	free(dh);
}

void ux_devhost_put(ux_devhost_t *dh)
{
	if (raw_atomic_int_dec(&dh->refcnt) == 0) {
		ux_devhost_unregister(dh);
		if (dh->dh_cnode_idx != hm_ucap_self_cnode_idx()) {
			if (hm_mem_munmap(dh->shm.netif_mpool, dh->shm.pool_size) != E_HM_OK) {
				net_error(HMNET_CORE, "unmap share mem failed\n");
			}
		}
		ux_devhost_free(dh);
	}
}

#define netdev_for_each_entry(pos) \
	dlist_for_each_entry(pos, &ux_netdev_list, ux_netdev_t, head)

ux_netdev_t *ux_netdev_alloc(const char *name, size_t priv)
{
	ux_netdev_t *dev = NULL;

	priv = ((priv != 0) ? priv : UX_NETDEV_PRIV_DEFAULT);

	dev = calloc(1, sizeof(ux_netdev_t) + priv);
	if (dev == NULL) {
		return NULL;
	}

	if (name != NULL) {
		if (memchr(name, '\0', IF_NAMESIZE) == NULL || strcpy_s(dev->name, IF_NAMESIZE, name) != 0) {
			free(dev);
			return NULL;
		}
	}

	(void)raw_atomic_int_set(&dev->refcnt, 0);
	dlist_init(&dev->head);
	dev->type = ARPHRD_ETHER;
	dev->devhost = NULL;

	return dev;
}

void ux_netdev_free(ux_netdev_t *dev)
{
	free(dev);
}

void ux_dev_add_pt(struct ux_packet_type *pt)
{
	(void)raw_rwlock_wrlock(&ux_ptype_lock);
	/*
	 * The max number limited by socket number, and socket number limited by
	 * hm share mem num CONFIG_SYSMGR_SHM_NODE_MAX_NUM
	 */
	dlist_insert_tail(&ptype_all, &pt->list);
	ux_netdev_nit++;
	(void)raw_rwlock_unlock(&ux_ptype_lock);
}

static void __ux_dev_remove_pt(struct ux_packet_type *pt)
{
	struct ux_packet_type *pt1 = NULL;
	struct ux_packet_type *n = NULL;

	dlist_for_each_entry_safe(pt1, n, &ptype_all, struct ux_packet_type, list) {
		if (pt == pt1) {
			ux_netdev_nit--;
			dlist_delete(&pt->list);
			return;
		}
	}
}

void ux_dev_remove_pt(struct ux_packet_type *pt)
{
	(void)raw_rwlock_wrlock(&ux_ptype_lock);
	__ux_dev_remove_pt(pt);
	(void)raw_rwlock_unlock(&ux_ptype_lock);
}

static int __ux_netdev_register(ux_netdev_t *dev, int devidx)
{
	ux_packet_demux_t *demux = NULL;
	ux_netdev_t *tmp = NULL;
	int ret = 0;
	unsigned int netdev_num;

	netdev_num = (unsigned int)raw_atomic_int_read(&ux_netdev_num);
	if (netdev_num >= MAX_NETDEV_NUM) {
		return E_HM_NOSPC;
	}

	if (dev->name[0] == '\0') {
		return E_HM_INVAL;
	}

	netdev_for_each_entry(tmp) {
		if (strcmp(tmp->name, dev->name) == 0) {
			net_info(HMNET_CORE, "netdev '%s' already exists.\n", dev->name);
			return E_HM_POSIX_EXIST;
		}
	}

	dev->index = devidx;

	/* do demux init for dev type */
	demux = ux_packet_demux_lookup(dev->type);
	if (demux != NULL) {
		if (demux->init != NULL) {
			ret = demux->init(dev);
			if (ret != 0) {
				return ret;
			}
		}
	}

	/* do dev init for dev ops */
	if (dev->ops != NULL && dev->ops->ndo_init != NULL) {
		ret = dev->ops->ndo_init(dev);
		if (ret != 0) {
			goto err_uninit;
		}
	}

	(void)raw_atomic_int_inc(&dev->refcnt);
	dlist_insert_tail(&ux_netdev_list, &dev->head);
	(void)raw_atomic_int_inc(&ux_netdev_num);

	return 0;
err_uninit:
	if (demux != NULL && demux->uninit != NULL) {
		demux->uninit(dev);
	}
	return ret;
}

static ux_netdev_t *__ux_netdev_get_by_index(int index)
{
	ux_netdev_t *dev = NULL;

	netdev_for_each_entry(dev) {
		if (dev->index == index) {
			return dev;
		}
	}

	return NULL;
}

ux_netdev_t *ux_netdev_get_by_index(int index)
{
	ux_netdev_t *dev = NULL;

	(void)raw_rwlock_rdlock(&ux_netdev_lock);
	dev = __ux_netdev_get_by_index(index);
	if (dev != NULL) {
		ux_netdev_get(dev);
	}
	(void)raw_rwlock_unlock(&ux_netdev_lock);

	return dev;
}

static int __ux_netdev_new_index(void)
{
	int ifindex = 0;
	int i = 0;

	for (i = 0; i < UX_NETDEV_NATIVE_NR; i++) {
		ifindex = raw_atomic_int_inc(&ux_netdev_index);
		if (ifindex > UX_NETDEV_NATIVE_INDEX_BASE +
		    UX_NETDEV_NATIVE_NR) {
			(void)raw_atomic_int_set(&ux_netdev_index,
				UX_NETDEV_NATIVE_INDEX_BASE);
			continue;
		}
		if (!__ux_netdev_get_by_index(ifindex)) {
			return ifindex;
		}
	}

	return E_HM_POSIX_NOENT;
}

int ux_netdev_register(ux_netdev_t *dev)
{
	int ifindex = 0;
	int ret = 0;

	(void)raw_rwlock_wrlock(&ux_netdev_lock);
	ifindex = __ux_netdev_new_index();
	if (ifindex < 0) {
		(void)raw_rwlock_unlock(&ux_netdev_lock);
		return ifindex;
	}
	ret = __ux_netdev_register(dev, ifindex);
	(void)raw_rwlock_unlock(&ux_netdev_lock);

	return ret;
}

int ux_netdev_register_devhost(ux_netdev_t *dev, int ifindex)
{
	int ret = 0;

	if (ifindex > UX_NETDEV_NATIVE_INDEX_BASE) {
		return E_HM_INVAL;
	}

	(void)raw_rwlock_wrlock(&ux_netdev_lock);
	if (__ux_netdev_get_by_index(ifindex)) {
		(void)raw_rwlock_unlock(&ux_netdev_lock);
		return E_HM_POSIX_EXIST;
	}

	ret = __ux_netdev_register(dev, ifindex);
	(void)raw_rwlock_unlock(&ux_netdev_lock);

	return ret;
}

static void __ux_netdev_unregister(ux_netdev_t *dev)
{
	ux_packet_demux_t *demux = NULL;

	if (!dlist_empty(&dev->head)) {
		/* do demux uninit for dev type */
		demux = ux_packet_demux_lookup(dev->type);
		if (demux != NULL && demux->uninit != NULL) {
			demux->uninit(dev);
		}

		dlist_delete(&dev->head);
		(void)raw_atomic_int_dec(&ux_netdev_num);
		ux_netdev_put(dev);
	}

	return;
}

void ux_netdev_unregister(ux_netdev_t *dev)
{
	if (dev == NULL) {
		return;
	}

	(void)raw_rwlock_wrlock(&ux_netdev_lock);
	__ux_netdev_unregister(dev);
	(void)raw_rwlock_unlock(&ux_netdev_lock);
}

int ux_netdev_for_each_device(int (*cb)(const ux_netdev_t *, void *), void *data)
{
	ux_netdev_t *dev = NULL;
	int ret = 0;

	(void)raw_rwlock_rdlock(&ux_netdev_lock);
	netdev_for_each_entry(dev) {
		ret = cb(dev, data);
		if (ret != 0) {
			break;
		}
	}
	(void)raw_rwlock_unlock(&ux_netdev_lock);

	return ret;
}

void ux_netdev_get(ux_netdev_t *dev)
{
	(void)raw_atomic_int_inc(&dev->refcnt);
}

void ux_netdev_put(ux_netdev_t *dev)
{
	if (raw_atomic_int_dec(&dev->refcnt) == 0) {
		ux_netdev_free(dev);
	}
}

static ux_netdev_t *__ux_netdev_get_by_name(const char *name)
{
	ux_netdev_t *dev = NULL;

	if (name == NULL) {
		return NULL;
	}

	if (memchr(name, '\0', IF_NAMESIZE) == NULL) {
		return NULL;
	}

	netdev_for_each_entry(dev) {
		if (strcmp(name, dev->name) == 0) {
			return dev;
		}
	}

	return NULL;
}

ux_netdev_t *ux_netdev_get_by_name(const char *name)
{
	ux_netdev_t *dev = NULL;
	char *colon = NULL;

	if (name == NULL) {
		return NULL;
	}

	colon = strchr(name, ':');
	if (colon != NULL) {
		*colon = 0;
	}

	(void)raw_rwlock_rdlock(&ux_netdev_lock);
	dev = __ux_netdev_get_by_name(name);
	if (dev != NULL) {
		ux_netdev_get(dev);
	}
	(void)raw_rwlock_unlock(&ux_netdev_lock);

	if (colon != NULL) {
		*colon = ':';
	}

	return dev;
}

int ux_netdev_get_index_by_name(const char *name, int *if_index)
{
	ux_netdev_t *dev = NULL;

	dev = ux_netdev_get_by_name(name);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	*if_index = dev->index;
	ux_netdev_put(dev);

	return 0;
}

int ux_netdev_get_name_by_index(int index, char *name)
{
	ux_netdev_t *dev = NULL;

	dev = ux_netdev_get_by_index(index);
	if (dev == NULL) {
		return E_HM_NXIO;
	}

	NOFAIL(memcpy_s(name, IF_NAMESIZE, dev->name, IF_NAMESIZE));

	ux_netdev_put(dev);
	return 0;
}

/* flags:  1 means raw packet, 0 means pbuf or sbuf packet */
static void ux_dev_queue_nit(const void *data, ux_netdev_t *dev, int len, int flags, int isxmit)
{
	struct ux_packet_type *ptype = NULL;

	if (ux_netdev_nit == 0) {
		return;
	}

	(void)raw_rwlock_rdlock(&ux_ptype_lock);
	dlist_for_each_entry(ptype, &ptype_all, struct ux_packet_type, list) {
		if (ptype->dev == dev || !ptype->dev) {
			ptype->func(data, len, dev, ptype, flags, isxmit);
		}
	}
	(void)raw_rwlock_unlock(&ux_ptype_lock);
}

void ux_dev_queue_nit_comm(const void *data, const void *netif, int len, int flags)
{
	ux_dev_queue_nit(data, ux_netdev_from_priv(netif), len, flags, 1);
}

/* flags:  1 means raw packet, 0 means pbuf or sbuf packet */
int ux_netdev_xmit(ux_netdev_t *dev, const void *data, int len, int flags)
{
	uev_trace_netdev_xmit(dev, len, flags);
	ux_dev_queue_nit(data, dev, len, flags, 1);
	if (dev->ops->ndo_xmit == NULL) {
		return E_HM_OPNOTSUPP;
	}
	return dev->ops->ndo_xmit(dev, data, len, flags);
}

static int __netdev_receive(ux_netdev_t *master, uintptr_t data, int len, int flags)
{
	if (master->priv_ops == NULL || master->priv_ops->recv == NULL) {
		return E_HM_OPNOTSUPP;
	}

	uev_trace_netdev_recv(master, len);
	ux_dev_queue_nit((void *)data, master, len, flags, 0);
	return master->priv_ops->recv(master, (void *)data, (size_t)(unsigned int)len);
}

int ux_netdev_receive(ux_netdev_t *dev, const void *data, int len, int flags)
{
	if (((dev->flags & IFF_SLAVE) != 0U) && ((dev->priv_flags & IFF_BONDING) != 0U)) {
		if (dev->master == NULL) {
			return E_HM_NODEV;
		}
		return __netdev_receive(dev->master, (uintptr_t)data, len, flags);
	}

	return __netdev_receive(dev, (uintptr_t)data, len, flags);
}

static int __ux_netdev_receive_zero_copy(ux_netdev_t *master, uintptr_t data, int len,
					 int offset, int flags)
{
	if (master->priv_ops == NULL ||
	    master->priv_ops->recv_zero_copy == NULL) {
		return E_HM_OPNOTSUPP;
	}

	uev_trace_netdev_recv(master, len);
	ux_dev_queue_nit((void *)data, master, len, flags, 0);
	return master->priv_ops->recv_zero_copy(master, (void *)data, (size_t)(unsigned int)len,
						(size_t)(unsigned int)offset);
}

int ux_netdev_receive_zero_copy(ux_netdev_t *dev, void *data, int len, int offset, int flags)
{
	int err = 0;
	struct ethhdr *ethhdr;
	struct vlanhdr *vlanhdr;
	ux_netdev_t *indev = dev;
	ux_netdev_t *vlandev = NULL;

	if (dev->type == ARPHRD_ETHER) {
		ethhdr = (struct ethhdr *)data;
		if (ethhdr->h_proto == htons(ETH_P_8021Q)) {
			vlanhdr = (struct vlanhdr *)(ethhdr + 1);
			vlandev = vlan_netdev_get(dev, htons(vlanhdr->tp_vid));
			if (vlandev == NULL) {
				net_debug(HMNET_INET, "No vlan device found for vlan id %hu\n", vlanhdr->tp_vid);
				phy_mem_free(dev->shmmgr, data, (unsigned int)offset);
				return 0;
			}

			ux_dev_queue_nit(data, dev, len, flags, 0);
			vlan_untag((void **)&data, (int *)&len);
			/* reset indev to vlan dev */
			indev = vlandev;
		}
	}

	if (((dev->flags & IFF_SLAVE) != 0U) && ((dev->priv_flags & IFF_BONDING) != 0U)) {
		if (dev->master == NULL) {
			net_warn(HMNET_INET, "Bonding master device of %s not found\n", dev->name);
			err = E_HM_NODEV;
			goto vlandev_put;
		}
		/* reset indev to bonding master */
		indev = dev->master;
	}

	err = __ux_netdev_receive_zero_copy(indev, (uintptr_t)data, len, offset, flags);

vlandev_put:
	if (vlandev != NULL) {
		ux_netdev_put(vlandev);
	}

	return err;
}

/*
 * flags:  1 means raw packet, 0/2 means pbuf or sbuf packet
 * 2 means lo packet, no link header
 */
int ux_netdev_copy_data(ux_netdev_t *dev, ux_copy_data_args_t *args)
{
	int ret;

	if (dev == NULL || args == NULL || args->dstlen < args->count) {
		return E_HM_INVAL;
	}
	if (args->flags == 1) {
		ret = memcpy_s(args->dst, args->dstlen,
			       (void *)((char *)args->src + args->offset), args->count);
		if (ret != 0) {
			return E_HM_POSIX_FAULT;
		} else {
			return (int)(unsigned int)args->count;
		}
	}
	if (dev->priv_ops == NULL || dev->priv_ops->copy == NULL) {
		return E_HM_OPNOTSUPP;
	}

	return dev->priv_ops->copy(dev, (void *)args->src, args->dst,
				   args->count, (size_t)(unsigned int)args->offset);
}

void ux_netdev_free_data(ux_netdev_t *dev, const void *data)
{
	if (dev->priv_ops != NULL && dev->priv_ops->free != NULL) {
		dev->priv_ops->free(dev, data);
	}
}

int __ux_netdev_get_mac(ux_netdev_t *dev, unsigned char *buf, int size)
{
	/* get form network stack */
	if (dev->priv_ops != NULL && dev->priv_ops->get_mac != NULL) {
		return dev->priv_ops->get_mac(dev, buf, (size_t)(unsigned int)size);
	}

	return 0;
}

int ux_netdev_get_mac(const char *name, unsigned char *buf, int size)
{
	ux_netdev_t *dev = NULL;
	int ret;

	dev = ux_netdev_get_by_name(name);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	ret = __ux_netdev_get_mac(dev, buf, size);

	ux_netdev_put(dev);

	return ret;
}

int ux_netdev_get_type(const char *name, unsigned short *type)
{
	ux_netdev_t *dev = NULL;

	if (type == NULL) {
		return E_HM_INVAL;
	}

	dev = ux_netdev_get_by_name(name);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	*type = dev->type;

	ux_netdev_put(dev);

	return E_HM_OK;
}

static inline bool is_zero_mac_addr(const unsigned char *buf, int size)
{
	int i;

	for (i = 0; i < size; i++) {
		if (buf[i] != 0) {
			return false;
		}
	}

	return true;
}

void __ux_sync_macaddr_to_stack(ux_netdev_t *dev)
{
	struct ifreq ifr = {0};
	int ret = 0;

	if (dev->ops == NULL || dev->ops->ndo_ioctl == NULL) {
		return;
	}

	NOFAIL(memcpy_s(ifr.ifr_name, sizeof(ifr.ifr_name), dev->name,
			sizeof(dev->name)));
	hm_actv_set_accessible_mem_range(ptr_to_ulong(&ifr), sizeof(ifr));
	ret = dev->ops->ndo_ioctl(dev, SIOCGIFHWADDR, (unsigned long)&ifr, false);
	if (ret != 0) {
		net_error(NET_CORE, "SIOCGIFHWADDR fail, ret=%d\n", ret);
		return;
	}

	/* notify network stack */
	if (dev->priv_ops != NULL && dev->priv_ops->set_mac != NULL) {
		/* mac addr len is 6 */
		ret = dev->priv_ops->set_mac(dev, (void *)ifr.ifr_hwaddr.sa_data, 6);
		if (ret != 0) {
			net_error(HMNET_CORE, "notfy stack fail, ret=%d\n", ret);
			return;
		}
	}
}

int __ux_netdev_set_mac(ux_netdev_t *dev, const unsigned char *buf, int size)
{
	struct ifreq ifr = {0};
	int ret = 0;

	if (buf == NULL) {
		return E_HM_POSIX_FAULT;
	}

	if ((buf[0] & MULTICAST_MASK) != 0U) {
		return E_HM_ADDRNOTAVAIL;
	}

	/* do not set mac 00:00:00:00:00:00 */
	if (is_zero_mac_addr(buf, size)) {
		return E_HM_ADDRNOTAVAIL;
	}

	NOFAIL(strncpy_s(ifr.ifr_name, IFNAMSIZ, dev->name,
			 sizeof(dev->name) - 1));
	ifr.ifr_hwaddr.sa_family = dev->type;

	ret = memcpy_s(ifr.ifr_hwaddr.sa_data, sizeof(ifr.ifr_hwaddr.sa_data),
		       buf, (unsigned int)size);
	if (ret != 0) {
		return E_HM_INVAL;
	}

	if (dev->ops != NULL && dev->ops->ndo_ioctl != NULL) {
		hm_actv_set_accessible_mem_range(ptr_to_ulong(&ifr), sizeof(ifr));
		ret = dev->ops->ndo_ioctl(dev, SIOCSIFHWADDR, (unsigned long)&ifr, false);
		if (ret != 0) {
			return ret;
		}
	}

	/* notify network stack */
	if (dev->priv_ops != NULL && dev->priv_ops->set_mac != NULL) {
		return dev->priv_ops->set_mac(dev, buf, (size_t)(unsigned int)size);
	}

	return 0;
}

int ux_netdev_set_mac(const char *name, const unsigned char *buf, int size)
{
	ux_netdev_t *dev = NULL;
	int ret;

	dev = ux_netdev_get_by_name(name);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	ret = __ux_netdev_set_mac(dev, buf, size);

	ux_netdev_put(dev);

	return ret;
}

int ux_netdev_set_ipaddr(const char *name, struct sockaddr *addr)
{
	ux_netdev_t *dev = NULL;
	struct sockaddr_in old_sockaddr;
	struct sockaddr_in *sockaddr = NULL;
	int ret = 0;

	dev = ux_netdev_get_by_name(name);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	mem_zero_s(old_sockaddr);
	sockaddr = (struct sockaddr_in *)addr;
	(void)ux_netdev_get_ipaddr(name, &old_sockaddr);
	/* if set the same ip, not set */
	if (old_sockaddr.sin_addr.s_addr == sockaddr->sin_addr.s_addr) {
		ux_netdev_put(dev);
		return 0;
	}

	if (dev->priv_ops != NULL && dev->priv_ops->set_ipaddr != NULL) {
		ret = dev->priv_ops->set_ipaddr(dev, addr, name, 0, false);
	}

	ux_netdev_put(dev);

	return ret;
}

int ux_netdev_set_netmask(const char *name, struct sockaddr *addr)
{
	ux_netdev_t *dev = NULL;
	struct sockaddr_in old_sockaddr;
	int ret = 0;

	dev = ux_netdev_get_by_name(name);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	/* can't set netmask when ip equals to zero */
	mem_zero_s(old_sockaddr);
	(void)ux_netdev_get_ipaddr(name, &old_sockaddr);
	if (old_sockaddr.sin_addr.s_addr == 0) {
		ux_netdev_put(dev);
		return E_HM_ADDRNOTAVAIL;
	}

	if (dev->priv_ops != NULL && dev->priv_ops->set_netmask != NULL) {
		ret = dev->priv_ops->set_netmask(dev, addr, name);
	}

	ux_netdev_put(dev);

	return ret;
}

#ifndef IN_CLASSE
#define IN_CLASSE(a) ((((in_addr_t)(a)) & 0xf0000000) == 0xf0000000)
#endif

static bool __ux_netdev_is_valid_dstaddr(struct sockaddr *addr)
{
	struct sockaddr_in *sin = NULL;
	in_addr_t haddr = 0;

	sin = (struct sockaddr_in *)addr;
	haddr = ntohl(sin->sin_addr.s_addr);
	if (haddr == INADDR_ANY || haddr == INADDR_BROADCAST ||
	    IN_CLASSA(haddr) || IN_CLASSB(haddr) ||
	    IN_CLASSC(haddr) || IN_CLASSE(haddr)) {
		return true;
	}

	return false;
}

int ux_netdev_set_dstaddr(const char *name, struct sockaddr *addr)
{
	ux_netdev_t *dev = NULL;
	struct sockaddr_in old_sockaddr;
	int ret = 0;

	dev = ux_netdev_get_by_name(name);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	/* can't set dstaddr when ip equals to zero */
	mem_zero_s(old_sockaddr);
	(void)ux_netdev_get_ipaddr(name, &old_sockaddr);
	if (old_sockaddr.sin_addr.s_addr == 0) {
		ux_netdev_put(dev);
		return E_HM_ADDRNOTAVAIL;
	}

	if(!__ux_netdev_is_valid_dstaddr(addr)) {
		ux_netdev_put(dev);
		return E_HM_INVAL;
	}

	if (dev->priv_ops != NULL && dev->priv_ops->set_dstaddr != NULL) {
		ret = dev->priv_ops->set_dstaddr(dev, addr);
	}

	ux_netdev_put(dev);

	return ret;
}

int __ux_netdev_set_mtu(ux_netdev_t *dev, int mtu)
{
	int ret = 0;

	if (dev->ops != NULL && dev->ops->ndo_set_mtu != NULL) {
		ret = dev->ops->ndo_set_mtu(dev, mtu);
		if (ret != 0) {
			return ret;
		}
	}

	/* notify network stack */
	if (dev->priv_ops != NULL && dev->priv_ops->set_mtu != NULL) {
		ret = dev->priv_ops->set_mtu(dev, mtu);
	}
	return ret;
}

int ux_netdev_set_mtu(const char *name, int mtu)
{
	ux_netdev_t *dev = NULL;
	int ret = 0;

	if (mtu < MTU_MIN || mtu > MTU_MAX) {
		return E_HM_INVAL;
	}

	dev = ux_netdev_get_by_name(name);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	ret = __ux_netdev_set_mtu(dev, mtu);
	ux_netdev_put(dev);

	return ret;
}

int ux_netdev_get_master(const ux_netdev_t *dev)
{
	ux_netdev_t *master_dev = NULL;
	int devno;

	if (dev != NULL && dev->ops != NULL && dev->ops->ndo_get_master != NULL) {
		devno = dev->ops->ndo_get_master(dev);
		master_dev = ux_netdev_get_by_index(devno);
		if (master_dev != NULL) {
			return master_dev->index;
		}
	}
	return E_HM_OPNOTSUPP;
}

/*
 * copy form uapi/linux/ethtool.h
 */
#define ETHTOOL_GRXCSUM		0x00000014 /* Get RX hw csum enable (ethtool_value) */
#define ETHTOOL_GTXCSUM		0x00000016 /* Get TX hw csum enable (ethtool_value) */
struct ethtool_value {
	uint32_t cmd;
	uint32_t data;
};

/*
 * return: 1, enable; 0, disable/unkown
 */
static int ux_netdev_get_hwcsum(ux_netdev_t *dev, uint32_t cmd)
{
	struct {
		struct ethtool_value edata;
		struct ifreq ifr;
	} rpc_mem;
	struct hmsrv_io_ctx ctx;
	int ret = 0;

	bzero(&rpc_mem, sizeof(rpc_mem));
	bzero(&ctx, sizeof(ctx));
	rpc_mem.edata.cmd = cmd;
	rpc_mem.edata.data = 0;
	rpc_mem.ifr.ifr_data = (char *)&rpc_mem.edata;
	NOFAIL(strncpy_s(rpc_mem.ifr.ifr_name, IFNAMSIZ, dev->name,
			 sizeof(dev->name) - 1));

	if (dev->ops != NULL && dev->ops->ndo_ioctl != NULL) {
		/*
		 * Devhost believes that a ctx can only correspond to one
		 * kind of activation forward behavior; Our netdev's ioctl
		 * default behavior is forward, so here we deal with it
		 * specifically.
		 */
		ctx = dev->ctx;
		ctx.flags = 0; /* set activation isfwd to false */
		hm_actv_set_accessible_mem_range(ptr_to_ulong(&rpc_mem), sizeof(rpc_mem));
		ret = dev->ops->ndo_ioctl(dev, SIOCETHTOOL, (unsigned long)(uintptr_t)&rpc_mem.ifr, false);
		if (ret != E_HM_OK) {
			return ret;
		}
	}

	return (int)rpc_mem.edata.data;
}

int ux_netdev_update_features(ux_netdev_t *dev, uint64_t features)
{
	int ret = 0;

	if (dev == NULL) {
		return E_HM_NODEV;
	}

	if (dev->features == features) {
		return 0;
	}

	/* notify network stack */
	if (dev->priv_ops != NULL && dev->priv_ops->set_rxhwcsum != NULL) {
		if (ux_netdev_get_hwcsum(dev, ETHTOOL_GRXCSUM) != 0) {
			ret = dev->priv_ops->set_rxhwcsum(dev, 1);
		} else {
			ret = dev->priv_ops->set_rxhwcsum(dev, 0);
		}
	}

	if (ret == 0) {
		dev->features = features;
		net_info(HMNET_CORE, "update %s features:%"PRIu64"\n",
			 dev->name, features);
	} else {
		net_error(HMNET_CORE, "update %s features:%"PRIu64" failed\n",
			 dev->name, features);
	}

	return ret;
}

int ux_netdev_dump_stats(void *buf, int size)
{
	ux_netdev_t *dev = NULL;

	(void)raw_rwlock_rdlock(&ux_netdev_lock);

	netdev_for_each_entry(dev) {
		/* devhost driver uses "dev->ops", native driver uses "dev->priv_ops" */
		if (dev->ops != NULL && dev->ops->ndo_dump_stats != NULL) {
			dev->ops->ndo_dump_stats(dev, buf, size);
		} else if (dev->priv_ops != NULL &&
			   dev->priv_ops->dump_stat != NULL) {
			if (dev->priv_ops->dump_stat(dev, buf, (size_t)(unsigned int)size) < 0) {
				(void)raw_rwlock_unlock(&ux_netdev_lock);
				return E_HM_INVAL;
			}
		}
	}

	(void)raw_rwlock_unlock(&ux_netdev_lock);

	return 0;
}

int __ux_netdev_set_flags(ux_netdev_t *dev, short int flags, const char *name)
{
	struct ifreq ifr = {0};
	int ret = E_HM_OPNOTSUPP;

	ifr.ifr_flags = flags;
	NOFAIL(strncpy_s(ifr.ifr_name, IFNAMSIZ, dev->name,
			 sizeof(dev->name) - 1));
	if (dev->ops != NULL && dev->ops->ndo_ioctl != NULL) {
		hm_actv_set_accessible_mem_range(ptr_to_ulong(&ifr), sizeof(ifr));
		ret = dev->ops->ndo_ioctl(dev, SIOCSIFFLAGS, (unsigned long)&ifr, false);
		if (ret != 0) {
			return ret;
		}
	}

	/* notify network stack */
	if (dev->priv_ops != NULL && dev->priv_ops->set_flags != NULL) {
		ret = dev->priv_ops->set_flags(dev, flags, name);
	}
	return ret;
}

int ux_netdev_set_flags(const char *name, short int flags)
{
	ux_netdev_t *dev = NULL;
	int ret;

	dev = ux_netdev_get_by_name(name);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	ret = __ux_netdev_set_flags(dev, flags, name);
	ux_netdev_put(dev);

	return ret;
}

int ux_netdev_set_default(const char *name, int action, struct sockaddr *gw)
{
	ux_packet_demux_t *demux = NULL;
	ux_netdev_t *dev = NULL;
	int ret;

	demux = ux_packet_demux_lookup(ARPHRD_ETHER);
	if (demux != NULL && demux->set_default != NULL) {
		/* dev could be null, it's ok */
		dev = ux_netdev_get_by_name(name);
		ret = demux->set_default(dev, action, gw);
		if (dev != NULL) {
			ux_netdev_put(dev);
		}
		return ret;
	}

	return 0;
}

int ux_netdev_getifaddrs(char *buf, int size)
{
	ux_netdev_t *dev = NULL;
	int offset = 0;
	int cnt = 0;
	int len;
	int each_cnt;

	(void)raw_rwlock_rdlock(&ux_netdev_lock);

	netdev_for_each_entry(dev) {
		if (dev->priv_ops != NULL &&
		    dev->priv_ops->getifaddrs != NULL) {
			len = dev->priv_ops->getifaddrs(dev, buf + offset,
				(size_t)(unsigned int)(size - offset), &each_cnt);
			if (len == 0) {
				break;
			}
			offset += len;
			cnt += each_cnt;
		}
		if (offset > size) {
			break;
		}
	}

	(void)raw_rwlock_unlock(&ux_netdev_lock);

	return cnt;
}

int __ux_netdev_get_mtu(ux_netdev_t *dev, int *mtu)
{
	if (dev->priv_ops != NULL && dev->priv_ops->get_mtu != NULL) {
		return dev->priv_ops->get_mtu(dev, mtu);
	}
	return 0;
}

int ux_netdev_get_mtu(const char *name, int *mtu)
{
	ux_netdev_t *dev = NULL;
	int ret = 0;

	dev = ux_netdev_get_by_name(name);
	if (dev == NULL) {
		return E_HM_NODEV;
	}
	ret = __ux_netdev_get_mtu(dev, mtu);
	ux_netdev_put(dev);

	return ret;
}

void __ux_netdev_get_flags(ux_netdev_t *dev, short int *flags, const char *name)
{
	if (dev->priv_ops != NULL && dev->priv_ops->get_flags != NULL) {
		dev->priv_ops->get_flags(dev, flags, name);
	}
}

int ux_netdev_get_flags(const char *name, short int *flags)
{
	ux_netdev_t *dev = NULL;

	dev = ux_netdev_get_by_name(name);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	__ux_netdev_get_flags(dev, flags, name);
	ux_netdev_put(dev);

	return 0;
}

int ux_netdev_get_ipaddr(const char *name, struct sockaddr_in *sockaddr)
{
	ux_netdev_t *dev = NULL;

	dev = ux_netdev_get_by_name(name);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	if (dev->priv_ops != NULL && dev->priv_ops->get_ipaddr != NULL) {
		dev->priv_ops->get_ipaddr(dev, sockaddr, name);
	}

	ux_netdev_put(dev);

	/* not show ip information if ip = 0.0.0.0 */
	if (ntohl(sockaddr->sin_addr.s_addr) == INADDR_ANY) {
		return E_HM_INVAL;
	}
	return 0;
}

int ux_netdev_get_netmask(const char *name, struct sockaddr_in *sockaddr)
{
	ux_netdev_t *dev = NULL;

	dev = ux_netdev_get_by_name(name);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	if (dev->priv_ops != NULL && dev->priv_ops->get_netmask != NULL) {
		dev->priv_ops->get_netmask(dev, sockaddr, name);
	}

	ux_netdev_put(dev);

	return 0;
}

int ux_netdev_get_dstaddr(const char *name, struct sockaddr_in *sockaddr)
{
	ux_netdev_t *dev = NULL;

	dev = ux_netdev_get_by_name(name);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	if (dev->priv_ops != NULL && dev->priv_ops->get_dstaddr != NULL) {
		dev->priv_ops->get_dstaddr(dev, sockaddr);
	}

	ux_netdev_put(dev);

	return 0;
}

int ux_netdev_get_broadaddr(const char *name, struct sockaddr_in *sockaddr)
{
	ux_netdev_t *dev = NULL;
	struct sockaddr_in ipaddr;
	struct sockaddr_in maskaddr;

	dev = ux_netdev_get_by_name(name);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	mem_zero_s(ipaddr);
	mem_zero_s(maskaddr);
	(void)ux_netdev_get_ipaddr(name, &ipaddr);
	(void)ux_netdev_get_netmask(name, &maskaddr);

	sockaddr->sin_addr.s_addr = ipaddr.sin_addr.s_addr & maskaddr.sin_addr.s_addr;
	sockaddr->sin_addr.s_addr |= (~maskaddr.sin_addr.s_addr);

	ux_netdev_put(dev);
	return 0;
}

static int get_each_dev_stats(ux_netdev_t *dev, char *msg, unsigned int *file_bytes)
{
	netdev_for_each_entry(dev) {
		if (dev->priv_ops != NULL && dev->priv_ops->get_stats != NULL) {
			if (dev->priv_ops->get_stats(dev, MAX_PER_NETDEV_INFO_SIZE,
						     msg + *file_bytes) < 0) {
				net_error(HMNET_CORE, "get stats error.\n");
				return E_HM_NOMEM;
			}
			*file_bytes = (unsigned int)strlen(msg);
		}
	}
	return 0;
}

int ux_get_netdev_info(void *rvaddr, size_t mpool_size)
{
	ux_netdev_t *dev = NULL;
	char *msg = NULL;
	unsigned int msg_len = 0;
	unsigned int file_bytes = 0;
	unsigned int net_device_num = 0;
	int ret = 0;

	const char *head_msg = "Inter-|   Receive                            "
			       "                    |  Transmit\n"
			       " face |bytes    packets errs drop fifo frame "
			       "compressed multicast|bytes    packets errs "
			       "drop fifo colls carrier compressed\n";

	(void)raw_rwlock_rdlock(&ux_netdev_lock);

	/*
	 * The current maximum value of net_device_num is less than 5000, and
	 * this value is not user-controllable, so it won't lead to the
	 * allocation of large memory here.
	 */
	net_device_num = ux_get_netdev_nums();
	msg_len = MAX_PER_NETDEV_INFO_SIZE * net_device_num + NETDEV_INFO_HEADER_SIZE;
	msg = malloc(msg_len);
	if (msg == NULL) {
		(void)raw_rwlock_unlock(&ux_netdev_lock);
		return E_HM_NOMEM;
	}

	file_bytes += (HEAD_MSG_SIZE + PER_NETDEV_SIZE);

	if (snprintf_s(msg, msg_len, file_bytes, "%s", head_msg) < 0) {
		net_error(HMNET_CORE, "snprintf_s error.\n");
		ret = E_HM_NOMEM;
		goto out;
	}

	if ((ret = get_each_dev_stats(dev, msg, &file_bytes)) < 0) {
		goto out;
	}

	if (mpool_size <= file_bytes) {
		net_debug(HMNET_CORE, "msg too long\n");
		ret = E_HM_FBIG;
		goto out;
	}

	if (snprintf_s(rvaddr, file_bytes + 1, file_bytes, "%s", msg) < 0) {
		net_error(HMNET_CORE, "snprintf_s error.\n");
		ret = E_HM_NOMEM;
		goto out;
	}
	ret = (int)file_bytes;

out:
	free(msg);
	(void)raw_rwlock_unlock(&ux_netdev_lock);
	return ret;
}

static int ux_netdev_ifr64_copy_to_user(void *dst_ifr, struct ifreq *src_ifr, void **next_ifr)
{
	if (hm_actv_write_xact_vm(dst_ifr, src_ifr, sizeof(struct ifreq)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	*next_ifr = (struct ifreq *)dst_ifr + 1;
	return 0;
}

static int ux_netdev_ifr32_copy_to_user(void *dst_ifr, struct ifreq *src_ifr, void **next_ifr)
{
	struct ifreq32 ifr_tmp32 = {0};

	ux_ifreq2compat_ifreq_imperfectly(src_ifr, &ifr_tmp32);
	if (hm_actv_write_xact_vm(dst_ifr, &ifr_tmp32, sizeof(struct ifreq32)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	*next_ifr = (struct ifreq32 *)dst_ifr + 1;
	return 0;
}

int ux_netdev_ifr_copy_to_user(void *dst_ifr, struct ifreq *src_ifr, void **next_ifr)
{
	if (ux_is_compat_client()) {
		return ux_netdev_ifr32_copy_to_user(dst_ifr, src_ifr, next_ifr);
	}

	return ux_netdev_ifr64_copy_to_user(dst_ifr, src_ifr, next_ifr);
}

static void *ux_netdev_get_next_ifr(void *ifr, int cnt)
{
	if (ux_is_compat_client()) {
		return (struct ifreq32 *)ifr + cnt;
	}

	return (struct ifreq *)ifr + cnt;
}

static int ux_netdev_get_each_ifr(void *ifr, int ifr_cnt, int *count)
{
	int ret = 0;
	int cur_cnt = 0;
	int each_cnt = 0;
	ux_netdev_t *dev = NULL;

	netdev_for_each_entry(dev) {
		if (dev->priv_ops == NULL ||
		    dev->priv_ops->get_ifconf == NULL) {
		    continue;
		}

		/*
		* If ifc_req is NULL, SIOCGIFCONF returns the necessary buffer size in bytes
		* for receiving all available addresses in ifc_len.
		*/
		if (ifr != NULL && cur_cnt >= ifr_cnt) {
			cur_cnt = ifr_cnt;
			break;
		}

		ret = dev->priv_ops->get_ifconf(dev, ifr, ifr_cnt - cur_cnt, &each_cnt);
		if (ret != 0) {
			net_error(HMNET_CORE, "get ifconf failed\n");
			return ret;
		}

		cur_cnt += each_cnt;
		if (ifr != NULL) {
			ifr = ux_netdev_get_next_ifr(ifr, each_cnt);
		}
	}

	*count = cur_cnt;
	return 0;
}

int ux_netdev_get_compat_ifconf(struct ifconf32 *ifconf)
{
	struct ifreq32 *ifr = (void *)(uintptr_t)ifconf->ifc_ifcu.ifcu_req;
	int count = 0;
	int ifr_cnt = ifconf->ifc_len / (int)sizeof(struct ifreq32);

	(void)raw_rwlock_rdlock(&ux_netdev_lock);
	if (ux_netdev_get_each_ifr(ifr, ifr_cnt, &count) != 0) {
		(void)raw_rwlock_unlock(&ux_netdev_lock);
		return E_HM_INVAL;
	}

	ifconf->ifc_len = count * (int)sizeof(struct ifreq32);

	(void)raw_rwlock_unlock(&ux_netdev_lock);
	return 0;
}

int ux_netdev_get_ifconf(struct ifconf *ifconf)
{
	struct ifreq *ifr = ifconf->ifc_ifcu.ifcu_req;
	int count = 0;
	int ifr_cnt = ifconf->ifc_len / (int)sizeof(struct ifreq);

	(void)raw_rwlock_rdlock(&ux_netdev_lock);
	if (ux_netdev_get_each_ifr(ifr, ifr_cnt, &count) != 0) {
		(void)raw_rwlock_unlock(&ux_netdev_lock);
		return E_HM_INVAL;
	}

	ifconf->ifc_len = (int)(count * (int)sizeof(struct ifreq));

	(void)raw_rwlock_unlock(&ux_netdev_lock);
	return 0;
}

unsigned int ux_get_netdev_nums(void)
{
	ux_netdev_t *dev = NULL;
	unsigned int count = 0;

	netdev_for_each_entry(dev) {
		count++;
	}

	return count;
}

static int ux_netdev_find_ip_reachable(const ux_packet_demux_t *demux,
				       char **ifname, const struct sockaddr_in *rt_gw)
{
	if (demux == NULL || demux->find_ip_reachable == NULL) {
		net_error(HMNET_CORE, "find_ip_reachable operation is not supported.\n");
		return E_HM_NOSYS;
	}

	if (demux->find_ip_reachable(ifname, rt_gw->sin_addr.s_addr) == 0) {
		net_error(HMNET_CORE, "Gateway is not reachable.\n");
		return E_HM_NETUNREACH;
	}

	if (*ifname == NULL) {
		net_error(HMNET_CORE, "Can't get a valid ifname.\n");
		return E_HM_INVAL;
	}

	return 0;
}

static int ux_netdev_set_route_entry(const char *ifname, struct rtentry *rt, const ux_packet_demux_t *demux, int action)
{
	int ret;

	/* dev can be either passed explicitly(rt->rt_dev) or find by gateway(dev->name) */
	if (ifname != NULL) {
		ret = strncpy_s(rt->rt_dev, IFNAMSIZ, ifname, IFNAMSIZ - 1);
		if (ret != 0) {
			net_error(HMNET_CORE, "strncpy from ifname to rt->rt_dev failed.\n");
			return E_HM_POSIX_FAULT;
		}
	}

	if (demux == NULL || demux->set_route_entry == NULL) {
		net_error(HMNET_CORE, "set_route_entry operation is not supported.\n");
		return E_HM_NOSYS;
	}

	/* op: 0-add 1-del */
	return demux->set_route_entry(action, rt);
}

int ux_netdev_set_route(struct rtentry *rt, int op)
{
	ux_packet_demux_t *demux = NULL;
	char *ifname = NULL;
	struct sockaddr_in *rt_gw = NULL;
	struct sockaddr_in *rt_dst = NULL;
	struct sockaddr_in *rt_msk = NULL;
	int ret;

	if (rt == NULL) {
		return E_HM_INVAL;
	}

	rt_gw = (struct sockaddr_in *)&rt->rt_gateway;
	rt_dst = (struct sockaddr_in *)&rt->rt_dst;
	rt_msk = (struct sockaddr_in *)&rt->rt_genmask;

	demux = ux_packet_demux_lookup(ARPHRD_ETHER);

	if (rt->rt_dev != NULL && *(rt->rt_dev) != '\0') {
		ifname = rt->rt_dev;
	}

	if (rt_gw->sin_addr.s_addr != 0) {
		/* if op is add, check if the gateway is reachable */
		if (op == 0) {
			ret = ux_netdev_find_ip_reachable(demux, &ifname, rt_gw);
			if (ret < 0) {
				return ret;
			}
		}

		/* both destination ip and mask equal to 0 means set the default gateway */
		if (rt_dst->sin_addr.s_addr == 0 && rt_msk->sin_addr.s_addr == 0) {
			return ux_netdev_set_default(ifname, op, (struct sockaddr *)rt_gw);
		}
	}

	if ((rt->rt_flags & RTF_HOST) != 0U) {
		rt_msk->sin_addr.s_addr = htonl(0xffffffffUL);
	}

	/*
	 * netmask can't be zero if dest ip is set, it only happens when
	 * user set route to net and forget to set the netmask.
	 */
	if (rt_dst->sin_addr.s_addr != 0 && rt_msk->sin_addr.s_addr == 0) {
		return E_HM_INVAL;
	}

	return ux_netdev_set_route_entry(ifname, rt, demux, op);
}

int ux_netdev_ioctl_get(int cmd, struct ifreq *ifr)
{
	int ret;

	switch (cmd) {
	case SIOCGIFMTU:
		ret = ux_netdev_get_mtu(ifr->ifr_name, &(ifr->ifr_mtu));
		break;
	case SIOCGIFHWADDR:
	{
		unsigned short type = 0;
		ret = ux_netdev_get_type(ifr->ifr_name, &type);
		if (ret != 0) {
			break;
		}

		ifr->ifr_hwaddr.sa_family = (sa_family_t)type;
		ret = ux_netdev_get_mac(ifr->ifr_name, (unsigned char *)ifr->ifr_hwaddr.sa_data, MAC_ADDR_SIZE);
		break;
	}
	case SIOCGIFFLAGS:
		ret = ux_netdev_get_flags(ifr->ifr_name, &(ifr->ifr_flags));
		break;
	case SIOCGIFADDR:
		ret = ux_netdev_get_ipaddr(ifr->ifr_name, (struct sockaddr_in *)(&(ifr->ifr_addr)));
		break;
	case SIOCGIFNETMASK:
		ret = ux_netdev_get_netmask(ifr->ifr_name, (struct sockaddr_in *)(&(ifr->ifr_netmask)));
		break;
	case SIOCGIFDSTADDR:
		ret = ux_netdev_get_dstaddr(ifr->ifr_name, (struct sockaddr_in *)(&(ifr->ifr_dstaddr)));
		break;
	case SIOCGIFBRDADDR:
		ret = ux_netdev_get_broadaddr(ifr->ifr_name, (struct sockaddr_in *)(&(ifr->ifr_broadaddr)));
		break;
	case SIOCGIFMETRIC:
		ifr->ifr_metric = 0;
		ret = 0;
		break;
	case SIOCGIFMAP:
		ret = E_HM_OPNOTSUPP;
		break;
	case SIOCGIFTXQLEN:
		ifr->ifr_qlen = IFR_QLEN;
		ret = 0;
		break;
	default:
		ret = E_HM_INVAL;
		break;
	}

	return ret;
}

int ux_route_ioctl(int cmd, struct rtentry *rt)
{
	int ret;

	switch (cmd) {
	case SIOCADDRT:
		/* 0-set */
		ret = ux_netdev_set_route(rt, 0);
		break;
	case SIOCDELRT:
		/* 1-unset */
		ret = ux_netdev_set_route(rt, 1);
		break;
	default:
		ret = E_HM_INVAL;
		break;
	}

	return ret;
}

static int ux_netdev_set_dev_multi(const char *ifname, int request, const struct ifreq *ifr)
{
	ux_netdev_t *dev = NULL;
	int ret = E_HM_OPNOTSUPP;

	dev = ux_netdev_get_by_name(ifname);
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	if (dev->ops != NULL && dev->ops->ndo_ioctl != NULL) {
		hm_actv_set_accessible_mem_range(ptr_to_ulong(ifr), sizeof(*ifr));
		ret = dev->ops->ndo_ioctl(dev, request, (unsigned long)(uintptr_t)ifr, false);
	}

	ux_netdev_put(dev);

	return ret;
}

int ux_netdev_ioctl(int cmd, struct ifreq *ifr)
{
	int ret;

	switch (cmd) {
	case SIOCSIFMTU:
		ret = ux_netdev_set_mtu(ifr->ifr_name, ifr->ifr_ifru.ifru_mtu);
		break;
	case SIOCSIFHWADDR:
		ret = ux_netdev_set_mac(ifr->ifr_name, (unsigned char *)ifr->ifr_hwaddr.sa_data, MAC_ADDR_SIZE);
		break;
	case SIOCADDMULTI:
	case SIOCDELMULTI:
	case SIOCBONDENSLAVE:
	case SIOCBONDRELEASE:
	case SIOCBONDCHANGEACTIVE:
		ret = ux_netdev_set_dev_multi(ifr->ifr_name, cmd, ifr);
		break;
	case SIOCSIFFLAGS:
		ret = ux_netdev_set_flags(ifr->ifr_name, ifr->ifr_flags);
		break;
	case SIOCSIFADDR:
		ret = ux_netdev_set_ipaddr(ifr->ifr_name, &(ifr->ifr_addr));
		break;
	case SIOCSIFNETMASK:
		ret = ux_netdev_set_netmask(ifr->ifr_name, &(ifr->ifr_netmask));
		break;
	case SIOCSIFDSTADDR:
		ret = ux_netdev_set_dstaddr(ifr->ifr_name, &(ifr->ifr_dstaddr));
		break;
	case SIOCGIFINDEX:
		ret = ux_netdev_get_index_by_name(ifr->ifr_name, &ifr->ifr_ifindex);
		break;
	case SIOCGIFNAME:
		ret = ux_netdev_get_name_by_index(ifr->ifr_ifindex, ifr->ifr_name);
		break;
	default:
		/* 15: max private ioctl cmds offset */
		if ((cmd >= SIOCDEVPRIVATE) && cmd <= (SIOCDEVPRIVATE + 15)) {
			ret = ux_netdev_set_dev_multi(ifr->ifr_name, cmd, ifr);
		} else {
			ret = ux_netdev_ioctl_get(cmd, ifr);
		}
		break;
	}

	return ret;
}
