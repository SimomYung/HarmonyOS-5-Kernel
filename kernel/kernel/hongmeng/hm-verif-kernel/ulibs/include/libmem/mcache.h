/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Interfaces of mcache in ulibs
 * Author: Huawei OS Kernel Lab
 * Create: Sat March 9 17:03:20 2024
 */

#ifndef ULIBS_INCLUDE_LIBMEM_MCACHE_H
#define ULIBS_INCLUDE_LIBMEM_MCACHE_H

#define DECLARE_ULIBS_MCACHE(mcachename)					\
void *mcache_##mcachename##_malloc(void);					\
void mcache_##mcachename##_free(void *ptr);					\
size_t mcache_##mcachename##_size(void);					\

#define DEFINE_ULIBS_MCACHE(mcachename, obj_size)				\
__attribute__((weak)) void *mcache_##mcachename##_malloc(void)			\
{										\
	return malloc(obj_size);						\
}										\
__attribute__((weak)) void mcache_##mcachename##_free(void *ptr)		\
{										\
	free(ptr);								\
}										\
size_t mcache_##mcachename##_size(void)						\
{										\
	return obj_size;							\
}										\

#define DECLARE_ULIBS_MCACHE_VARIANT(mcachename)				\
void *mcache_##mcachename##_malloc(size_t obj_size);				\
void mcache_##mcachename##_free(void *ptr);					\

#define DEFINE_ULIBS_MCACHE_VARIANT(mcachename)					\
__attribute__((weak)) void *mcache_##mcachename##_malloc(size_t obj_size)	\
{										\
	return malloc(obj_size);						\
}										\
__attribute__((weak)) void mcache_##mcachename##_free(void *ptr)		\
{										\
	free(ptr);								\
}										\

#endif
