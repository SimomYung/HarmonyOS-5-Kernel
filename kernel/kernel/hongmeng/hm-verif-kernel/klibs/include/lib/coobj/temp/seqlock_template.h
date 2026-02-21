/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: A Multi-data-structure generic co-object and its allocator
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 16 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_TEMP_SEQLOCK_TEMPLATE_H
#define KLIBS_COOBJ_TEMP_SEQLOCK_TEMPLATE_H

#include <lib/coobj/ator.h>
#include <lib/coobj/temp/base_template.h>

#define ____coobj_define_ator(object_type_name, ator_kind, type, coobj_typeid, ...) \
	typedef type ____coobj_tepar_self_type(object_type_name); \
	typedef struct { \
		__u64 version; \
		type self; \
	} ____coobj_tepar_type(object_type_name); \
	enum { \
		____coobj_tepar_typeid(object_type_name) = coobj_typeid, \
		____coobj_tepar_type_size(object_type_name) = sizeof(____coobj_tepar_type(object_type_name)), \
		____coobj_tepar_urenter_acqst_size(object_type_name) = ____coobj_##ator_kind##_urenter_acqst_size(), \
	}; \
	____coobj_define_ator_base_function(object_type_name, ator_kind) \
	static inline \
	____coobj_tepar_type(object_type_name) *____coobj_##object_type_name##_uholder_tgtobj( \
		struct coobj_##object_type_name##_uholder_s *uholder, __relpos_t relpos) \
	{ \
		return (____coobj_tepar_type(object_type_name) *)____coobj_##ator_kind##_uholder_tgtobj( \
			&uholder->ator, relpos, \
			____coobj_tepar_typeid(object_type_name), \
			____coobj_tepar_type_size(object_type_name), \
			__VA_ARGS__); \
	} \
	static inline \
	__relpos_t coobj_##object_type_name##_alloc( \
		struct coobj_##object_type_name##_uholder_s *uholder) \
	{ \
		__relpos_t relpos =  ____coobj_##ator_kind##_uholder_alloc( \
			&uholder->ator, \
			____coobj_tepar_typeid(object_type_name), \
			____coobj_tepar_type_size(object_type_name), \
			__VA_ARGS__); \
		if (relpos != ____COOBJ_INVALID_POS) { \
			____coobj_tepar_type(object_type_name) *object = \
				____coobj_##object_type_name##_uholder_tgtobj(uholder, relpos); \
			__atomic_store_n(&object->version, 1, __ATOMIC_RELEASE); \
		} \
		return relpos; \
	} \
	static inline \
	int coobj_##object_type_name##_free( \
		struct coobj_##object_type_name##_uholder_s *uholder, __relpos_t relpos) \
	{ \
		return ____coobj_##ator_kind##_uholder_free( \
			&uholder->ator, relpos, \
			____coobj_tepar_typeid(object_type_name), \
			____coobj_tepar_type_size(object_type_name), \
			__VA_ARGS__); \
	} \
	static inline \
	int coobj_##object_type_name##_recycle( \
		struct coobj_##object_type_name##_uholder_s *uholder) \
	{ \
		return ____coobj_##ator_kind##_uholder_recycle( \
			&uholder->ator, \
			____coobj_tepar_typeid(object_type_name), \
			____coobj_tepar_type_size(object_type_name), \
			__VA_ARGS__); \
	} \
	static inline \
	__bool ____coobj_##object_type_name##_urenter_acquire_ptr( \
		struct coobj_##object_type_name##_urenter_s *urenter, __relpos_t relpos, __bool *acqst) \
	{ \
		return ____coobj_##ator_kind##_urenter_acquire_ptr( \
			&urenter->ator, relpos, acqst, \
			____coobj_tepar_typeid(object_type_name), \
			____coobj_tepar_type_size(object_type_name), \
			__VA_ARGS__); \
	} \
	static inline \
	void ____coobj_##object_type_name##_urenter_release_ptr( \
		struct coobj_##object_type_name##_urenter_s *urenter, __relpos_t relpos, __bool *acqst) \
	{ \
		____coobj_##ator_kind##_urenter_release_ptr( \
			&urenter->ator, relpos, acqst, \
			____coobj_tepar_typeid(object_type_name), \
			____coobj_tepar_type_size(object_type_name), \
			__VA_ARGS__); \
	} \
	static inline \
	____coobj_tepar_type(object_type_name) *____coobj_##object_type_name##_urenter_tgtobj( \
		struct coobj_##object_type_name##_urenter_s *urenter, __relpos_t relpos) \
	{ \
		return (____coobj_tepar_type(object_type_name) *)____coobj_##ator_kind##_urenter_tgtobj( \
			&urenter->ator, relpos, \
			____coobj_tepar_typeid(object_type_name), \
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
			mapper_shmvs, &mholder->ator_kind##_mholder, chunkid, mapped, size, large_obj, \
			____coobj_tepar_typeid(object_type_name), \
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
			____coobj_tepar_typeid(object_type_name), \
			____coobj_tepar_type_size(object_type_name), \
			__VA_ARGS__); \
	} \
	static inline \
	struct coobj_mator_op_s coobj_##object_type_name##_mator_op() \
	{ \
		struct coobj_mator_op_s op = { \
			.mator_mholder_init = ____coobj_mator_##ator_kind##_mholder_init, \
			.mator_mholder_init_shmvs = ____coobj_mator_##ator_kind##_mholder_init_shmvs, \
			.mator_mholder_init_resv_vm = ____coobj_mator_##ator_kind##_mholder_init_resv_vm, \
			.mator_mrenter_init = ____coobj_mator_##ator_kind##_mrenter_init, \
			.mator_mrenter_init_shmvs = ____coobj_mator_##ator_kind##_mrenter_init_shmvs, \
			.mator_mrenter_init_resv_vm = ____coobj_mator_##ator_kind##_mrenter_init_resv_vm, \
			.mator_map = ____coobj_##object_type_name##_mholder_map, \
			.mator_unmap = ____coobj_##object_type_name##_mholder_unmap, \
		}; \
		return op; \
	}

#define ____coobj_read_sequence(object, read_entry, arg_num, ...) \
	do { \
		__u64 version_pre = 0; \
		__u64 version_post = 0; \
		do { \
			version_pre = __atomic_load_n(&(object)->version, __ATOMIC_ACQUIRE); \
			read_entry(&(object)->self, ____coobj_define_param_ref(arg_num, __VA_ARGS__)); \
			version_post = __atomic_load_n(&(object)->version, __ATOMIC_ACQUIRE); \
		} while (!____coobj_urenter_version_check(version_pre, version_post)); \
	} while (0)

#define ____coobj_write_sequence(object, write_entry, arg_num, ...) \
	do { \
		__atomic_add_fetch(&(object)->version, 1, __ATOMIC_RELEASE); \
		write_entry(&(object)->self, ____coobj_define_param_ref(arg_num, __VA_ARGS__)); \
		__atomic_add_fetch(&(object)->version, 1, __ATOMIC_RELEASE); \
	} while (0)

#define ____coobj_define_ator_read(object_type_name, method_name, arg_num, ...) \
	static inline \
	void ____coobj_##object_type_name##_read_##method_name( \
		____coobj_tepar_self_type(object_type_name) *self, \
		____coobj_define_param_read(arg_num, __VA_ARGS__)); \
	static inline \
	void coobj_##object_type_name##_read_##method_name( \
		struct coobj_##object_type_name##_urenter_s *urenter, \
		__relpos_t relpos, \
		____coobj_define_param_read(arg_num, __VA_ARGS__)) \
	{ \
		__bool acqst[____coobj_tepar_urenter_acqst_size(object_type_name)] = {__false, __false, __false, __false}; \
		if (____coobj_##object_type_name##_urenter_acquire_ptr(urenter, relpos, acqst)) { \
			____coobj_tepar_type(object_type_name) *object = \
				____coobj_##object_type_name##_urenter_tgtobj(urenter, relpos); \
			____coobj_read_sequence( \
				object, ____coobj_##object_type_name##_read_##method_name, arg_num, __VA_ARGS__); \
		} \
		____coobj_##object_type_name##_urenter_release_ptr( \
			urenter, relpos, acqst); \
	} \
	static inline \
	void ____coobj_##object_type_name##_read_##method_name( \
		____coobj_tepar_self_type(object_type_name) *self, \
		____coobj_define_param_read(arg_num, __VA_ARGS__))

#define ____coobj_define_ator_write(object_type_name, method_name, arg_num, ...) \
	static inline \
	void ____coobj_##object_type_name##_write_##method_name( \
		____coobj_tepar_self_type(object_type_name) *self, ____coobj_define_param_write(arg_num, __VA_ARGS__)); \
	static inline  \
	void coobj_##object_type_name##_hwrite_##method_name( \
		struct coobj_##object_type_name##_uholder_s *uholder, \
		__relpos_t relpos, \
		____coobj_define_param_write(arg_num, __VA_ARGS__)) \
	{ \
		____coobj_tepar_type(object_type_name) *object = \
			____coobj_##object_type_name##_uholder_tgtobj( \
				uholder, relpos); \
		____coobj_write_sequence( \
			object, ____coobj_##object_type_name##_write_##method_name, arg_num, __VA_ARGS__); \
	} \
	static inline \
	void ____coobj_##object_type_name##_write_##method_name( \
		____coobj_tepar_self_type(object_type_name) *self, ____coobj_define_param_write(arg_num, __VA_ARGS__))

#endif
