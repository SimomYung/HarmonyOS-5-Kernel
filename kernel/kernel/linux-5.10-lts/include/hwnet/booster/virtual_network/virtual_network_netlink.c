/*
 * virtual_network_netlink.c
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

#include "virtual_network_netlink.h"
#include "virtual_network.h"
#include "virtual_network_utils.h"

#include <linux/if.h>
#include <linux/slab.h>
#include <net/sock.h>
#include <net/netlink.h>
#include <uapi/linux/netlink.h>

#ifndef NETLINK_BOOSTERD
#define NETLINK_BOOSTERD 47
#endif

#define NETLINK_MSG_LEN_MAX 4096

enum vnet_nl_msg_header_type {
    SET_USER_PID = 0,
    NETLINK_CMD_MSG = 1,
    NL_MSG_HEADER_NTYPE_NUM_MAX
};

typedef struct vnet_netlink_ctrl {
    struct sock *dev_sock;
    unsigned int user_pid;
    atomic_t status;
    struct vnet_list_queue receive_queue;
    struct work_struct receive_work;
    struct vnet_list_queue report_queue;
    struct work_struct report_work;
    struct tasklet_struct report_tasklet;
} vnet_netlink_ctrl_t;

static vnet_netlink_ctrl_t *g_netlink_ctrl = NULL;

static vnet_netlink_msg_handle_t g_msg_type_module_dispatch_tbl[VNET_MSG_TYPE_NUM_MAX];

static void vnet_netlink_msg_dispatch(const vnet_netlink_msg_head_t *msg)
{
    u32 msg_len;
    if (unlikely(msg->msg_type >= VNET_MSG_TYPE_NUM_MAX)) {
        vnet_log_err("msg type %u id is invalid, error", msg->msg_type);
    }

    vnet_log_info("dispatch msg type %u", msg->msg_type);
    msg_len = msg->msg_len - sizeof(vnet_netlink_msg_head_t);
    if (g_msg_type_module_dispatch_tbl[msg->msg_type].msg_receive_func != NULL) {
        g_msg_type_module_dispatch_tbl[msg->msg_type].msg_receive_func(msg_len, msg->msg_data);
    }
}

static void vnet_netlink_msgs_dispatch_work(struct work_struct *work)
{
    vnet_netlink_ctrl_t *ctrl = NULL;
    struct vnet_list_node *node = NULL;

    if (unlikely(work == NULL)) {
        vnet_log_err("work is NULL, error");
        return;
    }

    ctrl = container_of(work, vnet_netlink_ctrl_t, receive_work);
    while (vnet_list_queue_empty(&(ctrl->receive_queue)) == 0) {
        node = vnet_dequeue_list_queue(&(ctrl->receive_queue));
        if (node == NULL) {
            vnet_log_err("dequeue a null node");
            break;
        }
        vnet_netlink_msg_dispatch((const vnet_netlink_msg_head_t *)(node->data));
        vnet_list_node_free(node);
        node = NULL;
    }
    return;
}

static void vnet_enqueue_netlink_msg(const void *msg_rev, unsigned int msg_len)
{
    vnet_netlink_msg_head_t *netlink_msg_head = (vnet_netlink_msg_head_t *)msg_rev;
    struct vnet_list_node *node = NULL;

    if (unlikely(msg_len <= sizeof(vnet_netlink_msg_head_t))) {
        vnet_log_err("msg_len %u to short", msg_len);
        return;
    }

    if (unlikely((netlink_msg_head->msg_len <= sizeof(vnet_netlink_msg_head_t)) ||
        (netlink_msg_head->msg_len > NETLINK_MSG_LEN_MAX) || (msg_len < netlink_msg_head->msg_len))) {
        vnet_log_err("msg buf len %u or msg_len %u invalid", msg_len, netlink_msg_head->msg_len);
        return;
    }

    node = vnet_list_node_alloc(msg_rev, netlink_msg_head->msg_len);
    if (unlikely(node == NULL)) {
        vnet_log_err("alloc node failed, error");
        return;
    }

    vnet_enqueue_list_queue(&(g_netlink_ctrl->receive_queue), node);
    schedule_work(&(g_netlink_ctrl->receive_work));
    return;
}

static void vnet_on_receive_netlink_msg(struct sk_buff *netlink_skb)
{
    struct sk_buff *skb = NULL;
    struct nlmsghdr *nlh = NULL;

    if (unlikely(netlink_skb == NULL || g_netlink_ctrl == NULL)) {
        vnet_log_err("NULL pointer, error");
        return;
    }

    skb = skb_get(netlink_skb);
    nlh = nlmsg_hdr(skb);
    if (unlikely(nlh == NULL)) {
        vnet_log_err("nlh is NULL");
        dev_kfree_skb_any(skb);
        return;
    }

    if (unlikely(!NLMSG_OK(nlh, skb->len) || (nlh->nlmsg_len > (NETLINK_MSG_LEN_MAX + NLMSG_HDRLEN)))) {
        vnet_log_err("nlmsg_len or skb->len is error");
        dev_kfree_skb_any(skb);
        return;
    }

    vnet_log_info("receive netlink msg type %u pid %u", nlh->nlmsg_type, nlh->nlmsg_pid);
    switch (nlh->nlmsg_type) {
        case SET_USER_PID:
            /* Store the user space daemon pid */
            g_netlink_ctrl->user_pid = nlh->nlmsg_pid;
            atomic_set(&(g_netlink_ctrl->status), 1);
            break;
        case NETLINK_CMD_MSG:
            vnet_enqueue_netlink_msg(NLMSG_DATA(nlh), nlh->nlmsg_len - NLMSG_HDRLEN);
            break;
        default:
            break;
    }
    dev_kfree_skb_any(skb);
    return;
}

static void vnet_netlink_send_unicast_data(vnet_netlink_ctrl_t *ctrl, struct vnet_list_node *node)
{
    struct sk_buff *skb = NULL;
    struct nlmsghdr *nlh = NULL;
    unsigned int msg_size;
    int ret;
    vnet_netlink_msg_head_t *msg = (vnet_netlink_msg_head_t *)(&node->data);

    vnet_log_info("send report type %u len %u", msg->msg_type, msg->msg_len);

    msg_size = (unsigned int)(NLMSG_SPACE((int)node->len));
    skb = alloc_skb(msg_size, GFP_ATOMIC);
    if (unlikely(skb == NULL)) {
        vnet_log_err("alloc skb failed");
        return;
    }

    nlh = nlmsg_put(skb, 0, 0, 0, msg_size - sizeof(struct nlmsghdr), 0);
    if (unlikely(nlh == NULL)) {
        vnet_log_err("nlh is NULL Pointer");
        dev_kfree_skb_any(skb);
        return;
    }
    NETLINK_CB(skb).portid = 0;
    NETLINK_CB(skb).dst_group = 0;
    if (memcpy_s(NLMSG_DATA(nlh), node->len, node->data, node->len) != 0) {
        vnet_log_err("memcpy node->data fail");
        dev_kfree_skb_any(skb);
        return;
    }

    ret = netlink_unicast(ctrl->dev_sock, skb, ctrl->user_pid, 0);
    if (ret < 0)
        vnet_log_err("netlink_unicast failed %d", ret);
    return;
}

static void vnet_netlink_msg_report_exec(vnet_netlink_ctrl_t *ctrl)
{
    struct vnet_list_queue *report_queue = &ctrl->report_queue;
    struct vnet_list_node *node = NULL;

    while (vnet_list_queue_empty(report_queue) == 0) {
        node = vnet_dequeue_list_queue(report_queue);
        if (node == NULL) {
            vnet_log_err("dequeue a null node");
            break;
        }

        vnet_netlink_send_unicast_data(ctrl, node);
        vnet_list_node_free(node);
        node = NULL;
    }
    return;
}

static void vnet_netlink_msg_report_work(struct work_struct *work)
{
    vnet_netlink_ctrl_t *ctrl = NULL;

    if (unlikely(work == NULL)) {
        vnet_log_err("work NULL Pointer");
        return;
    }

    ctrl = container_of(work, vnet_netlink_ctrl_t, report_work);
    vnet_netlink_msg_report_exec(ctrl);
    return;
}

static void vnet_netlink_msg_report_tasket(unsigned long arg)
{
    vnet_netlink_ctrl_t *ctrl = (vnet_netlink_ctrl_t *)arg;

    if (unlikely(ctrl == NULL)) {
        vnet_log_err("wifi_audio_netlink is NULL Pointer");
        return;
    }

    vnet_netlink_msg_report_exec(ctrl);
    return;
}

int vnet_netlink_ctrl_init(void)
{
    vnet_msg_type_t i;
    struct netlink_kernel_cfg netlink_cfg = {
        .groups = 0,
        .flags = 0,
        .input = vnet_on_receive_netlink_msg,
        .cb_mutex = NULL,
        .bind = NULL,
        .unbind = NULL,
        .compare = NULL,
    };

    vnet_log_info("netlink_ctrl init");

    if (unlikely(g_netlink_ctrl != NULL)) {
        vnet_log_err("netlink has inited");
        vnet_clean_list_queue(&(g_netlink_ctrl->receive_queue));
        vnet_clean_list_queue(&(g_netlink_ctrl->report_queue));
        return SUCCESS;
    }

    g_netlink_ctrl = (vnet_netlink_ctrl_t *)kmalloc(sizeof(vnet_netlink_ctrl_t), GFP_ATOMIC);
    if (unlikely(g_netlink_ctrl == NULL)) {
        vnet_log_err("kmalloc failed");
        return ERROR;
    }

    g_netlink_ctrl->dev_sock = netlink_kernel_create(&init_net, NETLINK_BOOSTERD, &netlink_cfg);
    if (unlikely(!g_netlink_ctrl->dev_sock)) {
        vnet_log_err("netlink kernel create error\n");
        kfree(g_netlink_ctrl);
        g_netlink_ctrl = NULL;
        return ERROR;
    }

    for (i = VNET_MSG_TYPE_NULL; i < VNET_MSG_TYPE_NUM_MAX; i++) {
        g_msg_type_module_dispatch_tbl[i].msg_type = i;
        g_msg_type_module_dispatch_tbl[i].msg_receive_func = NULL;
    }

    atomic_set(&(g_netlink_ctrl->status), 0);
    vnet_init_list_queue(&(g_netlink_ctrl->receive_queue));
    INIT_WORK(&(g_netlink_ctrl->receive_work), vnet_netlink_msgs_dispatch_work);
    vnet_init_list_queue(&(g_netlink_ctrl->report_queue));
    INIT_WORK(&(g_netlink_ctrl->report_work), vnet_netlink_msg_report_work);
    tasklet_init(&(g_netlink_ctrl->report_tasklet), vnet_netlink_msg_report_tasket, (unsigned long)g_netlink_ctrl);
    return SUCCESS;
}

void vnet_netlink_ctrl_exit(void)
{
    vnet_log_info("netlink_ctrl exit");
    if (unlikely(g_netlink_ctrl == NULL)) {
        vnet_log_err("netlink has exited");
        return;
    }

    if (likely(g_netlink_ctrl->dev_sock != NULL))
        netlink_kernel_release(g_netlink_ctrl->dev_sock);

    tasklet_kill(&(g_netlink_ctrl->report_tasklet));
    cancel_work_sync(&(g_netlink_ctrl->receive_work));
    cancel_work_sync(&(g_netlink_ctrl->report_work));
    vnet_clean_list_queue(&(g_netlink_ctrl->receive_queue));
    vnet_clean_list_queue(&(g_netlink_ctrl->report_queue));

    kfree(g_netlink_ctrl);
    g_netlink_ctrl = NULL;
    return;
}

void vnet_register_netlink_msg_handle(vnet_netlink_msg_handle_t *msg_handle)
{
    if (unlikely((msg_handle == NULL) || (msg_handle->msg_type >= VNET_MSG_TYPE_NUM_MAX))) {
        return;
    }
    g_msg_type_module_dispatch_tbl[msg_handle->msg_type].msg_receive_func = msg_handle->msg_receive_func;
}

void vnet_unregister_netlink_msg_handle(vnet_msg_type_t msg_type)
{
    if (unlikely(msg_type >= VNET_MSG_TYPE_NUM_MAX)) {
        return;
    }
    g_msg_type_module_dispatch_tbl[msg_type].msg_receive_func = NULL;
}

void vnet_send_netlink_report(const vnet_netlink_msg_head_t *msg, vnet_exec_type_t type)
{
    struct vnet_list_node *node = NULL;

    if (unlikely((g_netlink_ctrl == NULL) || (msg == NULL))) {
        vnet_log_err("NULL Pointer, error");
        return;
    }

    node = vnet_list_node_alloc((const void *)msg, msg->msg_len);
    if (unlikely(node == NULL)) {
        vnet_log_err("alloc node failed, error");
        return;
    }

    vnet_log_info("enqueue report type %u msg_len %u", msg->msg_type, msg->msg_len);
    vnet_enqueue_list_queue(&(g_netlink_ctrl->report_queue), node);
    if (type == VNET_TASKLET_EXEC) {
        tasklet_schedule(&(g_netlink_ctrl->report_tasklet));
    } else {
        schedule_work(&(g_netlink_ctrl->report_work));
    }
    return;
}
