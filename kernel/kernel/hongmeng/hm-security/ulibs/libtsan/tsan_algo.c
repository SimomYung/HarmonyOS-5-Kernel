/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: The soft watchpoint and delay based thread sanitizer, named tsan
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 25 16:14:53 2022
 */

/*
 * Software Implementation of Watchpoint & Delay-based Data Dace Detection
 * Ref: https://www.usenix.org/legacy/event/osdi10/tech/full_papers/Erickson.pdf
 *
 * NOTE: For now, the following customed check function only applies to GNU GCC.
 */
#ifdef __GNUC__

#include <dlfcn.h>
#include <libtsan/tsan.h>
#include <tsanhook.h>
#include "macro.h"
/*
 * tsan_algo.c is the abstract common process for tsan, a watchpoint-like(not really
 * hardware watchpoint) race-detection.
 * For complete compilation and usage, a tsan_algo.h is required, where the following
 * infomation should be provided:
 *
 * 1. Type definitions of needed data types:
 *    uext_lock_t, uint8_t/uint16_t/uint32_t/uint64_t, uext_atomic64_t
 *
 * 2. Implementation of tsan-relied external function:
 *        uext_thread_get_id, uext_log, uext_delay,
 *        uext_lock, uext_try_lock, uext_unlock, uext_lock_init,
 *        uext_atomic64_read, uext_atomic64_cond_xchg, uext_atomic64_add
 *
 *     Functions and data types provided by tsan_algo.h precedes with prefix "uext" (tsan external)
 */
#include "tsan_algo.h"
#include "tsan_utils.h"
#include "tsan_report.h"

#ifdef __HOST_LLT__
#define STATIC
#else
#define STATIC static
#endif

STATIC bool tsan_inited = false;
STATIC bool tsan_enabled = false;

/* Rounds of delay for a watchpoint */
STATIC unsigned int delay_time = 1u;

/* Limit for a id consecutively installing the wp */
STATIC unsigned int consecutive_limit = 8u;

STATIC struct tsan_mgr_s tsan_mgr = {0};

/*
 * Make a watchpoint for check, provided with  necessary info.
 */
STATIC struct wp_s
wp_make(unsigned long addr, size_t size, unsigned int id, int is_write, unsigned long pc)
{
	struct wp_s wp = {
		.compact_info = {
			.size = size,
			.id = id,
			.is_write = is_write,
		},
		.addr = addr,
		.pc = pc,
	};

	return wp;
}

STATIC unsigned long
wp_addr_of(struct wp_s wp)
{
	return wp.addr;
}

STATIC unsigned long
wp_pc_of(struct wp_s wp)
{
	return wp.pc;
}

STATIC size_t
wp_size_of(struct wp_s wp)
{
	return wp.compact_info.size;
}

STATIC unsigned int
wp_id_of(struct wp_s wp)
{
	return wp.compact_info.id;
}

STATIC bool
wp_is_read(struct wp_s wp)
{
	return wp.compact_info.is_write == WP_READ;
}

STATIC bool
wp_is_write(struct wp_s wp)
{
	return wp.compact_info.is_write == WP_WRITE;
}

STATIC bool
wp_match(struct wp_s wp_a, struct wp_s wp_b)
{
	unsigned long start_a = wp_addr_of(wp_a);
	unsigned long start_b = wp_addr_of(wp_b);
	unsigned long end_a = start_a + wp_size_of(wp_a);
	unsigned long end_b = start_b + wp_size_of(wp_b);
	if (end_a < start_a || end_b < start_b) {
		return false;
	}
	return (start_a >= start_b && start_a < end_b) ||
	       (start_b >= start_a && start_b < end_a);
}

/*
 * Try to "atomically" read the watchpoint's content from a slot. It is "atomic" because
 * we use a generation for consistency check.
 *  Return:
 *    1. true on success, that is, the watchpoint is consistent during read and it is
 *      in in-use and filled state;
 *    2. false otherwise.
 */
STATIC bool
slot_read(struct wp_slot_s *slot, struct wp_s *wp, uint64_t *gen)
{
	uint64_t gen0, gen1;
	bool suc = false;
	gen0 = uext_atomic64_read(&slot->generation);
	*wp = slot->wp;
	gen1 = uext_atomic64_read(&slot->generation);
	if (gen0 == gen1) {
		suc = gen_is_filled(gen0);
	} else {
		suc = (gen_is_unhit(gen0) && (gen1 == gen0 + INC_GEN));
	}
	*gen = gen0;
	return suc;
}

/*
 * Try to acqurie access of the slot.
 * Return:
 *   1. true on success, meaning the slot is exclusively occupied by the caller;
 *   2. false on failure.
 */
STATIC bool
slot_try_acquire(struct wp_slot_s *slot, unsigned int new_id)
{
	bool suc = false;
	uint64_t gen = uext_atomic64_read(&slot->generation);
	if (gen_is_free(gen) && \
	    (new_id != wp_id_of(slot->wp) || slot->install_avail > 0)) {
		suc = uext_atomic64_cond_xchg(&slot->generation, gen, gen + INC_GEN);
	}

	if (suc) {
		if (wp_id_of(slot->wp) != new_id) {
			/*
			 * A new thread acquires the watchpoint, reset the consecutive
			 * set limit.
			 */
			slot->install_avail = consecutive_limit;
		} else {
			/*
			 * The previous thread again acquires the slot , decrease its
			 * availability.
			 */
			slot->install_avail--;
		}
	}

	return suc;
}

STATIC void
slot_fill(struct wp_slot_s *slot, struct wp_s wp)
{
	slot->wp = wp;
	uext_atomic64_add(&slot->generation, INC_GEN);
}

/*
 * Release the exclusive occupation of a slot.
 * The generation should be v % 4 == 2 before release, we add 2 to make it
 * order of 2 for performance.
 */
STATIC void
slot_release(struct wp_slot_s *slot)
{
	uint64_t gen = uext_atomic64_read(&slot->generation);
	gen = gen_base_of(gen) + GEN_SIZE;
	uext_atomic64_write(&slot->generation, gen);
}

STATIC struct race_entry_s
race_entry_make(struct wp_s wp_a, struct wp_s wp_b)
{
	unsigned long pc_a, pc_b;

	/* make in order: wp_a.pc > wp_a.pc */
	if (wp_pc_of(wp_a) > wp_pc_of(wp_b)) {
		pc_a = wp_pc_of(wp_a);
		pc_b = wp_pc_of(wp_b);
	} else {
		pc_a = wp_pc_of(wp_b);
		pc_b = wp_pc_of(wp_a);
	}

	struct race_entry_s re = {
		.pc_a = pc_a,
		.pc_b = pc_b,
	};

	return re;
}

STATIC bool
race_entry_match(struct race_entry_s r0, struct race_entry_s r1)
{
	return r0.pc_a == r1.pc_a && r0.pc_b == r1.pc_b;
}

STATIC struct wp_slot_s *
tsan_mgr_slot_of(int idx)
{
	return &tsan_mgr.wp_slots[idx];
}

/*
 * Choose a slot for the new watchpoint
 * Return: the chosen slot
 * Note: for now, we have only 1 watchpoint, thus always use slot 0
 */
STATIC struct wp_slot_s *
tsan_mgr_choose_slot(struct wp_s wp)
{
	UNUSED(wp);
	return tsan_mgr_slot_of(0);
}

STATIC bool
tsan_mgr_lookup_race(struct race_entry_s re)
{
	unsigned int i;
	bool found = false;
	for (i = 0; i < tsan_mgr.race_records.used; i++) {
		if (race_entry_match(re, tsan_mgr.race_records.race[i])) {
			found = true;
			break;
		}
	}
	return found;
}

/* Must hold report lock */
STATIC void
tsan_mgr_record_race(struct race_entry_s re)
{
	if (tsan_mgr.race_records.used < CONFIG_TSAN_RECORD_SIZE) {
		tsan_mgr.race_records.race[tsan_mgr.race_records.used++] = re;
	}
}

STATIC void __attribute__((noinline))
tsan_race_report_locked(struct wp_s wp_a, struct wp_s wp_b, bool trace)
{
	struct race_entry_s re = race_entry_make(wp_a, wp_b);
	if (!tsan_mgr_lookup_race(re)) {
		tsan_mgr_record_race(re);
		tsan_disable();
		uext_log("\n................. DATA RACE................\n\n"
			 "RaceInfo: idx: %u, addr: 0x%lx, size: %d, tid0: %u, "
			 "pc0: 0x%lx, tid1: %u, pc1: 0x%lx, mode: %c%c \n\n"
			 "..............................................\n\n",
			 tsan_mgr.race_records.all_cnt++,
			 wp_addr_of(wp_a), (int)wp_size_of(wp_a),
			 wp_id_of(wp_a), wp_pc_of(wp_a), wp_id_of(wp_b), wp_pc_of(wp_b),
			 wp_is_write(wp_a) ? 'w' : (wp_is_read(wp_a) ? 'r' : '?'),
			 wp_is_write(wp_b) ? 'w' : (wp_is_read(wp_b) ? 'r' : '?'));
		if(trace) {
			tsan_handle_race();
		}
		tsan_enable();
	}
}

STATIC void __attribute__((noinline))
tsan_race_report(struct wp_s wp_a, struct wp_s wp_b)
{
	uext_lock(&tsan_mgr.race_report_lock);
	tsan_race_report_locked(wp_a, wp_b, true);
	uext_unlock(&tsan_mgr.race_report_lock);
}

STATIC uint64_t
__read_v(unsigned long addr, size_t size)
{
	uint64_t v = 0;
	switch (size) {
	case sizeof(uint8_t):
		v = *(uint8_t*)(uintptr_t)addr;
		break;
	case sizeof(uint16_t):
		v = *(uint16_t*)(uintptr_t)addr;
		break;
	case sizeof(uint32_t):
		v = *(uint32_t*)(uintptr_t)addr;
		break;
	case sizeof(uint64_t):
		v = *(uint64_t*)(uintptr_t)addr;
		break;
	default:
		break;
	}
	return v;
}

STATIC void
__delay_and_detect(struct wp_slot_s *slot, struct wp_s wp)
{
	uint64_t v_old, v_new;
	unsigned long addr = wp_addr_of(wp);
	size_t size = wp_size_of(wp);

	v_old = __read_v(addr, size);
	uext_delay(delay_time);
	v_new = __read_v(addr, size);
	if (v_old != v_new || gen_is_hit(uext_atomic64_read(&slot->generation))) {
		int is_write = WP_WRITE;
		if (wp_is_write(wp) && (v_old == v_new)) {
			is_write = WP_UNKNOWN;
		}
		struct wp_s tmp = wp_make(addr, size, 0, is_write, 0);
		tsan_race_report(wp, tmp);
	}
}

/*
 * Try installing watchpoint and delay, detect race.
 */
STATIC void
tsan_check_by_setup(struct wp_s new)
{
	unsigned int id = wp_id_of(new);
	struct wp_slot_s *slot = tsan_mgr_choose_slot(new);
	if (slot_try_acquire(slot, id)) {
		slot_fill(slot, new);
		__delay_and_detect(slot, new);
		slot_release(slot);
	}
}

/*
 * Try to find a match from installed watchpoint
 */
STATIC int
tsan_check_by_match(struct wp_s new)
{
	int ret = 0;
	struct wp_s match;
	uint64_t gen0, gen1;
	struct wp_slot_s *slot = tsan_mgr_slot_of(0);

	/* Temporarily we have only 1 watchpoint slot, so hardcoded idx: 0. */
	if (slot_read(slot, &match, &gen0)) {
		if (wp_match(new, match) &&
		    (wp_is_write(new) || wp_is_write(match))) {
			gen1 = uext_atomic64_read(&slot->generation);
			if (gen_is_unhit(gen1) &&
			    gen_base_of(gen1) == gen_base_of(gen0)) {
				(void)uext_atomic64_cond_xchg(&slot->generation, gen1, gen1 + INC_GEN);
			}
			tsan_race_report(new, match);
		}
		ret = 1;
	}

	return ret;
}

/*
 * Order of checks on a new memory access:
 *   Check.1:
 *       check by match. That is, find if there is already a matched watchpoint, report the race if necessary;
 *   Check.2 (conditional):
 *       If Check.1 returns 0, continue to check by setup, that is, install
 *       the watchpoint and delay for a while for detection. Otherwise check is over.
 */
STATIC void
tsan_mem_access(unsigned long addr, size_t size, int is_write, unsigned long pc)
{
	struct wp_s new;
	if (tsan_enabled) {
		int tid = uext_thread_get_id();
		if (tid != TLS_UNREADY) {
			new = wp_make(addr, size, (unsigned int)tid, is_write, pc);
			if (tsan_check_by_match(new) == 0) {
				tsan_check_by_setup(new);
			}
		}
	}
}

/*
 * * * * * * * * * * * * *
 * Following functions are intended for overwritting gcc's built-in
 * this_write sanitizer's check function.
 * * * * * * * * * * * * *
 */

/*
 * The inner func is registered to libc to avoid global symbol ovverride
 * Because libc has the same func named __tsan_...
 */
STATIC void __inner_tsan_init(void)
{
}
void __tsan_init(void)
{
}

/* NOTE: We don't use func entry/exit info temporarily */
STATIC void __inner_tsan_func_entry(unsigned long pc)
{
	UNUSED(pc);
}
void __tsan_func_entry(unsigned long pc)
{
	UNUSED(pc);
}

STATIC void __inner_tsan_func_exit(void)
{
}
void __tsan_func_exit(void)
{
}

/* NOTE: Enable range R/W in future... */
STATIC void __inner_tsan_read_range(unsigned long addr, unsigned long size)
{
	UNUSED(addr);
	UNUSED(size);
}
void __tsan_read_range(unsigned long addr, unsigned long size)
{
	UNUSED(addr);
	UNUSED(size);
}

STATIC void __inner_tsan_write_range(unsigned long addr, unsigned long size)
{
	UNUSED(addr);
	UNUSED(size);
}
void __tsan_write_range(unsigned long addr, unsigned long size)
{
	UNUSED(addr);
	UNUSED(size);
}

#define _RET_IP_ ((uintptr_t)__builtin_return_address(0))

#define DEF_MEM_ACCESS_CHECK(size)			\
STATIC void __inner_tsan_read##size(unsigned long addr, unsigned long pc)  \
{							\
	tsan_mem_access(addr, size, 0, pc);		\
}							\
void __tsan_read##size(unsigned long addr)		\
{							\
	__inner_tsan_read##size(addr, _RET_IP_);	\
}							\
STATIC void __inner_tsan_unaligned_read##size(unsigned long addr, unsigned long pc)  \
{							\
	tsan_mem_access(addr, size, 0, pc);		\
}							\
void __tsan_unaligned_read##size(unsigned long addr)	\
{							\
	__inner_tsan_unaligned_read##size(addr, _RET_IP_);	\
}							\
STATIC void __inner_tsan_read##size##_pc(unsigned long addr, unsigned long pc)	\
{							\
	tsan_mem_access(addr, size, 0, pc);		\
}							\
void __tsan_read##size##_pc(unsigned long addr, unsigned long pc)	\
{							\
	__inner_tsan_read##size##_pc(addr, pc);		\
}							\
STATIC void __inner_tsan_write##size(unsigned long addr, unsigned long pc)  \
{							\
	tsan_mem_access(addr, size, 1, pc);		\
}							\
void __tsan_write##size(unsigned long addr)		\
{							\
	__inner_tsan_write##size(addr, _RET_IP_);	\
}							\
STATIC void __inner_tsan_unaligned_write##size(unsigned long addr, unsigned long pc)  \
{							\
	tsan_mem_access(addr, size, 1, pc);		\
}							\
void __tsan_unaligned_write##size(unsigned long addr)	\
{							\
	__inner_tsan_unaligned_write##size(addr, _RET_IP_);	\
}							\
STATIC void __inner_tsan_write##size##_pc(unsigned long addr, unsigned long pc)	\
{							\
	tsan_mem_access(addr, size, 1, pc);		\
}							\
void __tsan_write##size##_pc(unsigned long addr, unsigned long pc)	\
{							\
	__inner_tsan_write##size##_pc(addr, pc);	\
}

DEF_MEM_ACCESS_CHECK(1)
DEF_MEM_ACCESS_CHECK(2)
DEF_MEM_ACCESS_CHECK(4)
DEF_MEM_ACCESS_CHECK(8)
DEF_MEM_ACCESS_CHECK(16)

#define TSAN_LOADSTORE_ASSIGN(size)					\
	.__tsan_read##size = __inner_tsan_read##size,				\
	.__tsan_unaligned_read##size = __inner_tsan_unaligned_read##size,	\
	.__tsan_read##size##_pc = __inner_tsan_read##size##_pc,			\
	.__tsan_write##size = __inner_tsan_write##size,				\
	.__tsan_unaligned_write##size = __inner_tsan_unaligned_write##size,	\
	.__tsan_write##size##_pc = __inner_tsan_write##size##_pc,

#define WEAK __attribute__((__weak__))
#define HIDDEN __attribute__((__visibility__("hidden")))

/*
 * when tsan is enabled for libc, we need to add tsan_read/write
 * wrap functions in libc, because some libc code will run before
 * dynamic link is done, so libtsan's tsan_read/write functions can't be used.
 * when libtsan is ready, we invode libc's callback function to
 * register our real tsan functions to libc.
 * */

 /*
 * Do not use this function, just for compile
 * If libc tsan is enable:
 *   For static link, symbol will be overrided because of weak
 *   For dynamic link, symbol will be ovverred because of hidden
 * If libc tsan is not enable, there is no compile errors because:
 *   For static link, we have the weak symbol
 *   For dynamic link, dlsym will return null because of hidden
 * */
WEAK HIDDEN void set_tsan_callback(const struct tsan_callback *tc)
{
	UNUSED(tc);
	uext_log("empty callback function in static link\n");
	return;
}

#ifdef TSAN_SHARED
typedef void (*tsan_callback_type)(const struct tsan_callback *);
#endif
STATIC void tsan_callback_init(void)
{
	/* reference to static inner func to avoid global symbol ovverride */
	const struct tsan_callback tc = {
		TSAN_LOADSTORE_ASSIGN(1)
		TSAN_LOADSTORE_ASSIGN(2)
		TSAN_LOADSTORE_ASSIGN(4)
		TSAN_LOADSTORE_ASSIGN(8)
		TSAN_LOADSTORE_ASSIGN(16)
		.__tsan_read_range = __inner_tsan_read_range,
		.__tsan_write_range = __inner_tsan_write_range,
		.__tsan_func_entry = __inner_tsan_func_entry,
		.__tsan_func_exit = __inner_tsan_func_exit,
		.__tsan_init = __inner_tsan_init,
	};
#ifdef TSAN_SHARED
	tsan_callback_type callback_func = dlsym(RTLD_DEFAULT, "set_tsan_callback");
	if (callback_func != NULL) {
		uext_log("find sec tsan callback function in dyn link\n");
		callback_func(&tc);
	}
#else
	set_tsan_callback(&tc);
#endif
}

/*
 * * * * * * * * * * * * *
 * Following are interfaces for user interaction with the tsan.
 * * * * * * * * * * * * *
 */
void tsan_enable(void)
{
	/* disable temporarily because we'll invoke uext_log */
	tsan_enabled = false;

	if (!tsan_inited) {
		tsan_inited = true;
		mem_zero_s(tsan_mgr);
		uext_lock_init(&tsan_mgr.race_report_lock);
		uext_log("======== tsan data race detector enable ========\n");
		tsan_callback_init();
	}
	tsan_enabled = true;
}

void tsan_disable(void)
{
	tsan_enabled = false;
}

bool tsan_set_delay(unsigned int t)
{
	bool ret;
	if (t > 0 && t <= DELAY_TIME_MAX) {
		delay_time = t;
		ret = true;
	} else {
		ret = false;
	}
	return ret;
}

bool tsan_set_consecutive(unsigned int t)
{
	bool ret;
	if (t > 0 && t <= CONSECUTIVE_LIMIT_MAX) {
		consecutive_limit = t;
		ret = true;
	} else {
		ret = false;
	}
	return ret;
}

#else
#error "tsan currently only supports GNU_C"
#endif
