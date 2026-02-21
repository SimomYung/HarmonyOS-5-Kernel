/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: interface for network vlan
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 22 20:35:17 2021
 */

#include <lib/dlist.h>
#include <net/if_arp.h>
#include <netinet/if_ether.h>

#include "ux_netdev.h"
#include "ux_vlan.h"

static DLIST_HEAD(ux_vlan_list);
static struct raw_mutex ux_vlan_lock = RAW_MUTEX_INITIALIZER;
#define vlan_for_each_entry(pos) \
	dlist_for_each_entry(pos, &ux_vlan_list, ux_netdev_vlan_t, list)

static int vlan_ndo_set_mtu(ux_netdev_t *dev, int mtu)
{
	UNUSED(dev);
	UNUSED(mtu);

	return E_HM_OPNOTSUPP;
}

static int vlan_ndo_xmit(ux_netdev_t *dev, const void *data, int len, int flags)
{
	ux_netdev_t *realdev = NULL;
	ux_netdev_vlan_t *vlan = NULL;
	int ret;

	BUG_ON((dev->priv_flags & IFF_802_1Q_VLAN) == 0U);

	vlan = ux_netdev_priv_vlan(dev);

	ret = vlan_tag(dev, (void **)(&data), (int *)(&len), vlan->vid);
	if (ret < 0) {
		net_warn(HMNET_CORE, "vlan_tag failed\n");
		return ret;
	}

	realdev = vlan->realdev;
	if (realdev == NULL) {
		return E_HM_INVAL;
	}

	if (realdev->ops == NULL || realdev->ops->ndo_xmit == NULL) {
		return E_HM_OPNOTSUPP;
	}

	return ux_netdev_xmit(realdev, data, len, flags);
}

static ux_netdev_ops_t vlan_ops = {
	.ndo_init = NULL,
	.ndo_set_mtu = vlan_ndo_set_mtu,
	.ndo_ioctl = NULL,
	.ndo_xmit = vlan_ndo_xmit,
	.ndo_dump_stats = NULL,
	.ndo_get_master = NULL,
	.ndo_set_mac = NULL,
	.ndo_dellink = NULL,
	.ndo_add_slave = NULL,
	.ndo_del_slave = NULL,
};

static bool __vlan_check_exist(const ux_netdev_t *realdev, unsigned short vlan_id)
{
	bool exists = false;
	ux_netdev_vlan_t *vlan;

	vlan_for_each_entry(vlan) {
		if (vlan->realdev == realdev && vlan->vid == vlan_id) {
			exists = true;
			break;
		}
	}

	return exists;
}

static void __vlan_sync_mtu(ux_netdev_t *vlandev, ux_netdev_t *realdev)
{
	int mtu;
	int ret;

	ret = __ux_netdev_get_mtu(realdev, &mtu);
	if (ret != 0) {
		net_warn(HMNET_CORE, "Failed to get mtu on dev %s, ret: %d\n", realdev->name, ret);
		return;
	}

	ret = __ux_netdev_set_mtu(vlandev, mtu);
	if (ret != 0) {
		net_warn(HMNET_CORE, "Failed to set mtu on dev %s, ret: %d\n", realdev->name, ret);
		return;
	}

	return;
}

static void __vlan_sync_mac(ux_netdev_t *vlandev, ux_netdev_t *realdev)
{
	int ret = 0;
	unsigned char hwaddr[ETH_ALEN] = {0};

	if (realdev->priv_ops != NULL && realdev->priv_ops->get_mac != NULL) {
		ret = realdev->priv_ops->get_mac(realdev, hwaddr, sizeof(hwaddr));
		if (ret != 0) {
			net_warn(HMNET_CORE, "Failed to get mac address on dev %s\n", realdev->name);
			return;
		}
	}

	if (vlandev->priv_ops != NULL && vlandev->priv_ops->set_mac != NULL) {
		ret = vlandev->priv_ops->set_mac(vlandev, hwaddr, sizeof(hwaddr));
		if (ret != 0) {
			net_warn(HMNET_CORE, "Failed to set mac address on dev %s\n", vlandev->name);
			return;
		}
	}

	return;
}

static ux_netdev_t* __ux_vlan_netdev_create(ux_netdev_t *realdev,
					    unsigned short vlan_id,
					    int *err)
{
	char name[IFNAMSIZ];
	ux_netdev_t* dev = NULL;

	if (__vlan_check_exist(realdev, vlan_id)) {
		net_debug(HMNET_CORE, "vid %hu already exists on %s\n",
			  vlan_id, realdev->name);
		*err = E_HM_POSIX_EXIST;
		return NULL;
	}

	if (snprintf_s(name, IFNAMSIZ, IFNAMSIZ - 1, "%s.%u", realdev->name, vlan_id) < 0) {
		*err = E_HM_NOMEM;
		return NULL;
	}

	dev = ux_netdev_get_by_name(name);
	if (dev != NULL) {
		*err = E_HM_POSIX_EXIST;
		ux_netdev_put(dev);
		return NULL;
	}

	dev = ux_netdev_alloc(name, UX_NETDEV_PRIV_DEFAULT + sizeof(ux_netdev_vlan_t));
	if (dev == NULL) {
		*err = E_HM_NOMEM;
		return NULL;
	}

	dev->ops = &vlan_ops;
	dev->priv_flags |= IFF_802_1Q_VLAN;

	return dev;
}

#define VLAN_VID_MAX	4094
int vlan_add(const char *realdev_name, unsigned short vlan_id)
{
	ux_netdev_t *dev = NULL;
	ux_netdev_t *realdev = NULL;
	ux_netdev_vlan_t *vlan = NULL;
	int ret = 0;

	if (vlan_id > VLAN_VID_MAX) {
		ret = E_HM_RANGE;
		goto err_out;
	}

	realdev = ux_netdev_get_by_name(realdev_name);
	if (realdev == NULL) {
		ret = E_HM_NODEV;
		goto err_out;
	}

	/* QinQ is not support */
	if ((realdev->priv_flags & IFF_802_1Q_VLAN) != 0U) {
		ret = E_HM_INVAL;
		goto err_put;
	}

	raw_mutex_lock(&ux_vlan_lock);
	dev = __ux_vlan_netdev_create(realdev, vlan_id, &ret);
	if (dev == NULL) {
		goto err_unlock;
	}

	vlan = ux_netdev_priv_vlan(dev);
	vlan->dev = dev;
	/* hold a reference to realdev, do not netdev_put on success */
	vlan->realdev = realdev;
	vlan->vid = vlan_id;

	/* hold a reference to dev */
	ret = ux_netdev_register(dev);
	if (ret != 0) {
		net_error(HMNET_CORE, "ux_netdev_register failed\n");
		goto err_free;
	}

	/* sync realdev's mtu to vlandev */
	__vlan_sync_mtu(dev, realdev);

	/* sync realdev's mac to vlandev */
	__vlan_sync_mac(dev, realdev);

	/* insert to global vlan dev list */
	dlist_insert_tail(&ux_vlan_list, &vlan->list);
	raw_mutex_unlock(&ux_vlan_lock);

	return 0;
err_free:
	ux_netdev_free(dev);
err_unlock:
	raw_mutex_unlock(&ux_vlan_lock);
err_put:
	ux_netdev_put(realdev);
err_out:
	return ret;
}

int vlan_delete(const char *vlan_name)
{
	ux_netdev_t *dev = NULL;
	ux_netdev_vlan_t *vlan = NULL;

	raw_mutex_lock(&ux_vlan_lock);
	dev = ux_netdev_get_by_name(vlan_name);
	if (dev == NULL) {
		raw_mutex_unlock(&ux_vlan_lock);
		return E_HM_NODEV;
	}

	if ((dev->priv_flags & IFF_802_1Q_VLAN) == 0U) {
		ux_netdev_put(dev);
		raw_mutex_unlock(&ux_vlan_lock);
		return E_HM_INVAL;
	}

	/* remove from global vlan dev list */
	vlan = ux_netdev_priv_vlan(dev);
	dlist_delete(&vlan->list);

	/* release a reference to dev */
	ux_netdev_unregister(dev);
	raw_mutex_unlock(&ux_vlan_lock);

	ux_netdev_put(vlan->realdev);
	ux_netdev_put(dev);

	return 0;
}

ux_netdev_t *vlan_netdev_get(const ux_netdev_t *realdev, unsigned short vlan_id)
{
	ux_netdev_t *vlandev = NULL;
	ux_netdev_vlan_t *vlan = NULL;

	raw_mutex_lock(&ux_vlan_lock);
	vlan_for_each_entry(vlan) {
		if (vlan->realdev == realdev && vlan->vid == vlan_id) {
			vlandev = vlan->dev;
			ux_netdev_get(vlandev);
			break;
		}
	}
	raw_mutex_unlock(&ux_vlan_lock);

	return vlandev;
}

int vlan_tag(ux_netdev_t *dev, void **data, int *tot_len, unsigned short vlan_id)
{
	struct vlanhdr vlanhdr = {0};
	struct ethhdr ethhdr = {0};
	int ret;

	if (dev->priv_ops == NULL ||
	    dev->priv_ops->add_header == NULL ||
	    dev->priv_ops->remove_header == NULL) {
		return E_HM_OPNOTSUPP;
	}

	ret = dev->priv_ops->remove_header(dev, *data, (size_t)(unsigned int)(*tot_len),
					   (void *)&ethhdr, sizeof(struct ethhdr));
	if (ret < 0) {
		return ret;
	}

	vlanhdr.tp_proto = ethhdr.h_proto;
	vlanhdr.tp_vid = htons(vlan_id);
	ethhdr.h_proto = htons(ETHERTYPE_VLAN);

	ret = dev->priv_ops->add_header(dev, *data, (size_t)(unsigned int)(*tot_len), (void *)&vlanhdr,
					sizeof(struct vlanhdr));
	if (ret < 0) {
		return ret;
	}

	ret = dev->priv_ops->add_header(dev, *data, (size_t)(unsigned int)(*tot_len),
					(void *)&ethhdr, sizeof(struct ethhdr));
	if (ret < 0) {
		return ret;
	}

	*tot_len += VLAN_HLEN;
	return ret;
}

void vlan_untag(void **data, int *len)
{
	struct vlanhdr vlanhdr = {0};
	struct ethhdr ethhdr = {0};
	struct ethhdr *newhdr = NULL;

	ethhdr = *((struct ethhdr *)*data);
	vlanhdr = *((struct vlanhdr *)(void *)((char *)*data + ETH_HLEN));

	*data = (char *)*data + VLAN_HLEN;
	*len -= VLAN_HLEN;

	newhdr = (struct ethhdr *)*data;
	*newhdr = ethhdr;
	newhdr->h_proto = vlanhdr.tp_proto;
}
