/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Data structures for radix tree allocator
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 16 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_NODE_RADIX_H
#define KLIBS_COOBJ_NODE_RADIX_H

#include <lib/coobj/node_array.h>

static inline
void *____coobj_mholder_lnode_get_child_node_list(
	struct ____coobj_mholder_lnode_s *lnode)
{
	return lnode->child_node_list;
}
static inline
struct ____coobj_mholder_dnode_s *____coobj_mholder_lnode_get_child_dnode(
	struct ____coobj_mholder_lnode_s *lnode,
	__uptr_t index)
{
	return &lnode->child_dnode_list[index];
}
static inline
struct ____coobj_mholder_lnode_s *____coobj_mholder_lnode_get_child_lnode(
	struct ____coobj_mholder_lnode_s *lnode,
	__uptr_t index)
{
	return &lnode->child_lnode_list[index];
}
static inline
____coobj_shmid_t ____coobj_mholder_lnode_get_child_lpage_mshmid(
	struct ____coobj_mholder_lnode_s *lnode)
{
	return lnode->child_lpage_mshmid;
}
static inline
____coobj_shmid_t ____coobj_mholder_lnode_get_child_lpage_hshmid(
	struct ____coobj_mholder_lnode_s *lnode)
{
	return lnode->child_lpage_hshmid;
}
static inline
__uptr_t ____coobj_mholder_lnode_get_child_lpage_mva(
	struct ____coobj_mholder_lnode_s *lnode)
{
	return lnode->child_lpage_mva;
}
static inline
struct ____coobj_uholder_dnode_s *____coobj_mholder_lnode_get_child_lpage_dnode(
	struct ____coobj_mholder_lnode_s *lnode,
	__uptr_t index)
{
	return &lnode->child_lpage_dnode_mva[index];
}
static inline
struct ____coobj_uholder_lnode_s *____coobj_mholder_lnode_get_child_lpage_lnode(
	struct ____coobj_mholder_lnode_s *lnode,
	__uptr_t index)
{
	return &lnode->child_lpage_lnode_mva[index];
}
static inline
__uptr_t ____coobj_mholder_lnode_get_child_lpage_hva(
	struct ____coobj_mholder_lnode_s *lnode)
{
	return lnode->child_lpage_hva;
}

static inline
void *____coobj_mrenter_lnode_get_child_node_list(
	struct ____coobj_mrenter_lnode_s *lnode)
{
	return lnode->child_node_list;
}
static inline
struct ____coobj_mrenter_dnode_s *____coobj_mrenter_lnode_get_child_dnode(
	struct ____coobj_mrenter_lnode_s *lnode,
	__uptr_t index)
{
	return &lnode->child_dnode_list[index];
}
static inline
struct ____coobj_mrenter_lnode_s *____coobj_mrenter_lnode_get_child_lnode(
	struct ____coobj_mrenter_lnode_s *lnode,
	__uptr_t index)
{
	return &lnode->child_lnode_list[index];
}
static inline
____coobj_shmid_t ____coobj_mrenter_lnode_get_child_lpage_rshmid(
	struct ____coobj_mrenter_lnode_s *lnode)
{
	return lnode->child_lpage_rshmid;
}
static inline
____coobj_shmid_t ____coobj_mrenter_lnode_get_child_lpage_mshmid(
	struct ____coobj_mrenter_lnode_s *lnode)
{
	return lnode->child_lpage_mshmid;
}
static inline
__uptr_t ____coobj_mrenter_lnode_get_child_lpage_mva(
	struct ____coobj_mrenter_lnode_s *lnode)
{
	return lnode->child_lpage_mva;
}
static inline
struct ____coobj_urenter_node_s *____coobj_mrenter_lnode_get_child_urenter_node(
	struct ____coobj_mrenter_lnode_s *lnode,
	__uptr_t index)
{
	return &lnode->child_lpage_node_mva[index];
}
static inline
struct ____coobj_mrenter_shmst_s ____coobj_mrenter_lnode_get_child_lpage_rshmst(
	struct ____coobj_mrenter_lnode_s *lnode)
{
	return lnode->child_lpage_rinfo.page_shmst;
}
static inline
__uptr_t ____coobj_mrenter_lnode_get_child_lpage_rva(
	struct ____coobj_mrenter_lnode_s *lnode)
{
	return lnode->child_lpage_rinfo.page_uva;
}

/***********************
 * uholder for lnode-1
 ***********************/
static inline
__bool ____coobj_uholder_l1node_alloc_check_nolock(
	struct ____coobj_uholder_lnode_s *lnode,
	__relpos_t tepar_chunk_order,
	__relpos_t tepar_level1_order)
{
	__bool check_result = __false;
	if (____coobj_relpos_check(lnode->atords.free_list.head, tepar_level1_order)) {
		/* If level free_list is not empty, the head should be valid relpos */
		struct ____coobj_uholder_dnode_s *dnode = &lnode->child_dnode_list[lnode->atords.free_list.head];
		check_result = ____coobj_relpos_check(dnode->atords.free_list.head, tepar_chunk_order);
	} else if (____coobj_relpos_check(lnode->atords.shrink_list.head, tepar_level1_order)) {
		/* If the shrink_list is not empty, the allocation could from shrink_list */
		check_result = __true;
	}
	return check_result;
}

static inline __always_inline
__relpos_t ____coobj_uholder_l1node_alloc_fastpath_nolock(
	struct ____coobj_uholder_lnode_s *lnode,
	__relpos_t tepar_type_size,
	__relpos_t tepar_chunk_order,
	__relpos_t tepar_level1_order)
{
	if (!____coobj_relpos_check(lnode->atords.free_list.head, tepar_level1_order)) {
		/* If free_list is empty, move one chunk from shrink_list to free_list */
		__relpos_t shrink_relpos = lnode->atords.shrink_list.head;
		____coobj_dlist_del(&lnode->atords.shrink_list, lnode->page_uva,
				    ____COOBJ_UHOLDER_DNODE_SIZE, shrink_relpos);
		____coobj_dlist_add(&lnode->atords.free_list, lnode->page_uva,
				    ____COOBJ_UHOLDER_DNODE_SIZE, shrink_relpos);
	}

	/* Allocate from free_list */
	struct ____coobj_uholder_dnode_s *dnode = &lnode->child_dnode_list[lnode->atords.free_list.head];
	__relpos_t chunk_relpos = ____coobj_uholder_dnode_alloc_fastpath_nolock(dnode, tepar_type_size);
	__relpos_t relpos = (lnode->atords.free_list.head << tepar_chunk_order) | chunk_relpos;
	if (dnode->atords.free_list.head == ____COOBJ_INVALID_POS) {
		____coobj_dlist_pop(&lnode->atords.free_list, lnode->page_uva, ____COOBJ_UHOLDER_DNODE_SIZE);
	}
	return relpos;
}

static inline
__bool ____coobj_uholder_l1node_free_check_nolock(
	void *lnode, __relpos_t relpos, __relpos_t tepar_chunk_order,
	__relpos_t tepar_level1_order, __bool var_ator)
{
	__bool check_result = __false;
	if (____coobj_relpos_check(relpos >> tepar_chunk_order, tepar_level1_order)) {
		if (!var_ator) {
			struct ____coobj_uholder_dnode_s *dnode = &((struct ____coobj_uholder_lnode_s *)lnode)
				->child_dnode_list[relpos >> tepar_chunk_order];
			if (dnode->page_uva != NULL) {
				check_result = __true;
			}
		} else {
			struct ____coobj_radix1var_uholder_dnode_s *dnode = &((struct ____coobj_radix1var_uholder_lnode_s *)lnode)
				->child_dnode_list[relpos >> tepar_chunk_order];
			if (dnode->page_uva != NULL) {
				check_result = __true;
			}
		}
	}
	return check_result;
}

static inline __always_inline
void ____coobj_uholder_l1node_free_fastpath_nolock(
	struct ____coobj_uholder_lnode_s *lnode,
	__relpos_t relpos,
	__relpos_t tepar_type_size,
	__relpos_t tepar_chunk_order,
	__relpos_t tepar_level1_order)
{
	__relpos_t level1_relpos = relpos >> tepar_chunk_order;
	__relpos_t chunk_relpos = relpos & ((1 << tepar_chunk_order) - 1);
	struct ____coobj_uholder_dnode_s *dnode = &lnode->child_dnode_list[level1_relpos];
	____coobj_uholder_dnode_free_fastpath_nolock(dnode, chunk_relpos, tepar_type_size, tepar_chunk_order);
	__relpos_t free_size = dnode->atords.free_list.size;
	if (free_size == ((__relpos_t)1 << tepar_chunk_order)) {
		/* If tepar_chunk_order is 0, then we should add the free chunk to shrink_list,
		 * so that it could be shrinked */
		____coobj_dlist_del(&lnode->atords.free_list, lnode->page_uva,
				    ____COOBJ_UHOLDER_DNODE_SIZE, level1_relpos);
		____coobj_dlist_add(&lnode->atords.shrink_list, lnode->page_uva,
				    ____COOBJ_UHOLDER_DNODE_SIZE, level1_relpos);
	} else if (free_size == 1) {
		____coobj_dlist_add(&lnode->atords.free_list, lnode->page_uva,
				    ____COOBJ_UHOLDER_DNODE_SIZE, level1_relpos);
	}
}

static inline
void ____coobj_uholder_l1node_extend_page(
	struct ____coobj_uholder_lnode_s *lnode,
	__relpos_t tepar_level1_order)
{
	____coobj_dlist_init(&lnode->atords.free_list);

	/* Initialize the null_list */
	____coobj_dlist_init(&lnode->atords.null_list);
	for (__relpos_t relpos = 0; relpos < ((__relpos_t)1 << tepar_level1_order); relpos++) {
		____coobj_uholder_dnode_init(&lnode->child_dnode_list[relpos]);
	}
	____coobj_dlist_init_link(&lnode->atords.null_list, lnode->page_uva,
				  ____COOBJ_UHOLDER_DNODE_SIZE, tepar_level1_order);
	____coobj_dlist_init(&lnode->atords.shrink_list);
}

static inline __always_inline
int ____coobj_uholder_l1node_extend_nolock(
	struct ____coobj_uholder_lnode_s *lnode,
	struct ____coobj_base_user_s *base_user,
	__relpos_t baseof_level1,
	coobj_typeid_t tepar_typeid,
	__relpos_t tepar_type_size,
	__relpos_t tepar_chunk_order,
	__relpos_t tepar_level1_order)
{
	int err = E_HM_OK;
	if (lnode->page_uva == NULL) {
		err = ____coobj_base_user_uholder_map(
			base_user, ____coobj_chunkid(tepar_typeid, 0x1, baseof_level1), 0, __false);
		if (err == E_HM_OK) {
			____coobj_uholder_l1node_extend_page(lnode, tepar_level1_order);
		}
	} else if (lnode->atords.null_list.head == ____COOBJ_INVALID_POS) {
		err = E_HM_EXIST;
	}
	if (err == E_HM_OK) {
		/* Allocate one new chunk */
		__relpos_t level1_relpos = lnode->atords.null_list.head;
		struct ____coobj_uholder_dnode_s *dnode = &lnode->child_dnode_list[level1_relpos];
		__relpos_t baseof_chunk = baseof_level1 + (level1_relpos << tepar_chunk_order);
		err = ____coobj_uholder_dnode_extend_nolock(
			dnode, base_user, baseof_chunk, tepar_typeid, tepar_type_size, tepar_chunk_order);
		if (err == E_HM_OK) {
			/* The newly allocated chunk is firstly moved from null_list to shrink_list */
			____coobj_dlist_pop(&lnode->atords.null_list, lnode->page_uva,
					    ____COOBJ_UHOLDER_DNODE_SIZE);
			____coobj_dlist_add(&lnode->atords.shrink_list, lnode->page_uva,
					    ____COOBJ_UHOLDER_DNODE_SIZE, level1_relpos);
		}
	}
	return err;
}

static inline
int ____coobj_uholder_l1node_shrink_nolock(
	struct ____coobj_uholder_lnode_s *lnode,
	struct ____coobj_base_user_s *base_user,
	__relpos_t baseof_level1,
	coobj_typeid_t tepar_typeid,
	__relpos_t tepar_type_size,
	__relpos_t tepar_chunk_order,
	__relpos_t tepar_level1_order)
{
	int err = E_HM_OK;
	__relpos_t level1_relpos = lnode->atords.shrink_list.head;
	if (err == E_HM_OK) {
		if (level1_relpos == ____COOBJ_INVALID_POS) {
			err = E_HM_NOENT;
		}
	}
	if (err == E_HM_OK) {
		__relpos_t baseof_chunk = baseof_level1 + (level1_relpos << tepar_chunk_order);
		err = ____coobj_base_user_uholder_unmap(
			base_user, ____coobj_chunkid(tepar_typeid, 0x0, baseof_chunk), __false);
	}
	if (err == E_HM_OK) {
		____coobj_dlist_pop(&lnode->atords.shrink_list, lnode->page_uva,
				    ____COOBJ_UHOLDER_DNODE_SIZE);
		____coobj_dlist_add(&lnode->atords.null_list, lnode->page_uva,
				    ____COOBJ_UHOLDER_DNODE_SIZE, level1_relpos);
	}
	if (err == E_HM_OK) {
		if (lnode->atords.null_list.size == ((__relpos_t)1 << tepar_level1_order)) {
			err = ____coobj_base_user_uholder_unmap(
				base_user, ____coobj_chunkid(tepar_typeid, 0x1, baseof_level1), __false);
			if (err == E_HM_OK) {
				lnode->atords.null_list = ____COOBJ_DLIST_INIT;
			}
		}
	}
	return err;
}

static inline
int ____coobj_mholder_map_lnode_nolock(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *holder_shmvs,
	struct ____coobj_resv_vm_info_s *resv_vm_info, __uptr_t datalen, struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_mholder_lnode_s *holder_lnode, __bool var_ator)
{
	____coobj_shmid_t child_lpage_mshmid = ____COOBJ_INVALID_SHMID;
	____coobj_shmid_t child_lpage_hshmid = ____COOBJ_INVALID_SHMID;
	__uptr_t child_lpage_mva = ____COOBJ_INVALID_PAGE;
	__uptr_t child_lpage_hva = ____COOBJ_INVALID_PAGE;

	int err = ____coobj_base_mapper_map_notaddr(
		base_mapper, mapper_shmvs, datalen, COOBJ_MAPPER_PERM_RW,
		&child_lpage_mshmid, &child_lpage_mva, __true);
	if (err == E_HM_OK) {
		if (resv_vm_info == NULL || resv_vm_info->resv_vm_cur == 0) {
			err = ____coobj_base_mapper_map_notaddr_exist(
				base_mapper, holder_shmvs, datalen, COOBJ_MAPPER_PERM_RW, mapper_shmvs,
				child_lpage_mshmid, &child_lpage_hshmid, &child_lpage_hva, __false);
		} else {
			err = ____coobj_base_mapper_map_fixaddr_exist_update(
				base_mapper, holder_shmvs, datalen, COOBJ_MAPPER_PERM_RW, mapper_shmvs,
				child_lpage_mshmid, &child_lpage_hshmid, resv_vm_info, &child_lpage_hva, __false);
		}
		if (err != E_HM_OK) {
			____coobj_base_mapper_unmap_fixaddr(
				base_mapper, mapper_shmvs, datalen,
				child_lpage_mshmid, NULL, child_lpage_mva, __true);
		}
	}
	if (err == E_HM_OK) {
		____coobj_mholder_lnode_set_child_lpage_mshmid(holder_lnode, child_lpage_mshmid);
		____coobj_mholder_lnode_set_child_lpage_hshmid(holder_lnode, child_lpage_hshmid);
		____coobj_mholder_lnode_set_child_lpage_mva(holder_lnode, child_lpage_mva);
		____coobj_mholder_lnode_set_child_lpage_hva(holder_lnode, child_lpage_hva, var_ator);
	}
	return err;
}

static inline
int ____coobj_mholder_unmap_lnode_nolock(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *holder_shmvs,
	struct ____coobj_resv_vm_info_s *resv_vm_info, __uptr_t datalen,
	struct coobj_vspace_s *mapper_shmvs, struct ____coobj_mholder_lnode_s *holder_lnode, __bool var_ator)
{
	____coobj_shmid_t child_lpage_hshmid = ____coobj_mholder_lnode_get_child_lpage_hshmid(holder_lnode);
	__uptr_t child_lpage_hva = ____coobj_mholder_lnode_get_child_lpage_hva(holder_lnode);

	____coobj_base_mapper_unmap_fixaddr(
		base_mapper, holder_shmvs, datalen,
		child_lpage_hshmid, resv_vm_info, child_lpage_hva, __false);

	____coobj_shmid_t child_lpage_mshmid = ____coobj_mholder_lnode_get_child_lpage_mshmid(holder_lnode);
	__uptr_t child_lpage_mva = ____coobj_mholder_lnode_get_child_lpage_mva(holder_lnode);

	____coobj_base_mapper_unmap_fixaddr(
		base_mapper, mapper_shmvs, datalen,
		child_lpage_mshmid, NULL, child_lpage_mva, __true);

	____coobj_mholder_lnode_set_child_lpage_mshmid(holder_lnode, ____COOBJ_INVALID_SHMID);
	____coobj_mholder_lnode_set_child_lpage_hshmid(holder_lnode, ____COOBJ_INVALID_SHMID);
	____coobj_mholder_lnode_set_child_lpage_mva(holder_lnode, ____COOBJ_INVALID_PAGE);
	____coobj_mholder_lnode_set_child_lpage_hva(holder_lnode, ____COOBJ_INVALID_PAGE, var_ator);
	return E_HM_OK;
}

static inline
int ____coobj_mrenter_map_lnode_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *renter_shmvs,
	struct ____coobj_resv_vm_info_s *resv_vm_info, __uptr_t datalen,
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_mrenter_lnode_s *renter_lnode)
{
	int err = E_HM_OK;

	____coobj_shmid_t child_lpage_mshmid = ____COOBJ_INVALID_SHMID;
	__uptr_t child_lpage_mva = ____COOBJ_INVALID_PAGE;
	if (err == E_HM_OK) {
		err = ____coobj_base_mapper_map_notaddr(
			base_mapper, mapper_shmvs, datalen, COOBJ_MAPPER_PERM_RW,
			&child_lpage_mshmid, &child_lpage_mva, __true);
	}
	struct ____coobj_mrenter_map_params_s params = {
		.perm = COOBJ_MAPPER_PERM_RW,
		.datalen = datalen,
		.owner_shmid = child_lpage_mshmid,
		.renter_shmid = &renter_lnode->child_lpage_rshmid,
		.renter_shmvs = renter_shmvs,
		.owner_shmvs = mapper_shmvs,
		.base_mapper = base_mapper,
		.urenter_node_mva = renter_lnode->urenter_node_mva,
		.page_info = &renter_lnode->child_lpage_rinfo,
		.resv_vm_info = resv_vm_info,
	};
	if (err == E_HM_OK) {
		err = ____coobj_mrenter_map_page_info(&params);
		if (err != E_HM_OK) {
			____coobj_base_mapper_unmap_fixaddr(
				base_mapper, mapper_shmvs, datalen,
				child_lpage_mshmid, NULL, child_lpage_mva, __true);
		}
	}
	if (err == E_HM_OK) {
		____coobj_mrenter_lnode_set_child_lpage_mshmid(renter_lnode, child_lpage_mshmid);
		____coobj_mrenter_lnode_set_child_lpage_mva(renter_lnode, child_lpage_mva);
	}
	return err;
}

static inline
int ____coobj_mrenter_unmap_lnode_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *renter_shmvs,
	struct ____coobj_resv_vm_info_s *resv_vm_info, __uptr_t datalen,
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_mrenter_lnode_s *renter_lnode)
{
	____coobj_shmid_t child_lpage_rshmid = ____coobj_mrenter_lnode_get_child_lpage_rshmid(renter_lnode);
	____coobj_mrenter_unmap_page_info(
		base_mapper, renter_shmvs, datalen,
		child_lpage_rshmid, renter_lnode->urenter_node_mva, &renter_lnode->child_lpage_rinfo, resv_vm_info);

	____coobj_shmid_t child_lpage_mshmid = ____coobj_mrenter_lnode_get_child_lpage_mshmid(renter_lnode);
	__uptr_t child_lpage_mva = ____coobj_mrenter_lnode_get_child_lpage_mva(renter_lnode);
	____coobj_base_mapper_unmap_fixaddr(
		base_mapper, mapper_shmvs, datalen,
		child_lpage_mshmid, NULL, child_lpage_mva, __true);

	____coobj_mrenter_lnode_set_child_lpage_mshmid(renter_lnode, ____COOBJ_INVALID_SHMID);
	____coobj_mrenter_lnode_set_child_lpage_rshmid(renter_lnode, ____COOBJ_INVALID_SHMID);
	____coobj_mrenter_lnode_set_child_lpage_mva(renter_lnode, ____COOBJ_INVALID_PAGE);
	____coobj_mrenter_lnode_set_child_lpage_rshmst(renter_lnode, ____COOBJ_MRENTER_SHMST_STATUS_UNMAP_NONE);
	____coobj_mrenter_lnode_set_child_lpage_rva(renter_lnode, ____COOBJ_INVALID_PAGE);
	return E_HM_OK;
}

static inline
int ____coobj_mrenter_unmap_child_node_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	__uptr_t datalen,
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_mrenter_lnode_s *renter_lnode)
{
	____coobj_shmid_t child_lpage_mshmid = ____coobj_mrenter_lnode_get_child_lpage_mshmid(renter_lnode);
	__uptr_t child_lpage_mva = ____coobj_mrenter_lnode_get_child_lpage_mva(renter_lnode);
	if (child_lpage_mva != 0) {
		____coobj_base_mapper_unmap_fixaddr(
			base_mapper, mapper_shmvs, datalen,
			child_lpage_mshmid, NULL, child_lpage_mva, __true);
	}
	return E_HM_OK;
}

#endif
