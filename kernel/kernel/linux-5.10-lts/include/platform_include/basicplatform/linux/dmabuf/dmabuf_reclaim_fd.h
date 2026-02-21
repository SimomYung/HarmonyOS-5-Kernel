/*
 * dmabuf_reclaim_fd.h
 *
 * Copyright (C) 2024 Hisilicon Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef _DMABUF_RECLAIM_FD_H
#define _DMABUF_RECLAIM_FD_H

#include <linux/dma-buf.h>
#include <linux/err.h>

#ifdef CONFIG_DMABUF_RECLAIM
int mm_dmabuf_reclaim_fd(int pid, unsigned int fd);
int mm_dmabuf_resume_fd(int pid, unsigned int fd);
int mm_dmabuf_reclaim_ino(int pid, unsigned long ino);
int mm_dmabuf_resume_pid(int pid);
int mm_dmabuf_clear_pid(int pid);
int gpu_dmabuf_resume_fd(int fd);
int gpu_pf_dmabuf_resume_inos(unsigned long *inos, uint32_t nums);
int cpu_pf_dmabuf_resume_ino(unsigned long ino);
int mm_dmabuf_free_ext(struct dma_buf *dmabuf);
#else
static inline int mm_dmabuf_reclaim_ino(int pid, unsigned long ino)
{
	return -EINVAL;
}

static inline int mm_dmabuf_resume_pid(int pid)
{
	return -EINVAL;
}

static int mm_dmabuf_clear_pid(int pid)
{
	return -EINVAL;
}

static inline int gpu_dmabuf_resume_fd(int fd)
{
	return -EINVAL;
}

static inline int gpu_pf_dmabuf_resume_inos(unsigned long *inos, uint32_t nums)
{
	return -EINVAL;
}

static inline int cpu_pf_dmabuf_resume_ino(unsigned long ino)
{
	return -EINVAL;
}

static inline int mm_dmabuf_free_ext(struct dma_buf *dmabuf)
{
	return -EINVAL;
}
#endif /* CONFIG_DMABUF_RECLAIM */

#endif /* _DMABUF_RECLAIM_FD_H */