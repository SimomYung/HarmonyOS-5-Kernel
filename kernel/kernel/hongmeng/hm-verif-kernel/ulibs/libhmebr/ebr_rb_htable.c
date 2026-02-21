/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementation of ebr-safe hash table based-on red-black tree
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 04 11:57:14 2021
 */

#include <libhmebr/ebr.h>
#include <libhmebr/ebr_rb_htable.h>
#include <hongmeng/errno.h>

#define U32_BIT_CNT		32U
#define U64_BIT_CNT		64U
#define HASH_CHAR_FACTOR	5
#define GOLDEN_MEAN_32		2654435769U
#define GOLDEN_MEAN_64		1400714813093931008ULL
#define MAX_SIZE (0xFFFFFFFFU / sizeof(struct ebr_rbt_tree))

/* next power of 2 for n */
static inline uint32_t next_power(uint32_t n)
{
	return (uint32_t)(1U << (uint32_t)((8U * sizeof(uint32_t)) -
	       (uint32_t)__builtin_clz(n - 1U)));
}

static inline struct ebr_rbt_tree *ebr_rbt_htable_bucket(struct ebr_rbt_htable *htable,
						  uint32_t hash)
{
	return &htable->table[hash & (htable->size - 1U)];
}

int ebr_rbt_htable_init(struct ebr_rbt_htable *htable,
			struct ebr_rbt_htable_ops *ops,
			uint32_t size, size_t max)
{
	size_t i;
	int ret = E_HM_OK;
	size_t htable_size;

	if (htable == NULL || ops == NULL ||
	    ops->alloc == NULL || ops->release == NULL ||
	    ops->key_hash == NULL || ops->node_hash == NULL ||
	    ops->key_cmp == NULL || ops->node_cmp == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		htable->size = next_power(size);
		if (htable->size < size || htable->size > MAX_SIZE) {
			ret = E_HM_OVERFLOW;
		}
	}
	if (ret == E_HM_OK) {
		htable_size = (size_t)htable->size * sizeof(struct ebr_rbt_tree);
		ebr_assign_pointer(htable->table, ops->alloc(htable_size));
		if (htable->table == NULL) {
			ret = E_HM_NOMEM;
		}
	}

	if (ret == E_HM_OK) {
		vatomic64_write(&htable->cnt, 0U);
		htable->cnt_max = max;
		htable->entropy = __U(DEFAULT_ENTROPY);
		ebr_assign_pointer(htable->ops, ops);
		for (i = 0; i < htable->size; i++) {
			ret = ebr_rbt_init(&htable->table[i],
					   ops->key_cmp, ops->node_cmp);
			BUG_ON(ret != E_HM_OK);
		}
	}
	return ret;
}

int ebr_rbt_htable_destroy(struct ebr_rbt_htable *htable)
{
	int ret = E_HM_OK;
	if (htable == NULL || htable->ops == NULL) {
		ret = E_HM_INVAL;
	} else if (vatomic64_read(&htable->cnt) > 0U) {
		ret = E_HM_PERM;
	} else {
		/* do nothing */
	}
	if (ret == E_HM_OK) {
		htable->ops->release(htable->table);
		htable->table = NULL;
		htable->ops = NULL;
	}
	return ret;
}

bool ebr_rbt_htable_is_empty(struct ebr_rbt_htable *htable)
{
	bool ret = true;
	if (htable != NULL) {
		ret = (vatomic64_read(&htable->cnt) == 0U);
	}
	return ret;
}

int ebr_rbt_htable_set_entropy(struct ebr_rbt_htable *htable,
			       uint32_t entropy)
{
	int ret = E_HM_OK;
	if (htable == NULL || entropy < __U(DEFAULT_ENTROPY)) {
		ret = E_HM_INVAL;
	} else if (vatomic64_read(&htable->cnt) > 0U) {
		ret = E_HM_OBJEXIST;
	} else {
		/* do nothing */
	}

	if (ret == E_HM_OK) {
		htable->entropy = entropy;
	}
	return ret;
}

ebr_rbt_hnode *ebr_rbt_htable_find(struct ebr_rbt_htable *htable,
					  const void *key)
{
	uint32_t hash;
	struct ebr_rbt_tree *bucket;

	if (unlikely(htable == NULL || htable->table == NULL || htable->ops == NULL)) {
		return NULL;
	}

	hash = htable->ops->key_hash(htable, key);
	bucket = ebr_rbt_htable_bucket(htable, hash);

	return ebr_rbt_find(bucket, key);
}

int ebr_rbt_htable_insert(struct ebr_rbt_htable *htable,
			  ebr_rbt_hnode *node)
{
	uint32_t hash = 0;
	int ret = E_HM_OK;
	struct ebr_rbt_tree *bucket = NULL;

	if (htable == NULL || htable->table == NULL ||
	    htable->ops == NULL || node == NULL) {
		ret = E_HM_INVAL;
	} else if (vatomic64_read(&htable->cnt) == (unsigned long long)htable->cnt_max) {
		ret = E_HM_OVERFLOW;
	} else {
		/* do nothing */
	}
	if (ret == E_HM_OK) {
		hash = htable->ops->node_hash(htable, node);
		bucket = ebr_rbt_htable_bucket(htable, hash);
		ret = ebr_rbt_insert(bucket, node);
		if (ret == E_HM_OK) {
			vatomic64_inc(&htable->cnt);
		}
	}
	return ret;
}

int ebr_rbt_htable_remove(struct ebr_rbt_htable *htable,
			  ebr_rbt_hnode *node)
{
	uint32_t hash = 0;
	int ret = E_HM_OK;
	struct ebr_rbt_tree *bucket = NULL;

	if (htable == NULL || htable->table == NULL ||
	    htable->ops == NULL || node == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		hash = htable->ops->node_hash(htable, node);
		bucket = ebr_rbt_htable_bucket(htable, hash);
		ret = ebr_rbt_remove(bucket, node);
		if (ret == E_HM_OK) {
			vatomic64_dec(&htable->cnt);
		}
	}
	return ret;
}
int ebr_rbt_htable_apply(struct ebr_rbt_htable *htable,
			 int (*callback)(ebr_rbt_hnode *node, void *arg),
			 void *arg)
{
	int ret = E_HM_OK;
	unsigned int i;

	if (callback == NULL || htable == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		for (i = 0; i < htable->size; i++) {
			ret = ebr_rbt_apply(&htable->table[i], callback, arg);
			if (ret != E_HM_OK) {
				break;
			}
		}
	}

	return ret;
}

/*
 * Hash Functions
 *
 * Compared to shift hashing such as thomas hashing , multiplicative hashing
 * for integer based on golden section provides better performance in
 * worst situation.
 *
 * In addition, the size of hash table also affects the result of integer
 * hashing.
 *
 */
uint32_t ebr_rbt_hash_u32(const struct ebr_rbt_htable *htable, uint32_t key)
{
	return (key * GOLDEN_MEAN_32 * htable->entropy) >>
	       (uint32_t)(U32_BIT_CNT - (uint32_t)__builtin_ctz(htable->size));
}

uint32_t ebr_rbt_hash_u64(const struct ebr_rbt_htable *htable, uint64_t key)
{
	return (uint32_t)((key * GOLDEN_MEAN_64 * htable->entropy) >>
			  (uint32_t)(U64_BIT_CNT - (uint32_t)__builtin_ctz(htable->size)));
}

uint32_t ebr_rbt_hash_str(const struct ebr_rbt_htable *htable, const char *str)
{
	size_t i;
	size_t len;
	uint32_t hash = 0;
	unsigned char uchar;

	len = strlen(str);
	for (i = 0; i < len; i++) {
		uchar = (unsigned char)str[i];
		hash = ((hash << HASH_CHAR_FACTOR) - hash) + (uint32_t)uchar;
	}

	return ebr_rbt_hash_u32(htable, hash);
}
