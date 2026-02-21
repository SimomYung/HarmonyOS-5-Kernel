/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Tsan common struct define
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 25 16:14:53 2022
 */

#ifndef TSAN_UTILS_H
#define TSAN_UTILS_H

#include "tsan_algo.h"

#define DELAY_TIME_MAX (30u)
#define CONSECUTIVE_LIMIT_MAX (30u)

/*
 * Watchpoint basic datatype, consisting of 3 major parts:
 * 1. short info: caller id, rw, mem access size, etc;
 * 2. pc: the pc where memory access is launched;
 * 3. addr: the memory addr accessed
 */
struct wp_s {
	struct {
		unsigned int is_write: 2;
		size_t size: 4;
		unsigned int id: 32;
	} compact_info;
	unsigned long pc;
	unsigned long addr;
};

/*
 * There can be many watchpoints in the thread-sanitized target. For management of a
 * watchpoint, we use the datatype watchpoint slot.
 */
struct wp_slot_s {
	struct wp_s wp;

	/*
	 * initial generation: 0
	 * Here is the trick:
	 *   generation increases monotonically, and its valud % 4 has special meaning:
	 *   0: The slot is free;
	 *   1: The slot is in-use, but not filled;
	 *   2: The slot is in-use, occupied and filled;
	 *   3: Not used.
	 */
	uext_atomic64_t generation;

	/* Performance hack:to limit the times that a single thread consecutively acquires the slot */
	unsigned int install_avail;
};

#define WP_READ	0
#define WP_WRITE	1
#define WP_UNKNOWN	2

#define INC_GEN 1               /* The gen number increased for normal usage */
#define INC_GEN_RELEASE 2       /* The gen number increased at the release of the slot */
#define GEN_SIZE 0x4

#define gen_is_free(gen) (((gen) & 0x3) == 0)
#define gen_is_unhit(gen) (((gen) & 0x3) == 2)
#define gen_is_hit(gen) (((gen) & 0x3) == 3)
#define gen_is_filled(gen) (gen_is_unhit(gen) || gen_is_hit(gen))
#define gen_base_of(gen) (0xFFFFFFFFFFFFFFFC & (gen))

/*
 * Size of cached records of found race, for suppressing repeated race report.
 * Default cache size: 100.
 */
#ifndef CONFIG_TSAN_RECORD_SIZE
#define CONFIG_TSAN_RECORD_SIZE 100
#endif

struct race_entry_s {
	unsigned long pc_a;
	unsigned long pc_b;
};

/*
 * tsan manager holds main management info of tsan.
 * NOTE:
 *    Temporarily only one slot is provided. Multiple slots will be enabled in future version.
 */
struct tsan_mgr_s {
	struct wp_slot_s wp_slots[1];

	struct race_record_s {
		unsigned int all_cnt;
		unsigned int used;
		struct race_entry_s race[CONFIG_TSAN_RECORD_SIZE];
	} race_records;

	/*
	 * Race will be reported if report_lock is acquired, otherwise
	 * it will be bufferd. As long as a thread acquires report_lock, it will print
	 * all buffered lock altogether.
	 * The reason for buffer_lock is that report will call extern function, who may
	 * acquire further locks, leading to system dead-lock. For example:
	 *     t0:   tsan --> report_lock(held) --> lock_vtimekeep(spin-waiting)
	 *     t1:   lock_vtimekeep(held) --> tsan --> report_lock(spin-waiting)
	 * Our solution is that report_lock is acquired by try_lock instead of spin-waiting,
     * and if failed, race will be stored in a buffer.
	 */
	uext_lock_t race_report_lock;
};

#endif
