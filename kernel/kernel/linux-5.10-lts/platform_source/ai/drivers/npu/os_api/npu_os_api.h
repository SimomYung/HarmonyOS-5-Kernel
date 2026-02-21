/*
 * npu_os_api.h
 *
 * about npu os api
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
#ifndef __NPU_OS_API_H
#define __NPU_OS_API_H

#include <linux/dma-buf.h>
#include <linux/types.h>

int npu_dma_buf_attach_lb(int fd, unsigned int pid, unsigned long offset, size_t size);
int npu_dma_buf_detach_lb(int fd);
bool npu_is_ion_dma_buf(struct dma_buf *dmabuf);
void __npu_inval_dcache_area(void *addr, size_t len);
#if defined(CONFIG_NPU_BBIT_DEBUGFS) || defined(CONFIG_NPU_BBIT_DEBUGFS_MODULE)
void npu_reset_current_kprobe(void);
#endif

#endif