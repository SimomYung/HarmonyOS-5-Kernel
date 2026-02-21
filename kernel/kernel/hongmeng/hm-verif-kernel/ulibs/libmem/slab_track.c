/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Implementation of ulibs slab user track dfx
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 06 21:19:53 2024
 *
 * slab page memory layout:
 * +------------------------------------------------------------------------+
 * | slab desc | redzone | slot | redzone | userzone | ... | redzone | slot | redzone | userzone |
 * +------------------------------------------------------------------------+
 * userzone: store user info, including comm/tid/pid/stack etc.
 * redzone is optional
 */
#include "slab.h"
#include <stdbool.h>
#include <libmem/slab_track.h>
#include <libmem/utils.h>
#include <libmem/debug.h>
#include <libmem/memm_slab.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_mutex_guard.h>
#include <hmkernel/barrier.h>
#include <hongmeng/types.h>
#include <hongmeng/panic.h>

static bool slab_is_userzone_enabled(uint32_t flags)
{
	return !(flags & SLAB_SKIP_DEBUG) && (flags & (SLAB_STORE_USER | SLAB_STORE_STACK));
}

static void *slab_slot_to_userzone(struct slab_ator *s_ator, void *slot)
{
	return (void *)(ptr_to_type(slot, uintptr_t) + s_ator->obj_size - SLAB_DEBUG_SIZE);
}

uint32_t slab_userzone_calc_obj_size(uint32_t size, uint32_t s_flags)
{
	return slab_is_userzone_enabled(s_flags) ? size + SLAB_DEBUG_SIZE : size;
}

#ifdef CONFIG_PER_CPU_SLAB
void set_in_cache_mark(struct slab_ator *s_ator, void *slot)
{
	struct slab_user_info *info = NULL;

	if (s_ator != NULL && slot != NULL && slab_is_userzone_enabled(s_ator->flags)) {
		info = (struct slab_user_info *)slab_slot_to_userzone(s_ator, slot);
		info->in_cache = true;
	}
}
void clear_in_cache_mark(struct slab_ator *s_ator, void *slot)
{
	struct slab_user_info *info = NULL;

	if (s_ator != NULL && slot != NULL && slab_is_userzone_enabled(s_ator->flags)) {
		info = (struct slab_user_info *)slab_slot_to_userzone(s_ator, slot);
		info->in_cache = false;
	}
}

bool is_obj_in_cache(struct slab_ator *s_ator, void *slot)
{
	bool result = false;
	struct slab_user_info *info = NULL;

	if (s_ator != NULL && slot != NULL && slab_is_userzone_enabled(s_ator->flags)) {
		info = (struct slab_user_info *)slab_slot_to_userzone(s_ator, slot);
		if (info->in_cache) {
			result = true;
		}
	}

	return result;
}
#endif

void slab_store_user_info(struct slab_ator *s_ator, void *slot)
{
	int rc = E_HM_OK;
	struct process_s *process = NULL;
	struct slab_user_info *info = NULL;

	if (s_ator == NULL || slot == NULL || !slab_is_userzone_enabled(s_ator->flags)) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		info = (struct slab_user_info *)slab_slot_to_userzone(s_ator, slot);
		process = slab_fill_user_info(info, s_ator->flags);
		process_add_slab_size(process, slab_slot_size(s_ator), s_ator);
#ifdef CONFIG_PER_CPU_SLAB
		barrier();
		clear_in_cache_mark(s_ator, slot);
#endif
	}
}

void slab_clear_user_info(struct slab_ator *s_ator, void *slot, pid_t *rpid)
{
	int rc = E_HM_OK;
	struct slab_user_info *info = NULL;

	if (s_ator == NULL || slot == NULL || !slab_is_userzone_enabled(s_ator->flags)) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
#ifdef CONFIG_PER_CPU_SLAB
		set_in_cache_mark(s_ator, slot);
		barrier();
#endif
		info = (struct slab_user_info *)slab_slot_to_userzone(s_ator, slot);
		if (rpid != NULL) {
			*rpid = info->alloc.pid;
		}
#ifdef CONFIG_KASAN_DUMPSTACK
		(void)slab_fill_user_info(info, s_ator->flags | SLAB_STORE_FREE);
#else
		info->alloc.pid = 0;
		info->alloc.tid = 0;
		NOFAIL(memset_s(info->alloc.comm, SLAB_DEBUG_COMM_LEN, '\0', SLAB_DEBUG_COMM_LEN));
		if (s_ator->flags & SLAB_STORE_STACK) {
			NOFAIL(memset_s(info->alloc.trace, sizeof(info->alloc.trace),
					0, sizeof(info->alloc.trace)));
		}
#endif
	}
}

void slab_free_track(struct track_head *th)
{
	if (th != NULL && th->ti != NULL) {
		free(th->ti);
		th->ti = NULL;
	}
}

static int slab_alloc_track(struct track_head *th, unsigned long max)
{
	int rc = E_HM_OK;
	struct track_info *ti = NULL;

	if (th == NULL || max == 0 || max <= th->max) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		ti = (struct track_info *)malloc(max * sizeof(struct track_info));
		if (ti == NULL) {
			rc = E_HM_NOMEM;
		}
	}

	if (rc == E_HM_OK) {
		if (th->ti != NULL) {
			NOFAIL(memcpy_s(ti, max * sizeof(struct track_info),
			       th->ti, th->items * sizeof(struct track_info)));
			free(th->ti);
		}
		th->max = max;
		th->ti = ti;
	}

	return rc;
}

int slab_init_track(struct track_head *th, unsigned long max, slab_count_func_t count_func)
{
	int rc = E_HM_OK;

	if (th == NULL) {
		rc = E_HM_INVAL;
	} else {
		mem_zero_s(*th);
		th->count_usage = count_func;
		if (max != 0) {
			rc = slab_alloc_track(th, max);
		}
	}

	return rc;
}

static int slab_compare_user(const struct slab_track_info *a, const struct track_info *b)
{
	int result = 0;

	result = a->pid > b->pid ? 1 : (a->pid < b->pid ? -1 : 0);
	if (result == 0) {
		result = a->tid > b->tid ? 1 : (a->tid < b->tid ? -1 : 0);
	}
	if (result == 0) {
		result = strcmp(a->comm, b->comm);
	}

	return result;
}

static int slab_compare_trace(const struct slab_track_info *a, const struct track_info *b)
{
	int result = 0;

	for (size_t i = 0; i < SLAB_DEBUG_TRACE_NR; i++) {
		if (a->trace[i] > b->trace[i]) {
			result = 1;
			break;
		} else if (a->trace[i] < b->trace[i]) {
			result = -1;
			break;
		}
	}

	return result;
}

static int slab_shift_to_right(struct track_head *th, unsigned long pos)
{
	int rc = E_HM_OK;
	struct track_info *ti = NULL;

	if (th->items >= th->max && slab_alloc_track(th, th->max << 1) != E_HM_OK) {
		rc = E_HM_NOMEM;
	}

	if (rc == E_HM_OK) {
		ti = th->ti + pos;
		if (pos != th->items) {
			NOFAIL(memmove_s(ti + 1, (th->max - pos - 1) * sizeof(struct track_info),
			       ti, (th->items - pos) * sizeof(struct track_info)));
		}
	}

	return rc;
}

int slab_count_amount(struct track_head *th, const struct slab_user_info *info)
{
	int rc = E_HM_INVAL;
	long start, end, pos;
	struct track_info *ti = NULL;
	int cmp;

	BUG_ON_D(th == NULL);
	BUG_ON_D(info == NULL);

	start = -1;
	end = th->items;

	while (1) {
		pos = start + ((end - start + 1) >> 1);
		if (pos == end) {
			break;
		}
		cmp = slab_compare_user(&info->alloc, &th->ti[pos]);
		if (cmp == 0) {
			ti = &th->ti[pos];
			ti->count++;
			rc = E_HM_OK;
			break;
		} else if (cmp > 0) {
			start = pos;
		} else {
			end = pos;
		}
	}

	if (rc != E_HM_OK) {
		rc = slab_shift_to_right(th, (unsigned long)pos);
		if (rc == E_HM_OK) {
			th->items++;
			ti = th->ti + pos;
			ti->count = 1;
			ti->pid = info->alloc.pid;
			ti->tid = info->alloc.tid;
			NOFAIL(memcpy_s(ti->comm, SLAB_DEBUG_COMM_LEN, info->alloc.comm,
					SLAB_DEBUG_COMM_LEN));
		}
	}

	return rc;
}

int slab_count_trace(struct track_head *th, const struct slab_user_info *info)
{
	int rc = E_HM_INVAL;
	long start, end, pos;
	struct track_info *ti = NULL;
	const struct slab_track_info *track_info = NULL;
	int cmp;

	BUG_ON_D(th == NULL);
	BUG_ON_D(info == NULL);

	start = -1;
	end = th->items;
	track_info = &info->alloc;

	while (1) {
		pos = start + ((end - start + 1) >> 1);
		if (pos == end) {
			break;
		}
		if ((cmp = slab_compare_trace(track_info, &th->ti[pos])) == 0) {
			cmp = slab_compare_user(track_info, &th->ti[pos]);
		}
		if (cmp == 0) {
			ti = &th->ti[pos];
			ti->count++;
			rc = E_HM_OK;
			break;
		} else if (cmp > 0) {
			start = pos;
		} else {
			end = pos;
		}
	}

	if (rc != E_HM_OK) {
		rc = slab_shift_to_right(th, pos);
		if (rc == E_HM_OK) {
			th->items++;
			ti = th->ti + pos;
			ti->count = 1;
			ti->pid = track_info->pid;
			ti->tid = track_info->tid;
			NOFAIL(memcpy_s(ti->comm, SLAB_DEBUG_COMM_LEN, track_info->comm,
					SLAB_DEBUG_COMM_LEN));
			NOFAIL(memcpy_s(ti->trace, sizeof(ti->trace), track_info->trace,
					sizeof(track_info->trace)));
		}
	}

	return rc;
}

static void slab_dump_page_usage(struct track_head *th, struct slab_ator *ator,
				 struct slab_desc *desc)
{
	uint32_t idx;
	struct slab_user_info *info = NULL;
	void *slot = NULL;

	th->slots += ator->max_slot_count;
	for (idx = 0; idx < ator->max_slot_count; idx++) {
		if (raw_bitmap_test_bit(desc->free_map, ator->max_slot_count, idx) ==
				SLAB_SLOT_FREE) {
			continue;
		}
		slot = ulong_to_ptr(slot_idx_to_ptr(ator, desc, idx), void);
#ifdef CONFIG_PER_CPU_SLAB
		if (is_obj_in_cache(ator, slot)) {
			th->caches++;
			continue;
		}
#endif
		info = slab_slot_to_userzone(ator, slot);
		if (th->count_usage) {
			th->count_usage(th, info);
		}
	}
}

/*
 * Remain Problem: The source files of the Slab module are compiled in different modules.
 * (eg: slab_track.c is compiled in ulibs, while slab.c is compiled in sysmgr),
 * which leads to an implicit compilation dependency. When the CONFIG of sysmgr
 * and ulibs is inconsistent, unexpected problems may occur. Add this function
 * to implement link checking to prevent CONFIG_USE_PREEMPT_PROCESS inconsistency.
 */
#ifdef CONFIG_USE_PREEMPT_PROCESS
void slab_config_consistency_check(void)
{
}
#endif

void slab_dump_ator_usage(struct track_head *th, struct slab_ator *ator, uint32_t s_flags)
{
	int rc = E_HM_OK;
	struct slab_desc *desc = NULL;
	struct slab_desc *tmp = NULL;
	unsigned long min;
	unsigned long max;

	if (th == NULL || ator == NULL || !(s_flags & ator->flags)) {
		rc = E_HM_INVAL;
	} else {
		min = SLAB_PAGE_SIZE / sizeof(struct track_info) + 1;
		max = (ator->partial_num + ator->full_num) * ator->max_slot_count;
		rc = slab_alloc_track(th, max >= min ? max : min);
	}

	if (rc == E_HM_OK) {
		ator_lock(ator);
		dlist_for_each_entry_safe(desc, tmp, &(ator->partial), struct slab_desc, node) {
			slab_dump_page_usage(th, ator, desc);
		}

		dlist_for_each_entry_safe(desc, tmp, &(ator->full), struct slab_desc, node) {
			slab_dump_page_usage(th, ator, desc);
		}
		ator_unlock(ator);
	}
}

#ifdef CONFIG_KASAN_DUMPSTACK
struct slab_user_info *slab_find_user_info(void *addr)
{
	struct slab_desc *s_desc = NULL;
	struct slab_ator *ator = NULL;
	struct slab_user_info *info = NULL;

	s_desc = slab_obj_to_slab_desc(addr, (size_t)SLAB_PAGE_SIZE);
	if (s_desc != NULL) {
		ator = s_desc->ator;
		uint32_t idx =  slot_ptr_to_idx(ator, s_desc, addr);
		void *slot = NULL;

		slot = ulong_to_ptr(slot_idx_to_ptr(ator, s_desc, idx), void);
		info = slab_slot_to_userzone(ator, slot);
	}

	return info;
}

void slab_store_free_info(void *obj)
{
	int ret = E_HM_OK;
	struct slab_ator *s_ator = slab_obj_to_ator(obj);
	pid_t pid __maybe_unused = 0;
	struct slab_desc *s_desc = NULL;

	if (unlikely(s_ator == NULL)) {
		hm_error("failed to search allocator from obj, maybe use error address:(0x%lx)\n",
			 ptr_to_ulong(obj));
		return;
	}
	obj = slab_redzone_obj_to_slot(obj);
	/* search for slab desc from this obj */
	s_desc = slab_obj_to_slab_desc(obj, (size_t)SLAB_PAGE_SIZE);
	if (likely(s_desc != NULL)) {
		/* judge this obj whether belong to this allocator */
		if (unlikely(s_desc->ator != s_ator)) {
			/* wrong allocator */
			hm_error("The allocator does not match the one of "
				 "the obj:(0x%lx)\n", ptr_to_ulong(obj));
			ret = E_HM_INVAL;
		}
	} else {
		hm_error("Obj to slab desc failed. "
			 "Maybe the obj:(0x%lx) does not belong to slab\n",
			 ptr_to_ulong(obj));
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		slab_clear_user_info(s_ator, obj, &pid);
	}
}
#endif
