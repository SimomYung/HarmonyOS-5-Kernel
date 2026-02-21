/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: File table management
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 23 14:15:39 2019
 */

#include "devhost.h"
#include <devhost/file.h>

#include <vfs.h> /* for 'MODE_BLK' */
#include <libalgo/idr.h>
#include <libalgo/rb_htable.h>
#include <hongmeng/errno.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsync/raw_rwlock_guard.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>

#include "types.h"
#include "devhost_api.h"
#include "devhost_bio.h"
#include "devhost_kobj.h"

static struct easy_htable g_filp_table;
struct filp_data {
	uint64_t cnode_idx;
	struct rbt_hnode node;
	struct idr idr;
};

STATIC_DEFINE_RAW_RWLOCK(__file_mapping_mutex);

#define MAX_FILE_PROCESS 4096
#define FILP_TABLE_BUCKET 64

int devhost_file_mapping_init(void)
{
	return easy_htable_init(&g_filp_table,
				malloc,
				FILP_TABLE_BUCKET,
				MAX_FILE_PROCESS,
				offset_of(struct filp_data, cnode_idx),
				offset_of(struct filp_data, node));
}

PUBLIC_SYMBOL
int devhost_register_file(struct filp_node *node, unsigned int *file_id)
{
	struct filp_data *data = NULL;
	int new_data = 0;
	int ret = E_HM_OK;

	if ((node == NULL) || (file_id == NULL)) {
		return E_HM_INVAL;
	}

	ret = devhost_get_file(node);
	BUG_ON(ret < 0);

	RAW_RWLOCK_WR_GUARD_START(_, &__file_mapping_mutex);
	data = easy_htable_lookup_u64(&g_filp_table, (uint64_t)node->cnode_idx, NULL);
	if (data == NULL) {
		data = malloc(sizeof(struct filp_data));
		if (data == NULL) {
			ret = E_HM_NOMEM;
			goto err_data_out;
		}
		data->cnode_idx = (uint64_t)node->cnode_idx;
		mem_zero_s(data->node);
		idr_init(&data->idr);
		new_data = 1;
	}

	ret = idr_alloc_cyclic(&data->idr, (void *)node, 1, IDR_MAX_ID);
	if (ret < 0) {
		goto err_node_out;
	}
	*file_id = (unsigned int)ret;

	if (new_data == 1) {
		ret = easy_htable_insert_u64(&g_filp_table, (uint64_t)node->cnode_idx,
					     &data->node);
		if (ret < 0) {
			if (ret == E_HM_OBJEXIST) {
				ret = E_HM_POSIX_EXIST;
			}
			goto err_node_out;
		}
	}

	return E_HM_OK;

err_node_out:
	if (new_data == 1) {
		idr_free(&data->idr);
		free(data);
	}
err_data_out:
	RAW_RWLOCK_WR_GUARD_END(_, &__file_mapping_mutex);
	devhost_put_file(node);
	return ret;
}

PUBLIC_SYMBOL
int devhost_unregister_file(uint64_t cnode_idx, unsigned int file_id)
{
	struct filp_data *data = NULL;
	struct filp_node *node = NULL;

	RAW_RWLOCK_WR_GUARD_START(_, &__file_mapping_mutex);

	data = easy_htable_lookup_u64(&g_filp_table, cnode_idx, NULL);
	if (data == NULL) {
		hm_debug("try to unregister a nonexist file\n");
		return E_HM_BADF;
	}

	node = (struct filp_node *)idr_remove(&data->idr, file_id);
	if (node == NULL) {
		hm_debug("try to unregister a nonexist file\n");
		return E_HM_BADF;
	}
	if (idr_is_empty(&data->idr)) {
		if (easy_htable_remove_u64(&g_filp_table, cnode_idx, &data->node) < 0) {
			hm_warn("unlikely to happen, data node does not exist");
		}
		idr_free(&data->idr);
		free(data);
	}
	RAW_RWLOCK_WR_GUARD_END(_, &__file_mapping_mutex);

	devhost_put_file(node);
	return E_HM_OK;
}

static void devhost_release_filp_node(struct raw_compounded_auto_refcnt *ref)
{
	int ret = E_HM_OK;
	struct filp_node *node = NULL;

	node = refcnt_to_filp_node(ref);
	BUG_ON(node == NULL || node->devinfo == NULL);

	/* cleanup poll resources needs to be executed before fops->close */
	devhost_poll_close(node);
	devhost_epoll_close2(node);

	/* call dev fops close if required */
	if ((node->devinfo->p->fops != NULL) && (node->devinfo->p->fops->close != NULL)) {
		ret = node->devinfo->p->fops->close(node->filp);
		if (ret < 0) {
			/* ignore fops close error, just show an warning log */
			hm_warn("fops->close err=%s\n", hmstrerror(ret));
		}
	}

	/* cleanup ctrlmem kobj resources */
	if (node->kobj_ctx != NULL) {
		devhost_put_kobj(node->kobj_ctx);
		node->kobj_ctx = NULL;
	}

	if (node->devinfo->p->vfs_mode == (int)MODE_BLK) {
		devhost_destroy_bio(node, true);
	}

	/* release blk_ctx */
	if ((node->devinfo->p->blkops != NULL) && (node->blk_ctx != NULL)) {
		struct block_ctx *ctx = node->blk_ctx;
		(void)hm_ucap_reject(0, ctx->rref_notify);
		free(ctx);
		node->blk_ctx = NULL;
	}
	devinfo_put(node->devinfo);

	free(node);
}

PUBLIC_SYMBOL
struct filp_node *devhost_alloc_filp_node(struct device_info *devinfo,
					  __u32 cnode_idx)
{
	struct filp_node *node = NULL;

	if (devinfo == NULL) {
		return NULL;
	}
	node = (struct filp_node *)malloc(sizeof(*node));
	if (node == NULL) {
		return NULL;
	}

	mem_zero_s(*node);

	raw_mutex_init(&node->ctx_lock);
	node->private_ctx = NULL;
	/* this refcnt will decrease in devhost_release_filp_node */
	node->devinfo = devinfo_get(devinfo);
	node->cnode_idx = cnode_idx;
	raw_compounded_auto_refcnt_init(&node->refcnt, 1U, devhost_release_filp_node);
	filp_poll_list_init(node);

	return node;
}

static struct filp_node *devhost_find_file_nolock(__u32 cnode_idx,
						  unsigned int file_id)
{
	struct filp_data *data = NULL;

	data = easy_htable_lookup_u64(&g_filp_table, (uint64_t)cnode_idx, NULL);
	if (data == NULL) {
		return NULL;
	}

	return (struct filp_node *)idr_find(&data->idr, file_id);
}

#define FILP_LOCK_GUARD  RAW_RWLOCK_RD_GUARD(_, &__file_mapping_mutex)
define_co_refcnt_function2(__filp_node, offset_of(struct filp_node, refcnt),
			   FILP_LOCK_GUARD, devhost_find_file_nolock,
			   __u32, cnode_idx, unsigned int, file_id)

PUBLIC_SYMBOL
struct filp_node *acquire_filp_node_ptr_coref(__u32 cnode_idx,
					      unsigned int fastpath,
					      unsigned long long file_id)
{
	struct filp_node *node = NULL;

	if (!fastpath) {
		node = acquire___filp_node_ptr_coref(cnode_idx, (unsigned int)file_id);
	} else {
		/*
		 * refer to `vfs_op_query_info` at vfs: if fd is legal but not anonymous fd.
		 * `query_file_id` will return sucess, but the return value(fastpath, file_idx) is invalid.
		 */
		if (file_id == (~0ULL)) {
			return NULL;
		}
		node = u64_to_ptr(file_id, struct filp_node);
		if (node != NULL) {
			if (raw_compounded_auto_refcnt_get(&node->refcnt) <= 0) {
				node= NULL;
			}
		}
	}

	return node;
}
