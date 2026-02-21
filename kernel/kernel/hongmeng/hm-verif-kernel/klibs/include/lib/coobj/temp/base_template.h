/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: A Multi-data-structure generic co-object and its allocator
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 16 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_TEMP_BASE_TEMPLATE_H
#define KLIBS_COOBJ_TEMP_BASE_TEMPLATE_H

#define ____coobj_tepar_type(object_type_name) ____coobj_tepar_type_##object_type_name
#define ____coobj_tepar_typeid(object_type_name) ____coobj_tepar_typeid_##object_type_name
#define ____coobj_tepar_type_size(object_type_name) ____coobj_tepar_type_size_##object_type_name
#define ____coobj_tepar_urenter_acqst_size(object_type_name) ____coobj_tepar_urenter_acqst_size_##object_type_name
#define ____coobj_tepar_self_type(object_type_name) ____coobj_tepar_self_type_##object_type_name

#define ____coobj_tepar_mholder_type(object_type_name) struct coobj_##object_type_name##_mholder_s
#define ____coobj_tepar_mrenter_type(object_type_name) struct coobj_##object_type_name##_mrenter_s
#define ____coobj_tepar_uholder_type(object_type_name) struct coobj_##object_type_name##_uholder_s
#define ____coobj_tepar_urenter_type(object_type_name) struct coobj_##object_type_name##_urenter_s
#define ____coobj_tepar_user_type(object_type_name) struct coobj_##object_type_name##_user_s

#define ____coobj_define_param_1(format, arg_type, arg_name) format(arg_type, arg_name)
#define ____coobj_define_param_2(format, arg_type, arg_name, ...) format(arg_type, arg_name), \
	____coobj_define_param_1(format, __VA_ARGS__)
#define ____coobj_define_param_3(format, arg_type, arg_name, ...) format(arg_type, arg_name), \
	____coobj_define_param_2(format, __VA_ARGS__)
#define ____coobj_define_param_4(format, arg_type, arg_name, ...) format(arg_type, arg_name), \
	____coobj_define_param_3(format, __VA_ARGS__)

#define ____coobj_define_param_read_format(arg_type, arg_name) arg_type *arg_name
#define ____coobj_define_param_write_format(arg_type, arg_name) arg_type arg_name
#define ____coobj_define_param_ref_format(arg_type, arg_name) arg_name

#define ____coobj_define_param_num_count(t0, n0, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, ...) t7
#define ____coobj_define_param_num(...) \
	____coobj_define_param_num_count(__VA_ARGS__, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0)

#define ____coobj_define_param_read(arg_num, ...) \
	____coobj_define_param_##arg_num(____coobj_define_param_read_format, __VA_ARGS__)

#define ____coobj_define_param_write(arg_num, ...) \
	____coobj_define_param_##arg_num(____coobj_define_param_write_format, __VA_ARGS__)

#define ____coobj_define_param_ref(arg_num, ...) \
	____coobj_define_param_##arg_num(____coobj_define_param_ref_format, __VA_ARGS__)

#define ____coobj_define_ator_base_function(object_type_name, ator_kind) \
	____coobj_tepar_mholder_type(object_type_name) { \
		struct ____coobj_##ator_kind##_mholder_s ator; \
	}; \
	____coobj_tepar_mrenter_type(object_type_name) { \
		struct ____coobj_##ator_kind##_mrenter_s ator; \
	}; \
	____coobj_tepar_uholder_type(object_type_name) { \
		struct ____coobj_##ator_kind##_uholder_s ator; \
	}; \
	____coobj_tepar_urenter_type(object_type_name) { \
		struct ____coobj_##ator_kind##_urenter_s ator; \
	}; \
	____coobj_tepar_user_type(object_type_name) { \
		union { \
			struct ____coobj_##ator_kind##_uholder_s holder_ator; \
			struct ____coobj_##ator_kind##_urenter_s renter_ator; \
		}; \
	}; \
	static inline \
	____coobj_tepar_mholder_type(object_type_name) *coobj_##object_type_name##_mholder( \
		struct ____coobj_mator_s *mator) \
	{ \
		return container_of(&mator->ator_kind##_mholder, ____coobj_tepar_mholder_type(object_type_name), ator); \
	} \
	static inline \
	____coobj_tepar_mrenter_type(object_type_name) *coobj_##object_type_name##_mrenter( \
		struct ____coobj_mator_s *mator) \
	{ \
		return container_of(&mator->ator_kind##_mrenter, ____coobj_tepar_mrenter_type(object_type_name), ator); \
	} \
	static inline \
	____coobj_tepar_uholder_type(object_type_name) *coobj_##object_type_name##_uholder( \
		struct ____coobj_uator_s *uator) \
	{ \
		return container_of(&uator->ator_kind##_uholder, ____coobj_tepar_uholder_type(object_type_name), ator); \
	} \
	static inline \
	____coobj_tepar_urenter_type(object_type_name) *coobj_##object_type_name##_urenter( \
		struct ____coobj_uator_s *uator) \
	{ \
		return container_of(&uator->ator_kind##_urenter, ____coobj_tepar_urenter_type(object_type_name), ator); \
	} \
	static inline \
	____coobj_tepar_uholder_type(object_type_name) *coobj_##object_type_name##_uholder_init( \
		struct ____coobj_uator_list_s *uator_list, coobj_typeid_t uator_typeid, struct ____coobj_base_user_s *base_user) \
	{ \
		____coobj_tepar_uholder_type(object_type_name) *uholder = NULL; \
		__uptr_t uholder_ator_index = ____coobj_uator_list_find_ator(uator_list, uator_typeid); \
		if (uholder_ator_index != ____COOBJ_INVALID_ATOR) { \
			uholder = coobj_##object_type_name##_uholder(&uator_list->data[uholder_ator_index]); \
			____coobj_##ator_kind##_uholder_init(&uholder->ator, base_user); \
		} \
		return uholder; \
	} \
	static inline \
	____coobj_tepar_urenter_type(object_type_name) *coobj_##object_type_name##_urenter_init( \
		struct ____coobj_uator_list_s *uator_list, coobj_typeid_t uator_typeid, \
		__u8 mapped, struct ____coobj_base_user_s *base_user) \
	{ \
		____coobj_tepar_urenter_type(object_type_name) *urenter = NULL; \
		__uptr_t renter_ator_index = ____coobj_uator_list_find_ator(uator_list, uator_typeid); \
		if (renter_ator_index != ____COOBJ_INVALID_ATOR) { \
			urenter = coobj_##object_type_name##_urenter(&uator_list->data[renter_ator_index]); \
			____coobj_##ator_kind##_urenter_init(&urenter->ator, base_user, mapped); \
		} \
		return urenter; \
	}

#endif
