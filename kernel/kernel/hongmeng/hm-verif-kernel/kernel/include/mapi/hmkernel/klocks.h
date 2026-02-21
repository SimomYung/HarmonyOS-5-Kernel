/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Kernel subsystem locks
 * Author: Huawei OS Kernel Lab
 * Create: Aug 05 11:22 2020
 */

#ifndef MAPI_HMKERNEL_KLOCKS_H
#define MAPI_HMKERNEL_KLOCKS_H

#include <hmkernel/reclock.h>
#include <hmkernel/rwlock.h>
#include <hmkernel/spinlock.h>
#include <hmkernel/ticketlock.h>
#include <hmkernel/rec_ticketlock.h>

#define lock_def(lockname) \
	LOCK_##lockname,
#define kspinlock_def(lockname) \
	LOCK_##lockname,
#define kticketlock_def(lockname) \
	LOCK_##lockname,
#define ticketlock_def(lockname) \
	LOCK_##lockname,
#define rwlock_def(lockname) \
	LOCK_##lockname,
#define lock_order(...)
enum klock_types {
	LOCK_TYPE_INVAL,
#include <hmkernel/klocks/klocks_def.h>
	LOCK_TYPE_END
};
#define LOCK_MAX_BITS 128
static_assert(LOCK_TYPE_END <= LOCK_MAX_BITS, lock_types_too_many);
#undef lock_def
#undef kspinlock_def
#undef kticketlock_def
#undef ticketlock_def
#undef rwlock_def
#undef lock_order

#if defined(CONFIG_SMP) && !defined(CONFIG_BKL)
#define lock_def(lockname) \
	void lock_##lockname(void); \
	void unlock_##lockname(void); \
	bool trylock_##lockname(void);
#define kspinlock_def(lockname) \
	void lock_##lockname(void); \
	void unlock_##lockname(void); \
	bool trylock_##lockname(void);
#define kticketlock_def(lockname) \
	void lock_##lockname(void); \
	void unlock_##lockname(void); \
	bool trylock_##lockname(void);
#define ticketlock_def(lockname) \
	void lock_##lockname(void); \
	void unlock_##lockname(void); \
	bool trylock_##lockname(void);
#define rwlock_def(lockname) \
	void read_lock_##lockname(void); \
	void read_unlock_##lockname(void); \
	bool read_trylock_##lockname(void); \
	void write_lock_##lockname(void); \
	void write_unlock_##lockname(void); \
	bool write_trylock_##lockname(void);
#define lock_order(n, lockname, ...)
#include <hmkernel/klocks/klocks_def.h>
#undef lock_def
#undef kspinlock_def
#undef kticketlock_def
#undef ticketlock_def
#undef rwlock_def
#undef lock_order
#else
#define lock_def(lockname) \
	static inline void lock_##lockname(void) {} \
	static inline void unlock_##lockname(void) {} \
	static inline bool trylock_##lockname(void) { return __true; }
#define kspinlock_def(lockname) \
	static inline void lock_##lockname(void) {} \
	static inline void unlock_##lockname(void) {} \
	static inline bool trylock_##lockname(void) { return __true; }
#define kticketlock_def(lockname) \
	static inline void lock_##lockname(void) {} \
	static inline void unlock_##lockname(void) {} \
	static inline bool trylock_##lockname(void) { return __true; }
#define ticketlock_def(lockname) \
	static inline void lock_##lockname(void) {} \
	static inline void unlock_##lockname(void) {} \
	static inline bool trylock_##lockname(void) { return __true; }
#define rwlock_def(lockname) \
	static inline void read_lock_##lockname(void) {} \
	static inline void read_unlock_##lockname(void) {} \
	static inline bool read_trylock_##lockname(void) { return __true; } \
	static inline void write_lock_##lockname(void) {} \
	static inline void write_unlock_##lockname(void) {} \
	static inline bool write_trylock_##lockname(void) { return __true; }
#define lock_order(n, lockname, ...)
#include <hmkernel/klocks/klocks_def.h>
#undef lock_def
#undef kspinlock_def
#undef kticketlock_def
#undef ticketlock_def
#undef rwlock_def
#undef lock_order
#endif

#endif /* !MAPI_HMKERNEL_KLOCKS_H */
