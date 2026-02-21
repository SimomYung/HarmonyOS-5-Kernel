/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: huge tlb page management
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 08 22:42:27 2020
 */
#ifndef SYSMGR_INTAPI_HUGE_TLB_H
#define SYSMGR_INTAPI_HUGE_TLB_H

#include <hmkernel/errno.h>
#include <hongmeng/types.h>
#include <libstrict/strict.h>


#ifdef CONFIG_HUGE_TLB_PAGE
int hgtlb_set_nr_pages(unsigned int order, unsigned long nr);
void hgtlb_set_nr_overcommit_pages(unsigned int order, unsigned long nr);

int hgtlb_for_each_ator(int (*callback)(char *name, unsigned int order));
int hgtlb_init_sysfs(void);

#ifdef __HOST_LLT__
int hgtlb_read(void *op_ctx, void *dst, size_t pos, size_t size, size_t *rsize);
int hgtlb_write(void *op_ctx, void *src, size_t pos, size_t size, size_t *wsize);
#endif /* __HOST_LLT__ */
#else /* !CONFIG_HUGE_TLB_PAGE */
static inline int hgtlb_set_nr_pages(unsigned int order, unsigned long nr)
{
	UNUSED(order, nr);
	return E_HM_OK;
}

static inline void hgtlb_set_nr_overcommit_pages(unsigned int order,
						unsigned long nr)
{
	UNUSED(order, nr);
}

static inline int hgtlb_for_each_ator(int (*callback)(char *name,
						      unsigned int order))
{
	UNUSED(callback);
	return E_HM_OK;
}

static inline int hgtlb_init_sysfs(void)
{
	return E_HM_OK;
}

#ifdef __HOST_LLT__
static inline int hgtlb_read(void *op_ctx, void *dst, size_t pos,
			     size_t size, size_t *rsize)
{
	UNUSED(op_ctx, dst, pos, size, rsize);
	return E_HM_OK;
}

static inline int hgtlb_write(void *op_ctx, void *src, size_t pos,
			      size_t size, size_t *wsize)
{
	UNUSED(op_ctx, src, pos, size, wsize);
	return E_HM_OK;
}
#endif /* __HOST_LLT__ */
#endif /* CONFIG_HUGE_TLB_PAGE */
#endif
