/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Implementation of shared memory list
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 10:50:28 2019
 */
/* direct header */
#include "libshlist/shlist.h"

/* libs */
#include <fcntl.h>
#include <string.h>

/* hongmeng ulibs */
#include <libmem/utils.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_mem.h>

static int shm_name(const char *prefix, uint32_t shm_idx,
		    struct memmgr_shm_name *n)
{
	int ret;

	ret = sprintf_s(n->path, sizeof(n->path), "%s%u", prefix, shm_idx);
	if (ret < 0) {
		hm_error("failed to copy shm name\n");
		ret = E_HM_INVAL;
	} else {
		ret = E_HM_OK;
	}

	return ret;
}

static int shm_create_and_open(const struct memmgr_shm_name *name, bool is_creator,
			       size_t *size, int *shm_id)
{
	int id;
	uint64_t oflags = 0;
	int ret = E_HM_OK;

	if (is_creator) {
		oflags |= (uint64_t)O_CREAT;
	}
	id = hm_mem_shm_open_with_mode((const char *)name->path, *size, oflags,
					(unsigned)S_IRUSR | (unsigned)S_IRGRP | (unsigned)S_IROTH, size);
	if (id < 0) {
		hm_error("failed to open shm\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		*shm_id = id;
	}

	return ret;
}

static void shm_close_and_unlink(const struct memmgr_shm_name *name, bool is_creator,
				 int shm_id)
{
	CERTAIN_NOFAIL(hm_mem_shm_close(shm_id));
	if (is_creator) {
		CERTAIN_NOFAIL(hm_mem_shm_unlink((const char *)name->path));
	}
}

static int _shl_shm_open(struct shl_shm_list *node, uint32_t shm_idx,
			 const char *prefix, size_t size, bool is_creator)
{
	struct memmgr_shm_name n = {0};
	void *addr = NULL;
	uint32_t prot = is_creator ? ((unsigned)PROT_WRITE | (unsigned)PROT_READ) : (unsigned)PROT_READ;
	int shm_id, rc;

	rc = shm_name(prefix, shm_idx, &n);
	if (rc == E_HM_OK) {
		rc = shm_create_and_open(&n, is_creator, &size, &shm_id);
	}

	if (rc == E_HM_OK) {
		addr = hm_mem_mmap(NULL, size, prot, MAP_SHARED | MAP_ANONYMOUS, shm_id, 0);
		if (addr == MAP_FAILED) {
			hm_error("failed to map shm\n");
			shm_close_and_unlink(&n, is_creator, shm_id);
			rc = E_HM_NOMEM;
		}
	}

	if (rc == E_HM_OK) {
		node->name = strdup(n.path);
		if (node->name == NULL) {
			hm_error("failed to dup shm name\n");
			CERTAIN_NOFAIL(hm_mem_munmap(addr, size));
			shm_close_and_unlink(&n, is_creator, shm_id);
			rc = E_HM_NOMEM;
		}
	}

	if (rc == E_HM_OK) {
		node->base = ptr_to_type(addr, uintptr_t);
		node->size = (uint32_t)size;
		node->shm_idx = shm_idx;
		CERTAIN_NOFAIL(hm_mem_shm_close(shm_id));
	}
	return rc;
}

static struct shl_shm_list *shl_shm_open(struct shlist *shl, uint32_t shm_idx)
{
	struct shl_shm_list *node = NULL;
	bool is_creator = shl->ctrl != NULL;
	size_t size = is_creator ? (size_t)shl->ctrl->shm_size : (size_t)0;
	int rc = E_HM_OK;

	node = malloc(sizeof(struct shl_shm_list));
	if (node == NULL) {
		hm_error("failed to malloc shm node\n");
		rc = E_HM_NOMEM;
	}

	if (rc == E_HM_OK) {
		rc = _shl_shm_open(node, shm_idx, shl->shm_prefix, size, is_creator);
		if (rc != E_HM_OK) {
			hm_error("failed to open shm, rc=%s, shm_idx=%"PRIu32"\n",
				 hmstrerror(rc), shm_idx);
			free(node);
			node = NULL;
		} else {
			dlist_insert_head(&shl->shm_list, &node->link);
			shl->shm_count++;
		}
	}

	return node;
}

static void shl_shm_close(struct shl_shm_list *node, bool is_creator)
{
	dlist_delete(&node->link);
	CERTAIN_NOFAIL(hm_mem_munmap(ptr_to_void(node->base), node->size));
	if (is_creator) {
		CERTAIN_NOFAIL(hm_mem_shm_unlink(node->name));
	}
	free(node->name);
	node->name = NULL;
}

static struct shl_shm_list *shl_shm_find(const struct shlist *shl, uint32_t shm_idx)
{
	struct shl_shm_list *node = NULL;
	struct shl_shm_list *res = NULL;
	dlist_for_each_entry(node, &shl->shm_list, struct shl_shm_list, link) {
		if (node->shm_idx == shm_idx) {
			res = node;
			break;
		}
	}
	return res;
}

uint32_t shl_shm_size_of(struct shlist *shl, uint32_t shm_idx)
{
	struct shl_shm_list *shm = NULL;
	uint32_t size = 0;

	if (shl != NULL) {
		raw_mutex_lock(&shl->shm_list_lock);
		shm = shl_shm_find(shl, shm_idx);
		if (shm == NULL) {
			shm = shl_shm_open(shl, shm_idx);
		}
		if (shm != NULL) {
			size = shm->size;
		}
		raw_mutex_unlock(&shl->shm_list_lock);
	}

	return size;
}

uintptr_t shl_shm_base_of(struct shlist *shl, uint32_t shm_idx)
{
	struct shl_shm_list *shm = NULL;
	uintptr_t base = 0;

	if (shl != NULL) {
		raw_mutex_lock(&shl->shm_list_lock);
		shm = shl_shm_find(shl, shm_idx);
		if (shm == NULL) {
			shm = shl_shm_open(shl, shm_idx);
		}
		if (shm != NULL) {
			base = shm->base;
		}
		raw_mutex_unlock(&shl->shm_list_lock);
	}

	return base;
}

static inline void shl_init_node(struct shl_list_node *node,
				 struct shl_shm_list *shm, uint32_t offset,
				 uint32_t size, bool is_free)
{
	node->shm = shm;
	node->offset = offset;
	node->size = size;
	node->free = is_free;
}

static struct shl_list_node *shl_split_free_node(struct shlist *shl,
						 struct shl_list_node *node,
						 uint32_t split_size)
{
	struct shl_list_node *split_node = NULL;
	struct shl_list_node *res = NULL;

	if (split_size > node->size) {
		hm_error("split size is too large\n");
	} else if (split_size == node->size) {
		dlist_delete(&node->free_link);
		node->free = false;
		res = node;
	} else {
		split_node = malloc(sizeof(struct shl_list_node));
		if (split_node == NULL) {
			hm_error("failed to malloc shl node\n");
		} else {
			dlist_insert_head(&shl->ctrl->free_list, &split_node->free_link);
			shl_init_node(split_node, node->shm, node->offset + split_size,
				      node->size - split_size, true);
			dlist_insert(&node->list_link, &split_node->list_link);
			node->size = split_size;
			dlist_delete(&node->free_link);
			node->free = false;
			res = node;
		}
	}

	return res;
}

/*
 * Function Description:
 * The nodes that are set to free and have the same index are combined into the "node",
 * and the memory blocks of these nodes are managed by the "node".
 * The "node" will be set to free and inserted into the shl->ctrl->free_list.
 * */
void shl_free(struct shlist *shl, struct shl_list_node *node)
{
	struct dlist_node *current = NULL;
	struct shl_list_node *tmp = NULL;

	if (shl == NULL || shl->ctrl == NULL || node == NULL) {
		hm_warn("invalid parameters\n");
	} else {
		RAW_MUTEX_GUARD(_, &shl->ctrl->list_lock);
		dlist_insert_head(&shl->ctrl->free_list, &node->free_link);

		current = dlist_get_next(&node->list_link);
		tmp = container_of(current, struct shl_list_node, list_link);
		if (current != &shl->ctrl->list &&
		    tmp->free &&
		    tmp->shm == node->shm) {
			node->size += tmp->size;
			dlist_delete(&tmp->free_link);
			dlist_delete(&tmp->list_link);
			free(tmp);
		}

		current = dlist_get_prev(&node->list_link);
		tmp = container_of(current, struct shl_list_node, list_link);
		if (current != &shl->ctrl->list &&
		    tmp->free &&
		    tmp->shm == node->shm) {
			node->size += tmp->size;
			node->offset = tmp->offset;
			dlist_delete(&tmp->free_link);
			dlist_delete(&tmp->list_link);
			free(tmp);
		}
		node->free = true;
	}
}

static struct shl_list_node *shl_extend(struct shlist *shl)
{
	struct shl_list_node *new_node = NULL;
	struct shl_shm_list *shm = NULL;
	uint32_t new_shm_idx = shl->shm_count;

	new_node = malloc(sizeof(struct shl_list_node));
	if (new_node == NULL) {
		hm_error("failed to malloc shl_list_node\n");
	} else {
		shm = shl_shm_open(shl, new_shm_idx);
		if (shm == NULL) {
			hm_error("failed to open shm\n");
			free(new_node);
			new_node = NULL;
		}
	}
	if (new_node != NULL) {
		shl_init_node(new_node, shm, 0, shl->ctrl->shm_size, true);
		dlist_insert_head(&shl->ctrl->list, &new_node->list_link);
		dlist_insert_head(&shl->ctrl->free_list, &new_node->free_link);
	}

	return new_node;
}

struct shl_list_node *shl_alloc(struct shlist *shl, size_t size)
{
	struct shl_list_node *pos = NULL;
	struct shl_list_node *n = NULL;
	struct shl_list_node *free_node = NULL;
	bool need_extend = true;

	if (shl == NULL || shl->ctrl == NULL) {
		hm_error("invalid parameters\n");
	} else if (size > shl->ctrl->shm_size) {
		hm_error("list size is too large\n");
	} else {
		RAW_MUTEX_GUARD(_, &shl->ctrl->list_lock);
		dlist_for_each_entry_safe(pos, n, &shl->ctrl->free_list, struct shl_list_node,
					free_link) {
			if (pos->size >= size) {
				free_node = shl_split_free_node(shl, pos, (uint32_t)size);
				need_extend = false;
				break;
			}
		}
		if (need_extend) {
			free_node = shl_extend(shl);
			if (free_node == NULL) {
				hm_error("falied to extend shl\n");
			} else {
				free_node = shl_split_free_node(shl, free_node, (uint32_t)size);
			}
		}
	}

	return free_node;
}

int shl_init(struct shlist *shl, const struct shl_config *cfg, bool has_ctrl)
{
	struct shl_ctrl *ctrl = NULL;
	char *prefix = NULL;
	int rc = E_HM_OK;

	if (shl == NULL || cfg == NULL) {
		return E_HM_INVAL;
	}

	raw_mutex_lock(&shl->shm_list_lock);
	if (raw_refcnt_nr_counter(&shl->ref_cnt) != SEC_SHL_UNINITIALIZED) {
		rc = E_HM_EXIST;
	}

	if (rc == E_HM_OK) {
		prefix = strdup(cfg->shm_prefix);
		if (prefix == NULL) {
			rc = E_HM_NOMEM;
		}
	}
	if (rc == E_HM_OK && has_ctrl) {
		if (cfg->shm_size > ((UINT32_MAX - (uint32_t)__PAGE_SIZE) + 1u)) {
			free(prefix);
			rc = E_HM_OVERFLOW;
		}
		if (rc == E_HM_OK) {
			ctrl = malloc(sizeof(*ctrl));
			if (ctrl == NULL) {
				free(prefix);
				rc = E_HM_NOMEM;
			}
		}
		if (rc == E_HM_OK) {
			dlist_init(&ctrl->list);
			dlist_init(&ctrl->free_list);
			raw_mutex_init(&ctrl->list_lock);
			ctrl->shm_size = (uint32_t)PAGE_ALIGN_UP(cfg->shm_size);
		}
	}
	if (rc == E_HM_OK) {
		shl->ctrl = ctrl;
		shl->shm_prefix = prefix;
	}

	if (rc == E_HM_OK || rc == E_HM_EXIST) {
		(void)raw_refcnt_nr_get(&shl->ref_cnt);
		rc = E_HM_OK;
	}
	raw_mutex_unlock(&shl->shm_list_lock);
	return rc;
}

void shl_fini(struct shlist *shl)
{
	struct shl_list_node *pos_list = NULL;
	struct shl_list_node *n_list = NULL;
	struct shl_shm_list *pos_mem = NULL;
	struct shl_shm_list *n_mem = NULL;
	int cnt = raw_refcnt_nr_put(&shl->ref_cnt);
	BUG_ON(cnt < SEC_SHL_UNINITIALIZED);

	if (cnt > SEC_SHL_UNINITIALIZED) {
		return;
	}

	dlist_for_each_entry_safe(pos_mem, n_mem, &shl->shm_list,
				  struct shl_shm_list, link) {
		shl_shm_close(pos_mem, true);
		free(pos_mem);
	}
	free(shl->shm_prefix);
	shl->shm_prefix = NULL;

	if (shl->ctrl != NULL) {
		dlist_for_each_entry_safe(pos_list, n_list, &shl->ctrl->list,
					  struct shl_list_node, list_link) {
			dlist_delete(&pos_list->list_link);
			dlist_delete(&pos_list->free_link);
			free(pos_list);
		}
		free(shl->ctrl);
		shl->ctrl = NULL;
	}
}
