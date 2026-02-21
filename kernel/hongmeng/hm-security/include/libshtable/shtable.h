/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Shared hash table head file
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 10:48:42 2019
 */
#ifndef ULIBS_LIBSHTABLE_SHTABLE_H
#define ULIBS_LIBSHTABLE_SHTABLE_H

/* There is a bug of LLT of sysmgr. This is just a trick. */
#ifndef __HOST_LLT__
#include <stdbool.h>
#endif
#include <stddef.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmsync/raw_mutex.h>
#include <libseqlock/seqlock.h>
#include "types.h"

typedef seq_t sht_seq_t;

typedef int (*sht_key_cmp_ft)(const void *one, const void *other);
typedef void (*sht_key_clear_ft)(void *key);

/*
 * Check use-after-free:
 * Just used in sht_config and sht_table, and it will be checked in these struct.
 */
struct sht_key_desc {
	size_t size;
	sht_key_cmp_ft cmp;
	sht_key_clear_ft clear;
};

typedef uint32_t (*sht_hash_ft)(const void *key, size_t size);
struct sht_hash_desc {
	sht_hash_ft hash;
};

/*
 * Check use-after-free:
 * sht_config will init and be used in libsec and libshtable. Before used, it will
 * check_valid to judge whether the pointers are NULL. And all pointers are static
 * initialization, and they won't be free.
 */
struct sht_config {
	struct sht_key_desc	key_desc;
	struct sht_hash_desc	hash_desc;
	size_t			size;
	const char		*prefix;
};

/*
 * Check use-after-free:
 * All pointers will init in sht_shm_open. And they will free and set
 * to NULL in sht_shm_close. For caller, properly use these two interface
 * will not cause UAF.
 */
struct sht_shm {
	char		*name;
	size_t		size;
	uintptr_t	base;
};

struct sht_list_node {
	sht_idx_t next;	/* the index of node */
	sht_idx_t prev;	/* the index of node */
};

/*
 * Check use-after-free:
 * Just used in shtable, and they will be checked together.
 */
struct sht_ctrl {
	struct sht_list_node	free_list;	/* the head of free list */
	sht_uint_t		free_num;
	struct raw_mutex	free_list_lock;
	struct raw_mutex	htable_lock;
};

/*
 * Check use-after-free:
 * For sht_shm struct in shtable, all shm use sht_shm_open and sht_shm_close
 * correctly to avoid UAF. For the key_desc and hash_desc, they will init by
 * sht_config and not be free. For prefix and ctrl, they will init in sht_init.
 * They will free and set to NULL in sht_fini or init error.
 *
 * For the caller, using shtable between sht_init and sht_fini will not cause
 * UAF.
 */
struct shtable {
	struct sht_shm		bucket_shm;
	struct sht_shm		node_shm;
	struct sht_shm		key_shm;
	struct sht_key_desc	key_desc;
	struct sht_hash_desc	hash_desc;
	sht_uint_t		size;
	char			*prefix;
	struct sht_ctrl		*ctrl;
};

/*
 * Check use-after-free:
 * For sht_shm struct in sht_mod, sht_mod_init and sht_mod_fini use sht_shm's
 * interfaces correctly to avoid UAF. For the caller, they need using 
 * sht_mod->sht between sht_init and sht_fini. And sht_mod is used in 
 * libsec/dac.c, and the shtable's interfaces are used correctly to aoivd UAF.
 * */
struct sht_mod {
	struct sht_shm		shm;
	size_t			element_size;
	const struct shtable	*sht;
};

int sht_init(struct shtable *sht, const struct sht_config *cfg, bool ctrl);
void sht_fini(struct shtable *sht);

int sht_mod_init(const struct shtable *sht, struct sht_mod *mod,
		 const char *name, size_t element_size);
void sht_mod_fini(struct sht_mod *mod);

static inline void *sht_mod_ref(const struct sht_mod *mod, sht_idx_t node)
{
	return ptr_to_void(mod->shm.base + mod->element_size * node);
}

int sht_search(const struct shtable *sht, const void *key, sht_idx_t *p_node,
	       seq_t *p_seq);

/* For creators */
typedef int (*sht_cb_ft)(sht_idx_t node, void *args);

/*
 * Check use-after-free:
 * The memory of all pointers depend caller's init value. After free memory pointed
 * by sht_call_back, caller needs to set the pointers in sht_call_back to NULL and
 * stop using related functions.
 */
struct sht_call_back {
	sht_cb_ft func;
	void *args;
	int *result;
};

static inline void sht_cb_init(struct sht_call_back *cb, sht_cb_ft func,
			       void *args, int *result)
{
	cb->func = func;
	cb->args = args;
	cb->result = result;
}

int sht_insert_raw(const struct shtable *sht, const void *key,
		   const struct sht_call_back *cb);
int sht_update_raw(const struct shtable *sht, const void *key,
		   const struct sht_call_back *cb);
int sht_read_raw(const struct shtable *sht, sht_idx_t node,
		 const struct sht_call_back *cb);
int sht_delete(const struct shtable *sht, const void *key);

static inline int sht_insert(const struct shtable *sht, const void *key,
			     sht_cb_ft func, void *args, int *result)
{
	struct sht_call_back cb;
	sht_cb_init(&cb, func, args, result);
	return sht_insert_raw(sht, key, &cb);
}

static inline int sht_update(const struct shtable *sht, const void *key,
			     sht_cb_ft func, void *args, int *result)
{
	struct sht_call_back cb;
	sht_cb_init(&cb, func, args, result);
	return sht_update_raw(sht, key, &cb);
}

static inline int sht_read(const struct shtable *sht, sht_idx_t node,
			   sht_cb_ft func, void *args, int *result)
{
	struct sht_call_back cb;
	sht_cb_init(&cb, func, args, result);
	return sht_read_raw(sht, node, &cb);
}

int sht_retry(const struct shtable *sht, sht_idx_t node, seq_t seq);

int sht_mod_query(const struct sht_mod *mod, const void *key, void *value);

/*
 * Check use-after-free:
 * The memory of all pointers depend caller's init value. After free memory pointed
 * by sht_mod_cb_arg, caller needs to set the pointers in struct to NULL and stop
 * using related functions.
 */
struct sht_mod_cb_arg {
	const struct sht_mod *mod;
	void *value;
	size_t size;
};

int sht_write_value_cb(sht_idx_t node, void *arg);
int sht_read_value_cb(sht_idx_t node, void *arg);


static inline struct sht_mod_cb_arg sht_mod_cb_make(const struct sht_mod *mod,
						    void *value, size_t size)
{
	struct sht_mod_cb_arg arg;
	arg.mod = mod;
	arg.value = value;
	arg.size = size;
	return arg;
}

static inline int sht_mod_update(const struct sht_mod *mod, const void *key,
				 void *value)
{
	int ret = (mod == NULL || value == NULL) ? E_HM_INVAL : E_HM_OK;
	struct sht_mod_cb_arg arg;

	if (ret == E_HM_OK) {
		arg = sht_mod_cb_make(mod, value, mod->element_size);
		ret = sht_update(mod->sht, key, sht_write_value_cb, &arg, NULL);
	}

	return ret;
}

static inline int sht_mod_read(const struct sht_mod *mod, sht_idx_t node,
			       void *value)
{
	int ret = (mod == NULL || value == NULL) ? E_HM_INVAL : E_HM_OK;
	struct sht_mod_cb_arg arg;

	if (ret == E_HM_OK) {
		arg = sht_mod_cb_make(mod, value, mod->element_size);
		ret = sht_read(mod->sht, node, sht_read_value_cb, &arg, NULL);
	}

	return ret;
}

/*
 * Check use-after-free:
 * shtable_simple just packs shtable and sht_mod. For the caller, they need
 * to use shtable_simple between init and fini to avoid UAF.
 */
struct shtable_simple {
	struct shtable htable;
	struct sht_mod value;
};

int sht_simple_init(struct shtable_simple *sht, const struct sht_config *cfg,
		    size_t value_size, bool ctrl);
void sht_simple_fini(struct shtable_simple *sht);

static inline int sht_simple_search(const struct shtable_simple *sht,
				    const void *key, sht_idx_t *p_node, seq_t *p_seq)
{
	int ret;
	if (sht == NULL) {
		ret = E_HM_INVAL;
	} else {
		ret = sht_search(&sht->htable, key, p_node, p_seq);
	}

	return ret;
}

static inline int sht_simple_delete(const struct shtable_simple *sht,
				    const void *key)
{
	int ret;
	if (sht == NULL) {
		ret = E_HM_INVAL;
	} else {
		ret = sht_delete(&sht->htable, key);
	}

	return ret;
}

static inline int sht_simple_insert(const struct shtable_simple *sht,
				    const void *key, void *value)
{
	int ret = (sht == NULL || value == NULL) ? E_HM_INVAL : E_HM_OK;
	struct sht_mod_cb_arg arg;

	if (ret == E_HM_OK) {
		arg = sht_mod_cb_make(&sht->value, value, sht->value.element_size);
		ret = sht_insert(&sht->htable, key, sht_write_value_cb, &arg, NULL);
	}

	return ret;
}

static inline int sht_simple_update(const struct shtable_simple *sht,
				    const void *key, void *value)
{
	int ret;
	if (sht == NULL) {
		ret = E_HM_INVAL;
	} else {
		ret = sht_mod_update(&sht->value, key, value);
	}

	return ret;
}

static inline int sht_simple_read(const struct shtable_simple *sht,
				  sht_idx_t node, void *value)
{
	int ret;
	if (sht == NULL) {
		ret = E_HM_INVAL;
	} else {
		ret = sht_mod_read(&sht->value, node, value);
	}

	return ret;
}

static inline int sht_simple_retry(const struct shtable_simple *sht,
				   sht_idx_t node, seq_t seq)
{
	int ret;
	if (sht == NULL) {
		ret = E_HM_INVAL;
	} else {
		ret = sht_retry(&sht->htable, node, seq);
	}

	return ret;
}

static inline int sht_simple_query(const struct shtable_simple *sht,
				   const void *key, void *value)
{
	int ret;
	if (sht == NULL) {
		ret = E_HM_INVAL;
	} else {
		ret = sht_mod_query(&sht->value, key, value);
	}

	return ret;
}
#endif /* ifndef ULIBS_LIBSHTABLE_SHTABLE_H */
