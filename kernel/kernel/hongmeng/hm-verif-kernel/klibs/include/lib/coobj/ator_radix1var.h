/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: variable-sized object allocator
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 21 15:34:34 2023 +0800
 */
#ifndef KLIBS_COOBJ_ATOR_RADIX1VAR_H
#define KLIBS_COOBJ_ATOR_RADIX1VAR_H

#include <lib/coobj/ator_base.h>

#define ____coobj_radix1var_urenter_acqst_size() 0x4

/* To do: shrink_list needs to be refactored to adapt current GC method */
static inline
__relpos_t ____coobj_radix1var_uholder_alloc_nolock(
	__relpos_t size, struct ____coobj_radix1var_uholder_s *uholder, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	__relpos_t relpos = ____COOBJ_INVALID_POS;
	__relpos_t obj_idx = ____COOBJ_INVALID_POS;
	__relpos_t target_idx = ____COOBJ_INVALID_POS;
	if (____coobj_radix1var_uholder_object_alloc_check_nolock(
		size, &obj_idx, &target_idx, &uholder->lnode, &uholder->free_list, tepar_chunk_order, tepar_level1_order) ||
		____coobj_radix1var_uholder_l1node_extend_nolock(&uholder->lnode, &uholder->free_list,
			uholder->base_user, &obj_idx, target_idx, 0, uholder->type_id, uholder->flags,
			____COOBJ_RADIX1VAR_TEPAR_ARG) == E_HM_OK) {
		relpos = ____coobj_radix1var_uholder_alloc_fastpath_nolock(
			&uholder->lnode, &uholder->free_list, obj_idx, target_idx, tepar_type_size, tepar_chunk_order,
			uholder->flags);
	}
	return relpos;
}

static inline
__relpos_t ____coobj_radix1var_uholder_alloc_slowpath_nolock(
	__relpos_t size, struct ____coobj_radix1var_uholder_s *uholder, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	__relpos_t relpos = ____COOBJ_INVALID_POS;
	if (____coobj_radix1var_uholder_object_alloc_slowpath_check_extend_nolock(
		&uholder->lnode_slow, uholder->base_user, 0, uholder->type_id, tepar_level1_order)) {
		__relpos_t aligned_size = ((size + ((__relpos_t)1 << ____COOBJ_PAGE_ORDER_4K) - 1) >>
			____COOBJ_PAGE_ORDER_4K) << ____COOBJ_PAGE_ORDER_4K;
		relpos = ____coobj_radix1var_uholder_alloc_slowpath_pop_pos_nolock(&uholder->lnode_slow,
			uholder->base_user, aligned_size, 0, uholder->type_id, tepar_chunk_order);
	}
	return relpos;
}

static inline
__bool ____coobj_radix1var_relpos_check(__relpos_t relpos, __relpos_t tepar_chunk_order, __relpos_t tepar_type_size)
{
	__relpos_t chunk_relpos = relpos & ((1 << tepar_chunk_order) - 1);
	return (chunk_relpos >= RADIX1VAR_REAL_DATA_OFFSET / tepar_type_size) ?
		__true : __false;
}

static inline
int ____coobj_radix1var_uholder_free_nolock(
	struct ____coobj_radix1var_uholder_s *uholder, __relpos_t relpos, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	int err = E_HM_NOENT;
	if (____coobj_uholder_l1node_free_check_nolock(
			&uholder->lnode, relpos, tepar_chunk_order, tepar_level1_order, __true) &&
	    ____coobj_radix1var_relpos_check(relpos, tepar_chunk_order, tepar_type_size)) {
		____coobj_radix1var_uholder_l1node_free_fastpath_nolock(
			&uholder->lnode, &uholder->free_list, relpos, tepar_type_size, tepar_chunk_order, uholder->flags);
		err = E_HM_OK;
	}
	return err;
}

static inline
int ____coobj_radix1var_uholder_free_unmap_nolock(struct ____coobj_base_user_s *base_user, void *lnode,
	__relpos_t relpos, __relpos_t baseof_level1, coobj_typeid_t tepar_typeid, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	int err = E_HM_INVAL;
	struct ____coobj_radix1var_uholder_lnode_s *uholder_lnode = (struct ____coobj_radix1var_uholder_lnode_s *)lnode;
	/* unmap the chunk in slowpath */
	err = ____coobj_base_user_uholder_unmap(
		base_user, ____coobj_chunkid(tepar_typeid, 0x0, relpos), __true);
	if (err == E_HM_OK) {
		____coobj_dlist_add(&uholder_lnode->atords.null_list, uholder_lnode->page_uva,
			____COOBJ_RADIX1VAR_UHOLDER_DNODE_SIZE, relpos >> tepar_chunk_order);
	}
	return err;
}

static inline
int ____coobj_radix1var_uholder_free_slowpath_nolock(
	struct ____coobj_radix1var_uholder_s *uholder, __relpos_t relpos, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	int err = E_HM_NOENT;
	if (____coobj_uholder_l1node_free_check_nolock(
			&uholder->lnode_slow, relpos, tepar_chunk_order, tepar_level1_order, __true)) {
		err = ____coobj_radix1var_uholder_free_unmap_nolock(uholder->base_user, &uholder->lnode_slow, relpos,
			0, uholder->type_id, ____COOBJ_RADIX1VAR_TEPAR_ARG);
	}
	return err;
}

static inline
__relpos_t ____coobj_radix1var_uholder_alloc(
	__relpos_t size, struct ____coobj_radix1var_uholder_s *uholder, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	__relpos_t relpos = ____COOBJ_INVALID_POS;
	____coobj_base_user_lock(uholder->base_user);
	if (size <= L_RADIX1VAR_MAX_OBJ_SIZE && size > 0) {
		relpos = ____coobj_radix1var_uholder_alloc_nolock(size, uholder, ____COOBJ_RADIX1VAR_TEPAR_ARG);
	} else {
		relpos = ____coobj_radix1var_uholder_alloc_slowpath_nolock(size, uholder, ____COOBJ_RADIX1VAR_TEPAR_ARG);
	}
	____coobj_base_user_unlock(uholder->base_user);
	return relpos;
}

static inline
int ____coobj_radix1var_uholder_free(
	struct ____coobj_radix1var_uholder_s *uholder, __relpos_t relpos, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	int err = E_HM_INVAL;
	if (relpos == ____COOBJ_INVALID_POS) {
		return err;
	}
	____coobj_base_user_lock(uholder->base_user);
	if (!____coobj_radix1var_is_large_obj(relpos)) {
		err = ____coobj_radix1var_uholder_free_nolock(uholder, relpos, ____COOBJ_RADIX1VAR_TEPAR_ARG);
	} else {
		err = ____coobj_radix1var_uholder_free_slowpath_nolock(uholder, relpos, ____COOBJ_RADIX1VAR_TEPAR_ARG);
	}
	____coobj_base_user_unlock(uholder->base_user);
	return err;
}
/**************************************************
 * mholder
 **************************************************/
static inline
void ____coobj_radix1var_mholder_init_shmvs(
	struct ____coobj_radix1var_mholder_s *mholder,
	struct coobj_vspace_s *holder_shmvs)
{
	mholder->holder_shmvs = holder_shmvs;
}
static inline
void ____coobj_radix1var_mholder_init_resv_vm(
	struct ____coobj_radix1var_mholder_s *mholder, __uptr_t resv_vm_start)
{
	mholder->resv_vm_info.resv_vm_start = resv_vm_start;
	mholder->resv_vm_info.resv_vm_cur = resv_vm_start;
	mholder->resv_vm_info.resv_vm_end = resv_vm_start + COOBJ_RESERVED_MEM_SIZE;
}

static inline
int ____coobj_radix1var_mholder_unmap_dnode_until_nolock(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1var_mholder_s *mholder, __bool large_obj, __uptr_t dnode_index, __uptr_t dnode_datalen,
	struct ____coobj_radix1var_mrenter_s *mrenter_until, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	struct ____coobj_mholder_dnode_s *holder_dnode = ____coobj_mholder_lnode_get_child_dnode(
			____coobj_radix1var_mholder_get_lnode(mholder, large_obj), dnode_index);
	if (large_obj) {
		dnode_datalen =____coobj_mholder_dnode_get_dpage_size(holder_dnode);
	}
	for (struct ____coobj_radix1var_mrenter_s *mrenter = mholder->head;
		mrenter != mrenter_until;
		mrenter = mrenter->next) {
		struct coobj_vspace_s *renter_shmvs = mrenter->renter_shmvs;
		if (renter_shmvs == NULL || (____coobj_radix1var_mrenter_get_lnode(mrenter, large_obj)->child_node_list == NULL)) {
			continue;
		}
		struct ____coobj_mrenter_dnode_s *renter_dnode = ____coobj_mrenter_lnode_get_child_dnode(
			____coobj_radix1var_mrenter_get_lnode(mrenter, large_obj), dnode_index);

		____coobj_mrenter_unmap_dnode_nolock(base_mapper, renter_shmvs, &mrenter->resv_vm_info,
						     dnode_datalen, renter_dnode);
	}
	struct coobj_vspace_s *holder_shmvs = mholder->holder_shmvs;
	____coobj_mholder_unmap_dnode_nolock(base_mapper, holder_shmvs, dnode_datalen, &mholder->resv_vm_info,
					     holder_dnode, __true);

	return E_HM_OK;
}

static inline
void ____coobj_mrenter_clean_lnode_nolock(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *renter_shmvs,
	struct ____coobj_resv_vm_info_s *resv_vm_info, __uptr_t lnode_datalen, struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_mrenter_lnode_s *renter_lnode)
{
	____coobj_mrenter_unmap_lnode_nolock(base_mapper, renter_shmvs, resv_vm_info, lnode_datalen,
					     mapper_shmvs, renter_lnode);
	void *child_node_list = ____coobj_mrenter_lnode_get_child_node_list(renter_lnode);
	____coobj_base_mapper_free(base_mapper, child_node_list);
	____coobj_mrenter_lnode_set_child_node_list(renter_lnode, NULL);
}

/* clean metadata when renter exits */
static inline
void ____coobj_mrenter_clean_child_node_nolock(
	struct ____coobj_base_mapper_s *base_mapper, __uptr_t lnode_datalen,
	struct coobj_vspace_s *mapper_shmvs, struct ____coobj_mrenter_lnode_s *renter_lnode)
{
	____coobj_mrenter_unmap_child_node_nolock(base_mapper, lnode_datalen, mapper_shmvs, renter_lnode);
	void *child_node_list = ____coobj_mrenter_lnode_get_child_node_list(renter_lnode);
	____coobj_base_mapper_free(base_mapper, child_node_list);
}

static inline
int ____coobj_radix1var_mholder_unmap_lnode_until_nolock(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1var_mholder_s *mholder, __bool large_obj, __uptr_t uholder_lnode_datalen,
	__uptr_t urenter_lnode_datalen, struct ____coobj_radix1var_mrenter_s *mrenter_until,
	____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	for (struct ____coobj_radix1var_mrenter_s *mrenter = mholder->head;
		mrenter != mrenter_until;
		mrenter = mrenter->next) {
		struct coobj_vspace_s *renter_shmvs = mrenter->renter_shmvs;
		struct ____coobj_mrenter_lnode_s *renter_lnode =
			____coobj_radix1var_mrenter_get_lnode(mrenter, large_obj);
		____coobj_mrenter_clean_lnode_nolock(
			base_mapper, renter_shmvs, &mrenter->resv_vm_info, urenter_lnode_datalen, mapper_shmvs, renter_lnode);
	}
	{
		struct coobj_vspace_s *holder_shmvs = mholder->holder_shmvs;
		struct ____coobj_mholder_lnode_s *holder_lnode =
			____coobj_radix1var_mholder_get_lnode(mholder, large_obj);

		____coobj_mholder_unmap_lnode_nolock(
			base_mapper, holder_shmvs, &mholder->resv_vm_info, uholder_lnode_datalen, mapper_shmvs,
			holder_lnode, __true);

		void *child_node_list = ____coobj_mholder_lnode_get_child_node_list(holder_lnode);
		____coobj_base_mapper_free(base_mapper, child_node_list);
		____coobj_mholder_lnode_set_child_node_list(holder_lnode, NULL);
	}
	return E_HM_OK;
}

static inline
int ____coobj_radix1var_mholder_unmap_until_nolock(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1var_mholder_s *mholder, struct ____coobj_chunkid_s chunkid,
	struct ____coobj_radix1var_mrenter_s *mrenter_until, __bool large_obj, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	int err = E_HM_OK;
	switch (chunkid.level) {
	case 0x0:
		{
			__uptr_t dnode_index =
				____coobj_radix1var_eval_dnode_index(chunkid, ____COOBJ_RADIX1VAR_TEPAR_ARG);
			__uptr_t dnode_datalen =
				____coobj_radix1var_eval_dnode_size(____COOBJ_RADIX1VAR_TEPAR_ARG);

			err = ____coobj_radix1var_mholder_unmap_dnode_until_nolock(
				base_mapper, mapper_shmvs, mholder, large_obj, dnode_index, dnode_datalen,
				mrenter_until, ____COOBJ_RADIX1VAR_TEPAR_ARG);
		}
		break;
	case 0x1:
		{
			__uptr_t uholder_lnode_datalen =
				____coobj_radix1var_eval_uholder_lnode_size(____COOBJ_RADIX1VAR_TEPAR_ARG);
			__uptr_t urenter_lnode_datalen =
				____coobj_radix1var_eval_urenter_lnode_size(____COOBJ_RADIX1VAR_TEPAR_ARG);

			err = ____coobj_radix1var_mholder_unmap_lnode_until_nolock(
				base_mapper, mapper_shmvs, mholder, large_obj, uholder_lnode_datalen,
				urenter_lnode_datalen, mrenter_until, ____COOBJ_RADIX1VAR_TEPAR_ARG);
		}
		break;
	default:
		err = E_HM_INVAL;
		break;
	}
	return err;
}

static inline
int ____coobj_radix1var_mholder_map_dnode_nolock(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1var_mholder_s *mholder, __bool large_obj, __uptr_t dnode_index,
	__uptr_t dnode_datalen, unsigned int flags, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	int err = E_HM_OK;

	struct coobj_vspace_s *holder_shmvs = mholder->holder_shmvs;
	struct ____coobj_mholder_dnode_s *mholder_dnode = ____coobj_mholder_lnode_get_child_dnode(
		____coobj_radix1var_mholder_get_lnode(mholder, large_obj), dnode_index);
	struct ____coobj_radix1var_uholder_dnode_s *uholder_dnode = ____coobj_radix1var_mholder_lnode_get_child_lpage_dnode(
		____coobj_radix1var_mholder_get_lnode(mholder, large_obj), dnode_index);

	____coobj_radix1var_mholder_dnode_init(mholder_dnode, uholder_dnode);
	if (large_obj) {
		____coobj_mholder_dnode_set_dpage_size(mholder_dnode, dnode_datalen);
	}
	err = ____coobj_mholder_map_dnode_nolock(
		base_mapper, holder_shmvs, &mholder->resv_vm_info,
		dnode_datalen, mholder_dnode, __true, flags);

	____coobj_shmid_t dpage_hshmid = ____COOBJ_INVALID_SHMID;
	if (err == E_HM_OK) {
		dpage_hshmid = ____coobj_mholder_dnode_get_dpage_hshmid(mholder_dnode);

		for (struct ____coobj_radix1var_mrenter_s *mrenter = mholder->head;
			mrenter != NULL && err == E_HM_OK;
			mrenter = mrenter->next) {
			struct coobj_vspace_s *renter_shmvs = mrenter->renter_shmvs;
			if (renter_shmvs == NULL || (____coobj_radix1var_mrenter_get_lnode(mrenter, large_obj)->child_node_list == NULL)) {
				continue;
			}
			struct ____coobj_mrenter_dnode_s *mrenter_dnode = ____coobj_mrenter_lnode_get_child_dnode(
				____coobj_radix1var_mrenter_get_lnode(mrenter, large_obj), dnode_index);
			struct ____coobj_urenter_node_s *urenter_dnode = ____coobj_mrenter_lnode_get_child_urenter_node(
				____coobj_radix1var_mrenter_get_lnode(mrenter, large_obj), dnode_index);
			____coobj_mrenter_dnode_init(mrenter_dnode, urenter_dnode);

			err = ____coobj_mrenter_map_dnode_nolock(
				base_mapper, renter_shmvs, &mrenter->resv_vm_info, dnode_datalen,
				holder_shmvs, dpage_hshmid, mrenter_dnode);
			if (err != E_HM_OK) {
				____coobj_radix1var_mholder_unmap_dnode_until_nolock(
					base_mapper, mapper_shmvs, mholder, large_obj,
					dnode_index, dnode_datalen, mrenter,
					____COOBJ_RADIX1VAR_TEPAR_ARG);
			}
		}
	}
	return err;
}

static inline
int ____coobj_radix1var_mrenter_create_lnode_nolock(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *mapper_shmvs, __bool large_obj,
	struct ____coobj_radix1var_mrenter_s *mrenter, __uptr_t urenter_lnode_datalen, __uptr_t mrenter_lnode_datalen)
{
	int err = E_HM_OK;
	void *renter_child_node_list = ____coobj_base_mapper_alloc(base_mapper, mrenter_lnode_datalen);
	if (renter_child_node_list == NULL) {
		err = E_HM_NOMEM;
	} else {
		err = ____coobj_mrenter_map_lnode_nolock(
			base_mapper,  mrenter->renter_shmvs, &mrenter->resv_vm_info,
			urenter_lnode_datalen, mapper_shmvs,
			____coobj_radix1var_mrenter_get_lnode(mrenter, large_obj));
		if (err == E_HM_OK) {
			____coobj_mrenter_lnode_set_child_node_list(
				____coobj_radix1var_mrenter_get_lnode(mrenter, large_obj), renter_child_node_list);
		} else {
			____coobj_base_mapper_free(base_mapper, renter_child_node_list);
		}
	}
	return err;
}

static inline
int ____coobj_radix1var_mholder_map_lnode_nolock(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1var_mholder_s *mholder, __bool large_obj, __uptr_t uholder_lnode_datalen,
	__uptr_t urenter_lnode_datalen, __uptr_t mholder_lnode_datalen, __uptr_t mrenter_lnode_datalen,
	____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	int err;

	struct coobj_vspace_s *holder_shmvs = mholder->holder_shmvs;
	void *holder_child_node_list = ____coobj_base_mapper_alloc(base_mapper, mholder_lnode_datalen);
	if (holder_child_node_list == NULL) {
		err = E_HM_NOMEM;
	} else {
		____coobj_mholder_dnode_init_dpage_hshmid(holder_child_node_list, ____COOBJ_RADIX1VAR_TEPAR_ARG);
		err = ____coobj_mholder_map_lnode_nolock(
			base_mapper, holder_shmvs, &mholder->resv_vm_info, uholder_lnode_datalen,
			mapper_shmvs, ____coobj_radix1var_mholder_get_lnode(mholder, large_obj), __true);
		if (err == E_HM_OK) {
			____coobj_mholder_lnode_set_child_node_list(
				____coobj_radix1var_mholder_get_lnode(mholder, large_obj), holder_child_node_list);
		} else {
			____coobj_base_mapper_free(base_mapper, holder_child_node_list);
		}
	}

	if (err == E_HM_OK) {
		for (struct ____coobj_radix1var_mrenter_s *mrenter = mholder->head;
			mrenter != NULL && err == E_HM_OK;
			mrenter = mrenter->next) {
			struct coobj_vspace_s *renter_shmvs = mrenter->renter_shmvs;
			if (renter_shmvs == NULL) {
				continue;
			}
			err = ____coobj_radix1var_mrenter_create_lnode_nolock(base_mapper, mapper_shmvs, large_obj, mrenter,
									      urenter_lnode_datalen,  mrenter_lnode_datalen);
			if (err != E_HM_OK) {
				____coobj_radix1var_mholder_unmap_lnode_until_nolock(
					base_mapper, mapper_shmvs, mholder, large_obj,
					uholder_lnode_datalen, urenter_lnode_datalen, mrenter,
					____COOBJ_RADIX1VAR_TEPAR_ARG);
			}
		}
	}
	return err;
}

static inline
int ____coobj_radix1var_mholder_map_nolock(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1var_mholder_s *mholder, struct ____coobj_chunkid_s chunkid, __bool large_obj,
	__relpos_t size, unsigned int flags, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	int err = E_HM_OK;
	switch (chunkid.level) {
	case 0x0:
		{
			__uptr_t dnode_index =
				____coobj_radix1var_eval_dnode_index(chunkid, ____COOBJ_RADIX1VAR_TEPAR_ARG);
			__uptr_t dnode_datalen;
			if (large_obj) {
				dnode_datalen = size;
			} else {
				dnode_datalen = ____coobj_radix1var_eval_dnode_size(____COOBJ_RADIX1VAR_TEPAR_ARG);
			}
			err = ____coobj_radix1var_mholder_map_dnode_nolock(
				base_mapper, mapper_shmvs, mholder, large_obj,
				dnode_index, dnode_datalen, flags,
				____COOBJ_RADIX1VAR_TEPAR_ARG);
		}
		break;
	case 0x1:
		{
			__uptr_t uholder_lnode_datalen =
				____coobj_radix1var_eval_uholder_lnode_size(____COOBJ_RADIX1VAR_TEPAR_ARG);
			__uptr_t urenter_lnode_datalen =
				____coobj_radix1var_eval_urenter_lnode_size(____COOBJ_RADIX1VAR_TEPAR_ARG);
			__uptr_t mholder_lnode_datalen =
				____coobj_radix1var_eval_mholder_lnode_size(____COOBJ_RADIX1VAR_TEPAR_ARG);
			__uptr_t mrenter_lnode_datalen =
				____coobj_radix1var_eval_mrenter_lnode_size(____COOBJ_RADIX1VAR_TEPAR_ARG);
			err = ____coobj_radix1var_mholder_map_lnode_nolock(
				base_mapper, mapper_shmvs, mholder, large_obj,
				uholder_lnode_datalen, urenter_lnode_datalen,
				mholder_lnode_datalen, mrenter_lnode_datalen,
				____COOBJ_RADIX1VAR_TEPAR_ARG);
		}
		break;
	default:
		err = E_HM_INVAL;
		break;
	}
	return err;
}

static inline
int ____coobj_radix1var_mholder_unmap_nolock(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1var_mholder_s *mholder, struct ____coobj_chunkid_s chunkid,
	__bool large_obj, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	return ____coobj_radix1var_mholder_unmap_until_nolock(
		base_mapper, mapper_shmvs, mholder, chunkid, NULL, large_obj, ____COOBJ_RADIX1VAR_TEPAR_ARG);
}

static inline
int ____coobj_radix1var_mholder_map(
	struct coobj_vspace_s *mapper_shmvs, struct ____coobj_radix1var_mholder_s *mholder,
	struct ____coobj_chunkid_s chunkid, __u8 *mapped, __relpos_t size, __bool large_obj,
	unsigned int flags, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	struct ____coobj_base_mapper_s *base_mapper = mholder->base_mapper;
	____coobj_base_mapper_sync_lock(base_mapper);
	if (large_obj) {
		*mapped |= LARGE_OBJ_ATOR_MAPPED;
	} else {
		*mapped |= SMALL_OBJ_ATOR_MAPPED;
	}
	int err = ____coobj_radix1var_mholder_map_nolock(
		base_mapper, mapper_shmvs, mholder, chunkid, large_obj, size, flags, ____COOBJ_RADIX1VAR_TEPAR_ARG);
	____coobj_base_mapper_sync_unlock(base_mapper);
	return err;
}

static inline
int ____coobj_radix1var_mholder_unmap(
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1var_mholder_s *mholder,
	struct ____coobj_chunkid_s chunkid, __bool large_obj,
	____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	struct ____coobj_base_mapper_s *base_mapper = mholder->base_mapper;
	____coobj_base_mapper_sync_lock(base_mapper);
	int err = ____coobj_radix1var_mholder_unmap_nolock(
		base_mapper, mapper_shmvs, mholder, chunkid, large_obj, ____COOBJ_RADIX1VAR_TEPAR_ARG);
	____coobj_base_mapper_sync_unlock(base_mapper);
	return err;
}

/**************************************************
 * mrenter
 **************************************************/
static inline
void ____coobj_radix1var_mrenter_init_shmvs(
	struct ____coobj_radix1var_mrenter_s *mrenter, struct coobj_vspace_s *renter_shmvs)
{
	mrenter->renter_shmvs = renter_shmvs;
}
static inline
void ____coobj_radix1var_mrenter_init_resv_vm(
	struct ____coobj_radix1var_mrenter_s *mrenter, __uptr_t resv_vm_start)
{
	mrenter->resv_vm_info.resv_vm_start = resv_vm_start;
	mrenter->resv_vm_info.resv_vm_cur = resv_vm_start;
	mrenter->resv_vm_info.resv_vm_end = resv_vm_start + COOBJ_RESERVED_MEM_SIZE;
}

static inline
void ____coobj_radix1var_mrenter_fini_shmvs_nolock(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1var_mholder_s *mholder, struct ____coobj_radix1var_mrenter_s *mrenter,
	__u8 *mapped, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	if (mrenter == mholder->head) {
		mholder->head = mrenter->next;
	} else {
		struct ____coobj_radix1var_mrenter_s *iter = mholder->head;
		while (iter != NULL && iter->next != mrenter) {
			iter = iter->next;
		}
		if (iter != NULL && iter->next == mrenter) {
			iter->next = mrenter->next;
		}
	}
	__uptr_t urenter_lnode_datalen = ____coobj_radix1var_eval_urenter_lnode_size(____COOBJ_RADIX1VAR_TEPAR_ARG);
	if (((*mapped) & SMALL_OBJ_ATOR_MAPPED) == SMALL_OBJ_ATOR_MAPPED) {
		____coobj_mrenter_clean_child_node_nolock(base_mapper, urenter_lnode_datalen, mapper_shmvs,
							  ____coobj_radix1var_mrenter_get_lnode(mrenter, __false));
	}
	if (((*mapped) & LARGE_OBJ_ATOR_MAPPED) == LARGE_OBJ_ATOR_MAPPED) {
		____coobj_mrenter_clean_child_node_nolock(base_mapper, urenter_lnode_datalen, mapper_shmvs,
							  ____coobj_radix1var_mrenter_get_lnode(mrenter, __true));
	}
}

static inline
void ____coobj_radix1var_mrenter_fini_shmvs(
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1var_mrenter_s *mrenter,
	__u8 *mapped, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	struct ____coobj_radix1var_mholder_s *mholder = mrenter->mholder;
	struct ____coobj_base_mapper_s *base_mapper = mholder->base_mapper;
	____coobj_base_mapper_sync_lock(base_mapper);
	____coobj_radix1var_mrenter_fini_shmvs_nolock(
		base_mapper, mapper_shmvs, mholder, mrenter, mapped, ____COOBJ_RADIX1VAR_TEPAR_ARG);
	____coobj_base_mapper_sync_unlock(base_mapper);
}

static inline
int coobj_radix1var_mrenter_notify_noref(
	struct ____coobj_radix1var_mrenter_s *mrenter, struct ____coobj_chunkid_s chunkid)
{
	return E_HM_OK;
}

static inline
int ____coobj_radix1var_mrenter_map_exist_nolock(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1var_mholder_s *mholder, struct coobj_vspace_s *renter_shmvs,
	__bool large_obj, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	int err = E_HM_OK;
	struct ____coobj_radix1var_mrenter_s *mrenter;
	struct coobj_vspace_s *holder_shmvs = mholder->holder_shmvs;
	if (holder_shmvs == NULL) {
		return E_HM_INVAL;
	}
	struct ____coobj_mholder_lnode_s *lnode = ____coobj_radix1var_mholder_get_lnode(mholder, large_obj);
	if (lnode->child_node_list == NULL) {
		return E_HM_INVAL;
	}
	for (mrenter = mholder->head; mrenter != NULL; mrenter = mrenter->next) {
		if (mrenter->renter_shmvs != NULL &&  mrenter->renter_shmvs == renter_shmvs) {
			break;
		}
	}
	if (mrenter == NULL) {
		return E_HM_NOENT;
	}
	/* map all lnodes (there's only 1 lnode in radix1var now) */
	__uptr_t urenter_lnode_datalen = ____coobj_radix1var_eval_urenter_lnode_size(____COOBJ_RADIX1VAR_TEPAR_ARG);
	__uptr_t mrenter_lnode_datalen = ____coobj_radix1var_eval_mrenter_lnode_size(____COOBJ_RADIX1VAR_TEPAR_ARG);
	err = ____coobj_radix1var_mrenter_create_lnode_nolock(
		base_mapper, mapper_shmvs, large_obj, mrenter, urenter_lnode_datalen,  mrenter_lnode_datalen);
	/* map all dnode */
	__uptr_t dnode_datalen;
	__uptr_t dnode_index;
	for (dnode_index = 0; dnode_index < (1U << tepar_level1_order) && err == E_HM_OK; dnode_index++) {
		struct ____coobj_mholder_dnode_s *mholder_dnode = ____coobj_mholder_lnode_get_child_dnode(
				____coobj_radix1var_mholder_get_lnode(mholder, large_obj), dnode_index);
		____coobj_shmid_t dpage_hshmid = ____COOBJ_INVALID_SHMID;
		if (mholder_dnode != NULL) {
			dpage_hshmid = ____coobj_mholder_dnode_get_dpage_hshmid(mholder_dnode);
		}
		if (dpage_hshmid != ____COOBJ_INVALID_SHMID) {
			if (large_obj) {
				dnode_datalen = ____coobj_mholder_dnode_get_dpage_size(mholder_dnode);
			} else {
				dnode_datalen = ____coobj_radix1var_eval_dnode_size(____COOBJ_RADIX1VAR_TEPAR_ARG);
			}
			struct ____coobj_mrenter_dnode_s *mrenter_dnode = ____coobj_mrenter_lnode_get_child_dnode(
				____coobj_radix1var_mrenter_get_lnode(mrenter, large_obj), dnode_index);
			struct ____coobj_urenter_node_s *urenter_dnode = ____coobj_mrenter_lnode_get_child_urenter_node(
				____coobj_radix1var_mrenter_get_lnode(mrenter, large_obj), dnode_index);
			____coobj_mrenter_dnode_init(mrenter_dnode, urenter_dnode);
			err = ____coobj_mrenter_map_dnode_nolock(
				base_mapper, renter_shmvs, &mrenter->resv_vm_info,
				dnode_datalen, holder_shmvs, dpage_hshmid, mrenter_dnode);
		}
	}
	if (err != E_HM_OK) {
		for (__uptr_t idx = 0; idx < dnode_index; idx++) {
			struct ____coobj_mrenter_dnode_s *mrenter_dnode = ____coobj_mrenter_lnode_get_child_dnode(
				____coobj_radix1var_mrenter_get_lnode(mrenter, large_obj), idx);
			____coobj_mrenter_unmap_dnode_nolock(base_mapper, renter_shmvs, &mrenter->resv_vm_info,
							     dnode_datalen, mrenter_dnode);
		}
		____coobj_mrenter_clean_lnode_nolock(
			base_mapper, renter_shmvs,  &mrenter->resv_vm_info, urenter_lnode_datalen, mapper_shmvs,
			____coobj_radix1var_mrenter_get_lnode(mrenter, large_obj));
	}
	return err;
}

static inline
int ____coobj_radix1var_mrenter_map_exist(
	struct coobj_vspace_s *mapper_shmvs, struct ____coobj_radix1var_mholder_s *mholder,
	struct coobj_vspace_s *renter_shmvs, __bool large_obj, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	struct ____coobj_base_mapper_s *base_mapper = mholder->base_mapper;
	int err = ____coobj_radix1var_mrenter_map_exist_nolock(
		base_mapper, mapper_shmvs, mholder, renter_shmvs, large_obj, ____COOBJ_RADIX1VAR_TEPAR_ARG);
	return err;
}

#endif
