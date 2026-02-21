/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Sysif handler for querying file info
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 14 09:10:55 2023
 */
#include <fs_server.h>
#include <hongmeng/errno.h>
#include <lnxbase/lnxbase.h>
#include <lnxbase/export.h>
#include <devhost/backend.h>
#include <devhost/file.h>
#include <devhost/plugin.h>
#include <libsysif/devhost/server.h>
#include <libhmactv/actv.h>
#include "file_info_api.h"
#include "file_info_ext.h"

#define min(x, y) ((x) > (typeof(x))(y) ? (typeof(x))(y) : (x))

static struct devhost_file_info_ops *file_info_ops;

static uint32_t vfs_cnode_idx = 0;

int devhost_file_info_vfs_cnode_idx_init(void)
{
	struct server_info *server = NULL;
	int ret = get_fsmgr_si(&server);
	if (ret != E_HM_OK) {
		hm_error("failed to get fsmgr server: err=%s\n", hmstrerror(ret));
		return ret;
	}

	vfs_cnode_idx = server->cnode_idx;
	put_server_info(server);
	return E_HM_OK;
}

PUBLIC_SYMBOL
int devhost_file_info_ops_init(struct devhost_file_info_ops *ops)
{
	BUG_ON(ops == NULL);

	file_info_ops = ops;
	return 0;
}
EXPORT_SYMBOL(devhost_file_info_ops_init);

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_file_info_query_dmabuf, dh_file_info_query_dmabuf_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, unsigned long long, file_idx)
{
	struct __actvret_drvcall_devhost_file_info_query_dmabuf *pret =
		actvhdlr_drvhandler_devhost_file_info_query_dmabuf_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL);
	int ret;
	struct filp_node *node = NULL;

	acquire_devhost_filp_node(node, vfs_cnode_idx, fastpath, file_idx);
	if (node == NULL || node->filp == NULL) {
		ret = E_HM_INVAL;
		goto out;
	}

	BUG_ON(file_info_ops == NULL);

	ret = file_info_ops->fill_dma_buf_info(node->filp, &pret->dma_buf_info);
	if (ret < 0) {
		ret = E_HM_PERM;
		goto out;
	}
	ret = E_HM_OK;

out:
	if (fastpath) {
		/* vfs_query_info flags=IO_DEV_FASTPATH_GET will get fastpath filp_node */
		devhost_put_file(node);
	}
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_file_info_query_fence, dh_file_info_query_fence_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, unsigned long long, file_idx,
			 unsigned int, offset)
{
	struct __actvret_drvcall_devhost_file_info_query_fence *pret =
		actvhdlr_drvhandler_devhost_file_info_query_fence_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL);
	int ret;
	unsigned int total_num_fences, num_fences;
	struct filp_node *node = NULL;

	acquire_devhost_filp_node(node, vfs_cnode_idx, fastpath, file_idx);
	if (node == NULL || node->filp == NULL) {
		ret = E_HM_INVAL;
		goto out;
	}

	BUG_ON(file_info_ops == NULL);

	ret = file_info_ops->fill_fence_info(node->filp, offset, FENCE_ARRAY_LEN,
					     &pret->sync_file_info.dma_fence_info,
					     pret->sync_file_info.fences_array);
	if (ret < 0) {
		ret = E_HM_PERM;
		goto out;
	}

	total_num_fences = pret->sync_file_info.dma_fence_info.total_num_fences;
	num_fences = pret->sync_file_info.dma_fence_info.num_fences;
	if (offset + num_fences < total_num_fences) {
		return E_HM_AGAIN;
	}
	ret = E_HM_OK;

out:
	if (fastpath) {
		/* vfs_query_info flags=IO_DEV_FASTPATH_GET will get fastpath filp_node */
		devhost_put_file(node);
	}
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_file_info_query_ion, dh_file_info_query_ion_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, fastpath, unsigned long long, file_idx)
{
	struct __actvret_drvcall_devhost_file_info_query_ion *pret =
		actvhdlr_drvhandler_devhost_file_info_query_ion_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL);
	int ret;
	struct filp_node *node = NULL;

	acquire_devhost_filp_node(node, vfs_cnode_idx, fastpath, file_idx);
	if (node == NULL || node->filp == NULL) {
		ret = E_HM_INVAL;
		goto out;
	}

	BUG_ON(file_info_ops == NULL);

	ret = file_info_ops->fill_ion_info(node->filp, &pret->ion_info);
	if (ret < 0) {
		ret = E_HM_PERM;
		goto out;
	}
	ret = E_HM_OK;

out:
	if (fastpath) {
		/* vfs_query_info flags=IO_DEV_FASTPATH_GET will get fastpath filp_node */
		devhost_put_file(node);
	}
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_file_info_query_meminfo, dh_file_info_query_meminfo_ext,
			 unsigned long long, sender, unsigned long, credential)
{
	struct __actvret_drvcall_devhost_file_info_query_meminfo *pret =
		actvhdlr_drvhandler_devhost_file_info_query_meminfo_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL);
	int ret;

	BUG_ON(file_info_ops == NULL);

	ret = file_info_ops->fill_mem_info(&pret->devhost_mem_info);
	if (ret < 0)
		return posix2hmerrno(-ret);

	lnxbase_pages_stat(&pret->devhost_mem_info);

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_file_info_lowmem_debug, dh_file_info_lowmem_debug_ext,
						 unsigned long long, sender, unsigned long, credential, int, oom_score_adj,
						 unsigned long long, mem_total, bool, is_emergency)
{
	UNUSED(sender, credential);
	int ret;

	BUG_ON(file_info_ops == NULL);

	ret = memcheck_vmalloc_info_show(is_emergency);
	if (ret < 0)
		return posix2hmerrno(-ret);

	ret = file_info_ops->signal_lowmem_debug((short)oom_score_adj, mem_total, is_emergency);
	if (ret < 0)
		return posix2hmerrno(-ret);

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_file_info_query_gpu, dh_file_info_query_gpu_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned long, pid)
{
	struct __actvret_drvcall_devhost_file_info_query_gpu *pret =
		actvhdlr_drvhandler_devhost_file_info_query_gpu_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL);
	int ret;

	BUG_ON(file_info_ops == NULL);
	unsigned long gpu_mem;
	ret = file_info_ops->fill_gpu_info(pid, &gpu_mem);
	pret->gpu_info.size = gpu_mem;
	if (ret < 0)
		return posix2hmerrno(-ret);

	return E_HM_OK;
}

/* load file_info module */
static int file_info_load_module(struct advice_ctx *ctx)
{
	int ret;

	ret = devhost_pal_request_module("file_info.ko");
	if (ret < 0) {
		hm_error("lnxbase: load file_info.ko failed, %s\n", hmstrerror(ret));
		return ret;
	}

	return ctx->call_next(ctx);
}

DEFINE_ADVICE_EXTENSION(dh_backend_done_ext, NULL, file_info_load_module, NULL);

/*
 * file_info sysif module
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_file_info_query_dmabuf`
 *
 * dependence: NULL
 */
DEFINE_DSO_PLUGIN(file_info, NULL,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_file_info_query_dmabuf, dh_file_info_query_dmabuf_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_file_info_query_fence, dh_file_info_query_fence_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_file_info_query_ion, dh_file_info_query_ion_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_file_info_query_meminfo, dh_file_info_query_meminfo_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_file_info_lowmem_debug, dh_file_info_lowmem_debug_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_file_info_query_gpu, dh_file_info_query_gpu_ext),
	IMPORT_ADVICE_EXTENSION(devhost_backend_init_done, dh_backend_done_ext)
)
