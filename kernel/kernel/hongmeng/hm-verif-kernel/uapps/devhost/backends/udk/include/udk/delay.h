/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: This is the header of udk busy-wait related interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 26 15:13:44 2019
 */

#ifndef __DEVHOST_UDK_DELAY_H__
#define __DEVHOST_UDK_DELAY_H__

void udk_udelay(unsigned long us);
void udk_mdelay(unsigned long ms);
void udk_ndelay(unsigned long ns);

#endif /* __DEVHOST_UDK_DELAY_H__ */
