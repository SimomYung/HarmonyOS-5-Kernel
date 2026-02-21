/*
 *  Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  Description: dubai lite data file
 *  Create: 2024-11-06
 */
#ifndef DUBAI_LITE_KERNEL_H
#define DUBAI_LITE_KERNEL_H

#include <linux/kernel.h>
#include <loadmonitor_plat.h>

struct lite_channel_data {
	u32 volt0;
	u32 volt1;
	u32 volt2;
	u32 volt3;
};

struct lite_data_size {
	u32 vote_col; /* volt level */
	u32 vote_row; /* channel + 1 */
};

#ifdef CONFIG_DUBAI_LITE_VOTE
void format_dubai_lite_data(struct dubai_lite *lite);
int clear_dubai_lite_data(void);
int ioctrl_get_dubai_lite_data(void __user *argp);
int ioctrl_get_dubai_lite_size(void __user *argp);
#else
void format_dubai_lite_data(struct dubai_lite *lite)
{
	(void)lite;
}
int clear_dubai_lite_data(void)
{
	return 0;
}
int ioctrl_get_dubai_lite_data(void __user *argp)
{
	(void)argp;
	return 0;
}
int ioctrl_get_dubai_lite_size(void __user *argp)
{
	(void)argp;
	return 0;
}
#endif
#endif
