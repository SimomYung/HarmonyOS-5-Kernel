/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Special devices zero & null header
 * Author: Huawei OS Kernel Lab
 * Create: Thur May 18 15:50:54 2023
 */

#ifndef __UDK_OF_DEV_H__
#define __UDK_OF_DEV_H__

int udk_special_device_create(void);
devnum_t udk_zerodev_devt(void);

struct udk_dt_node;
struct udk_device;
int udk_of_device_instantiate(struct udk_dt_node *devnode,
			      struct udk_device *parent_udkdev);

struct gzeropg_page_s;
extern struct gzeropg_page_s *gzeropg_ptr(void);

#endif
