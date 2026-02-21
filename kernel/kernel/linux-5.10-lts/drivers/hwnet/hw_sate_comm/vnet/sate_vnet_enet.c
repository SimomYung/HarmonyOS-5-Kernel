/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
 * Description: This module is used to start vcom between ril and kernel
 * Author: baijiaxuan2@huawei.com
 * Create: 2024-12-06
 */
#include <net/sock.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/capability.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <securec.h>
#include "sate_vnet_log.h"

#define VNETNAMESIZE 16
#define PDCP_INFO_LEN 8

unsigned int sate_log_level = SATE_LOG_LVL_HIGH | SATE_LOG_LVL_ERR;
module_param(sate_log_level, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(sate_log_level, "sate vnet log level");

static DECLARE_WAIT_QUEUE_HEAD(sate_wq_head);

static struct net_device *sate_vnet_dev = NULL;

static struct sk_buff_head tx_list;
static int g_byteRemain = 0;
static int g_sendSpeed = 0;

struct sate_vnet_ioctl_hdlr_s {
	int cmd;
	int (*func)(void);
};

static const struct sate_vnet_ioctl_hdlr_s sate_vnet_ioctl_hdlrs[] = {
};

/***********************
 * file_operations->open
 ***********************/
static int sate_proc_open(struct inode *inode, struct file *file)
{
	if (inode == NULL)
		return -1;
	if (file == NULL)
		return -1;
	SATE_LOGD("%s done \n", __func__);
	return 0;
}

static void sate_wake_queue(void)
{
	wake_up_interruptible(&sate_wq_head);
}

static void sate_vnet_set_ethhdr(struct sk_buff *skb, struct net_device *netdev)
{
	struct ethhdr *ethh = (struct ethhdr *)(skb->data - ETH_HLEN);

	ether_addr_copy(ethh->h_dest, netdev->dev_addr);
	ether_addr_copy(ethh->h_source, netdev->dev_addr);
	ethh->h_proto = skb->protocol;
}

static void recv_vnet_speed(struct msg_head *vnetMsg, struct sate_vnet_priv_s *priv)
{
    g_sendSpeed = *(int *)vnetMsg->value;
    priv->speed = g_sendSpeed;
    SATE_LOGI("%s: recv send speed: %d", __func__, g_sendSpeed);
}

static int recv_vnet_pdcp(struct msg_head *vnetMsg, struct sate_vnet_priv_s *priv)
{
    int pdcpNum = 0;
    int id = 0;
    int *pdcpInfo;
    pdcpInfo = (int *)vnetMsg->value;
    if (pdcpInfo == NULL || vnetMsg->len < PDCP_INFO_LEN) {
        SATE_LOGE("%s: pdcp info err", __func__);
        return -1;
    }
    pdcpNum = pdcpInfo[0];
    id = pdcpInfo[1];
    if (priv->sendPdcpResult != NULL) {
        SATE_LOGI("%s: pdcp callback registered, recv pdcp result: %d, %d", __func__, pdcpNum, id);
        priv->sendPdcpResult(pdcpNum, id);
    }
    return 0;
}

static void recv_vnet_byte(struct msg_head *vnetMsg, struct sate_vnet_priv_s *priv)
{
    g_byteRemain = *(int *)vnetMsg->value;
    priv->byteRemain = g_byteRemain;
    SATE_LOGI("%s: recv byte remain: %d", __func__, g_byteRemain);
}

static void recv_vnet_rx(struct msg_head *vnetMsg)
{
	SATE_LOGI("%s: recv rx len: %d, count: %d", __func__, vnetMsg->len);
	struct sk_buff *skb;
	skb = alloc_skb(vnetMsg->len + 50, GFP_ATOMIC);
    if (skb == NULL) {
        SATE_LOGE("%s: alloc_skb fail", __func__);
        return;
    }
    skb_put(skb, vnetMsg->len);
    if (!memcpy_s(skb->data, vnetMsg->len, vnetMsg->value, vnetMsg->len)) {
        SATE_LOGE("%s: skb data copy fail", __func__);
        return;
    }

	skb->dev = sate_vnet_dev;
	skb->protocol = (skb->data[0] & SATE_IPVER_MASK) == SATE_IPVER_4 ?
		htons(ETH_P_IP) : htons(ETH_P_IPV6);

	sate_vnet_set_ethhdr(skb, sate_vnet_dev);
	skb_set_mac_header(skb, -ETH_HLEN);
	skb_set_network_header(skb, 0);
	skb->pkt_type = PACKET_HOST;

	int ret = netif_rx_ni(skb);
    if (ret == 0) {
		sate_vnet_dev->stats.rx_packets++;
		sate_vnet_dev->stats.rx_bytes += vnetMsg->len;
		SATE_LOGD("%s: sate_proc_write rx pkg succ", __func__);
	} else {
		SATE_LOGE("%s: sate_proc_write rx pkg err, ret: %d", __func__, ret);
	}
}

/************************
 * file_operations->write
 ************************/
static ssize_t sate_proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *f_pos)
{
	int pdcpNum = 0;
	int id = 0;
	int *pdcpInfo;
    char *tmp = kmalloc((count + 1), GFP_ATOMIC);
    if (!tmp)
        return -ENOMEM;

    if (copy_from_user(tmp, buffer, count)) {
        kfree(tmp);
        return -EFAULT;
    }
	struct sate_vnet_priv_s *priv = netdev_priv(sate_vnet_dev);

	struct msg_head *vnetMsg = (struct msg_head *)tmp;
	switch (vnetMsg->type) {
        case VNET_MSG_SPEED:
			recv_vnet_speed(vnetMsg, priv);
            break;
        case VNET_MSG_PDCP:
			if (recv_vnet_pdcp(vnetMsg, priv)) {
				kfree(vnetMsg);
				kfree(tmp);
                return count;
            }
            break;
		case VNET_MSG_BYTE:
			recv_vnet_byte(vnetMsg, priv);
			break;
		case VNET_MSG_RX:
			recv_vnet_rx(vnetMsg);
			break;
		default:
			break;
	}
	kfree(vnetMsg);
	kfree(tmp);
    return count;
}

static ssize_t sate_proc_read(struct file *file, char __user *buffer, size_t count, loff_t *f_pos)
{
	struct sk_buff *skbn = NULL;
	int len = 0;

	if (skb_queue_empty(&tx_list)) {
		SATE_LOGE("%s: skb queue is empty", __func__);
		buffer = NULL;
		return 0;
	}

    struct sk_buff *skb = skb_dequeue(&tx_list);
	if (skb) {
		skbn = skb_clone(skb, GFP_ATOMIC);
		if (skbn == NULL) {
			goto err;
		}
		int ret = copy_to_user(buffer, skbn->data, skbn->len);
		if (!ret) {
			goto err;
		}
		SATE_LOGI("%s: sate_proc_read send skb, ret: %d, len: %d", __func__, ret, skbn->len);
		len = skbn->len;
	}
	
    return len;

err:
	SATE_LOGE("%s: sate_proc_read fail", __func__);
	return 0;
}

static unsigned int sate_proc_poll(struct file *file, struct poll_table_struct *poll_table)
{
    unsigned int mask = 0;

	poll_wait(file, &sate_wq_head, poll_table);
	if (!skb_queue_empty(&tx_list)) {
		SATE_LOGD("skb queue not empty, wake poll");
        mask = POLLIN | POLLRDNORM;
    }

    return mask;
}

static int sate_proc_release(struct inode *inode, struct file *filp)
{
	if (inode == NULL)
		return -1;
	SATE_LOGD("%s done\n", __func__);
	return 0;
}

static struct proc_ops sate_fops = {
    .proc_open	= sate_proc_open,
    .proc_release = sate_proc_release,
    .proc_read	= sate_proc_read,
    .proc_write 	= sate_proc_write,
	.proc_poll = sate_proc_poll,
};

static int __init sate_init_proc(void)
{
    struct proc_dir_entry* file;

    file = proc_create("sate_vnet", 0777, NULL, &sate_fops);
    if (!file)
        return -ENOMEM;

    return 0;
}

static void __exit sate_exit_proc(void)
{
    remove_proc_entry("sate_vnet", NULL);
}

static const struct sate_vnet_ioctl_hdlr_s *sate_vnet_ioctl_get_handler(int cmd)
{
	u32 i;

	for (i = 0; i < ARRAY_SIZE(sate_vnet_ioctl_hdlrs); i++) {
		if (cmd == sate_vnet_ioctl_hdlrs[i].cmd)
			return &sate_vnet_ioctl_hdlrs[i];
	}

	return NULL;
}

static int sate_vnet_open(struct net_device *netdev)
{
	netif_start_queue(netdev);
	return 0;
}

static int sate_vnet_stop(struct net_device *netdev)
{
	netif_stop_queue(netdev);
	return 0;
}

static netdev_tx_t sate_vnet_start_xmit(struct sk_buff *skb, struct net_device *netdev)
{
	if (skb == NULL) {
        return NETDEV_TX_BUSY;
    }
	netif_stop_queue(netdev);

	struct sk_buff *skbn = NULL;
	skbn = skb_clone(skb, GFP_ATOMIC);

	netdev->stats.tx_packets++;
    netdev->stats.tx_bytes += skb->len;

	if (skbn) {
		skb_queue_tail(&tx_list, skbn);
        SATE_LOGI("%s: sate_vnet_start_xmit ok, len: %d", __func__, skb->len);
	}
    sate_wake_queue();
    netif_wake_queue(netdev);

	return NETDEV_TX_OK;
}

static int sate_vnet_ioctl_handler(int cmd)
{
	const struct sate_vnet_ioctl_hdlr_s *handler = NULL;

	handler = sate_vnet_ioctl_get_handler(cmd);
	if (handler == NULL) {
		SATE_LOGE("unsupported cmd %d", cmd);
		return -EOPNOTSUPP;
	}

	int ret = handler->func();
	if (!ret) {
		SATE_LOGE("copy to priv ioctl data failed");
		ret = -EFAULT;
	}

	return ret;
}

static int sate_vnet_do_ioctl(struct net_device *netdev, struct ifreq *ifr, int cmd)
{
	int ret;

	if (ifr->ifr_ifru.ifru_data == NULL)
		return -EINVAL;

	ret = sate_vnet_ioctl_handler(cmd);
	if (!ret) {
		SATE_LOGE("ioctl handler err");
	}

	return ret;
}

static const struct net_device_ops sate_vnet_netdev_ops = {
	.ndo_open		= sate_vnet_open,
	.ndo_stop		= sate_vnet_stop,
	.ndo_start_xmit		= sate_vnet_start_xmit,
	.ndo_do_ioctl		= sate_vnet_do_ioctl,
};

static int sate_vnet_init_netdev(void)
{
	int ret;

	sate_vnet_dev = alloc_etherdev(0);
	if (sate_vnet_dev == NULL) {
		SATE_LOGE("alloc netdev %d failed");
		return -ENOMEM;
	}

	sate_vnet_dev->netdev_ops = &sate_vnet_netdev_ops;
	sate_vnet_dev->flags |= IFF_NOARP;
	scnprintf(sate_vnet_dev->name, VNETNAMESIZE,
			  "%s", "hw_sate_vnet");
	sate_vnet_dev->mtu = 450;

	eth_hw_addr_random(sate_vnet_dev);

	ret = register_netdev(sate_vnet_dev);
	if (ret) {
		SATE_LOGE("register netdev failed, %d", ret);
		goto err;
	}

	skb_queue_head_init(&tx_list);

	sate_init_proc();

	return 0;

err:
	free_netdev(sate_vnet_dev);
	return ret;
}

static void sate_vnet_exit_netdev(void)
{
    if (sate_vnet_dev) {
        unregister_netdev(sate_vnet_dev);
        free_netdev(sate_vnet_dev);
	}
	sate_exit_proc();
}

module_init(sate_vnet_init_netdev);
module_exit(sate_vnet_exit_netdev);