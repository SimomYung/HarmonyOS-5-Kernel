/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Description: Allocator for actv slot metadata
 * Create: Tue May 12 19:57:49 2020
 */

#include <libhmucap/actv_kobjpool.h>

#include <hmasm/page.h>
#include <libmem/utils.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hmkernel/const.h>
#include <libhmlog/hmlog.h>
#include <libhmlog/hmlog_ratelimit.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>
#include <hmkernel/attributes/hmattributes.h>

#include <libhmucap/kobjpool.h>

#define ACTV_SLOT_PMEM_SIZE	(__CAP_ACTIVATION_OBJ_SIZE)
#define ACTV_SLOTS_PER_PAGE	((size_t)PAGE_SIZE / ACTV_SLOT_PMEM_SIZE)
#define ROW_SLOTS_SIZE		(sizeof(struct actv_kobj_slot) * ACTV_SLOTS_PER_PAGE)

static bool is_slot_free(const struct actv_kobj_slot *slot)
{
	return (slot->used == 0U);
}

static bool has_page(const struct actv_kobj_slot *slot)
{
	return (slot->mem.pmem != 0ULL);
}

static int get_free_actv_kobj_slot(struct actv_kobj_slot *base, uint32_t row,
				   struct actv_kobj_slot **slot)
{
	unsigned int i = 0;
	unsigned int j = 0;
	bool found = false;
	int first = -1;

	for (i = 0; i < row; i++) {
		if (!has_page(base + (i * ACTV_SLOTS_PER_PAGE))) {
			first = (first >= 0) ? first : (int)i;
			continue;
		}

		if ((base + (i * ACTV_SLOTS_PER_PAGE))->remain_actv == 0U) {
			continue;
		}
		for (j = 0; j < ACTV_SLOTS_PER_PAGE; j++) {
			found = is_slot_free(base + (i * ACTV_SLOTS_PER_PAGE) + j);
			if (found) {
				break;
			}
		}
		if (found) {
			*slot = base + (i * ACTV_SLOTS_PER_PAGE) + j;
			break;
		}
	}

	return first;
}

static void init_actv_kobj_slots(struct actv_kobj_slot *slot, const struct ucap_mem *mem)
{
	unsigned int i;

	for (i = 0; i < ACTV_SLOTS_PER_PAGE; i++) {
		(slot + i)->remain_actv = (uint16_t)ACTV_SLOTS_PER_PAGE;
		(slot + i)->used = 0;
		(slot + i)->inner_idx = 0;
		(slot + i)->kobj_res = NULL;
#ifdef CONFIG_NEED_SHADOW_STACK
		(slot + i)->sstack = NULL;
#endif
		(slot + i)->mem = *mem;
	}
}

static void reset_actv_kobj_slots(struct actv_kobj_slot *slots)
{
	mem_zero_p(slots, ROW_SLOTS_SIZE, ROW_SLOTS_SIZE);
}

struct actv_kobj_pool *actv_kobj_pool_init(uint32_t nr_actvs)
{
	bool err = false;
	void *slot_pool = NULL;
	struct actv_kobj_pool *pool = NULL;
	size_t len = sizeof(struct actv_kobj_slot) * nr_actvs;

	if (nr_actvs % ACTV_SLOTS_PER_PAGE != 0U) {
		hm_debug("invalid actv nums\n");
		err = true;
	} else if (nr_actvs > __ACTV_POOL_MAX_ACTVS) {
		hm_debug("init with too much actvs: %u\n", nr_actvs);
		err = true;
	} else {
		len = PAGE_ALIGN_UP(len);
		slot_pool = hm_mem_mmap(NULL, len, __U(PROT_READ) | __U(PROT_WRITE),
					__U(MAP_PRIVATE) | __U(MAP_ANON), -1, 0);
		if (slot_pool == MAP_FAILED) {
			hm_warn("hm_mem_mmap slot_pool failed\n");
			err = true;
		}
	}

	if (!err && slot_pool != NULL) {
		mem_zero_p(slot_pool, sizeof(struct actv_kobj_slot) * nr_actvs, sizeof(struct actv_kobj_slot) * nr_actvs);
		pool = malloc(sizeof(struct actv_kobj_pool));
		if (pool == NULL) {
			hm_warn("malloc pool failed\n");
			(void)hm_mem_munmap(slot_pool, len);
			err = true;
		}
	}

	if (!err && pool != NULL) {
		pool->nr_actvs = nr_actvs;
		pool->slot_pool = slot_pool;
	}
	return pool;
}

struct actv_kobj_slot *actv_kobj_slot_alloc(struct actv_kobj_pool *pool, void *kobj_res)
{
	int index;
	uint32_t max_row;
	struct actv_kobj_slot *slot = NULL;
	struct actv_kobj_slot *base = NULL;

	if (pool == NULL) {
		__hmattr_sideff_resolved("no relative side effect");
		return NULL;
	}

	base = ptr_from_void(pool->slot_pool, struct actv_kobj_slot);
	max_row = pool->nr_actvs / (unsigned int)ACTV_SLOTS_PER_PAGE;
	index = get_free_actv_kobj_slot(base, max_row, &slot);
	if (slot != NULL) {
		uintptr_t size = (uintptr_t)slot - (uintptr_t)base;
		index = (int)(size / ROW_SLOTS_SIZE);
	} else if (index >= 0 && (uint32_t)index < max_row) {
		struct ucap_mem mem;
		if (has_page(base + ((uintptr_t)(unsigned int)index * ACTV_SLOTS_PER_PAGE))) {
			hm_panic("error slot state\n");
			__hmattr_sideff_resolved("no relative side effect");
			return NULL;
		}
		mem.pmem = ucap_default_ops.alloc_pmem(0, PAGE_SIZE, &mem);
		if (IS_REF_ERR(mem.pmem)) {
			hm_error("failed to alloc pmem, err=%s\n", hmstrerror(REF_TO_ERR(mem.pmem)));
			__hmattr_sideff_resolved("no relative side effect");
			return NULL;
		}
		slot = base + ((uintptr_t)(unsigned int)index * ACTV_SLOTS_PER_PAGE);
		init_actv_kobj_slots(slot, &mem);
	} else {
		hm_error_ratelimited("there is no slot\n");
	}

	if (slot != NULL) {
		slot->used = 1;
		slot->kobj_res = kobj_res;
		(base + ((uintptr_t)(unsigned int)index * ACTV_SLOTS_PER_PAGE))->remain_actv--;
	}
	return slot;
}

struct actv_kobj_slot *actv_kobj_slot_search(struct actv_kobj_pool *pool, uint32_t inner_idx)
{
	unsigned int i;
	unsigned int j;
	unsigned int max_row;
	struct actv_kobj_slot *slot = NULL;
	struct actv_kobj_slot *base = NULL;

	if (pool == NULL) {
		hm_debug("pool is NULL\n");
		__hmattr_sideff_resolved("no relative side effect");
		return NULL;
	}

	base = ptr_from_void(pool->slot_pool, struct actv_kobj_slot);
	max_row = pool->nr_actvs / (unsigned int)ACTV_SLOTS_PER_PAGE;
	for (i = 0; i < max_row; i++) {
		if (!has_page(base + (i * ACTV_SLOTS_PER_PAGE))) {
			continue;
		}
		for (j = 0; j < ACTV_SLOTS_PER_PAGE; j++) {
			if ((base + (i * ACTV_SLOTS_PER_PAGE) + j)->inner_idx == inner_idx) {
				slot = base + (i * ACTV_SLOTS_PER_PAGE) + j;
				__hmattr_sideff_resolved("no relative side effect");
				return slot;
			}
		}
	}

	hm_error("not find the slot with inner idx, idx=%x\n", inner_idx);
	return NULL;
}

void actv_kobj_pool_free_one_slot(struct actv_kobj_pool *pool, struct actv_kobj_slot *slot)
{
	struct actv_kobj_slot *slots = NULL;
	struct actv_kobj_slot *base = NULL;
	uintptr_t size;
	unsigned int index;
	int err;

	if (pool != NULL && slot != NULL) {
		base = ptr_from_void(pool->slot_pool, struct actv_kobj_slot);
		size = (uintptr_t)slot - (uintptr_t)base;
		index = (unsigned int)(size / ROW_SLOTS_SIZE);
		slot->used = 0;
		slot->inner_idx = 0;
		slots = (base + (index * ACTV_SLOTS_PER_PAGE));
		slots->remain_actv++;

		if (slots->remain_actv == ACTV_SLOTS_PER_PAGE) {
			err = ucap_default_ops.free_pmem(0, &slots->mem);
			if (err < 0) {
				hm_warn("failed to free pmem, lead to memory leak, err=%s\n", hmstrerror(err));
			}
			reset_actv_kobj_slots(slots);
		}
	}
}

void actv_kobj_slot_free(struct actv_kobj_pool *pool, uint32_t inner_idx)
{
	struct actv_kobj_slot *slot = NULL;

	slot = actv_kobj_slot_search(pool, inner_idx);
	if (slot == NULL) {
		hm_warn("error inner_idx\n");
	} else {
		actv_kobj_pool_free_one_slot(pool, slot);
	}
}

void actv_kobj_res_set(void *actv_kobj_pool, cref_t a_cref, void *kobj_res)
{
	struct actv_kobj_slot *slot = NULL;
	uint32_t inner_idx = __REF_INNER_IDX_OF(a_cref);

	slot = actv_kobj_slot_search(actv_kobj_pool, inner_idx);
	BUG_ON(slot == NULL);

	slot->kobj_res = kobj_res;
}

#ifdef CONFIG_NEED_SHADOW_STACK
void actv_kobj_sstack_set(void *actv_kobj_pool, cref_t a_cref, void *sstack)
{
	struct actv_kobj_slot *slot = NULL;
	uint32_t inner_idx = __REF_INNER_IDX_OF(a_cref);

	slot = actv_kobj_slot_search(actv_kobj_pool, inner_idx);
	BUG_ON(slot == NULL);

	slot->sstack = sstack;
}

void *actv_kobj_pool_sstack_of(struct actv_kobj_slot *slot)
{
	return (slot == NULL) ? NULL : slot->sstack;
}
#endif

void *actv_kobj_pool_res_of(struct actv_kobj_slot *slot)
{
	return (slot == NULL) ? NULL : slot->kobj_res;
}
