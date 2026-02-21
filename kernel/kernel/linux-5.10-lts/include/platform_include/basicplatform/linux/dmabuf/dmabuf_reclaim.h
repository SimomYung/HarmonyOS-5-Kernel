/*
 * dmabuf_reclaim.h
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
#ifndef _DMABUF_RECLAIM_H
#define _DMABUF_RECLAIM_H

#include <linux/dma-buf.h>
#include <linux/err.h>

enum dmabuf_gpu_cb_type {
	DMABUF_CB_GPU_MAP = 0,
	DMABUF_CB_GPU_UNMAP = 1,
	DMABUF_CB_GPU_MAX
};

struct dmabuf_gpu_callback {
	int (*cb)(struct dma_buf *dmabuf);
};

#ifdef CONFIG_DMABUF_RECLAIM_TEST
int try_to_unmap_uva(struct dma_buf *dmabuf);
int try_to_map_uva(struct dma_buf *dmabuf);
int try_to_unmap_kva(struct dma_buf *dmabuf);
int try_to_map_kva(struct dma_buf *dmabuf);
int try_to_unmap_iova(struct dma_buf *dmabuf);
int try_to_map_iova(struct dma_buf *dmabuf);
int try_to_unmap_gva(struct dma_buf *dmabuf);
int try_to_map_gva(struct dma_buf *dmabuf);

void register_dmabuf_reclaim_test(struct kobject *kobj);
#endif

#ifdef CONFIG_DMABUF_RECLAIM
bool is_direct_reclaim_buffer(void *buf);
bool is_direct_reclaim_dmabuf(struct dma_buf *dmabuf);
int dmabuf_reclaim(struct dma_buf *dmabuf);
int dmabuf_resume(struct dma_buf *dmabuf);
int mm_dmabuf_assign_reclaim(unsigned long ino_num);
int mm_dmabuf_assign_resume(unsigned long ino_num);
int mm_dmabuf_init_swap_space(unsigned long total_size);
bool is_dmabuf_reclaim_enable(void);

/* gpu map/unmap register ops */
int dmabuf_register_gpu_callback(enum dmabuf_gpu_cb_type type,
				int (*cb)(struct dma_buf *dmabuf));
#else
static inline bool is_direct_reclaim_buffer(void *buf)
{
	return false;
}

static inline bool is_direct_reclaim_dmabuf(struct dma_buf *dmabuf)
{
	return false;
}

int mm_dmabuf_init_swap_space(unsigned long total_size)
{
	return -EINVAL;
}

static inline int dmabuf_reclaim(struct dma_buf *dmabuf)
{
	return -EINVAL;
}

static inline int dmabuf_resume(struct dma_buf *dmabuf)
{
	return -EINVAL;
}

static inline int mm_dmabuf_assign_reclaim(unsigned long ino)
{
	return -EINVAL;
}

static inline int mm_dmabuf_assign_resume(unsigned long ino)
{
	return -EINVAL;
}

static bool is_dmabuf_reclaim_enable()
{
	return false;
}

/* gpu map/unmap register ops */
static inline int dmabuf_register_gpu_callback(enum dmabuf_gpu_cb_type type,
				int (*cb)(struct dma_buf *dmabuf))
{
	return -EINVAL;
}
#endif /* CONFIG_DMABUF_RECLAIM */

#endif /* _DMABUF_RECLAIM_H */
