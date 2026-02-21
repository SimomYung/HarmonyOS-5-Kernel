/*
 * virtual_network.c
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

#include "virtual_network.h"
#include "virtual_network_utils.h"
#include "virtual_network_device.h"
#include "virtual_network_session.h"
#include "virtual_network_netlink.h"
#include "virtual_network_tx_rx.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/rbtree.h>


#define DTS_NODE_HUAWEI_VIRTUAL_NETWORK "huawei,virtual_network"
#define DTS_PROP_VIRTUAL_NETWORK_ENABLE "virtual_network_enable"

#define VNET_PROC_DATA_LEN 32

typedef struct vnet_app_node {
    struct rb_node app_rb_node;
    u32 uid;
    int policy_id;
} vnet_app_node_t;

static struct proc_dir_entry *g_proc_vnet_dir = NULL;
static struct proc_dir_entry *g_proc_vnet_dynamic_state = NULL;
static struct proc_dir_entry *g_proc_vnet_log_level = NULL;

static atomic_t g_vnet_dynamic_state = ATOMIC_INIT(0); // 1:vnet on, 0:vnet down

static atomic_t g_inited = ATOMIC_INIT(0);

static __cacheline_aligned_in_smp rwlock_t g_app_infos_lock;
static __read_mostly struct rb_root g_app_infos = RB_ROOT;

void vnet_init_app_infos(void)
{
    rwlock_init(&g_app_infos_lock);
}

// should be called with write_lock_bh or read_lock_bh
static vnet_app_node_t *vnet_search_app_info(int uid)
{
    struct rb_node *node = g_app_infos.rb_node;
    while (node != NULL) {
        vnet_app_node_t *app_node = rb_entry(node, vnet_app_node_t, app_rb_node);
        if (uid < app_node->uid) {
            node = node->rb_left;
        } else if (uid > app_node->uid) {
            node = node->rb_right;
        } else {
            return app_node;
        }
    }
    return NULL;
}

int vnet_add_app_info(vnet_app_info_t *app_info)
{
    struct rb_node **insert = NULL;
    struct rb_node *parent = NULL;
    vnet_app_node_t *app_node = NULL;

    if (unlikely(app_info == NULL || app_info->policy_id <= 0)) {
        return ERROR;
    }

    app_node = kzalloc(sizeof(vnet_app_node_t), GFP_ATOMIC);
    if (unlikely(app_node == NULL)) {
        vnet_log_err("alloc vnet_app_node_t fail");
        return ERROR;
    }
    app_node->uid = app_info->uid;
    app_node->policy_id = app_info->policy_id;

    write_lock_bh(&g_app_infos_lock);
    insert = &g_app_infos.rb_node;
    while (*insert != NULL) {
        vnet_app_node_t *cur = rb_entry(*insert, vnet_app_node_t, app_rb_node);
        parent = *insert;
        if (app_info->uid < cur->uid) {
            insert = &((*insert)->rb_left);
        } else if (app_info->uid > cur->uid) {
            insert = &((*insert)->rb_right);
        } else {
            write_unlock_bh(&g_app_infos_lock);
            kfree(app_node);
            vnet_log_err("app uid %d has already added", app_info->uid);
            return ERROR;
        }
    }
    rb_link_node(&app_node->app_rb_node, parent, insert);
    rb_insert_color(&app_node->app_rb_node, &g_app_infos);
    write_unlock_bh(&g_app_infos_lock);

    vnet_log_info("add app uid %d", app_info->uid);
    return SUCCESS;
}

void vnet_remove_app_info(u32 uid)
{
    vnet_app_node_t *app_node = NULL;

    write_lock_bh(&g_app_infos_lock);
    app_node = vnet_search_app_info(uid);
    if (app_node != NULL) {
        vnet_log_info("remove app uid %d", uid);
        rb_erase(&app_node->app_rb_node, &g_app_infos);
        kfree(app_node);
    }
    write_unlock_bh(&g_app_infos_lock);
    return;
}

bool vnet_set_policy_for_uid(u32 uid, int policy_id)
{
    vnet_app_node_t *app_node;

    if (policy_id <= 0) {
        return false;
    }

    read_lock_bh(&g_app_infos_lock);
    app_node = vnet_search_app_info(uid);
    read_unlock_bh(&g_app_infos_lock);
    if (app_node != NULL) {
        app_node->policy_id = policy_id;
        return true;
    }
    return false;
}

int vnet_get_policy_for_uid(u32 uid)
{
    vnet_app_node_t *app_node;

    read_lock_bh(&g_app_infos_lock);
    app_node = vnet_search_app_info(uid);
    read_unlock_bh(&g_app_infos_lock);
    if (app_node != NULL) {
        return app_node->policy_id;
    }
    return -1;
}

void vnet_clear_app_infos(void)
{
    vnet_app_node_t *app_node;
    struct rb_node *node;

    write_lock_bh(&g_app_infos_lock);
    while (true) {
        node = rb_first(&g_app_infos);
        if (node == NULL) {
            break;
        }
        app_node = rb_entry(node, vnet_app_node_t, app_rb_node);
        vnet_log_info("clear app uid %d", app_node->uid);
        rb_erase(&app_node->app_rb_node, &g_app_infos);
        kfree(app_node);
    }
    g_app_infos = RB_ROOT;
    write_unlock_bh(&g_app_infos_lock);
    return;
}

static void vnet_set_vnet_state(int state)
{
    atomic_set(&g_vnet_dynamic_state, state);
    return;
}

static ssize_t vnet_proc_state_read(struct file *fp, char __user *user_buf, size_t count, loff_t *ppos)
{
    char buf[VNET_PROC_DATA_LEN];
    if (!fp || !user_buf || !ppos || (count < sizeof(int))) {
        return count;
    }

    if (sprintf_s(buf, VNET_PROC_DATA_LEN, "%d\r\n", atomic_read(&g_vnet_dynamic_state)) < 0) {
        return -1;
    }
    return simple_read_from_buffer(user_buf, count, ppos, buf, strlen(buf) + 1);
}

static ssize_t vnet_proc_state_write(struct file *fp, const char __user *user_buf, size_t count, loff_t *ppos)
{
    char buf[VNET_PROC_DATA_LEN];
    int state;

    if (!fp || !user_buf || !ppos) {
        return count;
    }

    (void)memset_s(buf, sizeof(buf), 0, sizeof(buf));
    state = simple_write_to_buffer(&buf, sizeof(buf), ppos, user_buf, count);
    if (state <= 0) {
        vnet_log_warn("simple_write_to_buffer %d", state);
        return count;
    }

    state = simple_strtol(buf, NULL, 10);
    vnet_set_vnet_state(state > 0 ? 1 : 0);
    return count;
}

static ssize_t vnet_proc_log_level_read(struct file *fp, char __user *user_buf, size_t count, loff_t *ppos)
{
    char buf[VNET_PROC_DATA_LEN];

    if (!fp || !user_buf || !ppos || (count < sizeof(int))) {
        return count;
    }

    if (sprintf_s(buf, VNET_PROC_DATA_LEN, "%d\r\n", vnet_log_level_get()) < 0) {
        return -1;
    }
    return simple_read_from_buffer(user_buf, count, ppos, buf, strlen(buf) + 1);
}

static ssize_t vnet_proc_log_level_write(struct file *fp, const char __user *user_buf, size_t count, loff_t *ppos)
{
    char buf[VNET_PROC_DATA_LEN];
    int log_level;

    if (!fp || !user_buf || !ppos) {
        return count;
    }

    (void)memset_s(buf, sizeof(buf), 0, sizeof(buf));
    log_level = simple_write_to_buffer(&buf, sizeof(buf), ppos, user_buf, count);
    if (log_level <= 0) {
        vnet_log_info("simple_write_to_buffer %d", log_level);
        return count;
    }

    log_level = simple_strtol(buf, NULL, 10);
    vnet_log_level_set(log_level);
    return count;
}

static const struct proc_ops g_vnet_state_fops = {
    .proc_read = vnet_proc_state_read,
    .proc_write = vnet_proc_state_write,
};

static const struct proc_ops g_vnet_log_level_fops = {
    .proc_read = vnet_proc_log_level_read,
    .proc_write = vnet_proc_log_level_write,
};

static void vnet_proc_debug_init(void)
{
    vnet_set_vnet_state(1);

    // create dir /proc/virtual_network
    g_proc_vnet_dir = proc_mkdir("virtual_network", NULL);
    if (g_proc_vnet_dir == NULL) {
        vnet_log_warn("create /proc/virtual_network failed.");
        return;
    }

    // create file /proc/virtual_network/dynamic_state
    g_proc_vnet_dynamic_state = proc_create("dynamic_state", S_IRUGO | S_IWUGO, g_proc_vnet_dir, &g_vnet_state_fops);
    if (g_proc_vnet_dir == NULL) {
        vnet_log_warn("create /proc/virtual_network/dynamic_state failed.");
        return;
    }

    // create file /proc/virtual_network/log_level
    g_proc_vnet_log_level = proc_create("log_level", S_IRUGO | S_IWUGO, g_proc_vnet_dir, &g_vnet_log_level_fops);
    if (g_proc_vnet_dir == NULL) {
        vnet_log_warn("create /proc/virtual_network/log_level failed.");
        return;
    }

    return;
}

static void vnet_proc_debug_exit(void)
{
    if (g_proc_vnet_dynamic_state) {
        proc_remove(g_proc_vnet_dynamic_state);
        g_proc_vnet_dynamic_state = NULL;
    }

    if (g_proc_vnet_log_level) {
        proc_remove(g_proc_vnet_log_level);
        g_proc_vnet_log_level = NULL;
    }

    if (g_proc_vnet_dir) {
        proc_remove(g_proc_vnet_dir);
        g_proc_vnet_dir = NULL;
    }
    return;
}

static bool vnet_is_support(void)
{
    struct device_node *node = of_find_compatible_node(NULL, NULL, DTS_NODE_HUAWEI_VIRTUAL_NETWORK);
    if (node == NULL) {
        vnet_log_info("not find huawei,virtual_network dts node, not support");
        return false;
    }

    if (of_property_read_bool(node, DTS_PROP_VIRTUAL_NETWORK_ENABLE)) {
        vnet_log_info("find virtual_network dts probe, is support");
        return true;
    }
    vnet_log_info("not find virtual_network dts probe, not support");
    return false;
}

bool vnet_is_available(void)
{
    return (atomic_read(&g_inited) == 1) && (atomic_read(&g_vnet_dynamic_state) == 1);
}

static int __init vnet_init(void)
{
    vnet_log_info("virtual network module init");

    if (!vnet_is_support()) {
        return -EINVAL;
    }

    if (vnet_netlink_ctrl_init() != SUCCESS) {
        vnet_log_info("netlink_ctrl_init_failed");
        goto netlink_ctrl_init_failed;
    }

    if (vnet_session_mgr_init() != SUCCESS) {
        vnet_log_info("session_mgr_init_failed");
        goto session_mgr_init_failed;
    }

    if (vnet_device_monitor_init() != SUCCESS) {
        vnet_log_info("device_monitor_init_failed");
        goto device_monitor_init_failed;
    }

    if (vnet_tx_rx_init() != SUCCESS) {
        vnet_log_info("tx_rx_init_failed");
        goto tx_rx_init_failed;
    }

    vnet_proc_debug_init();
    vnet_init_app_infos();
    atomic_set(&g_inited, 1);

    vnet_log_info("virtual network module init done");
    return SUCCESS;

tx_rx_init_failed:
    vnet_device_monitor_exit();
device_monitor_init_failed:
    vnet_session_mgr_exit();
session_mgr_init_failed:
    vnet_netlink_ctrl_exit();
netlink_ctrl_init_failed:
    return ERROR;
}

static void __exit vnet_exit(void)
{
    vnet_log_info("virtual network module exit");

    if (atomic_read(&g_inited) == 0)
        return;

    atomic_set(&g_inited, 0);
    vnet_clear_app_infos();
    vnet_proc_debug_exit();
    vnet_tx_rx_exit();
    vnet_session_mgr_exit();
    vnet_device_monitor_exit();
    vnet_netlink_ctrl_exit();

    vnet_log_info("virtual network module exit done");
    return;
}

module_init(vnet_init);
module_exit(vnet_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("virtual_network_module");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
