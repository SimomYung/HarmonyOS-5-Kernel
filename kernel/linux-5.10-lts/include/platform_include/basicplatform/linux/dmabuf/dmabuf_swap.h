/*
 * dmabuf_swap.h
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
#ifndef _DMABUF_SWAP_H
#define _DMABUF_SWAP_H

#include <linux/dma-buf.h>

#ifdef CONFIG_DMABUF_SWAP
/* GPU swap usage */
int mm_gpu_page_swap_space_init(unsigned long total_size);
int mm_gpu_page_array_out(struct page **page, int num, int *ext_ids);
int mm_gpu_page_array_in(struct page **page, int num, int *ext_ids);
void mm_gpu_page_array_free_extid(int num, int *ext_ids);

/* DMABUF swap usage */
int dmabuf_page_swap_space_init(unsigned long total_size);
int dmabuf_page_swap_out(struct dma_buf *dmabuf);
int dmabuf_page_swap_in(struct dma_buf *dmabuf);
int dmabuf_page_swap_alloc_extid(struct dma_buf *dmabuf);
int dmabuf_page_swap_free_extid(struct dma_buf *dmabuf);
#else
/* GPU swap usage */
static inline int mm_gpu_page_swap_space_init(unsigned long total_size)
{
	return -EINVAL;
}

static inline int mm_gpu_page_array_out(struct page **page, int num, int *ext_ids)
{
	return -EINVAL;
}

static inline int mm_gpu_page_array_in(struct page **page, int num, int *ext_ids)
{
	return -EINVAL;
}

static inline void mm_gpu_page_array_free_extid(int num, int *ext_ids)
{
}

/* DMABUF swap usage */
static inline int dmabuf_page_swap_space_init(unsigned long total_size)
{
	return -EINVAL;
}

static inline int dmabuf_page_swap_out(struct dma_buf *dmabuf)
{
	return -EINVAL;
}

static inline int dmabuf_page_swap_in(struct dma_buf *dmabuf)
{
	return -EINVAL;
}

static inline int dmabuf_page_swap_alloc_extid(struct dma_buf *dmabuf)
{
	return -EINVAL;
}

static inline int dmabuf_page_swap_free_extid(struct dma_buf *dmabuf)
{
	return -EINVAL;
}
#endif /* CONFIG_DMABUF_SWAP */
#endif /* _DMABUF_SWAP_H */
