/*
 * dmabuf_reclaim.c
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
#include <asm/page.h>
#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/dma-heap.h>
#include <linux/err.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/iommu.h>
#include <linux/iommu/mm_iommu.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/pagemap.h>
#include <linux/proc_fs.h>
#include <linux/scatterlist.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/dmabuf/mm_dma_heap.h>
#include <linux/dmabuf/dmabuf_reclaim.h>
#include <linux/dmabuf/dmabuf_reclaim_manage.h>
#include <linux/dmabuf/dmabuf_swap.h>
#include <linux/dmabuf/dmabuf_map_info.h>

#include <asm/tlbflush.h>
#include <securec.h>

#include <linux/delay.h>
#include <linux/mutex.h>

#include "mm_heap_helpers.h"

static struct dmabuf_gpu_callback dmabuf_gpu_cb[DMABUF_CB_GPU_MAX];
static DEFINE_MUTEX(dmabuf_gpu_cb_mutex);
static DEFINE_MUTEX(dmabuf_reclaim_enable_mutex);
static bool dmabuf_reclaim_enable;

const char *dev_suffix = "hvgr";

unsigned long __pgprot_to_posix_prot(pgprot_t pgprot);

int dmabuf_register_gpu_callback(enum dmabuf_gpu_cb_type type,
	int (*cb)(struct dma_buf *dmabuf))
{
	unsigned int index = type;

	if (cb == NULL || type >= DMABUF_CB_GPU_MAX)
		return -EINVAL;

	mutex_lock(&dmabuf_gpu_cb_mutex);

	if (dmabuf_gpu_cb[index].cb != NULL) {
		pr_err("%s, had register callback func already\n", __func__);
		mutex_unlock(&dmabuf_gpu_cb_mutex);
		return -EEXIST;
	}

	dmabuf_gpu_cb[index].cb = cb;
	mutex_unlock(&dmabuf_gpu_cb_mutex);

	return 0;
}

int try_to_unmap_uva(struct dma_buf *dmabuf)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct buffer_map_info *info = NULL;
	int ret = 0;

	info = buffer_info_get(dmabuf->priv);
	if (!info)
		return -EINVAL;

	mutex_lock(&info->uva_info_mutex);
	list_for_each_safe(pos, q, &info->uva_info) {
		struct uva_map_info *uva_info =
				list_entry(pos, struct uva_map_info, list);

		if (uva_info->state == DMABUF_UNMAP)
			continue;
		ret = mm_dmabuf_page_runmap_user(uva_info->vma);
		if (ret)
			break;
		uva_info->state = DMABUF_UNMAP;
	}
	mutex_unlock(&info->uva_info_mutex);

	return ret;
}

int try_to_map_uva(struct dma_buf *dmabuf)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct buffer_map_info *info = NULL;
	int ret = 0;

	info = buffer_info_get(dmabuf->priv);
	if (!info)
		return -EINVAL;

	mutex_lock(&info->uva_info_mutex);
	list_for_each_safe(pos, q, &info->uva_info) {
		struct uva_map_info *uva_info =
				list_entry(pos, struct uva_map_info, list);

		if (uva_info->state == DMABUF_MAP)
			continue;
		ret = mm_dmabuf_page_rmap_user(dmabuf, uva_info->vma);
		if (ret)
			break;
		uva_info->state = DMABUF_MAP;
	}
	mutex_unlock(&info->uva_info_mutex);

	return ret;
}

int try_to_unmap_kva(struct dma_buf *dmabuf)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct buffer_map_info *info = NULL;
	int ret = 0;

	info = buffer_info_get(dmabuf->priv);
	if (!info)
		return -EINVAL;

	mutex_lock(&info->kva_info_mutex);
	list_for_each_safe(pos, q, &info->kva_info) {
		struct kva_map_info *kva_info =
				list_entry(pos, struct kva_map_info, list);

		if (kva_info->state == DMABUF_UNMAP)
			continue;
		mm_dmabuf_page_runmap_kernel(kva_info->kva, kva_info->len);
		kva_info->state = DMABUF_UNMAP;
	}
	mutex_unlock(&info->kva_info_mutex);

	return ret;
}

static int to_dmabuf_pages(struct dma_buf *dmabuf, struct page **pages, unsigned int npages)
{
	struct sg_table *table = NULL;
	struct scatterlist *sg = NULL;
	struct page **tmp = pages;
	int i, j;

	if (!dmabuf->priv)
		return -EINVAL;

	table = buffer_sgtable_get(dmabuf->priv);
	if (!table)
		return -EINVAL;

	for_each_sg(table->sgl, sg, table->nents, i) {
		int npages_this_entry = PAGE_ALIGN(sg->length) / PAGE_SIZE;
		struct page *page = sg_page(sg);

		WARN_ON(i >= npages);
		for (j = 0; j < npages_this_entry; j++)
			*(tmp++) = page++;
	}

	return 0;
}

int try_to_map_kva(struct dma_buf *dmabuf)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct buffer_map_info *info = NULL;
	struct page **pages = NULL;
	unsigned int npages;
	unsigned int flag;
	int ret = 0;

	npages = PAGE_ALIGN(dmabuf->size) / PAGE_SIZE;
	pages = vmalloc(sizeof(struct page *) * npages);
	if (!pages)
		return -EINVAL;

	ret = to_dmabuf_pages(dmabuf, pages, npages);
	if (ret) {
		ret = -EINVAL;
		goto done;
	}

	info = buffer_info_get(dmabuf->priv);
	if (!info) {
		ret = -EINVAL;
		goto done;
	}

	mutex_lock(&info->kva_info_mutex);
	list_for_each_safe(pos, q, &info->kva_info) {
		struct kva_map_info *kva_info =
				list_entry(pos, struct kva_map_info, list);

		if (kva_info->state == DMABUF_MAP)
			continue;
		flag = LIBLINUX_PAL_REMAP_CACHE;
		if (pgprot_val(pgprot_writecombine(kva_info->prot)) == pgprot_val(kva_info->prot))
			flag = LIBLINUX_PAL_REMAP_NORMAL_NC;

		ret = do_vmap(kva_info->kva, pages, npages, flag, __pgprot_to_posix_prot(kva_info->prot));
		if (ret < 0)
			break;
		kva_info->state = DMABUF_MAP;
	}
	mutex_unlock(&info->kva_info_mutex);

done:
	vfree(pages);

	return ret;
}

int try_to_unmap_iova(struct dma_buf *dmabuf)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct buffer_map_info *info = NULL;
	int ret = 0;

	info = buffer_info_get(dmabuf->priv);
	if (!info)
		return -EINVAL;

	mutex_lock(&info->iova_info_mutex);
	list_for_each_safe(pos, q, &info->iova_info) {
		struct iova_map_info *iova_info =
				list_entry(pos, struct iova_map_info, list);

		if (iova_info->state == DMABUF_UNMAP)
			continue;
		ret = mm_iommu_unmap_dmabuf_page(iova_info->dev, iova_info->iova, iova_info->len);
		if (ret)
			break;
		iova_info->state = DMABUF_UNMAP;
	}
	mutex_unlock(&info->iova_info_mutex);

	return ret;
}

int try_to_map_iova(struct dma_buf *dmabuf)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct buffer_map_info *info = NULL;
	int ret = 0;

	info = buffer_info_get(dmabuf->priv);
	if (!info)
		return -EINVAL;

	mutex_lock(&info->iova_info_mutex);
	list_for_each_safe(pos, q, &info->iova_info) {
		struct iova_map_info *iova_info =
				list_entry(pos, struct iova_map_info, list);

		if (iova_info->state == DMABUF_MAP)
			continue;
		ret = mm_iommu_map_dmabuf_page(iova_info->dev, dmabuf, iova_info->iova,
			iova_info->len, iova_info->prot);
		if (ret)
			break;
		iova_info->state = DMABUF_MAP;
	}
	mutex_unlock(&info->iova_info_mutex);

	return ret;
}

/* gva is not support now, if need, should add gva map/unmap call */
int try_to_unmap_gva(struct dma_buf *dmabuf)
{
	int ret = 0;

	if (dmabuf_gpu_cb[DMABUF_CB_GPU_UNMAP].cb)
		ret = dmabuf_gpu_cb[DMABUF_CB_GPU_UNMAP].cb(dmabuf);

	return ret;
}

int try_to_map_gva(struct dma_buf *dmabuf)
{
	int ret = 0;

	if (dmabuf_gpu_cb[DMABUF_CB_GPU_MAP].cb)
		ret = dmabuf_gpu_cb[DMABUF_CB_GPU_MAP].cb(dmabuf);

	return ret;
}

/* if need support gva, should add gva map/unmap in this func */
static int try_to_unmap_dmabuf_page(struct dma_buf *dmabuf)
{
	int ret = 0;
	ret = try_to_unmap_uva(dmabuf);
	if (ret) {
		pr_err("%s: try_to_unmap_uva fail\n", __func__);
		goto uva_unmap_failed;
	}

	ret = try_to_unmap_kva(dmabuf);
	if (ret) {
		pr_err("%s: try_to_unmap_kva fail\n", __func__);
		goto kva_unmap_failed;
	}

	ret = try_to_unmap_iova(dmabuf);
	if (ret) {
		pr_err("%s: try_to_unmap_iova fail\n", __func__);
		goto iova_unmap_failed;
	}

	if (buffer_attach_num_get(dmabuf->priv)) {
		ret = try_to_unmap_gva(dmabuf);
		if (ret) {
			pr_err("%s: try_to_unmap_gva fail\n", __func__);
			goto gva_unmap_failed;
		}
	}

	return 0;
/* gpu will deal gva map in hmos */
gva_unmap_failed:
iova_unmap_failed:
	if (try_to_map_iova(dmabuf))
		pr_err("%s: try_to_map_iova fail\n", __func__);
kva_unmap_failed:
	if (try_to_map_kva(dmabuf))
		pr_err("%s: try_to_map_kva fail\n", __func__);
uva_unmap_failed:
	if (try_to_map_uva(dmabuf))
		pr_err("%s: try_to_map_uva fail\n", __func__);

	return ret;
}

static int try_to_map_dmabuf_page(struct dma_buf *dmabuf)
{
	int ret = 0;
	bool is_from_resume = false;

	ret = try_to_map_uva(dmabuf);
	if (ret) {
		pr_err("%s: try_to_map_uva fail\n", __func__);
		goto uva_map_failed;
	}

	ret = try_to_map_kva(dmabuf);
	if (ret) {
		pr_err("%s: try_to_map_kva fail\n", __func__);
		goto kva_map_failed;
	}

	if (buffer_reclaim_status_get(dmabuf->priv)) {
		buffer_reclaim_status_set(dmabuf->priv, false);
		is_from_resume = true;
	}

	ret = try_to_map_iova(dmabuf);
	if (ret) {
		pr_err("%s: try_to_map_uva fail\n", __func__);
		goto iova_map_failed;
	}

	ret = try_to_map_gva(dmabuf);
	if (ret) {
		pr_err("%s: try_to_map_gva fail\n", __func__);
		goto gva_map_failed;
	}
 
	return 0;
/* gpu will deal gva unmap in hmos */
gva_map_failed:
iova_map_failed:
	if (try_to_unmap_iova(dmabuf))
		pr_err("%s: try_to_unmap_iova fail\n", __func__);
kva_map_failed:
	if (try_to_unmap_kva(dmabuf))
		pr_err("%s: try_to_unmap_kva fail\n", __func__);
uva_map_failed:
	if (try_to_unmap_uva(dmabuf))
		pr_err("%s: try_to_unmap_uva fail\n", __func__);
	if (is_from_resume)
		buffer_reclaim_status_set(dmabuf->priv, true);

	return ret;
}

/* Not used currently */
static void init_dmabuf_page(struct dma_buf *dmabuf)
{
}

/* Not used currently */
static void clear_dmabuf_page(struct dma_buf *dmabuf)
{
}

static bool dmabuf_is_attached(struct dma_buf *dmabuf)
{
	struct dma_buf_attachment *attach_obj;
	size_t dev_suffix_len = strlen(dev_suffix);
	
	if (!dmabuf) {
		return false;
	}
	list_for_each_entry(attach_obj, &dmabuf->attachments, node) {
		const char *name = dev_name(attach_obj->dev);
		if (name == NULL) {
			return false;
		}
		size_t name_len = strlen(name);
		if (dev_suffix_len > name_len || strcmp(name + name_len - dev_suffix_len, dev_suffix) != 0) {
			return false;
		}
	}
	return true;
}

static int __dmabuf_reclaim(struct dma_buf *dmabuf)
{
	int ret = 0;

	ret = dmabuf_page_swap_alloc_extid(dmabuf);
	if (ret) {
		pr_err("%s, alloc dmabuf extentid fail\n", __func__);
		return ret;
	}

	ret = try_to_unmap_dmabuf_page(dmabuf);
	if (ret) {
		pr_err("%s: try_to_unmap_dmabuf_page fail\n", __func__);
		goto free_extid;
	}

	dma_heap_direct_flush_buffer_cache(dmabuf->priv);

	/* dmabuf page data swap out */
	ret = dmabuf_page_swap_out(dmabuf);
	if (ret) {
		pr_err("%s, dmabuf page swap out fail\n", __func__);
		goto map_dmabuf;
	}

	clear_dmabuf_page(dmabuf);

	ret = buffer_free_sgtable(dmabuf->priv);
	if (ret) {
		pr_err("%s, dmabuf free sgtable fail\n", __func__);
		goto init_dmabuf;
	}

	pr_debug("%s, dmabuf page reclaim succ\n", __func__);
	return ret;

init_dmabuf:
	init_dmabuf_page(dmabuf);
map_dmabuf:
	(void)try_to_map_dmabuf_page(dmabuf);
free_extid:
	dmabuf_page_swap_free_extid(dmabuf);

	return ret;
}

bool is_dmabuf_reclaim_enable(void)
{
	bool ret = false;
	mutex_lock(&dmabuf_reclaim_enable_mutex);
	ret = dmabuf_reclaim_enable;
	mutex_unlock(&dmabuf_reclaim_enable_mutex);
	return ret;
}

int dmabuf_reclaim(struct dma_buf *dmabuf)
{
	struct mutex *lock = NULL;
	struct rw_semaphore *rwsem = NULL;
	int ret = 0;

	if (!dmabuf || !dmabuf->priv) {
		pr_err("%s, dmabuf or dmabuf->priv is NULL\n", __func__);
		return 0;
	}

	if (!is_dma_heap_dmabuf(dmabuf)) {
		pr_err("%s, is not dma_heap dmabuf\n", __func__);
		return 0;
	}

	rwsem = buffer_rwsem_get(dmabuf->priv);
	if (!rwsem) {
		pr_err("%s, buffer get rwsem fail\n", __func__);
		return 0;
	}

	ret = down_write_killable(rwsem);
	if (ret) {
		pr_err("%s, buffer down-ret-%d, exit\n", __func__, ret);
		return 0;
	}

	lock = buffer_lock_get(dmabuf->priv);
	if (!lock) {
		pr_err("%s, buffer get lock fail\n", __func__);
		goto release_rwsem;
	}

	mutex_lock(lock);

	if (!is_direct_reclaim_dmabuf(dmabuf))
		goto done;

	if (buffer_reclaim_status_get(dmabuf->priv))
		goto done;

	if (!dmabuf_is_attached(dmabuf)) {
		goto done;
	}

	ret = __dmabuf_reclaim(dmabuf);
	if (ret) {
		pr_err("%s, fail, ret[%d]\n", __func__, ret);
		goto done;
	}

	buffer_reclaim_status_set(dmabuf->priv, true);
done:
	mutex_unlock(lock);
release_rwsem:
	up_write(rwsem);

	return ret;
}

int mm_dmabuf_assign_reclaim(unsigned long ino_num)
{
	struct dma_buf *dmabuf = NULL;
	struct file *file;
	int ret;

	dmabuf = mm_dmaheap_dmabuf_get(ino_num);
        /*
	 * When waiting for a lock in the reclaim phase,
	 * the DMABUF may be released and the entire structure is destructed,
	 * which causes abnormal pointer access. Therefore,
	 * the count of DMABUF file is added before the reclaim phase to ensure that
	 * the DMABUF is not released during reclaim.
	 */
	if (dmabuf == NULL || dmabuf->file == NULL || atomic64_add_unless(&dmabuf->file->f_count, 1, 0) == 0) {
		pr_err("%s, dmabuf get by ino_num[%lu] fail\n", __func__, ino_num);
		return 0;
	}
	ret = dmabuf_reclaim(dmabuf);
	fput(dmabuf->file);

	return ret;
}

static int __dmabuf_resume(struct dma_buf *dmabuf)
{
	int ret = 0;

	ret = buffer_alloc_sgtable(dmabuf->priv);
	if (ret) {
		pr_err("%s, dmabuf realloc sgtable fail\n", __func__);
		return -EINVAL;
	}
 
	/* dmabuf page data swap in */
	ret = dmabuf_page_swap_in(dmabuf);
	if (ret) {
		pr_err("%s, dmabuf page swap in fail\n", __func__);
		goto free_sgtable;
	}

	dma_heap_direct_flush_buffer_cache(dmabuf->priv);

	ret = try_to_map_dmabuf_page(dmabuf);
	if (ret) {
		pr_err("%s, try_to_map_dmabuf_page fail\n", __func__);
		goto free_sgtable;
	}

	init_dmabuf_page(dmabuf);

	ret = dmabuf_page_swap_free_extid(dmabuf);
	if (ret)
		pr_err("%s, free extid fail\n", __func__);

	return ret;

free_sgtable:
	(void)buffer_free_sgtable(dmabuf->priv);

	return ret;
}

int dmabuf_resume(struct dma_buf *dmabuf)
{
	struct mutex *lock = NULL;
	struct rw_semaphore *rwsem = NULL;
	int ret = 0;

	if (!dmabuf || !dmabuf->priv) {
		pr_err("%s, dmabuf or dmabuf->priv is NULL\n", __func__);
		return 0;
	}

	if (!is_dma_heap_dmabuf(dmabuf)) {
		pr_err("%s, is not dma_heap dmabuf\n", __func__);
		return 0;
	}

	rwsem = buffer_rwsem_get(dmabuf->priv);
	if (!rwsem) {
		pr_err("%s, buffer get rwsem fail\n", __func__);
		return 0;
	}

	ret = down_write_killable(rwsem);
	if (ret) {
		pr_err("%s, buffer down-ret-%d, exit\n", __func__, ret);
		return 0;
	}

	lock = buffer_lock_get(dmabuf->priv);
	if (!lock) {
		pr_err("%s, buffer get lock fail\n", __func__);
		goto release_rwsem;
	}

	mutex_lock(lock);

	if (!is_direct_reclaim_dmabuf(dmabuf))
		goto done;

	if (!buffer_reclaim_status_get(dmabuf->priv))
		goto done;

	ret = __dmabuf_resume(dmabuf);
	if (ret) {
		pr_err("%s, fail, ret[%d]\n", __func__, ret);
		goto done;
	}
done:
	mutex_unlock(lock);
release_rwsem:
	up_write(rwsem);

	return ret;
}

int mm_dmabuf_assign_resume(unsigned long ino_num)
{
	struct dma_buf *dmabuf = NULL;
	int ret;

	dmabuf = mm_dmaheap_dmabuf_get(ino_num);
        /*
	 * When waiting for a lock in the resume phase,
	 * the DMABUF may be released and the entire structure is destructed,
	 * which causes abnormal pointer access. Therefore,
	 * the count of DMABUF file is added before the resume phase to ensure that
	 * the DMABUF is not released during resume.
	 */
	if (dmabuf == NULL || dmabuf->file == NULL || atomic64_add_unless(&dmabuf->file->f_count, 1, 0) == 0) {
		pr_err("%s, dmabuf get by ino_num[%lu] fail\n", __func__, ino_num);
		return 0;
	}
	ret = dmabuf_resume(dmabuf);
	fput(dmabuf->file);

	return ret;
}

static int dmabuf_reclaim_enable_show(struct seq_file *m, void *v)
{
	seq_printf(m, "dmabuf_reclaim_enable = %d\n", dmabuf_reclaim_enable);
	return 0;
}

static int dmabuf_reclaim_enable_open(struct inode *inode, struct file *file)
{
	return single_open(file, dmabuf_reclaim_enable_show, NULL);
}

int mm_dmabuf_init_swap_space(unsigned long total_size)
{
	int ret = 0;
	mutex_lock(&dmabuf_reclaim_enable_mutex);
	if (!dmabuf_reclaim_enable) {
		pr_debug("[%s] will call dmabuf_page_swap_space_init\n", __func__);
		ret = dmabuf_page_swap_space_init(total_size);
		if (ret == 0) {
			dmabuf_reclaim_enable = true;
		} else {
			pr_err("%s: dmabuf page swap space init failed!\n", __func__);
		}
	} else {
		pr_debug("%s: dmabuf reclaim is already enable!\n", __func__);
	}
	mutex_unlock(&dmabuf_reclaim_enable_mutex);
	return ret;
}

#define DMABUF_SWAP_SIZE	(0x40000000)
static ssize_t dmabuf_reclaim_enable_write(struct file *file, const char __user *buf,
					size_t count, loff_t *ppos)
{
	char buffer[16] = {}; // 16: max buffer length
	int enable = -1;
	int ret = 0;

	memset_s(buffer, sizeof(buffer), 0, sizeof(buffer));
	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;

	if (copy_from_user(buffer, buf, count)) {
		return -EFAULT;
	}

	if (sscanf_s(buffer, "%d", &enable) != 1) { // 1: one param
		pr_err("Input parameters are invalid\n");
		return -EFAULT;
	}

	pr_info("dmabuf_reclaim_enable_write: enable=%d\n", enable);
	if (enable == 1) {
		mutex_lock(&dmabuf_reclaim_enable_mutex);
		if (!dmabuf_reclaim_enable) {
			pr_debug("will call dmabuf_page_swap_space_init\n");
			ret = dmabuf_page_swap_space_init(DMABUF_SWAP_SIZE);
			if (ret == 0) {
				pr_err("dmabuf_page_swap_space_init success\n");
				dmabuf_reclaim_enable = true;
			}
		}
		mutex_unlock(&dmabuf_reclaim_enable_mutex);
	} else if (enable == 0) {
		mutex_lock(&dmabuf_reclaim_enable_mutex);
		dmabuf_reclaim_enable = false;
		mutex_unlock(&dmabuf_reclaim_enable_mutex);
	} else {
		pr_err("dmabuf_reclaim_enable_write: invalid input = %d\n", enable);
	}

	if (ret) {
		pr_err("reclaim_dmabuf_enable_write: dmabuf_page_swap_space_init fail\n");
		return ret;
	}
	return count;
}

static const struct proc_ops enable_fops = {
	.proc_open = dmabuf_reclaim_enable_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
	.proc_write = dmabuf_reclaim_enable_write,
};

static int dmabuf_swapout_show(struct seq_file *m, void *v)
{
	seq_printf(m, "dmabuf_swapout\n"); 
	return 0;
}

static int dmabuf_swapout_open(struct inode *inode, struct file *file)
{
	return single_open(file, dmabuf_swapout_show, NULL);
}

static ssize_t dmabuf_swapout_write(struct file *file, const char __user *buf,
					size_t count, loff_t *ppos)
{
	char buffer[16] = {}; // 16: max buffer length
	unsigned long ino = 0;
	int ret = 0;

	memset_s(buffer, sizeof(buffer), 0, sizeof(buffer));
	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	if (sscanf_s(buffer, "%lu", &ino) != 1) { // 1: one params
		pr_err("Input parameters are invalid\n");
		return -EFAULT;
	}

	pr_debug("dmabuf_swapout_write: ino=%lu\n", ino);
	ret = mm_dmabuf_assign_reclaim(ino);
	if (ret) {
        pr_err("dmabuf_swapout_write: ino=%lu fail\n", ino);
		return ret;
	}
	return count;
}

static const struct proc_ops swapout_fops = {
	.proc_open = dmabuf_swapout_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
	.proc_write = dmabuf_swapout_write,
};

static int dmabuf_swapin_show(struct seq_file *m, void *v)
{
	seq_printf(m, "dmabuf_swapin\n"); 
	return 0;
}

static int dmabuf_swapin_open(struct inode *inode, struct file *file)
{
	return single_open(file, dmabuf_swapin_show, NULL);
}

static ssize_t dmabuf_swapin_write(struct file *file, const char __user *buf,
					size_t count, loff_t *ppos)
{
	char buffer[16]; // 16: max buffer length
	unsigned long ino = 0;
	int ret = 0;

	memset_s(buffer, sizeof(buffer), 0, sizeof(buffer));
	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;

	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	if (sscanf_s(buffer, "%lu", &ino) != 1) { // 1: one params
		pr_err("Input parameters are invalid\n");
		return -EFAULT;
	}

	pr_debug("dmabuf_swapin_write: ino=%lu\n", ino);
	ret = mm_dmabuf_assign_resume(ino);
	if (ret) {
		pr_err("dmabuf_swapin_write: ino=%lu fail\n", ino);
        return ret;
    }
	return count;
}

static const struct proc_ops swapin_fops = {
	.proc_open = dmabuf_swapin_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
	.proc_write = dmabuf_swapin_write,
};

static int dmabuf_reclaim_file_node_init(void)
{
	struct proc_dir_entry *entry = NULL;

	entry = proc_create_data("ldk_reclaim_process_dmabuf_enable", 0664,
				NULL, &enable_fops, NULL);
	if (!entry) {
		pr_err("Failed to create swapout_process_dmabuf\n");
		return -EFAULT;
	}

	entry = proc_create_data("ldk_swapout_process_dmabuf", 0664,
				NULL, &swapout_fops, NULL);
	if (!entry) {
		pr_err("Failed to create swapout_process_dmabuf\n");
		return -EFAULT;
	}

	entry = proc_create_data("ldk_swapin_process_dmabuf", 0664,
				NULL, &swapin_fops, NULL);
	if (!entry) {
		pr_err("Failed to create swapin_process_dmabuf\n");
		return -EFAULT;
	}

	return 0;
}

static int __init dmabuf_reclaim_init(void)
{
	int ret;

	mutex_lock(&dmabuf_reclaim_enable_mutex);
	dmabuf_reclaim_enable = false;

	ret = dmabuf_reclaim_file_node_init();
	if (ret) {
		pr_err("%s, create file node fail\n", __func__);
		goto err;
	}
	mutex_unlock(&dmabuf_reclaim_enable_mutex);

	pr_err("%s: init succ!\n", __func__);
	return 0;

err:
	pr_err("%s: init fail!\n", __func__);

	mutex_unlock(&dmabuf_reclaim_enable_mutex);
	return ret;
}

subsys_initcall(dmabuf_reclaim_init);
