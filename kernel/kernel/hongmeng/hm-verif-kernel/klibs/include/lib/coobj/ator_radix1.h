/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: A Multi-data-structure generic co-object and its container
 * Author: Huawei OS Kernel Lab
 * Create: Mon April 11 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_ATOR_RADIX1_H
#define KLIBS_COOBJ_ATOR_RADIX1_H

#include <lib/coobj/ator_base.h>

#define ____coobj_radix1_urenter_acqst_size() 0x4

/**************************************************
 * uholder
 **************************************************/
static inline
__relpos_t ____coobj_radix1_uholder_alloc_nolock(
	struct ____coobj_radix1_uholder_s *uholder, ____COOBJ_RADIX1_TEPAR_DECL)
{
	__relpos_t relpos = ____COOBJ_INVALID_POS;
	if (____coobj_uholder_l1node_alloc_check_nolock(
		&uholder->lnode, tepar_chunk_order, tepar_level1_order) ||
	    ____coobj_uholder_l1node_extend_nolock(
			&uholder->lnode, uholder->base_user, 0, ____COOBJ_RADIX1_TEPAR_ARG) == E_HM_OK) {
		relpos = ____coobj_uholder_l1node_alloc_fastpath_nolock(
			&uholder->lnode, tepar_type_size, tepar_chunk_order, tepar_level1_order);
	}
	return relpos;
}

static inline
int ____coobj_radix1_uholder_free_nolock(
	struct ____coobj_radix1_uholder_s *uholder, __relpos_t relpos, ____COOBJ_RADIX1_TEPAR_DECL)
{
	int err = E_HM_NOENT;
	if (____coobj_uholder_l1node_free_check_nolock(
		&uholder->lnode, relpos, tepar_chunk_order, tepar_level1_order, __false)) {
		____coobj_uholder_l1node_free_fastpath_nolock(
			&uholder->lnode, relpos, tepar_type_size, tepar_chunk_order, tepar_level1_order);
		err = E_HM_OK;
	}
	return err;
}

static inline
int ____coobj_radix1_uholder_recycle_nolock(
	struct ____coobj_radix1_uholder_s *uholder, ____COOBJ_RADIX1_TEPAR_DECL)
{
	return ____coobj_uholder_l1node_shrink_nolock(
		&uholder->lnode, uholder->base_user, 0, ____COOBJ_RADIX1_TEPAR_ARG);
}

static inline
__relpos_t ____coobj_radix1_uholder_alloc(
	struct ____coobj_radix1_uholder_s *uholder, ____COOBJ_RADIX1_TEPAR_DECL)
{
	____coobj_base_user_lock(uholder->base_user);
	__relpos_t relpos = ____coobj_radix1_uholder_alloc_nolock(
		uholder, ____COOBJ_RADIX1_TEPAR_ARG);
	____coobj_base_user_unlock(uholder->base_user);
	return relpos;
}

static inline
int ____coobj_radix1_uholder_free(
	struct ____coobj_radix1_uholder_s *uholder, __relpos_t relpos, ____COOBJ_RADIX1_TEPAR_DECL)
{
	____coobj_base_user_lock(uholder->base_user);
	int err = ____coobj_radix1_uholder_free_nolock(
		uholder, relpos, ____COOBJ_RADIX1_TEPAR_ARG);
	____coobj_base_user_unlock(uholder->base_user);
	return err;
}

static inline
int ____coobj_radix1_uholder_recycle(
	struct ____coobj_radix1_uholder_s *uholder, ____COOBJ_RADIX1_TEPAR_DECL)
{
	____coobj_base_user_lock(uholder->base_user);
	int err = ____coobj_radix1_uholder_recycle_nolock(
		uholder, ____COOBJ_RADIX1_TEPAR_ARG);
	____coobj_base_user_unlock(uholder->base_user);
	return err;
}

/**************************************************
 * urenter
 **************************************************/
static inline
__bool ____coobj_radix1_urenter_dnode_acquire_ptr(
	struct ____coobj_radix1_urenter_s *urenter, __relpos_t relpos, ____COOBJ_RADIX1_TEPAR_DECL)
{
	__relpos_t level1_relpos = relpos >> tepar_chunk_order;
	return ____coobj_urenter_acquire_ptr(
		&urenter->node.child_node_list[level1_relpos]);
}

static inline
__bool ____coobj_radix1_urenter_l1node_acquire_ptr(
	struct ____coobj_radix1_urenter_s *urenter, __relpos_t relpos, ____COOBJ_RADIX1_TEPAR_DECL)
{
	return ____coobj_urenter_acquire_ptr(
		&urenter->node);
}

static inline
void ____coobj_radix1_urenter_dnode_release_ptr(
	struct ____coobj_radix1_urenter_s *urenter, __relpos_t relpos, ____COOBJ_RADIX1_TEPAR_DECL)
{
	__relpos_t level1_relpos = relpos >> tepar_chunk_order;
	struct ____coobj_chunkid_s chunkid = ____coobj_chunkid(
		tepar_typeid, 0x0, level1_relpos << tepar_chunk_order);
	return ____coobj_urenter_release_ptr(
		&urenter->node.child_node_list[level1_relpos], urenter->base_user, chunkid);
}

static inline
void ____coobj_radix1_urenter_l1node_release_ptr(
	struct ____coobj_radix1_urenter_s *urenter, __relpos_t relpos, ____COOBJ_RADIX1_TEPAR_DECL)
{
	struct ____coobj_chunkid_s chunkid = ____coobj_chunkid(
		tepar_typeid, 0x1, 0x0);
	return ____coobj_urenter_release_ptr(
		&urenter->node, urenter->base_user, chunkid);
}

static inline
__bool ____coobj_radix1_urenter_acquire_ptr(
	struct ____coobj_radix1_urenter_s *urenter, __relpos_t relpos, __bool *acqst, ____COOBJ_RADIX1_TEPAR_DECL)
{
	__bool acq = __false;
	acqst[0] = __true;
	if (____coobj_radix1_urenter_l1node_acquire_ptr(urenter, relpos, ____COOBJ_RADIX1_TEPAR_ARG)) {
		acqst[1] = __true;
		if (____coobj_radix1_urenter_dnode_acquire_ptr(urenter, relpos, ____COOBJ_RADIX1_TEPAR_ARG)) {
			acq = __true;
		}
	}
	return acq;
}

static inline
void ____coobj_radix1_urenter_release_ptr(
	struct ____coobj_radix1_urenter_s *urenter, __relpos_t relpos, __bool *acqst, ____COOBJ_RADIX1_TEPAR_DECL)
{
	if (acqst[1] == __true) {
		____coobj_radix1_urenter_dnode_release_ptr(urenter, relpos, ____COOBJ_RADIX1_TEPAR_ARG);
		acqst[1] = __false;
	}
	if (acqst[0] == __true) {
		____coobj_radix1_urenter_l1node_release_ptr(urenter, relpos, ____COOBJ_RADIX1_TEPAR_ARG);
		acqst[0] = __false;
	}
}

/**************************************************
 * mholder
 **************************************************/
static inline
void ____coobj_radix1_mholder_init_resv_vm(
	struct ____coobj_radix1_mholder_s *mholder, __uptr_t resv_vm_start)
{
	mholder->resv_vm_info.resv_vm_start = resv_vm_start;
	mholder->resv_vm_info.resv_vm_cur = resv_vm_start;
	mholder->resv_vm_info.resv_vm_end = resv_vm_start + COOBJ_RESERVED_MEM_SIZE;
}
static inline
int ____coobj_radix1_mholder_unmap_dnode_until_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1_mholder_s *mholder,
	__uptr_t dnode_index,
	__uptr_t dnode_datalen,
	struct ____coobj_radix1_mrenter_s *mrenter_until,
	____COOBJ_RADIX1_TEPAR_DECL)
{
	for (struct ____coobj_radix1_mrenter_s *mrenter = mholder->head;
	     mrenter != mrenter_until;
	     mrenter = mrenter->next) {
		struct coobj_vspace_s *renter_shmvs = mrenter->renter_shmvs;
		struct ____coobj_mrenter_dnode_s *renter_dnode =
			____coobj_mrenter_lnode_get_child_dnode(&mrenter->lnode, dnode_index);

		____coobj_mrenter_unmap_dnode_nolock(
			base_mapper, renter_shmvs, NULL, dnode_datalen, renter_dnode);
	}
	{
		struct coobj_vspace_s *holder_shmvs = mholder->holder_shmvs;
		struct ____coobj_mholder_dnode_s *holder_dnode =
			____coobj_mholder_lnode_get_child_dnode(&mholder->lnode, dnode_index);

		____coobj_mholder_unmap_dnode_nolock(
			base_mapper, holder_shmvs, dnode_datalen, NULL, holder_dnode, __false);
	}
	return E_HM_OK;
}

static inline __always_inline
int ____coobj_radix1_mholder_unmap_lnode_until_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1_mholder_s *mholder,
	__uptr_t uholder_lnode_datalen,
	__uptr_t urenter_lnode_datalen,
	struct ____coobj_radix1_mrenter_s *mrenter_until,
	____COOBJ_RADIX1_TEPAR_DECL)
{
	for (struct ____coobj_radix1_mrenter_s *mrenter = mholder->head;
	     mrenter != mrenter_until;
	     mrenter = mrenter->next) {
		struct coobj_vspace_s *renter_shmvs = mrenter->renter_shmvs;
		struct ____coobj_mrenter_lnode_s *renter_lnode = &mrenter->lnode;

		____coobj_mrenter_unmap_lnode_nolock(base_mapper, renter_shmvs,
			NULL, urenter_lnode_datalen, mapper_shmvs, renter_lnode);

		void *child_node_list = ____coobj_mrenter_lnode_get_child_node_list(renter_lnode);
		____coobj_base_mapper_free(base_mapper, child_node_list);
		____coobj_mrenter_lnode_set_child_node_list(renter_lnode, NULL);
	}
	{
		struct coobj_vspace_s *holder_shmvs = mholder->holder_shmvs;
		struct ____coobj_mholder_lnode_s *holder_lnode = &mholder->lnode;

		____coobj_mholder_unmap_lnode_nolock(
			base_mapper, holder_shmvs, NULL, uholder_lnode_datalen, mapper_shmvs, holder_lnode, __false);

		void *child_node_list = ____coobj_mholder_lnode_get_child_node_list(holder_lnode);
		____coobj_base_mapper_free(base_mapper, child_node_list);
		____coobj_mholder_lnode_set_child_node_list(holder_lnode, NULL);
	}
	return E_HM_OK;
}

static inline __always_inline
int ____coobj_radix1_mholder_unmap_until_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1_mholder_s *mholder,
	struct ____coobj_chunkid_s chunkid,
	struct ____coobj_radix1_mrenter_s *mrenter_until,
	____COOBJ_RADIX1_TEPAR_DECL)
{
	int err = E_HM_OK;
	switch (chunkid.level) {
	case 0x0:
		{
			__uptr_t dnode_index = ____coobj_radix1_eval_dnode_index(chunkid, ____COOBJ_RADIX1_TEPAR_ARG);
			__uptr_t dnode_datalen = ____coobj_radix1_eval_dnode_size(____COOBJ_RADIX1_TEPAR_ARG);

			err = ____coobj_radix1_mholder_unmap_dnode_until_nolock(
				base_mapper, mapper_shmvs, mholder, dnode_index, dnode_datalen, mrenter_until,
				____COOBJ_RADIX1_TEPAR_ARG);
		}
		break;
	case 0x1:
		{
			__uptr_t uholder_lnode_datalen =
				____coobj_radix1_eval_uholder_lnode_size(____COOBJ_RADIX1_TEPAR_ARG);
			__uptr_t urenter_lnode_datalen =
				____coobj_radix1_eval_urenter_lnode_size(____COOBJ_RADIX1_TEPAR_ARG);

			err = ____coobj_radix1_mholder_unmap_lnode_until_nolock(
				base_mapper, mapper_shmvs, mholder, uholder_lnode_datalen, urenter_lnode_datalen, mrenter_until,
				____COOBJ_RADIX1_TEPAR_ARG);
		}
		break;
	default:
		err = E_HM_INVAL;
		break;
	}
	return err;
}

static inline
int ____coobj_radix1_mholder_map_dnode_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1_mholder_s *mholder,
	__uptr_t dnode_index,
	__uptr_t dnode_datalen,
	____COOBJ_RADIX1_TEPAR_DECL)
{
	int err = E_HM_OK;

	struct coobj_vspace_s *holder_shmvs = mholder->holder_shmvs;
	struct ____coobj_mholder_dnode_s *mholder_dnode =
		____coobj_mholder_lnode_get_child_dnode(&mholder->lnode, dnode_index);
	if (err == E_HM_OK) {
		struct ____coobj_uholder_dnode_s *uholder_dnode =
			____coobj_mholder_lnode_get_child_lpage_dnode(&mholder->lnode, dnode_index);

		____coobj_mholder_dnode_init(mholder_dnode, uholder_dnode);

		err = ____coobj_mholder_map_dnode_nolock(
			base_mapper, holder_shmvs, &mholder->resv_vm_info,
			dnode_datalen, mholder_dnode, __false, 0);
	}

	____coobj_shmid_t dpage_hshmid = ____COOBJ_INVALID_SHMID;
	if (err == E_HM_OK) {
		dpage_hshmid = ____coobj_mholder_dnode_get_dpage_hshmid(mholder_dnode);
	}

	for (struct ____coobj_radix1_mrenter_s *mrenter = mholder->head;
	     mrenter != NULL && err == E_HM_OK; mrenter = mrenter->next) {
		struct coobj_vspace_s *renter_shmvs = mrenter->renter_shmvs;
		struct ____coobj_mrenter_dnode_s *mrenter_dnode =
			____coobj_mrenter_lnode_get_child_dnode(&mrenter->lnode, dnode_index);
		struct ____coobj_urenter_node_s *urenter_dnode =
			____coobj_mrenter_lnode_get_child_urenter_node(&mrenter->lnode, dnode_index);

		____coobj_mrenter_dnode_init(mrenter_dnode, urenter_dnode);

		err = ____coobj_mrenter_map_dnode_nolock(
			base_mapper, renter_shmvs, &mrenter->resv_vm_info,
			dnode_datalen, holder_shmvs, dpage_hshmid, mrenter_dnode);
		if (err != E_HM_OK) {
			____coobj_radix1_mholder_unmap_dnode_until_nolock(
				base_mapper, mapper_shmvs, mholder,
				dnode_index, dnode_datalen, mrenter,
				____COOBJ_RADIX1_TEPAR_ARG);
		}
	}
	return err;
}

static inline
int ____coobj_radix1_mholder_map_lnode_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1_mholder_s *mholder,
	__uptr_t uholder_lnode_datalen,
	__uptr_t urenter_lnode_datalen,
	__uptr_t mholder_lnode_datalen,
	__uptr_t mrenter_lnode_datalen,
	____COOBJ_RADIX1_TEPAR_DECL)
{
	int err = E_HM_OK;

	struct coobj_vspace_s *holder_shmvs = mholder->holder_shmvs;
	if (err == E_HM_OK) {
		void *holder_child_node_list = ____coobj_base_mapper_alloc(base_mapper, mholder_lnode_datalen);
		if (holder_child_node_list == NULL) {
			err = E_HM_NOMEM;
		} else {
			err = ____coobj_mholder_map_lnode_nolock(
				base_mapper, holder_shmvs, &mholder->resv_vm_info, uholder_lnode_datalen,
				mapper_shmvs, &mholder->lnode, __false);
			if (err == E_HM_OK) {
				____coobj_mholder_lnode_set_child_node_list(
					&mholder->lnode, holder_child_node_list);
			} else {
				____coobj_base_mapper_free(base_mapper, holder_child_node_list);
			}
		}
	}
	for (struct ____coobj_radix1_mrenter_s *mrenter = mholder->head;
	     mrenter != NULL && err == E_HM_OK;
	     mrenter = mrenter->next) {
		struct coobj_vspace_s *renter_shmvs = mrenter->renter_shmvs;

		void *renter_children_list = ____coobj_base_mapper_alloc(base_mapper, mrenter_lnode_datalen);
		if (renter_children_list == NULL) {
			err = E_HM_NOMEM;
		} else {
			err = ____coobj_mrenter_map_lnode_nolock(
				base_mapper, renter_shmvs, &mrenter->resv_vm_info,
				urenter_lnode_datalen, mapper_shmvs, &mrenter->lnode);
			if (err == E_HM_OK) {
				____coobj_mrenter_lnode_set_child_node_list(
					&mrenter->lnode, renter_children_list);
			} else {
				____coobj_base_mapper_free(base_mapper, renter_children_list);
			}
		}

		if (err != E_HM_OK) {
			____coobj_radix1_mholder_unmap_lnode_until_nolock(
				base_mapper, mapper_shmvs, mholder,
				uholder_lnode_datalen, urenter_lnode_datalen, mrenter,
				____COOBJ_RADIX1_TEPAR_ARG);
		}
	}
	return err;
}

static inline
int ____coobj_radix1_mholder_map_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1_mholder_s *mholder,
	struct ____coobj_chunkid_s chunkid,
	____COOBJ_RADIX1_TEPAR_DECL)
{
	int err = E_HM_OK;
	switch (chunkid.level) {
	case 0x0:
		{
			__uptr_t dnode_index = ____coobj_radix1_eval_dnode_index(chunkid, ____COOBJ_RADIX1_TEPAR_ARG);
			__uptr_t dnode_datalen = ____coobj_radix1_eval_dnode_size(____COOBJ_RADIX1_TEPAR_ARG);

			err = ____coobj_radix1_mholder_map_dnode_nolock(
				base_mapper, mapper_shmvs, mholder,
				dnode_index, dnode_datalen,
				____COOBJ_RADIX1_TEPAR_ARG);
		}
		break;
	case 0x1:
		{
			__uptr_t uholder_lnode_datalen =
				____coobj_radix1_eval_uholder_lnode_size(____COOBJ_RADIX1_TEPAR_ARG);
			__uptr_t urenter_lnode_datalen =
				____coobj_radix1_eval_urenter_lnode_size(____COOBJ_RADIX1_TEPAR_ARG);
			__uptr_t mholder_lnode_datalen =
				____coobj_radix1_eval_mholder_lnode_size(____COOBJ_RADIX1_TEPAR_ARG);
			__uptr_t mrenter_lnode_datalen =
				____coobj_radix1_eval_mrenter_lnode_size(____COOBJ_RADIX1_TEPAR_ARG);
			err = ____coobj_radix1_mholder_map_lnode_nolock(
				base_mapper, mapper_shmvs, mholder,
				uholder_lnode_datalen, urenter_lnode_datalen,
				mholder_lnode_datalen, mrenter_lnode_datalen,
				____COOBJ_RADIX1_TEPAR_ARG);
		}
		break;
	default:
		err = E_HM_INVAL;
		break;
	}
	return err;
}

static inline
int ____coobj_radix1_mholder_unmap_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1_mholder_s *mholder,
	struct ____coobj_chunkid_s chunkid,
	____COOBJ_RADIX1_TEPAR_DECL)
{
	return ____coobj_radix1_mholder_unmap_until_nolock(
		base_mapper, mapper_shmvs, mholder, chunkid, NULL, ____COOBJ_RADIX1_TEPAR_ARG);
}

static inline
int ____coobj_radix1_mholder_map(
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1_mholder_s *mholder,
	struct ____coobj_chunkid_s chunkid, __u8 *mapped, __relpos_t size, __bool large_obj,
	____COOBJ_RADIX1_TEPAR_DECL)
{
	UNUSED(mapped, size, large_obj);
	struct ____coobj_base_mapper_s *base_mapper = mholder->base_mapper;
	____coobj_base_mapper_sync_lock(base_mapper);
	int err = ____coobj_radix1_mholder_map_nolock(
		base_mapper, mapper_shmvs, mholder, chunkid, ____COOBJ_RADIX1_TEPAR_ARG);
	____coobj_base_mapper_sync_unlock(base_mapper);
	return err;
}

static inline
int ____coobj_radix1_mholder_unmap(
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_radix1_mholder_s *mholder,
	struct ____coobj_chunkid_s chunkid, __bool large_obj,
	____COOBJ_RADIX1_TEPAR_DECL)
{
	UNUSED(large_obj);
	struct ____coobj_base_mapper_s *base_mapper = mholder->base_mapper;
	____coobj_base_mapper_sync_lock(base_mapper);
	int err = ____coobj_radix1_mholder_unmap_nolock(
		base_mapper, mapper_shmvs, mholder, chunkid, ____COOBJ_RADIX1_TEPAR_ARG);
	____coobj_base_mapper_sync_unlock(base_mapper);
	return err;
}

/**************************************************
 * mrenter
 **************************************************/
static inline
void ____coobj_radix1_mrenter_init_resv_vm(
	struct ____coobj_radix1_mrenter_s *mrenter, __uptr_t resv_vm_start)
{
	mrenter->resv_vm_info.resv_vm_start = resv_vm_start;
	mrenter->resv_vm_info.resv_vm_cur = resv_vm_start;
	mrenter->resv_vm_info.resv_vm_end = resv_vm_start + COOBJ_RESERVED_MEM_SIZE;
}
static inline
int coobj_radix1_mrenter_notify_noref(
	struct ____coobj_radix1_mrenter_s *mrenter,
	struct ____coobj_chunkid_s chunkid)
{
	return E_HM_OK;
}

#endif
