/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation for /proc/vmalloc_info
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 23 05:54:55 2023
 */
#include <stdarg.h>
#include <devhost/log.h>
#include <devhost/memory.h>
#include <libmem/types.h>
#include <libhmelf/vsymbol.h>
#include <libdevhost/procfs.h>
#include <libdevhost/kernfs.h>
#include <liblinux/pal.h>
#include <lnxbase/lnxbase.h>
#include <lnxbase/vmap.h>
#include "file_info_ext.h"

#define BUF_SZ 512U
struct procfs_ctx {
	const struct libdh_kernfs_rw_args *r_args;
	unsigned long *r_size_out;

	void *buf;
	size_t size;
	size_t used;

	void *tmp;
	size_t tmp_sz;

	unsigned char *vec;
	bool is_procfs;
	bool is_emergency;
};

static int __procfs_ctx_init(struct procfs_ctx *ctx)
{
	*(ctx->r_size_out) = 0;
	ctx->buf = ulong_to_ptr(ctx->r_args->ubuf, void);
	ctx->size = (size_t)ctx->r_args->size;
	ctx->used = 0;

	ctx->tmp = (void *)malloc(BUF_SZ);
	if (ctx->tmp == NULL) {
		return E_HM_NOMEM;
	}
	ctx->tmp_sz = BUF_SZ;

	ctx->vec = (unsigned char *)malloc(PAGE_SIZE);
	if (ctx->vec == NULL) {
		free(ctx->tmp);
		ctx->tmp = NULL;
		return E_HM_NOMEM;
	}

	ctx->is_procfs = true;
	ctx->is_emergency = false;
	return E_HM_OK;
}

static void __procfs_ctx_deinit(struct procfs_ctx *ctx)
{
	free(ctx->tmp);
	ctx->tmp = NULL;
	free(ctx->vec);
	ctx->vec = NULL;
}

static int __kmesg_ctx_init(struct procfs_ctx *ctx, bool is_emergency)
{
	if (!is_emergency) {
		ctx->vec = (unsigned char *)malloc(PAGE_SIZE);
		if (ctx->vec == NULL) {
			return E_HM_NOMEM;
		}
	} else {
		ctx->vec = NULL;
	}

	ctx->is_procfs = false;
	ctx->is_emergency = is_emergency;
	return E_HM_OK;
}

static void __kmesg_ctx_deinit(struct procfs_ctx *ctx)
{
	if (ctx->vec != NULL) {
		free(ctx->vec);
		ctx->vec = NULL;
	}
}

static int file_info_copy_to_user(struct procfs_ctx *ctx, unsigned long size)
{
	int ret;
	if ((ctx->size - ctx->used) <= size) {
		return E_HM_FBIG;
	}
	ret = devhost_pal_copy_to_user(ptr_add(ctx->buf, ctx->used), ctx->tmp, size);
	if (ret == E_HM_OK) {
		ctx->used += size;
		*(ctx->r_size_out) += size;
	}
	return ret;
}

#define __procfs_ctx_print(ctx, fmt, ...)						\
({											\
	int __ret;									\
	if ((ctx)->is_procfs) {									\
		__ret = sprintf_s((ctx)->tmp, (ctx)->tmp_sz, fmt, ##__VA_ARGS__);	\
		if (__ret < 0) {							\
			__ret = E_HM_OVERFLOW;						\
		} else {								\
			__ret = file_info_copy_to_user(ctx, (unsigned long)__ret);	\
		}									\
	} else {									\
		dh_info(fmt, ##__VA_ARGS__);						\
		__ret = E_HM_OK;							\
	}										\
	__ret;										\
})

#define VMALLOC_INFO_LINE_SIZE 256
static int vmalloc_info_print_info(struct procfs_ctx *ctx, struct vmap_node *vnode)
{
	char line_buffer[VMALLOC_INFO_LINE_SIZE] = { 0 };
	char buf[VMALLOC_INFO_LINE_SIZE] = { 0 };
	unsigned long pmem_size = 0;
	unsigned int nr_pages = 0;
	int iter = 0;
	int ret = E_HM_OK;

	if (!ctx->is_emergency) {
		ret = liblinux_pal_vmalloc_pmem_stat(vnode, ctx->vec, &pmem_size, &nr_pages);
		if (ret < 0) {
			return posix2hmerrno(-ret);
		}
	} else {
		pmem_size = vnode->size;
		nr_pages = ALIGN_UP(vnode->size, PAGE_SIZE) >> PAGE_SHIFT;
	}

	ret = snprintf_s(line_buffer, VMALLOC_INFO_LINE_SIZE, VMALLOC_INFO_LINE_SIZE - 1, "%15lu", pmem_size);
	if (ret < 0) {
		return E_HM_OVERFLOW;
	}
	iter += ret;

	if (vnode->caller != NULL) {
		ret = lnxbase_resolve_caller(buf, VMALLOC_INFO_LINE_SIZE, vnode->caller);
		if (ret < 0) {
			return posix2hmerrno(-ret);
		}
		ret = snprintf_s(line_buffer + iter, VMALLOC_INFO_LINE_SIZE - iter,
				 VMALLOC_INFO_LINE_SIZE - iter - 1, " %50s", buf);
		if (ret < 0) {
			return E_HM_OVERFLOW;
		}
		iter += ret;
	}

	ret = snprintf_s(line_buffer + iter, VMALLOC_INFO_LINE_SIZE - iter, VMALLOC_INFO_LINE_SIZE - iter - 1,
			 " %10u\n", nr_pages);
	if (ret < 0) {
		return E_HM_OVERFLOW;
	}

	return __procfs_ctx_print(ctx, "%s", line_buffer);
}

/* vfs doest not support seq file, so if buf is not large enough,
 * we should return E_HM_FBIG to let vfs retry */
static int vmalloc_info_read_one(struct vmap_node *vnode, void *args)
{
	struct procfs_ctx *ctx = (struct procfs_ctx *)args;
	int ret = E_HM_OK;

	if (____IS(vnode->type & VMAP_TYPE_ALLOC)) {
		ret = vmalloc_info_print_info(ctx, vnode);
	}

	return ret;
}

static int vmalloc_info_show(struct procfs_ctx *ctx)
{
	int ret = 0;
	ret = __procfs_ctx_print(ctx, "Vmalloc info:\n");
	if (ret < 0)
		return ret;
	ret = __procfs_ctx_print(ctx, "----------------------------------------------------\n");
	if (ret < 0)
		return ret;
	ret = __procfs_ctx_print(ctx, "%15s %50s %10s\n", "Size", "PC", "Pages");
	if (ret < 0)
		return ret;
	if (!ctx->is_emergency) {
		ret = vmap_tree_foreach(vmalloc_info_read_one, ctx);
	} else {
		ret = vmap_tree_foreach_trylock(vmalloc_info_read_one, ctx);
	}
	if (ret < 0)
		return ret;
	ret = __procfs_ctx_print(ctx, "----------------------------------------------------\n");
	return ret;
}

static int vmalloc_total_pages(bool is_emergency, unsigned long *total_nr_pages)
{
	int ret;
	if (!is_emergency) {
		*total_nr_pages = liblinux_pal_vmalloc_nr_pages();
		return E_HM_OK;
	} else {
		ret = liblinux_pal_vmalloc_nr_pages_trylock(total_nr_pages);
		if (ret < 0) {
			return posix2hmerrno(-ret);
		} else {
			return E_HM_OK;
		}
	}
}

#define MAX_VMALLOC_SIZE (700 * 1024 * 1024)
int memcheck_vmalloc_info_show(bool is_emergency)
{
	int ret;
	unsigned long long total_nr_pages = 0;
	unsigned long long total;
	struct procfs_ctx ctx = { 0 };

	ret = __kmesg_ctx_init(&ctx, is_emergency);
	if (ret != E_HM_OK) {
		dh_error("%s init procfs_ctx failed, err=%s", __func__, hmstrerror(ret));
		return ret;
	}

	ret = vmalloc_total_pages(is_emergency, &total_nr_pages);
	if (ret != E_HM_OK) {
		/* trylock failed in emergency mode, skip vmalloc_info */
		dh_error("vmalloc_total_pages acquire vmap tree lock failed, err=%s", hmstrerror(ret));
		__kmesg_ctx_deinit(&ctx);
		return E_HM_OK;
	}

	total = total_nr_pages << PAGE_SHIFT;
	__procfs_ctx_print(&ctx, "Total Vmalloc usage is %llu\n", total);
	if (total < MAX_VMALLOC_SIZE) {
		__kmesg_ctx_deinit(&ctx);
		return E_HM_OK;
	}

	ret = vmalloc_info_show(&ctx);
	__kmesg_ctx_deinit(&ctx);
	return ret;
}

static int vmalloc_info_proc_read(const struct kernfs_entry *entry,
				  const struct libdh_kernfs_rw_args *r_args,
				  unsigned long *r_size_out)
{
	int ret = 0;
	struct procfs_ctx ctx = { 0 };

	UNUSED(entry);

	ctx.r_args = r_args;
	ctx.r_size_out = r_size_out;
	ret = __procfs_ctx_init(&ctx);
	if (ret != E_HM_OK) {
		dh_error("%s init procfs_ctx failed, err=%s", __func__, hmstrerror(ret));
		return ret;
	}
	ret = vmalloc_info_show(&ctx);
	__procfs_ctx_deinit(&ctx);
	return ret;
}

static struct libdh_kernfs_ops vmalloc_info_procfs_fops __read_mostly = {
	.read = vmalloc_info_proc_read,
	.write = NULL,
	.poll = NULL,
};

int devhost_vmalloc_info_create_file(void)
{
	int ret = 0;
	struct libdh_procfs_args args = {
		.ops = NULL,
		.ctx = NULL,
		.mode = 0444,
	};

	ret = libdh_procfs_create_file("vmalloc_info", &vmalloc_info_procfs_fops, &args, NULL);
	if (ret < 0) {
		dh_error("create vmalloc_info proc file failed, ret=%s\n", hmstrerror(ret));
	}

	return ret;
}
