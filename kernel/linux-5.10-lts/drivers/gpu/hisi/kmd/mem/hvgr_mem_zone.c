/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */
#include <securec.h>

#include "hvgr_mem_zone.h"
#include "hvgr_mem_space.h"
#include "hvgr_mem_api.h"
#include "hvgr_log_api.h"

static void hvgr_mem_zone_destroy(struct hvgr_mem_ctx * const pmem, uint64_t prot);

bool hvgr_mem_zone_init_proc(struct hvgr_mem_ctx * const pmem)
{
	struct hvgr_mem_space *mem_space = NULL;

	if (unlikely(pmem == NULL))
		return false;

	(void)memset_s(pmem->zones, sizeof(pmem->zones), 0, sizeof(pmem->zones));

	mem_space = &pmem->zones[HVGR_MEM_PROPERTY_2M_PAGE].va_space;
	mem_space->start = HVGR_MEM_2M_START;
	mem_space->size = HVGR_MEM_2M_SIZE;
	if (hvgr_mem_space_init(mem_space) != 0)
		return false;

	mem_space = &pmem->zones[HVGR_MEM_PROPERTY_IMPORT].va_space;
	mem_space->start = HVGR_MEM_IMPORT_START;
	mem_space->size = HVGR_MEM_IMPORT_SIZE;
	if (hvgr_mem_space_init(mem_space) != 0)
		goto import_space_fail;

	mem_space = &pmem->zones[HVGR_MEM_PROPERTY_GPU_MEM].va_space;
	mem_space->start = HVGR_MEM_GPU_START;
	mem_space->size = HVGR_MEM_GPU_SIZE;
	if (hvgr_mem_space_init(mem_space) != 0)
		goto gpu_space_fail;

	mem_space = &pmem->zones[HVGR_MEM_PROPERTY_REPLAY].va_space;
	mem_space->start = HVGR_MEM_REPLAY_START;
	mem_space->size = HVGR_MEM_REPLAY_SIZE;
	if (hvgr_mem_space_init(mem_space) != 0)
		goto replay_space_fail;

	return true;
replay_space_fail:
	mem_space = &pmem->zones[HVGR_MEM_PROPERTY_GPU_MEM].va_space;
	hvgr_mem_space_term(mem_space);
gpu_space_fail:
	mem_space = &pmem->zones[HVGR_MEM_PROPERTY_IMPORT].va_space;
	hvgr_mem_space_term(mem_space);
import_space_fail:
	mem_space = &pmem->zones[HVGR_MEM_PROPERTY_2M_PAGE].va_space;
	hvgr_mem_space_term(mem_space);
	return false;
}

void hvgr_mem_zone_term_proc(struct hvgr_mem_ctx * const pmem)
{
	uint32_t idx;
	struct hvgr_mem_space *mem_space = NULL;

	for (idx = 0; idx < HVGR_MEM_PROPERTY_MAX; idx++)
		hvgr_mem_zone_destroy(pmem, idx);

	mem_space = &pmem->zones[HVGR_MEM_PROPERTY_REPLAY].va_space;
	hvgr_mem_space_term(mem_space);

	mem_space = &pmem->zones[HVGR_MEM_PROPERTY_GPU_MEM].va_space;
	hvgr_mem_space_term(mem_space);

	mem_space = &pmem->zones[HVGR_MEM_PROPERTY_IMPORT].va_space;
	hvgr_mem_space_term(mem_space);

	mem_space = &pmem->zones[HVGR_MEM_PROPERTY_2M_PAGE].va_space;
	hvgr_mem_space_term(mem_space);

	(void)memset_s(pmem->zones, sizeof(pmem->zones), 0, sizeof(pmem->zones));
}

bool hvgr_mem_zone_create(struct hvgr_mem_ctx * const pmem,
	uint64_t prot, struct hvgr_zone_opts const * const opts,
	void * const para)
{
	struct hvgr_ctx *ctx = NULL;
	struct hvgr_mem_zone *zone = NULL;

	if ((pmem == NULL) || (prot >= HVGR_MEM_PROPERTY_MAX) ||
		(opts == NULL))
		return false;

	zone = &pmem->zones[prot];
	zone->pmem = pmem;
	zone->opts = opts;
	zone->property = prot;
	if (!zone->opts->init(zone, para)) {
		ctx = container_of(pmem, struct hvgr_ctx, mem_ctx);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u memory zone %d init FAIL.", ctx->id, prot);
		(void)memset_s(zone, sizeof(*zone), 0, sizeof(*zone));
		return false;
	}

	return true;
}

static void hvgr_mem_zone_destroy(struct hvgr_mem_ctx * const pmem,
	uint64_t prot)
{
	struct hvgr_mem_zone *zone = NULL;

	zone = hvgr_mem_zone_get_zone(pmem, prot);
	if (unlikely(zone == NULL))
		return;

	(void)zone->opts->term(zone);
}

struct hvgr_mem_zone *hvgr_mem_zone_get_zone(
	struct hvgr_mem_ctx * const pmem, uint64_t prot)
{
	struct hvgr_mem_zone *zone = NULL;

	if (unlikely((pmem == NULL) || (prot >= HVGR_MEM_PROPERTY_MAX)))
		return NULL;

	zone = &pmem->zones[prot];
	if (zone->opts == NULL)
		return NULL;

	return zone;
}

static inline struct hvgr_mem_area *hvgr_mem_zone_find_area_va(
	struct hvgr_kv_map * const root, uint64_t va)
{
	struct hvgr_mem_area *area = NULL;
	struct hvgr_kv_node *node = hvgr_kv_get(root, va);

	if (node != NULL)
		area = container_of(node, struct hvgr_mem_area, node);

	return area;
}

static inline struct hvgr_mem_area *hvgr_mem_zone_find_area_va_range(
	struct hvgr_kv_map * const root, uint64_t va)
{
	struct hvgr_mem_area *area = NULL;
	struct hvgr_kv_node *node = hvgr_kv_find_range(root, va);

	if (node != NULL)
		area = container_of(node, struct hvgr_mem_area, node);

	return area;
}

struct hvgr_mem_area *hvgr_mem_zone_find_area_by_gva(
	struct hvgr_mem_ctx * const pmem, uint64_t gva)
{
	if (unlikely(pmem == NULL))
		return NULL;

	return hvgr_mem_zone_find_area_va(&pmem->area_rec, gva);
}

struct hvgr_mem_area *hvgr_mem_zone_find_area_by_gva_range(
	struct hvgr_mem_ctx * const pmem, uint64_t gva)
{
	if (unlikely(pmem == NULL))
		return NULL;

	return hvgr_mem_zone_find_area_va_range(&pmem->area_rec, gva);
}

void __attribute__((weak)) hvgr_mem_zone_update_kva(struct hvgr_mem_area *area)
{
}

long hvgr_mem_zone_alloc_memory(struct hvgr_mem_area * const area)
{
	struct hvgr_ctx *ctx = NULL;
	long ret;

	if (unlikely((area == NULL) || (area->zone == NULL)))
		return -EINVAL;

	ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
	/*
	 * Put the check_para at the general procedure because
	 * every implementation must check input parameters.
	 */
	if (!area->zone->opts->check_para(area)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u mem alloc check para fail. %lu:0x%lx",
			ctx->id, area->property, area->attribute);
		return -EINVAL;
	}

	/*
	 * same va, 4g va, drv mem has HVGR_MEM_MASK_KMD_ACCESS,
	 * the alloc user should call hvgr_mem_unmap_kva
	 */
	if (area->attribute & HVGR_MEM_MASK_KMD_ACCESS) {
		if (!hvgr_mem_map_kva(area)){
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u mem kmap fail %lu:0x%lx:0x%lx:0x%lx",
				ctx->id, area->property, area->attribute, area->pages,
				area->max_pages);
			return -ENOMEM;
		}
	}

	ret = area->zone->opts->alloc(area);
	if (ret != 0) {
		hvgr_mem_unmap_kva(area);
		if (area->property != HVGR_MEM_PROPERTY_JIT)
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u mem alloc fail %d. %lu:0x%lx:0x%lx:0x%lx",
				ctx->id, ret, area->property, area->attribute, area->pages,
				area->max_pages);
		return -ENOMEM;
	}
	hvgr_mem_zone_update_kva(area);

	return 0;
}

void hvgr_mem_zone_free_memory(struct hvgr_mem_area * const area)
{
	struct hvgr_ctx *ctx = NULL;

	if (unlikely((area == NULL) || (area->zone == NULL)))
		return;

	if (area->zone->opts->free(area) != 0) {
		ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u memory Free fail. gva = %llx",
			ctx->id, area->gva);
	}
}

long hvgr_mem_zone_import_memory(struct hvgr_mem_area * const area, int fd)
{
	if (unlikely((area == NULL) || (area->zone == NULL) || (area->zone->opts == NULL) ||
		(area->zone->opts->import == NULL)))
		return -EINVAL;

	return area->zone->opts->import(area, fd);
}

static uint32_t hvgr_mem_zone_get_name(enum hvgr_zone_type zone_type)
{
	uint32_t zone_name[ZONE_TYPE_MAX] = {HVGR_MEM_PROPERTY_GPU_MEM, HVGR_MEM_PROPERTY_2M_PAGE,
		HVGR_MEM_PROPERTY_IMPORT, HVGR_MEM_PROPERTY_REPLAY};

	return zone_name[zone_type];
}

uint64_t hvgr_mem_zone_alloc_gva(struct hvgr_mem_ctx * const pmem, enum hvgr_zone_type zone_type,
	uint64_t target_addr, uint64_t size)
{
	uint64_t addr;
	struct hvgr_mem_space *mem_space = NULL;
	struct hvgr_ctx *ctx = container_of(pmem, struct hvgr_ctx, mem_ctx);

	if (zone_type >= ZONE_TYPE_MAX)
		return 0U;

	mem_space = &pmem->zones[hvgr_mem_zone_get_name(zone_type)].va_space;
	if (hvgr_mem_space_split(mem_space, target_addr, size, &addr) != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u alloc %u va 0x%lx %lu FAIL",
			ctx->id, zone_type, target_addr, size);
		addr = 0U;
	}

	return addr;
}

void hvgr_mem_zone_free_gva(struct hvgr_mem_ctx * const pmem, enum hvgr_zone_type zone_type,
	uint64_t addr, uint64_t size)
{
	struct hvgr_mem_space *mem_space = NULL;
	struct hvgr_ctx *ctx = container_of(pmem, struct hvgr_ctx, mem_ctx);

	if (zone_type >= ZONE_TYPE_MAX)
		return;

	mem_space = &pmem->zones[hvgr_mem_zone_get_name(zone_type)].va_space;
	if (hvgr_mem_space_joint(mem_space, addr, size) != 0)
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u free %u va 0x%lx %lu FAIL",
			ctx->id, zone_type, addr, size);
}

