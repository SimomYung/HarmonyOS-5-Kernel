/*
 * virtual_network_utils.c
 *
 * cloud network kernel module implementation
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
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

#include "virtual_network_utils.h"

#include <linux/slab.h>
#include <linux/list.h>

static int g_log_level = LOG_WRN;

void vnet_log_level_set(int level)
{
    if (level >= LOG_DBG && level <= LOG_NONE) {
        g_log_level = level;
    }
}

int vnet_log_level_get(void)
{
    return g_log_level;
}

struct vnet_list_node *vnet_list_node_alloc(const void *data, unsigned int len)
{
    struct vnet_list_node *node = NULL;
    int sec_ret;
    unsigned int size;

    if (len > LIST_NODE_DATA_LEN_MAX) {
        vnet_log_err("len %u > LIST_NODE_DATA_LEN_MAX", len);
        return NULL;
    }
    if (data == NULL) {
        vnet_log_err("data is NULL");
        return NULL;
    }

    size = len + sizeof(*node);
    node = (struct vnet_list_node *)kmalloc(size, GFP_ATOMIC);
    if (node == NULL) {
        vnet_log_err("kmalloc failed");
        return NULL;
    }
    node->len = len;
    sec_ret = memcpy_s(node->data, len, data, len);
    if (sec_ret != EOK) {
        vnet_log_err("memcpy data failed! sec_ret = %d", sec_ret);
        kfree(node);
        return NULL;
    }
    return node;
}

void vnet_list_node_free(struct vnet_list_node *node)
{
    if (node != NULL)
        kfree(node);
    return;
}

void vnet_init_list_queue(struct vnet_list_queue *queue)
{
    if (queue == NULL) {
        vnet_log_err("queue is NULL");
        return;
    }
    INIT_LIST_HEAD(&(queue->queue));
    queue->len = 0;
    spin_lock_init(&(queue->lock));
    return;
}

void vnet_enqueue_list_queue(struct vnet_list_queue *queue, struct vnet_list_node *node)
{
    unsigned long flags = 0;

    if ((queue == NULL) || (node == NULL)) {
        vnet_log_err("queue or node is NULL");
        return;
    }
    spin_lock_irqsave(&(queue->lock), flags);
    list_add_tail(&node->list, &(queue->queue));
    queue->len++;
    spin_unlock_irqrestore(&(queue->lock), flags);
    return;
}

struct vnet_list_node *vnet_dequeue_list_queue(struct vnet_list_queue *queue)
{
    struct vnet_list_node *node = NULL;
    unsigned long flags = 0;

    if (queue == NULL) {
        vnet_log_err("queue is NULL");
        return NULL;
    }
    spin_lock_irqsave(&(queue->lock), flags);
    node = list_first_entry(&(queue->queue), struct vnet_list_node, list);
    if (node == NULL) {
        spin_unlock_irqrestore(&(queue->lock), flags);
        vnet_log_err("get list first entry is NULL");
        return NULL;
    }
    list_del(&(node->list));
    queue->len--;
    spin_unlock_irqrestore(&(queue->lock), flags);
    return node;
}

int vnet_list_queue_empty(struct vnet_list_queue *queue)
{
    unsigned long flags = 0;
    int ret;

    if (queue == NULL) {
        vnet_log_err("queue is NULL");
        return 1; /* when queue is invalid, return 1 */
    }
    spin_lock_irqsave(&(queue->lock), flags);
    ret = list_empty(&(queue->queue));
    spin_unlock_irqrestore(&(queue->lock), flags);
    return ret;
}

unsigned int vnet_list_queue_size(struct vnet_list_queue *queue)
{
    unsigned long flags = 0;
    unsigned int queue_len;

    if (queue == NULL) {
        vnet_log_err("queue is NULL");
        return 0; /* when queue is invalid, return 0 */
    }
    spin_lock_irqsave(&(queue->lock), flags);
    queue_len = queue->len;
    spin_unlock_irqrestore(&(queue->lock), flags);
    return queue_len;
}

void vnet_clean_list_queue(struct vnet_list_queue *queue)
{
    struct vnet_list_node *node = NULL;

    if (queue == NULL) {
        vnet_log_err("queue is NULL");
        return;
    }

    while (vnet_list_queue_empty(queue) == 0) {
        node = vnet_dequeue_list_queue(queue);
        if (node == NULL) {
            vnet_log_err("dequeue a null node");
            break;
        }
        vnet_list_node_free(node);
        node = NULL;
    }
    return;
}
