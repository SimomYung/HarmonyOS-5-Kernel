/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Prototypes for net device support
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 31 12:18:44 2020
 */

#ifndef __DEVHOST_NETDEV_H__
#define __DEVHOST_NETDEV_H__

#define NET_CELL_POOL_UNIT_SIZE (1U << CONFIG_NET_POOL_UNIT_BITS)
#define NET_CELL_POOL_LEN       (1U << CONFIG_NET_POOL_BITS)

struct netdev_status;
struct io_uring;

int prepare_cell_pool_once(int *shm_id);
void prepare_cell_pool_cleanup(void);
struct io_uring *prepare_rx_uring_once(void);
void prepare_rx_uring_cleanup(void);
unsigned long long pool_offset2pa(unsigned int offset);
void devhost_netstat_inc_tx(void);
void devhost_netstat_get(struct netdev_status *status);

#endif /* __DEVHOST_NETDEV_H__ */
