/*
 * dmabuf_reclaim_dev.h
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
#ifndef _DMABUF_RECLAIM_DEV_H
#define _DMABUF_RECLAIM_DEV_H

#include <linux/ioctl.h>

#define DMA_BUF_RECLAIM_IOC_MAGIC 'd'

struct dma_reclaim_ioctl_sw_para {
	pid_t pid;
	unsigned long ino;
	unsigned int fd;
};

#define DMA_BUF_RECLAIM_INIT_ESWAP_SPACE _IOW(DMA_BUF_RECLAIM_IOC_MAGIC, 0x01, int) // init dmabuf reclaim swap space
#define DMA_BUF_RECLAIM_SW_TO_BACKGROUND _IOWR(DMA_BUF_RECLAIM_IOC_MAGIC, 0x04, int) // reclaim dmabuf based on ino's value
#define DMA_BUF_RECLAIM_SW_TO_FRONT _IOWR(DMA_BUF_RECLAIM_IOC_MAGIC, 0x05, int) // resume dmabuf in one process
#define DMA_BUF_RECLAIM_APP_DIE _IOWR(DMA_BUF_RECLAIM_IOC_MAGIC, 0x06, int) // notify the deconstructed process management structure
#define DMA_BUF_RECLAIM_FD _IOWR(DMA_BUF_RECLAIM_IOC_MAGIC, 0x07, int) // reclaim dmabuf based on fd's value
#define DMA_BUF_RESUME_FD _IOWR(DMA_BUF_RECLAIM_IOC_MAGIC, 0x08, int) // resume dmabuf based on fd's value

#endif /* _DMABUF_RECLAIM_DEV_H */