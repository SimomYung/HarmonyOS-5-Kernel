/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Provide hmprobe basic structure
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 23 15:29:10 2023
 */

#ifndef KLIBS_HMPROBE_H
#define KLIBS_HMPROBE_H

#include <lib/dlist.h>
#ifdef __KERNEL__
#include <lib/atomic.h>
#include <hmkernel/errno.h>
#else
#include <vsync/atomic.h>
#include <hongmeng/errno.h>
#endif
#include <lib/hmprobe/utils.h>
#include <hmkit/types.h>

typedef void* __attribute__((format(printf, 1, 2))) (*hmprobe_log_func_t)(const char *fmt, ...);
struct hmprobe_if_s {
	void *(*event_malloc)(void);
	void (*event_free)(void *ptr, unsigned long long *pfn_wait_free);
	void *(*runtime_malloc)(void);
	void (*runtime_free)(void *ptr, unsigned long long *pfn_wait_free);
	void *(*module_malloc)(void);
	void (*module_free)(void *ptr, unsigned long long *pfn_wait_free);
	void (*lock_init)(void*);
	void (*lock_acquire)(void*);
	void (*lock_release)(void*);
	hmprobe_log_func_t log_info;
	hmprobe_log_func_t log_warn;
	hmprobe_log_func_t log_error;
};

int hmprobe_ifs_init(struct hmprobe_if_s *ifs);

struct hmprobe_obj_s {
	vatomic32_t refcnt;
};

#define HMPROBE_MODULE_KEEP_HOST_DATA_MAX_SIZE		8

struct hmpsf_module_s;
struct hmprobe_module_keep_s {
	struct hmprobe_obj_s obj;
	unsigned int module_id;
	/*
	 * For the kernel, the module type is struct ctrlmem_syscall_ukobjhmpsfmod_s.
	 * For others, the module type is struct hmpsf_module_s.
	 */
	void *module;
	u8_t lock[HMPROBE_LOCK_SIZE];
	/* linked by this module's hmprobe_run_entry_s */
	struct dlist_node run_entry_list;
	struct dlist_node list;

	/*
	 * Currently it is only used to store kobj state in the kernel.
	 */
	u8 host_data[HMPROBE_MODULE_KEEP_HOST_DATA_MAX_SIZE];
};

struct hmprobe_run_args_s {
	unsigned char *ctx;
	unsigned long long ctx_size;
};

struct hmprobe_runtime_s;
struct hmprobe_event_s;
struct hmprobe_run_entry_s {
	char func_name[HMPROBE_FUNC_NAME_MAX_SIZE];
	struct hmprobe_module_keep_s *module_keep;
	unsigned int prog_id;
	/* get before used */
	void *prog;
	/* in which runtime entry */
	struct hmprobe_runtime_s *runtime_entry;
	struct hmprobe_event_s *parent_event;
	struct dlist_node list;
};

/*
 * hmprobe supports three program runtime states:
 * (1) HMPROBE_PROG_STATE_INIT:
 *     Initialization state, no prog has been attached here.
 * (2) HMPROBE_PROG_STATE_ACTIVE:
 *     prog has been attached, runnable.
 * (3) HMPROBE_PROG_STATE_INVALID:
 *     prog has been attached, but not runnable, this always
 *     happens during detach.
 */
enum hmprobe_prog_status {
	HMPROBE_PROG_STATE_INIT = 0,
	HMPROBE_PROG_STATE_ACTIVE = 1,
	HMPROBE_PROG_STATE_INVALID = 2,
};

struct hmprobe_prog_s {
	struct hmprobe_obj_s obj;
	enum hmprobe_prog_status status;
	struct hmprobe_run_entry_s run_entry;
};

struct hmprobe_runtime_s {
	struct hmprobe_obj_s obj;
	void *event_instance;
	u8_t lock[HMPROBE_LOCK_SIZE];
	int (*prog_run_default_handler)(void *context, unsigned long long ctx_size, void *this_runtime_entry);
	struct hmprobe_prog_s prog_array[HMPROBE_EVENT_MAX_ATTACH_TIMES];
};

struct hmprobe_event_s {
	struct hmprobe_obj_s obj;
	char event_name[HMPROBE_EVENT_NAME_MAX_SIZE];
	unsigned int event_id;
	/* kev, uprobe or uev... */
	hmprobe_event_type_t event_type;
	/* to this host's event_ifs */
	struct hmprobe_event_if_s *event_ifs;
	/* pointer to main event instanceor this event's runtime_entry */
	struct hmprobe_runtime_s *runtime_entry;
	u8_t lock[HMPROBE_LOCK_SIZE];
	/* insert to global event list */
	struct dlist_node list;
};

#define HMPROBE_EVENT_METADATA_PAGE_ORDER 0
#define HMPROBE_RUNTIME_METADATA_PAGE_ORDER 1
#define HMPROBE_MODULE_METADATA_PAGE_ORDER 0

enum hmprobe_prealloc_zone_type {
	HMPROBE_PREALLOC_MODULE_ZONE_TYPE = 0,
	HMPROBE_PREALLOC_EVENT_ZONE_TYPE = 1,
	HMPROBE_PREALLOC_RUNTIME_ZONE_TYPE = 2,
	NR_HMPROBE_PREALLOC_ZONE_TYPE,
};

typedef int (*hmprobe_hook_func_t)(void *, unsigned long long, void *);

struct hmprobe_event_if_s {
	int (*create)(union hmprobe_event_point_s *ev_point, void **event_instance);
	int (*destroy)(void *event_instance);
	int (*enable)(void *event_instance);
	int (*disable)(void *event_instance);
	int (*attach)(struct hmprobe_runtime_s *arg, void *event_instance);
	int (*detach)(struct hmprobe_runtime_s *arg, void *event_instance);
};

int hmprobe_create_event(struct hmprobe_event_host_attr_s *attr, unsigned int *event_id);
int hmprobe_destroy_event(unsigned int event_id, struct hmprobe_event_pre_alloc_pfn_s *pfn_list);
int hmprobe_event_attach(struct hmprobe_module_attach_attr_s *attr, unsigned int *prog_id);
int hmprobe_event_detach(struct hmprobe_module_attach_attr_s *attr, unsigned int prog_id);
void hmprobe_event_clean(struct hmprobe_event_s *event, bool new_runtime);
void hmprobe_event_put(struct hmprobe_event_s *event);
struct hmprobe_event_s *hmprobe_event_get(unsigned int event_id);
int hmprobe_keep_hmpsf_module(unsigned int module_id);
int hmprobe_unkeep_hmpsf_module(unsigned int module_id, struct hmprobe_module_pre_alloc_pfn_s *pfn_list);
void hmprobe_module_keep_insert(struct hmprobe_module_keep_s *module_keep);
int hmprobe_module_keep_invalidate(unsigned int module_id);
struct hmprobe_module_keep_s *hmprobe_module_keep_find_locked(unsigned int module_id);
struct hmprobe_runtime_s *hmprobe_runtime_entry_get(void *event_instance);
int hmprobe_delete_run_entry_safe(struct hmprobe_run_entry_s *old);
int hmprobe_insert_run_entry_safe(struct hmprobe_runtime_s *runtime_entry,
					 struct hmprobe_run_entry_s *new);
unsigned int hmprobe_alloc_id(void);
void hmprobe_free_id(unsigned int id);
void hmprobe_init_if(struct hmprobe_if_s *ifs);
void hmprobe_init_event_if(struct hmprobe_event_if_s *event_tracepoint_ifs,
			   struct hmprobe_event_if_s *event_probe_ifs);
void hmprobe_init(void);

extern struct hmprobe_event_if_s *hmprobe_event_if[NR_HMPROBE_EVENT_TYPE];
extern struct dlist_node g_hmprobe_event_list;
extern u8 g_hmprobe_module_lock[HMPROBE_LOCK_SIZE];
extern u8 g_hmprobe_event_lock[HMPROBE_LOCK_SIZE];

struct hmprobe_module_keep_s *hmprobe_module_keep_get(unsigned int module_id);
void hmprobe_module_keep_put(struct hmprobe_module_keep_s *module_keep);

static inline void hmprobe_obj_refcnt_set(struct hmprobe_obj_s *obj, unsigned int refcnt)
{
	vatomic32_init(&obj->refcnt, refcnt);
}

static inline unsigned int hmprobe_obj_refcnt_of(struct hmprobe_obj_s *obj)
{
	return vatomic32_read(&obj->refcnt);
}

static inline unsigned int hmprobe_obj_refcnt_inc(struct hmprobe_obj_s *obj)
{
	return vatomic32_inc_get(&obj->refcnt);
}

static inline unsigned int hmprobe_obj_refcnt_dec(struct hmprobe_obj_s *obj)
{
	return vatomic32_dec_get(&obj->refcnt);
}

static inline bool hmprobe_vatomic32_inc_when_not_zero(vatomic32_t *refcnt)
{
	__u32 oldv;
	do {
		oldv = vatomic32_read_rlx(refcnt);
		if (unlikely(oldv == 0)) {
			break;
		}
	} while (vatomic32_cmpxchg(refcnt, oldv, oldv + 1) != oldv);
	return oldv != 0;
}

static inline bool hmprobe_obj_inc_when_not_zero(struct hmprobe_obj_s *obj)
{
	return hmprobe_vatomic32_inc_when_not_zero(&obj->refcnt);
}

static inline struct hmprobe_event_if_s *hmprobe_event_if_of(hmprobe_event_type_t event_type)
{
	struct hmprobe_event_if_s *event_if = NULL;

	if ((unsigned int)event_type < NR_HMPROBE_EVENT_TYPE) {
		event_if = hmprobe_event_if[event_type];
	}
	return event_if;
}
#endif
