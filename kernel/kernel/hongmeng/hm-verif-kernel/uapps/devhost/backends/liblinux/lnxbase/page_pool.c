/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Page pool helpers
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 23 11:36:50 2022
 */
#include "page_pool.h"

#include <devhost/log.h>
#include <devhost/memory.h>
#include <libdevhost/procfs.h>
#include <libhwsecurec/securec.h>
#ifdef CONFIG_LNXBASE_BUDDYPOOL
#include "pal/buddypool.h"
#endif
#ifdef CONFIG_LNXBASE_DISCRETEPOOL
#include "pal/discretepool.h"
#endif
#include "pal/mempool.h"
#include "internal.h"

#include "../../../devhost_backend.h"

struct page_pool_group {
	unsigned int pool_num;
	struct page_pool *pool[MAX_POOL_NUM];
};
struct page_pool_group pool_group = {0};

/*
 * Table of relationships between devhost_pool_type and the corresponding
 * creation functions. pool_create_by_type function will just follow
 * the records in this table and create the requested pool.
 *
 * Every time a new type is added or a relationship is changed, just
 * need to modify the content in this table, which will be more clear
 * and easyier to maintain.
 */
static struct page_pool *(*pool_create_factory_table[DEVHOST_POOL_TYPE_MAX])
					(struct pool_info *, unsigned int) = {
	[DEVHOST_POOL_TYPE_NONE] = NULL,
	[DEVHOST_POOL_TYPE_MEM] = mempool_allocator_create,
#ifdef CONFIG_LNXBASE_BUDDYPOOL
	[DEVHOST_POOL_TYPE_CMA] = buddypool_allocator_create,
#else
	[DEVHOST_POOL_TYPE_CMA] = NULL,
#endif
#ifdef CONFIG_LNXBASE_DISCRETEPOOL
	[DEVHOST_POOL_TYPE_DISCRETE] = discretepool_allocator_create,
#else
	[DEVHOST_POOL_TYPE_DISCRETE] = NULL,
#endif
};

static struct page_pool *pool_create_by_type(enum devhost_pool_type type,
					     struct pool_info *info,
					     unsigned int page_sizeorder)
{
	struct page_pool *pool = NULL;

	pool = pool_create_factory_table[type](info, page_sizeorder);
	if (pool != NULL) {
		pool_group.pool[pool_group.pool_num++] = pool;
	}

	return pool;
}

struct page_pool *pool_creat_mem(struct pool_info *info,
				 unsigned int page_sizeorder)
{
	return pool_create_by_type(DEVHOST_POOL_TYPE_MEM, info, page_sizeorder);
}

int lnxbase_page_pool_init(unsigned int page_sizeorder)
{
	struct page_pool *pool = NULL;
	unsigned int pool_num;
	enum devhost_pool_type type;

	pool_num = devhost_get_pool_num();
	for (unsigned int pool_idx = 0; pool_idx < pool_num; pool_idx++) {
		type = devhost_get_pool_type(pool_idx);
		if (pool_create_factory_table[type] == NULL) {
			dh_error("pal page alloc pool type error, type=%#x\n", (unsigned int)type);
		} else {
			pool = pool_create_by_type(type, devhost_get_pool_info(pool_idx),
					page_sizeorder);
			BUG_ON(pool == NULL);
		}
	}

	return E_HM_OK;
}

struct page_pool *lnxbase_page_pool_get(const char *name)
{
	for (unsigned int pool_idx = 0; pool_idx < pool_group.pool_num; pool_idx++) {
		if (pool_group.pool[pool_idx] == NULL) {
			return NULL;
		}
		if (strcmp(name, pool_group.pool[pool_idx]->info->name) == 0) {
			return pool_group.pool[pool_idx];
		}
	}
	return NULL;
}

int lnxbase_page_pool_setup(struct page_pool *pool, const struct pool_base *_base)
{
	struct pool_base *base = NULL;
	int ret = E_HM_OK;

	if (pool == NULL || _base == NULL) {
		return E_HM_INVAL;
	}

	/* return error if already setup */
	if (pool->base != NULL) {
		return E_HM_BUSY;
	}

	base = (struct pool_base *)malloc(sizeof(*base));
	if (base == NULL) {
		return E_HM_NOMEM;
	}

	NOFAIL(memset_s(base, sizeof(*base), 0, sizeof(struct pool_base)));

	/* copy pool base parameters */
	base->kmap_virt_start = _base->kmap_virt_start;
	base->kmap_virt_end = _base->kmap_virt_end;
	base->ram_phys_start = _base->ram_phys_start;

	/* set poll->base */
	pool->base = base;

	if (pool->setup != NULL) {
		ret = (*pool->setup)(pool);
	}

	if (ret < 0) {
		free(base);
		pool->base = NULL;
	}

	return ret;
}

void page_pool_init(struct page_pool *pool, unsigned int page_sizeorder,
		    struct pool_info *info, page_pool_setup_cb setup)
{
	mem_zero_s(*pool);

	pool->info = info;
	pool->cur_size = 0;
	pool->page_sizeorder = page_sizeorder;
	pool->watermark = 1UL << (info->reclaimorder + page_sizeorder);
	pool->base = NULL;
	pool->setup = setup;
	raw_atomic_ullong_init(&pool->alloc_size, 0);
}

void page_pool_destroy(struct page_pool *pool)
{
	if (pool->base != NULL) {
		/* malloc in `lnxbase_page_pool_setup` */
		free(pool->base);
	}
}

void *page_pool_prepare_map(const struct page_pool *pool,
			    unsigned long long paddr, unsigned long size)
{
	if ((pool == NULL) || (pool->base == NULL)) {
		return NULL;
	}
	return devhost_backend_prepare_map(paddr, size);
}

void page_pool_prepare_unmap(const struct page_pool *pool,
			     void *vaddr, unsigned long size)
{
	if ((pool == NULL) || (pool->base == NULL)) {
		return;
	}
	devhost_backend_prepare_unmap(vaddr, size);
}

static int pool_get_stat(struct page_pool *pool, char *outbuf, unsigned long outbuf_size, unsigned long *rsize)
{
	int ret_len = 0;
	int ret;
	if (pool == NULL) {
		return E_HM_INVAL;
	}
	*rsize = 0;

	ret = sprintf_s(outbuf, outbuf_size, "page pool %s stat:\n", pool->info->name);
	if (ret < 0) {
		return E_HM_FBIG;
	}
	ret_len += ret;
	ret = sprintf_s(outbuf + ret_len, outbuf_size - (unsigned int)ret_len, "%-20s%lu B\n",
				" cur size:", pool->cur_size);
	if (ret < 0) {
		return E_HM_FBIG;
	}
	ret_len += ret;
	ret = sprintf_s(outbuf + ret_len, outbuf_size - (unsigned int)ret_len, "%-20s%lu B\n",
				" alloc size:",
				(unsigned long)raw_atomic_ullong_read_relaxed(&pool->alloc_size));
	if (ret < 0) {
		return E_HM_FBIG;
	}
	ret_len += ret;

	ret = sprintf_s(outbuf + ret_len, outbuf_size - (unsigned int)ret_len, "%-20s%#lu B\n\n",
				" watermark:", pool->watermark);
	if (ret < 0) {
		return E_HM_FBIG;
	}
	ret_len += ret;

	*rsize = (unsigned long)(unsigned int)ret_len;
	return 0;
}

static int pool_info_get_stat(const struct page_pool *pool,
			      char *outbuf,
			      unsigned long outbuf_size,
			      unsigned long *rsize)
{
	int ret_len = 0;
	int ret = 0;
	if (pool == NULL) {
		return E_HM_INVAL;
	}
	*rsize = 0;

	ret = sprintf_s(outbuf, outbuf_size, "page pool %s info stat:\n", pool->info->name);
	if (ret < 0) {
		return E_HM_FBIG;
	}
	ret_len += ret;
	ret = sprintf_s(outbuf + ret_len, outbuf_size - (unsigned int)ret_len, "%-20s%#-20x\n",
				" pool type:", pool->info->type);
	if (ret < 0) {
		return E_HM_FBIG;
	}
	ret_len += ret;
	ret = sprintf_s(outbuf + ret_len, outbuf_size - (unsigned int)ret_len, "%-20s%#-20x\n\n",
				" max size order:", pool->info->sizeorder);
	if (ret < 0) {
		return E_HM_FBIG;
	}
	ret_len += ret;
	*rsize = (unsigned long)(unsigned int)ret_len;
	return 0;
}

static int pool_base_get_stat(const struct page_pool *pool,
			      char *outbuf,
			      unsigned long outbuf_size,
			      unsigned long *rsize)
{
	int retlen = 0;
	int ret;
	if (pool == NULL) {
		return E_HM_INVAL;
	}
	*rsize = 0;

	ret = sprintf_s(outbuf, outbuf_size, "page pool %s base stat:\n", pool->info->name);
	if (ret < 0) {
		return E_HM_FBIG;
	}
	retlen += ret;
	ret = sprintf_s(outbuf + retlen, outbuf_size - (unsigned int)retlen, "%-20s%#-20x\n\n",
				" page size order:", pool->page_sizeorder);
	if (ret < 0) {
		return E_HM_FBIG;
	}
	retlen += ret;
	*rsize = (unsigned long)(unsigned int)retlen;
	return 0;
}

static int __mempool_procfs_pool_stat(const struct libdh_procfs_args *args, char *proc_buf,
				      unsigned long buf_size, unsigned long *r_size_out)
{
	int ret;
	unsigned long rsize = 0;
	struct page_pool *pool = NULL;
	page_pool_extend_stat_fn statfn = NULL;

	statfn = ptr_to_type(args->ops, page_pool_extend_stat_fn);
	pool = (struct page_pool *)args->ctx;

	ret = pool_get_stat(pool, proc_buf, buf_size, &rsize);
	if (ret < 0) {
		return ret;
	}
	*r_size_out = rsize;

	ret = pool_info_get_stat(pool, proc_buf + *r_size_out,
				 buf_size - *r_size_out, &rsize);
	if (ret < 0) {
		return ret;
	}
	*r_size_out += rsize;

	/* dump pool mapping info if setup */
	if (pool->base == NULL) {
		ret = sprintf_s(proc_buf + *r_size_out, buf_size - *r_size_out,
				"page pool %s base stat: ignore, not setup\n\n", pool->info->name);
		if (ret >= 0) {
			rsize = ret;
		}
	} else {
		ret = pool_base_get_stat(pool, proc_buf + *r_size_out,
					 buf_size - *r_size_out, &rsize);
	}
	if (ret < 0) {
		return ret;
	}
	*r_size_out += rsize;

	/* allocator customized stat */
	if ((pool->base != NULL) && (statfn != NULL)) {
		ret = (*statfn)(pool, proc_buf + *r_size_out,
				buf_size - *r_size_out, &rsize);
		if (ret < 0) {
			return ret;
		}
		*r_size_out += rsize;
	}

	return ret;
}

#define MEMPOOL_PROC_BUF_LEN 8192
static int __mempool_procfs_read(const struct kernfs_entry *entry,
				const struct libdh_kernfs_rw_args *r_args,
				unsigned long *r_size_out)
{
	int ret;
	const struct libdh_procfs_args *args = NULL;
	char *proc_buf = NULL;

	if (r_args == NULL) {
		return E_HM_INVAL;
	}

	args = libdh_procfs_acquire_args(entry);
	if ((args == NULL) || (args->ctx == NULL) || (r_size_out == NULL)) {
		return E_HM_INVAL;
	}

	proc_buf = (char *)malloc(MEMPOOL_PROC_BUF_LEN);
	if (proc_buf == NULL) {
		return E_HM_NOMEM;
	}

	ret = __mempool_procfs_pool_stat(args, proc_buf, MEMPOOL_PROC_BUF_LEN, r_size_out);
	if (ret >= 0) {
		ret = devhost_pal_copy_to_user(ulong_to_ptr(r_args->ubuf, void), (void *)proc_buf, *r_size_out);
	}

	free(proc_buf);
	return ret;
}

static struct libdh_kernfs_ops mempool_procfs_fops __read_mostly = {
	.read = __mempool_procfs_read,
	.write = NULL,
	.poll = NULL,
};

struct pagepool_procfs_data {
	struct page_pool *pool;
	page_pool_extend_stat_fn statfn;
};

static int __mempool_procfs_create_file(void *_data)
{
	struct pagepool_procfs_data *data = (struct pagepool_procfs_data *)_data;
	struct libdh_procfs_args args;
	int ret;

	args.ops = data->statfn;
	args.ctx = data->pool;
	args.mode = S_IFREG;

	ret = libdh_procfs_create_file("%s", &mempool_procfs_fops, &args,
					NULL, data->pool->info->name);
	if (ret < 0) {
		dh_error("mempool procfs \"%s\" create failed, ret=%s\n",
			 data->pool->info->name, hmstrerror(ret));
	}

	free(data);

	return ret;
}

int page_pool_procfs_register(struct page_pool *pool, page_pool_extend_stat_fn statfn)
{
	struct pagepool_procfs_data *data = NULL;
	int ret;

	data = (struct pagepool_procfs_data *)malloc(sizeof(*data));
	if (data == NULL) {
		return E_HM_NOMEM;
	}

	data->pool = pool;
	data->statfn = statfn;

	ret = devhost_postinit(__mempool_procfs_create_file, data);
	if (ret < 0) {
		free(data);
	}
	return ret;
}
