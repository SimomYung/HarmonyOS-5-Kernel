/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: scopedptr used only by procmgr and threadmgr
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 08 15:38:27 2021
 */
#ifndef __LIBHWSYNC_SYSMGRSCOPEDPTR_H_
#define __LIBHWSYNC_SYSMGRSCOPEDPTR_H_

#ifndef SYSMGR_SCOPEDPTR
# error "Only included by thread/thread_scopedptr.h process/proc_scopedptr.h and prcess/proc_acquire.h"
#endif

#include <libhmsync/raw_scopedptr.h>

#define define_refcnt_function_withlock(name, offset, lock_func, unlock_func, lock_type, lock_name, 		       \
					mid_type, find_get, find_get_target, put, ...)                                 \
	static inline void *acquire_##name##_ptr_ref_withlock(__scopedptr_map(2, __scopedptr_def, __VA_ARGS__))        \
	{                                                                                                              \
		void *ret = NULL;                                                                                      \
		void *ptr = find_get(__scopedptr_map(2, __scopedptr_call, __VA_ARGS__));                               \
		if (ptr != NULL) {                                                                                     \
			void *lock = &((mid_type)ptr)->lock_name;                                                      \
			lock_func((lock_type)lock);                                                                    \
			ret = find_get_target((mid_type)ptr);                                                          \
			unlock_func((lock_type)lock);                                                                  \
			put((mid_type)ptr);                                                                            \
		}                                                                                                      \
		return ret;                                                                                            \
	}

#define declare_scopeptr_withlock(name, offset, lock_func, unlock_func, lock_type, lock_name, mid_type,	               \
				  find_get, find_get_target, put, ...)                                                 \
	declare_scopedptr_type(name)										       \
	define_refcnt_function_withlock(name, offset, lock_func, unlock_func, lock_type, lock_name, 		       \
					mid_type, find_get, find_get_target, put, __VA_ARGS__)			       \
	define_scopedptr_release(name)

#define acquire_scopedptr_withlock(name, ptr_name, offset, ...) \
	ptr_name = acquire_##name##_ptr_ref_withlock(__VA_ARGS__); \
	struct raw_##name##_scopedptr __scopedptr_to_##ptr_name \
		__attribute__((cleanup(raw_##name##_scopedptr_auto_release))) = SCOPEDPTR_INITIALIZER; \
	scopedptr_init(&__scopedptr_to_##ptr_name, (ptr_name != NULL) ? (char*)((uintptr_t)(ptr_name) + (offset)) : NULL);


#endif
