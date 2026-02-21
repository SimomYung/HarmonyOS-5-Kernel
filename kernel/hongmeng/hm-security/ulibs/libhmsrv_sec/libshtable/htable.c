/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Shared hash table functions
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 10:49:50 2019
 */
#include "htable.h"

#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex.h>
#include <libhwsecurec/securec.h>

#include "hash.h"

static inline sht_idx_t sht_key2bucket(const struct shtable *sht,
				       const void *key)
{
	return (sht_idx_t)(sht_hash(sht, key, sht->key_desc.size) % (uint32_t)(sht->size));
}

static inline struct sht_bucket_s *sht_bucket(const struct shtable *sht,
					    sht_idx_t bucket)
{
	return &((struct sht_bucket_s *)sht->bucket_shm.base)[bucket];
}

static inline struct sht_node_s *sht_node(const struct shtable *sht,
					sht_idx_t node)
{
	return &((struct sht_node_s *)sht->node_shm.base)[node];
}

static inline void *sht_key(const struct shtable *sht, sht_idx_t node)
{
	return ptr_to_void(sht->key_shm.base + sht->key_desc.size * node);
}

static inline void sht_htable_lock(const struct shtable *sht)
{
	raw_mutex_lock(&sht->ctrl->htable_lock);
}

static inline void sht_htable_unlock(const struct shtable *sht)
{
	raw_mutex_unlock(&sht->ctrl->htable_lock);
}

static inline void sht_node_free_list_lock(const struct shtable *sht)
{
	raw_mutex_lock(&sht->ctrl->free_list_lock);
}

static inline void sht_node_free_list_unlock(const struct shtable *sht)
{
	raw_mutex_unlock(&sht->ctrl->free_list_lock);
}

static inline struct sht_list_node *sht_node_free_list_head(
	const struct shtable *sht)
{
	return &sht->ctrl->free_list;
}

static inline struct sht_list_node *sht_node_ref_node(
	const struct shtable *sht, sht_idx_t node)
{
	return &sht_node(sht, node)->node;
}

static inline struct sht_list_node *sht_node_free_list_node(
	const struct shtable *sht, sht_idx_t node)
{
	struct sht_list_node *ret = NULL;
	/* the node is the head of the free list */
	if (node == SHT_INVALID) {
		ret = sht_node_free_list_head(sht);
	} else {
		ret = sht_node_ref_node(sht, node);
	}
	return ret;
}

static inline void sht_node_free_list_add(const struct shtable *sht,
					  sht_idx_t node)
{
	struct sht_list_node *head = sht_node_free_list_head(sht);
	struct sht_list_node *new  = sht_node_ref_node(sht, node);
	struct sht_list_node *next = sht_node_free_list_node(sht, head->next);

	new->next = head->next;
	new->prev = SHT_INVALID; /* the head of the free list */
	head->next = node;
	next->prev = node;

	(sht->ctrl->free_num)++;
}

static inline void sht_node_free_list_del(const struct shtable *sht,
					  sht_idx_t node)
{
	struct sht_list_node *cur  = sht_node_ref_node(sht, node);
	struct sht_list_node *next = sht_node_free_list_node(sht, cur->next);
	struct sht_list_node *prev = sht_node_free_list_node(sht, cur->prev);

	prev->next = cur->next;
	next->prev = cur->prev;

	(sht->ctrl->free_num)--;
}

static inline sht_idx_t sht_node_alloc(const struct shtable *sht)
{
	sht_idx_t node;

	sht_node_free_list_lock(sht);

	node = sht_node_free_list_head(sht)->next;
	if (node != SHT_INVALID) {
		sht_node_free_list_del(sht, node);
	}
	sht_node_free_list_unlock(sht);

	return node;
}

static inline void sht_node_free(const struct shtable *sht, sht_idx_t node)
{
	sht_node_free_list_lock(sht);

	sht_node_free_list_add(sht, node);

	sht_node_free_list_unlock(sht);
}

void sht_htable_init(const struct shtable *sht)
{
	struct sht_list_node *head = sht_node_free_list_head(sht);

	raw_mutex_init(&sht->ctrl->htable_lock);
	raw_mutex_init(&sht->ctrl->free_list_lock);

	sht->ctrl->free_num = 0;

	head->next = SHT_INVALID;
	head->prev = SHT_INVALID;

	/*
	 * Loop termination condition:
	 * Following loop terminate when i reach sht->size, Since i keep increasing
	 * in loop body, it has a upper bound.
	 * Loop condition is constant and reasonable:
	 * sht->size is cfg->size, which is limited, so the loop condition is reasonable.
	 */
	for (sht_uint_t i = 0; i < sht->size; i++) {
		sht_bucket(sht, i)->node = SHT_INVALID;
		sht->key_desc.clear(sht_key(sht, i));
		sht_node(sht, i)->node_seq = 0;
		sht_node(sht, i)->ctx_seq = 0;
		sht_node_free_list_add(sht, i);
	}
}

void sht_htable_fini(const struct shtable *sht)
{
	raw_mutex_destroy(&sht->ctrl->htable_lock);
	raw_mutex_destroy(&sht->ctrl->free_list_lock);
}

static int sht_bucket_node_seq(const struct shtable *sht, sht_idx_t bucket, sht_idx_t *p_node, seq_t *p_seq)
{
	int rc = E_HM_POSIX_NOENT;
	sht_idx_t node, new_node;
	seq_t seq;

	node = sht_idx_read(&sht_bucket(sht, bucket)->node);

	/*
	 * Loop termination condition:
	 * Following loop terminate when node is SHT_INVALID, Since node keep changing
	 * in loop body, it has a bound.
	 * Loop condition is constant and reasonable:
	 * the bucket is limited, so the loop condition is reasonable.
	 */
	while (node != SHT_INVALID) {
		seq = seq_read(&sht_node(sht, node)->node_seq);
		/* The node is being deleted. */
		if (!seq_is_readable(seq)) {
			cpu_relax();
			continue;
		}
		__smp_rmb();
		new_node = sht_idx_read(&sht_bucket(sht, bucket)->node);
		/* We sure that the node is in the bucket. We could find the
		 * node which was deleted and inserted again, but it's ok.
		 * There are two cases:
		 * 1. We get the new sequence number. It's what we want.
		 * 2. We get the old sequence number. It's equivalent to that
		 *    we get the node and then the node is deleted. The user of
		 *    the node could check whether the sequence is expired. */
		if (new_node == node) {
			*p_node = node;
			*p_seq  = seq;
			rc = E_HM_OK;
			break;
		}
		/* The node was changed. */
		node = new_node;
	}

	return rc;
}

static int sht_bucket_next_seq(const struct shtable *sht, sht_idx_t node,
			       sht_idx_t *p_next, seq_t *p_seq)
{
	sht_idx_t next, new_next;
	seq_t seq;
	int rc = E_HM_POSIX_NOENT;

	next = sht_idx_read(&sht_node(sht, node)->node.next);

	/*
	 * Loop termination condition:
	 * Following loop terminate when node is SHT_INVALID, Since node keep changing
	 * in loop body, it has a bound.
	 * Loop condition is constant and reasonable:
	 * the bucket is limited, so the loop condition is reasonable.
	 */
	while (next != SHT_INVALID) {
		seq = seq_read(&sht_node(sht, next)->node_seq);
		/* The next node is being deleted. */
		if (!seq_is_readable(seq)) {
			cpu_relax();
			continue;
		}
		__smp_rmb();
		new_next = sht_idx_read(&sht_node(sht, node)->node.next);
		/* We sure that the next node is in the bucket. */
		if (new_next == next) {
			*p_next = next;
			*p_seq  = seq;
			rc = E_HM_OK;
			break;
		}
		/* The next node was changed. */
		next = new_next;
	}

	return rc;
}

static int sht_bucket_search(const struct shtable *sht, sht_idx_t bucket,
			     const void *key, sht_idx_t *p_node,
			     seq_t *p_seq)
{
	sht_idx_t node;
	sht_idx_t next = {0};
	seq_t node_seq;
	seq_t next_seq = {0};
	bool found = false;
	int ret, cmp;

	ret = sht_bucket_node_seq(sht, bucket, &next, &next_seq);

	/*
	 * Loop termination condition:
	 * Following loop terminate when found is true or ret is not E_HM_OK, Since node keep changing
	 * in loop body, it has a bound.
	 * Loop condition is constant and reasonable:
	 * the bucket is limited, so the loop condition is reasonable.
	 */
	while (!found && ret == E_HM_OK) {
		node = next;
		node_seq = next_seq;

		cmp = sht->key_desc.cmp(sht_key(sht, node), key);
		if (cmp == 0) {
			found = true;
			ret = E_HM_OK;
		} else if (cmp > 0) {
			ret = E_HM_POSIX_NOENT;
		} else {
			ret = sht_bucket_next_seq(sht, node, &next, &next_seq);
		}
		if (seq_need_retry(&sht_node(sht, node)->node_seq,
				   node_seq)) {
			ret = E_HM_AGAIN;
			break;
		}
	}

	if (found) {
		*p_node = node;
		*p_seq  = node_seq;
	}

	return ret;
}

static int sht_bucket_add(const struct shtable *sht, sht_idx_t bucket,
			  sht_idx_t node, const void *key, size_t key_size)
{
	sht_idx_t prev = SHT_INVALID;
	sht_idx_t next;
	int cmp;
	int rc = E_HM_OK;

	sht_htable_lock(sht);

	/*
	 * Loop termination condition:
	 * Following loop terminate when next is SHT_INVALID, Since next keep increasing
	 * in loop body, it has a upper bound.
	 * Loop condition is constant and reasonable:
	 * the sht is limited, so the loop condition is reasonable.
	 */
	next = sht_bucket(sht, bucket)->node;
	while (next != SHT_INVALID) {
		cmp = sht->key_desc.cmp(sht_key(sht, next), key);
		if (cmp == 0) {
			rc = E_HM_POSIX_EXIST;
			break;
		}
		if (cmp > 0) {
			break;
		}
		prev = next;
		next = sht_node(sht, next)->node.next;
	}

	if (rc == E_HM_OK) {
		if (memcpy_s(sht_key(sht, node), sht->key_desc.size,
			     key, key_size) != EOK) {
			hm_panic("memcpy_s failed\n");
		}
		sht_node(sht, node)->node.next = next;
		sht_node(sht, node)->node.prev = prev;

		__smp_wmb();

		if (prev != SHT_INVALID) {
			sht_node(sht, prev)->node.next = node;
		} else {
			sht_bucket(sht, bucket)->node = node;
		}

		if (next != SHT_INVALID) {
			sht_node(sht, next)->node.prev = node;
		}
	}

	sht_htable_unlock(sht);
	return rc;
}

static int sht_bucket_del(const struct shtable *sht, sht_idx_t bucket,
			  sht_idx_t node, seq_t node_seq)
{
	int rc = E_HM_OK;
	sht_idx_t next, prev;

	sht_htable_lock(sht);

	if (seq_need_retry_raw(&sht_node(sht, node)->node_seq, node_seq)) {
		rc = E_HM_AGAIN;
	}

	if (rc == E_HM_OK) {
		/* Wait for the end of update */
		seq_write_start(&sht_node(sht, node)->ctx_seq);

		sht_node(sht, node)->node_seq++;
		__smp_wmb();

		next = sht_node(sht, node)->node.next;
		prev = sht_node(sht, node)->node.prev;
		if (prev != SHT_INVALID) {
			sht_node(sht, prev)->node.next = next;
		} else {
			sht_bucket(sht, bucket)->node = next;
		}

		if (next != SHT_INVALID) {
			sht_node(sht, next)->node.prev = prev;
		}

		__smp_wmb();
		sht->key_desc.clear(sht_key(sht, node));

		__smp_wmb();
		sht_node(sht, node)->node_seq++;

		seq_write_end(&sht_node(sht, node)->ctx_seq);
	}

	sht_htable_unlock(sht);
	return rc;
}

static inline void sht_node_init(sht_idx_t node, const struct sht_call_back *cb)
{
	int res;

	/* No lock. The node is not mounted. */
	res = cb->func(node, cb->args);

	if (cb->result != NULL) {
		*cb->result = res;
	}

	return;
}

static int sht_node_update(const struct shtable *sht, sht_idx_t node,
			   seq_t seq, const struct sht_call_back *cb)
{
	int ret, res;

	seq_write_start(&sht_node(sht, node)->ctx_seq);

	/* The node was deleted or is being deleted. */
	if (seq_need_retry_raw(&sht_node(sht, node)->node_seq, seq)) {
		ret = E_HM_AGAIN;
	} else {
		res = cb->func(node, cb->args);
		if (cb->result != NULL) {
			*cb->result = res;
		}
		ret = E_HM_OK;
	}

	seq_write_end(&sht_node(sht, node)->ctx_seq);

	return ret;
}

static inline int sht_write_check(const struct shtable *sht, const void *key)
{
	int rc = E_HM_OK;
	if (sht == NULL || sht->ctrl == NULL || key == NULL) {
		rc = E_HM_INVAL;
	}
	return rc;
}

static inline int sht_readonly_check(const struct shtable *sht, const void *key)
{
	int rc = E_HM_OK;
	if (sht == NULL || key == NULL) {
		rc = E_HM_INVAL;
	}
	return rc;
}

static inline int sht_cb_check(const struct sht_call_back *cb)
{
	int rc = E_HM_OK;
	/* We don't check whether args is NULL. Some call back functions
	 * don't need arguments, so we could pass NULL and some call back
	 * functions which use arguments can handle NULL. */
	if (cb == NULL || cb->func == NULL) {
		rc = E_HM_INVAL;
	}
	return rc;
}

int sht_insert_raw(const struct shtable *sht, const void *key,
		   const struct sht_call_back *cb)
{
	sht_idx_t bucket, node;
	int ret;

	ret = sht_write_check(sht, key);
	if (ret == E_HM_OK) {
		ret = sht_cb_check(cb);
		if (ret != E_HM_OK) {
			hm_error("failed to pass the call back check of shared hash table, err=%s\n", hmstrerror(ret));
		}
	} else {
		hm_error("failed to pass the write check of shared hash table, err=%s\n", hmstrerror(ret));
	}

	if (ret == E_HM_OK) {
		node = sht_node_alloc(sht);
		if (node == SHT_INVALID) {
			ret = E_HM_NOMEM;
			hm_error("failed to allocate shared hash table node, err=%s\n", hmstrerror(ret));
		}
	}

	if (ret == E_HM_OK) {
		sht_node_init(node, cb);
		bucket = sht_key2bucket(sht, key);
		ret = sht_bucket_add(sht, bucket, node, key, sht->key_desc.size);
		if (ret != E_HM_OK) {
			sht_node_free(sht, node);
		}
	}

	return ret;
}

int sht_delete(const struct shtable *sht, const void *key)
{
	sht_idx_t bucket, node;
	seq_t node_seq;
	int ret;

	ret = sht_write_check(sht, key);
	if (ret == E_HM_OK) {
		bucket = sht_key2bucket(sht, key);
		/*
		 * Loop termination condition:
		 * Following loop terminate when ret is not E_HM_AGAIN, Since ret keep changing
		 * in loop body, it has a bound.
		 * Loop condition is constant and reasonable:
		 * the bucket is limited, so the loop condition is reasonable.
		 */
		do {
			ret = sht_bucket_search(sht, bucket, key,
						&node, &node_seq);
			if (ret != E_HM_OK) {
				continue;
			}

			ret = sht_bucket_del(sht, bucket, node, node_seq);
			if (ret != E_HM_OK) {
				continue;
			}

			sht_node_free(sht, node);
		} while (ret == E_HM_AGAIN);
	} else {
		hm_error("failed to pass write check of shared hash table,  err=%s\n", hmstrerror(ret));
	}

	return ret;
}

int sht_update_raw(const struct shtable *sht, const void *key,
		   const struct sht_call_back *cb)
{
	sht_idx_t bucket, node;
	seq_t node_seq;
	int ret;

	ret = sht_write_check(sht, key);
	if (ret == E_HM_OK) {
		ret = sht_cb_check(cb);
		if (ret != E_HM_OK) {
			hm_error("failed to pass call back check, err=%s\n", hmstrerror(ret));
		}
	} else {
		hm_error("failed to pass sht write check, err=%s\n", hmstrerror(ret));
	}

	if (ret == E_HM_OK) {
		bucket = sht_key2bucket(sht, key);
		/*
		 * Loop termination condition:
		 * Following loop terminate when ret is not E_HM_AGAIN, Since ret keep changing
		 * in loop body, it has a bound.
		 * Loop condition is constant and reasonable:
		 * the bucket is limited, so the loop condition is reasonable.
		 */
		do {
			ret = sht_bucket_search(sht, bucket, key,
						&node, &node_seq);
			if (ret != E_HM_OK) {
				continue;
			}
			ret = sht_node_update(sht, node, node_seq, cb);
		} while (ret == E_HM_AGAIN);
	}

	return ret;
}

int sht_search(const struct shtable *sht, const void *key, sht_idx_t *p_node,
	       seq_t *p_seq)
{
	sht_idx_t bucket, node;
	seq_t node_seq;
	int ret;

	ret = sht_readonly_check(sht, key);
	if (ret != E_HM_OK) {
		hm_error("failed to pass read-only check, err=%s\n", hmstrerror(ret));
	}

	if (ret == E_HM_OK) {
		bucket = sht_key2bucket(sht, key);

		/*
		 * Loop termination condition:
		 * Following loop terminate when ret is not E_HM_AGAIN, Since ret keep changing
		 * in loop body, it has a bound.
		 * Loop condition is constant and reasonable:
		 * the bucket is limited, so the loop condition is reasonable.
		 */
		do {
			ret = sht_bucket_search(sht, bucket, key, &node, &node_seq);
		} while (ret == E_HM_AGAIN);
	}

	if (ret == E_HM_OK) {
		*p_node = node;
		*p_seq  = node_seq;
	}

	return ret;
}

int sht_read_raw(const struct shtable *sht, sht_idx_t node,
		 const struct sht_call_back *cb)
{
	seq_t ctx_seq;
	int res;
	int rc = E_HM_OK;

	if (sht == NULL) {
		rc = E_HM_INVAL;
		hm_error("sht is NULL, err=%s\n", hmstrerror(rc));
	}

	if (rc == E_HM_OK) {
		rc = sht_cb_check(cb);
		if (rc != E_HM_OK) {
			hm_error("failed to pass call back check, err=%s\n", hmstrerror(rc));
		}
	}

	if (rc == E_HM_OK) {
		/*
		 * Loop termination condition:
		 * Following loop terminate when seq_need_retry return false,
		 * since ctx_seq keep changing in loop body, it has a bound.
		 * Loop condition is constant and reasonable:
		 * the ctx_seq is seq_try in loop body,
		 * seq_need_retry will return false after seq_retry,
		 * so the loop condition is reasonable.
		 */
		do {
			ctx_seq = seq_try(&sht_node(sht, node)->ctx_seq);
			res = cb->func(node, cb->args);
		} while (seq_need_retry(&sht_node(sht, node)->ctx_seq, ctx_seq));

		if (cb->result != NULL) {
			*cb->result = res;
		}
	}

	return rc;
}

int sht_retry(const struct shtable *sht, sht_idx_t node, seq_t seq)
{
	int rc = E_HM_OK;

	if (sht == NULL) {
		rc = E_HM_INVAL;
		hm_debug("parameters are invalid, err=%s\n", hmstrerror(rc));
	}
	if (rc == E_HM_OK) {
		if (seq_need_retry(&sht_node(sht, node)->node_seq, seq)) {
			rc = E_HM_AGAIN;
		}
	}
	return rc;
}
