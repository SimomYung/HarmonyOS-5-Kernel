/*
 * virtual_network_device.c
 *
 * virtual network module implementation
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

#include "virtual_network_device.h"
#include "virtual_network_utils.h"
#include "virtual_network.h"
#include "virtual_network_netlink.h"

#include <linux/module.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/inetdevice.h>
#include <linux/mm.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/rculist.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/interrupt.h>
#include <linux/timer.h>
#include <linux/gfp.h>
#include <net/xfrm.h>
#include <linux/jhash.h>
#include <linux/rtnetlink.h>
#include <net/netfilter/nf_conntrack.h>

#define VNET_DEV_HTABLE_LOCK_NUM 512

#define VNET_DEV_MATCH_MODE_NAME 0x0001

#define TUN_NAME "vnic-tun"

struct vnet_device_tun {
    atomic_t ip_enable;
    rwlock_t lock;
    struct net_device *dev;
    __be32 ip;
};

struct vnet_device_stat {
    struct hlist_node dev_hash;
    atomic_t dev_enable;
    atomic_t ip_enable;
    rwlock_t lock;
    vnet_device_role_id_t role;
    struct net_device *dev;
    union nf_inet_addr addr;
    unsigned char dest_eth_addr[ETH_ALEN]; /* destination eth addr */
};

struct dev_name_match {
    char name[IFNAMSIZ];
    int start;
    int end;
};

struct net_device_match {
    unsigned long match_mode; /* bit0:dev_name_match */
    struct dev_name_match dev_name_rule;
};

struct vnet_device_events_notifier_node {
    struct list_head node;
    vnet_device_events_notifier_t *notifier;
};

static atomic_t g_link_scene;

static __read_mostly struct hlist_head *g_dev_htable;
static __read_mostly unsigned int g_dev_htable_size;
static __cacheline_aligned_in_smp rwlock_t g_dev_htable_locks[VNET_DEV_HTABLE_LOCK_NUM];
static atomic_t g_dev_htable_count;

/* available link paths : bit0 WiFi,bit1 primary modem,bit2 secondary modem */
static __read_mostly u32 g_available_paths = 0;
static __read_mostly u32 g_available_paths_last = 0;
static DEFINE_SPINLOCK(g_available_paths_lock);

static __cacheline_aligned_in_smp rwlock_t g_path_events_notifiers_lock;
static __read_mostly struct list_head g_path_events_notifiers;

static struct vnet_device_tun g_device_tun;

static __read_mostly struct vnet_device_stat g_net_devices[VNET_ROLE_MAX];
static __read_mostly struct net_device_match g_dev_match[VNET_ROLE_MAX] = {
    {.match_mode = VNET_DEV_MATCH_MODE_NAME, .dev_name_rule = {.name = "wlan0", .start = 0, .end = 4}},
    {.match_mode = VNET_DEV_MATCH_MODE_NAME, .dev_name_rule = {.name = "rmnet0", .start = 0, .end = 5}},
    {.match_mode = VNET_DEV_MATCH_MODE_NAME, .dev_name_rule = {.name = "rmnet3", .start = 0, .end = 5}},
};

static __read_mostly struct dev_name_match g_tun_dev_name_match = {.name = "vnic-tun", .start = 0, .end = 7};

static void vnet_on_inetaddr_up_event(struct net_device *dev, const struct in_ifaddr *ifa);
static int vnet_handle_device_event(struct notifier_block *this, unsigned long event, void *ptr);
static int vnet_handle_inet_event(struct notifier_block *this, unsigned long event, void *ptr);
static struct notifier_block g_device_event_notifier = {
    .notifier_call = vnet_handle_device_event,
};

static struct notifier_block g_inet_event_notifier = {
    .notifier_call = vnet_handle_inet_event,
};

static void vnet_do_available_paths_change_notify(u32 available_paths, u32 available_paths_last)
{
    struct vnet_device_events_notifier_node *p;
    struct vnet_device_events_notifier_node *n;

    read_lock_bh(&g_path_events_notifiers_lock);
    list_for_each_entry_safe(p, n, &g_path_events_notifiers, node)
    {
        if (p->notifier->available_paths_change) {
            p->notifier->available_paths_change(available_paths, available_paths_last);
        }
    }
    read_unlock_bh(&g_path_events_notifiers_lock);
    return;
}

static void vnet_do_addr_conflict_notify(u32 ipv, const union nf_inet_addr *addr)
{
    struct vnet_device_events_notifier_node *p;
    struct vnet_device_events_notifier_node *n;

    read_lock_bh(&g_path_events_notifiers_lock);
    list_for_each_entry_safe(p, n, &g_path_events_notifiers, node)
    {
        if (p->notifier->addr_conflict_with_tun) {
            p->notifier->addr_conflict_with_tun(ipv, addr);
        }
    }
    read_unlock_bh(&g_path_events_notifiers_lock);
    return;
}

static void vnet_do_tun_status_change_notify(u32 status, u32 ipv, const union nf_inet_addr *addr)
{
    struct vnet_device_events_notifier_node *p;
    struct vnet_device_events_notifier_node *n;

    read_lock_bh(&g_path_events_notifiers_lock);
    list_for_each_entry_safe(p, n, &g_path_events_notifiers, node)
    {
        if (p->notifier->tun_status_change) {
            p->notifier->tun_status_change(status, ipv, addr);
        }
    }
    read_unlock_bh(&g_path_events_notifiers_lock);
    return;
}

static void vnet_set_device_tun_up(struct net_device *dev, const struct in_ifaddr *ifa)
{
    dev->wanted_features &= ~(NETIF_F_GSO | NETIF_F_TSO);
    netdev_update_features(dev);

    write_lock_bh(&g_device_tun.lock);
    g_device_tun.dev = dev;
    g_device_tun.ip = ifa->ifa_address;
    write_unlock_bh(&g_device_tun.lock);
    atomic_set(&(g_device_tun.ip_enable), 1);
    return;
}

static void vnet_set_device_tun_down(struct net_device *dev)
{
    atomic_set(&(g_device_tun.ip_enable), 0);
    write_lock_bh(&g_device_tun.lock);
    g_device_tun.dev = NULL;
    g_device_tun.ip = 0;
    write_unlock_bh(&g_device_tun.lock);
    return;
}

static void vnet_init_device_tun(void)
{
    rwlock_init(&g_device_tun.lock);
    atomic_set(&(g_device_tun.ip_enable), 0);
    g_device_tun.ip = 0;
    g_device_tun.dev = NULL;
    return;
}

/* Called with rcu_read_lock() or hash_table_locks[] held */
static struct vnet_device_stat *vnet_dev_htable_find(struct hlist_head *htable, const struct net_device *dev)
{
    struct vnet_device_stat *net_dev_stat = NULL;
    struct vnet_device_stat *nds = NULL;
    unsigned int bucket_hash;

    rcu_read_lock();
    bucket_hash = reciprocal_scale(dev->ifindex, g_dev_htable_size);
    hlist_for_each_entry_rcu(nds, &(htable[bucket_hash]), dev_hash)
    {
        if ((nds->dev != NULL) && (nds->dev->ifindex == dev->ifindex)) {
            net_dev_stat = nds;
            break;
        }
    }
    rcu_read_unlock();
    return net_dev_stat;
}

static void vnet_dev_htable_delete(struct vnet_device_stat *net_dev_stat)
{
    unsigned int bucket_hash;
    rwlock_t *lock = NULL;

    vnet_log_info("remove ifindex %d name %s role %d", net_dev_stat->dev->ifindex, net_dev_stat->dev->name,
                  net_dev_stat->role);

    atomic_set(&(net_dev_stat->dev_enable), 0);
    atomic_set(&(net_dev_stat->ip_enable), 0);

    write_lock_bh(&(net_dev_stat->lock));
    bucket_hash = reciprocal_scale(net_dev_stat->dev->ifindex, g_dev_htable_size);
    lock = &(g_dev_htable_locks[bucket_hash]);
    write_lock_bh(lock);
    if (hash_hashed(&(net_dev_stat->dev_hash))) {
        hlist_del_rcu(&(net_dev_stat->dev_hash));
    }
    atomic_dec(&g_dev_htable_count);
    write_unlock_bh(lock);

    INIT_HLIST_NODE(&(net_dev_stat->dev_hash));
    net_dev_stat->role = VNET_ROLE_MAX;
    net_dev_stat->dev = NULL;
    (void)memset_s(&(net_dev_stat->addr), sizeof(net_dev_stat->addr), 0x00, sizeof(net_dev_stat->addr));
    (void)memset_s(net_dev_stat->dest_eth_addr, ETH_ALEN, 0x00, ETH_ALEN);

    write_unlock_bh(&(net_dev_stat->lock));
    return;
}

static void vnet_dev_htable_insert(struct net_device *dev, vnet_device_role_id_t role_id)
{
    unsigned int bucket_hash;
    rwlock_t *lock;

    write_lock_bh(&(g_net_devices[role_id].lock));

    if (g_net_devices[role_id].dev != NULL || atomic_read(&g_net_devices[role_id].dev_enable) != 0 ||
        atomic_read(&g_net_devices[role_id].ip_enable) != 0) {
        write_unlock_bh(&(g_net_devices[role_id].lock));
        vnet_log_err("this net_device not delete, ERROR");
        return;
    }

    vnet_log_info("insert ifindex %d name %s role_id %d", dev->ifindex, dev->name, role_id);

    g_net_devices[role_id].role = role_id;
    g_net_devices[role_id].dev = dev;

    bucket_hash = reciprocal_scale(dev->ifindex, g_dev_htable_size);
    lock = &(g_dev_htable_locks[bucket_hash]);
    write_lock_bh(lock);
    hlist_add_head_rcu(&g_net_devices[role_id].dev_hash, &(g_dev_htable[bucket_hash]));
    atomic_inc(&g_dev_htable_count);
    write_unlock_bh(lock);

    write_unlock_bh(&(g_net_devices[role_id].lock));
    return;
}

static bool vnet_match_net_device_by_name(const struct net_device *dev, struct dev_name_match *match)
{
    int i;
    for (i = match->start; i <= match->end; i++) {
        if (match->name[i] != dev->name[i]) {
            return false;
        }
    }
    return true;
}

static bool vnet_match_net_device(const struct net_device *dev, struct net_device_match *match)
{
    if ((match->match_mode & VNET_DEV_MATCH_MODE_NAME) == VNET_DEV_MATCH_MODE_NAME) {
        if (vnet_match_net_device_by_name(dev, &match->dev_name_rule)) {
            return true;
        }
    }
    return false;
}

vnet_device_role_id_t vnet_get_role_id(const struct net_device *dev)
{
    int i;
    vnet_device_role_id_t role_id = VNET_ROLE_ERR;

    if (unlikely(dev == NULL)) {
        return role_id;
    }

    for (i = 0; i < VNET_ROLE_MAX; i++) {
        if (vnet_match_net_device(dev, &g_dev_match[i])) {
            role_id = i;
            break;
        }
    }
    return role_id;
}

static void vnet_on_netdevice_register_event(struct net_device *dev)
{
    struct vnet_device_stat *net_dev_stat = NULL;
    vnet_device_role_id_t role_id;

    role_id = vnet_get_role_id(dev);
    if ((role_id <= VNET_ROLE_ERR) || (role_id >= VNET_ROLE_MAX)) {
        vnet_log_debug("ignore ifindex %d name %s", dev->ifindex, dev->name);
        return;
    }

    net_dev_stat = vnet_dev_htable_find(g_dev_htable, dev);
    if (net_dev_stat != NULL) {
        vnet_log_warn("this net_device not unregister, will unregister it first.");
        vnet_dev_htable_delete(net_dev_stat);
    }

    vnet_log_info("REGISTER ifindex %d name %s role_id %d", dev->ifindex, dev->name, role_id);
    vnet_dev_htable_insert(dev, role_id);
    return;
}

static void vnet_on_netdevice_up_event(struct net_device *dev)
{
    struct vnet_device_stat *net_dev_stat = NULL;

    net_dev_stat = vnet_dev_htable_find(g_dev_htable, dev);
    if (net_dev_stat == NULL) {
        return;
    }

    vnet_log_info("UP ifindex %d name %s role_id %d", dev->ifindex, dev->name, net_dev_stat->role);
    smp_mb__before_atomic();
    atomic_set(&(net_dev_stat->dev_enable), 1);
    return;
}

static void vnet_on_netdevice_down_event(struct net_device *dev)
{
    struct vnet_device_stat *net_dev_stat = NULL;

    net_dev_stat = vnet_dev_htable_find(g_dev_htable, dev);
    if (net_dev_stat == NULL) {
        return;
    }

    vnet_log_info("DOWN ifindex %d name %s role_id %d", dev->ifindex, dev->name, net_dev_stat->role);
    smp_mb__before_atomic();
    atomic_set(&(net_dev_stat->dev_enable), 0);
    return;
}

static void vnet_on_netdevice_unregister_event(struct net_device *dev)
{
    struct vnet_device_stat *net_dev_stat = NULL;

    net_dev_stat = vnet_dev_htable_find(g_dev_htable, dev);
    if (net_dev_stat == NULL) {
        vnet_log_debug("this net_device ifindex %d name %s not registered, ignore it.", dev->ifindex, dev->name);
        return;
    }

    vnet_log_info("UNREGISTER ifindex %d name %s role_id %d", dev->ifindex, dev->name, net_dev_stat->role);
    vnet_dev_htable_delete(net_dev_stat);
    return;
}

static int vnet_handle_device_event(struct notifier_block *this, unsigned long event, void *ptr)
{
    struct net_device *dev = NULL;

    if (unlikely(ptr == NULL)) {
        return NOTIFY_DONE;
    }

    dev = netdev_notifier_info_to_dev(ptr);
    if (unlikely(dev == NULL)) {
        return NOTIFY_DONE;
    }

    ASSERT_RTNL();

    vnet_log_debug("event %lu ifindex %d name %s", event, dev->ifindex, dev->name);

    switch (event) {
        case NETDEV_REGISTER:
            vnet_on_netdevice_register_event(dev);
            break;
        case NETDEV_UP:
            vnet_on_netdevice_up_event(dev);
            if (dev->ip_ptr && dev->ip_ptr->ifa_list) {
                vnet_on_inetaddr_up_event(dev, dev->ip_ptr->ifa_list);
            }
            break;
        case NETDEV_DOWN:
            vnet_on_netdevice_down_event(dev);
            break;
        case NETDEV_UNREGISTER:
            vnet_on_netdevice_unregister_event(dev);
            break;
        default:
            break;
    }
    return NOTIFY_DONE;
}

static int vnet_check_ip_conflict_with_tun_dev(const struct net_device *dev, const struct in_ifaddr *ifa)
{
    struct net_device *tun_dev = NULL;
    struct in_ifaddr *ifa_list;

    if (!vnet_is_available()) {
        return 0;
    }

    if (vnet_net_device_is_tun_dev(dev)) {
        return 0;
    }

    tun_dev = dev_get_by_name(&init_net, TUN_NAME);
    if (tun_dev == NULL) {
        vnet_log_warn("tun dev is NULL");
        return 0;
    }

    if (unlikely(tun_dev->ip_ptr == NULL)) {
        dev_put(tun_dev);
        return 0;
    }

    ifa_list = tun_dev->ip_ptr->ifa_list;
    while (ifa_list != NULL) {
        if (ifa_list->ifa_address == 0 || ifa->ifa_address == 0) {
            ifa_list = ifa_list->ifa_next;
            continue;
        }
        if (ifa_list->ifa_address == ifa->ifa_address) {
            union nf_inet_addr addr;
            addr.ip = ifa->ifa_address;
            vnet_log_info("ip conflict tun %s dev %s", tun_dev->name, dev->name);
            vnet_do_addr_conflict_notify(4, &addr);
            dev_put(tun_dev);
            return 1;
        }
        ifa_list = ifa_list->ifa_next;
    }
    dev_put(tun_dev);
    return 0;
}

static void vnet_set_net_device_stat_up(struct vnet_device_stat *net_dev_stat, const struct in_ifaddr *ifa)
{
    write_lock_bh(&(net_dev_stat->lock));
    (void)memset_s(net_dev_stat->dest_eth_addr, ETH_ALEN, 0x00, ETH_ALEN);
    (void)memset_s(&(net_dev_stat->addr), sizeof(net_dev_stat->addr), 0, sizeof(net_dev_stat->addr));
    net_dev_stat->addr.ip = ifa->ifa_address;
    write_unlock_bh(&(net_dev_stat->lock));

    smp_mb__before_atomic();
    atomic_set(&(net_dev_stat->ip_enable), 1);
    return;
}

static void vnet_set_net_device_stat_down(struct vnet_device_stat *net_dev_stat)
{
    smp_mb__before_atomic();
    atomic_set(&(net_dev_stat->ip_enable), 0);

    write_lock_bh(&(net_dev_stat->lock));
    (void)memset_s(net_dev_stat->dest_eth_addr, ETH_ALEN, 0x00, ETH_ALEN);
    (void)memset_s(&(net_dev_stat->addr), sizeof(net_dev_stat->addr), 0x00, sizeof(net_dev_stat->addr));
    write_unlock_bh(&(net_dev_stat->lock));
    return;
}

static inline void vnet_available_paths_change_check(u32 available_paths, u32 available_paths_last)
{
    if (available_paths != available_paths_last) {
        vnet_log_info("available paths 0x%08X last paths 0x%08X", available_paths, available_paths_last);
        vnet_do_available_paths_change_notify(available_paths, available_paths_last);
    }
    return;
}

static void vnet_set_available_paths_up(vnet_device_role_id_t role)
{
    u32 available_paths;
    u32 available_paths_last;

    spin_lock_bh(&g_available_paths_lock);

    available_paths_last = g_available_paths_last;
    available_paths = g_available_paths;
    g_available_paths_last = g_available_paths;

    if (role == VNET_ROLE_PRIMARY_WIFI) {
        g_available_paths |= VNET_PATH_WIFI_1; /* set WiFi path */
        if (atomic_read(&g_link_scene) == VNET_SCENE_TYPE_MAX) {
            vnet_set_link_scene(VNET_SCENE_WIFI_MODEM);
            vnet_log_info("scene not set, so set it to VNET_SCENE_WIFI_MODEM");
        }
    } else if (role == VNET_ROLE_PRIMARY_MODEM) {
        g_available_paths |= VNET_PATH_MODEM_1; /* set primary modem path */
    } else if (role == VNET_ROLE_SECONDARY_MODEM) {
        g_available_paths |= VNET_PATH_MODEM_2; /* set secondary modem path */
    }
    spin_unlock_bh(&g_available_paths_lock);

    vnet_available_paths_change_check(available_paths, available_paths_last);
    return;
}

static void vnet_set_available_paths_down(vnet_device_role_id_t role)
{
    u32 available_paths;
    u32 available_paths_last;

    spin_lock_bh(&g_available_paths_lock);

    available_paths_last = g_available_paths_last;
    available_paths = g_available_paths;
    g_available_paths_last = g_available_paths;

    if (role == VNET_ROLE_PRIMARY_WIFI) {
        g_available_paths &= (~VNET_PATH_WIFI_1); /* set WiFi path */
    } else if (role == VNET_ROLE_PRIMARY_MODEM) {
        g_available_paths &= (~VNET_PATH_MODEM_1); /* set primary modem path */
    } else if (role == VNET_ROLE_SECONDARY_MODEM) {
        g_available_paths &= (~VNET_PATH_MODEM_2); /* set secondary modem path */
    }
    spin_unlock_bh(&g_available_paths_lock);

    vnet_available_paths_change_check(available_paths, available_paths_last);
    return;
}

static void vnet_tun_ip_up(struct net_device *dev, const struct in_ifaddr *ifa)
{
    union nf_inet_addr addr;

    vnet_set_device_tun_up(dev, ifa);
    addr.ip = ifa->ifa_address;
    vnet_do_tun_status_change_notify(1, 4, &addr);
    return;
}

static void vnet_tun_ip_down(struct net_device *dev, const struct in_ifaddr *ifa)
{
    union nf_inet_addr addr;

    vnet_set_device_tun_down(dev);
    addr.ip = ifa->ifa_address;
    vnet_do_tun_status_change_notify(0, 4, &addr);
    return;
}

static void vnet_on_inetaddr_up_event(struct net_device *dev, const struct in_ifaddr *ifa)
{
    struct vnet_device_stat *net_dev_stat = NULL;

    vnet_check_ip_conflict_with_tun_dev(dev, ifa);

    if (vnet_net_device_is_tun_dev(dev)) {
        vnet_log_info("IP_UP ifindex %d name %s addr %08x", dev->ifindex, dev->name, ifa->ifa_address);
        vnet_tun_ip_up(dev, ifa);
        return;
    }

    net_dev_stat = vnet_dev_htable_find(g_dev_htable, dev);
    if (net_dev_stat == NULL) {
        return;
    }

    vnet_log_info("IP_UP ifindex %d name %s addr %08x", dev->ifindex, dev->name, ifa->ifa_address);
    vnet_set_net_device_stat_up(net_dev_stat, ifa);
    vnet_set_available_paths_up(net_dev_stat->role);
    return;
}

static void vnet_on_inetaddr_down_event(struct net_device *dev, const struct in_ifaddr *ifa)
{
    struct vnet_device_stat *net_dev_stat = NULL;

    if (vnet_net_device_is_tun_dev(dev)) {
        vnet_log_info("IP_DOWN ifindex %d name %s addr %08x", dev->ifindex, dev->name, ifa->ifa_address);
        vnet_tun_ip_down(dev, ifa);
        return;
    }

    net_dev_stat = vnet_dev_htable_find(g_dev_htable, dev);
    if (net_dev_stat == NULL) {
        vnet_log_info("ignore the device");
        return;
    }

    vnet_log_info("IP_DOWN ifindex %d name %s addr %08x", dev->ifindex, dev->name, ifa->ifa_address);
    vnet_set_net_device_stat_down(net_dev_stat);
    vnet_set_available_paths_down(net_dev_stat->role);
    return;
}

static int vnet_handle_inet_event(struct notifier_block *this, unsigned long event, void *ptr)
{
    const struct in_ifaddr *ifa = ptr;
    const struct in_device *idev = NULL;
    struct net_device *dev = NULL;

    idev = ifa->ifa_dev;
    if (idev->dead) {
        return NOTIFY_DONE;
    }

    dev = idev->dev;
    if (unlikely(dev == NULL)) {
        return NOTIFY_DONE;
    }

    vnet_log_info("event %lu ifindex %d name %s", event, dev->ifindex, dev->name);

    switch (event) {
        case NETDEV_UP:
        case NETDEV_CHANGEADDR:
        case NETDEV_NOTIFY_PEERS:
            if (!inetdev_valid_mtu(dev->mtu)) {
                break;
            }
            vnet_on_inetaddr_up_event(dev, ifa);
            break;
        case NETDEV_CHANGEMTU:
            if (inetdev_valid_mtu(dev->mtu)) {
                break;
            }
            /* disable IP when MTU is invalid */
            fallthrough;
        case NETDEV_DOWN:
            vnet_on_inetaddr_down_event(dev, ifa);
            break;
        default:
            break;
    }
    return NOTIFY_DONE;
}

int vnet_device_monitor_init(void)
{
    int i;
    int j;
    vnet_log_info("virtual network device module init");

    INIT_LIST_HEAD(&g_path_events_notifiers);
    rwlock_init(&g_path_events_notifiers_lock);

    atomic_set(&g_link_scene, VNET_SCENE_TYPE_MAX);
    atomic_set(&g_dev_htable_count, 0);
    g_dev_htable_size = VNET_DEV_HTABLE_LOCK_NUM;
    g_dev_htable = nf_ct_alloc_hashtable(&g_dev_htable_size, 0);
    if (!g_dev_htable) {
        vnet_log_err("devices hash table alloc failed, error");
        return ERROR;
    }

    for (i = 0; i < VNET_DEV_HTABLE_LOCK_NUM; i++) {
        rwlock_init(&g_dev_htable_locks[i]);
    }

    for (j = 0; j < VNET_ROLE_MAX; j++) {
        INIT_HLIST_NODE(&(g_net_devices[j].dev_hash));
        atomic_set(&(g_net_devices[j].dev_enable), 0);
        atomic_set(&(g_net_devices[j].ip_enable), 0);
        rwlock_init(&g_net_devices[j].lock);
        g_net_devices[j].role = VNET_ROLE_MAX;
        g_net_devices[j].dev = NULL;
        memset_s(&(g_net_devices[j].addr), sizeof(g_net_devices[j].addr), 0, sizeof(g_net_devices[j].addr));
    }
    vnet_init_device_tun();

    /* Register for device stat reports */
    register_netdevice_notifier(&g_device_event_notifier);
    /* Register IP address stat reports */
    register_inetaddr_notifier(&g_inet_event_notifier);
    return SUCCESS;
}

void vnet_device_monitor_exit(void)
{
    vnet_log_info("virtual network device module exit");

    /* unRegister for device stat reports */
    unregister_netdevice_notifier(&g_device_event_notifier);
    /* unRegister IP address stat reports */
    unregister_inetaddr_notifier(&g_inet_event_notifier);

    if (g_dev_htable) {
        kvfree(g_dev_htable);
        g_dev_htable = NULL;
    }
    return;
}

int vnet_register_device_events_notifier(vnet_device_events_notifier_t *notifier)
{
    struct vnet_device_events_notifier_node *p;
    struct vnet_device_events_notifier_node *n;
    struct vnet_device_events_notifier_node *notifier_node;

    if (unlikely((notifier == NULL) || ((notifier->available_paths_change == NULL) &&
        (notifier->addr_conflict_with_tun == NULL) && (notifier->tun_status_change == NULL)))) {
        return ERROR;
    }
    notifier_node = kmalloc(sizeof(struct vnet_device_events_notifier_node), GFP_ATOMIC);
    if (notifier_node == NULL) {
        return ERROR;
    }

    write_lock_bh(&g_path_events_notifiers_lock);
    list_for_each_entry_safe(p, n, &g_path_events_notifiers, node)
    {
        if (p->notifier == notifier) {
            write_unlock_bh(&g_path_events_notifiers_lock);
            kfree(notifier_node);
            vnet_log_warn("double register detected");
            return SUCCESS;
        }
    }

    list_add_tail(&notifier_node->node, &g_path_events_notifiers);
    write_unlock_bh(&g_path_events_notifiers_lock);
    return SUCCESS;
}

void vnet_unregister_device_events_notifier(vnet_device_events_notifier_t *notifier)
{
    struct vnet_device_events_notifier_node *p;
    struct vnet_device_events_notifier_node *n;

    if (unlikely(notifier == NULL)) {
        return;
    }

    write_lock_bh(&g_path_events_notifiers_lock);
    list_for_each_entry_safe(p, n, &g_path_events_notifiers, node)
    {
        if (p->notifier == notifier) {
            list_del(&p->node);
            kfree(p);
            break;
        }
    }
    write_unlock_bh(&g_path_events_notifiers_lock);
    return;
}

void vnet_set_link_scene(vnet_link_scene_t scene)
{
    if (likely(scene >= VNET_SCENE_WIFI_MODEM && scene < VNET_SCENE_TYPE_MAX)) {
        atomic_set(&g_link_scene, scene);
    }
}

vnet_device_role_id_t vnet_get_default_device_role_id(void)
{
    vnet_link_scene_t scene = atomic_read(&g_link_scene);
    u32 available_paths = vnet_get_available_paths();
    if ((available_paths & VNET_PATH_WIFI_1) && (scene == VNET_SCENE_WIFI_MODEM)) {
        return VNET_ROLE_PRIMARY_WIFI;
    }

    /* primary modem path check */
    if (available_paths & VNET_PATH_MODEM_1) {
        return VNET_ROLE_PRIMARY_MODEM;
    }

    /* secondary modem path check */
    if (available_paths & VNET_PATH_MODEM_2) {
        return VNET_ROLE_SECONDARY_MODEM;
    }

    return VNET_ROLE_ERR;
}

vnet_device_path_bit_t vnet_device_role_id_to_path_id(vnet_device_role_id_t role_id)
{
    switch (role_id) {
        case VNET_ROLE_PRIMARY_WIFI:
            return VNET_PATH_WIFI_1;
        case VNET_ROLE_PRIMARY_MODEM:
            return VNET_PATH_MODEM_1;
        case VNET_ROLE_SECONDARY_MODEM:
            return VNET_PATH_MODEM_2;
        default:
            return VNET_PATH_NONE;
    }
}

vnet_device_role_id_t vnet_device_path_bit_to_role_id(vnet_device_path_bit_t path_bit)
{
    switch (path_bit) {
        case VNET_PATH_WIFI_1:
            return VNET_ROLE_PRIMARY_WIFI;
        case VNET_PATH_MODEM_1:
            return VNET_ROLE_PRIMARY_MODEM;
        case VNET_PATH_MODEM_2:
            return VNET_ROLE_SECONDARY_MODEM;
        default:
            return VNET_ROLE_ERR;
    }
}

u32 vnet_get_available_paths(void)
{
    u32 ret;
    spin_lock_bh(&g_available_paths_lock);
    ret = g_available_paths;
    spin_unlock_bh(&g_available_paths_lock);
    return ret;
}

struct net_device *vnet_get_device(vnet_device_role_id_t role_id)
{
    struct net_device *dev;
    if (unlikely(role_id <= VNET_ROLE_ERR || role_id >= VNET_ROLE_MAX)) {
        vnet_log_info("error role_id %d", role_id);
        return NULL;
    }

    if (unlikely(atomic_read(&(g_net_devices[role_id].ip_enable)) == 0)) {
        return NULL;
    }

    read_lock_bh(&(g_net_devices[role_id].lock));
    dev = g_net_devices[role_id].dev;
    read_unlock_bh(&(g_net_devices[role_id].lock));
    return dev;
}

__be32 vnet_select_addr(vnet_device_role_id_t role_id)
{
    __be32 src;

    if (unlikely(role_id <= VNET_ROLE_ERR || role_id >= VNET_ROLE_MAX)) {
        return 0;
    }

    if (unlikely(atomic_read(&(g_net_devices[role_id].ip_enable)) == 0)) {
        vnet_log_info("the net device is ip down");
        return 0;
    }

    read_lock_bh(&(g_net_devices[role_id].lock));
    src = g_net_devices[role_id].addr.ip;
    read_unlock_bh(&(g_net_devices[role_id].lock));
    return src;
}

void vnet_set_device_mac(vnet_device_role_id_t role_id, const char *data)
{
    errno_t err;

    if (unlikely(role_id <= VNET_ROLE_ERR || role_id >= VNET_ROLE_MAX)) {
        vnet_log_err("id is invalid, error");
        return;
    }

    write_lock_bh(&(g_net_devices[role_id].lock));
    err = memcpy_s(g_net_devices[role_id].dest_eth_addr, ETH_ALEN, data, ETH_ALEN);
    write_unlock_bh(&(g_net_devices[role_id].lock));
    if (err != EOK) {
        vnet_log_err("memcpy_s failed");
    }

    return;
}

const char *vnet_get_device_mac(vnet_device_role_id_t role_id)
{
    if (unlikely(role_id <= VNET_ROLE_ERR || role_id >= VNET_ROLE_MAX)) {
        vnet_log_err("id is invalid, error");
        return NULL;
    }

    if (unlikely(atomic_read(&(g_net_devices[role_id].ip_enable)) == 0)) {
        return NULL;
    }

    if (g_net_devices[role_id].dest_eth_addr[NUM_0] == 0 && g_net_devices[role_id].dest_eth_addr[NUM_1] == 0 &&
        g_net_devices[role_id].dest_eth_addr[NUM_2] == 0 && g_net_devices[role_id].dest_eth_addr[NUM_3] == 0 &&
        g_net_devices[role_id].dest_eth_addr[NUM_4] == 0 && g_net_devices[role_id].dest_eth_addr[NUM_5] == 0) {
        return NULL;
    }

    return g_net_devices[role_id].dest_eth_addr;
}

__be32 vnet_get_tun_ip(void)
{
    __be32 ip = 0;
    if (atomic_read(&(g_device_tun.ip_enable)) != 1) {
        return 0;
    }

    read_lock_bh(&g_device_tun.lock);
    ip = g_device_tun.ip;
    read_unlock_bh(&g_device_tun.lock);
    return ip;
}

struct net_device *vnet_get_tun_dev(void)
{
    struct net_device *dev = NULL;
    if (atomic_read(&(g_device_tun.ip_enable)) != 1) {
        return NULL;
    }
    read_lock_bh(&g_device_tun.lock);
    dev = g_device_tun.dev;
    read_unlock_bh(&g_device_tun.lock);
    return dev;
}

bool vnet_net_device_is_tun_dev(const struct net_device *dev)
{
    if (unlikely(dev == NULL)) {
        return false;
    }

    return vnet_match_net_device_by_name(dev, &g_tun_dev_name_match);
}

bool vnet_tun_dev_skb_match(const struct sk_buff *skb)
{
    struct iphdr *iph = NULL;
    bool ret = false;

    if (unlikely(skb == NULL)) {
        return false;
    }

    if (!vnet_net_device_is_tun_dev(skb->dev)) {
        return false;
    }

    if (atomic_read(&(g_device_tun.ip_enable)) != 1) {
        vnet_log_debug("tun dev ip not enable");
        return false;
    }

    iph = ip_hdr(skb);
    if (iph->protocol == IPPROTO_UDP) {
        return true;
    }

    read_lock_bh(&g_device_tun.lock);
    if (likely(iph->saddr == g_device_tun.ip)) {
        ret = true;
    }
    read_unlock_bh(&g_device_tun.lock);
    return ret;
}
