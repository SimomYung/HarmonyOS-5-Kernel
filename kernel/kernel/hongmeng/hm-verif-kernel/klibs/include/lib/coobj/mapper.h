/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: A Multi-data-structure generic co-object and its container
 * Author: Huawei OS Kernel Lab
 * Create: Mon April 11 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_MAPPER_H
#define KLIBS_COOBJ_MAPPER_H

#include <hmasm/types.h>
#include <hmkernel/errno.h>
#include <lib/dlist.h>
#include <sec/ctx.h>
#include <libsysif/sysmgr/api.h>

#include <lib/coobj/coobj.h>

struct coobj_config_type_s {
	char name[COOBJ_OBJ_NAME_MAX];
	struct coobj_mator_op_s mator_op;
	coobj_ator_kind_t ator_kind;

	coobj_vspaceid_t holder_vspaceid;
	coobj_vspaceid_t *renter_vspaceid_list;
	__uptr_t renter_vspaceid_size;
	struct sec_natv_obj_ctx sec_ctx;
	__u8 mapped;
	unsigned int flags;
};

struct coobj_config_s {
	struct coobj_config_type_s *type_list;
	coobj_typeid_t type_size;
};

struct coobj_root_vspace_s {
	coobj_vspaceid_t vspaceid;

	__uptr_t mator_size;
	struct ____coobj_mator_s *mator_list;

	/* shmid and mva could be allocated before the vspace's registration */
	____coobj_shmid_t uator_list_mshmid;
	struct ____coobj_uator_list_s *uator_list_mva;

	struct coobj_vspace_s *uator_shmvs;
	____coobj_shmid_t uator_list_ushmid;
	__uptr_t uator_list_uva;
};

struct coobj_root_s {
	struct coobj_config_s *config;
	struct ____coobj_base_param_s base_param;
	struct ____coobj_base_mapper_s *base_mapper;

	struct coobj_vspace_s *mapper_shmvs;

	struct coobj_root_vspace_s *vspace_list;
	coobj_vspaceid_t vspace_size;
};

static inline
int ____coobj_root_init_create_vspace(
	struct coobj_root_s *root,
	struct coobj_root_vspace_s *vspace,
	coobj_vspaceid_t vspaceid)
{
	int err = E_HM_OK;
	if (vspace->vspaceid != COOBJ_VSPACEID_INVALID) {
		err = E_HM_EXIST;
	}

	struct ____coobj_mator_s *mator_list = NULL;
	____coobj_shmid_t uator_list_mshmid = ____COOBJ_INVALID_SHMID;
	__uptr_t uator_list_mva = 0;

	if (err == E_HM_OK) {
		/* allocate mator list for managing the allocators in mapper */
		__uptr_t mator_list_size = sizeof(struct ____coobj_mator_s) * vspace->mator_size;
		mator_list = (struct ____coobj_mator_s *)____coobj_base_mapper_alloc(root->base_mapper, mator_list_size);
		if (mator_list != NULL) {
			/* mmap the uator list for being shared to the user */
			__uptr_t uator_datalen = ____coobj_uator_list_size(vspace->mator_size);

			err = ____coobj_base_mapper_map_notaddr(
				root->base_mapper, root->mapper_shmvs, uator_datalen, COOBJ_MAPPER_PERM_RW,
				&uator_list_mshmid, &uator_list_mva, __true);
			if (err != E_HM_OK) {
				____coobj_base_mapper_free(root->base_mapper, mator_list);
			}
		}
	}
	if (err == E_HM_OK) {
		/* Initialize all the fields in the root_vspace */
		vspace->vspaceid = vspaceid;
		vspace->mator_list = mator_list;

		vspace->uator_list_mshmid = uator_list_mshmid;
		vspace->uator_list_mva = (struct ____coobj_uator_list_s *)uator_list_mva;
		vspace->uator_list_mva->size = vspace->mator_size;

		vspace->uator_shmvs = NULL;
		vspace->uator_list_ushmid = ____COOBJ_INVALID_SHMID;
		vspace->uator_list_uva = 0;
	}
	return err;
}

static inline
int ____coobj_root_init_create_vspace_list(
	struct coobj_root_s *root)
{
	int err = E_HM_OK;
	/* Find all referenced vspace in the root's configuration
	 * Hard coding: All processes' {m/u}ator_size are initialized as predefined MAX num
	 */
	for (coobj_typeid_t coobj_typeid = 0; (coobj_typeid < root->config->type_size) && (err == E_HM_OK); coobj_typeid++) {
		struct coobj_config_type_s *type = &root->config->type_list[coobj_typeid];
		sec_obj_ctx_init(&type->sec_ctx);
		if (type->holder_vspaceid < root->vspace_size) {
			root->vspace_list[type->holder_vspaceid].mator_size++;
		} else {
			err = E_HM_INVAL;
		}
		for (__uptr_t renter_index = 0; renter_index < type->renter_vspaceid_size; renter_index++) {
			coobj_vspaceid_t renter_vspaceid = type->renter_vspaceid_list[renter_index];
			if (renter_vspaceid < root->vspace_size) {
				root->vspace_list[renter_vspaceid].mator_size++;
			} else {
				err = E_HM_INVAL;
			}
		}
	}
	if (err == E_HM_OK) {
		/* Initialize all referenced vspace */
		for (coobj_vspaceid_t vspaceid = 0; (vspaceid < root->vspace_size) && (err == E_HM_OK); vspaceid++) {
			struct coobj_root_vspace_s *vspace = &root->vspace_list[vspaceid];
			if (vspace->mator_size != 0) {
				err = ____coobj_root_init_create_vspace(root, vspace, vspaceid);
			}
		}
	}
	return err;
}

static inline
void ____coobj_root_init_mator_list(struct coobj_root_s *root)
{
	/* We use uator_list_mva->size as a per vspace counter, so that we should clear it first */
	for (coobj_vspaceid_t vspaceid = 0; vspaceid < root->vspace_size; vspaceid++) {
		struct coobj_root_vspace_s *vspace = &root->vspace_list[vspaceid];
		if (vspace->uator_list_mva != NULL) {
			vspace->uator_list_mva->size = 0;
		}
	}

	for (coobj_typeid_t coobj_typeid = 0; coobj_typeid < root->config->type_size; coobj_typeid++) {
		struct coobj_config_type_s *type = &root->config->type_list[coobj_typeid];

		struct ____coobj_mator_s *mholder = NULL;
		{
			struct ____coobj_ator_info_s ator_info = {
				.coobj_typeid = coobj_typeid,
				.role_kind = COOBJ_ROLE_HOLDER,
				.ator_kind = type->ator_kind,
			};

			coobj_vspaceid_t holder_vspaceid = type->holder_vspaceid;
			struct coobj_root_vspace_s *holder_vspace = &root->vspace_list[holder_vspaceid];
			__uptr_t holder_ator_index = holder_vspace->uator_list_mva->size++;

			mholder = &holder_vspace->mator_list[holder_ator_index];
			struct ____coobj_uator_s *uholder_mva = &holder_vspace->uator_list_mva->data[holder_ator_index];

			mholder->ator_info = ator_info;
			uholder_mva->ator_info = ator_info;

			type->mator_op.mator_mholder_init(mholder, root->base_mapper, uholder_mva);
		}

		for (__uptr_t renter_index = 0; renter_index < type->renter_vspaceid_size; renter_index++) {
			struct ____coobj_ator_info_s ator_info = {
				.coobj_typeid = coobj_typeid,
				.role_kind = COOBJ_ROLE_RENTER,
				.ator_kind = type->ator_kind,
			};

			coobj_vspaceid_t renter_vspaceid = type->renter_vspaceid_list[renter_index];
			struct coobj_root_vspace_s *renter_vspace = &root->vspace_list[renter_vspaceid];
			__uptr_t renter_ator_index = renter_vspace->uator_list_mva->size++;

			struct ____coobj_mator_s *mrenter = &renter_vspace->mator_list[renter_ator_index];
			struct ____coobj_uator_s *urenter_mva = &renter_vspace->uator_list_mva->data[renter_ator_index];

			mrenter->ator_info = ator_info;
			urenter_mva->ator_info = ator_info;

			type->mator_op.mator_mrenter_init(mrenter, mholder, urenter_mva);
		}
	}
}

static inline
int coobj_root_init(
	struct coobj_root_s *root,
	struct coobj_config_s *config,
	struct ____coobj_base_param_s base_param,
	struct ____coobj_base_mapper_s *base_mapper,
	struct coobj_vspace_s *mapper_shmvs,
	struct coobj_root_vspace_s *vspace_list,
	coobj_vspaceid_t vspace_size)
{
	root->config = config;
	root->base_param = base_param;
	root->base_mapper = base_mapper;
	root->mapper_shmvs = mapper_shmvs;
	root->vspace_list = vspace_list;
	root->vspace_size = vspace_size;

	for (coobj_vspaceid_t vspaceid = 0; vspaceid < vspace_size; vspaceid++) {
		struct coobj_root_vspace_s *vspace = &vspace_list[vspaceid];
		vspace->vspaceid = COOBJ_VSPACEID_INVALID;
		vspace->mator_size = 0;
		vspace->mator_list = NULL;
		vspace->uator_list_mshmid = ____COOBJ_INVALID_SHMID;
		vspace->uator_list_mva = NULL;
		vspace->uator_shmvs = NULL;
		vspace->uator_list_ushmid = ____COOBJ_INVALID_SHMID;
		vspace->uator_list_uva = 0;
	}

	____coobj_base_mapper_sync_init(root->base_mapper);
	int err = ____coobj_root_init_create_vspace_list(root);
	if (err == E_HM_OK) {
		err = ____coobj_base_mapper_init(root->base_mapper, root->base_param.page_order);
	}
	if (err == E_HM_OK) {
		____coobj_root_init_mator_list(root);
	}
	return err;
}

static inline
int ____coobj_root_vspace_init_type_nolock(
	struct coobj_root_s *root, struct coobj_root_vspace_s *vspace,
	struct coobj_vspace_s *uator_shmvs, coobj_typeid_t uator_typeid, unsigned int flags)
{
	int err =  E_HM_OK;
	__uptr_t mator_index;
	struct ____coobj_mator_s *mator = NULL;

	for (mator_index = 0; mator_index < vspace->mator_size; mator_index++) {
		mator = &vspace->mator_list[mator_index];
		if (mator->ator_info.coobj_typeid == uator_typeid) {
			break;
		}
	}
	if (mator_index == vspace->mator_size) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		struct coobj_config_type_s *type = &root->config->type_list[uator_typeid];

		if (mator->ator_info.role_kind == COOBJ_ROLE_HOLDER) {
			type->flags |= flags;
			type->mator_op.mator_mholder_init_shmvs(mator, uator_shmvs);
		} else if (mator->ator_info.role_kind == COOBJ_ROLE_RENTER) {
			type->mator_op.mator_mrenter_init_shmvs(mator, uator_shmvs);
		} else {
			err = E_HM_INVAL;
		}
	}
	return err;
}

static inline
int ____coobj_root_vspace_register(struct coobj_root_s *root, struct coobj_root_vspace_s *vspace,
				struct coobj_vspace_s *uator_shmvs, __uptr_t *p_uator_list_uva)
{
	int err =  E_HM_OK;

	____coobj_shmid_t uator_list_ushmid = ____COOBJ_INVALID_SHMID;
	__uptr_t uator_list_uva = 0;
	__uptr_t uator_datalen = ____coobj_uator_list_size(vspace->mator_size);
	err = ____coobj_base_mapper_map_notaddr_exist(
		root->base_mapper, uator_shmvs, uator_datalen, COOBJ_MAPPER_PERM_RW,
		root->mapper_shmvs, vspace->uator_list_mshmid,
		&uator_list_ushmid, &uator_list_uva, __true);
	if (err == E_HM_OK) {
		vspace->uator_shmvs = uator_shmvs;
		vspace->uator_list_ushmid = uator_list_ushmid;
		vspace->uator_list_uva = uator_list_uva;
		*p_uator_list_uva = uator_list_uva;
	}
	return err;
}

static inline
int coobj_root_vspace_register(
	struct coobj_root_s *root,
	__sptr_t vspaceid,
	struct coobj_vspace_s *uator_shmvs,
	__uptr_t *p_uator_list_uva)
{
	int err = vspaceid < root->vspace_size ? E_HM_OK : E_HM_INVAL;
	if (err == E_HM_OK) {
		err = ____coobj_base_mapper_init_zero(root->base_mapper, uator_shmvs);
	}
	if (err == E_HM_OK) {
		struct coobj_root_vspace_s *vspace = &root->vspace_list[vspaceid];
		err = ____coobj_root_vspace_register(root, vspace, uator_shmvs, p_uator_list_uva);
	}
	return err;
}

static inline
int ____coobj_root_reserve_vm(
	struct coobj_root_s *root, struct coobj_root_vspace_s *vspace, int coobj_role,
	coobj_typeid_t coobj_typeid, uintptr_t resv_va_start)
{
	int err =  E_HM_OK;
	struct ____coobj_mator_s *mator = NULL;
	for (__uptr_t mator_index = 0; mator_index < vspace->mator_size; mator_index++) {
		struct ____coobj_mator_s *tmp = &vspace->mator_list[mator_index];
		if (tmp->ator_info.coobj_typeid == coobj_typeid) {
			mator = tmp;
			break;
		}
	}
	struct coobj_config_type_s *type = &root->config->type_list[coobj_typeid];

	if (coobj_role == COOBJ_ROLE_HOLDER) {
		type->mator_op.mator_mholder_init_resv_vm(mator, resv_va_start);
	} else if (coobj_role == COOBJ_ROLE_RENTER) {
		type->mator_op.mator_mrenter_init_resv_vm(mator, resv_va_start);
	} else {
		err = E_HM_INVAL;
	}

	return err;
}

static inline
int coobj_root_reserve_vm(
	struct coobj_root_s *root, __sptr_t vspaceid, int coobj_role,
	coobj_typeid_t coobj_typeid, __uptr_t resv_va_start)
{
	int err = vspaceid < root->vspace_size ? E_HM_OK : E_HM_INVAL;

	if (err == E_HM_OK) {
		struct coobj_root_vspace_s *vspace = &root->vspace_list[vspaceid];
		err = ____coobj_root_reserve_vm(root, vspace, coobj_role, coobj_typeid, resv_va_start);
	}
	return err;
}

static inline
int ____coobj_root_vspace_unregister(
	struct coobj_root_s *root,
	struct coobj_root_vspace_s *vspace)
{
	int err = E_HM_OK;

	for (__uptr_t mator_index = 0; (mator_index < vspace->mator_size) && (err == E_HM_OK); mator_index++) {
		struct ____coobj_mator_s *mator = &vspace->mator_list[mator_index];
		struct coobj_config_type_s *type = &root->config->type_list[mator->ator_info.coobj_typeid];

		if (mator->ator_info.role_kind == COOBJ_ROLE_HOLDER) {
			type->mator_op.mator_mholder_fini_shmvs(root->mapper_shmvs, mator);
		} else if (mator->ator_info.role_kind == COOBJ_ROLE_RENTER) {
			type->mator_op.mator_mrenter_fini_shmvs(root->mapper_shmvs, mator, &type->mapped);
		} else {
			err = E_HM_INVAL;
			break;
		}
	}
	return err;
}

static inline
int coobj_root_vspace_unregister(
	struct coobj_root_s *root,
	__sptr_t vspaceid)
{
	int err = vspaceid < root->vspace_size ? E_HM_OK : E_HM_INVAL;
	if (err == E_HM_OK) {
		struct coobj_root_vspace_s *vspace = &root->vspace_list[vspaceid];
		err = ____coobj_root_vspace_unregister(root, vspace);
	}
	return err;
}

static inline
int coobj_root_vspace_map(
	struct coobj_root_s *root, __sptr_t vspaceid,
	struct ____coobj_chunkid_s chunkid, __relpos_t size, __bool large_obj)
{
	int err = vspaceid < root->vspace_size ? E_HM_OK : E_HM_INVAL;
	if (err == E_HM_OK) {
		struct coobj_root_vspace_s *holder_root_vspace = &root->vspace_list[vspaceid];
		__uptr_t ator_index = ____coobj_mator_list_find_ator(holder_root_vspace->mator_list,
			holder_root_vspace->mator_size, chunkid.coobj_typeid);

		struct ____coobj_mator_s *mholder = &holder_root_vspace->mator_list[ator_index];
		struct coobj_config_type_s *type = &root->config->type_list[chunkid.coobj_typeid];

		err = type->mator_op.mator_map(root->mapper_shmvs, mholder, holder_root_vspace->uator_shmvs,
					       chunkid, &type->mapped, size, large_obj, type->flags);
	}
	return err;
}

static inline
int ____coobj_root_vspace_reinit(
	struct coobj_root_s *root, struct coobj_root_vspace_s *vspace, __sptr_t vspaceid)
{
	int err = E_HM_INVAL;
	struct ____coobj_ator_info_s ator_info[COOBJ_TYPE_MAX];
	if (vspace->mator_list != NULL) {
		for (__uptr_t mator_index = 0; (mator_index < vspace->mator_size); mator_index++) {
			ator_info[mator_index] = vspace->mator_list[mator_index].ator_info;
		}
		__uptr_t uator_datalen = ____coobj_uator_list_size(vspace->mator_size);
		____coobj_base_mapper_unmap_fixaddr(
			root->base_mapper, root->mapper_shmvs, uator_datalen,
			vspace->uator_list_mshmid, NULL, (__uptr_t)vspace->uator_list_mva, __true);
		____coobj_base_mapper_free(root->base_mapper, vspace->mator_list);
	}
	vspace->vspaceid = COOBJ_VSPACEID_INVALID;
	vspace->uator_list_mshmid = ____COOBJ_INVALID_SHMID;
	vspace->uator_list_mva = NULL;
	vspace->uator_shmvs = NULL;
	vspace->uator_list_ushmid = ____COOBJ_INVALID_SHMID;
	vspace->uator_list_uva = 0;

	err = ____coobj_root_init_create_vspace(root, vspace, vspaceid);
	if (err == E_HM_OK && vspace->uator_list_mva != NULL) {
		vspace->uator_list_mva->size = vspace->mator_size;
		for (__uptr_t mator_index = 0; (mator_index < vspace->mator_size); mator_index++) {
			struct ____coobj_mator_s *mrenter = &vspace->mator_list[mator_index];
			struct ____coobj_uator_s *urenter_mva = &vspace->uator_list_mva->data[mator_index];
			mrenter->ator_info = ator_info[mator_index];
			urenter_mva->ator_info = ator_info[mator_index];

			struct ____coobj_mator_s *mator = &vspace->mator_list[mator_index];
			struct coobj_config_type_s *type = &root->config->type_list[mator->ator_info.coobj_typeid];
			coobj_vspaceid_t holder_vspaceid = type->holder_vspaceid;
			struct coobj_root_vspace_s *holder_vspace = &root->vspace_list[holder_vspaceid];
			struct ____coobj_mator_s *mholder = &holder_vspace->mator_list[mator_index];

			type->mator_op.mator_mrenter_init(mrenter, mholder, urenter_mva);
		}
	}
	return err;
}

static inline
int coobj_root_vspace_reinit(
	struct coobj_root_s *root, __sptr_t vspaceid)
{
	int err = vspaceid < root->vspace_size ? E_HM_OK : E_HM_INVAL;
	if (err == E_HM_OK) {
		struct coobj_root_vspace_s *vspace = &root->vspace_list[vspaceid];
		err = ____coobj_root_vspace_reinit(root, vspace, vspaceid);
	}
	return err;
}

static inline
int coobj_root_vspace_unmap(
	struct coobj_root_s *root, __sptr_t vspaceid,
	struct ____coobj_chunkid_s chunkid, __bool large_obj)
{
	int err = vspaceid < root->vspace_size ? E_HM_OK : E_HM_INVAL;
	if (err == E_HM_OK) {
		struct coobj_root_vspace_s *holder_root_vspace = &root->vspace_list[vspaceid];
		__uptr_t ator_index = ____coobj_mator_list_find_ator(
			holder_root_vspace->mator_list, holder_root_vspace->mator_size, chunkid.coobj_typeid);

		struct ____coobj_mator_s *mholder = &holder_root_vspace->mator_list[ator_index];
		struct coobj_config_type_s *type = &root->config->type_list[chunkid.coobj_typeid];

		err = type->mator_op.mator_unmap(
			root->mapper_shmvs, mholder, holder_root_vspace->uator_shmvs, chunkid, large_obj);
	}
	return err;
}

/* Map given type's l/d nodes for a renter that registers after holder's allocation. */
static inline
int coobj_root_renter_map_exist(
	struct coobj_root_s *root, __sptr_t vspaceid, coobj_typeid_t coobj_typeid, __u8 *mapped)
{
	int err = E_HM_OK;
	struct coobj_config_type_s *type = &root->config->type_list[coobj_typeid];
	*mapped = type->mapped;
	if (type->mapped == NONE_OBJ_ATOR_MAPPED) {
		return err;
	}
	struct coobj_root_vspace_s *renter_root_vspace = &root->vspace_list[vspaceid];
	struct coobj_vspace_s *renter_shmvs = renter_root_vspace->uator_shmvs;
	__sptr_t holder_vspaceid = type->holder_vspaceid;
	struct coobj_root_vspace_s *holder_root_vspace = &root->vspace_list[holder_vspaceid];
	__uptr_t ator_index = ____coobj_mator_list_find_ator(holder_root_vspace->mator_list,
		holder_root_vspace->mator_size, coobj_typeid);
	struct ____coobj_mator_s *mholder = &holder_root_vspace->mator_list[ator_index];
	if (((*mapped) & SMALL_OBJ_ATOR_MAPPED) == SMALL_OBJ_ATOR_MAPPED) {
		err = type->mator_op.mator_map_exist(root->mapper_shmvs, mholder, renter_shmvs, __false);
	}
	if (err == E_HM_OK && ((*mapped) & LARGE_OBJ_ATOR_MAPPED) == LARGE_OBJ_ATOR_MAPPED) {
		err = type->mator_op.mator_map_exist(root->mapper_shmvs, mholder, renter_shmvs, __true);
	}
	return err;
}

static inline
int coobj_root_vspace_init_type(
	struct coobj_root_s *root, struct coobj_root_vspace_s *vspace, struct coobj_vspace_s *uator_shmvs,
	coobj_typeid_t coobj_typeid, int coobj_role, __sptr_t vspaceid, __u8 *mapped, unsigned int flags)
{
	____coobj_base_mapper_sync_lock(root->base_mapper);
	int err = ____coobj_root_vspace_init_type_nolock(root, vspace, uator_shmvs, coobj_typeid, flags);
	if (err == E_HM_OK && coobj_role == COOBJ_ROLE_RENTER) {
		err = coobj_root_renter_map_exist(root, vspaceid, coobj_typeid, mapped);
	}
	____coobj_base_mapper_sync_unlock(root->base_mapper);
	return err;
}

#endif
