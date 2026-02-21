/*
 * vcodec_vdec_memory.c
 *
 * Implement vdec memory function, it used by both formal version
 * and smmu bypass version
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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
#include "vcodec_vdec.h"
#include "vcodec_vdec_memory.h"
#include "vcodec_vdec_plat.h"

static DEFINE_MUTEX(g_mem_mutex);

int32_t vdec_mem_probe(void *dev)
{
	int32_t ret;

	if (!dev) {
		dprint(PRN_ERROR, "vdec dev is null");
		return -EINVAL;
	}

	ret = dma_set_mask_and_coherent((struct device *)dev, ~0ULL);
	if (ret) {
		dprint(PRN_ERROR, "dma set mask and coherent failed, ret = %d", ret);
		return ret;
	}

	return 0;
}

void vdec_share_fd_list_init(struct vdec_fd_list *list)
{
	mutex_init(&list->lock);
	INIT_LIST_HEAD(&list->fdhead);
}

int32_t vdec_share_fd_list_get(struct vdec_fd_list *list, int32_t share_fd,
	uint32_t *iova_addr, uint32_t *iova_size)
{
	struct vdec_fd_listnode *pos = NULL;

	if (share_fd < 0) {
		dprint(PRN_ERROR, "invalid sharefd:%d", share_fd);
		return -EINVAL;
	}

	mutex_lock(&list->lock);
	list_for_each_entry(pos, &list->fdhead, head) {
		if (pos->share_fd == share_fd) {
			*iova_addr = pos->iova;
			*iova_size = pos->iova_size;
			mutex_unlock(&list->lock);
			return 0;
		}
	}

	dprint(PRN_ERROR, "invalid sharefd:%d", share_fd);
	mutex_unlock(&list->lock);
	return -EINVAL;
}

int32_t vdec_share_fd_list_add(struct vdec_fd_list *list, int32_t share_fd,
	struct dma_buf *dmabuf, uint32_t iova_addr, uint32_t iova_size)
{
	struct vdec_fd_listnode *pos = NULL;
	struct vdec_fd_listnode *node = NULL;

	mutex_lock(&list->lock);
	list_for_each_entry(pos, &list->fdhead, head) {
		if (pos->share_fd == share_fd) {
			dprint(PRN_ERROR, "duplicate sharefd:%d", share_fd);
			mutex_unlock(&list->lock);
			return -EFAULT;
		}
	}

	node = vmalloc(sizeof(struct vdec_fd_listnode));
	if (!node) {
		dprint(PRN_ERROR, "malloc fail");
		mutex_unlock(&list->lock);
		return -ENOMEM;
	}

	node->share_fd = share_fd;
	node->dmabuf = dmabuf;
	node->iova = iova_addr;
	node->iova_size = iova_size;

	dprint(PRN_DBG, "add share_fd:%d", node->share_fd);
	list_add_tail(&node->head, &list->fdhead);
	mutex_unlock(&list->lock);
	return 0;
}

int32_t vdec_share_fd_list_remove(struct vdec_fd_list *list, int32_t share_fd,
	struct dma_buf **dmabuf, uint32_t *iova_addr)
{
	struct vdec_fd_listnode *pos = NULL;
	struct vdec_fd_listnode *node = NULL;

	mutex_lock(&list->lock);
	list_for_each_entry_safe(pos, node, &list->fdhead, head) {
		if (pos->share_fd == share_fd) {
			*dmabuf = pos->dmabuf;
			*iova_addr = pos->iova;

			dprint(PRN_DBG, "remove share_fd:%d", pos->share_fd);
			pos->share_fd = -1;
			pos->dmabuf = VCODEC_NULL;
			pos->iova = VCODEC_NULL;
			pos->iova_size = 0;
			list_del(&pos->head);
			vfree(pos);
			mutex_unlock(&list->lock);
			return 0;
		}
	}

	dprint(PRN_ERROR, "invalid sharefd:%d", share_fd);
	mutex_unlock(&list->lock);
	return -EINVAL;
}

int32_t vdec_mem_iommu_map(void *dev, struct vdec_buffer_record *buf_record,
	struct file *file_handle, struct dma_buf **dmabuf)
{
	uint64_t iova_addr;
	unsigned long phy_size;
	vdec_entry *entry = vdec_get_entry();

	buf_record->iova = 0;

	vdec_check_ret(buf_record->share_fd >= 0, -EINVAL);

	vdec_mutex_lock(&g_mem_mutex);

	*dmabuf = dma_buf_get(buf_record->share_fd);
	if (IS_ERR_OR_NULL(*dmabuf)) {
		dprint(PRN_FATAL, "dma_buf_get failed");
		vdec_mutex_unlock(&g_mem_mutex);
		return -EFAULT;
	}

	iova_addr = kernel_iommu_map_dmabuf((struct device *)dev, *dmabuf, 0, &phy_size);
	if (!iova_addr) {
		dprint(PRN_FATAL, "kernel_iommu_map_dmabuf failed");
		dma_buf_put(*dmabuf);
		vdec_mutex_unlock(&g_mem_mutex);
		return -EFAULT;
	}

	buf_record->iova = (UADDR)iova_addr;
	buf_record->iova_size = (uint32_t)phy_size;

	entry->setting_info.ion_alloc_total_size += phy_size;
	vdec_mutex_unlock(&g_mem_mutex);
	dprint(PRN_DBG, "share_fd:%d, size:%u, virt_addr:%pK",
		buf_record->share_fd, buf_record->iova_size, buf_record->vir_addr);
	return 0;
}

int32_t vdec_mem_iommu_unmap(void *dev, int32_t share_fd, struct file *file_handle,
	struct dma_buf *dmabuf, uint32_t iova_addr)
{
	int32_t ret;

	vdec_check_ret(share_fd >= 0, -EINVAL);

	vdec_mutex_lock(&g_mem_mutex);

	ret = kernel_iommu_unmap_dmabuf((struct device *)dev, dmabuf, iova_addr);
	if (ret)
		dprint(PRN_ERROR, "kernel iommu unmap dmabuf failed");

	dma_buf_put(dmabuf);

	vdec_mutex_unlock(&g_mem_mutex);
	return ret;
}

void *vdec_dma_buf_get(int32_t share_fd)
{
	struct dma_buf *dmabuf = dma_buf_get(share_fd);

	if (IS_ERR_OR_NULL(dmabuf)) {
		dprint(PRN_FATAL, "dma_buf_get failed");
		return NULL;
	}
	return dmabuf;
}

void vdec_dma_buf_put(void *dmabuf)
{
	if (!dmabuf) {
		dprint(PRN_FATAL, "dma buf is null");
		return;
	}
	dma_buf_put((struct dma_buf *)dmabuf);
}

uint32_t *vdec_mmap(uint32_t addr, uint32_t range)
{
	uint32_t *res_addr = NULL;

	res_addr = (uint32_t *)ioremap(addr, range);
	return res_addr;
}

void vdec_munmap(uint32_t *mem_addr)
{
	if (!mem_addr) {
		dprint(PRN_ERROR, "params is invaild");
		return;
	}

	iounmap(mem_addr);
}

int32_t vdec_mem_iommu_vir_map(void *dev, struct vdec_hw_hal_mem_info *mem_info)
{
	uint64_t iova_addr;
	int32_t rc;
	unsigned long phy_size;
	struct dma_buf *dmabuf = VCODEC_NULL;
	void *virt_addr = NULL;
	vdec_entry *entry = vdec_get_entry();

	mem_info->iova = 0;
	mem_info->iova_size = 0;

	vdec_check_ret(mem_info->share_fd >= 0, -EINVAL);

	vdec_mutex_lock(&g_mem_mutex);

	dmabuf = mem_info->dmabuf;
	if (IS_ERR(dmabuf)) {
		dprint(PRN_FATAL, "dma_buf_get failed");
		vdec_mutex_unlock(&g_mem_mutex);
		return -EINVAL;
	}

	iova_addr = kernel_iommu_map_dmabuf((struct device *)dev, dmabuf, 0, &phy_size);
	if (!iova_addr) {
		dprint(PRN_FATAL, "kernel_iommu_map_dmabuf failed");
		goto exit;
	}

	if (mem_info->is_map_virtual == MEM_BUF_MAPPED) {
		rc = dma_buf_begin_cpu_access(dmabuf, DMA_BIDIRECTIONAL);
		if (rc < 0) {
			dprint(PRN_ERROR, "%s:dma_buf_begin_cpu_access failed", __func__);
			goto error_1;
		}

		virt_addr = dma_buf_kmap(dmabuf, 0);
		if (!virt_addr) {
			dprint(PRN_FATAL, "%s, kmap failed", __func__);
			goto error_2;
		}

		mem_info->virt_addr = virt_addr;
	}

	mem_info->iova = (uint32_t)iova_addr;
	mem_info->iova_size = phy_size;

	entry->setting_info.ion_alloc_total_size += phy_size;
	vdec_mutex_unlock(&g_mem_mutex);
	dprint(PRN_DBG, "size:%lu, virt_addr:%pK", mem_info->iova_size, mem_info->virt_addr);
	return 0;
error_2:
	rc = dma_buf_end_cpu_access(dmabuf, DMA_BIDIRECTIONAL);
	if (rc < 0)
		dprint(PRN_ERROR, "%s: dma buf end cpu access failed", __func__);
error_1:
	rc = kernel_iommu_unmap_dmabuf((struct device *)dev, dmabuf,  phy_size);
	if (rc != 0)
		dprint(PRN_ERROR, "%s: kernel iommu unmap dmabuf failed", __func__);
exit:
	dma_buf_put(dmabuf);
	vdec_mutex_unlock(&g_mem_mutex);
	return -EFAULT;
}

int32_t vdec_mem_iommu_vir_unmap(void *dev, struct vdec_hw_hal_mem_info *mem_info)
{
	struct dma_buf *dmabuf = VCODEC_NULL;
	int32_t ret;

	vdec_check_ret(mem_info->share_fd >= 0, -EINVAL);
	vdec_check_ret(mem_info->iova != 0, -EINVAL);

	vdec_mutex_lock(&g_mem_mutex);

	dmabuf = mem_info->dmabuf;
	if (IS_ERR(dmabuf)) {
		dprint(PRN_FATAL, "dma buf get failed");
		vdec_mutex_unlock(&g_mem_mutex);
		return -EINVAL;
	}

	ret = kernel_iommu_unmap_dmabuf((struct device *)dev, dmabuf, mem_info->iova);
	if (ret) {
		dprint(PRN_ERROR, "kernel iommu unmap dmabuf failed");
		goto exit;
	}

	if (mem_info->is_map_virtual == MEM_BUF_MAPPED) {
		dma_buf_kunmap(dmabuf, 0, NULL);

		ret = dma_buf_end_cpu_access(dmabuf, DMA_BIDIRECTIONAL);
		if (ret) 
			dprint(PRN_ERROR, "%s: end cpu access failed", __func__);
	}

exit:

	vdec_mutex_unlock(&g_mem_mutex);
	return ret;
}