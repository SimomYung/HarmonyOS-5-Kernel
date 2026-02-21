/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Rbtree-based hash table implementation
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 3 19:29:37 2018
 */
#include <libalgo/rb_htable.h>
#include <hongmeng/errno.h>

#define U32_BIT_CNT		32
#define U64_BIT_CNT		64
#define HASH_CHAR_FACTOR	5
#define GOLDEN_MEAN_32		2654435769U
#define GOLDEN_MEAN_64		1400714813093931008ULL
#define MAX_SIZE		(0xFFFFFFFFU / sizeof(struct rbt_bucket))

/* next power of 2 for n */
static inline uint32_t next_power(uint32_t n)
{
	/* 8 refers to 8 bits in one byte */
	return ((uint32_t)1 << (((uint32_t)8 * sizeof(uint32_t)) - (uint32_t)__builtin_clz(n - 1U)));
}

int rbt_htable_init(struct rbt_htable *htable, void *(*alloc)(size_t sz),
		    uint32_t size, size_t max)
{
	size_t i;
	int rc = E_HM_OK;

	htable->size = next_power(size);
	/*
	 * check inversion for bucket size
	 * 1. next power of 2 inversion
	 * 2. memory size for alloc
	 *
	 */
	if ((htable->size < size) || (htable->size > MAX_SIZE)) {
		rc = E_HM_OVERFLOW;
	}
	if (rc == E_HM_OK) {
		raw_atomic_long_init(&htable->cnt, 0);
		htable->cnt_max = (long)max;
		/* set entropy value to default */
		htable->entropy = DEFAULT_ENTROPY;

		htable->table = (struct rbt_bucket *)
				alloc(htable->size * sizeof(struct rbt_bucket));
		if (htable->table != NULL) {
			for (i = 0; i < htable->size; i++) {
				htable->table[i].tree.root = NULL;
				raw_rwlock_init(&htable->table[i].rwlock, NULL);
			}
		} else {
			rc = E_HM_NOMEM;
		}
	}

	return rc;
}

int rbt_htable_set_entropy(struct rbt_htable *htable, uint32_t entropy)
{
	int rc = E_HM_OK;

	if (entropy < (uint32_t)DEFAULT_ENTROPY) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		if (raw_atomic_long_read(&htable->cnt) > 0) {
			rc = E_HM_OBJEXIST;
		}
	}
	if (rc == E_HM_OK) {
		htable->entropy = entropy;
	}

	return rc;
}

struct rbt_bucket *rbt_htable_bucket(struct rbt_htable *htable, uint32_t hcode)
{
	return &htable->table[hcode & (htable->size - 1U)];
}

int rbt_htable_insert(struct rbt_htable *htable, struct rbt_bucket *bucket,
		      struct rbt_hnode *node, struct rbt_hnode *parent, bool is_right)
{
	int rc = E_HM_OK;

	if ((raw_atomic_long_read(&htable->cnt) >= 0) &&
	    raw_atomic_long_read(&htable->cnt) == htable->cnt_max) {
		rc = E_HM_OVERFLOW;
	}
	if (rc == E_HM_OK) {
		rbt_insert(&bucket->tree, node, parent, is_right);
		(void)raw_atomic_long_inc(&htable->cnt);
	}

	return rc;
}

void rbt_htable_remove(struct rbt_htable *htable, struct rbt_bucket *bucket,
		       struct rbt_hnode *node)
{
	if (rbt_is_removable_node(node)) {
		rbt_remove(&bucket->tree, node);
		(void)raw_atomic_long_dec(&htable->cnt);
	}
}

int rbt_htable_destroy(struct rbt_htable *htable, void (*free)(void *table))
{
	int rc = E_HM_OK;

	/* bucket table cannot be freed if not empty */
	if (raw_atomic_long_read(&htable->cnt) > 0) {
		rc = E_HM_PERM;
	}
	if (rc == E_HM_OK) {
		free(htable->table);
		htable->table = NULL;
	}

	return rc;
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
uint32_t rbt_htable_hash_u32(const struct rbt_htable *htable, uint32_t key)
{
	return (key * GOLDEN_MEAN_32 * htable->entropy) >>
	       ((uint32_t)U32_BIT_CNT - (uint32_t)__builtin_ctz(htable->size));
}

uint32_t rbt_htable_hash_u64(const struct rbt_htable *htable, uint64_t key)
{
	return (uint32_t)((key * GOLDEN_MEAN_64 * htable->entropy) >>
			  ((uint32_t)U64_BIT_CNT - (uint32_t)__builtin_ctz(htable->size)));
}

uint32_t rbt_htable_hash_str(const struct rbt_htable *htable, const char *str)
{
	size_t i, len;
	uint32_t hash = 0;
	unsigned char uchar;

	len = strlen(str);
	for (i = 0; i < len; i++) {
		uchar = (unsigned char)str[i];
		hash = ((hash << HASH_CHAR_FACTOR) - hash) + (uint32_t)uchar;
	}

	return rbt_htable_hash_u32(htable, hash);
}
