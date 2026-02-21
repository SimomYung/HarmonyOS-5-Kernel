/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: A Multi-data-structure generic co-object and its container
 * Author: Huawei OS Kernel Lab
 * Create: Mon April 11 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_ATOR_H
#define KLIBS_COOBJ_ATOR_H

#include <lib/coobj/node.h>
#include <lib/coobj/ator_array.h>
#include <lib/coobj/ator_radix1.h>
#include <lib/coobj/ator_radix1var.h>

struct ____coobj_ator_info_s {
	coobj_typeid_t coobj_typeid:16;
	coobj_role_kind_t role_kind:8;
	coobj_ator_kind_t ator_kind:8;
};

struct ____coobj_mator_s {
	struct ____coobj_ator_info_s ator_info;
	union {
		struct ____coobj_array_mholder_s array_mholder;
		struct ____coobj_array_mrenter_s array_mrenter;
		struct ____coobj_radix1_mholder_s radix1_mholder;
		struct ____coobj_radix1_mrenter_s radix1_mrenter;
		struct ____coobj_radix1var_mholder_s radix1var_mholder;
		struct ____coobj_radix1var_mrenter_s radix1var_mrenter;
	};
};

struct ____coobj_uator_s {
	struct ____coobj_ator_info_s ator_info;
	union {
		struct ____coobj_array_uholder_s array_uholder;
		struct ____coobj_array_urenter_s array_urenter;
		struct ____coobj_radix1_uholder_s radix1_uholder;
		struct ____coobj_radix1_urenter_s radix1_urenter;
		struct ____coobj_radix1var_uholder_s radix1var_uholder;
		struct ____coobj_radix1var_urenter_s radix1var_urenter;
	};
};

/* The whole uator list shared to the user with RW permission, so that user
 * could modify the record necessary information. However, as mapper don't
 * trust user, the mapper has to backup the information in the mator's list.
 */
struct ____coobj_uator_list_s {
	__uptr_t size;
	struct ____coobj_uator_s data[0];
};

static inline
__uptr_t ____coobj_uator_list_size(__uptr_t ator_size)
{
	return sizeof(struct ____coobj_uator_list_s) + sizeof(struct ____coobj_uator_s) * ator_size;
}

static inline
__uptr_t ____coobj_mator_list_find_ator(
	struct ____coobj_mator_s *mator_list,
	__uptr_t mator_size,
	coobj_typeid_t coobj_typeid)
{
	__uptr_t ator = ____COOBJ_INVALID_ATOR;
	for (__uptr_t ator_index = 0; ator_index < mator_size; ator_index++) {
		if (mator_list[ator_index].ator_info.coobj_typeid == coobj_typeid) {
			ator = ator_index;
			break;
		}
	}
	return ator;
}

static inline
__uptr_t ____coobj_uator_list_find_ator(
	struct ____coobj_uator_list_s *uator_list,
	coobj_typeid_t coobj_typeid)
{
	__uptr_t ator = ____COOBJ_INVALID_ATOR;
	for (__uptr_t ator_index = 0; ator_index < uator_list->size; ator_index++) {
		if (uator_list->data[ator_index].ator_info.coobj_typeid == coobj_typeid) {
			ator = ator_index;
			break;
		}
	}
	return ator;
}

/*
 * Generic allocator
 */
static inline
void ____coobj_mator_array_mholder_init(
	struct ____coobj_mator_s *mholder,
	struct ____coobj_base_mapper_s *base_mapper,
	struct ____coobj_uator_s *uholder_mva)
{
	____coobj_array_mholder_init(
		&mholder->array_mholder, base_mapper, &uholder_mva->array_uholder);
}
static inline
void ____coobj_mator_array_mholder_init_shmvs(
	struct ____coobj_mator_s *mholder,
	struct coobj_vspace_s *holder_shmvs)
{
	____coobj_array_mholder_init_shmvs(
		&mholder->array_mholder, holder_shmvs);
}
static inline
void ____coobj_mator_array_mholder_fini_shmvs(
	struct coobj_vspace_s *mapper_shmvs, struct ____coobj_mator_s *mholder, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
}
static inline
void ____coobj_mator_array_mholder_init_resv_vm(
	struct ____coobj_mator_s *mholder, __uptr_t resv_vm_start)
{
	____coobj_array_mholder_init_resv_vm(
		&mholder->array_mholder, resv_vm_start);
}
static inline
void ____coobj_mator_array_mrenter_init(
	struct ____coobj_mator_s *mrenter,
	struct ____coobj_mator_s *mholder,
	struct ____coobj_uator_s *urenter_mva)
{
	____coobj_array_mrenter_init(
		&mrenter->array_mrenter, &mholder->array_mholder, &urenter_mva->array_urenter);
}
static inline
void ____coobj_mator_array_mrenter_init_shmvs(
	struct ____coobj_mator_s *mrenter,
	struct coobj_vspace_s *renter_shmvs)
{
	____coobj_array_mrenter_init_shmvs(
		&mrenter->array_mrenter, renter_shmvs);
}
static inline
void ____coobj_mator_array_mrenter_fini_shmvs(
	struct coobj_vspace_s *mapper_shmvs, struct ____coobj_mator_s *mrenter, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
}
static inline
void ____coobj_mator_array_mrenter_init_resv_vm(
	struct ____coobj_mator_s *mrenter, __uptr_t resv_vm_start)
{
	____coobj_array_mrenter_init_resv_vm(
		&mrenter->array_mrenter, resv_vm_start);
}
static inline
void ____coobj_mator_radix1_mholder_init(
	struct ____coobj_mator_s *mholder,
	struct ____coobj_base_mapper_s *base_mapper,
	struct ____coobj_uator_s *uholder_mva)
{
	____coobj_radix1_mholder_init(
		&mholder->radix1_mholder, base_mapper, &uholder_mva->radix1_uholder);
}
static inline
void ____coobj_mator_radix1_mholder_init_shmvs(
	struct ____coobj_mator_s *mholder,
	struct coobj_vspace_s *holder_shmvs)
{
	____coobj_radix1_mholder_init_shmvs(
		&mholder->radix1_mholder, holder_shmvs);
}
static inline
void ____coobj_mator_radix1_mholder_fini_shmvs(
	struct coobj_vspace_s *mapper_shmvs, struct ____coobj_mator_s *mholder, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
}
static inline
void ____coobj_mator_radix1_mholder_init_resv_vm(
	struct ____coobj_mator_s *mholder, __uptr_t resv_vm_start)
{
	____coobj_radix1_mholder_init_resv_vm(
		&mholder->radix1_mholder, resv_vm_start);
}
static inline
void ____coobj_mator_radix1_mrenter_init(
	struct ____coobj_mator_s *mrenter,
	struct ____coobj_mator_s *mholder,
	struct ____coobj_uator_s *urenter_mva)
{
	____coobj_radix1_mrenter_init(
		&mrenter->radix1_mrenter, &mholder->radix1_mholder, &urenter_mva->radix1_urenter);
}
static inline
void ____coobj_mator_radix1_mrenter_init_shmvs(
	struct ____coobj_mator_s *mrenter,
	struct coobj_vspace_s *renter_shmvs)
{
	____coobj_radix1_mrenter_init_shmvs(
		&mrenter->radix1_mrenter, renter_shmvs);
}
static inline
void ____coobj_mator_radix1_mrenter_fini_shmvs(
	struct coobj_vspace_s *mapper_shmvs, struct ____coobj_mator_s *mrenter, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
}
static inline
void ____coobj_mator_radix1_mrenter_init_resv_vm(
	struct ____coobj_mator_s *mrenter, __uptr_t resv_vm_start)
{
	____coobj_radix1_mrenter_init_resv_vm(
		&mrenter->radix1_mrenter, resv_vm_start);
}
static inline
void ____coobj_mator_radix1var_mholder_init(
	struct ____coobj_mator_s *mholder,
	struct ____coobj_base_mapper_s *base_mapper,
	struct ____coobj_uator_s *uholder_mva)
{
	____coobj_radix1var_mholder_init(
		&mholder->radix1var_mholder, base_mapper, &uholder_mva->radix1var_uholder);
}
static inline
void ____coobj_mator_radix1var_mholder_init_shmvs(
	struct ____coobj_mator_s *mholder,
	struct coobj_vspace_s *holder_shmvs)
{
	____coobj_radix1var_mholder_init_shmvs(
		&mholder->radix1var_mholder, holder_shmvs);
}
static inline
void ____coobj_mator_radix1var_mholder_fini_shmvs(
	struct coobj_vspace_s *mapper_shmvs, struct ____coobj_mator_s *mholder, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
}
static inline
void ____coobj_mator_radix1var_mholder_init_resv_vm(
	struct ____coobj_mator_s *mholder, __uptr_t resv_vm_start)
{
	____coobj_radix1var_mholder_init_resv_vm(
		&mholder->radix1var_mholder, resv_vm_start);
}

static inline
void ____coobj_mator_radix1var_mrenter_init(
	struct ____coobj_mator_s *mrenter,
	struct ____coobj_mator_s *mholder,
	struct ____coobj_uator_s *urenter_mva)
{
	____coobj_radix1var_mrenter_init(
		&mrenter->radix1var_mrenter, &mholder->radix1var_mholder, &urenter_mva->radix1var_urenter);
}
static inline
void ____coobj_mator_radix1var_mrenter_init_shmvs(
	struct ____coobj_mator_s *mrenter,
	struct coobj_vspace_s *renter_shmvs)
{
	____coobj_radix1var_mrenter_init_shmvs(
		&mrenter->radix1var_mrenter, renter_shmvs);
}
static inline
void ____coobj_mator_radix1var_mrenter_fini_shmvs(
	struct coobj_vspace_s *mapper_shmvs, struct ____coobj_mator_s *mrenter,
	__u8 *mapped, ____COOBJ_RADIX1VAR_TEPAR_DECL)
{
	____coobj_radix1var_mrenter_fini_shmvs(
		mapper_shmvs, &mrenter->radix1var_mrenter, mapped, ____COOBJ_RADIX1VAR_TEPAR_ARG);
}
static inline
void ____coobj_mator_radix1var_mrenter_init_resv_vm(
	struct ____coobj_mator_s *mrenter, __uptr_t resv_vm_start)
{
	____coobj_radix1var_mrenter_init_resv_vm(
		&mrenter->radix1var_mrenter, resv_vm_start);
}

struct coobj_mator_op_s {
	void (*mator_mholder_init)(
		struct ____coobj_mator_s *mholder,
		struct ____coobj_base_mapper_s *base_mapper,
		struct ____coobj_uator_s *uholder_mva);
	void (*mator_mholder_init_shmvs)(
		struct ____coobj_mator_s *mholder,
		struct coobj_vspace_s *shmvs);
	void (*mator_mholder_fini_shmvs)(
		struct coobj_vspace_s *mapper_shmvs,
		struct ____coobj_mator_s *mholder);
	void (*mator_mholder_init_resv_vm)(
		struct ____coobj_mator_s *mholder,
		__uptr_t resv_vm_start);

	void (*mator_mrenter_init)(
		struct ____coobj_mator_s *mrenter,
		struct ____coobj_mator_s *mholder,
		struct ____coobj_uator_s *urenter_mva);
	void (*mator_mrenter_init_shmvs)(
		struct ____coobj_mator_s *mrenter,
		struct coobj_vspace_s *shmvs);
	void (*mator_mrenter_fini_shmvs)(
		struct coobj_vspace_s *mapper_shmvs,
		struct ____coobj_mator_s *mrenter,
		__u8 *mapped);
	void (*mator_mrenter_init_resv_vm)(
		struct ____coobj_mator_s *mrenter,
		__uptr_t resv_vm_start);
	int (*mator_map_exist)(
		struct coobj_vspace_s *mapper_shmvs, struct ____coobj_mator_s *mholder,
		struct coobj_vspace_s *renter_shmvs, __bool large_obj);

	int (*mator_map)(
		struct coobj_vspace_s *mapper_shmvs, struct ____coobj_mator_s *mholder, struct coobj_vspace_s *shmvs,
		struct ____coobj_chunkid_s chunkid, __u8 *mapped, __relpos_t size, __bool large_obj, unsigned int flags);
	int (*mator_unmap)(
		struct coobj_vspace_s *mapper_shmvs, struct ____coobj_mator_s *mholder, struct coobj_vspace_s *shmvs,
		struct ____coobj_chunkid_s chunkid, __bool large_obj);
};

#endif
