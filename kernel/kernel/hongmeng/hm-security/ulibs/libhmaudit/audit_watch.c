/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Operations of audit watch
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 1 17:27:55 2021
 */
#include <fcntl.h>
#include <stdint.h>
#include <limits.h>

#include <hongmeng/errno.h>

#include <vfs.h>
#include <lib/dlist.h>
#include <libhmlog/hmlog.h>
#include <libhmaudit/audit.h>
#include <libhmaudit/audit_log.h>
#include <libhmaudit/rule.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/raw_rwlock_guard.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_audit/audit.h>

#define FS_AUDIT_RULE_STATUS_NOMAL		1
#define FS_AUDIT_RULE_STATUS_WAIT_FREE		2

#ifdef __HOST_LLT__
#define STATIC
#else
#define STATIC static
#endif

STATIC DLIST_HEAD(audit_fs_watch_list);
STATIC DLIST_HEAD(audit_fs_watch_dir_list);
STATIC DLIST_HEAD(audit_fs_rule_list);
STATIC struct raw_mutex watch_rule_mutex = RAW_MUTEX_INITIALIZER;
struct audit_mount_entry *fs_am_entries = NULL;    /* fs audit mount entry array head */
int fs_am_entry_num;

#define AUDIT_WATCH_INODE_BUCKETS 32
STATIC struct dlist_node audit_fs_watch_list_hash[AUDIT_WATCH_INODE_BUCKETS];
STATIC inline uint32_t audit_hash_index(uint32_t index)
{
	return (index & (AUDIT_WATCH_INODE_BUCKETS - 1));
}

STATIC struct audit_watch_dir_rel *audit_watch_dir_rel_find(const struct audit_watch_dir *dir, int me_id)
{
	struct audit_watch_dir_rel *rel = NULL;
	dlist_for_each_entry(rel, &dir->rel_list, struct audit_watch_dir_rel, dir_node) {
		if (rel->me_id == me_id) {
			return rel;
		}
	}

	return NULL;
}

STATIC struct audit_fs_rule *audit_watch_rule_find(uint64_t rule_id)
{
	struct audit_rule_common *rule = NULL;

	dlist_for_each_entry(rule, &audit_fs_rule_list, struct audit_rule_common, dlist) {
		if (rule->id == rule_id) {
			return container_of(rule, struct audit_fs_rule, common);
		}
	}

	return NULL;
}

STATIC void audit_watch_file_del(struct audit_watch_file *watch_file, uint32_t del)
{
	if (del != 0) {
		dlist_delete(&watch_file->aw.node);
		watch_file->aw.path = NULL;
		free(watch_file);
	}
}

STATIC void audit_watch_dir_rel_del(struct audit_watch_dir_rel *rel)
{
	// remove from relation list
	dlist_delete(&rel->ame_node);
	rel->ame = NULL;
	dlist_delete(&rel->dir_node);
	rel->dir = NULL;

	free(rel);
}

int audit_watch_delete_rule_rel(uint64_t rule_id, int me_id)
{
	struct audit_fs_rule *rule = NULL;
	struct audit_watch_dir_rel *rel = NULL;

	RAW_MUTEX_GUARD(_, &watch_rule_mutex);
	rule = audit_watch_rule_find(rule_id);
	if (rule == NULL) {
		return E_HM_POSIX_NOENT;
	}

	rel = audit_watch_dir_rel_find(rule->watch_dir, me_id);
	if (rel == NULL) {
		/* In the unmount process, the rel is deleted first. */
		return E_HM_OK;
	}

	if (raw_refcnt_nr_put(&rel->refcnt) == 0) {
		audit_watch_dir_rel_del(rel);
	}
	return E_HM_OK;
}

STATIC void __audit_watch_dir_del(struct audit_watch_dir *dir)
{
	dir->aw.path = NULL;
	free(dir);
}

STATIC void _audit_watch_dir_del(struct audit_watch_dir *dir)
{
	struct audit_watch_dir_rel *rel = NULL;
	struct audit_watch_dir_rel *next = NULL;

	dlist_delete(&dir->aw.node);
	if (!dlist_empty(&dir->rel_list)) {
		hm_warn("relation list is not empty\n");
		dlist_for_each_entry_safe(rel, next, &dir->rel_list, struct audit_watch_dir_rel, dir_node) {
			audit_watch_dir_rel_del(rel);
		}
	}

	__audit_watch_dir_del(dir);
}

STATIC void audit_watch_dir_del(struct audit_watch_dir *watch_dir,
				const struct audit_fs_node_keys *node_key)
{
	struct audit_watch_dir_rel *rel = NULL;

	// remove relation by meid
	rel = audit_watch_dir_rel_find(watch_dir, node_key->me_id);
	if (rel != NULL && raw_refcnt_nr_put(&rel->refcnt) == 0) {
		audit_watch_dir_rel_del(rel);
	}

	// release watch dir when ref count is 0
	if (raw_refcnt_nr_put(&watch_dir->aw.refcnt) == 0) {
		_audit_watch_dir_del(watch_dir);
	}
}

STATIC void audit_rule_release(struct audit_fs_rule *rule, uint32_t flags,
			       const struct audit_fs_node_keys *node_key)
{
	free(rule->common.fields);
	rule->common.fields = NULL;

	// remove rule from audit watch rule list
	dlist_delete(&rule->watch_node);
	if ((flags & HM_AUDIT_FS_WATCH_FILE) != 0) {
		dlist_delete(&rule->common.hlist);
		audit_watch_file_del(rule->watch_file, flags & HM_AUDIT_FS_WATCH_DEL);
		rule->watch_file = NULL;
	} else {
		audit_watch_dir_del(rule->watch_dir, node_key);
		rule->watch_dir = NULL;
	}
	free(rule);
	rule = NULL;
}

STATIC int audit_watch_dir_rel_add(int me_id, struct audit_watch_dir *dir, uint32_t flags)
{
	struct audit_watch_dir_rel *aw_rel = NULL;
	struct audit_mount_entry *ame = NULL;

	aw_rel = malloc(sizeof(struct audit_watch_dir_rel));
	if (aw_rel == NULL) {
		return E_HM_NOMEM;
	}

	ame = &fs_am_entries[me_id];
	aw_rel->me_id = me_id;
	aw_rel->ame = ame;
	aw_rel->dir = dir;
	aw_rel->is_root_node = ((flags & HM_AUDIT_FS_DIR_ROOT) != 0) ? true : false;
	raw_refcnt_nr_init(&aw_rel->refcnt, 1);

	dlist_insert_tail(&ame->rel_list, &aw_rel->ame_node);
	dlist_insert_tail(&dir->rel_list, &aw_rel->dir_node);
	return E_HM_OK;
}

int audit_watch_insert_rule_rel(uint16_t rule_id, uint16_t rule_num, int me_id)
{
	int rc;
	struct audit_fs_rule *rule = NULL;
	struct audit_shared_rule *shm_rule = NULL;
	struct audit_watch_dir_rel *rel = NULL;
	struct audit_shared_rule_addr addr;

	if (me_id >= fs_am_entry_num || me_id < 0) {
		hm_error("me_id is invalid: %d\n", me_id);
		return E_HM_INVAL;
	}
	addr.shm_id = rule_id;
	addr.shm_off = rule_num;
	RAW_MUTEX_GUARD(_, &watch_rule_mutex);
	shm_rule = audit_shared_rule_nolock(&addr);
	if (shm_rule == NULL) {
		hm_error("failed to find shared audit rule, rule_id=%hu, rule_num=%hu\n",
			 rule_id, rule_num);
		return E_HM_NOMEM;
	}

	rule = audit_watch_rule_find(shm_rule->rule_id);
	if (rule == NULL) {
		hm_error("failed to find target rule, shm_page_id=%hu, rule_id=%"PRIx64"\n",
			 rule_id, shm_rule->rule_id);
		return E_HM_POSIX_NOENT;
	}

	rel = audit_watch_dir_rel_find(rule->watch_dir, me_id);
	if (rel != NULL) {
		(void)raw_refcnt_nr_get(&rel->refcnt);
		return E_HM_OK;
	}

	rc = audit_watch_dir_rel_add(me_id, rule->watch_dir, 0);
	if (rc != E_HM_OK) {
		hm_error("failed to add audit watch dir relation, err=%s\n", hmstrerror(rc));
		return rc;
	}
	return E_HM_OK;
}

STATIC struct audit_watch_dir *audit_watch_dir_new(const char *path, size_t path_len,
						   const struct audit_fs_node_keys *node_key)
{
	struct audit_watch_dir *dir = NULL;
	if (path_len + 1 > PATH_MAX) {
		return NULL;
	}

	dir = malloc(sizeof(struct audit_watch_dir));
	if (dir == NULL) {
		return NULL;
	}

	dir->aw.len = path_len;
	dir->aw.path = path;
	dir->aw.type = HM_AUDIT_DIR;
	raw_refcnt_nr_init(&dir->aw.refcnt, 1);
	dlist_init(&dir->aw.node);
	dlist_init(&dir->aw.rlist);
	dlist_init(&dir->rel_list);
	if (node_key != NULL) {
		dir->dev_id  = node_key->dev_id;
		dir->index   = node_key->index;
	}

	return dir;
}

STATIC struct audit_watch_dir *audit_watch_dir_find(const char *path, size_t path_len)
{
	struct audit_watch *aw = NULL;
	struct audit_watch_dir *dir = NULL;

	dlist_for_each_entry(aw, &audit_fs_watch_dir_list, struct audit_watch, node) {
		dir = (struct audit_watch_dir *)aw;
		if (path_len == dir->aw.len && strcmp(dir->aw.path, path) == 0) {
			return dir;
		}
	}
	return NULL;
}

STATIC int audit_rule_watch_dir_fill(struct audit_fs_rule *rule, const char *path, size_t path_len,
				     uint32_t flags, const struct audit_fs_node_keys *node_key)
{
	int rc;
	int me_id = node_key->me_id;
	struct audit_watch_dir *dir = NULL;

	dir = audit_watch_dir_find(path, path_len);
	if (dir != NULL) {
		struct audit_watch_dir_rel *rel = NULL;
		rel = audit_watch_dir_rel_find(dir, me_id);
		if (rel != NULL) {
			(void)raw_refcnt_nr_get(&rel->refcnt);
		} else {
			rc = audit_watch_dir_rel_add(me_id, dir, flags);
			if (rc != E_HM_OK) {
				return rc;
			}
		}
		(void)raw_refcnt_nr_get(&dir->aw.refcnt);
	} else {
		dir = audit_watch_dir_new(path, path_len, node_key);
		if (dir == NULL) {
			return E_HM_NOMEM;
		}

		rc = audit_watch_dir_rel_add(me_id, dir, flags);
		if (rc != E_HM_OK) {
			__audit_watch_dir_del(dir);
			return rc;
		}
		dlist_insert_tail(&audit_fs_watch_dir_list, &dir->aw.node);
	}

	dlist_insert_tail(&dir->aw.rlist, &rule->watch_node);
	rule->watch_dir = dir;
	return E_HM_OK;
}

STATIC struct audit_watch_file *audit_new_watch_file(const char *path,
						     size_t path_len, const struct audit_fs_node_keys *node_key)
{
	struct audit_watch_file *watch = NULL;

	if (path_len + 1 > PATH_MAX) {
		return NULL;
	}

	watch = malloc(sizeof(struct audit_watch_file));
	if (watch == NULL) {
		return NULL;
	}

	watch->aw.len = path_len;
	watch->aw.path = path;
	watch->aw.type = HM_AUDIT_WATCH;
	watch->dev_id  = node_key->dev_id;
	watch->index   = node_key->index;
	dlist_init(&watch->aw.rlist);
	dlist_insert_tail(&audit_fs_watch_list, &watch->aw.node);

	return watch;
}

STATIC struct audit_watch_file *audit_find_watch_file(const char *path, size_t path_len)
{
	struct audit_watch *watch = NULL;

	dlist_for_each_entry(watch, &audit_fs_watch_list, struct audit_watch, node) {
		if (watch->len == path_len &&
		    (strncmp(watch->path, path, path_len) == 0)) {
			return container_of(watch, struct audit_watch_file, aw);
		}
	}
	return NULL;
}

STATIC int audit_rule_fill_watch_file(struct audit_fs_rule *rule, const char *path, size_t path_len,
				      uint32_t flags, const struct audit_fs_node_keys *node_key)
{
	struct audit_watch_file *watch = NULL;

	if (node_key->path != NULL && strcmp(path, node_key->path) != 0) {
		return E_HM_POSIX_NOENT;
	}
	if ((flags & HM_AUDIT_FS_WATCH_NEW) != 0) {
		watch = audit_new_watch_file(path, path_len, node_key);
		if (watch == NULL) {
			return E_HM_NOMEM;
		}
	} else {
		watch = audit_find_watch_file(path, path_len);
		if (watch == NULL) {
			return E_HM_POSIX_NOENT;
		}
	}

	rule->watch_file = watch;
	dlist_insert_tail(&watch->aw.rlist, &rule->watch_node);
	return E_HM_OK;
}

STATIC int audit_rule_fill_watch(struct audit_fs_rule *rule, uint32_t flags,
				 const struct audit_shared_rule_addr *addr,
				 const struct audit_fs_node_keys *node_key)
{
	int rc;
	size_t path_len = 0;
	char *path = NULL;

	// The size of the string is at least sizeof(char)
	path = (char *)audit_shared_rule_comm_nolock(addr, (uint32_t)sizeof(char));
	if (path == NULL) {
		return E_HM_NOMEM;
	}

	rc = audit_rule_watch_path_len(path, addr, &path_len);
	if (rc != E_HM_OK) {
		hm_error("failed to get watch path length, err=%s\n", hmstrerror(rc));
		return E_HM_NODATA;
	}

	if ((flags & HM_AUDIT_FS_WATCH_FILE) != 0) {
		rc = audit_rule_fill_watch_file(rule, path, path_len, flags, node_key);
	} else {
		rc = audit_rule_watch_dir_fill(rule, path, path_len, flags, node_key);
	}
	return rc;
}

STATIC int audit_rule_common_fill(struct audit_rule_common *rule,
				  const struct audit_shared_rule *shm_rule)
{
	int rc = E_HM_OK;

	if (shm_rule->field_count == 0UL) {
		return E_HM_INVAL;
	}

	rule->id = shm_rule->rule_id;
	rule->action = shm_rule->action;
	rule->field_count = shm_rule->field_count;
	rule->status = FS_AUDIT_RULE_STATUS_NOMAL;

	for (int i = 0; i < HM_AUDIT_BITMASK_SIZE; i++) {
		rule->mask[i] = shm_rule->mask[i];
	}

	rule->fields = malloc(sizeof(struct audit_field) * rule->field_count);
	if (rule->fields == NULL) {
		rc = E_HM_NOMEM;
	} else {
		for (unsigned i = 0; i < rule->field_count; i++) {
			rule->fields[i].type = shm_rule->fields[i].type;
			rule->fields[i].val = shm_rule->fields[i].value;
			rule->fields[i].op = shm_rule->fields[i].op;
		}
	}

	return rc;
}

STATIC inline void audit_rule_common_destroy(struct audit_rule_common *rule)
{
	free(rule->fields);
	rule->fields = NULL;
}

STATIC void audit_watch_rule_list_insert(struct audit_fs_rule *rule, uint32_t flags, uint32_t index)
{
	uint32_t h = audit_hash_index(index);
	if ((flags & HM_AUDIT_FS_WATCH_APPEND) != 0U) {
		dlist_insert_tail(&audit_fs_rule_list, &rule->common.dlist);
		if ((flags & HM_AUDIT_FS_WATCH_FILE) != 0U) {
			dlist_insert_tail(&audit_fs_watch_list_hash[h], &rule->common.hlist);
		}
	} else {
		dlist_insert(&audit_fs_rule_list, &rule->common.dlist);
		if ((flags & HM_AUDIT_FS_WATCH_FILE) != 0U) {
			dlist_insert(&audit_fs_watch_list_hash[h], &rule->common.hlist);
		}
	}
}

int audit_watch_insert_rule(uint16_t rule_id, uint16_t rule_num,
			    uint32_t flags, struct audit_fs_node_keys *vfs_node_key)
{
	struct audit_shared_rule_addr addr;
	struct audit_fs_rule *rule;
	struct audit_shared_rule *shm_rule = NULL;
	int rc = 0;
	struct audit_fs_node_keys *node_key = vfs_node_key;

	addr.shm_id = rule_id;
	addr.shm_off = rule_num;

	rule = malloc(sizeof(*rule));
	if (rule == NULL) {
		return E_HM_NOMEM;
	}

	RAW_RWLOCK_RD_GUARD(_s, audit_shared_rule_list_lock(HM_AUDIT_FILTER_EXIT));
	RAW_MUTEX_GUARD(_, &watch_rule_mutex);
	shm_rule = audit_shared_rule_nolock(&addr);
	if (shm_rule == NULL) {
		free(rule);
		return E_HM_NOMEM;
	}

	rc = audit_rule_common_fill(&rule->common, shm_rule);
	if (rc != E_HM_OK) {
		free(rule);
		return rc;
	}

	rc = audit_rule_fill_watch(rule, flags, &shm_rule->watch_addr, node_key);
	if (rc < 0) {
		hm_warn("failed to fill watch rule, err=%s\n", hmstrerror(rc));
		audit_rule_common_destroy(&rule->common);
		free(rule);
		return rc;
	}

	audit_watch_rule_list_insert(rule, flags, node_key->index);

	return rc;
}

int audit_watch_delete_rule(uint64_t rule_id, uint32_t flags, struct audit_fs_node_keys *vfs_node_key)
{
	struct audit_fs_rule *rule;
	struct audit_fs_node_keys *node_key = vfs_node_key;

	RAW_MUTEX_GUARD(_, &watch_rule_mutex);
	rule = audit_watch_rule_find(rule_id);
	if (rule != NULL) {
		dlist_delete(&rule->common.dlist);
		audit_rule_release(rule, flags, node_key);
	}
	return E_HM_OK;
}

STATIC int audit_watch_compare(const struct audit_watch_field *awf, const struct audit_watch_file *watch)
{
	if (watch->dev_id != awf->dev_id) {
		return WATCH_NOT_MATCH;
	}
	if (watch->index != awf->index) {
		return WATCH_NOT_MATCH;
	}

	return WATCH_IS_MATCH;
}

STATIC int audit_watch_rule_field_match(const struct audit_fs_rule *rule,
					const struct audit_match *match,
					const struct audit_watch_field *awf)
{
	struct audit_field *f = NULL;
	uint16_t i;
	int rc;

	for (i = 0; i < rule->common.field_count; i++) {
		f = &rule->common.fields[i];
		switch (f->type) {
		case HM_AUDIT_WATCH:
			rc = audit_watch_compare(awf, rule->watch_file);
			if (rc == WATCH_NOT_MATCH) {
				return MATCH_RES_NOT_MATCHED;
			}
			break;
		case HM_AUDIT_DEVMAJOR:
			if (audit_field_cmp(MAJOR(awf->devno), f->op, f->val) != 0) {
				return MATCH_RES_NOT_MATCHED;
			}
			break;
		case HM_AUDIT_DEVMINOR:
			if (audit_field_cmp(MINOR(awf->devno), f->op, f->val) != 0) {
				return MATCH_RES_NOT_MATCHED;
			}
			break;
		case HM_AUDIT_INODE:
			if (audit_field_cmp(awf->index, f->op, f->val) != 0) {
				return MATCH_RES_NOT_MATCHED;
			}
			break;
		default:
			rc = audit_field_match(f, match);
			if (rc == FIELD_NOT_MATCH) {
				return MATCH_RES_NOT_MATCHED;
			}
			break;
		}
	}
	return MATCH_RES_MATCHED;
}

STATIC int audit_watch_rule_match(const struct audit_fs_rule *rule, struct audit_match *match,
				  const struct audit_watch_field *awf)
{
	struct audit_ctx *ctx = match->ctx;
	int rc;

	rc = audit_syscall_match(rule->common.mask, ctx->syscall_num);
	if (rc == 0) {
		return MATCH_RES_NOT_MATCHED;
	}

	rc = audit_watch_rule_field_match(rule, match, awf);
	if (rc != MATCH_RES_MATCHED) {
		return MATCH_RES_NOT_MATCHED;
	}

	ctx->rule_id = rule->common.id;
	switch (rule->common.action) {
	case HM_AUDIT_ALWAYS:
		ctx->record_state = AUDIT_RECORD;
		break;
	case HM_AUDIT_NEVER:
		ctx->record_state = AUDIT_DISABLED;
		break;
	default:
		break;
	}
	return MATCH_RES_MATCHED;
}

int audit_watch_rule_iterate(const struct audit_watch_field *awf, struct audit_match *match)
{
	uint32_t h = audit_hash_index(awf->index);
	struct dlist_node *list = &audit_fs_watch_list_hash[h];
	struct audit_rule_common *common_rule = NULL;
	struct audit_fs_rule *rule = NULL;
	int rc = MATCH_RES_NOT_MATCHED;

	RAW_MUTEX_GUARD(_, &watch_rule_mutex);
	if (dlist_empty(list)) {
		return WATCH_NOT_MATCH;
	}

	dlist_for_each_entry(common_rule, list, struct audit_rule_common, hlist) {
		rule = container_of(common_rule, struct audit_fs_rule, common);
		rc = audit_watch_rule_match(rule, match, awf);
		if (rc == MATCH_RES_MATCHED) {
			break;
		}
	}
	return rc;
}

int audit_watch_file_match(uint64_t rule_id, const struct audit_watch_field *awf)
{
	struct audit_fs_rule *rule = NULL;
	struct audit_watch_file *watch = NULL;

	RAW_MUTEX_GUARD(_, &watch_rule_mutex);
	rule = audit_watch_rule_find(rule_id);
	if (rule == NULL) {
		hm_error("failed to find audit watch rule, rule_id=%"PRIx64"\n", rule_id);
		return WATCH_NOT_MATCH;
	}
	watch = rule->watch_file;

	return audit_watch_compare(awf, watch);
}

int audit_watch_dir_match(uint64_t rule_id, int me_id)
{
	struct audit_mount_entry *ame = NULL;
	struct audit_fs_rule *rule = NULL;
	struct audit_watch_dir *dir = NULL;
	struct audit_watch_dir_rel *rel = NULL;

	if (me_id >= fs_am_entry_num || me_id < 0) {
		hm_error("me_id is invalid: %d\n", me_id);
		return E_HM_INVAL;
	}
	ame = &fs_am_entries[me_id];
	dlist_for_each_entry(rel, &ame->rel_list, struct audit_watch_dir_rel, ame_node) {
		dir = rel->dir;
		dlist_for_each_entry(rule, &dir->aw.rlist, struct audit_fs_rule, watch_node) {
			if (rule->common.id == rule_id) {
				return MATCH_RES_MATCHED;
			}
		}
	}

	return MATCH_RES_NOT_MATCHED;
}

STATIC int audit_watch_local_match(uint32_t audit_type, const struct audit_shared_rule *rule,
				   struct audit_match *match)
{
	int rc = MATCH_RES_NOT_MATCHED;
	struct audit_watch_ctx *w_ctx = &match->ctx->watch;

	if (audit_type == HM_AUDIT_WATCH) {
		for (unsigned int i = 0; i < w_ctx->field_count; ++i) {
			struct audit_watch_field *awf = w_ctx->field[i];
			rc = audit_watch_file_match(rule->rule_id, awf);
			if (rc == MATCH_RES_MATCHED) {
				break;
			}
		}
	} else {
		for (unsigned int i = 0; i < w_ctx->dir_field_count; ++i) {
			rc = audit_watch_dir_match(rule->rule_id, w_ctx->dir_field[i].me_idx);
			if (rc == MATCH_RES_MATCHED) {
				break;
			}
		}
	}
	return rc;
}

STATIC int audit_watch_remote_match(uint32_t audit_type, const struct audit_shared_rule *rule,
				    struct audit_match *match)
{
	int rc = MATCH_RES_NOT_MATCHED;
	struct audit_watch_ctx *w_ctx = &match->ctx->watch;

	if (audit_type == HM_AUDIT_WATCH) {
		for (unsigned int i = 0; i < w_ctx->field_count; ++i) {
			struct audit_watch_field *awf = w_ctx->field[i];
			rc = vfs_match_audit_watch_file(rule->rule_id, awf);
			if (rc == MATCH_RES_MATCHED) {
				break;
			}
		}
	} else {
		for (unsigned int i = 0; i < w_ctx->dir_field_count; ++i) {
			rc = vfs_match_audit_watch_dir(rule->rule_id, w_ctx->dir_field[i].me_idx,
						       w_ctx->field[w_ctx->dir_field[i].field_index]);
			if (rc == MATCH_RES_MATCHED) {
				break;
			}
		}
	}
	return rc;
}

static int audit_watch_match(uint32_t audit_type,
			     const struct audit_shared_rule *rule,
			     struct audit_match *match)
{
	int rc = MATCH_RES_NOT_MATCHED;

	if (audit_server_num_get() == __server_fs) {
		rc = audit_watch_local_match(audit_type, rule, match);
	} else {
		rc = audit_watch_remote_match(audit_type, rule, match);
	}

	return rc;
}

static int audit_watch_dev_match(bool major, const struct audit_shared_field *f, struct audit_match *match)
{
	int rc = MATCH_RES_NOT_MATCHED;
	struct audit_watch_ctx *w_ctx = &match->ctx->watch;

	for (unsigned int i = 0; i < w_ctx->field_count; ++i) {
		struct audit_watch_field *awf = w_ctx->field[i];
		if (major) {
			if (audit_field_cmp(MAJOR(awf->devno), f->op, f->value) != 0) {
				rc = MATCH_RES_MATCHED;
				break;
			}
		} else {
			if (audit_field_cmp(MINOR(awf->devno), f->op, f->value) != 0) {
				rc = MATCH_RES_MATCHED;
				break;
			}
		}
	}
	return rc;
}

static int audit_watch_inode_match(const struct audit_shared_field *f, struct audit_match *match)
{
	int rc = MATCH_RES_NOT_MATCHED;
	struct audit_watch_ctx *w_ctx = &match->ctx->watch;

	for (unsigned int i = 0; i < w_ctx->field_count; ++i) {
		struct audit_watch_field *awf = w_ctx->field[i];
		if (audit_field_cmp(awf->index, f->op, f->value) != 0) {
			rc = MATCH_RES_MATCHED;
			break;
		}
	}

	return rc;
}

int audit_watch_info_match(const struct audit_shared_rule *rule,
			   struct audit_match *match)
{
	const struct audit_shared_field *f = NULL;
	int rc = MATCH_RES_NOT_MATCHED;
	uint16_t i;

	for (i = 0; i < rule->field_count; i++) {
		f = &rule->fields[i];
		switch (f->type) {
		case HM_AUDIT_WATCH:
		case HM_AUDIT_DIR:
			rc = audit_watch_match(f->type, rule, match);
			if (rc == MATCH_RES_NOT_MATCHED) {
				return rc;
			}
			break;
		case HM_AUDIT_DEVMAJOR:
			rc = audit_watch_dev_match(true, f, match);
			if (rc == MATCH_RES_NOT_MATCHED) {
				return rc;
			}
			break;
		case HM_AUDIT_DEVMINOR:
			rc = audit_watch_dev_match(false, f, match);
			if (rc == MATCH_RES_NOT_MATCHED) {
				return rc;
			}
			break;
		case HM_AUDIT_INODE:
			rc = audit_watch_inode_match(f, match);
			if (rc == MATCH_RES_NOT_MATCHED) {
				return rc;
			}
			break;
		default:
			break;
		}
	}

	return MATCH_RES_MATCHED;
}

STATIC void audit_watch_send_rule(struct audit_fs_rule *rule)
{
	int rc;
	struct audit_log log;
	if (rule->common.status == FS_AUDIT_RULE_STATUS_WAIT_FREE) {
		return;
	}
	rule->common.status = FS_AUDIT_RULE_STATUS_WAIT_FREE;
	mem_zero_s(log);
	log.audit_type = HM_AUDIT_DEL_RULE;
	log.rule_id = rule->common.id;
	log.total_len = (uint32_t)sizeof(struct audit_log);
	rc = audit_log_write(&log);
	if (rc != E_HM_OK) {
		hm_error("failed to send the message for deleting rule, rc=%s\n", hmstrerror(rc));
	}
}

STATIC void audit_watch_notify_mgr_rule_del(const struct audit_watch_dir *dir)
{
	struct audit_fs_rule *rule = NULL;

	dlist_for_each_entry(rule, &dir->aw.rlist, struct audit_fs_rule, watch_node) {
		audit_watch_send_rule(rule);
	}
}

int audit_watch_dir_lookup(int me_id, dev_t dev_id, uint32_t index, bool is_mp)
{
	struct audit_watch_dir *dir = NULL;
	struct audit_watch_dir_rel *rel = NULL;

	if (me_id >= fs_am_entry_num || me_id < 0) {
		hm_error("me_id is invalid: %d\n", me_id);
		return E_HM_INVAL;
	}
	dlist_for_each_entry(rel, &fs_am_entries[me_id].rel_list, struct audit_watch_dir_rel, ame_node) {
		dir = rel->dir;
		if ((is_mp && !rel->is_root_node) ||
		    ((dev_id == dir->dev_id) &&
		    (index == dir->index))) {
			return E_HM_OK;
		}
	}
	return E_HM_POSIX_NOENT;
}

STATIC void audit_watch_dir_rule_del_4_unlink(const char *path)
{
	struct audit_watch_dir *dir = NULL;
	struct audit_watch_dir_rel *rel = NULL;
	struct audit_watch_dir_rel *next = NULL;

	RAW_MUTEX_GUARD(_, &watch_rule_mutex);
	dir = audit_watch_dir_find(path, (uint32_t)strlen(path));
	if (dir == NULL) {
		return;
	}

	dlist_for_each_entry_safe(rel, next, &dir->rel_list, struct audit_watch_dir_rel, dir_node) {
		dlist_delete(&rel->ame_node);
		rel->ame = NULL;

		dlist_delete(&rel->dir_node);
		if (rel->is_root_node) {
			audit_watch_notify_mgr_rule_del(dir);
		}
		rel->dir = NULL;
		free(rel);
	}
}

void audit_watch_dir_rule_refresh(struct audit_watch_del_ctx *w_ctx)
{
	if (audit_server_num_get() != __server_fs) {
		return;
	}

	for (unsigned int i = 0; i < w_ctx->del_dir_count; ++i) {
		struct audit_watch_del_dir *del_dir = &w_ctx->del_dir[i];
		audit_watch_dir_rule_del_4_unlink(del_dir->path);
	}
}

void audit_mount_entry_del(int me_id)
{
	struct audit_watch_dir_rel *rel = NULL;
	struct audit_watch_dir_rel *next = NULL;
	struct audit_mount_entry *ame = NULL;

	if (me_id >= fs_am_entry_num || me_id < 0) {
		hm_error("me_id is invalid: %d\n", me_id);
		return;
	}
	ame = &fs_am_entries[me_id];
	RAW_MUTEX_GUARD(_, &watch_rule_mutex);
	dlist_for_each_entry_safe(rel, next, &ame->rel_list, struct audit_watch_dir_rel, ame_node) {
		dlist_delete(&rel->ame_node);
		rel->ame = NULL;

		dlist_delete(&rel->dir_node);
		if (rel->is_root_node) {
			audit_watch_notify_mgr_rule_del(rel->dir);
		}
		rel->dir = NULL;
		free(rel);
	}
	return;
}

int audit_watch_dir_init(uint32_t max_me_num)
{
	size_t size = max_me_num * sizeof(struct audit_mount_entry);
	fs_am_entries = (struct audit_mount_entry *)malloc(size);
	if (fs_am_entries == NULL) {
		hm_error("failed to malloc fs audit mount entries\n");
		return E_HM_NOMEM;
	}

	NOFAIL(memset_s((char *)fs_am_entries, size, 0, size));
	for (uint32_t i = 0; i < max_me_num; ++i) {
		struct audit_mount_entry *ame = &fs_am_entries[i];
		dlist_init(&ame->rel_list);
	}
	for (uint32_t i = 0; i < AUDIT_WATCH_INODE_BUCKETS; i++) {
		dlist_init(&audit_fs_watch_list_hash[i]);
	}

	fs_am_entry_num = (int)max_me_num;
	return E_HM_OK;
}

STATIC void update_rules(const struct audit_watch *watch, dev_t dev_id, uint32_t index)
{
	struct audit_watch_file *awf = container_of(watch, struct audit_watch_file, aw);
	struct audit_fs_rule *rule = NULL;
	struct audit_fs_rule *tmp= NULL;
	uint32_t h = audit_hash_index(index);
	awf->dev_id = dev_id;
	awf->index = index;
	dlist_for_each_entry_safe(rule, tmp, &watch->rlist, struct audit_fs_rule, watch_node) {
		dlist_delete(&rule->common.hlist);
		dlist_insert_tail(&audit_fs_watch_list_hash[h], &rule->common.hlist);
	}
}

STATIC void remove_rules(const struct audit_watch *watch)
{
	struct audit_fs_rule *rule = NULL;
	dlist_for_each_entry(rule, &watch->rlist, struct audit_fs_rule, watch_node) {
		audit_watch_send_rule(rule);
	}
}

void audit_update_rule(const char *path, dev_t dev_id, uint32_t index, bool del)
{
	struct audit_watch *watch = NULL;

	RAW_MUTEX_GUARD(_, &watch_rule_mutex);
	dlist_for_each_entry(watch, &audit_fs_watch_list, struct audit_watch, node) {
		size_t len1 = strlen(path);
		size_t len2 = strlen(watch->path);
		if (len1 == len2 && strcmp(path, watch->path) == 0) {
			update_rules(watch, dev_id, index);
		} else {
			if (del && len1 < len2 && strncmp(watch->path, path, len1) == 0 && watch->path[len1] == '/') {
				remove_rules(watch);
			}
		}
	}
}

bool audit_is_no_fs_dir_rule(void)
{
	return dlist_empty(&audit_fs_watch_dir_list);
}

bool audit_is_no_fs_rule(void)
{
	return (dlist_empty(&audit_fs_watch_dir_list) && dlist_empty(&audit_fs_watch_list));
}
