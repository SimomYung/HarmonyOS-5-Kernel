/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Basic data structures and initializers for different allocators
 * Author: Huawei OS Kernel Lab
 * Create: Mon April 11 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_ATOR_BASE_H
#define KLIBS_COOBJ_ATOR_BASE_H

#include <hmasm/types.h>
#include <hmkernel/errno.h>
#include <hongmeng/compiler.h>
#include <lib/coobj/node.h>

#define COOBJ_RESERVED_MEM_ORDER 25
#define COOBJ_RESERVED_MEM_SIZE (1 << COOBJ_RESERVED_MEM_ORDER)
#define COOBJ_RESERVED_MEM_MASK (COOBJ_RESERVED_MEM_SIZE -1)

#define NONE_OBJ_ATOR_MAPPED 0x0
#define SMALL_OBJ_ATOR_MAPPED 0x1
#define LARGE_OBJ_ATOR_MAPPED 0x2
#define ALL_OBJ_ATOR_MAPPED (SMALL_OBJ_ATOR_MAPPED | LARGE_OBJ_ATOR_MAPPED)

#define ____coobj_define_user_get_node(ator_kind, role_type_name, node_type_name) \
	static inline \
	struct ____coobj_##role_type_name##_##node_type_name##_s* \
	____coobj_##ator_kind##_##role_type_name##_get_##node_type_name( \
		struct ____coobj_##ator_kind##_##role_type_name##_s *role_name, __bool large) \
		{ \
		if (large) { \
			return &role_name->lnode_slow; \
		} else { \
			return &role_name->lnode; \
		} \
	}
/**************************************************
 * ARRAY ATOR
 **************************************************/
#define ____COOBJ_ARRAY_TEPAR_DECL \
	__relpos_t tepar_typeid, \
	__relpos_t tepar_type_size, \
	__relpos_t tepar_chunk_order
#define ____COOBJ_ARRAY_TEPAR_ARG \
	tepar_typeid, \
	tepar_type_size, \
	tepar_chunk_order

struct ____coobj_array_uholder_s {
	struct ____coobj_base_user_s *base_user;
	struct ____coobj_uholder_dnode_s dnode;
};

struct ____coobj_array_urenter_s {
	struct ____coobj_base_user_s *base_user;
	struct ____coobj_urenter_node_s node;
};

struct ____coobj_array_mholder_s {
	struct ____coobj_array_uholder_s *uholder_mva;

	struct ____coobj_base_mapper_s *base_mapper;
	struct ____coobj_array_mrenter_s *head;

	struct coobj_vspace_s *holder_shmvs;
	struct ____coobj_mholder_dnode_s dnode;
	struct ____coobj_resv_vm_info_s resv_vm_info;
};

struct ____coobj_array_mrenter_s {
	struct ____coobj_array_urenter_s *urenter_mva;

	struct ____coobj_array_mholder_s *mholder;
	struct ____coobj_array_mrenter_s *next;

	struct coobj_vspace_s *renter_shmvs;
	struct ____coobj_mrenter_dnode_s dnode;
	struct ____coobj_resv_vm_info_s resv_vm_info;
};

static inline
__uptr_t ____coobj_array_eval_dnode_size(
	____COOBJ_ARRAY_TEPAR_DECL)
{
	return tepar_type_size << tepar_chunk_order;
}

static inline
void ____coobj_array_uholder_init(
	struct ____coobj_array_uholder_s *uholder,
	struct ____coobj_base_user_s *base_user)
{
	uholder->base_user = base_user;
	____coobj_uholder_dnode_init(&uholder->dnode);
	____coobj_base_user_init(base_user);
}

static inline
void *____coobj_array_uholder_tgtobj(
	struct ____coobj_array_uholder_s *uholder, __relpos_t relpos, ____COOBJ_ARRAY_TEPAR_DECL)
{
	return ____coobj_user_raw_relpos(uholder->dnode.page_uva, relpos, tepar_type_size);
}

static inline
void ____coobj_array_urenter_init(
	struct ____coobj_array_urenter_s *urenter,
	struct ____coobj_base_user_s *base_user, __u8 mapped)
{
	UNUSED(mapped);
	urenter->base_user = base_user;
	____coobj_urenter_node_init(&urenter->node);
	____coobj_base_user_init(base_user);
}

static inline
void *____coobj_array_urenter_tgtobj(
	struct ____coobj_array_urenter_s *urenter, __relpos_t relpos, ____COOBJ_ARRAY_TEPAR_DECL)
{
	return ____coobj_user_raw_relpos(urenter->node.page_uva, relpos, tepar_type_size);
}

static inline
void ____coobj_array_mholder_init(
	struct ____coobj_array_mholder_s *mholder,
	struct ____coobj_base_mapper_s *base_mapper,
	struct ____coobj_array_uholder_s *uholder_mva)
{
	mholder->uholder_mva = uholder_mva;

	mholder->base_mapper = base_mapper;
	mholder->head = NULL;

	mholder->holder_shmvs = NULL;
	mholder->resv_vm_info = ____COOBJ_RESV_VM_INFO_INIT;
	____coobj_mholder_dnode_init(&mholder->dnode, &uholder_mva->dnode);
	____coobj_uholder_dnode_init(&uholder_mva->dnode);
}

static inline
void ____coobj_array_mholder_init_shmvs(
	struct ____coobj_array_mholder_s *mholder,
	struct coobj_vspace_s *holder_shmvs)
{
	mholder->holder_shmvs = holder_shmvs;
}

static inline
void ____coobj_array_mrenter_init(
	struct ____coobj_array_mrenter_s *mrenter,
	struct ____coobj_array_mholder_s *mholder,
	struct ____coobj_array_urenter_s *urenter_mva)
{
	mrenter->urenter_mva = urenter_mva;

	mrenter->mholder = mholder;
	mrenter->next = mholder->head;
	mholder->head = mrenter;

	mrenter->renter_shmvs = NULL;
	mrenter->resv_vm_info = ____COOBJ_RESV_VM_INFO_INIT;
	____coobj_mrenter_dnode_init(&mrenter->dnode, &urenter_mva->node);
	____coobj_urenter_node_init(&urenter_mva->node);
}
static inline
void ____coobj_array_mrenter_init_shmvs(
	struct ____coobj_array_mrenter_s *mrenter,
	struct coobj_vspace_s *renter_shmvs)
{
	mrenter->renter_shmvs = renter_shmvs;
}

/**************************************************
 * RADIX ATOR
 **************************************************/
#define ____COOBJ_RADIX1_TEPAR_DECL \
	__relpos_t tepar_typeid, \
	__relpos_t tepar_type_size, \
	__relpos_t tepar_chunk_order, \
	__relpos_t tepar_level1_order
#define ____COOBJ_RADIX1_TEPAR_ARG \
	tepar_typeid, \
	tepar_type_size, \
	tepar_chunk_order, \
	tepar_level1_order

struct ____coobj_radix1_uholder_s {
	struct ____coobj_base_user_s *base_user;
	struct ____coobj_uholder_lnode_s lnode;
};

struct ____coobj_radix1_urenter_s {
	struct ____coobj_base_user_s *base_user;
	struct ____coobj_urenter_node_s node;
};

struct ____coobj_radix1_mholder_s {
	struct ____coobj_radix1_uholder_s *uholder_mva;

	struct ____coobj_base_mapper_s *base_mapper;
	struct ____coobj_radix1_mrenter_s *head;

	struct coobj_vspace_s *holder_shmvs;
	struct ____coobj_mholder_lnode_s lnode;
	struct ____coobj_resv_vm_info_s resv_vm_info;
};

struct ____coobj_radix1_mrenter_s {
	struct ____coobj_radix1_urenter_s *urenter_mva;

	struct ____coobj_radix1_mholder_s *mholder;
	struct ____coobj_radix1_mrenter_s *next;

	struct coobj_vspace_s *renter_shmvs;
	struct ____coobj_mrenter_lnode_s lnode;
	struct ____coobj_resv_vm_info_s resv_vm_info;
};

static inline
__uptr_t ____coobj_radix1_eval_dnode_index(
	struct ____coobj_chunkid_s chunkid,
	____COOBJ_RADIX1_TEPAR_DECL)
{
	return (chunkid.relpos >> tepar_chunk_order) & ____coobj_order_mask(tepar_level1_order);
}
static inline
__uptr_t ____coobj_radix1_eval_dnode_size(
	____COOBJ_RADIX1_TEPAR_DECL)
{
	return tepar_type_size << tepar_chunk_order;
}
static inline
__uptr_t ____coobj_radix1_eval_mholder_lnode_size(
	____COOBJ_RADIX1_TEPAR_DECL)
{
	return ____COOBJ_MHOLDER_DNODE_SIZE << tepar_level1_order;
}
static inline
__uptr_t ____coobj_radix1_eval_mrenter_lnode_size(
	____COOBJ_RADIX1_TEPAR_DECL)
{
	return ____COOBJ_MRENTER_DNODE_SIZE << tepar_level1_order;
}
static inline
__uptr_t ____coobj_radix1_eval_uholder_lnode_size(
	____COOBJ_RADIX1_TEPAR_DECL)
{
	return ____COOBJ_UHOLDER_DNODE_SIZE << tepar_level1_order;
}
static inline
__uptr_t ____coobj_radix1_eval_urenter_lnode_size(
	____COOBJ_RADIX1_TEPAR_DECL)
{
	return ____COOBJ_URENTER_NODE_SIZE << tepar_level1_order;
}

static inline
void ____coobj_radix1_uholder_init(
	struct ____coobj_radix1_uholder_s *uholder,
	struct ____coobj_base_user_s *base_user)
{
	uholder->base_user = base_user;
	____coobj_uholder_lnode_init(&uholder->lnode);
	____coobj_base_user_init(base_user);
}

static inline
void *____coobj_radix1_uholder_tgtobj(
	struct ____coobj_radix1_uholder_s *uholder, __relpos_t relpos, ____COOBJ_RADIX1_TEPAR_DECL)
{
	__uptr_t level1_relpos = (relpos >> tepar_chunk_order) & ____coobj_order_mask(tepar_level1_order);
	__uptr_t chunk_relpos = relpos & ____coobj_order_mask(tepar_chunk_order);
	return ____coobj_user_raw_relpos(
		uholder->lnode.child_dnode_list[level1_relpos].page_uva, chunk_relpos, tepar_type_size);
}

static inline
void ____coobj_radix1_urenter_init(
	struct ____coobj_radix1_urenter_s *urenter,
	struct ____coobj_base_user_s *base_user, __u8 mapped)
{
	UNUSED(mapped);
	urenter->base_user = base_user;
	____coobj_urenter_node_init(&urenter->node);
	____coobj_base_user_init(base_user);
}

static inline
void *____coobj_radix1_urenter_tgtobj(
	struct ____coobj_radix1_urenter_s *urenter, __relpos_t relpos, ____COOBJ_RADIX1_TEPAR_DECL)
{
	__uptr_t level1_relpos = (relpos >> tepar_chunk_order) & ____coobj_order_mask(tepar_level1_order);
	__uptr_t chunk_relpos = relpos & ____coobj_order_mask(tepar_chunk_order);
	return ____coobj_user_raw_relpos(
		urenter->node.child_node_list[level1_relpos].page_uva, chunk_relpos, tepar_type_size);
}

static inline
void ____coobj_radix1_mholder_init(
	struct ____coobj_radix1_mholder_s *mholder,
	struct ____coobj_base_mapper_s *base_mapper,
	struct ____coobj_radix1_uholder_s *uholder_mva)
{
	mholder->uholder_mva = uholder_mva;

	mholder->base_mapper = base_mapper;
	mholder->head = NULL;

	mholder->holder_shmvs = NULL;
	mholder->resv_vm_info = ____COOBJ_RESV_VM_INFO_INIT;
	____coobj_mholder_lnode_init(&mholder->lnode, &uholder_mva->lnode, __false);
	____coobj_uholder_lnode_init(&uholder_mva->lnode);
}
static inline
void ____coobj_radix1_mholder_init_shmvs(
	struct ____coobj_radix1_mholder_s *mholder,
	struct coobj_vspace_s *holder_shmvs)
{
	mholder->holder_shmvs = holder_shmvs;
}

static inline
void ____coobj_radix1_mrenter_init(
	struct ____coobj_radix1_mrenter_s *mrenter,
	struct ____coobj_radix1_mholder_s *mholder,
	struct ____coobj_radix1_urenter_s *urenter_mva)
{
	mrenter->urenter_mva = urenter_mva;

	mrenter->mholder = mholder;
	mrenter->next = mholder->head;
	mholder->head = mrenter;

	mrenter->renter_shmvs = NULL;
	mrenter->resv_vm_info = ____COOBJ_RESV_VM_INFO_INIT;
	____coobj_mrenter_lnode_init(&mrenter->lnode, &urenter_mva->node);
	____coobj_urenter_node_init(&urenter_mva->node);
}
static inline
void ____coobj_radix1_mrenter_init_shmvs(
	struct ____coobj_radix1_mrenter_s *mrenter,
	struct coobj_vspace_s *renter_shmvs)
{
	mrenter->renter_shmvs = renter_shmvs;
}

/**************************************************
 * RADIX1VAR ATOR
 **************************************************/
#define ____COOBJ_RADIX1VAR_TEPAR_DECL \
	__relpos_t tepar_type_size, \
	__relpos_t tepar_chunk_order, \
	__relpos_t tepar_level1_order
#define ____COOBJ_RADIX1VAR_TEPAR_ARG \
	tepar_type_size, \
	tepar_chunk_order, \
	tepar_level1_order
struct ____coobj_radix1var_uholder_s {
	int type_id;
	struct ____coobj_base_user_s *base_user;
	struct ____coobj_radix1var_uholder_lnode_s lnode;
	struct ____coobj_radix1var_uholder_lnode_s lnode_slow;
	struct ____coobj_radix1var_free_list free_list;
	unsigned int flags;
};

struct ____coobj_radix1var_urenter_s {
	int type_id;
	struct ____coobj_base_user_s *base_user;
	struct ____coobj_urenter_node_s node;
	struct ____coobj_urenter_node_s node_slow;
};

struct ____coobj_radix1var_mholder_s {
	struct ____coobj_radix1var_uholder_s *uholder_mva;

	struct ____coobj_base_mapper_s *base_mapper;
	struct ____coobj_radix1var_mrenter_s *head;

	struct coobj_vspace_s *holder_shmvs;
	struct ____coobj_mholder_lnode_s lnode;
	struct ____coobj_mholder_lnode_s lnode_slow;
	struct ____coobj_resv_vm_info_s resv_vm_info;
};

struct ____coobj_radix1var_mrenter_s {
	struct ____coobj_radix1var_urenter_s *urenter_mva;

	struct ____coobj_radix1var_mholder_s *mholder;
	struct ____coobj_radix1var_mrenter_s *next;

	struct coobj_vspace_s *renter_shmvs;
	struct ____coobj_mrenter_lnode_s lnode;
	struct ____coobj_mrenter_lnode_s lnode_slow;
	struct ____coobj_resv_vm_info_s resv_vm_info;
};

static inline
void ____coobj_radix1var_uholder_init(
	struct ____coobj_radix1var_uholder_s *uholder,
	struct ____coobj_base_user_s *base_user)
{
	uholder->base_user = base_user;
	____coobj_radix1var_uholder_lnode_init(&uholder->lnode);
	____coobj_radix1var_uholder_lnode_init(&uholder->lnode_slow);
	____coobj_radix1var_free_list_init(&uholder->free_list);
	____coobj_base_user_init(base_user);
}

static inline
void ____coobj_radix1var_urenter_init(
	struct ____coobj_radix1var_urenter_s *urenter,
	struct ____coobj_base_user_s *base_user, __u8 mapped)
{
	if (mapped == NONE_OBJ_ATOR_MAPPED) {
		urenter->base_user = base_user;
		____coobj_base_user_init(base_user);
	}
	if ((mapped & SMALL_OBJ_ATOR_MAPPED) != SMALL_OBJ_ATOR_MAPPED) {
		____coobj_urenter_node_init(&urenter->node);
	}
	if ((mapped & LARGE_OBJ_ATOR_MAPPED) != LARGE_OBJ_ATOR_MAPPED) {
		____coobj_urenter_node_init(&urenter->node_slow);
	}
}

____coobj_define_user_get_node(radix1var, mholder, lnode);
____coobj_define_user_get_node(radix1var, mrenter, lnode);

static inline
struct ____coobj_urenter_node_s* ____coobj_radix1var_urenter_get_node(
	struct ____coobj_radix1var_urenter_s *urenter, __bool large)
{
	if (large) {
		return &urenter->node_slow;
	} else {
		return &urenter->node;
	}
}

static inline
struct ____coobj_radix1var_uholder_lnode_s* ____coobj_radix1var_uholder_get_lnode(
	struct ____coobj_radix1var_uholder_s *uholder, __bool large)
{
	if (large) {
		return &uholder->lnode_slow;
	} else {
		return &uholder->lnode;
	}
}
static inline
void *____coobj_radix1var_uholder_tgtobj(
	struct ____coobj_radix1var_uholder_s *uholder, __relpos_t relpos, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	void *ret = NULL;

	if (relpos != ____COOBJ_INVALID_POS) {
		__uptr_t level1_relpos = (relpos >> tepar_chunk_order) & ____coobj_order_mask(tepar_level1_order);
		__uptr_t chunk_relpos = relpos & ____coobj_order_mask(tepar_chunk_order);
		ret = ____coobj_user_raw_relpos(
			____coobj_radix1var_uholder_get_lnode(uholder, ____coobj_radix1var_is_large_obj(relpos)
			)->child_dnode_list[level1_relpos].page_uva, chunk_relpos, tepar_type_size);
	}
	return ret;
}

static inline
void *____coobj_radix1var_urenter_tgtobj(
	struct ____coobj_radix1var_urenter_s *urenter, __relpos_t relpos, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	void *ret = NULL;

	if (relpos != ____COOBJ_INVALID_POS) {
		__uptr_t level1_relpos = (relpos >> tepar_chunk_order) & ____coobj_order_mask(tepar_level1_order);
		__uptr_t chunk_relpos = relpos & ____coobj_order_mask(tepar_chunk_order);
		ret = ____coobj_user_raw_relpos(
			____coobj_radix1var_urenter_get_node(urenter, ____coobj_radix1var_is_large_obj(relpos)
			)->child_node_list[level1_relpos].page_uva, chunk_relpos, tepar_type_size);
	}
	return ret;
}

static inline
__uptr_t ____coobj_radix1var_eval_dnode_index(
	struct ____coobj_chunkid_s chunkid,
	____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	return (chunkid.relpos >> tepar_chunk_order) & ____coobj_order_mask(tepar_level1_order);
}
static inline
__uptr_t ____coobj_radix1var_eval_dnode_size(
	____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	return tepar_type_size << tepar_chunk_order;
}
static inline
__uptr_t ____coobj_radix1var_eval_mholder_lnode_size(
	____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	return ____COOBJ_MHOLDER_DNODE_SIZE << tepar_level1_order;
}
static inline
__uptr_t ____coobj_radix1var_eval_mrenter_lnode_size(
	____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	return ____COOBJ_MRENTER_DNODE_SIZE << tepar_level1_order;
}
static inline
__uptr_t ____coobj_radix1var_eval_uholder_lnode_size(
	____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	return ____COOBJ_RADIX1VAR_UHOLDER_DNODE_SIZE << tepar_level1_order;
}
static inline
__uptr_t ____coobj_radix1var_eval_urenter_lnode_size(
	____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	return ____COOBJ_URENTER_NODE_SIZE << tepar_level1_order;
}

static inline
void ____coobj_mholder_dnode_init_dpage_hshmid(struct ____coobj_mholder_dnode_s *list, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	for (__uptr_t index = 0; index < (1U << tepar_level1_order); index++) {
		(list + index)->dpage_hshmid = ____COOBJ_INVALID_SHMID;
	}
}

static inline
void ____coobj_radix1var_mholder_init(
	struct ____coobj_radix1var_mholder_s *mholder,
	struct ____coobj_base_mapper_s *base_mapper,
	struct ____coobj_radix1var_uholder_s *uholder_mva)
{
	mholder->uholder_mva = uholder_mva;

	mholder->base_mapper = base_mapper;
	mholder->head = NULL;

	mholder->holder_shmvs = NULL;
	mholder->resv_vm_info = ____COOBJ_RESV_VM_INFO_INIT;
	____coobj_mholder_lnode_init(&mholder->lnode, &uholder_mva->lnode, __true);
	____coobj_mholder_lnode_init(&mholder->lnode_slow, &uholder_mva->lnode_slow, __true);
	____coobj_radix1var_uholder_lnode_init(&uholder_mva->lnode);
	____coobj_radix1var_uholder_lnode_init(&uholder_mva->lnode_slow);
}

static inline
void ____coobj_radix1var_mrenter_init(
	struct ____coobj_radix1var_mrenter_s *mrenter,
	struct ____coobj_radix1var_mholder_s *mholder,
	struct ____coobj_radix1var_urenter_s *urenter_mva)
{
	mrenter->urenter_mva = urenter_mva;
	struct ____coobj_base_mapper_s *base_mapper = mholder->base_mapper;

	____coobj_base_mapper_sync_lock(base_mapper);
	mrenter->mholder = mholder;
	mrenter->next = mholder->head;
	mholder->head = mrenter;
	____coobj_base_mapper_sync_unlock(base_mapper);

	mrenter->renter_shmvs = NULL;
	mrenter->resv_vm_info = ____COOBJ_RESV_VM_INFO_INIT;
	____coobj_mrenter_lnode_init(&mrenter->lnode, &urenter_mva->node);
	____coobj_mrenter_lnode_init(&mrenter->lnode_slow, &urenter_mva->node_slow);
	____coobj_urenter_node_init(&urenter_mva->node);
	____coobj_urenter_node_init(&urenter_mva->node_slow);
}

raw_static_assert(sizeof(struct ____coobj_radix1var_uholder_dnode_s) == sizeof(struct ____coobj_urenter_node_s),
		  coobj_radix1var_uholder_dnode_compatible_with_urenter_node);
raw_static_assert(sizeof(struct ____coobj_radix1var_uholder_lnode_s) == sizeof(struct ____coobj_urenter_node_s),
		  coobj_radix1var_uholder_lnode_compatible_with_urenter_node);
raw_static_assert(__builtin_offsetof(struct ____coobj_radix1var_uholder_lnode_s, page_uva) ==
		  __builtin_offsetof(struct ____coobj_urenter_node_s, page_uva), coobj_user_node_check_page_uva);
raw_static_assert(__builtin_offsetof(struct ____coobj_radix1var_uholder_lnode_s, dlist_node) == 0,
		  coobj_radix1var_uholder_lnode_check_dlist_node);
raw_static_assert(__builtin_offsetof(struct ____coobj_urenter_node_s, dlist_node) == 0,
		  coobj_urenter_node_check_dlist_node);
raw_static_assert(__builtin_offsetof(struct ____coobj_radix1var_uholder_s, lnode) ==
		  __builtin_offsetof(struct ____coobj_radix1var_urenter_s, node), coobj_user_check_node_fast);
raw_static_assert(__builtin_offsetof(struct ____coobj_radix1var_uholder_s, lnode_slow) ==
		  __builtin_offsetof(struct ____coobj_radix1var_urenter_s, node_slow), coobj_user_check_node_slow);

#endif
