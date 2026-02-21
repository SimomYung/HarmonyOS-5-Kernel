/*
 * virtual_network_device.h
 *
 * virtual network kernel module implementation
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef _VIRTUAL_NETWORK_DEVICE_H_
#define _VIRTUAL_NETWORK_DEVICE_H_

#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <linux/netfilter.h>

typedef enum vnet_link_scene {
    VNET_SCENE_WIFI_MODEM = 0, // WiFi + modem
    VNET_SCENE_MODEM_MODEM = 1, // modem + modem
    VNET_SCENE_TYPE_MAX
} vnet_link_scene_t;

typedef enum vnet_device_path_bit {
    VNET_PATH_NONE = 0,
    VNET_PATH_WIFI_1 = 0b0001, // 0x01
    VNET_PATH_MODEM_1 = 0b0010, // 0x02
    VNET_PATH_MODEM_2 = 0b0100, // 0x04
    VNET_PATH_WIFI_2 = 0b1000,  // 0x08
} vnet_device_path_bit_t;

typedef enum vnet_device_role_id {
    VNET_ROLE_ERR = -1,
    VNET_ROLE_PRIMARY_WIFI = 0,
    VNET_ROLE_PRIMARY_MODEM = 1,
    VNET_ROLE_SECONDARY_MODEM = 2,
    VNET_ROLE_SECONDARY_WIFI = 3,
    VNET_ROLE_MAX = 3,
} vnet_device_role_id_t;

typedef struct vnet_device_events_notifier {
    struct list_head node;
    // available paths changed
    void (*available_paths_change)(u32 available_paths, u32 available_paths_last);
    // dev addr conflict with tun_dev. |ipv| ip type: 4 ipv4, 6 ipv6
    void (*addr_conflict_with_tun)(u32 ipv, const union nf_inet_addr *addr);
    // tun status changed. status: 0 down, 1 up. |ipv| ip type: 4 ipv4, 6 ipv6
    void (*tun_status_change)(u32 status, u32 ipv, const union nf_inet_addr *addr);
} vnet_device_events_notifier_t;

int vnet_register_device_events_notifier(vnet_device_events_notifier_t *notifier);
void vnet_unregister_device_events_notifier(vnet_device_events_notifier_t *notifier);

// 0 : WiFi + modem, 1: modem + modem
void vnet_set_link_scene(vnet_link_scene_t scene);
vnet_device_role_id_t vnet_get_default_device_role_id(void);

vnet_device_path_bit_t vnet_device_role_id_to_path_id(vnet_device_role_id_t role_id);
vnet_device_role_id_t vnet_device_path_bit_to_role_id(vnet_device_path_bit_t path_bit);

u32 vnet_get_available_paths(void);

struct net_device *vnet_get_device(vnet_device_role_id_t role_id);
vnet_device_role_id_t vnet_get_role_id(const struct net_device *dev);

__be32 vnet_select_addr(vnet_device_role_id_t role_id);

void vnet_set_device_mac(vnet_device_role_id_t role_id, const char *data);
const char *vnet_get_device_mac(vnet_device_role_id_t role_id);

__be32 vnet_get_tun_ip(void);

struct net_device *vnet_get_tun_dev(void);

bool vnet_net_device_is_tun_dev(const struct net_device *dev);

bool vnet_tun_dev_skb_match(const struct sk_buff *skb);

int vnet_device_monitor_init(void);
void vnet_device_monitor_exit(void);

#endif // _VIRTUAL_NETWORK_DEVICE_H_
