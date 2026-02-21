/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Define headfile of udk unload test module
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 06 23:04:22 2020
 */

#ifndef __UDKTEST_UNLOAD_H__
#define __UDKTEST_UNLOAD_H__

#define DRVNAME "udktest_unload"
#define DRVCOMPAT "udktest_unload"
#define CHRDEV_NAME "udktest_unload"
#define TEST_CHRDEV_NAME "udktest_unload_chrdev"

#define TEST_MINOR_START 0
#define TEST_MAX_DEV_NUM 255

int udktest_unload_driver(void);
int udktest_unload_device(void);

#endif /* __UDKTEST_UNLOAD_H__ */
