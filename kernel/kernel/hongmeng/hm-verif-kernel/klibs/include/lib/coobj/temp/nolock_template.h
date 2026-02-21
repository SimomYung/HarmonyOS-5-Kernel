/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: no-lock template for variable-size allocator
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 28 12:33:34 2023 +0800
 */
#ifndef KLIBS_COOBJ_TEMP_NOLOCK_TEMPLATE_H
#define KLIBS_COOBJ_TEMP_NOLOCK_TEMPLATE_H

#include <lib/coobj/ator.h>
#include <lib/coobj/temp/base_template.h>

#define ____coobj_define_ator_nolock(object_type_name, ator_kind, type, ...) \
	typedef type ____coobj_tepar_self_type(object_type_name); \
	enum { \
		____coobj_tepar_type_size(object_type_name) = sizeof(____coobj_tepar_self_type(object_type_name)), \
		____coobj_tepar_urenter_acqst_size(object_type_name) = ____coobj_##ator_kind##_urenter_acqst_size(), \
	}; \
	____coobj_define_ator_base_function(object_type_name, ator_kind) \
	static inline \
	uintptr_t coobj_##object_type_name##_user_init( \
		struct ____coobj_uator_list_s *uator_list, coobj_typeid_t uator_typeid, unsigned int flags,\
		enum coobj_role_kind_e role, struct ____coobj_base_user_s *base_user, __u8 mapped) \
	{ \
		uintptr_t ret = 0; \
		__uptr_t ator_index = ____coobj_uator_list_find_ator(uator_list, uator_typeid); \
		if (role == COOBJ_ROLE_HOLDER) { \
			____coobj_tepar_uholder_type(object_type_name) *uholder = NULL; \
			if (ator_index != ____COOBJ_INVALID_ATOR) { \
				uholder = coobj_##object_type_name##_uholder(&uator_list->data[ator_index]); \
				uholder->ator.type_id = uator_typeid; \
				uholder->ator.flags = flags; \
				____coobj_##ator_kind##_uholder_init(&uholder->ator, base_user); \
			} \
			return (uintptr_t)uholder; \
		} else if (role == COOBJ_ROLE_RENTER) { \
			____coobj_tepar_urenter_type(object_type_name) *urenter = NULL; \
			if (ator_index != ____COOBJ_INVALID_ATOR) { \
				urenter = coobj_##object_type_name##_urenter(&uator_list->data[ator_index]); \
				urenter->ator.type_id = uator_typeid; \
				if (mapped != ALL_OBJ_ATOR_MAPPED) { \
					____coobj_##ator_kind##_urenter_init(&urenter->ator, base_user, mapped); \
				} \
			} \
			return (uintptr_t)urenter; \
		} \
		return ret; \
	} \
	static inline \
	void *coobj_user_idx_to_ptr( \
		struct coobj_##object_type_name##_user_s *user, __relpos_t relpos) \
	{ \
		return (void *)____coobj_##ator_kind##_uholder_tgtobj( \
			&user->holder_ator, relpos, \
			____coobj_tepar_type_size(object_type_name), \
			__VA_ARGS__); \
	} \
	static inline \
	__relpos_t coobj_##object_type_name##_malloc( \
		struct coobj_##object_type_name##_uholder_s *uholder, __relpos_t size) \
	{ \
		__relpos_t relpos =  ____coobj_##ator_kind##_uholder_alloc( \
			size, \
			&uholder->ator, \
			____coobj_tepar_type_size(object_type_name), \
			__VA_ARGS__); \
		return relpos; \
	} \
	static inline \
	int coobj_##object_type_name##_free( \
		struct coobj_##object_type_name##_uholder_s *uholder, __relpos_t relpos) \
	{ \
		return ____coobj_##ator_kind##_uholder_free( \
			&uholder->ator, relpos, \
			____coobj_tepar_type_size(object_type_name), \
			__VA_ARGS__); \
	} \
	static inline \
	int ____coobj_##object_type_name##_mholder_map( \
		struct coobj_vspace_s *mapper_shmvs, \
		struct ____coobj_mator_s *mholder, struct coobj_vspace_s *shmvs, \
		struct ____coobj_chunkid_s chunkid, __u8 *mapped, __relpos_t size, __bool large_obj, unsigned int flags) \
	{ \
		return ____coobj_##ator_kind##_mholder_map( \
			mapper_shmvs, &mholder->ator_kind##_mholder, chunkid, mapped, size, large_obj, flags, \
			____coobj_tepar_type_size(object_type_name), \
			__VA_ARGS__); \
	} \
	static inline \
	int ____coobj_##object_type_name##_mholder_unmap( \
		struct coobj_vspace_s *mapper_shmvs, \
		struct ____coobj_mator_s *mholder, struct coobj_vspace_s *shmvs, \
		struct ____coobj_chunkid_s chunkid, __bool large_obj) \
	{ \
		return ____coobj_##ator_kind##_mholder_unmap( \
			mapper_shmvs, &mholder->ator_kind##_mholder, chunkid, large_obj, \
			____coobj_tepar_type_size(object_type_name), \
			__VA_ARGS__); \
	} \
	static inline \
	void ____coobj_mator_##object_type_name##_mholder_fini_shmvs( \
		struct coobj_vspace_s *mapper_shmvs, \
		struct ____coobj_mator_s *mholder) \
	{ \
		return ____coobj_mator_##ator_kind##_mholder_fini_shmvs( \
			mapper_shmvs, mholder, \
			____coobj_tepar_type_size(object_type_name), \
			__VA_ARGS__); \
	} \
	static inline \
	void ____coobj_mator_##object_type_name##_mrenter_fini_shmvs( \
		struct coobj_vspace_s *mapper_shmvs, \
		struct ____coobj_mator_s *mrenter, __u8 *mapped) \
	{ \
		return ____coobj_mator_##ator_kind##_mrenter_fini_shmvs( \
			mapper_shmvs, mrenter, mapped, \
			____coobj_tepar_type_size(object_type_name), \
			__VA_ARGS__); \
	} \
	static inline \
	int ____coobj_##object_type_name##_mrenter_map_exist( \
		struct coobj_vspace_s *mapper_shmvs, \
		struct ____coobj_mator_s *mholder, struct coobj_vspace_s *renter_shmvs, __bool large_obj) \
	{ \
		return ____coobj_##ator_kind##_mrenter_map_exist( \
			mapper_shmvs, &mholder->ator_kind##_mholder, renter_shmvs, large_obj, \
			____coobj_tepar_type_size(object_type_name), \
			__VA_ARGS__); \
	} \
	static inline \
	struct coobj_mator_op_s coobj_##object_type_name##_mator_op(void) \
	{ \
		struct coobj_mator_op_s op = { \
			.mator_mholder_init = ____coobj_mator_##ator_kind##_mholder_init, \
			.mator_mholder_init_shmvs = ____coobj_mator_##ator_kind##_mholder_init_shmvs, \
			.mator_mholder_fini_shmvs = ____coobj_mator_##object_type_name##_mholder_fini_shmvs, \
			.mator_mholder_init_resv_vm = ____coobj_mator_##ator_kind##_mholder_init_resv_vm, \
			.mator_mrenter_init = ____coobj_mator_##ator_kind##_mrenter_init, \
			.mator_mrenter_init_shmvs = ____coobj_mator_##ator_kind##_mrenter_init_shmvs, \
			.mator_mrenter_fini_shmvs = ____coobj_mator_##object_type_name##_mrenter_fini_shmvs, \
			.mator_mrenter_init_resv_vm = ____coobj_mator_##ator_kind##_mrenter_init_resv_vm, \
			.mator_map_exist = ____coobj_##object_type_name##_mrenter_map_exist, \
			.mator_map = ____coobj_##object_type_name##_mholder_map, \
			.mator_unmap = ____coobj_##object_type_name##_mholder_unmap, \
		}; \
		return op; \
	}

#endif
