/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: A Multi-data-structure generic co-object and its container
 * Author: Huawei OS Kernel Lab
 * Create: Mon April 11 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_ATOR_ARRAY_H
#define KLIBS_COOBJ_ATOR_ARRAY_H

#include <lib/coobj/ator_base.h>

#define ____coobj_array_urenter_acqst_size() 0x4

/**************************************************
 * uholder
 **************************************************/
static inline
__relpos_t ____coobj_array_uholder_alloc_nolock(
	struct ____coobj_array_uholder_s *uholder, ____COOBJ_ARRAY_TEPAR_DECL)
{
	__relpos_t relpos = ____COOBJ_INVALID_POS;
	if (____coobj_uholder_dnode_alloc_check_nolock(
		&uholder->dnode, tepar_chunk_order) ||
	    ____coobj_uholder_dnode_extend_nolock(
			&uholder->dnode, uholder->base_user, 0, ____COOBJ_ARRAY_TEPAR_ARG) == E_HM_OK) {
		relpos = ____coobj_uholder_dnode_alloc_fastpath_nolock(
			&uholder->dnode, tepar_type_size);
	}
	return relpos;
}

static inline
int ____coobj_array_uholder_free_nolock(
	struct ____coobj_array_uholder_s *uholder, __relpos_t relpos, ____COOBJ_ARRAY_TEPAR_DECL)
{
	int err = E_HM_NOENT;
	if (____coobj_uholder_dnode_free_check_nolock(
		relpos, tepar_chunk_order)) {
		____coobj_uholder_dnode_free_fastpath_nolock(
			&uholder->dnode, relpos, tepar_type_size, tepar_chunk_order);
		err = E_HM_OK;
	}
	return err;
}

static inline
int ____coobj_array_uholder_recycle_nolock(
	struct ____coobj_array_uholder_s *uholder, ____COOBJ_ARRAY_TEPAR_DECL)
{
	return ____coobj_uholder_dnode_shrink_nolock(
		&uholder->dnode, uholder->base_user, 0, ____COOBJ_ARRAY_TEPAR_ARG);
}

static inline
__relpos_t ____coobj_array_uholder_alloc(
	struct ____coobj_array_uholder_s *uholder, ____COOBJ_ARRAY_TEPAR_DECL)
{
	____coobj_base_user_lock(uholder->base_user);
	__relpos_t relpos = ____coobj_array_uholder_alloc_nolock(
		uholder, ____COOBJ_ARRAY_TEPAR_ARG);
	____coobj_base_user_unlock(uholder->base_user);
	return relpos;
}

static inline
int ____coobj_array_uholder_free(
	struct ____coobj_array_uholder_s *uholder, __relpos_t relpos, ____COOBJ_ARRAY_TEPAR_DECL)
{
	____coobj_base_user_lock(uholder->base_user);
	int err = ____coobj_array_uholder_free_nolock(
		uholder, relpos, ____COOBJ_ARRAY_TEPAR_ARG);
	____coobj_base_user_unlock(uholder->base_user);
	return err;
}

static inline
int ____coobj_array_uholder_recycle(
	struct ____coobj_array_uholder_s *uholder, ____COOBJ_ARRAY_TEPAR_DECL)
{
	____coobj_base_user_lock(uholder->base_user);
	int err = ____coobj_array_uholder_recycle_nolock(
		uholder, ____COOBJ_ARRAY_TEPAR_ARG);
	____coobj_base_user_unlock(uholder->base_user);
	return err;
}

/**************************************************
 * urenter
 **************************************************/
static inline
__bool ____coobj_array_urenter_dnode_acquire_ptr(
	struct ____coobj_array_urenter_s *urenter, __relpos_t relpos, ____COOBJ_ARRAY_TEPAR_DECL)
{
	return ____coobj_urenter_acquire_ptr(&urenter->node);
}

static inline
void ____coobj_array_urenter_dnode_release_ptr(
	struct ____coobj_array_urenter_s *urenter, __relpos_t relpos, ____COOBJ_ARRAY_TEPAR_DECL)
{
	struct ____coobj_chunkid_s chunkid = ____coobj_chunkid(tepar_typeid, 0x0, 0x0);
	return ____coobj_urenter_release_ptr(&urenter->node, urenter->base_user, chunkid);
}

static inline
__bool ____coobj_array_urenter_acquire_ptr(
	struct ____coobj_array_urenter_s *urenter, __relpos_t relpos, __bool *acqst, ____COOBJ_ARRAY_TEPAR_DECL)
{
	__bool acq = __false;
	if (____coobj_array_urenter_dnode_acquire_ptr(urenter, relpos, ____COOBJ_ARRAY_TEPAR_ARG)) {
		acq = __true;
	}
	acqst[0] = __true;
	return acq;
}

static inline
void ____coobj_array_urenter_release_ptr(
	struct ____coobj_array_urenter_s *urenter, __relpos_t relpos, __bool *acqst, ____COOBJ_ARRAY_TEPAR_DECL)
{
	if (acqst[0] == __true) {
		____coobj_array_urenter_dnode_release_ptr(urenter, relpos, ____COOBJ_ARRAY_TEPAR_ARG);
		acqst[0] = __false;
	}
}

/**************************************************
 * mholder
 **************************************************/
static inline
void ____coobj_array_mholder_init_resv_vm(
	struct ____coobj_array_mholder_s *mholder, __uptr_t resv_vm_start)
{
	mholder->resv_vm_info.resv_vm_start = resv_vm_start;
	mholder->resv_vm_info.resv_vm_cur = resv_vm_start;
	mholder->resv_vm_info.resv_vm_end = resv_vm_start + COOBJ_RESERVED_MEM_SIZE;
}
static inline __always_inline
int ____coobj_array_mholder_unmap_dnode_until_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_array_mholder_s *mholder,
	__uptr_t dnode_datalen,
	struct ____coobj_array_mrenter_s *mrenter_until,
	____COOBJ_ARRAY_TEPAR_DECL)
{
	for (struct ____coobj_array_mrenter_s *mrenter = mholder->head;
	     mrenter != mrenter_until;
	     mrenter = mrenter->next) {
		struct coobj_vspace_s *renter_shmvs = mrenter->renter_shmvs;
		struct ____coobj_mrenter_dnode_s *renter_dnode = &mrenter->dnode;

		____coobj_mrenter_unmap_dnode_nolock(
			base_mapper, renter_shmvs, NULL, dnode_datalen, renter_dnode);
	}
	{
		struct coobj_vspace_s *holder_shmvs = mholder->holder_shmvs;
		struct ____coobj_mholder_dnode_s *holder_dnode = &mholder->dnode;

		____coobj_mholder_unmap_dnode_nolock(
			base_mapper, holder_shmvs, dnode_datalen, NULL, holder_dnode, __false);
	}
	return E_HM_OK;
}

static inline __always_inline
int ____coobj_array_mholder_unmap_until_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_array_mholder_s *mholder,
	struct ____coobj_chunkid_s chunkid,
	struct ____coobj_array_mrenter_s *mrenter_until,
	____COOBJ_ARRAY_TEPAR_DECL)
{
	int err = E_HM_OK;
	switch (chunkid.level) {
	case 0x0:
		{
			__uptr_t dnode_datalen =
				____coobj_array_eval_dnode_size(____COOBJ_ARRAY_TEPAR_ARG);

			err = ____coobj_array_mholder_unmap_dnode_until_nolock(
				base_mapper, mapper_shmvs, mholder, dnode_datalen, mrenter_until,
				____COOBJ_ARRAY_TEPAR_ARG);
		}
		break;
	default:
		err = E_HM_INVAL;
		break;
	}
	return err;
}

static inline __always_inline
int ____coobj_array_mholder_map_dnode_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_array_mholder_s *mholder,
	__uptr_t dnode_datalen,
	____COOBJ_ARRAY_TEPAR_DECL)
{
	int err = E_HM_OK;

	struct coobj_vspace_s *holder_shmvs = mholder->holder_shmvs;
	if (err == E_HM_OK) {
		err = ____coobj_mholder_map_dnode_nolock(
			base_mapper, holder_shmvs, &mholder->resv_vm_info,
			dnode_datalen, &mholder->dnode, __false, 0);
	}

	____coobj_shmid_t dpage_hshmid = ____COOBJ_INVALID_SHMID;
	if (err == E_HM_OK) {
		dpage_hshmid = ____coobj_mholder_dnode_get_dpage_hshmid(&mholder->dnode);
	}

	for (struct ____coobj_array_mrenter_s *mrenter = mholder->head;
	     mrenter != NULL && err == E_HM_OK;
	     mrenter = mrenter->next) {
		struct coobj_vspace_s *renter_shmvs = mrenter->renter_shmvs;
		err = ____coobj_mrenter_map_dnode_nolock(
			base_mapper, renter_shmvs, &mholder->resv_vm_info,
			dnode_datalen, holder_shmvs, dpage_hshmid, &mrenter->dnode);
		if (err != E_HM_OK) {
			____coobj_array_mholder_unmap_dnode_until_nolock(
				base_mapper, mapper_shmvs, mholder, dnode_datalen, mrenter,
				____COOBJ_ARRAY_TEPAR_ARG);
		}
	}
	return err;
}

static inline __always_inline
int ____coobj_array_mholder_map_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_array_mholder_s *mholder,
	struct ____coobj_chunkid_s chunkid,
	____COOBJ_ARRAY_TEPAR_DECL)
{
	int err = E_HM_OK;
	switch (chunkid.level) {
	case 0x0:
		{
			__uptr_t dnode_datalen =
				____coobj_array_eval_dnode_size(____COOBJ_ARRAY_TEPAR_ARG);

			err = ____coobj_array_mholder_map_dnode_nolock(
				base_mapper, mapper_shmvs, mholder, dnode_datalen,
				____COOBJ_ARRAY_TEPAR_ARG);
		}
		break;
	default:
		err = E_HM_INVAL;
		break;
	}
	return err;
}

static inline
int ____coobj_array_mholder_unmap_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_array_mholder_s *mholder,
	struct ____coobj_chunkid_s chunkid,
	____COOBJ_ARRAY_TEPAR_DECL)
{
	return ____coobj_array_mholder_unmap_until_nolock(
		base_mapper, mapper_shmvs, mholder, chunkid, NULL, ____COOBJ_ARRAY_TEPAR_ARG);
}

static inline
int ____coobj_array_mholder_map(
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_array_mholder_s *mholder,
	struct ____coobj_chunkid_s chunkid, __u8 *mapped, __relpos_t size, __bool large_obj,
	____COOBJ_ARRAY_TEPAR_DECL)
{
	UNUSED(mapped, size, large_obj);
	struct ____coobj_base_mapper_s *base_mapper = mholder->base_mapper;

	____coobj_base_mapper_sync_lock(base_mapper);
	int err = ____coobj_array_mholder_map_nolock(
		base_mapper, mapper_shmvs, mholder, chunkid, ____COOBJ_ARRAY_TEPAR_ARG);
	____coobj_base_mapper_sync_unlock(base_mapper);
	return err;
}

static inline
int ____coobj_array_mholder_unmap(
	struct coobj_vspace_s *mapper_shmvs,
	struct ____coobj_array_mholder_s *mholder,
	struct ____coobj_chunkid_s chunkid, __bool large_obj,
	____COOBJ_ARRAY_TEPAR_DECL)
{
	UNUSED(large_obj);
	struct ____coobj_base_mapper_s *base_mapper = mholder->base_mapper;

	____coobj_base_mapper_sync_lock(base_mapper);
	int err = ____coobj_array_mholder_unmap_nolock(
		base_mapper, mapper_shmvs, mholder, chunkid, ____COOBJ_ARRAY_TEPAR_ARG);
	____coobj_base_mapper_sync_unlock(base_mapper);
	return err;
}

/**************************************************
 * mrenter
 **************************************************/
static inline
int coobj_array_mrenter_notify_noref(
	struct ____coobj_array_mrenter_s *mrenter,
	struct ____coobj_chunkid_s chunkid)
{
	return E_HM_OK;
}
static inline
void ____coobj_array_mrenter_init_resv_vm(
	struct ____coobj_array_mrenter_s *mrenter, __uptr_t resv_vm_start)
{
	mrenter->resv_vm_info.resv_vm_start = resv_vm_start;
	mrenter->resv_vm_info.resv_vm_cur = resv_vm_start;
	mrenter->resv_vm_info.resv_vm_end = resv_vm_start + COOBJ_RESERVED_MEM_SIZE;
}
#endif
