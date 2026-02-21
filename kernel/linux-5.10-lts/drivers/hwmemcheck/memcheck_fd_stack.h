/*
 * memcheck_fd_stack.h
 *
 * save and read fd stack information, send signal to process
 *
 * Copyright (c) 2024 Huawei Technologies Co., Ltd.
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

#ifndef _MEMCHECK_FD_STACK_H
#define _MEMCHECK_FD_STACK_H

#include <linux/types.h>
#include <platform/linux/memcheck.h>

#ifdef CONFIG_DFX_MEMCHECK_STACK
int memcheck_fd_do_command(const struct track_cmd *cmd);
int memcheck_fd_stack_read(void *buf, struct fd_stack_info *info);
int memcheck_fd_stack_write(const void *buf, const struct fd_stack_info *info);
#else /* CONFIG_DFX_MEMCHECK_STACK */
static int memcheck_fd_do_command(const struct track_cmd *cmd)
{
	return -EINVAL;
}
static inline int memcheck_fd_stack_read(void *buf, struct fd_stack_info *info)
{
	return -EINVAL;
}
static inline int memcheck_fd_stack_write(void *buf, struct fd_stack_info *info)
{
	return -EINVAL;
}
#endif /* CONFIG_DFX_MEMCHECK_STACK */
#endif /* _MEMCHECK_DETAIL_H */
