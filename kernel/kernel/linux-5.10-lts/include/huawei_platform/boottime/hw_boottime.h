/*
  * drivers/boottime/hw_boottime.h
  *
  * boottime check
  *
  * Copyright (c) 2012-2020 Huawei Technologies Co., Ltd.
  *
  * This software is licensed under the terms of the GNU General Public
  * License version 2, as published by the Free Software Foundation, and
  * may be copied, distributed, and modified under those terms.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  * GNU General Public License for more details.
  *
  */

#ifndef __LINUX_HW_BOOTTIME_H__
#define __LINUX_HW_BOOTTIME_H__

#include <linux/module.h>
#ifndef CONFIG_HUAWEI_BOOT_TIME
void boot_record(const char *str)
{
}
int __init_or_module do_boottime_initcall(initcall_t fn)
{
}
#else
void boot_record(const char *str);
int __init_or_module do_boottime_initcall(initcall_t fn);
#endif
#define BOOT_STR_SIZE 128
#endif
