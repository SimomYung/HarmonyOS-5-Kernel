/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 *
 * Description: bfgx_bt.c header file
 * Author: @CompanyNameTag
 */

#ifndef BFGX_BT_H
#define BFGX_BT_H
#include "hw_bfg_ps.h"
#include "bfgx_dev.h"

#define BT_IOCTL_MAGIC             'S'
#define BT_IOCTL_OFF_FIND_CMD     _IOW(BT_IOCTL_MAGIC, 1, int)

#ifndef _PRE_BT_IOCTL_DEF_OLD_VER
#define BT_IOCTL_FAKE_CLOSE_CMD     _IOW(BT_IOCTL_MAGIC, 100, int)
#define BT_IOCTL_HCISETPROTO        _IOW(BT_IOCTL_MAGIC, 101, int)
#define BT_IOCTL_HCIUNSETPROTO      _IOW(BT_IOCTL_MAGIC, 102, int)
#define BT_IOCTL_OPEN               _IOW(BT_IOCTL_MAGIC, 103, int)
#define BT_IOCTL_RELEASE            _IOW(BT_IOCTL_MAGIC, 104, int)
#else
/* Pangux PC项目涉及第三方公司代码,暂时沿用原有的不规范定义,后续有新增需求时同步修改 */
#define BT_IOCTL_FAKE_CLOSE_CMD      100
#define BT_IOCTL_HCISETPROTO         101
#define BT_IOCTL_HCIUNSETPROTO       102
#define BT_IOCTL_OPEN                103
#define BT_IOCTL_RELEASE             104
#endif

#define BT_FAKE_CLOSE               1
#define BT_REAL_CLOSE               0

struct bfgx_bt_private_data {
    struct bt_data_combination data_combination;
};

struct bfgx_subsys_driver_desc *get_bt_default_drv_desc(void);
ssize_t hw_bt_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t hw_bt_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
uint32_t hw_bt_poll(struct file *filp, poll_table *wait);
long hw_bt_ioctl(struct file *filp, uint32_t cmd, unsigned long arg);
#endif // _BFGX_BT_H_
