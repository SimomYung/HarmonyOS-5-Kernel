/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Operations of audit rules
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 28 17:27:55 2020
 */
#include <libhmaudit/rule.h>

#include <fcntl.h>
#include <stdint.h>

#include <hongmeng/errno.h>

#include <lib/dlist.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_rwlock_guard.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_audit/audit.h>
#include <libshlist/shlist.h>

#include <libhmaudit/audit.h>

__thread cref_t audit_wp_cref;

#define CFG_SHM_SIZE 4096u
#define CTRL_SHM_SIZE 4096u
#define RULE_SHM_SIZE 4096u
#define INVALID_SHM_ID UINT16_MAX
#define INVALID_SHM_OFF UINT16_MAX

#define AUDIT_CFG_SHM_NAME "audit_cfg"
#define AUDIT_CTRL_SHM_NAME "audit_ctrl"

static struct audit_shm *cfg_shm;
static struct audit_shm *ctrl_shm;
static struct shlist rule_list = SEC_CRED_SHLIST_INIT(rule_list);

#define audit_cfg(n) (((struct audit_shared_cfg_page *)(cfg_shm->base))->cfg[n])
#define audit_ctrl(n) (((struct audit_shared_ctrl *)ctrl_shm->base)[n])
#define audit_rule_lock(n) (audit_ctrl(n).lock)
#define audit_rule_count(n) (audit_cfg(n).rule_count)
#define audit_rule_first(n) (audit_cfg(n).list.next)
#define audit_rule_last(n) (audit_cfg(n).list.prev)

bool audit_rule_empty(void)
{
	return ((struct audit_shared_cfg_page *)cfg_shm->base)->stat.rule_empty;
}

static inline void audit_rule_set_empty_flag(bool flag)
{
	((struct audit_shared_cfg_page *)cfg_shm->base)->stat.rule_empty = flag;
}

static inline bool rule_is_empty(void)
{
	int i;
	for (i = 0; i < RULE_LIST_COUNT; i++) {
		if (audit_rule_count(i) > 0) {
			return false;
		}
	}
	return true;
}

static inline void audit_rule_count_set(unsigned n, uint16_t count)
{
	((struct audit_shared_cfg_page *)(cfg_shm->base))->cfg[n].rule_count = count;
}

static inline struct audit_shared_rule *shared_rule_of(const struct shl_list_node *node)
{
	return (struct audit_shared_rule *)shl_shm_addr_of(node);
}

static inline char *shared_watch_rule_of(const struct shl_list_node *node)
{
	return (char *)shl_shm_addr_of(node);
}

static inline void audit_rule_first_set(unsigned n, const struct audit_shared_rule_addr *addr)
{
	((struct audit_shared_cfg_page *)(cfg_shm->base))->cfg[n].list.next = *addr;
}

static inline void audit_rule_last_set(unsigned n, const struct audit_shared_rule_addr *addr)
{
	((struct audit_shared_cfg_page *)(cfg_shm->base))->cfg[n].list.prev = *addr;
}

static inline struct audit_shared_rule_addr *audit_rule_first_get(unsigned n)
{
	return &audit_rule_first(n);
}

static inline struct audit_shared_rule_addr *audit_rule_last_get(unsigned n)
{
	return &audit_rule_last(n);
}

static int audit_shm_open(struct audit_shm *shm, const char *name, size_t sz,
			  bool create, bool writable)
{
	uint64_t oflags = 0;
	uint32_t prot = PROT_READ;
	void *addr = NULL;
	size_t len = 0;
	int rc = E_HM_OK;
	int fd;

	if (strlen(name) >= AUDIT_SHM_NAME_LEN) {
		hm_error("shm name is too long\n");
		return E_HM_INVAL;
	}
	oflags |= create ? O_CREAT : 0;
	oflags |= writable ? O_RDWR : O_RDONLY;
	fd = hm_mem_shm_open_with_mode(name, sz, oflags, S_ISYS_SRVC | S_IRUSR | S_IRGRP, &len);
	if (fd < 0) {
		hm_error("failed to open the shared memory, name=%s\n", name);
		return fd;
	}
	prot |= (create || writable) ? PROT_WRITE : 0;
	addr = hm_mem_mmap(0, len, prot, MAP_SHARED | MAP_ANON, fd, 0);
	if (addr == MAP_FAILED) {
		hm_error("mmap failed, name=%s\n", name);
		rc = E_HM_NOMEM;
		goto close_fd;
	}

	shm->base = (uintptr_t)addr;
	shm->size = len;
	NOFAIL(strcpy_s(shm->name, AUDIT_SHM_NAME_LEN, name));

close_fd:
	(void)hm_mem_shm_close(fd);
	return rc;
}

static struct audit_shm *audit_shm_make(const char *name, size_t sz, bool create, bool writable)
{
	struct audit_shm *shm = NULL;
	int rc;

	shm = malloc(sizeof(*shm));
	if (shm == NULL) {
		hm_error("failed to allocate memory for the share memory of audit\n");
		return NULL;
	}

	rc = audit_shm_open(shm, name, sz, create, writable);
	if (rc != E_HM_OK) {
		hm_error("failed to open shared memory, rc=%s,name=%s\n",
			 hmstrerror(rc), name);
		free(shm);
		shm = NULL;
	}
	return shm;
}

static void audit_shm_destroy(struct audit_shm *shm)
{
	(void)hm_mem_munmap((void*)shm->base, shm->size);
	(void)hm_mem_shm_unlink(shm->name);
	free(shm);
}

void audit_shared_rule_fini(void)
{
	shl_fini(&rule_list);
	audit_shm_destroy(cfg_shm);
	cfg_shm = NULL;
	audit_shm_destroy(ctrl_shm);
	ctrl_shm = NULL;
}

static inline void audit_shared_addr_invalidate(struct audit_shared_rule_addr *addr)
{
	addr->shm_id = INVALID_SHM_ID;
	addr->shm_off = INVALID_SHM_OFF;
}

static inline void audit_shared_addr_ctor(struct audit_shared_rule_addr *addr,
					  const struct shl_list_node *node)
{
	addr->shm_id = (uint16_t)node->shm->shm_idx;
	addr->shm_off = (uint16_t)node->offset;
}

int audit_shared_rule_init(bool create)
{
	int rc;
	unsigned i;
	struct shl_config cfg = {
		.shm_prefix = "audit_rule_",
		.shm_size = RULE_SHM_SIZE
	};

	cfg_shm = audit_shm_make(AUDIT_CFG_SHM_NAME, CFG_SHM_SIZE, create, false);
	if (cfg_shm == NULL) {
		hm_error("failed to make audit_cfg\n");
		return E_HM_NOMEM;
	}

	ctrl_shm = audit_shm_make(AUDIT_CTRL_SHM_NAME, CTRL_SHM_SIZE, create, true);
	if (ctrl_shm == NULL) {
		audit_shm_destroy(cfg_shm);
		return E_HM_NOMEM;
	}

	if (create) {
		audit_rule_set_empty_flag(true);
		for (i = 0U; i < RULE_LIST_COUNT; i++) {
			raw_rwlock_init_shared(&audit_rule_lock(i));
			audit_rule_count_set(i, 0U);
			audit_shared_addr_invalidate(audit_rule_first_get(i));
			audit_shared_addr_invalidate(audit_rule_last_get(i));
		}
	}

	rc = shl_init(&rule_list, &cfg, create);
	if (rc != E_HM_OK) {
		hm_error("failed to initialize shared list of fields, rc=%s\n",
			 hmstrerror(rc));
		audit_shm_destroy(cfg_shm);
		audit_shm_destroy(ctrl_shm);
		return rc;
	}

	return E_HM_OK;
}

int audit_syscall_match(const uint32_t *mask,
			unsigned int syscall_num)
{
	uint32_t word;
	uint32_t bit;

	word = HM_AUDIT_WORD(syscall_num);
	if (word >= HM_AUDIT_BITMASK_SIZE) {
		return 0;
	}

	bit = HM_AUDIT_BIT(syscall_num);

	return ((mask[word] & bit) != 0U);
}

static bool audit_is_need_match_watch(uint32_t type)
{
	return (type == HM_AUDIT_WATCH || type == HM_AUDIT_DIR
		|| type == HM_AUDIT_DEVMAJOR || type == HM_AUDIT_DEVMINOR
		|| type == HM_AUDIT_INODE);
}

static int audit_shared_rule_match(const struct audit_shared_rule *rule, struct audit_match *match)
{
	struct audit_field f;
	bool is_match_watch = false;
	uint16_t i;
	int rc;

	if (rule->rule_type == HM_AUDIT_WATCH && audit_server_num_get() == __server_fs) {
		return MATCH_RES_NOT_MATCHED;
	}

	rc = audit_syscall_match(rule->mask, match->ctx->syscall_num);
	if (rc == 0) {
		return MATCH_RES_NOT_MATCHED;
	}

	for (i = 0; i < rule->field_count; i++) {
		f.type = rule->fields[i].type;
		f.val = rule->fields[i].value;
		f.op = rule->fields[i].op;
		if (audit_is_need_match_watch(f.type)) {
			/* skip no watch quickly */
			if (match->ctx->watch.field_count == 0) {
				return MATCH_RES_NOT_MATCHED;
			}
			/* One or zero fields whose type is watch or dir. */
			is_match_watch = true;
		} else {
			rc = audit_field_match(&f, match);
			if (rc == FIELD_NOT_MATCH) {
				return MATCH_RES_NOT_MATCHED;
			}
		}
	}

	/* Match lazily because matching watch may be heavy. */
	if (is_match_watch) {
		rc = audit_watch_info_match(rule, match);
		if (rc != MATCH_RES_MATCHED) {
			return MATCH_RES_NOT_MATCHED;
		}
	}

	match->ctx->rule_id = rule->rule_id;
	switch (rule->action) {
	case HM_AUDIT_ALWAYS:
		match->ctx->record_state = AUDIT_RECORD;
		break;
	case HM_AUDIT_NEVER:
		match->ctx->record_state = AUDIT_DISABLED;
		break;
	default:
		break;
	}
	return MATCH_RES_MATCHED;
}

static int audit_field_related_type(const struct audit_shared_field *field)
{
	int ret = AUDIT_OTHE_FIELD;
	switch (field->type) {
	case HM_AUDIT_PID:
	case HM_AUDIT_PPID:
	case HM_AUDIT_UID:
	case HM_AUDIT_EUID:
	case HM_AUDIT_SUID:
	case HM_AUDIT_FSUID:
	case HM_AUDIT_GID:
	case HM_AUDIT_EGID:
	case HM_AUDIT_SGID:
	case HM_AUDIT_FSGID:
	case HM_AUDIT_LOGINUID:
	case HM_AUDIT_SESSIONID:
		ret = AUDIT_CRED_FIELD;
		break;
	default:
		break;
	}
	return ret;
}

static void audit_fill_split_field(const struct audit_shared_field *s_field,
				   struct audit_filter_rule *f_rule, uint32_t field_count)
{
	uint32_t i;
	int filed_type;
	struct audit_field f;
	for (i = 0; i < field_count; i++) {
		filed_type = audit_field_related_type(&s_field[i]);
		f.type = s_field[i].type;
		f.val = s_field[i].value;
		f.op = s_field[i].op;
		if (filed_type == AUDIT_CRED_FIELD) {
			f_rule->prefilter[f_rule->pre_count] = f;
			f_rule->pre_count++;
		} else {
			f_rule->prefilter[(field_count - 1) - f_rule->post_count] = f;
			f_rule->post_count++;
		}
	}
	if (f_rule->pre_count < field_count) {
		f_rule->postfilter = &f_rule->prefilter[f_rule->pre_count];
	}
}

static int audit_filter_rule_split(const struct audit_shared_rule *s_rule,
				    struct audit_filter_rule *f_rule)
{
	int ret;
	uint32_t field_count = (uint32_t)s_rule->field_count;

	f_rule->prefilter = (struct audit_field *)malloc(sizeof(struct audit_field) * field_count);
	if (f_rule->prefilter == NULL) {
		ret = E_HM_NOMEM;
	} else {
		audit_fill_split_field(s_rule->fields, f_rule, field_count);
		ret = E_HM_OK;
	}
	return ret;
}

int audit_filter_rule_fill(struct audit_filter_rule *f_rule, unsigned list, bool append)
{
	int ret = E_HM_OK;
	uint16_t i;
	struct audit_shared_rule *s_rule = NULL;

	if (f_rule == NULL || list >= RULE_LIST_COUNT) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		mem_zero_s(*f_rule);

		RAW_RWLOCK_RD_GUARD(_, &audit_rule_lock(list));

		s_rule = audit_shared_rule_nolock(
			append ? &audit_rule_last(list) : &audit_rule_first(list));
		if (s_rule != NULL) {
			for (i = 0; i < HM_AUDIT_BITMASK_SIZE; i++) {
				f_rule->mask[i] = s_rule->mask[i];
			}
			f_rule->rule_id = s_rule->rule_id;
			f_rule->action = s_rule->action;
			if (s_rule->field_count > 0) {
				ret = audit_filter_rule_split(s_rule, f_rule);
			}
		} else {
			ret = E_HM_NODATA;
		}
	}

	return ret;
}

struct raw_rwlock *audit_shared_rule_list_lock(unsigned list)
{
	return &audit_rule_lock(list);
}

int audit_rule_watch_path_len(const char *path,
			      const struct audit_shared_rule_addr *addr,
			      size_t *path_len)
{
	uint32_t shm_size = 0;
	if (addr == NULL || path == NULL) {
		hm_error("NULL is detected\n");
		return E_HM_INVAL;
	}

	shm_size = shl_shm_size_of(&rule_list, addr->shm_id);
	if (shm_size == 0) {
		hm_error("failed to get the base of the shared memory, id=%hu\n",
			 addr->shm_id);
		return E_HM_NODATA;
	}

	*path_len = strnlen(path, (shm_size - addr->shm_off) - 1);
	return E_HM_OK;
}

void *audit_shared_rule_comm_nolock(const struct audit_shared_rule_addr *addr, uint32_t rule_size)
{
	void *p = NULL;
	uintptr_t base;
	uint32_t shm_size = 0;
	if (addr != NULL) {
		base = shl_shm_base_of(&rule_list, addr->shm_id);
		shm_size = shl_shm_size_of(&rule_list, addr->shm_id);
		if (base == 0) {
			hm_error("failed to get the base of the shared memory, id=%hu\n",
				 addr->shm_id);
		} else if (rule_size > shm_size || addr->shm_off > shm_size - rule_size) {
			hm_error("shm_off is invalid: offset=%hu, size=%u\n", addr->shm_off, shm_size);
		} else {
			p = ulong_to_ptr(base + addr->shm_off, void);
		}
	} else {
		hm_error("NULL is detected\n");
	}
	return p;
}

static int audit_shared_rule_iterate_watch(struct audit_match *match)
{
	int rc = MATCH_RES_NOT_MATCHED;
	struct audit_watch_ctx *w_ctx = &match->ctx->watch;
	for (unsigned int i = 0; i < w_ctx->field_count; ++i) {
		struct audit_watch_field *awf = w_ctx->field[i];
		rc = audit_watch_rule_iterate(awf, match);
		if (rc == MATCH_RES_MATCHED) {
			break;
		}
	}
	return rc;
}

int audit_shared_rule_iterate(unsigned list, struct audit_match *match)
{
	struct audit_shared_rule_addr *addr = &audit_rule_first(list);
	struct audit_shared_rule *rule = NULL;
	uint16_t i;
	int rc = MATCH_RES_NOT_MATCHED;

	RAW_RWLOCK_RD_GUARD(_, &audit_rule_lock(list));

	if (audit_server_num_get() == __server_fs && audit_shared_rule_iterate_watch(match) == MATCH_RES_MATCHED) {
		return MATCH_RES_MATCHED;
	}
	for (i = 0; i < audit_rule_count(list); i++) {
		rule = audit_shared_rule_nolock(addr);
		if (rule == NULL) {
			hm_error("failed to fetch rule\n");
			return E_HM_NODATA;
		}
		rc = audit_shared_rule_match(rule, match);
		if (rc != MATCH_RES_NOT_MATCHED) {
			return rc;
		}
		addr = &rule->node.next;
	}

	return rc;
}

static void audit_shared_rule_list_add(struct audit_shared_rule *s_rule,
				       const struct shl_list_node *node,
				       unsigned list, bool append)
{
	struct audit_shared_rule_addr addr;
	struct audit_shared_rule *last = NULL;
	struct audit_shared_rule *first = NULL;

	audit_shared_addr_ctor(&addr, node);

	if (append) {
		if (audit_rule_last(list).shm_id != INVALID_SHM_ID) {
			last = audit_shared_rule_nolock(&audit_rule_last(list));
			BUG_ON(last == NULL);
			last->node.next = addr;
		} else { /* this is the first inserted rule */
			audit_rule_first_set(list, &addr);
		}
		audit_shared_addr_invalidate(&s_rule->node.next);
		s_rule->node.prev = audit_rule_last(list);
		audit_rule_last_set(list, &addr);
	} else {
		if (audit_rule_first(list).shm_id != INVALID_SHM_ID) {
			first = audit_shared_rule_nolock(&audit_rule_first(list));
			BUG_ON(first == NULL);
			first->node.prev = addr;
		} else { /* this is the first inserted rule */
			audit_rule_last_set(list, &addr);
		}
		audit_shared_addr_invalidate(&s_rule->node.prev);
		s_rule->node.next = audit_rule_first(list);
		audit_rule_first_set(list, &addr);
	}
}

static void audit_shared_rule_fill(struct audit_shared_rule *s_rule,
				   const struct audit_rule *rule,
				   const struct shl_list_node *node,
				   bool append)
{
	uint16_t i;

	s_rule->rule_id = rule->common.id;
	s_rule->action = rule->common.action;
	s_rule->rule_type = 0;

	for (i = 0; i < (uint16_t)HM_AUDIT_BITMASK_SIZE; i++) {
		s_rule->mask[i] = rule->common.mask[i];
	}
	s_rule->field_count = 0;
	for (i = 0; i < rule->common.field_count; i++) {
		if (rule->common.fields[i].type == HM_AUDIT_FILTERKEY) {
			continue;
		}
		s_rule->fields[i].type = rule->common.fields[i].type;
		s_rule->fields[i].value = rule->common.fields[i].val;
		s_rule->fields[i].op = rule->common.fields[i].op;
		s_rule->field_count ++;
		if (rule->common.fields[i].type == HM_AUDIT_WATCH) {
			s_rule->rule_type = HM_AUDIT_WATCH;
		}
	}
	audit_shared_rule_list_add(s_rule, node, rule->common.list_num, append);

	if (rule->watch == NULL) {
		audit_shared_addr_invalidate(&s_rule->watch_addr);
	} else {
		audit_shared_addr_ctor(&s_rule->watch_addr, rule->watch->shared_watch);
	}
}

static int audit_shl_alloc(struct shl_list_node **rule_node, size_t size)
{
	struct shl_list_node *node = NULL;
	int ret = E_HM_OK;
	node = shl_alloc(&rule_list, size);
	if (node == NULL) {
		ret = E_HM_NOMEM;
	}
	if (ret == E_HM_OK) {
		if (node->shm->shm_idx >= INVALID_SHM_ID || node->offset >= INVALID_SHM_OFF) {
			shl_free(&rule_list, node);
			node = NULL;
			ret = E_HM_RANGE;
		}
	}
	if (ret == E_HM_OK) {
		*rule_node = node;
	}
	return ret;
}

int audit_shared_rule_add(struct audit_rule *rule, bool append)
{
	struct audit_shared_rule *s_rule = NULL;
	struct shl_list_node *node = NULL;
	unsigned list = rule->common.list_num;
	size_t size = sizeof(struct audit_shared_rule);
	int ret = E_HM_OK;

	if (rule->common.field_count >= (RULE_SHM_SIZE - sizeof(struct audit_shared_rule)) /
	    sizeof(struct audit_shared_field)) {
		ret = E_HM_NOMEM;
	}
	if (ret == E_HM_OK) {
		size += ALIGN_UP(sizeof(struct audit_shared_field) * rule->common.field_count,
				 sizeof(uint64_t));
		ret = audit_shl_alloc(&node, size);
	}
	if (ret == E_HM_OK) {
		s_rule = shared_rule_of(node);
		RAW_RWLOCK_WR_GUARD(_, &audit_rule_lock(list));
		audit_shared_rule_fill(s_rule, rule, node, append);
		audit_rule_count(list)++;
		rule->common.shared_rule = node;
	}
	if (ret == E_HM_OK && audit_rule_empty()) {
		audit_rule_set_empty_flag(false);
	}
	return ret;
}

static void audit_shared_rule_list_del(struct audit_shared_rule *s_rule,
				       unsigned list)
{
	struct audit_shared_rule *prev = NULL;
	struct audit_shared_rule *next = NULL;
	if (s_rule->node.next.shm_id != INVALID_SHM_ID) {
		next = audit_shared_rule_nolock(&s_rule->node.next);
		BUG_ON(next == NULL);
		next->node.prev = s_rule->node.prev;
	} else {
		audit_rule_last_set(list, &(s_rule->node.prev));
	}
	if (s_rule->node.prev.shm_id != INVALID_SHM_ID) {
		prev = audit_shared_rule_nolock(&s_rule->node.prev);
		BUG_ON(prev == NULL);
		prev->node.next = s_rule->node.next;
	} else {
		audit_rule_first_set(list, &(s_rule->node.next));
	}
	audit_shared_addr_invalidate(&s_rule->node.next);
	audit_shared_addr_invalidate(&s_rule->node.prev);
}

void audit_shared_rule_del(struct audit_rule *rule)
{
	struct audit_shared_rule *s_rule = NULL;
	unsigned list = rule->common.list_num;

	RAW_RWLOCK_WR_GUARD(_, &audit_rule_lock(list));

	s_rule = shared_rule_of(rule->common.shared_rule);
	BUG_ON(s_rule == NULL);
	audit_shared_rule_list_del(s_rule, list);

	shl_free(&rule_list, rule->common.shared_rule);
	audit_rule_count(list)--;
	rule->common.shared_rule = NULL;
	if (rule_is_empty()) {
		audit_rule_set_empty_flag(true);
	}
}

int audit_shared_watch_rule_add(struct audit_watch *watch)
{
	struct shl_list_node *node = NULL;
	size_t path_len = strlen(watch->path);
	size_t path_aligned_size = ALIGN_UP(path_len + 1, sizeof(uint64_t));
	int rc = E_HM_OK;

	rc = audit_shl_alloc(&node, path_aligned_size);
	if (rc == E_HM_OK) {
		NOFAIL(strcpy_s(shared_watch_rule_of(node), path_aligned_size, watch->path));
		watch->shared_watch = node;
	}
	return rc;
}

void audit_shared_watch_rule_del(struct audit_watch *watch)
{
	shl_free(&rule_list, watch->shared_watch);
	watch->shared_watch = NULL;
}

#define WATCH_FIELD_SHM_SIZE 4096u
#define WATCH_FIELD_SHM_NAME_LEN 64u
#define WATCH_FIELD_SHM_NAME_PREFIX "audit-wf-shm-"

#define audit_shm_watch_field_page(shm)    ((struct audit_shared_watch_field_page *)((shm)->base))
#define audit_shm_watch_field_lock(shm)    (audit_shm_watch_field_page(shm)->lock)
#define audit_shm_watch_field_cnt(shm)     (audit_shm_watch_field_page(shm)->free_count)
#define audit_shm_watch_field(shm, n)      (audit_shm_watch_field_page(shm)->field[n])

struct audit_shm *audit_wf_shm;

static inline void audit_shared_watch_field_count_set(struct audit_shm *wf_shm, uint32_t n)
{
	((struct audit_shared_watch_field_page *)((wf_shm)->base))->free_count = n;
}

int audit_shared_watch_field_init(bool create, uint32_t cnode_idx, struct audit_shm **new_shm)
{
	int rc;
	struct audit_shm *wf_shm = NULL;
	char shm_name[WATCH_FIELD_SHM_NAME_LEN] = {0};

	rc = sprintf_s(shm_name, sizeof(shm_name), "%s%u", WATCH_FIELD_SHM_NAME_PREFIX, cnode_idx);
	if (rc < 0) {
		hm_error("failed to concatenate shared watch field name, cnode_idx=%u\n", cnode_idx);
		return E_HM_INVAL;
	}

	wf_shm = audit_shm_make(shm_name, WATCH_FIELD_SHM_SIZE, create, true);
	if (wf_shm == NULL) {
		hm_error("failed to make shared memory\n");
		return E_HM_NOMEM;
	}
	*new_shm = wf_shm;

	if (create) {
		raw_rwlock_init_shared(&audit_shm_watch_field_lock(wf_shm));
		audit_shared_watch_field_count_set(wf_shm, AUDIT_SHM_WATCH_FIELD_MAX_NUM);
		for (unsigned int i = 0; i < AUDIT_SHM_WATCH_FIELD_MAX_NUM; i++) {
			audit_shm_watch_field(wf_shm, i).caller_cref = 0;
		}
	}

	return E_HM_OK;
}

int audit_shared_wf_default_init(bool create, uint32_t cnode_idx)
{
	return audit_shared_watch_field_init(create, cnode_idx, &audit_wf_shm);
}

static struct audit_shared_watch_field *audit_shared_watch_field_find(cref_t cref, struct audit_shm *wf_shm)
{
	struct audit_shared_watch_field *field = NULL;

	if (audit_shm_watch_field_cnt(wf_shm) < AUDIT_SHM_WATCH_FIELD_MAX_NUM) {
		for (unsigned int i = 0; i < AUDIT_SHM_WATCH_FIELD_MAX_NUM; i++) {
			if (audit_shm_watch_field(wf_shm, i).caller_cref == cref) {
				field = &audit_shm_watch_field(wf_shm, i);
				break;
			}
		}
	}

	return field;
}

static struct audit_shared_watch_field *audit_shm_watch_field_alloc(struct audit_shm *wf_shm)
{
	cref_t cref;
	struct audit_shared_watch_field *field = NULL;

	if (audit_shm_watch_field_cnt(wf_shm) == 0) {
		hm_error("the watch fields exhaust\n");
		return NULL;
	}

	cref = hm_thread_self_cref();
	if (IS_REF_ERR(cref)) {
		hm_error("failed to get thread cref, err=%s\n",
			 hmstrerror(REF_TO_ERR(cref)));
		return NULL;
	}

	if (audit_shared_watch_field_find(cref, wf_shm) != NULL) {
		hm_error("the shared watch field exist\n");
		return NULL;
	}

	for (unsigned int i = 0; i < AUDIT_SHM_WATCH_FIELD_MAX_NUM; i++) {
		if (audit_shm_watch_field(wf_shm, i).caller_cref == 0) {
			field = &audit_shm_watch_field(wf_shm, i);
			field->caller_cref = cref;
			field->is_valid = false;
			field->me_idx = -1;
			field->index = 0;
			field->dev_id = 0;
			field->devno = 0;
			audit_shm_watch_field_cnt(wf_shm)--;
			audit_wp_cref = cref;
			break;
		}
	}

	return field;
}

static void audit_shared_watch_field_free(struct audit_shared_watch_field *field, struct audit_shm *wf_shm)
{
	field->caller_cref = 0;
	audit_wp_cref = 0;
	audit_shm_watch_field_cnt(wf_shm)++;
}

void audit_watch_point_start(void)
{
	struct audit_shared_watch_field *field = NULL;

	if (!audit_ctx_is_valid()) {
		return;
	}

	RAW_RWLOCK_WR_GUARD(_, &audit_shm_watch_field_lock(audit_wf_shm));

	/* alloc shared watch field */
	field = audit_shm_watch_field_alloc(audit_wf_shm);
	if (field == NULL) {
		hm_error("failed to allocate a shared watch field\n");
	}
}

static void audit_fill_field(size_t len, const char *path, const char *srv_name,
			     const struct audit_shared_watch_field *shm_field,
			     struct audit_watch_field *field)
{
	field->index = shm_field->index;
	field->dev_id = shm_field->dev_id;
	field->devno = shm_field->devno;
	field->type = shm_field->type;
	field->path_len = len;
	NOFAIL(memcpy_s(field->full_path, len + 1, path, len + 1));
	NOFAIL(memcpy_s(field->srv_name, AUDIT_VFS_SERVER_NAME_MAX_LEN,
			srv_name, strlen(srv_name)));
	field->srv_name[strlen(srv_name)] = '\0';
}

static void audit_fill_shm_field_2_ctx(bool is_exec, const char *path, const char *srv_name,
				       const struct audit_shared_watch_field *shm_field)
{
	int rc;
	size_t len;
	int perm;
	uint32_t field_index;
	struct audit_watch_field *field = NULL;

	do {
		if (!shm_field->is_valid) {
			break;
		}

		perm = is_exec ? (int)((unsigned)shm_field->perm | HM_AUDIT_PERM_EXEC) : shm_field->perm;
		audit_ctx_perm_fill(perm);
		field = (struct audit_watch_field *)malloc(sizeof(struct audit_watch_field));
		if (field == NULL) {
			hm_error("failed to allocate memory for audit watch field\n");
			break;
		}

		len = strlen(path);
		field->full_path = malloc(len + 1);
		if (field->full_path == NULL) {
			hm_error("failed to allocate memory for field path\n");
			goto free_field;
		}
		field->path = field->full_path;

		audit_fill_field(len, path, srv_name, shm_field, field);
		if (audit_ctx_watch_field_exist(field)) {
			goto free_path;
		} else {
			rc = audit_ctx_watch_field_add(field, &field_index);
			if (rc != E_HM_OK) {
				hm_error("failed to add new audit watch field, err=%s\n", hmstrerror(rc));
				goto free_path;
			}
			if (shm_field->me_idx >= 0) {
				audit_ctx_meinfo_fill(shm_field->me_idx, field_index);
			}
		}
	} while(0);

	return;
free_path:
		free(field->full_path);
free_field:
		free(field);
}

void audit_watch_point_end(bool is_exec, const char *path, const char *srv_name)
{
	struct audit_shared_watch_field *shm_field = NULL;

	if (!audit_ctx_is_valid() || audit_wp_cref == 0) {
		return;
	}

	RAW_RWLOCK_WR_GUARD(_, &audit_shm_watch_field_lock(audit_wf_shm));

	shm_field = audit_shared_watch_field_find(audit_wp_cref, audit_wf_shm);
	if (shm_field == NULL) {
		audit_wp_cref = 0;
		hm_error("no matched audit watch fields are found\n");
		return;
	}

	/* save watch field info into local field */
	audit_fill_shm_field_2_ctx(is_exec, path, srv_name, shm_field);

	/* free shared watch field */
	audit_shared_watch_field_free(shm_field, audit_wf_shm);
}

void audit_fill_field_2_shm_field(int me_idx, int perm,
				  const struct audit_watch_field *field,
				  struct audit_shm *wf_shm)
{
	cref_t cref;
	struct audit_shared_watch_field *shm_field = NULL;

	cref = hm_thread_self_cref();
	if (IS_REF_ERR(cref)) {
		hm_error("failed to get thread cref, err=%s\n",
			 hmstrerror(REF_TO_ERR(cref)));
		return;
	}

	shm_field = audit_shared_watch_field_find(cref, wf_shm);
	if (shm_field == NULL) {
		return;
	}

	shm_field->is_valid = true;
	shm_field->me_idx = me_idx;
	shm_field->index = field->index;
	shm_field->dev_id = field->dev_id;
	shm_field->devno = field->devno;
	shm_field->type = field->type;
	shm_field->perm = perm;
}
