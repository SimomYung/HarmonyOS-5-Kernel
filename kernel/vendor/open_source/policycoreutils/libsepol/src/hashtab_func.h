#ifndef _SEPOL_INTERNAL_HASHTAB_FUNC_H_
#define _SEPOL_INTERNAL_HASHTAB_FUNC_H_

#include "private.h"
#define AVRULEX_TABLE_SIZE (1 <<  10)

ignore_unsigned_overflow_
static inline unsigned long
partial_name_hash(unsigned long c, unsigned long prevhash)
{
	return (prevhash + (c << 4) + (c >> 4)) * 11;
}

static inline unsigned int role_trans_hash(hashtab_t h, const_hashtab_key_t key)
{
	const role_trans_t *k = (const role_trans_t *)key;
	return ((k->role + (k->type << 2) +
				(k->tclass << 5)) & (h->size - 1));
}

static inline int role_trans_compare(hashtab_t h
             __attribute__ ((unused)), const_hashtab_key_t key1,
			              const_hashtab_key_t key2)
{
	const role_trans_t *a = (const role_trans_t *)key1;
	const role_trans_t *b = (const role_trans_t *)key2;

	return a->role != b->role || a->type != b->type || a->tclass != b->tclass;
}

/* Based on MurmurHash3, written by Austin Appleby and placed in the
 * public domain.
 */
static inline unsigned int avrulex_hash(__attribute__((unused)) hashtab_t h, const_hashtab_key_t key)
{
	const avtab_key_t *k = (const avtab_key_t *)key;

	static const uint32_t c1 = 0xcc9e2d51;
	static const uint32_t c2 = 0x1b873593;
	static const uint32_t r1 = 15;
	static const uint32_t r2 = 13;
	static const uint32_t m  = 5;
	static const uint32_t n  = 0xe6546b64;

	uint32_t hash = 0;

#define mix(input) do { \
	uint32_t v = input; \
	v *= c1; \
	v = (v << r1) | (v >> (32 - r1)); \
	v *= c2; \
	hash ^= v; \
	hash = (hash << r2) | (hash >> (32 - r2)); \
	hash = hash * m + n; \
} while (0)

	mix(k->target_class);
	mix(k->target_type);
	mix(k->source_type);
	mix(k->specified);

#undef mix

	hash ^= hash >> 16;
	hash *= 0x85ebca6b;
	hash ^= hash >> 13;
	hash *= 0xc2b2ae35;
	hash ^= hash >> 16;

	return hash & (AVRULEX_TABLE_SIZE - 1);
}

static inline int avrulex_compare(hashtab_t h
             __attribute__ ((unused)), const_hashtab_key_t key1,
			              const_hashtab_key_t key2)
{
	const avtab_key_t *a = (const avtab_key_t *)key1;
	const avtab_key_t *b = (const avtab_key_t *)key2;

	return a->source_type != b->source_type || a->target_type != b->target_type || a->target_class != b->target_class || a->specified != b->specified;
}

static inline unsigned int cil_strpool_hash(hashtab_t h, const_hashtab_key_t key)
{
	const char *p;
	size_t size;
	unsigned int val;

	val = 0;
	size = strlen(key);
	for (p = key; ((size_t) (p - key)) < size; p++)
		val =
		    (val << 4 | (val >> (8 * sizeof(unsigned int) - 4))) ^ (*p);
	return val & (h->size - 1);
}

static inline int cil_strpool_compare(hashtab_t h __attribute__ ((unused)), const_hashtab_key_t key1, const_hashtab_key_t key2)
{
	return strcmp(key1, key2);
}

static inline unsigned int filenametr_hash(hashtab_t h, const_hashtab_key_t k)
{
	const filename_trans_key_t *ft = (const filename_trans_key_t *)k;
	unsigned long hash;
	unsigned int byte_num;
	unsigned char focus;

	hash = ft->ttype ^ ft->tclass;

	byte_num = 0;
	while ((focus = ft->name[byte_num++]))
		hash = partial_name_hash(focus, hash);
	return hash & (h->size - 1);
}

static inline int filenametr_cmp(hashtab_t h __attribute__ ((unused)),
		   const_hashtab_key_t k1, const_hashtab_key_t k2)
{
	const filename_trans_key_t *ft1 = (const filename_trans_key_t *)k1;
	const filename_trans_key_t *ft2 = (const filename_trans_key_t *)k2;
	int v;

	v = spaceship_cmp(ft1->ttype, ft2->ttype);
	if (v)
		return v;

	v = spaceship_cmp(ft1->tclass, ft2->tclass);
	if (v)
		return v;

	return strcmp(ft1->name, ft2->name);

}

static inline unsigned int rangetr_hash(hashtab_t h, const_hashtab_key_t k)
{
	const struct range_trans *key = (const struct range_trans *)k;
	return (key->source_type + (key->target_type << 3) +
		(key->target_class << 5)) & (h->size - 1);
}

static inline int rangetr_cmp(hashtab_t h __attribute__ ((unused)),
	       const_hashtab_key_t k1, const_hashtab_key_t k2)
{
	const struct range_trans *key1 = (const struct range_trans *)k1;
	const struct range_trans *key2 = (const struct range_trans *)k2;
	int v;

	v = spaceship_cmp(key1->source_type, key2->source_type);
	if (v)
		return v;

	v = spaceship_cmp(key1->target_type, key2->target_type);
	if (v)
		return v;

	v = spaceship_cmp(key1->target_class, key2->target_class);

	return v;
}

ignore_unsigned_overflow_
static inline unsigned int symhash(hashtab_t h, const_hashtab_key_t key)
{
	const char *p, *keyp;
	size_t size;
	unsigned int val;

	val = 0;
	keyp = (const char *)key;
	size = strlen(keyp);
	for (p = keyp; ((size_t) (p - keyp)) < size; p++)
		val =
		    (val << 4 | (val >> (8 * sizeof(unsigned int) - 4))) ^ (*p);
	return val & (h->size - 1);
}

static inline int symcmp(hashtab_t h __attribute__ ((unused)), const_hashtab_key_t key1,
		  const_hashtab_key_t key2)
{
	return strcmp(key1, key2);
}

static inline unsigned int hashtab_hash_value(hashtab_t h, const_hashtab_key_t key, hashtab_type type_id)
{
	switch(type_id) {
	case HASHTAB_SYM:
		return symhash(h, key);
	case HASHTAB_CIL_STRPOOL:
		return cil_strpool_hash(h, key);
	case HASHTAB_FILENAMETR:
		return filenametr_hash(h, key);
	case HASHTAB_RANGETR:
		return rangetr_hash(h, key);
	case HASHTAB_ROLE_TRANS:
		return role_trans_hash(h, key);
	case HASHTAB_AVRULEX:
		return avrulex_hash(h, key);
	default:
		break;
	}
	return -1;
}

static inline int hashtab_keycmp(hashtab_t h, const_hashtab_key_t key1, const_hashtab_key_t key2, hashtab_type type_id)
{
	switch(type_id) {
	case HASHTAB_SYM:
		return symcmp(h, key1, key2);
	case HASHTAB_CIL_STRPOOL:
		return cil_strpool_compare(h, key1, key2);
	case HASHTAB_FILENAMETR:
		return filenametr_cmp(h, key1, key2);
	case HASHTAB_RANGETR:
		return rangetr_cmp(h, key1, key2);
	case HASHTAB_ROLE_TRANS:
		return role_trans_compare(h, key1, key2);
	case HASHTAB_AVRULEX:
		return avrulex_compare(h, key1, key2);
	default:
		break;
	}
	return 1;
}

#endif
