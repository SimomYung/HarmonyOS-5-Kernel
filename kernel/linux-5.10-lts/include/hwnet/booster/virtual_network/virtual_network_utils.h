/*
 * virtual_network_utils.h
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

#ifndef _VIRTUAL_NETWORK_UTILS_H_
#define _VIRTUAL_NETWORK_UTILS_H_

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>

#include <securec.h>

/* return code */
#define ERROR (-1)
#define SUCCESS 0

#define NUM_0 0
#define NUM_1 1
#define NUM_2 2
#define NUM_3 3
#define NUM_4 4
#define NUM_5 5
#define NUM_6 6
#define NUM_7 7
#define NUM_8 8
#define NUM_9 9
#define NUM_10 10
#define NUM_11 11
#define NUM_12 12
#define NUM_13 13
#define NUM_14 14
#define NUM_15 15
#define NUM_16 16
#define NUM_17 17
#define NUM_18 18
#define NUM_19 19
#define NUM_20 20
#define NUM_21 21
#define NUM_22 22
#define NUM_23 23
#define NUM_24 24
#define NUM_25 25
#define NUM_26 26
#define NUM_27 27
#define NUM_28 28
#define NUM_29 29
#define NUM_30 30
#define NUM_31 31
#define NUM_32 32
#define NUM_33 33
#define NUM_34 34
#define NUM_35 35
#define NUM_36 36
#define NUM_37 37
#define NUM_38 38
#define NUM_39 39
#define NUM_40 40
#define NUM_41 41
#define NUM_42 42
#define NUM_43 43
#define NUM_44 44
#define NUM_45 45
#define NUM_46 46
#define NUM_47 47
#define NUM_48 48

#define LIST_NODE_DATA_LEN_MAX 8192

void vnet_log_level_set(int level);
int vnet_log_level_get(void);

#define vnet_log_print(level, fmt, ...) \
    do { \
        if (vnet_log_level_get() > level) { \
            break; \
        } \
        pr_err(fmt, ##__VA_ARGS__); \
    } while (0)

#define vnet_log_debug(fmt, ...) \
    vnet_log_print(LOG_DBG, "VNET<DBG>[%s:%d]: " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define vnet_log_info(fmt, ...) \
    vnet_log_print(LOG_INF, "VNET<INF>[%s:%d]: " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define vnet_log_warn(fmt, ...) \
    vnet_log_print(LOG_WRN, "VNET<WRN>[%s:%d]: " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define vnet_log_err(fmt, ...) \
    vnet_log_print(LOG_ERR, "VNET<ERR>[%s:%d]: " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

enum vnet_log_level {
    LOG_DBG = 0,
    LOG_INF = 1,
    LOG_WRN = 2,
    LOG_ERR = 3,
    LOG_NONE = 4,
};

struct vnet_list_node {
    struct list_head list;
    unsigned int len;
    char data[0];
};

struct vnet_list_queue {
    struct list_head queue;
    unsigned int len;
    spinlock_t lock;
};

struct vnet_list_node *vnet_list_node_alloc(const void *data, unsigned int len);
void vnet_list_node_free(struct vnet_list_node *node);
void vnet_init_list_queue(struct vnet_list_queue *queue);
void vnet_enqueue_list_queue(struct vnet_list_queue *queue, struct vnet_list_node *node);
struct vnet_list_node *vnet_dequeue_list_queue(struct vnet_list_queue *queue);
int vnet_list_queue_empty(struct vnet_list_queue *queue);
unsigned int vnet_list_queue_size(struct vnet_list_queue *queue);
void vnet_clean_list_queue(struct vnet_list_queue *queue);

#endif /* _VIRTUAL_NETWORK_UTILS_H_ */
