/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Data structures for array allocator
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 16 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_NODE_ARRAY_H
#define KLIBS_COOBJ_NODE_ARRAY_H

#include <lib/coobj/node_base.h>

static inline
____coobj_shmid_t ____coobj_mholder_dnode_get_dpage_hshmid(
	struct ____coobj_mholder_dnode_s *dnode)
{
	return dnode->dpage_hshmid;
}
static inline
__relpos_t ____coobj_mholder_dnode_get_dpage_size(
	struct ____coobj_mholder_dnode_s *dnode)
{
	return dnode->dpage_size;
}
static inline
void ____coobj_mholder_dnode_set_dpage_size(
	struct ____coobj_mholder_dnode_s *dnode, __relpos_t size)
{
	dnode->dpage_size = size;
}
static inline
__uptr_t ____coobj_mholder_dnode_get_dpage_hva(
	struct ____coobj_mholder_dnode_s *dnode)
{
	return dnode->dpage_hva;
}

static inline
____coobj_shmid_t ____coobj_mrenter_dnode_get_dpage_rshmid(
	struct ____coobj_mrenter_dnode_s *dnode)
{
	return dnode->dpage_rshmid;
}
static inline
struct ____coobj_mrenter_shmst_s ____coobj_mrenter_dnode_get_dpage_rshmst(
	struct ____coobj_mrenter_dnode_s *dnode)
{
	return dnode->dpage_rinfo.page_shmst;
}
static inline
__uptr_t ____coobj_mrenter_dnode_get_dpage_rva(
	struct ____coobj_mrenter_dnode_s *dnode)
{
	return dnode->dpage_rinfo.page_uva;
}

/***********************
 * uholder for dnode
 ***********************/

static inline
__bool ____coobj_uholder_dnode_alloc_check_nolock(
	struct ____coobj_uholder_dnode_s *dnode,
	__relpos_t tepar_chunk_order)
{
	return ____coobj_relpos_check(dnode->atords.free_list.head, tepar_chunk_order);
}

static inline
__relpos_t ____coobj_uholder_dnode_alloc_fastpath_nolock(
	struct ____coobj_uholder_dnode_s *dnode,
	__relpos_t tepar_type_size)
{
	__relpos_t relpos = ____coobj_slist_pop(&dnode->atords.free_list, dnode->page_uva, tepar_type_size);
	return relpos;
}

static inline
__bool ____coobj_uholder_dnode_free_check_nolock(
	__relpos_t relpos,
	__relpos_t tepar_chunk_order)
{
	return ____coobj_relpos_check(relpos, tepar_chunk_order);
}

static inline
void ____coobj_uholder_dnode_free_fastpath_nolock(
	struct ____coobj_uholder_dnode_s *dnode,
	__relpos_t relpos,
	__relpos_t tepar_type_size,
	__relpos_t tepar_chunk_order)
{
	____coobj_slist_add(&dnode->atords.free_list, dnode->page_uva, tepar_type_size, relpos);
}

static inline
void ____coobj_uholder_dnode_extend_page_nolock(
	struct ____coobj_uholder_dnode_s *dnode,
	__relpos_t tepar_type_size,
	__relpos_t tepar_chunk_order)
{
	____coobj_slist_init(&dnode->atords.free_list);
	____coobj_slist_init_link(&dnode->atords.free_list, dnode->page_uva, tepar_type_size, tepar_chunk_order);
}

static inline __always_inline
int ____coobj_uholder_dnode_extend_nolock(
	struct ____coobj_uholder_dnode_s *dnode,
	struct ____coobj_base_user_s *base_user,
	__relpos_t baseof_chunk,
	coobj_typeid_t tepar_typeid,
	__relpos_t tepar_type_size,
	__relpos_t tepar_chunk_order)
{
	int err = E_HM_OK;
	if (dnode->page_uva == NULL) {
		err = ____coobj_base_user_uholder_map(
			base_user, ____coobj_chunkid(tepar_typeid, 0x0, baseof_chunk), 0, __false);
		if (err == E_HM_OK) {
			____coobj_uholder_dnode_extend_page_nolock(dnode, tepar_type_size, tepar_chunk_order);
		}
	} else {
		err = E_HM_EXIST;
	}
	return E_HM_OK;
}

static inline __always_inline
int ____coobj_uholder_dnode_shrink_nolock(
	struct ____coobj_uholder_dnode_s *dnode,
	struct ____coobj_base_user_s *base_user,
	__relpos_t baseof_chunk,
	coobj_typeid_t tepar_typeid,
	__relpos_t tepar_type_size,
	__relpos_t tepar_chunk_order)
{
	int err = E_HM_OK;
	if (err == E_HM_OK) {
		if (dnode->atords.free_list.size != (__relpos_t)(1 << tepar_chunk_order)) {
			err = E_HM_NOENT;
		}
	}
	if (err == E_HM_OK) {
		err = ____coobj_base_user_uholder_unmap(
			base_user, ____coobj_chunkid(tepar_typeid, 0x0, baseof_chunk), __false);
	}
	if (err == E_HM_OK) {
		____coobj_slist_init(&dnode->atords.free_list);
	}
	return E_HM_OK;
}

/**************************************************
 * mapper utility
 **************************************************/

/***********************
 * mholder for chunk
 ***********************/
static inline
int ____coobj_mholder_map_dnode_nolock(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *holder_shmvs,
	struct ____coobj_resv_vm_info_s *resv_vm_info, __uptr_t datalen, struct ____coobj_mholder_dnode_s *holder_dnode,
	__bool var_ator, unsigned int flags)
{
	____coobj_shmid_t holder_dpage_hshmid = ____COOBJ_INVALID_SHMID;
	__uptr_t holder_dpage_huva = ____COOBJ_INVALID_PAGE;
	int err = E_HM_OK;
	if (resv_vm_info == NULL || resv_vm_info->resv_vm_cur == 0) {
		err = ____coobj_base_mapper_map_notaddr(
			base_mapper, holder_shmvs, datalen, ____coobj_flags_to_perm(flags),
			&holder_dpage_hshmid, &holder_dpage_huva, __true);
	} else {
		err = ____coobj_base_mapper_map_fixaddr(
			base_mapper, holder_shmvs, datalen, ____coobj_flags_to_perm(flags),
			&holder_dpage_hshmid, resv_vm_info, &holder_dpage_huva, __true);
	}
	if (err == E_HM_OK) {
		____coobj_mholder_dnode_set_dpage_hshmid(holder_dnode, holder_dpage_hshmid);
		____coobj_mholder_dnode_set_dpage_hva(holder_dnode, holder_dpage_huva, var_ator);
	}
	return err;
}

static inline
int ____coobj_mholder_unmap_dnode_nolock(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *holder_shmvs, __uptr_t datalen,
	struct ____coobj_resv_vm_info_s *resv_vm_info, struct ____coobj_mholder_dnode_s *holder_dnode, __bool var_ator)
{
	____coobj_shmid_t dpage_hshmid = ____coobj_mholder_dnode_get_dpage_hshmid(holder_dnode);
	__uptr_t dpage_hva = ____coobj_mholder_dnode_get_dpage_hva(holder_dnode);
	____coobj_base_mapper_unmap_fixaddr(
		base_mapper, holder_shmvs, datalen,
		dpage_hshmid, resv_vm_info, dpage_hva, __true);
	____coobj_mholder_dnode_set_dpage_hshmid(holder_dnode, ____COOBJ_INVALID_SHMID);
	____coobj_mholder_dnode_set_dpage_hva(holder_dnode, ____COOBJ_INVALID_PAGE, var_ator);
	return E_HM_OK;
}

/***********************
 * mrenter for chunk
 ***********************/
static inline
void ____coobj_mrenter_store_page_info_atomic(
	struct ____coobj_mrenter_tgt_page_info_s *page_info,
	struct ____coobj_urenter_node_s *urenter_node_mva,
	struct ____coobj_mrenter_shmst_s shmst)
{
	__atomic_store_n(&page_info->page_shmst.status, shmst.status, __ATOMIC_RELEASE);
	page_info->page_shmst.action = shmst.action;
	if (shmst.status == ____COOBJ_MRENTER_SHMST_BACKEND_UNMAP) {
		urenter_node_mva->page_uva = NULL;
		page_info->page_uva = ____COOBJ_INVALID_PAGE;
	} else {
		urenter_node_mva->page_uva = (void *)page_info->page_uva;
	}
}

static inline
__u16 ____coobj_mrenter_load_page_info_atomic(
	struct ____coobj_mrenter_tgt_page_info_s *page_info)
{
	return __atomic_load_n(&page_info->page_shmst.status, __ATOMIC_ACQUIRE);
}

struct ____coobj_mrenter_map_params_s {
	__u32 perm;
	__uptr_t datalen;
	____coobj_shmid_t owner_shmid;
	____coobj_shmid_t *renter_shmid;
	struct coobj_vspace_s *renter_shmvs;
	struct coobj_vspace_s *owner_shmvs;
	struct ____coobj_base_mapper_s *base_mapper;
	struct ____coobj_urenter_node_s *urenter_node_mva;
	struct ____coobj_mrenter_tgt_page_info_s *page_info;
	struct ____coobj_resv_vm_info_s *resv_vm_info;
};

static inline __always_inline
int ____coobj_mrenter_map_notaddr_or_fixaddr(struct ____coobj_mrenter_map_params_s *params)
{
	int err = E_HM_OK;
	__u16 map_status = ____coobj_mrenter_load_page_info_atomic(params->page_info);
	if (map_status == ____COOBJ_MRENTER_SHMST_BACKEND_UNMAP) {
		if (params->resv_vm_info == NULL || params->resv_vm_info->resv_vm_cur == 0) {
			err = ____coobj_base_mapper_map_notaddr_exist(
				params->base_mapper, params->renter_shmvs, params->datalen, params->perm,
				params->owner_shmvs, params->owner_shmid,
				params->renter_shmid, &params->page_info->page_uva, __false);
		} else {
			err = ____coobj_base_mapper_map_fixaddr_exist_update(
				params->base_mapper, params->renter_shmvs, params->datalen, params->perm,
				params->owner_shmvs, params->owner_shmid,
				params->renter_shmid, params->resv_vm_info, &params->page_info->page_uva, __false);
		}
		if (err == E_HM_OK) {
			____coobj_mrenter_store_page_info_atomic(
				params->page_info, params->urenter_node_mva, ____COOBJ_MRENTER_SHMST_STATUS_MAP_NONE);
		}
	} else if (map_status == ____COOBJ_MRENTER_SHMST_BACKEND_MAP) {
		/* BUG_ON(1, "memory leak!") */
	} else if (map_status == ____COOBJ_MRENTER_SHMST_BACKEND_ZERO) {
		err = ____coobj_base_mapper_map_fixaddr_exist(
			params->base_mapper, params->renter_shmvs, params->datalen, params->perm,
			params->owner_shmvs, params->owner_shmid,
			params->renter_shmid, params->page_info->page_uva, __false);
		if (err == E_HM_OK) {
			____coobj_mrenter_store_page_info_atomic(
				params->page_info, params->urenter_node_mva, ____COOBJ_MRENTER_SHMST_STATUS_MAP_NONE);
		}
	} else {
		/* BUG_ON(1, "unreachable!") */
	}
	return err;
}

static inline __always_inline
int ____coobj_mrenter_unmap_page_info(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *renter_shmvs,
	__uptr_t datalen,
	____coobj_shmid_t renter_shmid,
	struct ____coobj_urenter_node_s *urenter_node_mva,
	struct ____coobj_mrenter_tgt_page_info_s *page_info,
	struct ____coobj_resv_vm_info_s *resv_vm_info)
{
	int err = E_HM_OK;
	__uptr_t ptr_status = __atomic_or_fetch(&urenter_node_mva->status, 1, __ATOMIC_RELEASE);
	__u16 map_status = ____coobj_mrenter_load_page_info_atomic(page_info);
	if (ptr_status == 1) {
		if (map_status == ____COOBJ_MRENTER_SHMST_BACKEND_UNMAP) {
			/* BUG_ON(1, "double free!") */
		} else if (map_status == ____COOBJ_MRENTER_SHMST_BACKEND_MAP) {
			err = ____coobj_base_mapper_unmap_fixaddr(
				base_mapper, renter_shmvs, datalen,
				renter_shmid, resv_vm_info, page_info->page_uva, __false);
			if (err == E_HM_OK) {
				____coobj_mrenter_store_page_info_atomic(
					page_info, urenter_node_mva, ____COOBJ_MRENTER_SHMST_STATUS_UNMAP_NONE);
			}
		} else if (map_status == ____COOBJ_MRENTER_SHMST_BACKEND_ZERO) {
			err = ____coobj_base_mapper_unmap_zero(
				base_mapper, renter_shmvs, datalen,
				page_info->page_uva);
			if (err == E_HM_OK) {
				____coobj_mrenter_store_page_info_atomic(
					page_info, urenter_node_mva, ____COOBJ_MRENTER_SHMST_STATUS_UNMAP_NONE);
			}
		} else {
			/* BUG_ON(1, "unreachable!") */
		}
	} else {
		if (map_status == ____COOBJ_MRENTER_SHMST_BACKEND_UNMAP) {
			/* BUG_ON(1, "double free!") */
		} else if (map_status == ____COOBJ_MRENTER_SHMST_BACKEND_MAP) {
			err = ____coobj_base_mapper_map_zero(
				base_mapper, renter_shmvs, datalen,
				page_info->page_uva);
			if (err == E_HM_OK) {
				____coobj_mrenter_store_page_info_atomic(
					page_info, urenter_node_mva, ____COOBJ_MRENTER_SHMST_STATUS_ZERO_FREE);
			}
		} else if (map_status == ____COOBJ_MRENTER_SHMST_BACKEND_ZERO) {
			/* BUG_ON(1, "double free!") */
		} else {
			/* BUG_ON(1, "unreachable!") */
		}
	}
	__atomic_and_fetch(&urenter_node_mva->status, ~1, __ATOMIC_RELEASE);
	return err;
}

static inline __always_inline
int ____coobj_mrenter_map_page_info(struct ____coobj_mrenter_map_params_s *params)
{
	int err = E_HM_OK;
	__uptr_t ptr_status = __atomic_or_fetch(&params->urenter_node_mva->status, 1, __ATOMIC_RELEASE);
	/* For map, currently we don't distinguish between only mapping without or with accessing,
	 * because we always need to map.
	 */
	if (ptr_status == 1) {
		err = ____coobj_mrenter_map_notaddr_or_fixaddr(params);
	} else {
		err = ____coobj_mrenter_map_notaddr_or_fixaddr(params);
	}
	__atomic_and_fetch(&params->urenter_node_mva->status, ~1, __ATOMIC_RELEASE);
	return err;
}

static inline
int ____coobj_mrenter_map_dnode_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *renter_shmvs,
	struct ____coobj_resv_vm_info_s *resv_vm_info, __uptr_t datalen,
	struct coobj_vspace_s *holder_shmvs,
	____coobj_shmid_t dpage_hshmid,
	struct ____coobj_mrenter_dnode_s *renter_dnode)
{
	struct ____coobj_mrenter_map_params_s params = {
		.perm = COOBJ_MAPPER_PERM_RO,
		.datalen = datalen,
		.owner_shmid = dpage_hshmid,
		.renter_shmid = &renter_dnode->dpage_rshmid,
		.renter_shmvs = renter_shmvs,
		.owner_shmvs = holder_shmvs,
		.base_mapper = base_mapper,
		.urenter_node_mva = renter_dnode->urenter_node_mva,
		.page_info = &renter_dnode->dpage_rinfo,
		.resv_vm_info = resv_vm_info,
	};
	return ____coobj_mrenter_map_page_info(&params);
}

static inline
int ____coobj_mrenter_unmap_dnode_nolock(
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *renter_shmvs,
	struct ____coobj_resv_vm_info_s *resv_vm_info, __uptr_t datalen,
	struct ____coobj_mrenter_dnode_s *renter_dnode)
{
	____coobj_mrenter_unmap_page_info(
		base_mapper, renter_shmvs, datalen,
		renter_dnode->dpage_rshmid,
		renter_dnode->urenter_node_mva, &renter_dnode->dpage_rinfo, resv_vm_info);

	____coobj_mrenter_dnode_set_dpage_rshmid(renter_dnode, ____COOBJ_INVALID_SHMID);
	____coobj_mrenter_dnode_set_dpage_rshmst(renter_dnode, ____COOBJ_MRENTER_SHMST_STATUS_UNMAP_NONE);
	____coobj_mrenter_dnode_set_dpage_rva(renter_dnode, ____COOBJ_INVALID_PAGE);
	return E_HM_OK;
}

#endif
