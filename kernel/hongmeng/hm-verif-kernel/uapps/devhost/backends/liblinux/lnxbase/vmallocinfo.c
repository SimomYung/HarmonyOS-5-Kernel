/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Implementation for /proc/vmallocinfo
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jun 12 20:54:26 2022
 */
#include <stdarg.h>
#include <devhost/log.h>
#include <devhost/memory.h>
#include <libmem/types.h>
#include <libhmelf/vsymbol.h>
#include <libdevhost/procfs.h>
#include <lnxbase/vmap.h>
#include <liblinux/pal.h>
#include "internal.h"

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

	return E_HM_OK;
}

static void __procfs_ctx_deinit(struct procfs_ctx *ctx)
{
	free(ctx->tmp);
	ctx->tmp = NULL;
	free(ctx->vec);
	ctx->vec = NULL;
}

static int __procfs_ctx_write(struct procfs_ctx *ctx,
			      const char *fmt, ...)
{
	int ret = 0;
	unsigned long size = 0;
	va_list args;
	va_start(args, fmt);
	ret = vsprintf_s(ctx->tmp, ctx->tmp_sz, fmt, args);
	if (ret < 0) {
		ret = E_HM_OVERFLOW;
	}
	va_end(args);
	if (ret >= 0) {
		size = (unsigned long)ret;
		if ((ctx->size - ctx->used) <= size) {
			return E_HM_FBIG;
		}
		ret = devhost_pal_copy_to_user(ptr_add(ctx->buf, ctx->used), ctx->tmp, size);
		if (ret == E_HM_OK) {
			ctx->used += size;
			*(ctx->r_size_out) += size;
		}
	}

	return ret;
}

static int __vmallocinfo_read_basic(struct procfs_ctx *ctx, struct vmap_node *vnode)
{
	int ret = 0;
#ifdef CONFIG_VMALLOC_VIRT_PAGES
	unsigned int nr_pages = 0;
	unsigned long pmem_size = 0;

	ret = liblinux_pal_vmalloc_pmem_stat(vnode, ctx->vec, &pmem_size, &nr_pages);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	ret = __procfs_ctx_write(ctx, "0x%llx-0x%llx %7lu",
				 (unsigned long long)ptr_to_ulong(vnode->vaddr),
				 (unsigned long long)ptr_to_ulong(vnode->vaddr) + vnode->size,
				 pmem_size);
#else
	ret = __procfs_ctx_write(ctx, "0x%llx-0x%llx %7ld",
				 (unsigned long long)ptr_to_ulong(vnode->vaddr),
				 (unsigned long long)ptr_to_ulong(vnode->vaddr) + vnode->size,
				 vnode->size);
#endif
	if (ret < 0) {
		return ret;
	}

	if (vnode->caller != NULL) {
		char *buf = (char *)malloc(BUF_SZ);
		if (buf == NULL) {
			return E_HM_NOMEM;
		}
		ret = lnxbase_resolve_caller(buf, BUF_SZ, vnode->caller);
		if (ret >= 0) {
			ret = __procfs_ctx_write(ctx, " %s", buf);
		}
		free(buf);
	}
	if (ret < 0) {
		return ret;
	}

#ifdef CONFIG_VMALLOC_VIRT_PAGES
	ret = __procfs_ctx_write(ctx, " pages=%d", nr_pages);
#else
	ret = __procfs_ctx_write(ctx, " pages=%d", ALIGN_UP(vnode->size, PAGE_SIZE) >> PAGE_SHIFT);
#endif

	return ret;
}

static int __vmallocinfo_read_ioremap(struct procfs_ctx *ctx, struct vmap_node *vnode)
{
	int ret = 0;

	struct ioremap_node *node = container_of(vnode, struct ioremap_node, vmap);

	ret = __procfs_ctx_write(ctx, " phys=0x%lx", node->phys_addr);
	if (ret < 0) {
		return ret;
	}
	ret = __procfs_ctx_write(ctx, " ioremap");

	return ret;
}

static int __vmallocinfo_read_vmalloc(struct procfs_ctx *ctx, const struct vmap_node *vnode)
{
	int ret = 0;

	(void)vnode;

	ret = __procfs_ctx_write(ctx, " vmalloc");

	return ret;
}

static int __vmallocinfo_read_vmap(struct procfs_ctx *ctx, const struct vmap_node *vnode)
{
	int ret = 0;

	(void)vnode;

	ret = __procfs_ctx_write(ctx, " vmap");

	return ret;
}

static int __vmallocinfo_read_dma_coherent(struct procfs_ctx *ctx, const struct vmap_node *vnode)
{
	int ret = 0;

	(void)vnode;

	ret = __procfs_ctx_write(ctx, " dma-coherent");

	return ret;
}

static int __vmallocinfo_read_page(struct procfs_ctx *ctx, const struct vmap_node *vnode)
{
	int ret = 0;

	(void)vnode;

	ret = __procfs_ctx_write(ctx, " vpages");

	return ret;
}

/* vfs doest not support seq file, so if buf is not large enough,
 * we should return E_HM_FBIG to let vfs retry */
static int __vmallocinfo_read_one(struct vmap_node *vnode, void *args)
{
	struct procfs_ctx *ctx = (struct procfs_ctx *)args;
	int ret = 0;

	ret = __vmallocinfo_read_basic(ctx, vnode);
	if (ret < 0) {
		return ret;
	}

	if (____IS(vnode->type & VMAP_TYPE_IOREMAP)) {
		ret = __vmallocinfo_read_ioremap(ctx, vnode);
		if (ret < 0) {
			return ret;
		}
	}

	if (____IS(vnode->type & VMAP_TYPE_ALLOC)) {
		ret = __vmallocinfo_read_vmalloc(ctx, vnode);
		if (ret < 0) {
			return ret;
		}
	}

	if (____IS(vnode->type & VMAP_TYPE_MAP)) {
		ret = __vmallocinfo_read_vmap(ctx, vnode);
		if (ret < 0) {
			return ret;
		}
	}

	if (____IS(vnode->type & VMAP_TYPE_DMA_COHERENT)) {
		ret = __vmallocinfo_read_dma_coherent(ctx, vnode);
		if (ret < 0) {
			return ret;
		}
	}

	if (____IS(vnode->type & (VMAP_TYPE_KMAP | VMAP_TYPE_VMEMMAP))) {
		ret = __vmallocinfo_read_page(ctx, vnode);
		if (ret < 0) {
			return ret;
		}
	}

	ret = __procfs_ctx_write(ctx, "\n");

	return ret;
}

static int __vmallocinfo_read(const struct kernfs_entry *entry,
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
	ret = vmap_tree_foreach(__vmallocinfo_read_one, &ctx);
	__procfs_ctx_deinit(&ctx);
	return ret;
}

static struct libdh_kernfs_ops vmallocinfo_procfs_fops __read_mostly = {
	.read = __vmallocinfo_read,
	.write = NULL,
	.poll = NULL,
};

int lnxbase_vmallocinfo_init(void)
{
	int ret = 0;
	struct libdh_procfs_args args = {
		.ops = NULL,
		.ctx = NULL,
		.mode = 0400,
	};

	dh_debug("init vmallocinfo\n");
	ret = libdh_procfs_create_file("vmallocinfo", &vmallocinfo_procfs_fops, &args, NULL);
	if (ret < 0) {
		dh_error("init vmallocinfo failed, ret=%s\n", hmstrerror(ret));
	}

	return ret;
}

static bool __enabled = false;
int lnxbase_enable_vmallocinfo(void)
{
	__enabled = true;
	return 0;
}

bool lnxbase_vmallocinfo_enabled(void)
{
	return __enabled;
}
