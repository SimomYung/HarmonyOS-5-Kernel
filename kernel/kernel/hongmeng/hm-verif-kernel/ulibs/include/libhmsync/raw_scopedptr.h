/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Interfaces of raw_scopedptr
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 13 13:11:53 2018
 */

#ifndef ULIBS_INCLUDE_LIBHMSYNC_RAWSCOPEDPTR_H
#define ULIBS_INCLUDE_LIBHMSYNC_RAWSCOPEDPTR_H

#include <hongmeng/errno.h>
#include <libhmsync/atomic.h>
#include <hongmeng/panic.h>

#ifdef  __cplusplus
extern "C" {
#endif

/*
	There are two objects in this file, one is raw refcnt and another is
	raw scoped pointer.

	-------------------
	raw reference count
	-------------------

	A raw reference count is a common way to manage reference count of an object(pointer).

	Scenario:
	    When we use a pointer like `struct process *`, it might be deleted by
	    another thread. Refcount mechanism could be indroduced.

	    We use a refcnt to record the object pointer usage count, the object can only be deleted
	    when the reference count is zero.

	Usage:
	1. Put `struct raw_refcnt` into the target object.

	2. Use macro `define_refcnt_functionn` to define acquire function.
	   Then we could use `acquire_xxx_ptr_ref` to acquire
	   object pointer. (object ptr is protected object ptr, like `struct process`).

	3. Init `struct raw_refcnt` by calling raw_refcnt_init().
	   `auto_release` callback function should be specified.
	   This function will be called when refcnt is zero in put & release.

	4. Get the object by calling acquire_xxx_ptr_ref().
	   1). Lock the object ptr owner.
	   2). acquire object by acquire function given by user.
	   3). increase the object refcnt.

	5. Put the object by calling raw_refcnt_put().
	   1). decrease the refcnt number.
	   2). call auto_release to release object resource if object is released and refcnt is zero.

	5. Release the object by calling raw_refcnt_release();
	   1). Set refcnt release bit and decrease refcnt.
	   2). if refcnt is zero, call auto_relase to release object resource.


	------------------
	raw scoped pointer
	------------------

	A Raw Scoped Pointer(scopedptr) is used to manage a pointer life-circle by reference count.
	Just like Scopedptr in C++.

	Usage:
	1. Use `declare_scopeptrn` to define the raw_scopedptr types and functions.

	2. Use `acquire_scopedptr` to acquire object ptr, and will release it atomatically
	   after the scopedptr(stack variable) is deleted.

	3. Example: see sysmgr/process.h

 */

#define REFCOUNT_RELEASE_BIT  30
#define REFCOUNT_COUNTER_MAX  0x3fffffff
#define REFCOUNT_COUNTER_MASK 0x3fffffffU

static inline bool
raw_refcnt_is_released(int val)
{
	return (((unsigned int)val & (1U << REFCOUNT_RELEASE_BIT)) == 0U) ? false : true;
}

/* raw refcnt no release-function */
struct raw_refcnt_nr {
	raw_atomic_int_t count;
};

static inline void
raw_refcnt_nr_init(struct raw_refcnt_nr *ref_nr, int count)
{
	/* mask release bit for raw_refcnt_nr */
	raw_atomic_int_init(&ref_nr->count,
			    (int)((unsigned int)count & REFCOUNT_COUNTER_MASK));
}

static inline int
raw_refcnt_nr_counter(struct raw_refcnt_nr *ref_nr)
{
	return (int)((unsigned int)raw_atomic_int_read(&ref_nr->count) &
		     REFCOUNT_COUNTER_MASK);
}

static inline __always_inline int
raw_refcnt_nr_get(struct raw_refcnt_nr *ref_nr)
{
	int old, ok;

	while (1) {
		old = raw_atomic_int_read(&ref_nr->count);
		if (raw_refcnt_is_released(old)) {
			__hmattr_sideff_resolved("no relative side effect");
			return E_HM_POSIX_NOENT;
		}

		/* check if counter overflow */
		if (old >= REFCOUNT_COUNTER_MAX) {
			__hmattr_sideff_resolved("no relative side effect");
			return E_HM_OVERFLOW;
		}

		/* ok, counter is not overflow, old+1 is safe for new value */
		ok = raw_atomic_int_cas(&ref_nr->count, old, old + 1);
		if (ok == old && ok >= 0) {
			/* return new counter */
			__hmattr_sideff_resolved("no relative side effect");
			return old + 1;
		}
	}
	__builtin_unreachable();
}

static inline __always_inline int
raw_refcnt_nr_inc(struct raw_refcnt_nr *ref_nr)
{
	int new_val;

	new_val = raw_atomic_int_add_relaxed(&ref_nr->count, 1);
	/* check if counter overflow */
	if (new_val >= REFCOUNT_COUNTER_MAX) {
		raw_atomic_int_dec_relaxed(&ref_nr->count);
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_OVERFLOW;
	}

	return new_val;
}

static inline __always_inline int
raw_refcnt_nr_release(struct raw_refcnt_nr *ref_nr)
{
	int old, new_val;
	while (1) {
		old = raw_atomic_int_read(&ref_nr->count);
		/*
		 * if refcnt was released, just return.
		 * else set release bit, and refcnt-1.
		 */
		if (raw_refcnt_is_released(old)) {
			__hmattr_sideff_resolved("no relative side effect");
			return E_HM_POSIX_NOENT;
		}
		new_val = (int)((unsigned int)(old - 1) | (1U << REFCOUNT_RELEASE_BIT));
		int ok = raw_atomic_int_cas(&ref_nr->count, old, new_val);
		if ((ok == old) && (ok >= 0)) {
			/* return the new count of the refcnt */
			__hmattr_sideff_resolved("no relative side effect");
			return old - 1;
		}
	}
	__builtin_unreachable();
}

static inline __always_inline int
raw_refcnt_nr_put(struct raw_refcnt_nr *ref_nr)
{
	int old, cnt, new_val;
	while (1) {
		old = raw_atomic_int_read(&ref_nr->count);
		cnt = (int)((unsigned int)old & REFCOUNT_COUNTER_MASK);
		/* if cnt==1, release bit should be set */
		if (cnt == 1) {
			new_val = (int)(1U << REFCOUNT_RELEASE_BIT);
		} else {
			new_val = old - 1;
		}

		int ok = raw_atomic_int_cas(&ref_nr->count, old, new_val);
		if (ok == old && ok >= 1) {
			/* return the new value of the refcnt, should mask release bit */
			__hmattr_sideff_resolved("no relative side effect");
			return cnt - 1;
		}
	}
	__builtin_unreachable();
}

static inline __always_inline int
raw_refcnt_nr_put_val(struct raw_refcnt_nr *ref_nr, int val)
{
	int old, cnt, new_val;

	while (true) {
		old = raw_atomic_int_read(&ref_nr->count);
		cnt = (int)((unsigned int)old & REFCOUNT_COUNTER_MASK);

		if (cnt < val) {
			return E_HM_POSIX_NOENT;
		}

		if (cnt == val) {
			new_val = (int)(1U << REFCOUNT_RELEASE_BIT);
		} else {
			new_val = old - val;
		}

		int ok = raw_atomic_int_cas(&ref_nr->count, old, new_val);
		if (ok == old && ok >= 1) {
			/* return the new value of the refcnt, should mask release bit */
			__hmattr_sideff_resolved("no relative side effect");
			return cnt - val;
		}
	}
	__builtin_unreachable();
}

static inline __always_inline int
raw_refcnt_nr_sub(struct raw_refcnt_nr *ref_nr, int val)
{
	int old, cnt, new_val;

	while (true) {
		old = raw_atomic_int_read(&ref_nr->count);
		cnt = (int)((unsigned int)old & REFCOUNT_COUNTER_MASK);

		if (cnt <= val) {
			return E_HM_POSIX_NOENT;
		}

		new_val = old - val;

		int ok = raw_atomic_int_cas(&ref_nr->count, old, new_val);
		if (ok == old && ok >= 1) {
			/* return the new value of the refcnt, should mask release bit */
			__hmattr_sideff_resolved("no relative side effect");
			return cnt - val;
		}
	}
	__builtin_unreachable();
}

static inline __always_inline int
raw_refcnt_nr_add(struct raw_refcnt_nr *ref_nr, int val)
{
	int old, ok;

	while (true) {
		old = raw_atomic_int_read(&ref_nr->count);
		if (raw_refcnt_is_released(old)) {
			return E_HM_POSIX_NOENT;
		}

		/* check if counter overflow */
		if (old + val > REFCOUNT_COUNTER_MAX) {
			return E_HM_OVERFLOW;
		}

		/* ok, counter is not overflow, old+val is safe for new value */
		ok = raw_atomic_int_cas(&ref_nr->count, old, old + val);
		if (ok == old && ok >= 0) {
			/* return new counter */
			__hmattr_sideff_resolved("no relative side effect");
			return old + val;
		}
	}
	__builtin_unreachable();
}

struct raw_refcnt;
typedef void (*release_func)(struct raw_refcnt *);

struct raw_refcnt {
	struct raw_refcnt_nr refcnt_nr;
	release_func release;
};

#define __RAW_REFCNT_INITIALIZER(n) {.refcnt_nr = {.count = RAW_ATOMIC_VAL_INIT(n)}, .release = NULL}
#define __RAW_REFCNT_NR_INITIALIZER(n) {.count = RAW_ATOMIC_VAL_INIT(n)}

static inline void
raw_refcnt_init(struct raw_refcnt *ref,
		int count,
		release_func func)
{
	ref->release = func;
	raw_refcnt_nr_init(&ref->refcnt_nr, count);
}

static inline int
raw_refcnt_counter(struct raw_refcnt *ref)
{
	return raw_refcnt_nr_counter(&ref->refcnt_nr);
}

static inline int
raw_refcnt_get(struct raw_refcnt *ref)
{
	return raw_refcnt_nr_get(&ref->refcnt_nr);
}

static inline int
raw_refcnt_release(struct raw_refcnt *ref)
{
	int cnt = raw_refcnt_nr_release(&ref->refcnt_nr);
	/* release the object */
	if ((cnt == 0) && (ref->release != NULL)) {
		ref->release(ref);
	}
	return cnt;
}

static inline int
raw_refcnt_put(struct raw_refcnt *ref)
{
	int cnt = raw_refcnt_nr_put(&ref->refcnt_nr);
	if ((cnt == 0) && (ref->release != NULL)) {
		ref->release(ref);
	}
	return cnt;
}

#define __scopedptr_map0(n, m, ...)
#define __scopedptr_map1(n, m, t, a, ...) m(n - 1, t, a)
#define __scopedptr_map2(n, m, t, a, ...) m(n - 2, t, a), __scopedptr_map1(n, m, __VA_ARGS__)
#define __scopedptr_map3(n, m, t, a, ...) m(n - 3, t, a), __scopedptr_map2(n, m, __VA_ARGS__)
#define __scopedptr_map4(n, m, t, a, ...) m(n - 4, t, a), __scopedptr_map3(n, m, __VA_ARGS__)
#define __scopedptr_map5(n, m, t, a, ...) m(n - 5, t, a), __scopedptr_map4(n, m, __VA_ARGS__)
#define __scopedptr_map6(n, m, t, a, ...) m(n - 6, t, a), __scopedptr_map5(n, m, __VA_ARGS__)
#define __scopedptr_map7(n, m, t, a, ...) m(n - 7, t, a), __scopedptr_map6(n, m, __VA_ARGS__)
#define __scopedptr_map8(n, m, t, a, ...) m(n - 8, t, a), __scopedptr_map7(n, m, __VA_ARGS__)
#define __scopedptr_map(n, ...) __scopedptr_map##n(n, __VA_ARGS__)

#define __scopedptr_def(n, t, a) t a
#define __scopedptr_call(n, t, a) a

#define define_acquire_ptr_refx(x, name, offset, lock_guard, acquire, ...) \
	static inline void* acquire_##name##_ptr_ref(__scopedptr_map(x, __scopedptr_def, __VA_ARGS__)) \
	{ \
		lock_guard; \
		void *ptr = acquire(__scopedptr_map(x, __scopedptr_call, __VA_ARGS__)); \
		if (ptr != NULL) { \
			struct raw_refcnt *ref = (struct raw_refcnt *)(uintptr_t)((char*)ptr + (offset)); \
			if (raw_refcnt_get(ref) <= 0) { \
				ptr = NULL; \
			} \
		} \
		return ptr;\
	}

#define define_refcnt_functionx(x, name, offset, lock_guard, acquire, ...) \
	define_acquire_ptr_refx(x, name, offset, lock_guard, acquire, __VA_ARGS__) \

#define define_refcnt_function1(name, offset, lock_guard, acquire, ...) \
	define_refcnt_functionx(1, name, offset, lock_guard, acquire, __VA_ARGS__)
#define define_refcnt_function2(name, offset, lock_guard, acquire, ...) \
	define_refcnt_functionx(2, name, offset, lock_guard, acquire, __VA_ARGS__)
#define define_refcnt_function3(name, offset, lock_guard, acquire, ...) \
	define_refcnt_functionx(3, name, offset, lock_guard, acquire, __VA_ARGS__)
#define define_refcnt_function4(name, offset, lock_guard, acquire, ...) \
	define_refcnt_functionx(4, name, offset, lock_guard, acquire, __VA_ARGS__)
#define define_refcnt_function5(name, offset, lock_guard, acquire, ...) \
	define_refcnt_functionx(5, name, offset, lock_guard, acquire, __VA_ARGS__)
#define define_refcnt_function6(name, offset, lock_guard, acquire, ...) \
	define_refcnt_functionx(6, name, offset, lock_guard, acquire, __VA_ARGS__)


#define declare_scopedptr_type(name) \
	struct raw_##name##_scopedptr { \
		struct raw_refcnt *ref; \
	};

#define SCOPEDPTR_INITIALIZER {.ref = NULL}
#define scopedptr_init(name, _ref) \
	{\
		(name)->ref = (struct raw_refcnt *)(uintptr_t)(_ref); \
	}

#define define_scopedptr_release(name)\
	static inline void raw_##name##_scopedptr_auto_release(struct raw_##name##_scopedptr *scopedptr) \
	{\
		if (scopedptr->ref != NULL) { \
			(void)raw_refcnt_put(scopedptr->ref);\
		} \
	}

/*
 *  name:       the name we protected
 *  offset:     offset is the ptr to struct raw_refcnt in object
 *  lock_guard: Lock when we try to acquire object pointer
 *  acquire:    acquire function of the object pointer
 */
#define declare_scopeptrx(x, name, offset, lock_guard, acquire, ...)\
	declare_scopedptr_type(name)\
	define_refcnt_functionx(x, name, offset, lock_guard, acquire, __VA_ARGS__) \
	define_scopedptr_release(name)

#define declare_scopeptr6(name, offset, lock_guard, acquire, ...) \
	declare_scopeptrx(6, name, offset, lock_guard, acquire,  __VA_ARGS__)
#define declare_scopeptr5(name, offset, lock_guard, acquire, ...) \
	declare_scopeptrx(5, name, offset, lock_guard, acquire,  __VA_ARGS__)
#define declare_scopeptr4(name, offset, lock_guard, acquire, ...) \
	declare_scopeptrx(4, name, offset, lock_guard, acquire, __VA_ARGS__)
#define declare_scopeptr3(name, offset, lock_guard, acquire, ...) \
	declare_scopeptrx(3, name, offset, lock_guard, acquire, __VA_ARGS__)
#define declare_scopeptr2(name, offset, lock_guard, acquire, ...) \
	declare_scopeptrx(2, name, offset, lock_guard, acquire, __VA_ARGS__)
#define declare_scopeptr1(name, offset, lock_guard, acquire, ...) \
	declare_scopeptrx(1, name, offset, lock_guard, acquire, __VA_ARGS__)


#define acquire_scopedptr(name, ptr_name, offset, ...) \
	ptr_name = acquire_##name##_ptr_ref(__VA_ARGS__); \
	struct raw_##name##_scopedptr __scopedptr_to_##ptr_name \
		__attribute__((cleanup(raw_##name##_scopedptr_auto_release))) = SCOPEDPTR_INITIALIZER; \
	scopedptr_init(&__scopedptr_to_##ptr_name, (ptr_name != NULL) ? (char*)((uintptr_t)(ptr_name) + (offset)) : NULL);


#ifdef __cplusplus
}
#endif

#endif
