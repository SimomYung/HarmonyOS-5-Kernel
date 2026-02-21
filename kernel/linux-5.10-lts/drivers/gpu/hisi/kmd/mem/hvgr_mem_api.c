/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_mem_api.h"

#include <linux/mm.h>
#include <linux/dma-buf.h>

#include "hvgr_ioctl_mem.h"
#include "hvgr_mem_zone.h"
#include "hvgr_dm_api.h"
#include "hvgr_mem_alias.h"
#include "hvgr_mem_sparse.h"
#include "hvgr_mem_debugfs.h"
#include "hvgr_mem_import.h"
#include "hvgr_mem_opts.h"
#include "hvgr_kv.h"
#include "hvgr_log_api.h"

static bool hvgr_mem_ioctl_alloc_para_is_valid(struct hvgr_ctx * const ctx, uint64_t property,
	uint64_t attr)
{
	/* Check the INVALID property from UMD */
	if ((!hvgr_ctx_flag(ctx, HVGR_CTX_FLAG_COMPAT) && property == HVGR_MEM_PROPERTY_GPU_MEM) ||
		(property == HVGR_MEM_PROPERTY_JIT) ||
		(property == HVGR_MEM_PROPERTY_DRIVER) ||
		(property == HVGR_MEM_PROPERTY_ALIAS) ||
		((attr & HVGR_MEM_ATTR_KMD_ACCESS) != 0))
		return false;

	return true;
}

static bool hvgr_mem_ioctl_para_is_valid(struct hvgr_ctx * const ctx, uint64_t property)
{
	/* Check the INVALID property from UMD */
	if ((!hvgr_ctx_flag(ctx, HVGR_CTX_FLAG_COMPAT) && property == HVGR_MEM_PROPERTY_GPU_MEM) ||
		(property == HVGR_MEM_PROPERTY_JIT) ||
		(property == HVGR_MEM_PROPERTY_DRIVER) ||
		(property == HVGR_MEM_PROPERTY_SAME_VA))
		return false;

	return true;
}

static long hvgr_mem_ioctl_alloc_memory(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_alloc_memory * const para)
{
	struct hvgr_mem_area *area = NULL;
	struct hvgr_mem_alloc_para info;
	struct hvgr_mem_area_va area_va = {0, 0};

	if (hvgr_mem_is_illegal_mm(ctx))
		return -EPERM;

	if (!hvgr_mem_ioctl_alloc_para_is_valid(ctx, para->in.property, para->in.attribute))
		return -EPERM;

	info.property = para->in.property;
	info.attribute = para->in.attribute;
	info.pages = para->in.pages;
	info.max_pages = para->in.max_pages;
	info.extent = para->in.extent;
	info.va = para->in.va;
	info.phy_pages = para->in.phy_pages;
	area = hvgr_mem_allocate(ctx, &info, &area_va, true);
	if (area == NULL)
		return -ENOMEM;

	para->out.gva = area_va.gva;
	para->out.attr = info.attribute;

	return 0;
}

static long hvgr_mem_ioctl_free_memory(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_free_memory * const para)
{
	struct hvgr_mem_area *area = NULL;

	if (para->in.gva >= HVGR_MEM_BITMAP_START_OFFSET &&
		para->in.gva < HVGR_MEM_BITMAP_END_OFFSET) {
		hvgr_err(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u dont support free pend 0x%lx",
			__func__, __LINE__, ctx->id, para->in.gva);
		return -EINVAL;
	}

	mutex_lock(&ctx->mem_ctx.area_mutex);
	area = hvgr_mem_zone_find_area_by_gva_range(&ctx->mem_ctx, para->in.gva);
	if (area == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "%s ctx_%u has no va 0x%lx",
			__func__, ctx->id, para->in.gva);
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		return -EINVAL;
	}

	if (!area->user_buf) {
		hvgr_err(ctx->gdev, HVGR_MEM, "%s ctx_%u perm deny: area 0x%lx 0x%lx:0x%lx",
			__func__, ctx->id, area->gva, area->property, area->attribute);
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		return -EPERM;
	}

	if (!hvgr_mem_ioctl_para_is_valid(ctx, area->property)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "%s ctx_%u prop deny: area 0x%lx 0x%lx:0x%lx",
			__func__, ctx->id, area->gva, area->property, area->attribute);
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		return -EPERM;
	}

	hvgr_mem_del_from_map_rec(ctx, area);
	area->is_freed = true;
	mutex_unlock(&ctx->mem_ctx.area_mutex);

	if (hvgr_mem_free_area_nolock(ctx, area) != 0)
		return -EINVAL;

	return 0;
}

static inline long hvgr_mem_ioctl_sync_memory(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_sync_memory * const para)
{
	return hvgr_mem_sync(ctx, para->in.uva, para->in.size, para->in.direct);
}

static long hvgr_mem_ioctl_import_memory(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_import_memory * const para)
{
	struct hvgr_mem_area *area = NULL;
	struct hvgr_mem_import_para info;

	info.fd = para->in.fd;
	info.attribute = para->in.attribute;
	info.gva = para->in.gva;
	info.pages = para->in.pages;
	area = hvgr_mem_import(ctx, &info, &para->out.uva, &para->out.gva);
	if (area == NULL)
		return -ENOMEM;

	return 0;
}

static long hvgr_mem_ioctl_query_memory(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_query_memory * const para)
{
	struct hvgr_mem_area *area = NULL;

	mutex_lock(&ctx->mem_ctx.area_mutex);
	/* Find area by gva range */
	area = hvgr_mem_zone_find_area_by_gva(&ctx->mem_ctx, para->in.gva);
	if (area == NULL) {
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		return -EINVAL;
	}

	para->out.property = area->property;
	para->out.attribute = area->attribute;
	para->out.pages = area->pages;
	para->out.max_pages = area->max_pages;
	para->out.va = area->gva;
	para->out.phy_pages = area->phy_pages;
	mutex_unlock(&ctx->mem_ctx.area_mutex);
	return 0;
}

static inline long hvgr_mem_ioctl_resize_memory(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_resize_memory * const para)
{
	if (hvgr_mem_is_illegal_mm(ctx))
		return -EPERM;

	return hvgr_mem_resize(ctx, para->in.gva, para->in.pages);
}

static long hvgr_mem_ioctl_get_fd_size_memory(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_get_fd_size_memory * const para)
{
	struct dma_buf *dmabuf = NULL;

	dmabuf = dma_buf_get(para->in.fd);
	if (IS_ERR_OR_NULL(dmabuf)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u get fd (%d) dma buf fail",
			ctx->id, para->in.fd);
		return -EINVAL;
	}

	para->out.pages = (PAGE_ALIGN(dmabuf->size) >> PAGE_SHIFT);

	dma_buf_put(dmabuf);
	return 0;
}

static long hvgr_mem_ioctl_alias_memory(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_alias_memory * const para)
{
	struct hvgr_mem_area *area = NULL;
	struct hvgr_mem_alias_para info;

	info.attribute = para->in.attribute;
	info.stride = para->in.stride;
	info.count = para->in.count;
	info.alias_info_ptr = para->in.alias_info_ptr;
	area = hvgr_mem_alias(ctx, &info, &para->out.pages, &para->out.gva);
	if (area == NULL)
		return -ENOMEM;

	return 0;
}

static inline long hvgr_mem_ioctl_add_profile_memory(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_profile_memory * const para)
{
	return hvgr_mem_add_profile(ctx, para->in.user_buf, para->in.size);
}


static inline long hvgr_mem_ioctl_bind_sparse_memory(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_sparse_bind * const para)
{
	return hvgr_mem_sparse_bind(ctx, para->sparse_addr, para->mem_addr, para->pages, false);
}

static inline long hvgr_mem_ioctl_unbind_sparse_memory(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_sparse_unbind * const para)
{
	return hvgr_mem_sparse_unbind(ctx, para->sparse_addr, para->pages);
}

static inline long hvgr_mem_ioctl_change_memory_attr(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_change_memory_attr * const para)
{
	return hvgr_mem_change_mem_attr(ctx, para->gva, para->attribute);
}


long hvgr_mem_ioctl_query_rsv(struct hvgr_ctx * const ctx, hvgr_ioctl_para_rsv * const para)
{
	struct hvgr_mem_process_res_cb *res_cb = NULL;
	struct hvgr_kv_node *node = NULL;
	uint64_t op = para->in.op;

	mutex_lock(&ctx->gdev->mem_dev.mem_process_res_lock);
	node = hvgr_kv_get(&ctx->gdev->mem_dev.mem_process_res_rec,
		(uint64_t)ctx->mem_ctx.process_mm);
	if (node == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u (%d:%d) query rsv find cb fail",
			ctx->id, ctx->tgid, ctx->pid);
		mutex_unlock(&ctx->gdev->mem_dev.mem_process_res_lock);
		return -EINVAL;
	}

	res_cb = container_of(node, struct hvgr_mem_process_res_cb, kv_node);
	para->out.cnts = 0;
	para->out.addr = 0U;

	if (res_cb->mem_4gva != NULL)
		para->out.addr = res_cb->mem_4gva->va_space.start;

	if (op == HVGR_RSV_QUREY_ADD) {
		para->out.cnts = atomic_add_return(1, &res_cb->rsv_cnt);
	} else if (op == HVGR_RSV_QUREY_SUB) {
		para->out.cnts = atomic_sub_return(1, &res_cb->rsv_cnt);
		if (para->out.cnts < 0)
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u (%d:%d) rsv_cnt err %ld",
				ctx->id, ctx->tgid, ctx->pid, para->out.cnts);
	}

	mutex_unlock(&ctx->gdev->mem_dev.mem_process_res_lock);
	return 0;
}

long hvgr_ioctl_mem(struct hvgr_ctx * const ctx, unsigned int cmd, unsigned long arg)
{
	long ret = -EINVAL;

	switch (cmd) {
	case HVGR_IOCTL_ALLOC_MEM:
		hvgr_ioctl_handle_wr(ctx, HVGR_IOCTL_ALLOC_MEM,
			hvgr_mem_ioctl_alloc_memory, arg, hvgr_ioctl_para_alloc_memory);
		break;
	case HVGR_IOCTL_FREE_MEM:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_FREE_MEM,
			hvgr_mem_ioctl_free_memory, arg, hvgr_ioctl_para_free_memory);
		break;
	case HVGR_IOCTL_SYNC_MEM:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_SYNC_MEM,
			hvgr_mem_ioctl_sync_memory, arg, hvgr_ioctl_para_sync_memory);
		break;
	case HVGR_IOCTL_IMPORT_MEMORY:
		hvgr_ioctl_handle_wr(ctx, HVGR_IOCTL_IMPORT_MEMORY,
			hvgr_mem_ioctl_import_memory, arg, hvgr_ioctl_para_import_memory);
		break;
	case HVGR_IOCTL_QUERY_MEM:
		hvgr_ioctl_handle_wr(ctx, HVGR_IOCTL_QUERY_MEM,
			hvgr_mem_ioctl_query_memory, arg, hvgr_ioctl_para_query_memory);
		break;
	case HVGR_IOCTL_RESIZE_MEM:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_RESIZE_MEM,
			hvgr_mem_ioctl_resize_memory, arg, hvgr_ioctl_para_resize_memory);
		break;
	case HVGR_IOCTL_GET_FD_SIZE_MEM:
		hvgr_ioctl_handle_wr(ctx, HVGR_IOCTL_GET_FD_SIZE_MEM,
			hvgr_mem_ioctl_get_fd_size_memory, arg, hvgr_ioctl_para_get_fd_size_memory);
		break;
	case HVGR_IOCTL_ALIAS_MEMORY:
		hvgr_ioctl_handle_wr(ctx, HVGR_IOCTL_ALIAS_MEMORY,
			hvgr_mem_ioctl_alias_memory, arg, hvgr_ioctl_para_alias_memory);
		break;
	case HVGR_IOCTL_ADD_PROFILE_MEM:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_ADD_PROFILE_MEM,
			hvgr_mem_ioctl_add_profile_memory, arg, hvgr_ioctl_para_profile_memory);
		break;
	case HVGR_IOCTL_SPARSE_MEM_BIND:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_SPARSE_MEM_BIND,
			hvgr_mem_ioctl_bind_sparse_memory, arg, hvgr_ioctl_para_sparse_bind);
		break;
	case HVGR_IOCTL_SPARSE_MEM_UNBIND:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_SPARSE_MEM_UNBIND,
			hvgr_mem_ioctl_unbind_sparse_memory, arg, hvgr_ioctl_para_sparse_unbind);
		break;
	case HVGR_IOCTL_CHANGE_MEM_ATTR:
		hvgr_ioctl_handle_w(ctx, HVGR_IOCTL_CHANGE_MEM_ATTR,
			hvgr_mem_ioctl_change_memory_attr, arg, hvgr_ioctl_para_change_memory_attr);
		break;
	default:
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u ioctl cmd 0x%x is INVALID", ctx->id, cmd);
		break;
	}

exit:
	return ret;
}

unsigned long hvgr_mem_get_ctx_using_page(struct hvgr_ctx * const ctx)
{
	int used_pages;

	used_pages = atomic_read(&(ctx->mem_ctx.used_pages));

	return (unsigned long)used_pages;
}
