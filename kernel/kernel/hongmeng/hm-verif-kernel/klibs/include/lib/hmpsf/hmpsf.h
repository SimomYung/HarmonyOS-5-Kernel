/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description:
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 08 19:51:42 2022
 */
#ifndef KLIBS_HMPSF_H
#define KLIBS_HMPSF_H

#include <lib/dlist.h>
#ifdef __KERNEL__
#include <lib/atomic.h>
#else
#include <vsync/atomic.h>
#endif
#include <lib/hmpsf/hmpsf_def.h>
#include <lib/utils.h>
#include <hmasm/page.h>
#include <hmasm/types.h>
#include <hmkernel/errno.h>
#include <hmkit/types.h>

typedef void* __attribute__((format(printf, 1, 2))) (*hmpsf_log_fn)(const char *fmt, ...);
typedef int (*hmpsf_bind_helpers_fn_t)(void *module, void *env);

#define HMPSF_MAP_HEADER_ALIGN 8

/*
 * Map access through syscall is subject to the limit of the buffer size,
 * while access through shared memory is not.
 */
#define HMPSF_INT_HASH_MAP_KEY_BUF_SIZE		256
#define HMPSF_BYTES_HASH_MAP_KEY_BUF_SIZE	128
#define HMPSF_BYTES_HASH_MAP_VAL_BUF_SIZE	128

struct hmpsf_module_s;

struct hmpsf_if_s {
	void		*(*malloc)(size_t size);
	void		(*free)(void* ptr);
	void		(*lock_init)(void*);
	void		(*lock_acquire)(void*);
	void		(*lock_release)(void*);
	unsigned int	(*lock_size)(void);
	hmpsf_log_fn	log_info;
	hmpsf_log_fn	log_warn;
	hmpsf_log_fn	log_error;
	int (*bind_maps)(struct hmpsf_module_s *module, void *host_module, unsigned int *map_ids, unsigned int map_cnt);
	int (*unbind_maps)(struct hmpsf_module_s *module);
};

struct hmpsf_vm_if_s {
	void* (*load)(struct hmpsf_module_host_attr_s *attr, void *bind_closure, void *psf_module);
	int (*proxy_load_phase_proxy)(struct hmpsf_module_proxy_attr_s *attr);
	void* (*proxy_load_phase_host)(struct hmpsf_module_proxy_attr_s *attr,
				       void *bind_closure, void *psf_module);
	int (*unload)(void *module);
	int (*array_map_lookup)(void *module, unsigned int map_id, unsigned int index,
				unsigned char *value, unsigned long *value_size);
	int (*array_map_update)(void *module, unsigned int map_id, unsigned int index,
				unsigned char *value, unsigned long value_size);
	int (*array_map_remove)(void *module, unsigned int map_id, unsigned int index);
	int (*int_hash_map_check)(void *map, __u32 *size);
	int (*int_hash_map_init)(void *map, void *buf, __u32 buf_size, void *lock_buf, bool init_buf);
	int (*int_hash_map_load)(void *map, const void *key, __u32 key_size, __u64 *val);
	int (*int_hash_map_store)(void *map, const void *key, __u32 key_size, __u64 val);
	int (*int_hash_map_next_key)(void *map, const void *key, __u32 key_size,
				     void *next_key, __u32 *next_key_val);
	int (*int_hash_map_exchange)(void *map, const void *key, __u32 key_size, __u64 val, __u64 *old);
	int (*int_hash_map_compare_exchange)(void *map, const void *key, __u32 key_size,
					     __u64 *expected, __u64 desired);
	int (*int_hash_map_add_fetch)(void *map, const void *key, __u32 key_size, __u64 *val);
	int (*int_hash_map_sub_fetch)(void *map, const void *key, __u32 key_size, __u64 *val);
	int (*int_hash_map_clear)(void *map);
	int (*bytes_hash_map_check)(void *map, __u32 *size);
	int (*bytes_hash_map_init)(void *map, void *buf, __u32 buf_size, void *lock_buf, bool init_buf);
	int (*bytes_hash_map_set)(void *map, const void *key, __u32 key_size,
				  const void *val, __u32 val_size);
	int (*bytes_hash_map_get)(void *map, const void *key, __u32 key_size,
				  void *val, __u32 *val_size);
	int (*bytes_hash_map_next_key)(void *map, const void *key, __u32 key_size,
				       void *next_key, __u32 *next_key_val);
	int (*bytes_hash_map_clear)(void *map);
	int (*perfbuf_check)(void *map, __u32 *size);
};

extern struct hmpsf_if_s *g_hmpsf_if;

#define HMPSF_MAP_OWNER_SYSMGR		0
#define HMPSF_MAP_OWNER_KERNEL		1

/*
 * hmpsf module struct in host vspace, except the kernel.
 *
 * Since the kernel implements hmpsf based on ctrlmem and does not need hmpsf_module_s and some
 * other structs/functions that are current declared and implemented in klibs, we may move them from
 * klibs to ulibs in the future.
 */
struct hmpsf_module_s {
	int id;
	enum hmpsf_module_type module_type;
	struct dlist_node node;
	struct hmpsf_vm_if_s *vm_ifs;
	void *vm;
	u8_t module_lock[HMPSF_MODULE_LOCK_SIZE];
	atomic_t refcnt;
	int (*invalidate[MAX_HMPSF_ATTACH_TYPE])(unsigned int id);
	bool_t invalidated;
	/*
	 * The type of the elements in @maps depends on the host.
	 * - If the host is kernel, it does not use struct hmpsf_module_s at all.
	 * - If the host is sysmgr, although we can store struct hmpsfmgr_map_s in this array,
	 *   we leave this array empty instead, because sysmgr has bookkeeping for the binding
	 *   between maps and modules.
	 * - For other hosts, the type is struct hmpsf_map_s.
	 */
	void *maps[HMPSF_MODULE_MAX_MAPS];
	unsigned int map_cnt;
} __attribute__((aligned(sizeof(__u64))));

int hmpsf_check_module_attr(struct hmpsf_module_attr_s *attr);

static inline int hmpsf_check_module_host_attr(struct hmpsf_module_host_attr_s *attr)
{
	int err;

	err = hmpsf_check_module_attr(&attr->module_attr);
	if (err == E_HM_OK) {
		if (attr->code_size == 0 || attr->code_size > HMPSF_MAX_CODE_SIZE) {
			err = E_HM_INVAL;
		}
	}

	return err;
}

static inline int hmpsf_check_module_proxy_attr(struct hmpsf_module_proxy_attr_s *attr)
{
	int err;

	err = hmpsf_check_module_host_attr(&attr->host_attr);
	if (err == E_HM_OK) {
		if (attr->total_size > HMPSF_MAX_PREALLOC_SIZE ||
		    attr->proxy_loaded_data_size > attr->total_size) {
			err = E_HM_INVAL;
		}
	}

	return err;
}

/*
 * hmpsf globally-shared map struct in host vspace
 */
struct hmpsf_map_s {
	/*
	 * As klibs cannot depend on hm-xvm, @map (of type xvm_map_t) is opaque to klibs.
	 * Therefore, klibs need to have a duplicated id here.
	 */
	unsigned int id;

	/*
	 * In the current simple implementation, hmpsf_map_s is allocated in reserved buffer. So,
	 * we do not need any linked list to store hmpsf_map_s. In future implementation where
	 * hmpsf_map_s is allocated dynamically, we need to add
	 *     struct dlist_node node;
	 * to store and manage hmpsf_map_s.
	 */

	atomic_t refcnt;
	/* The map is located at the end of struct hmpsf_map_s */
} __attribute__((aligned(sizeof(__u64))));

struct hmpsf_vm_if_s *hmpsf_vm_ifs_lookup(enum hmpsf_vm_type type);
int hmpsf_init(struct hmpsf_if_s *ifs, void *module_list_lock, unsigned char shmem_lf_hash_map_prio);
void hmpsf_kernel_init(void);
int hmpsf_load_vm_with_helpers(struct hmpsf_module_host_attr_s *attr,
			       struct hmpsf_module_s *module, hmpsf_bind_helpers_fn_t bind_helpers);
int hmpsf_proxy_load_vm_with_helpers(struct hmpsf_module_proxy_attr_s *attr,
				     struct hmpsf_module_s *module, hmpsf_bind_helpers_fn_t bind_helpers);
int hmpsf_load(int id, struct hmpsf_module_host_attr_s *attr, void *host_module,
	       hmpsf_bind_helpers_fn_t bind_helpers);
int hmpsf_proxy_load_phase_proxy(struct hmpsf_module_proxy_attr_s *attr);
int hmpsf_unload(unsigned int module_id);
int hmpsf_unload_and_return(unsigned int module_id, struct hmpsf_module_s **module);
int hmpsf_invalidate(int id);
void hmpsf_insert_module(struct hmpsf_module_s *module);
struct hmpsf_module_s *hmpsf_module_get(int id);
int hmpsf_module_init(struct hmpsf_module_s *module, int id, struct hmpsf_module_host_attr_s *attr);
void hmpsf_module_put(struct hmpsf_module_s *module);
void hmpsf_module_lock(struct hmpsf_module_s *module);
void hmpsf_module_unlock(struct hmpsf_module_s *module);

int hmpsf_array_map_lookup(unsigned int module_id, unsigned int map_id, unsigned int index,
			   unsigned char *value, unsigned long *value_size);
int hmpsf_array_map_update(unsigned int module_id, unsigned int map_id, unsigned int index,
			   unsigned char *value, unsigned long value_size);
int hmpsf_array_map_remove(unsigned int module_id, unsigned int map_id, unsigned int index);

#if defined(__HOST_LLT__) || defined(CONFIG_DEBUG_BUILD)
#define HMPSF_TEST_FAKE_MAP_ID 5
#define FAKE_MAP_MAX_VALUE_SIZE 20
struct hmpsf_vm_if_s* hmpsf_fake_vm_register(struct hmpsf_if_s* ifs);
#endif

int hmpsf_empty_bind_helpers(void *module, void *env);

#endif
