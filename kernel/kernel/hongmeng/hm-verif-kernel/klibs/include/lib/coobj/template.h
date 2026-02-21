/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: A Multi-data-structure generic co-object and its allocator
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 16 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_TEMPLATE_H
#define KLIBS_COOBJ_TEMPLATE_H

#include <lib/coobj/temp/seqlock_template.h>
#include <lib/coobj/temp/nolock_template.h>

#define coobj_define_array(object_type_name, type, tepar_typeid, tepar_chunk_order) \
	____coobj_define_ator( \
		object_type_name, array, type, tepar_typeid, tepar_chunk_order)
#define coobj_define_radix1(object_type_name, type, tepar_typeid, tepar_chunk_order, tepar_level1_order) \
	____coobj_define_ator( \
		object_type_name, radix1, type, tepar_typeid, tepar_chunk_order, tepar_level1_order)
#define coobj_define_radix1var(object_type_name, type, tepar_chunk_order, tepar_level1_order) \
	____coobj_define_ator_nolock( \
		object_type_name, radix1var, type, tepar_chunk_order, tepar_level1_order)

#define coobj_define_read(object_type_name, method_name, ...) \
	____coobj_define_ator_read( \
		object_type_name, method_name, ____coobj_define_param_num(__VA_ARGS__), __VA_ARGS__)
#define coobj_define_write(object_type_name, method_name, ...) \
	____coobj_define_ator_write( \
		object_type_name, method_name, ____coobj_define_param_num(__VA_ARGS__), __VA_ARGS__)

#define coobj_define_read_nolock(object_type_name, ...) \
	____coobj_define_ator_read_nolock( \
		object_type_name, ____coobj_define_param_num(__VA_ARGS__), __VA_ARGS__)
#define coobj_define_write_nolock(object_type_name, ...) \
	____coobj_define_ator_write_nolock( \
		object_type_name, ____coobj_define_param_num(__VA_ARGS__), __VA_ARGS__)

#endif
