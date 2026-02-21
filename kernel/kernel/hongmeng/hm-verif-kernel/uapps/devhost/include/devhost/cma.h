/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Prototypes for devhost cma APIs
 * Author: Huawei OS Kernel Lab
 * Create: Tues May 10 19:17:12 2022
 */
#ifndef __DEVHOST_API_CMA_H__
#define __DEVHOST_API_CMA_H__

#include <stdint.h>
#include <stddef.h>
#include <hongmeng/types.h>
#include <libhmsync/raw_mutex.h>

struct devhost_cma {
	unsigned int cma_id;
	size_t size;
	unsigned long svaddr;
	unsigned long long spaddr;
	struct raw_mutex mutex;
};

/*
 * Devhost cma static initialization.
 *
 * @param size		[I] size of static cma area
 *
 * @param rtcma		[O] devhost cma ptr if operation succeeds
 *
 * @return E_HM_OK if operation succeeds.
 */
int devhost_backend_cma_st_init(size_t size, struct devhost_cma **rtcma);

/*
 * Devhost cma dynamic initialization.
 *
 * @param size		[I] size of dynamic cma area
 *
 * @param rtcma		[O] devhost cma ptr if operation succeeds
 *
 * @return E_HM_OK if operation succeeds.
 */
int devhost_backend_cma_dyn_init(size_t size, struct devhost_cma **rtcma);

/*
 * Devhost cma dynamic initialization.
 *
 * @param size		[I] size of dynamic cma area
 *
 * @return E_HM_OK if operation succeeds.
 */
int devhost_backend_cma_dyn_destroy(const struct devhost_cma *cma);

/*
 * Devhost cma setup mapping
 *
 * @param cma		[I] devhost cma info
 *
 * @param vaddr		[I] target vaddr for cma mapping
 *
 * @return E_HM_OK if operation succeeds.
 */
int devhost_backend_cma_setup(struct devhost_cma *cma, void *vaddr);

/*
 * Devhost cma dynamic initialization.
 *
 * @param cma		[I] devhost cma info
 *
 * @param size		[I] size of dynamic cma area
 *
 * @param mprot		[I] mem mprot info
 *
 * @param rpaddr	[O] return phy addr if operation succeeds
 *
 * @return cma vaddr.
 */
void *devhost_backend_cma_alloc(struct devhost_cma *cma,
				size_t size,
				uint32_t mprot,
				unsigned long long *rpaddr);

/*
 * Devhost cma dynamic initialization.
 *
 * @param cma		[I] devhost cma info
 *
 * @param addr		[I] cma addr need to free
 *
 * @param size		[I] size of free
 *
 * @return E_HM_OK if operation succeeds.
 */
int devhost_backend_cma_free(struct devhost_cma *cma, void *addr, size_t size);

static inline unsigned long long devhost_cma_virt_to_phys(const struct devhost_cma *cma,
							  const void *vaddr)
{
	return cma->spaddr + (ptr_to_ulong(vaddr) - cma->svaddr);
}

static inline void *devhost_cma_phys_to_virt(const struct devhost_cma *cma,
					     unsigned long long paddr)
{
	return ulong_to_ptr(cma->svaddr + (paddr - cma->spaddr), void);
}
#endif /* __DEVHOST_CMA_H__ */
