/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: This module is to support wifi multilink interface for kernel.
 * Create: 2023-03-31
 */

#include <huawei_platform/net/amax/amax_multilink_interface.h>
#include <linux/etherdevice.h>

#define TX_NOT_PROCESS 0
#define RX_NOT_PROCESS 1

/* MultiLink报文处理钩子函数 */
static amax_tx_hook_func g_amax_tx_hook = NULL;
static amax_tx_complete_func g_amax_tx_complete_hook = NULL;
static amax_rx_func_hook g_amax_rx_hook = NULL;

/* hmac驱动提供的发包钩子函数 */
static hmac_tx_func g_hmac_driver_tx_send = NULL;
static hmac_rx_func g_hmac_driver_netif_rx = NULL;

#define ML_ROAM_NOT_PROCESS 2
static amax_ml_roam_func g_amax_ml_roam_info_hook = NULL;
void amax_roam_hook_reg(amax_ml_roam_func ml_roam_info_func)
{
	g_amax_ml_roam_info_hook = ml_roam_info_func;
}
EXPORT_SYMBOL(amax_roam_hook_reg);

void amax_roam_hook_unreg(void)
{
	g_amax_ml_roam_info_hook = NULL;
}
EXPORT_SYMBOL(amax_roam_hook_unreg);

/* MultiLink报文处理函数钩子注册 */
void amax_hook_reg(amax_tx_hook_func tx_func, amax_tx_complete_func tx_cpt_func, amax_rx_func_hook rx_fun)
{
	g_amax_tx_hook = tx_func;
	g_amax_tx_complete_hook = tx_cpt_func;
	g_amax_rx_hook = rx_fun;
}
EXPORT_SYMBOL(amax_hook_reg);

/* MultiLink报文处理函数钩子解注册 */
void amax_hook_unreg(void)
{
	g_amax_tx_hook = NULL;
	g_amax_tx_complete_hook = NULL;
	g_amax_rx_hook = NULL;
}
EXPORT_SYMBOL(amax_hook_unreg);

/* MultiLink tx报文处理 */
int32_t amax_hook_tx_data(struct sk_buff *netbuf, struct net_device **net_dev)
{
	if (g_amax_tx_hook != NULL)
		return g_amax_tx_hook(netbuf, net_dev);
	return TX_NOT_PROCESS;
}

/* MultiLink发送完成中断报文处理 */
int32_t amax_hook_tx_complete(const struct sk_buff *netbuf)
{
	if (g_amax_tx_complete_hook != NULL)
		return g_amax_tx_complete_hook(netbuf);
	return TX_NOT_PROCESS;
}

/* MultiLink rx报文报文处理 */
int32_t amax_hook_rx_data(struct sk_buff *netbuf)
{
	if (g_amax_rx_hook != NULL)
		return g_amax_rx_hook(netbuf);
	return RX_NOT_PROCESS;
}

/* MultiLink 双频漫游信息上报 */
int32_t amax_hook_roam_info_handle(amax_dualband_roam_info_stru *dualband_roam_info)
{
	if (g_amax_ml_roam_info_hook != NULL) {
		return g_amax_ml_roam_info_hook(dualband_roam_info);
	}
		
	return ML_ROAM_NOT_PROCESS;
}

/* hmac驱动报文处理函数钩子注册 */
void hmac_hook_driver_reg(hmac_tx_func tx_func, hmac_rx_func rx_func)
{
	g_hmac_driver_tx_send = tx_func;
	g_hmac_driver_netif_rx = rx_func;
}

/* 驱动报文处理函数钩子解注册 */
void hmac_hook_driver_unreg(void)
{
	g_hmac_driver_tx_send = NULL;
	g_hmac_driver_netif_rx = NULL;
}

/* hmac驱动报文处理函数钩子解注册 */
void hmac_hook_driver_tx_send(struct sk_buff *netbuf, struct net_device *net_dev)
{
	int ret;
	if (netbuf == NULL || net_dev == NULL)
		return;

	ret = net_dev->netdev_ops->ndo_start_xmit(netbuf, net_dev);
	if (ret != 0) {
		kfree_skb(netbuf);
		netbuf = NULL;
	}
}
EXPORT_SYMBOL(hmac_hook_driver_tx_send);

void hmac_hook_driver_netif_rx(struct sk_buff *netbuf, struct net_device *net_dev)
{
	if (netbuf == NULL || net_dev == NULL)
		return;

	netif_rx_ni(netbuf);
}
EXPORT_SYMBOL(hmac_hook_driver_netif_rx);