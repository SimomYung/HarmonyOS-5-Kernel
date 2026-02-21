/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Provide basic functionality for coobj
 * Author: Huawei OS Kernel Lab
 * Create: Mon April 11 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_UTILS_H
#define KLIBS_COOBJ_UTILS_H

#include <hmasm/types.h>
#include <hmasm/memory.h>
#include <hmkernel/errno.h>
#include <hmkernel/compiler.h>
#include <hongmeng/panic.h>
#include <lib/utils.h>

static inline void *__ptr_add(void *ptr, __uptr_t offset)
{
	return (void *)((__uptr_t)(ptr) + offset);
}

#ifndef NULL
#define NULL ((void *)0)
#endif

#define ____COOBJ_TYPE_SIZE_BITS 16
#define ____COOBJ_PTR_BITS (sizeof(__uptr_t) * 8)
#define ____COOBJ_U64_BITS 64

typedef __sptr_t coobj_vspaceid_t;
typedef __sptr_t coobj_typeid_t;
typedef __sptr_t coobj_ator_kind_t;
typedef __sptr_t coobj_role_kind_t;

typedef __s32 ____coobj_shmid_t;

typedef struct {
	__uptr_t type_size: ____COOBJ_TYPE_SIZE_BITS;
	__uptr_t type: ____COOBJ_PTR_BITS - ____COOBJ_TYPE_SIZE_BITS;
} ____coobj_typeinfo_t;

typedef struct {
	__uptr_t chunk_order : 8;
	__uptr_t level1_order : 8;
	__uptr_t level2_order : 8;
	__uptr_t level3_order : 8;
} ____coobj_orderinfo_t;

#define ____COOBJ_INVALID_SHMID ((____coobj_shmid_t)-1)
#define ____COOBJ_INVALID_ATOR ((__uptr_t)-1)
#define ____COOBJ_INVALID_PAGE ((__uptr_t)0)

enum {
	____COOBJ_MRENTER_SHMST_BACKEND_UNMAP,
	____COOBJ_MRENTER_SHMST_BACKEND_ZERO,
	____COOBJ_MRENTER_SHMST_BACKEND_MAP,
};

enum {
	____COOBJ_MRENTER_SHMST_ACTION_NONE,
	____COOBJ_MRENTER_SHMST_ACTION_FREE,
};
struct ____coobj_mrenter_shmst_s {
	__u16 status;
	__u16 action;
};
#define ____COOBJ_MRENTER_SHMST_STATUS_UNMAP_NONE \
	((struct ____coobj_mrenter_shmst_s) { \
		.status = ____COOBJ_MRENTER_SHMST_BACKEND_UNMAP, \
		.action = ____COOBJ_MRENTER_SHMST_ACTION_NONE, \
	})
#define ____COOBJ_MRENTER_SHMST_STATUS_MAP_NONE \
	((struct ____coobj_mrenter_shmst_s) { \
		.status = ____COOBJ_MRENTER_SHMST_BACKEND_MAP, \
		.action = ____COOBJ_MRENTER_SHMST_ACTION_NONE, \
	})
#define ____COOBJ_MRENTER_SHMST_STATUS_ZERO_FREE \
	((struct ____coobj_mrenter_shmst_s) { \
		.status = ____COOBJ_MRENTER_SHMST_BACKEND_ZERO, \
		.action = ____COOBJ_MRENTER_SHMST_ACTION_FREE, \
	})

#define ____coobj_order_mask(order) ((1 << ((__uptr_t)(order))) - 1)

static inline
__uptr_t ____coobj_page_datalen(__uptr_t size, __uptr_t page_order)
{
	return ((size + (1 << (page_order)) - 1) >> page_order) << page_order;
}

#ifndef ____coobj_array_size
#define ____coobj_array_size(n)	(sizeof(n) / sizeof((n)[0]))
#endif

#define HM_COOBJ_FLAGS_HKIP_ENABLE  0x10

int hm_hkip_rowm_cpy(const void *dst, const void *src, size_t size);
int hm_hkip_rowm_set(const void *dst, uint8_t val, size_t size);

#define hm_hkip_rowm_cpy_nofail(dst, src, size)			\
do {													\
	int ___err;											\
	___err = hm_hkip_rowm_cpy(dst, src, size);			\
	if (___err < 0)										\
		panic("hkip rowm cpy failed, err=%d", ___err);	\
} while (1 != 1)

#endif
