/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Mtrc management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 27 17:00:55 2019
 */
#ifndef SYSMGR_MEMMGR_MEM_MTRC_H
#define SYSMGR_MEMMGR_MEM_MTRC_H

#include <intapi/mem/vspace.h>
#include <hmkernel/mm/pgtable.h>
#include <hmasm/mm/pgtable.h>
#include <hmasm/pgtbl/pgtblhdlr.h>

#define MTRC_MAX_SIZE 100

#ifdef __arm__

#ifdef __HMPGTBL_ARM_LPAE__
#define MTRC_MAX_ADDR 0x10000000000
#define mtrc_blockdesc_extract_uxn(v)   (((__u64)(v) >> 54U) & 0x1UL)
#define mtrc_blockdesc_extract_pxn(v)   (((__u64)(v) >> 53U) & 0x1UL)
#define mtrc_blockdesc_extract_ap21(v)  (((__u64)(v) >> 6U)  & 0x3UL)
#define mtrc_blockdesc_extract_attr(v)  (((__u64)(v) >> 2U)  & 0x7UL)
#define ARM_LPAE_PAGE_STRONG_ORD        (0)
#define ARM_LPAE_PAGE_DEVICE            (1)
#define ARM_LPAE_PAGE_NORMAL_NC         (2)
#define ARM_LPAE_PAGE_NORMAL            (3)
#define RW_AT_ALL_LEVELS        1
#define RO_AT_HIGHER_LEVEL      2
#define RO_AT_ALL_LEVELS        3
#else
#define MTRC_MAX_ADDR 0x100000000
#define MTRC_MAX_TYPE 7
#endif
#else

#define MTRC_MAX_ADDR 0x1000000000000
#define mtrc_blockdesc_extract_uxn(v)	(((__u64)(v) >> 54U) & 0x1UL)
#define mtrc_blockdesc_extract_pxn(v)	(((__u64)(v) >> 53U) & 0x1UL)
#define mtrc_blockdesc_extract_ap21(v)	(((__u64)(v) >> 6U)  & 0x3UL)
#define mtrc_blockdesc_extract_attr(v)	(((__u64)(v) >> 2U)  & 0x7UL)
#define A64_PAGE_DEVICE_NGNRNE	0
#define A64_PAGE_DEVICE_NGNRE	1
#define A64_PAGE_DEVICE_GRE	2
#define A64_PAGE_NORMAL_NC	3
#define A64_PAGE_NORMAL		4
#define RW_AT_ALL_LEVELS	1
#define RO_AT_HIGHER_LEVEL	2
#define RO_AT_ALL_LEVELS	3

#endif
struct mem_type_region_cfg {
	uint64_t start;
	uint64_t len;
	unsigned int type;
	uint64_t attr;
	struct dlist_node dnode;
};

struct mtrc_entry_s {
	uint64_t start;
	uint64_t len;
	int type;
	uint64_t attr;
	bool is_add;
};

int mtrc_add_region(const struct mtrc_entry_s *entry);
int mtrc_del_region(const struct mtrc_entry_s *entry);
#if defined(__arm__) && !defined(__HMPGTBL_ARM_LPAE__)
int mtrc_type_of_region(uint64_t start, uint64_t len, int *type);
#else
int mtrc_attr_of_region(uint64_t start, uint64_t len, uint64_t *attr);
#endif
void mtrc_list_init(void);
struct dlist_node *mtrc_global_list(void);
struct raw_mutex *mtrc_global_mutex(void);

#endif
