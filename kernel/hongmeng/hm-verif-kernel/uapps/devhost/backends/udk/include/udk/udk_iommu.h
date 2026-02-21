/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: udk iommu framework interfaces
 * Author: Kirin Plat Drv Department
 * Create: Wed Mar 13 15:45:12 2024
 */
#ifndef __UDK_IOMMU_H__
#define __UDK_IOMMU_H__

#include <udk/thread.h>
#include <udk/thread_cond.h>
#include <udk/types.h>
#include <udk/device.h>
#include <udk/mempool.h>
#include <udk/lib/scatterlist.h>
#include <libmem/utils.h>
#include <libhmsync/uspinlock.h>

#define PINGPONG_SHIFT 31
#define PINGPONG_MASK (1UL << PINGPONG_SHIFT)
#define IOVA_SIZE_MASK (PINGPONG_MASK - 1)
#define LAZY_FREE_WATERLINE	10

/* iommu prot */
#define IOMMU_READ		(1 << 0)
#define IOMMU_WRITE		(1 << 1)
#define IOMMU_CACHE		(1 << 2) /* DMA cache coherency */
#define IOMMU_NOEXEC	(1 << 3)
#define IOMMU_MMIO		(1 << 4) /* e.g. things like MSI doorbells */
#define IOMMU_SEC		(1 << 8)
#define IOMMU_EXEC		(1 << 9)

/*
 * enum for iommu ids:
 * Since sec registers are still accessed in atf/tzsp,
 * the defination of the enum id should be the same with atf.
 */
enum iommu_id {
	SMMU_MEDIA1,
	SMMU_MEDIA2,
	SMMU_NPU,
	SMMU_PCIE0,
	SMMU_PCIE1,
	SMMU_MAX_ID,
};

/* for potential new iommu types in the future */
enum iommu_type {
	IOMMU_TYPE_SMMUV3,
	IOMMU_TYPE_MAX,
};

/* for potential svm domain type in the future */
enum domain_type {
	IOVA_DOMAIN_TYPE,
	SVM_DOMAIN_TYPE,
	DOMAIN_TYPE_MAX,
};

enum iova_free_type {
	NORMAL_FREE,
	LAZY_FREE,
	FREE_TYPE_MAX
};

struct udk_iommu_ops;

struct udk_iova_info {
	unsigned long long iova_start;
	size_t iova_size;
	unsigned long long iova_align;
	unsigned int lazy_free; /* UNUSED currently */
};

struct udk_iommu_info {
	unsigned int iommu_id;
	unsigned int sid;
	unsigned int ssid_base;
	/* for dss multiple ssid, default: 1 */
	unsigned int ssid_num;
	unsigned int tlb_flush;
	bool is_svm;
	struct udk_iova_info iova_info;
	void *iommu_priv;
};

struct udk_iommu_device {
	unsigned int iommu_id;
	enum iommu_type type;
	struct udk_iommu_ops *ops;
	struct dlist_node list;
};

struct udk_iova_lazy_free {
	u32 *free_size;
	u32 pingpong;
	unsigned long pages;
	unsigned long waterline;
	bool end;
	udk_thread_t task;
	struct udk_mutex mutex;
	struct uspinlock_s lock;
	struct udk_thread_cond cond;
};

struct udk_iommu_dom_cookie {
	struct uspinlock_s iova_lock;
	struct udk_mempool *iova_pool;
	struct udk_iommu_domain *domain;
	struct udk_iova_info iova;
	struct udk_iova_lazy_free *lazy_free;
	/* iova_root(rely on rb_tree): map_dmabuf not supported now */
};

struct udk_iommu_domain {
	unsigned int dom_type; /* iova/svm */
	const struct udk_iommu_ops *ops;
	void *iova_cookie;
};

struct udk_iommu_ops {
	int (*probe_dev)(struct udk_device *dev);
	void (*release_dev)(struct udk_device *dev);
	struct udk_iommu_domain *(*get_domain_by_dev)(struct  udk_device *dev);
	struct udk_iommu_domain *(*domain_alloc)(struct udk_device *dev, unsigned domain_type);
	void (*domain_free)(struct udk_iommu_domain *domain);
	int (*attach_dev)(struct udk_device *dev, struct udk_iommu_domain *domain);
	void (*detach_dev)(struct udk_device *dev, struct udk_iommu_domain *domain);
	int (*map)(struct udk_iommu_domain *domain, uintptr_t iova,
			uintptr_t paddr, size_t size, unsigned int prot);
	size_t (*unmap)(struct udk_iommu_domain *domain, uintptr_t iova,
			size_t size);
	uintptr_t (*iova_to_phys)(struct udk_iommu_domain *domain, uintptr_t iova);
	void (*flush_tlb)(struct udk_device *dev, struct udk_iommu_domain *domain);
	void (*flush_tlb_by_ssid)(struct udk_device *dev, struct udk_iommu_domain *domain, unsigned int ssid);
	void (*flush_ttwc)(struct udk_iommu_domain *domain);
};

/* iommu attach process */
struct udk_iommu_device *udk_get_iommu_by_dev(struct udk_device *dev);
void udk_iommu_device_register(struct udk_iommu_device *iommu);
void udk_dev_iommu_configure(struct device_info *info);

/* dev pgtable ops */
bool udk_iommu_check_sgt_valid(struct udk_sg_table *sgt);
size_t udk_iommu_sgt_size_get(struct udk_sg_table *sgt);

/* the following funcs cope with iova_alloc */
uintptr_t udk_iommu_map(struct udk_device *dev, uintptr_t paddr, size_t size, unsigned int prot);
int udk_iommu_unmap(struct udk_device *dev, uintptr_t iova, size_t size);
uintptr_t udk_iommu_map_sg(struct udk_device *dev, struct udk_sg_table *sgt, unsigned int prot, size_t *out_size);
int udk_iommu_unmap_sg(struct udk_device *dev, uintptr_t iova, struct udk_sg_table *sgt);
/* map_dmabuf/unmap_dmabuf not supported currently */
uintptr_t udk_iommu_iova_to_phys(struct udk_device *dev, uintptr_t iova);
struct udk_iommu_domain *udk_iommu_get_domain_by_dev(struct udk_device *dev);
#endif