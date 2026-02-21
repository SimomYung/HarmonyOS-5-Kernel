/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Structures and defines of net device
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 12 19:11:36 2019
 */
#ifndef __UDK_NETDEV_H__
#define __UDK_NETDEV_H__

#include <udk/sync.h>
#include <udk/compiler.h>
#include <udk/device.h>
#include <udk/timer.h>
#include <udk/net/pbuf.h>
#include <udk/net/ethtool.h>
#include <udk/slab.h>

#define	UDK_MAC_ADDR_GRP_FLAG	1
#define	UDK_MAC_ADDR_LOCAL_FLAG	(1U << 1)
#define	UDK_MAC_ADDR_SIZE	6
#define	UDK_NAME_SIZE		16
#define NET_DEV_BITS		4
#define NET_DEV_SIZE		(1 << NET_DEV_BITS)

enum udk_netdev_queue_state_t {
	UDK_QUEUE_STATE_OFF = 1,
	UDK_QUEUE_STATE_ON,
};

enum udk_netdev_reg_state_t {
	UDK_NETDEV_UNREGISTER = 1,
	UDK_NETDEV_REGISTER,
};

enum udk_netdev_tx {
	UDK_NETDEV_TX_OK     = 0,
	UDK_NETDEV_TX_BUSY   = 1,
};

/*
 * UDK_NETDEV_INITIALIZE means the initial netdev state when alloced by user
 * calling udk_netdev_alloc.
 * UDK_NETDEV_ADD means netdev has already added to devhost, it's previos state
 * must be UDK_NETDEV_INITIALIZE.
 * UDK_NETDEV_OPEN means the netdev is already opened,  it's previos state
 * should be UDK_NETDEV_ADD or UDK_NETDEV_CLOSE, when netdev is opend now,
 * return ok directly.
 * UDK_NETDEV_CLOSE means the netdev is already closed, when netdev is not
 * opend now, return ok directly.
 * UDK_NETDEV_DELETE means the netdev is already deleted, when netdev is
 * deleted now, report err and return.
 */
enum udk_netdev_state_t {
	UDK_NETDEV_INITIALIZE = 1,
	UDK_NETDEV_ADD,
	UDK_NETDEV_OPEN,
	UDK_NETDEV_CLOSE,
	UDK_NETDEV_DELETE,
};

struct udk_netdev_queue {
	unsigned int		state;
	unsigned int		pbuf_num;
	/* the max pbuf num to send per tx_dequeue() */
	unsigned int		pbuf_cnt_once;
	unsigned int		pbuf_max;
	struct dlist_node	pbuf_list;
	struct udk_mutex	queue_lock;
	struct udk_mutex	xmit_lock;
};

struct udk_netdrv_statistics {
	unsigned long	rx_pkts;
	unsigned long	tx_pkts;
	unsigned long	rx_bytes;
	unsigned long	tx_bytes;
	unsigned long	rx_errors;
	unsigned long	tx_errors;
	unsigned long	rx_drop;
	unsigned long	tx_drop;
};

struct udk_netdev_statistics {
	/* pkts info in net layer */
	unsigned long	rx_in;
	unsigned long	tx_in;
	unsigned long	full_drop;
	unsigned long	deact_drop;

	/* unit memory used info */
	unsigned long	drv_alloc;
	unsigned long	drv_free;
	unsigned long	devhost_alloc;
	unsigned long	devhost_free;
};

struct udk_net_device {
	unsigned int		state;
	int			irq;
	int			dev_id;
	unsigned int		flags;
	unsigned int		name_id;
	unsigned long		time_interval;
	uintptr_t		addr;
	void			*private;
	char			name[UDK_NAME_SIZE];
	unsigned char		mac_addr[UDK_MAC_ADDR_SIZE];

	struct udk_mutex	flags_lock;
	struct udk_mutex	dev_lock;
	struct udk_device	udk_dev;
	struct udk_timer	poll_timer;

	struct udk_netdev_queue tx_queue;
	struct udk_slab pbuf_slab;

	/* net layer to statistic */
	struct udk_netdev_statistics	dev_stats;
	/* driver to statistic */
	struct udk_netdrv_statistics	drv_stats;

	const struct udk_netdev_ops	*netdev_ops;
	const struct udk_ethtool_ops	*ethtool_ops;
};

struct udk_ethtool_drvinfo;
struct udk_ethtool_link_ksettings;

struct udk_ethtool_ops {
	int	(*pre)(struct udk_net_device *dev);
	void	(*done)(struct udk_net_device *dev);
	void	(*get_driver_info)(struct udk_net_device *dev, struct udk_ethtool_drvinfo *drvinfo);
	int	(*get_info_nums)(struct udk_net_device *dev, int sset);
	int	(*get_eeprom_datalen)(struct udk_net_device *dev);
	int	(*get_regs_datalen)(struct udk_net_device *dev);
	int	(*get_link_ksettings)(struct udk_net_device *dev, struct udk_ethtool_link_ksettings *ksettings);
	int	(*set_link_ksettings)(struct udk_net_device *dev, struct udk_ethtool_link_ksettings *ksettings);
	int	(*nway_reset)(struct udk_net_device *dev);
};

struct udk_netdev_ops {
	int	(*init)(struct udk_net_device *dev);
	void	(*uninit)(struct udk_net_device *dev);
	int	(*open)(struct udk_net_device *dev);
	int	(*stop)(struct udk_net_device *dev);
	int	(*start_xmit)(struct pkt_buf *pbuf,
			      struct udk_net_device *dev);
	int	(*set_mac_address)(struct udk_net_device *dev,
				   void *addr);
	int	(*do_ioctl)(struct udk_net_device *dev, void *hm_ifr, int cmd);
	void	(*tx_timeout)(struct udk_net_device *dev);
	void	(*show_net_status)(struct udk_net_device *dev);
	void	(*change_rx_flags)(struct udk_net_device *dev, int flags);
	int	(*set_multicast_list)(struct udk_net_device *dev, const char *addr, bool add);
	void	**owner;
};

__maybe_unused
static void udk_netdev_start_queue(struct udk_net_device *dev)
{
	struct udk_netdev_queue *queue = &dev->tx_queue;
	struct udk_mutex *q_lock = &queue->queue_lock;

	udk_mutex_lock(q_lock);
	if (dev->tx_queue.state != UDK_QUEUE_STATE_ON) {
		dev->tx_queue.state = UDK_QUEUE_STATE_ON;
		dlist_init(&dev->tx_queue.pbuf_list);
	}
	udk_mutex_unlock(q_lock);
}

__maybe_unused
static void udk_netdev_stop_queue(struct udk_net_device *dev)
{
	struct udk_netdev_queue *queue = &dev->tx_queue;
	struct udk_mutex *q_lock = &queue->queue_lock;

	udk_mutex_lock(q_lock);
	if (dev->tx_queue.state != UDK_QUEUE_STATE_OFF) {
		dev->tx_queue.state = UDK_QUEUE_STATE_OFF;
		dev->tx_queue.pbuf_num = 0;
		free_pbuf_list(&(dev->tx_queue.pbuf_list));
	}
	udk_mutex_unlock(q_lock);
}

static inline int udk_netdev_is_working(const struct udk_net_device *dev)
{
	return dev->state == UDK_NETDEV_OPEN;
}

__maybe_unused
static bool udk_check_mac_addr(const unsigned char *addr)
{
	unsigned char val;

	if (addr == NULL) {
		return false;
	}

	val = *addr;
	if ((val & UDK_MAC_ADDR_GRP_FLAG) != 0) {
		return false;
	}

	val = 0;
	for (int i = 0; i < UDK_MAC_ADDR_SIZE; i++) {
		val |= *(addr + i);
	}
	if (val == 0) {
		return false;
	}

	return true;
}

/* rx */
int udk_netif_rx(struct pkt_buf *pbuf);
int udk_netif_rx_batch(struct pkt_buf **pbuf_array, unsigned int pbuf_num);
int udk_netif_tx(struct pkt_buf *pbuf);

/* alloc and free net device */
struct udk_net_device *udk_netdev_alloc(const char *name,
					void (*setup)(struct udk_net_device *));
void udk_netdev_free(struct udk_net_device *dev);
void udk_netdev_default_setup(struct udk_net_device *dev);

/* add and delete net device */
int udk_netdev_add(struct udk_net_device *dev);
void udk_netdev_delete(struct udk_net_device *dev);
int udk_netdev_set_random_mac_addr(struct udk_net_device *dev);

#endif /* __UDK_NETDEV_H__ */
