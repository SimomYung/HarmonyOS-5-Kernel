/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Net device framework
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 16 10:45:12 2019
 */
#include "net.h"
#include <devhost/net.h>
#include <lib/utils.h>
#include <udk/api.h>
#include <udk/log.h>
#include <udk/slab.h>
#include <udk/sync.h>
#include <udk/iolib.h>
#include <udk/device.h>
#include <internal/device.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/bitops/generic.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>
#include <intapi/mem/mcache.h>

#include "queue_priv.h"

DEFINE_MCACHE_STATIC(udk_net_device, 60, sizeof(struct udk_net_device))

#define PBUF_SLAB_SIZE (16 * PAGE_SIZE)
#define PBUF_QUEUE_MAX_LEN	1000
#define PBUF_QUEUE_ONCE_LEN	64
#define NETDEV_NAME_OFFSET	1
#define NETDEV_TIMER_DEFAULT_INTERVAL	5000

int udk_netif_tx(struct pkt_buf *pbuf)
{
	int ret;
	struct udk_net_device *dev = NULL;
	struct udk_netdev_queue *queue = NULL;

	if (pbuf == NULL || pbuf->dev == NULL) {
		return E_HM_OK;
	}
	dev = pbuf->dev;
	queue = &dev->tx_queue;

	dev->dev_stats.tx_in++;

	ret = tx_enqueue(pbuf, queue);
	if (ret != 0) {
		udk_free_pbuf(pbuf);
		return E_HM_OK;
	}

	tx_queue_xmit(queue);

	return E_HM_OK;
}

int udk_netif_rx(struct pkt_buf *pbuf)
{
	int err;
	unsigned int shm_offset;

	if ((pbuf == NULL) || (pbuf->dev == NULL)) {
		udk_error("pbuf or pbuf->dev is null\n");
		return -EINVAL;
	}

	shm_offset = pbuf->shm_offset;
	pbuf->dev->dev_stats.rx_in++;
	err = devhost_native_net_rx(shm_offset, 0, (int)pbuf->len,
				    (int)pbuf->offset, pbuf->dev->dev_id);
	if (err < 0) {
		udk_free_pbuf(pbuf);
		udk_error("devhost_native_net_rx failed err:%s\n", hmstrerror(err));
		return -hmerrno2posix(err);
	}
	(void)udk_slab_unit_free(&pbuf->dev->pbuf_slab, pbuf);

	return 0;
}

int udk_netif_rx_batch(struct pkt_buf **pbuf_array, unsigned int pbuf_num)
{
	struct netdev_rx_cb rx_cb[MAX_RX_CB_NUM];
	unsigned int num = 0;
	unsigned int cnt = 0;
	unsigned int i;
	int err;

	if (pbuf_array == NULL) {
		udk_error("pbuf_array is null\n");
		return -EINVAL;
	}

	NOFAIL(memset_s(rx_cb, MAX_RX_CB_NUM * sizeof(struct netdev_rx_cb),
			0, MAX_RX_CB_NUM * sizeof(struct netdev_rx_cb)));
	while (num < pbuf_num) {
		if (pbuf_array[num] == NULL || pbuf_array[num]->dev == NULL) {
			udk_error("pbuf_array[%u] or pbuf_array[%u]->dev is null\n", num, num);
			err = E_HM_INVAL;
			goto err_out;
		}
		rx_cb[cnt].cell_pool_offset = pbuf_array[num]->shm_offset;
		rx_cb[cnt].rx_len = (int)pbuf_array[num]->len;
		rx_cb[cnt].offset = (int)pbuf_array[num]->offset;
		rx_cb[cnt].dev_idx = pbuf_array[num]->dev->dev_id;
		pbuf_array[num]->dev->dev_stats.rx_in++;

		cnt++;
		num++;

		if (cnt == MAX_RX_CB_NUM || num == pbuf_num) {
			err = devhost_native_net_rx_ex((int)cnt, rx_cb,
						       (int)(unsigned int)(sizeof(struct netdev_rx_cb) * cnt));
			if (err < 0) {
				goto err_out;
			}
			cnt = 0;
		}
	}

	for (cnt = 0; cnt < pbuf_num; cnt++) {
		(void)udk_slab_unit_free(&(pbuf_array[cnt]->dev->pbuf_slab), pbuf_array[cnt]);
	}

	return 0;
err_out:
	for (i = 0; i < num - cnt; i++) {
		(void)udk_slab_unit_free(&(pbuf_array[i]->dev->pbuf_slab), pbuf_array[i]);
	}
	for (cnt = num - cnt; cnt < pbuf_num; cnt++) {
		udk_free_pbuf(pbuf_array[cnt]);
	}
	udk_error("devhost_native_net_rx_ex failed err:%s\n", hmstrerror(err));
	return -hmerrno2posix(err);
}

static void netdev_poll_queue(unsigned long para)
{
	struct udk_net_device *dev = ulong_to_ptr(para, struct udk_net_device);
	struct udk_netdev_queue *queue = &dev->tx_queue;

	tx_queue_xmit(queue);
}

static int setup_netdev_timer(struct udk_net_device *dev)
{
	int ret;

	if (dev->time_interval == 0) {
		dev->time_interval = NETDEV_TIMER_DEFAULT_INTERVAL;
	}
	udk_timer_init(&dev->poll_timer, netdev_poll_queue, ptr_to_ulong(dev), dev->time_interval);
	udk_timer_setflags(&dev->poll_timer, UDK_TIMER_FLAGS_PERIODIC);

	ret = udk_timer_add(&dev->poll_timer);
	if (ret < 0) {
		udk_error("add netdev timer failed. name=%s, time_interval= %lu\n",
			  dev->name, dev->time_interval);
	}

	return ret;
}

int udk_netdev_open(struct udk_net_device *dev)
{
	int ret = 0;
	const struct udk_netdev_ops *ops = NULL;

	if (dev == NULL) {
		return -EINVAL;
	}

	udk_mutex_lock(&dev->dev_lock);
	ops = dev->netdev_ops;
	if (dev->state == UDK_NETDEV_OPEN) {
		udk_mutex_unlock(&dev->dev_lock);
		return ret;
	}
	if (!((dev->state == UDK_NETDEV_ADD) || (dev->state == UDK_NETDEV_CLOSE))) {
		udk_error("open error: name=%s state=%u\n", dev->name, dev->state);
		udk_mutex_unlock(&dev->dev_lock);
		return -ENODEV;
	}

	if (ops->open != NULL) {
		ret = ops->open(dev);
		if (ret != 0) {
			udk_error("open failed. err: %s\n", strerror(-ret));
			udk_mutex_unlock(&dev->dev_lock);
			return ret;
		}
	}

	ret = setup_netdev_timer(dev);
	if (ret != 0) {
		udk_error("setup_netdev_timer failed. err: %s\n", strerror(-ret));
		if (ops->stop != NULL) {
			ret = ops->stop(dev);
			if (ret != 0) {
				udk_error("stop failed. err: %s\n", strerror(-ret));
			}
		}
		udk_mutex_unlock(&dev->dev_lock);
		return ret;
	}

	udk_netdev_start_queue(dev);
	dev->state = UDK_NETDEV_OPEN;
	udk_mutex_unlock(&dev->dev_lock);

	return E_HM_OK;
}

static void close_netdev(struct udk_net_device *dev)
{
	int ret;
	const struct udk_netdev_ops *ops = dev->netdev_ops;

	if (dev->state == UDK_NETDEV_OPEN) {
		udk_netdev_stop_queue(dev);

		ret = udk_timer_del_sync(&dev->poll_timer);
		if (ret != 0) {
			/* do not care the delete failure */
			udk_error("delete dev timer err, name=%s, err: %s\n",
				  dev->name, strerror(-ret));
		}

		if (ops->stop != NULL) {
			ret = ops->stop(dev);
			if (ret != 0) {
				/* do not care the stop failure */
				udk_debug("stop failed. err: %s\n", strerror(-ret));
			}
		}
		dev->state = UDK_NETDEV_CLOSE;
	}
}

int udk_netdev_close(struct udk_net_device *dev)
{
	if (dev == NULL) {
		return -EINVAL;
	}

	udk_mutex_lock(&dev->dev_lock);
	close_netdev(dev);
	udk_mutex_unlock(&dev->dev_lock);

	return E_HM_OK;
}

static int netdev_event(unsigned long event, struct udk_net_device *dev)
{
	int ret;

	switch (event) {
	case UDK_NETDEV_REGISTER:
		/*
		 * The device is initialized with udk_device_init,
		 * safe for __udk_device_devinfo_of
		 */
		dev->dev_id = 0; /* use dynamic ifindex */
		ret = devhost_register_netdev(__udk_device_devinfo_of(&dev->udk_dev),
					      dev->name, 0, &dev->dev_id);
		if (ret < 0) {
			udk_debug("netdev_event netdev failed, name=%s\n", dev->name);
			return ret;
		}
		break;
	case UDK_NETDEV_UNREGISTER:
		devhost_unregister_netdev(dev->dev_id);
		break;
	default:
		udk_debug("Unhandled event=%lu.\n", event);
		break;
	}

	return E_HM_OK;
}

static void unregister_netdevice(struct udk_net_device *dev)
{
	int ret;

	if (dev->state == UDK_NETDEV_DELETE) {
		udk_error("netdevice was not added or already delete, name=%s\n",
			  dev->name);
		return;
	}

	close_netdev(dev);
	ret = netdev_event(UDK_NETDEV_UNREGISTER, dev);
	if (ret < 0) {
		udk_error("dev unregister error, name=%s, err: %s\n", dev->name, strerror(-ret));
	}
	if (dev->netdev_ops->uninit != NULL) {
		dev->netdev_ops->uninit(dev);
	}
	udk_slab_destroy(&dev->pbuf_slab);
	dev->state = UDK_NETDEV_DELETE;
}

static void netdev_init_queue(struct udk_net_device *dev)
{
	dev->tx_queue.state = UDK_QUEUE_STATE_OFF;
	dlist_init(&dev->tx_queue.pbuf_list);
}

int udk_netdev_add(struct udk_net_device *dev)
{
	int ret;

	if (dev == NULL) {
		udk_error("dev is null\n");
		return -EINVAL;
	}

	udk_mutex_lock(&dev->dev_lock);
	if (dev->state != UDK_NETDEV_INITIALIZE) {
		udk_error("add udk device state err, name=%s, state=%u\n",
			  dev->name, dev->state);
		udk_mutex_unlock(&dev->dev_lock);
		return -EINVAL;
	}

	ret = udk_slab_init(&dev->pbuf_slab, PBUF_SLAB_SIZE, sizeof(struct pkt_buf));
	if (ret != 0) {
		udk_error("failed to init udk slab in device%s\n", dev->name);
		udk_mutex_unlock(&dev->dev_lock);
		return ret;
	}

	udk_device_capable_net(&dev->udk_dev, &udk_net_ops);

	if (dev->netdev_ops != NULL && dev->netdev_ops->init != NULL) {
		ret = dev->netdev_ops->init(dev);
		if (ret != 0) {
			udk_slab_destroy(&dev->pbuf_slab);
			udk_mutex_unlock(&dev->dev_lock);
			return ret;
		}
	}

	ret = udk_device_add(&dev->udk_dev);
	if (ret != 0) {
		udk_error("failed to register native device %s\n", dev->name);
		udk_slab_destroy(&dev->pbuf_slab);
		udk_mutex_unlock(&dev->dev_lock);
		return ret;
	}

	ret = netdev_event(UDK_NETDEV_REGISTER, dev);
	if (ret != 0) {
		unregister_netdevice(dev); /* slab destroy is included in unregister */
		udk_mutex_unlock(&dev->dev_lock);
		return ret;
	}

	netdev_init_queue(dev);
	dev->state = UDK_NETDEV_ADD;
	udk_mutex_unlock(&dev->dev_lock);

	return 0;
}

void udk_netdev_delete(struct udk_net_device *dev)
{
	if (dev == NULL) {
		udk_error("dev is null\n");
		return;
	}

	udk_mutex_lock(&dev->dev_lock);
	unregister_netdevice(dev);
	udk_mutex_unlock(&dev->dev_lock);
}

static DEFINE_RAW_BITMAP(g_devname_id_bitmap, NET_DEV_SIZE);

static int check_devname(const char *name, struct udk_net_device *dev)
{
	int err;
	const char *p = name;
	const char *tmp = NULL;
	unsigned int id = NET_DEV_SIZE;
	char buf[UDK_NAME_SIZE];

	if (*name == '\0' || strlen(name) >= sizeof(dev->name) ||
	    strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
		udk_error("invalid dev name=%s\n", name);
		return E_HM_INVAL;
	}

	if ((tmp = strchr(name, '%')) != NULL) {
		/* to check the input name format must contains only one %d */
		if ((tmp[NETDEV_NAME_OFFSET] != 'd') ||
		    (strchr(tmp + NETDEV_NAME_OFFSET + 1, '%') != NULL)) {
			udk_error("err format. invalid dev name=%s \n", name);
			return E_HM_INVAL;
		}

		id = raw_bitmap_find_first_zero(g_devname_id_bitmap, NET_DEV_SIZE);
		err = raw_bitmap_set_bit(g_devname_id_bitmap, NET_DEV_SIZE, id);
		if (err < 0) {
			udk_error("invalid dev id=%u, err:%s \n", id, hmstrerror(err));
			return E_HM_INVAL;
		}

		err = snprintf_s(buf, UDK_NAME_SIZE, UDK_NAME_SIZE - 1, name, id);
		if (err < 0) {
			udk_error("snprintf_s name=%s err: %d \n", name, err);
			err = raw_bitmap_clear_bit(g_devname_id_bitmap, NET_DEV_SIZE, id);
			if (err < 0) {
				udk_warn("clear bit failed, id=%u\n", id);
			}
			return E_HM_INVAL;
		}
		p = buf;
	}

	NOFAIL(strncpy_s(dev->name, sizeof(dev->name), p, strlen(p)));
	dev->name_id = id;

	udk_debug("dev name=%s dev->name_id=%u\n", dev->name, dev->name_id);

	return 0;
}

static int setup_netdev_after_alloc(struct udk_net_device *dev,
				    const char *name,
				    void (*setup)(struct udk_net_device *))
{
	int ret;

	ret = check_devname(name, dev);
	if (ret < 0) {
		udk_error("check_devname fail. name=%s\n", name);
		return E_HM_INVAL;
	}

	if (setup != NULL) {
		setup(dev);
	}

	dev->state = UDK_NETDEV_INITIALIZE;
	udk_mutex_init(&dev->flags_lock);
	udk_mutex_init(&dev->dev_lock);
	udk_mutex_init(&dev->tx_queue.queue_lock);
	udk_mutex_init(&dev->tx_queue.xmit_lock);

	return ret;
}

static void udk_netdev_release(struct udk_device *dev)
{
	struct udk_net_device *netdev = container_of(dev, struct udk_net_device, udk_dev);
	if (netdev->name_id != NET_DEV_SIZE) {
		int err = raw_bitmap_clear_bit(g_devname_id_bitmap, NET_DEV_SIZE, netdev->name_id);
		if (err < 0) {
			udk_error("invalid dev, id=%u err=%s\n", netdev->name_id, strerror(-err));
		}
	}
	mcache_udk_net_device_free(netdev);
}

struct udk_net_device *udk_netdev_alloc(const char *name,
					void (*setup)(struct udk_net_device *))
{
	int ret;
	struct udk_net_device *dev = NULL;

	if (name == NULL) {
		udk_error("netdev name is null\n");
		return NULL;
	}

	dev = (struct udk_net_device *)mcache_udk_net_device_malloc();
	if (dev == NULL) {
		udk_error("udk_malloc fail\n");
		return NULL;
	}
	mem_zero_s(*dev);

	ret = setup_netdev_after_alloc(dev, name, setup);
	if (ret != 0) {
		udk_error("setup_netdev_after_alloc fail. devname=%s\n", name);
		udk_free(dev);
		return NULL;
	}

	/*
	 * init dev flags here, udk framework don't support multiple flags such as
	 * broadcast and promisc, so set dev->flags to 0 is ok.
	 */
	dev->flags = 0;
	dev->udk_dev.name = dev->name;
	ret = udk_device_init(&dev->udk_dev);
	if (ret < 0) {
		udk_error("failed to init udk device, name=%s\n", dev->name);
		udk_netdev_release(&dev->udk_dev);
		return NULL;
	}
	dev->udk_dev.release = udk_netdev_release;

	return dev;
}

void udk_netdev_default_setup(struct udk_net_device *dev)
{
	if (dev == NULL) {
		return;
	}

	dev->tx_queue.pbuf_max      = PBUF_QUEUE_MAX_LEN;
	dev->tx_queue.pbuf_cnt_once = PBUF_QUEUE_ONCE_LEN;
}

void udk_netdev_free(struct udk_net_device *dev)
{
	if (dev == NULL) {
		udk_error("dev is null\n");
		return;
	}

	udk_mutex_lock(&dev->dev_lock);
	if (!((dev->state == UDK_NETDEV_INITIALIZE) || (dev->state == UDK_NETDEV_DELETE))) {
		udk_error("first close and delete device: name=%s\n", dev->name);
		udk_mutex_unlock(&dev->dev_lock);
		return;
	}
	udk_mutex_unlock(&dev->dev_lock);

	udk_device_reset(&dev->udk_dev);
}

int udk_netdev_set_random_mac_addr(struct udk_net_device *dev)
{
	int ret;

	if (dev == NULL) {
		return -EINVAL;
	}

	ret = udk_get_random_bytes(dev->mac_addr, UDK_MAC_ADDR_SIZE);
	if (ret < 0) {
		return ret;
	}
	/* randomized mac address is unicast and locally administered */
	dev->mac_addr[0] &= ~UDK_MAC_ADDR_GRP_FLAG;
	dev->mac_addr[0] |= UDK_MAC_ADDR_LOCAL_FLAG;

	return 0;
}
