/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interfaces of raw_scoped_compounded_ptr
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 28 11:35:44 2023
 */

#ifndef ULIBS_INCLUDE_LIBHMSYNC_RAWSCOPED_COPTR_H
#define ULIBS_INCLUDE_LIBHMSYNC_RAWSCOPED_COPTR_H

#include <libhmsync/raw_scopedptr.h>
#include <libhmsync/raw_compounded_refcnt.h>

#ifdef  __cplusplus
extern "C" {
#endif

struct raw_compounded_auto_refcnt;
typedef void (*coref_release_func)(struct raw_compounded_auto_refcnt *);

struct raw_compounded_auto_refcnt {
	struct raw_compounded_refcnt refcnt_nr;
	coref_release_func release;
};

#define __RAW_COMPOUNDED_REFCNT_INITIALIZER(n) {.refcnt_nr = {.v = RAW_ATOMIC_VAL_INIT(n)}, .release = NULL}

static inline void
raw_compounded_auto_refcnt_init(struct raw_compounded_auto_refcnt *ref,
		uint64_t count,
		coref_release_func func)
{
	ref->release = func;
	raw_compounded_refcnt_init(&ref->refcnt_nr, count);
}

static inline int
raw_compounded_auto_refcnt_counter(struct raw_compounded_auto_refcnt *ref)
{
	uint64_t res = (uint64_t)-1;
	int ret = raw_compounded_refcnt_of(&ref->refcnt_nr, &res);

	/* it is safe. the refcnt of `raw_compounded_refcnt` is 24bit */
	return (ret < 0) ? ret : (int)res;
}

static inline int
raw_compounded_auto_refcnt_get(struct raw_compounded_auto_refcnt *ref)
{
	uint64_t res = (uint64_t)-1;
	int ret = raw_compounded_refcnt_get(&ref->refcnt_nr, &res);

	return (ret < 0) ? ret : (int)__raw_compounded_v2refcnt(res);
}

static inline int
raw_compounded_auto_refcnt_put(struct raw_compounded_auto_refcnt *ref)
{
	uint64_t res = (uint64_t)-1;
	int ret = raw_compounded_refcnt_put(&ref->refcnt_nr, &res);
	if ((ret == E_HM_OK) && (res == 0) && (ref->release != NULL)) {
		ref->release(ref);
	}
	return (ret < 0) ? ret : (int)__raw_compounded_v2refcnt(res);
}

static inline int
raw_compounded_auto_rpccnt_put(struct raw_compounded_auto_refcnt *ref)
{
	uint64_t res = (uint64_t)-1;
	int ret = raw_compounded_rpccnt_put(&ref->refcnt_nr, &res);
	if ((ret == E_HM_OK) && (res == 0) && (ref->release != NULL)) {
		ref->release(ref);
	}
	return ret;
}

#define define_acquire_ptr_corefx(x, name, offset, lock_guard, acquire, ...) \
	static inline void* acquire_##name##_ptr_coref(__scopedptr_map(x, __scopedptr_def, __VA_ARGS__)) \
	{ \
		lock_guard; \
		void *ptr = acquire(__scopedptr_map(x, __scopedptr_call, __VA_ARGS__)); \
		if (ptr != NULL) { \
			struct raw_compounded_auto_refcnt *ref = (struct raw_compounded_auto_refcnt *)(uintptr_t)((char*)ptr + (offset)); \
			if (raw_compounded_auto_refcnt_get(ref) < 0) { \
				ptr = NULL; \
			} \
		} \
		return ptr;\
	}

#define define_co_refcnt_functionx(x, name, offset, lock_guard, acquire, ...) \
	define_acquire_ptr_corefx(x, name, offset, lock_guard, acquire, __VA_ARGS__) \

#define define_co_refcnt_function1(name, offset, lock_guard, acquire, ...) \
	define_co_refcnt_functionx(1, name, offset, lock_guard, acquire, __VA_ARGS__)
#define define_co_refcnt_function2(name, offset, lock_guard, acquire, ...) \
	define_co_refcnt_functionx(2, name, offset, lock_guard, acquire, __VA_ARGS__)
#define define_co_refcnt_function3(name, offset, lock_guard, acquire, ...) \
	define_co_refcnt_functionx(3, name, offset, lock_guard, acquire, __VA_ARGS__)
#define define_co_refcnt_function4(name, offset, lock_guard, acquire, ...) \
	define_co_refcnt_functionx(4, name, offset, lock_guard, acquire, __VA_ARGS__)
#define define_co_refcnt_function5(name, offset, lock_guard, acquire, ...) \
	define_co_refcnt_functionx(5, name, offset, lock_guard, acquire, __VA_ARGS__)
#define define_co_refcnt_function6(name, offset, lock_guard, acquire, ...) \
	define_co_refcnt_functionx(6, name, offset, lock_guard, acquire, __VA_ARGS__)


#define declare_scoped_coptr_type(name) \
	struct raw_##name##_scoped_coptr { \
		struct raw_compounded_auto_refcnt *ref; \
	}

#define SCOPED_COPTR_INITIALIZER {.ref = NULL}
#define scoped_coptr_init(name, _ref) \
	{\
		(name)->ref = (struct raw_compounded_auto_refcnt *)(uintptr_t)(_ref); \
	}

#define define_scoped_coptr_release(name, lock_guard)\
	static inline void raw_##name##_scoped_coptr_auto_release(struct raw_##name##_scoped_coptr *scopedptr) \
	{\
		if (scopedptr->ref != NULL) { \
			(void)raw_compounded_auto_refcnt_put(scopedptr->ref);\
		} \
	}

/*
 *  name:       the name we protected
 *  offset:     offset is the ptr to struct raw_refcnt in object
 *  lock_guard: Lock when we try to acquire object pointer
 *  acquire:    acquire function of the object pointer
 */
#define declare_scope_coptrx(x, name, offset, lock_guard, acquire, ...)\
	declare_scoped_coptr_type(name); \
	define_co_refcnt_functionx(x, name, offset, lock_guard, acquire, __VA_ARGS__) \
	define_scoped_coptr_release(name, lock_guard)

#define declare_scope_coptr6(name, offset, lock_guard, acquire, ...) \
	declare_scope_coptrx(6, name, offset, lock_guard, acquire,  __VA_ARGS__)
#define declare_scope_coptr5(name, offset, lock_guard, acquire, ...) \
	declare_scope_coptrx(5, name, offset, lock_guard, acquire,  __VA_ARGS__)
#define declare_scope_coptr4(name, offset, lock_guard, acquire, ...) \
	declare_scope_coptrx(4, name, offset, lock_guard, acquire, __VA_ARGS__)
#define declare_scope_coptr3(name, offset, lock_guard, acquire, ...) \
	declare_scope_coptrx(3, name, offset, lock_guard, acquire, __VA_ARGS__)
#define declare_scope_coptr2(name, offset, lock_guard, acquire, ...) \
	declare_scope_coptrx(2, name, offset, lock_guard, acquire, __VA_ARGS__)
#define declare_scope_coptr1(name, offset, lock_guard, acquire, ...) \
	declare_scope_coptrx(1, name, offset, lock_guard, acquire, __VA_ARGS__)


#define acquire_scoped_coptr(name, ptr_name, offset, ...) \
	ptr_name = acquire_##name##_ptr_coref(__VA_ARGS__); \
	struct raw_##name##_scoped_coptr __scopedptr_to_##ptr_name \
		__attribute__((cleanup(raw_##name##_scoped_coptr_auto_release))) = SCOPED_COPTR_INITIALIZER; \
	scoped_coptr_init(&__scopedptr_to_##ptr_name, (ptr_name != NULL) ? (char*)((uintptr_t)(ptr_name) + (offset)) : NULL)


#ifdef __cplusplus
}
#endif

#endif
