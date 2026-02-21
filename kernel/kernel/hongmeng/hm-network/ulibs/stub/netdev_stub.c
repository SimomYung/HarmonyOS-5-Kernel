/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Stub interface for network device
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 11 10:15:31 2022
 */

#include "hmnet/hmnet_devices.h"
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <sys/types.h>

int hm_netdev_rx(unsigned int cell_pool_offset, int shm_len, int rx_len,
		 int offset, int linux_devno)
{
	UNUSED(cell_pool_offset, shm_len, rx_len, offset, linux_devno);

	return E_HM_NOSYS;
}

int hm_netdev_rx_ex(int num, void *data, int data_size)
{
	UNUSED(num, data, data_size);

	return E_HM_NOSYS;
}

int hm_netdev_notify(enum hm_netdev_event event, const char *ifname, int linux_devno,
		     int option, uint64_t features)
{
	UNUSED(event, ifname, linux_devno, option, features);

	return E_HM_NOSYS;
}

int hm_netdev_update_features(int linux_devno, uint64_t features)
{
	UNUSED(linux_devno, features);

	return E_HM_NOSYS;
}

void hm_netdev_fifo_shm_init(uint32_t shm_size, char *fifo)
{
	UNUSED(shm_size, fifo);
}
