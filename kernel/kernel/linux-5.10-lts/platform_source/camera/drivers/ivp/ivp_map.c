/*
 * This file define fd maps
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "ivp_map.h"
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/slab.h>
#ifdef __OHOS__
#include <linux/dmabuf/mm_dma_heap.h>
#else
#include <linux/ion.h>
#include <linux/ion/mm_ion.h>
#endif
#include <linux/dma-buf.h>
#include <linux/iommu/mm_iommu.h>
#include "ivp_log.h"

#ifndef EOK
#define EOK 0
#endif


int ivp_map_hidl_fd(struct device *dev, struct ivp_map_info *map_buf)
{
	struct dma_buf *buf = NULL;

	if (!dev || !map_buf) {
		ivp_err("invalid input");
		return -EINVAL;
	}

	if (map_buf->fd < 0) {
		ivp_err("fd is invalid");
		return -EINVAL;
	}

	buf = dma_buf_get(map_buf->fd);
	if (IS_ERR(buf)) {
		ivp_err("invalid file shared_fd = %d", map_buf->fd);
		return -EINVAL;
	}

	map_buf->iova = kernel_iommu_map_dmabuf(dev, buf,
		map_buf->fd_prot, &(map_buf->mapped_size));
	if (map_buf->iova == 0) {
		ivp_err("fail to map iommu iova");
		dma_buf_put(buf);
		return -ENOMEM;
	}

	dma_buf_put(buf);
	return EOK;
}

int ivp_unmap_hidl_fd(struct device *dev,
		struct ivp_map_info *map_buf)
{
	int ret = EOK;
	struct dma_buf *buf = NULL;

	if (!dev || !map_buf) {
		ivp_err("invalid input");
		return -EINVAL;
	}

	if (map_buf->fd < 0) {
		ivp_err("fd is invalid");
		return -EINVAL;
	}

	buf = dma_buf_get(map_buf->fd);
	if (IS_ERR(buf)) {
		ivp_err("fail to get dma dmabuf, fd = %d", map_buf->fd);
		return -EINVAL;
	}

	if (kernel_iommu_unmap_dmabuf(dev, buf, map_buf->iova) < 0) {
		ivp_err("fail to unmap fd = %d", map_buf->fd);
		ret = -ENOMEM;
	}

	map_buf->iova = 0;
	dma_buf_put(buf);
	return ret;
}


