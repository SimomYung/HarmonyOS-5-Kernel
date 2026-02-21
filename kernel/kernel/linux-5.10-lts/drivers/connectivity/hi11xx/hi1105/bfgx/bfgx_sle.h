/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 *
 * Description: bfgx_platform_msg_handle.c header file
 * Author: @CompanyNameTag
 */

#ifndef BFGX_SLE_H
#define BFGX_SLE_H

/* 其他头文件包含 */
#include <linux/miscdevice.h>
#include "hw_bfg_ps.h"
#include "bfgx_dev.h"

#define SLE_IOCTL_MAGIC             'S'
#define SLE_IOCTL_OFF_FIND_CMD     _IOW(SLE_IOCTL_MAGIC, 1, int)
struct bfgx_sle_private_data {
    struct bt_data_combination data_combination;
};

/* 函数声明 */
struct bfgx_subsys_driver_desc *get_sle_default_drv_desc(void);
ssize_t hw_sle_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t hw_sle_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
long hw_sle_ioctl(struct file *filp, uint32_t cmd, unsigned long arg);
uint32_t hw_sle_poll(struct file *filp, poll_table *wait);
#endif /* __BFGX_SLE_H__ */
