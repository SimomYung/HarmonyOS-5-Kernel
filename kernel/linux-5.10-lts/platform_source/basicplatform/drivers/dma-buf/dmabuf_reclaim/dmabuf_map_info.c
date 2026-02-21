/*
 * dmabuf_map_info.c
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
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/iommu.h>
#include <linux/sched.h>
#include <linux/sched/mm.h>
#include <linux/mm_types.h>
#include <linux/dmabuf/mm_dma_heap.h>
#include <linux/dmabuf/dmabuf_map_info.h>
#include <liblinux/pal.h>

static void __dump_uva_info(struct buffer_map_info *info)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;

	mutex_lock(&info->uva_info_mutex);
	list_for_each_safe(pos, q, &info->uva_info) {
		struct uva_map_info *uva_info =
				list_entry(pos, struct uva_map_info, list);
		pr_err("%s vma[0x%pK] vma_start[0x%llx] vma_end[0x%llx] addr[0x%llx] len[0x%llx]\n",
			__func__, uva_info->vma,
			uva_info->vma->vm_start, uva_info->vma->vm_end,
			uva_info->uva, uva_info->len);
	}
	mutex_unlock(&info->uva_info_mutex);
}

static void __dump_kva_info(struct buffer_map_info *info)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;

	mutex_lock(&info->kva_info_mutex);
	list_for_each_safe(pos, q, &info->kva_info) {
		struct kva_map_info *kva_info =
				list_entry(pos, struct kva_map_info, list);
		pr_err("%s kva base[0x%lx] len[0x%lx] prot[0x%lx]\n",
			__func__, kva_info->kva, kva_info->len, kva_info->prot);
	}
	mutex_unlock(&info->kva_info_mutex);
}

static void __dump_iova_info(struct buffer_map_info *info)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;

	mutex_lock(&info->iova_info_mutex);
	list_for_each_safe(pos, q, &info->iova_info) {
		struct iova_map_info *iova_info =
				list_entry(pos, struct iova_map_info, list);
		pr_err("%s dev[%s] iova[0x%llx] len[0x%llx] prot[0x%x]\n",
			__func__, dev_name(iova_info->dev),
			iova_info->iova, iova_info->len, iova_info->prot);
	}
	mutex_unlock(&info->iova_info_mutex);
}

static void __dump_gva_info(struct buffer_map_info *info)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;

	mutex_lock(&info->gva_info_mutex);
	list_for_each_safe(pos, q, &info->gva_info) {
		struct gva_map_info *gva_info =
				list_entry(pos, struct gva_map_info, list);
		pr_err("%s gva[0x%lx], len[0x%lx]\n",
			__func__, gva_info->gva, gva_info->len);
	}
	mutex_unlock(&info->gva_info_mutex);
}

static inline void __init_va_info(struct list_head *list,
					struct mutex *list_mutex)
{
	INIT_LIST_HEAD(list);
	mutex_init(list_mutex);
}

#define __deinit_va_info(pos, q, info, node, type, lock) ({ \
	mutex_lock(lock); \
	list_for_each_safe(pos, q, info) { \
		node = list_entry(pos, type, list); \
		list_del(pos); \
		kfree(node); \
	} \
	mutex_unlock(lock); \
})

int alloc_buffer_map_info(void *buf)
{
	struct buffer_map_info *info = NULL;

	info = kmalloc(sizeof(*info), GFP_KERNEL);
	if (!info) {
		pr_err("%s buffer map info alloc failed\n", __func__);
		return -ENOMEM;
	}

	__init_va_info(&info->iova_info, &info->iova_info_mutex);
	__init_va_info(&info->uva_info, &info->uva_info_mutex);
	__init_va_info(&info->kva_info, &info->kva_info_mutex);
	__init_va_info(&info->gva_info, &info->gva_info_mutex);

	buffer_info_set(buf, (void *)info);

	return 0;
}

void free_buffer_map_info(void *buf)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct iova_map_info *iova_info = NULL;
	struct uva_map_info *uva_info = NULL;
	struct kva_map_info *kva_info = NULL;
	struct gva_map_info *gva_info = NULL;
	struct buffer_map_info *info = NULL;

	info = buffer_info_get(buf);
	if (!info)
		return;

	__deinit_va_info(pos, q, &info->kva_info,
		kva_info, struct kva_map_info, &info->kva_info_mutex);

	__deinit_va_info(pos, q, &info->uva_info,
		uva_info, struct uva_map_info, &info->uva_info_mutex);

	__deinit_va_info(pos, q, &info->iova_info,
		iova_info, struct iova_map_info, &info->iova_info_mutex);

	__deinit_va_info(pos, q, &info->gva_info,
		gva_info, struct gva_map_info, &info->gva_info_mutex);

	kfree(info);
}

static int __modify_uva_in_list(struct uva_map_info *node,
	struct list_head *info_list, struct mutex *list_mutex,
	struct vm_area_struct *vma, enum va_ops_type type)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct uva_map_info *temp = NULL;
	struct uva_map_info *uva_in_list = NULL;
	int ret = 0;

	mutex_lock(list_mutex);
	list_for_each_safe(pos, q, info_list) {
		temp = list_entry(pos, struct uva_map_info, list);
		if (temp->vma == vma && temp->uva == vma->vm_start) {
			uva_in_list = temp;
			break;
		}
	}

	if ((type == __ADD_VA) && (!uva_in_list)) {
		list_add_tail(&node->list, info_list);
		pr_debug("%s add_va, mm[0x%pK], vma[0x%pK] vm_start[0x%llx] vm_end[0x%llx]\n",
			__func__, vma->vm_mm, vma, vma->vm_start, vma->vm_end);
	} else if ((type == __DELETE_VA) && uva_in_list) {
		pr_debug("%s delete_va, mm[0x%pK], vma[0x%pK] vm_start[0x%llx] vm_end[0x%llx]\n",
			__func__, vma->vm_mm, vma, vma->vm_start, vma->vm_end);
		list_del(&uva_in_list->list);
		kfree(uva_in_list);
	} else {
		/* something wrong in modify operation */
		ret = -1;
	}
	mutex_unlock(list_mutex);

	return ret;
}

int record_buffer_uva_info(void *buf,
	struct vm_area_struct *vma, unsigned long len)
{
	struct uva_map_info *uva_info = NULL;
	struct buffer_map_info *info = NULL;
	int ret;

	if (!is_direct_reclaim_buffer(buf))
		return 0;

	info = buffer_info_get(buf);
	if (!info)
		return -EINVAL;

	uva_info = kzalloc(sizeof(*uva_info), GFP_KERNEL);
	if (!uva_info) {
		pr_err("%s alloc failed\n", __func__);
		return -ENOMEM;
	}
	uva_info->vma = vma;
	uva_info->len = len;
	uva_info->uva = vma->vm_start;

	ret = __modify_uva_in_list(uva_info, &info->uva_info, &info->uva_info_mutex,
		vma, __ADD_VA);
	if (ret) {
		kfree(uva_info);
		pr_err("%s vm_start-0x%llx has already in info_list\n",
			__func__, vma->vm_start);
	} else {
		pr_debug("%s ok BUF-0x%pK vma-0x%pK vm-start-0x%llx\n",
			__func__, buf, vma, vma->vm_start);
	}

	return ret;
}

void delete_buffer_uva_info(void *buf, struct vm_area_struct *vma)
{
	struct buffer_map_info *info = NULL;
	int ret;

	if (!is_direct_reclaim_buffer(buf))
		return;

	info = buffer_info_get(buf);
	if (!info)
		return;

	ret = __modify_uva_in_list(NULL, &info->uva_info,
			&info->uva_info_mutex, vma, __DELETE_VA);
	if (ret) {
		__dump_uva_info(info);
		pr_err("%s caller-%pS vma-0x%pK vm_start-0x%llx has not find in info_list\n",
			__func__, (void *)_RET_IP_, vma, vma->vm_start);
	} else {
		pr_debug("%s succ, task-%s vma-0x%pK addr-0x%llx uva delete ok\n",
			__func__, current->comm, vma, vma->vm_start);
	}
}

static int __modify_kva_in_list(struct kva_map_info *node,
	struct list_head *info_list, struct mutex *list_mutex,
	unsigned long kva, enum va_ops_type type)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct kva_map_info *temp = NULL;
	struct kva_map_info *kva_in_list = NULL;
	int ret = 0;

	mutex_lock(list_mutex);
	list_for_each_safe(pos, q, info_list) {
		temp = list_entry(pos, struct kva_map_info, list);
		if (temp->kva == kva) {
			kva_in_list = temp;
			break;
		}
	}

	if ((type == __ADD_VA) && (!kva_in_list)) {
		list_add_tail(&node->list, info_list);
	} else if ((type == __DELETE_VA) && kva_in_list) {
		list_del(&kva_in_list->list);
		kfree(kva_in_list);
	} else {
		/* something wrong in modify operation */
		ret = -1;
	}
	mutex_unlock(list_mutex);

	return ret;
}

int record_buffer_kva_info(void *buf, unsigned long kva, unsigned long len, pgprot_t prot)
{
	struct kva_map_info *kva_info = NULL;
	struct buffer_map_info *info = NULL;
	int ret;

	if (!is_direct_reclaim_buffer(buf))
		return 0;

	info = buffer_info_get(buf);
	if (!info)
		return -EINVAL;

	kva_info = kzalloc(sizeof(*kva_info), GFP_KERNEL);
	if (!kva_info) {
		pr_err("%s alloc failed\n", __func__);
		return -ENOMEM;
	}
	kva_info->kva = kva;
	kva_info->len = len;
	kva_info->prot = prot;

	ret = __modify_kva_in_list(kva_info, &info->kva_info, &info->kva_info_mutex,
		kva_info->kva, __ADD_VA);
	if (ret) {
		pr_err("%s kva[0x%lx] already exist\n", __func__, kva);
		kfree(kva_info);
	} else {
		pr_debug("%s ok, kva[0x%lx]\n", __func__, kva_info->kva);
	}

	return ret;
}

void delete_buffer_kva_info(void *buf, unsigned long kva)
{
	struct buffer_map_info *info = NULL;
	int ret;

	if (!is_direct_reclaim_buffer(buf))
		return;

	info = buffer_info_get(buf);
	if (!info)
		return;

	ret = __modify_kva_in_list(NULL, &info->kva_info, &info->kva_info_mutex,
		kva, __DELETE_VA);
	if (ret) {
		__dump_kva_info(info);
		pr_info("%s caller-%pS kva[0x%lx] has not find in info_list\n",
			__func__, (void *)_RET_IP_, kva);
	} else {
		pr_debug("%s ok, kva[0x%lx]\n", __func__, kva);
	}
}

static int __modify_iova_in_list(struct iova_map_info *node,
	struct list_head *info_list, struct mutex *list_mutex,
	struct device *dev, unsigned long iova, enum va_ops_type type)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct iova_map_info *temp = NULL;
	struct iova_map_info *iova_in_list = NULL;
	int ret = 0;

	mutex_lock(list_mutex);
	list_for_each_safe(pos, q, info_list) {
		temp = list_entry(pos, struct iova_map_info, list);
		if (type == __DELETE_ALL_VA) {
			list_del(pos);
			kfree(temp);
			continue;
		}

		if (temp->dev == dev && temp->iova == iova) {
			iova_in_list = temp;
			break;
		}
	}

	if ((type == __ADD_VA) && (!iova_in_list)) {
		list_add_tail(&node->list, info_list);
	} else if ((type == __DELETE_VA) && iova_in_list) {
		list_del(&iova_in_list->list);
		kfree(iova_in_list);
	} else if (type == __DELETE_ALL_VA) {
		/* do nothing */
	} else {
		/* something wrong in modify operation */
		ret = -1;
	}
	mutex_unlock(list_mutex);

	return ret;
}

int record_buffer_iova_info(void *buf, struct device *dev,
	unsigned long iova, unsigned long len, int prot)
{
	struct buffer_map_info *info = NULL;
	struct iova_map_info *iova_info = NULL;
	int ret;

	if (!is_direct_reclaim_buffer(buf))
		return 0;

	info = buffer_info_get(buf);
	if (!info)
		return -EINVAL;

	iova_info = kzalloc(sizeof(*iova_info), GFP_KERNEL);
	if (!iova_info) {
		pr_err("%s alloc failed\n", __func__);
		return -ENOMEM;
	}
	iova_info->dev = dev;
	iova_info->iova = iova;
	iova_info->len = len;
	iova_info->prot = prot | IOMMU_READ | IOMMU_WRITE;

	ret = __modify_iova_in_list(iova_info, &info->iova_info,
			&info->iova_info_mutex, dev, iova, __ADD_VA);
	if (ret) {
		kfree(iova_info);
		pr_err("%s dev-%s iova-0x%lx len-0x%lx has already exist\n",
			__func__, dev_name(dev), iova, len);
	} else {
		pr_debug("%s ok dev-%s, iova-0x%lx len-0x%lx\n",
			__func__, dev_name(dev), iova, len);
	}

	return ret;
}

void delete_buffer_iova_info(void *buf,
	struct device *dev, unsigned long iova, bool is_delete_all)
{
	struct buffer_map_info *info = NULL;
	enum va_ops_type va_ops = __DELETE_VA;
	int ret;

	if (!is_direct_reclaim_buffer(buf))
		return;

	info = buffer_info_get(buf);
	if (!info)
		return;

	if (is_delete_all)
		va_ops = __DELETE_ALL_VA;

	ret = __modify_iova_in_list(NULL, &info->iova_info, &info->iova_info_mutex,
		dev, iova, va_ops);
	if (ret) {
		__dump_iova_info(info);
		pr_err("%s dev-%s iova-%lx has not find in info_list\n",
			__func__, dev ? dev_name(dev) : "delete_all", iova);
	} else {
		pr_debug("%s ok, dev-%s iova-%lx\n",
			__func__, dev ? dev_name(dev) : "delete_all", iova);
	}
}

static int __modify_gva_in_list(struct gva_map_info *node,
	struct list_head *info_list, struct mutex *list_mutex,
	unsigned long gva, enum va_ops_type type)
{
	struct list_head *pos = NULL;
	struct list_head *q = NULL;
	struct gva_map_info *temp = NULL;
	struct gva_map_info *gva_in_list = NULL;
	int ret = 0;

	mutex_lock(list_mutex);
	list_for_each_safe(pos, q, info_list) {
		temp = list_entry(pos, struct gva_map_info, list);
		if (temp->gva == gva) {
			gva_in_list = temp;
			break;
		}
	}

	if ((type == __ADD_VA) && (!gva_in_list)) {
		list_add_tail(&node->list, info_list);
	} else if ((type == __DELETE_VA) && gva_in_list) {
		list_del(&gva_in_list->list);
		kfree(gva_in_list);
	} else {
		/* something wrong in modify operation */
		ret = -1;
	}
	mutex_unlock(list_mutex);

	return ret;
}

int record_buffer_gva_info(void *buf, unsigned long gva, unsigned long len)
{
	struct buffer_map_info *info = NULL;
	struct gva_map_info *gva_info = NULL;
	int ret;

	if (!is_direct_reclaim_buffer(buf))
		return 0;

	info = buffer_info_get(buf);
	if (!info)
		return -EINVAL;

	gva_info = kzalloc(sizeof(*gva_info), GFP_KERNEL);
	if (!gva_info) {
		pr_err("%s alloc failed\n", __func__);
		return -ENOMEM;
	}
	gva_info->gva = gva;
	gva_info->len = len;

	ret = __modify_gva_in_list(gva_info, &info->gva_info,
			&info->gva_info_mutex, gva, __ADD_VA);
	if (ret) {
		kfree(gva_info);
		pr_err("%s gva-0x%lx len-0x%lx has already exist\n",
			__func__, gva, len);
	} else {
		pr_debug("%s ok gva-0x%lx len-0x%lx\n",
			__func__, gva, len);
	}

	return ret;
}

void delete_buffer_gva_info(void *buf, unsigned long gva)
{
	struct buffer_map_info *info = NULL;
	enum va_ops_type va_ops = __DELETE_VA;
	int ret;

	if (!is_direct_reclaim_buffer(buf))
		return;

	info = buffer_info_get(buf);
	if (!info)
		return;

	ret = __modify_gva_in_list(NULL, &info->gva_info, &info->gva_info_mutex,
		gva, va_ops);
	if (ret) {
		__dump_gva_info(info);
		pr_err("%s gva-%lx has not find in info_list\n",
			__func__, gva);
	} else {
		pr_debug("%s ok, gva-%lx\n",
			__func__, gva);
	}
}

void __dump_buffer_map_info(void *buf)
{
	struct buffer_map_info *info = NULL;

	info = buffer_info_get(buf);
	if (!info)
		return;

	__dump_uva_info(info);
	__dump_kva_info(info);
	__dump_iova_info(info);
	__dump_gva_info(info);
}
