/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Oct 20 17:49:32 2019
 */

#ifndef __UDK_TEST_BASE_SETUP_H__
#define __UDK_TEST_BASE_SETUP_H__

#include <sys/ioctl.h>

#define UDK_TEST_BASE_MODNAME "udktest.udk"
#define DEFAULT_DEVID 0
#define DEFAULT_CNODE_IDX (~0U)

#define TESTIO 0xEA

#define UDKTEST_DEVICE 1
#define UDKTEST_DRIVER 2
#define UDKTEST_IOCTL  3
#define UDKTEST_SETDHPID  4
#define UDKTEST_SHMLOCK   5

#define UDKTEST_IOC_DEVICE           _IO(TESTIO, UDKTEST_DEVICE)
#define UDKTEST_IOC_DRIVER           _IO(TESTIO, UDKTEST_DRIVER)
#define UDKTEST_IOC_IOCTL            _IO(TESTIO, UDKTEST_IOCTL)
#define UDKTEST_IOC_SETDHPID         _IO(TESTIO, UDKTEST_SETDHPID)
#define UDKTEST_IOC_SHMLOCK          _IO(TESTIO, UDKTEST_SHMLOCK)

#endif /* __UDK_TEST_BASE_SETUP_H__ */
