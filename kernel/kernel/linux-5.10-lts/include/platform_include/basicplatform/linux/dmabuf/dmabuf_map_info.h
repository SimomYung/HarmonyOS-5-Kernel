
/*
 * dmabuf_map_info.h
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
#ifndef _DMABUF_MAP_INFO_H
#define _DMABUF_MAP_INFO_H

#include <linux/mutex.h>
#include <linux/types.h>
#include <linux/dma-buf.h>

/**
 * enum dmabuf_map_state - records the mapping status of the va info
 * only used in dmabuf reclaim and resume 
 */
enum dmabuf_map_state {
	DMABUF_NONE,
	DMABUF_MAP,
	DMABUF_UNMAP,
};

/**
 * struct iova_map_info - reclaim iommu map va record struct
 * @list: Used by the info_manager to keep a list of record iova
 * @dev: struct device for iommu map handling
 * @iova: VA by iommu map
 * @len: Length corresponding to IOVA
 * @prot: Map-Prot corresponding to IOVA
 */
struct iova_map_info {
	struct list_head list;
	struct device *dev;
	unsigned long iova;
	unsigned long len;
	int prot;
	enum dmabuf_map_state state;
};

/**
 * struct uva_map_info - reclaim ACPU maped User-VA record struct
 * @list: Used by the info_manager to keep a list of record uva
 * @address: VA by ACPU user-task mmap
 * @len: Length corresponding to uva
 * @vma: VMA corresponding to uva and task
 */
struct uva_map_info {
	struct list_head list;
	unsigned long uva;
	unsigned long len;
	struct vm_area_struct *vma;
	enum dmabuf_map_state state;
};

/**
 * struct kva_map_info - reclaim ACPU maped Kernel-VA record struct
 *
 * @list: Used by the info_manager to keep a list of record kva
 * @kva: VA by ACPU Kernel-driver map
 * @len: Length corresponding to kva
 */
struct kva_map_info {
	struct list_head list;
	unsigned long kva;
	unsigned long len;
	pgprot_t prot;
	enum dmabuf_map_state state;
};

/**
 * struct gva_map_info - reclaim DPU maped VA record struct
 *
 * @list: Used by the info_manager to keep a list of record gva
 * @gva: VA by GPU map
 * @len: Length corresponding to gva
 */
struct gva_map_info {
	struct list_head list;
	unsigned long gva;
	unsigned long len;
};

/**
 * struct buffer_map_info - reclaim dmabuf map info struct
 * @attach_num: numbers of device attach
 * @iova_info: List of record iova
 * @uva_info: List of record uva
 * @kva_info: List of record kva
 * @iova_info_mutex: Mutex of iova_info list
 * @uva_info_mutex: Mutex of uva_info list
 */
struct buffer_map_info {
	struct list_head iova_info;
	struct list_head uva_info;
	struct list_head kva_info;
	struct list_head gva_info;
	struct mutex iova_info_mutex;
	struct mutex uva_info_mutex;
	struct mutex kva_info_mutex;
	struct mutex gva_info_mutex;
};

#ifdef CONFIG_DMABUF_RECLAIM
enum va_ops_type {
	__ADD_VA,
	__DELETE_VA,
	__DELETE_ALL_VA,
};

int alloc_buffer_map_info(void *buf);
void free_buffer_map_info(void *buf);

int record_buffer_uva_info(void *buf,
	struct vm_area_struct *vma, unsigned long len);
void delete_buffer_uva_info(void *buf, struct vm_area_struct *vma);
int record_buffer_kva_info(void *buf, unsigned long kva, unsigned long len, pgprot_t prot);
void delete_buffer_kva_info(void *buf, unsigned long kva);
int record_buffer_iova_info(void *buf, struct device *dev,
	unsigned long iova, unsigned long len, int prot);
void delete_buffer_iova_info(void *buf,
	struct device *dev, unsigned long iova, bool is_delete_all);
int record_buffer_gva_info(void *buf, unsigned long gva, unsigned long len);
void delete_buffer_gva_info(void *buf, unsigned long gva);
void __dump_buffer_map_info(void *buf);
#else
static inline int alloc_buffer_map_info(void *buf)
{
	return 0;
}

static inline void free_buffer_map_info(void *buf)
{
}

static inline int record_buffer_uva_info(void *buf,
	struct vm_area_struct *vma, unsigned long len)
{
	return 0;
}

static inline void delete_buffer_uva_info(void *buf, struct vm_area_struct *vma)
{
}

static inline int record_buffer_kva_info(void *buf, unsigned long kva, unsigned long len, pgprot_t prot)
{
	return 0;
}

static inline void delete_buffer_kva_info(void *buf, unsigned long kva)
{
}

static inline int record_buffer_iova_info(void *buf, struct device *dev,
	unsigned long iova, unsigned long len, int prot)
{
	return 0;
}

static inline void delete_buffer_iova_info(void *buf,
	struct device *dev, unsigned long iova, bool is_delete_all)
{
}

static inline void __dump_buffer_map_info(void *buf)
{
}
#endif /* CONFIG_DMABUF_RECLAIM */
#endif /* _DMABUF_MAP_INFO_H */
