/*
 * npu_os_api.c
 *
 * about npu mailbox
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
#include "npu_os_api.h"

#include <asm/cacheflush.h>
#include <linux/ion/mm_ion.h>
#ifdef CONFIG_NPU_BBIT_DEBUGFS
#include <linux/kprobes.h>
#endif

int npu_dma_buf_attach_lb(int fd, unsigned int pid, unsigned long offset, size_t size)
{
    return dma_buf_attach_lb(fd, pid, offset, size);
}
EXPORT_SYMBOL(npu_dma_buf_attach_lb);

int npu_dma_buf_detach_lb(int fd)
{
    return dma_buf_detach_lb(fd);
}
EXPORT_SYMBOL(npu_dma_buf_detach_lb);

bool npu_is_ion_dma_buf(struct dma_buf *dmabuf)
{
    return is_ion_dma_buf(dmabuf);
}
EXPORT_SYMBOL(npu_is_ion_dma_buf);

void __npu_inval_dcache_area(void *addr, size_t len)
{
    __inval_dcache_area(addr, len);
}
EXPORT_SYMBOL(__npu_inval_dcache_area);

#if defined(CONFIG_NPU_BBIT_DEBUGFS) || defined(CONFIG_NPU_BBIT_DEBUGFS_MODULE)
void npu_reset_current_kprobe(void)
{
    reset_current_kprobe();
}
EXPORT_SYMBOL(npu_reset_current_kprobe);
#endif