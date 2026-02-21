/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK dtb interfaces defination
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 26 15:13:44 2019
 */

#ifndef __DEVHOST_UDK_DTB_H__
#define __DEVHOST_UDK_DTB_H__

void udk_set_dtb(void *dtb);
void *udk_get_dtb(void);
int udk_firmware_data(unsigned long long mpool_id, void **buf);
int udk_acquire_bootfdt(void **buf, unsigned long *size);

#endif /* __DEVHOST_UDK_DTB_H__ */
