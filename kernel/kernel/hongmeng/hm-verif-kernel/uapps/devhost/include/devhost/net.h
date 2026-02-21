/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes for devhost exported APIs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#ifndef __DEVHOST_API_NET_H__
#define __DEVHOST_API_NET_H__

#include <devhost/device.h>

/*
 * CAUSION: This file will be used in linux source,
 *          **DO NOT** include any HMOS headers.
 */

int
devhost_netcall_nic_rx(unsigned int pool_offset, int shm_len, int rx_len, int offset, int ifindex);
int devhost_native_net_rx(unsigned int pool_offset, int shm_len,
			  int rx_len, int offset, int ifindex);
int devhost_netcall_nic_rx_ex(int num, void *data, int data_size);
int devhost_native_net_rx_ex(int num, void *data, int data_size);

void devhost_netdev_update_features(int ifindex, uint64_t features);
int devhost_acquire_netdev(int ifindex, struct device_info **pdev);
int devhost_netdev_alloc_ifindex(int ifindex);
int
devhost_register_netdev(struct device_info *net_dev,
			const char *name, uint64_t features, int *ifindex);
void
devhost_unregister_netdev(int ifindex);

#endif /* __DEVHOST_API_NET_H__ */
