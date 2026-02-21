/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Index generator implementation and management
 * Author: Huawei OS Kernel Lab
 * Create: Sat Apr 06 15:15:20 2019
 */
#include <libalgo/id_generator.h>

#include <malloc.h>
#include <stdbool.h>
#include <libmem/utils.h>
#include <libhmlog/hmlog.h>
#include <libstrict/overflow.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/bitops/generic.h>
#include <libhmsync/raw_mutex_guard.h>

struct id_generator {
	struct id_generator_attr attr;
	unsigned long long *id_bit_map;
	struct raw_mutex mutex;
	size_t id_map_sz;
	unsigned int cache_id_off;
};

static int init_new_generator(const struct id_generator_attr *attr,
			      struct id_generator *generator,
			      unsigned long long *id_bit_map,
			      size_t id_map_sz)
{
	size_t generator_size = 0UL;
	unsigned int generator_count;
	unsigned int normal_count;
	int err = E_HM_OK;

	/* set zero for generator */
	BUG_ON(generator == NULL);
	mem_zero_s(*generator);
	raw_mutex_init(&generator->mutex);
	generator->attr.min_index = attr->min_index;
	generator->attr.max_index = attr->max_index;
	generator->cache_id_off = 0;

	generator_count = (unsigned int)BITS_TO_LONGLONGS(((size_t)attr->max_index - (size_t)attr->min_index) + 1UL);
	normal_count = (unsigned int)sizeof(unsigned long long);
	noovfl_umul(generator_size, generator_count, normal_count, "generator_sz overflow");
	if ((generator_size == 0UL) || (generator_size > id_map_sz)) {
		err = E_HM_INVAL;
	} else {
		/* clean bit map */
		generator->id_bit_map = id_bit_map;
		generator->id_map_sz = id_map_sz;
		mem_zero_b(generator->id_bit_map, id_map_sz);
	}

	return err;
}

int id_generator_create(const struct id_generator_attr *attr, struct id_generator **id_generator)
{
	int err = E_HM_OK;
	size_t id_map_sz = 0;
	unsigned int id_count;
	unsigned int normal_count;
	unsigned long long *id_bit_map = NULL;
	struct id_generator *generator = NULL;

	/* args check */
	if ((attr == NULL) || (id_generator == NULL) || (attr->min_index > attr->max_index)) {
		hm_debug("Id generator: invalid parameters, attr ptr = 0x%lx, id_generator ptr = 0x%lx\n",
			 ptr_to_ulong(attr), ptr_to_ulong(id_generator));
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		/* here we think min index and max index are all valid */
		id_count = (unsigned int)BITS_TO_LONGLONGS(((size_t)attr->max_index - (size_t)attr->min_index) + 1UL);
		normal_count = (unsigned int)sizeof(unsigned long long);
		noovfl_umul(id_map_sz, id_count, normal_count, "id_map_sz overflow");
		if (id_map_sz == (size_t)0) {
			err = E_HM_INVAL;
		}
	}
	if (err == E_HM_OK) {
		/* apply id generator memory */
		generator = (struct id_generator *)struct_id_generator_malloc();
		if (generator == NULL) {
			err = E_HM_NOMEM;
		}
	}
	if (err == E_HM_OK) {
		/* apply id bit map memory */
		id_bit_map = (unsigned long long *)malloc(id_map_sz);
		if (id_bit_map == NULL) {
			struct_id_generator_free(generator);
			err = E_HM_NOMEM;
		}
	}
	if (err == E_HM_OK) {
		/* init new id generator */
		err = init_new_generator(attr, generator, id_bit_map, id_map_sz);
		if (err != E_HM_OK) {
			free(id_bit_map);
			struct_id_generator_free(generator);
		} else {
			*id_generator = generator;
		}
	}
	return err;
}

int id_generator_alloc(struct id_generator *id_generator,
		       unsigned int *id)
{
	int err = E_HM_OK;
	unsigned int id_off, id_count, min_index, max_index;

	if ((id_generator == NULL) || (id == NULL)) {
		hm_debug("Id generator: id_generator ptr = 0x%lx, id ptr = 0x%lx\n",
			 ptr_to_ulong(id_generator), ptr_to_ulong(id));
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		BUG_ON(id_generator->id_bit_map == NULL);

		min_index = id_generator->attr.min_index;
		max_index = id_generator->attr.max_index;
		id_count = (max_index - min_index) + 1U;
		RAW_MUTEX_GUARD(_, &id_generator->mutex);
		/* fast alloc */
		id_off = raw_bitmap_find_next_zero(id_generator->id_bit_map,
						   id_count,
						   id_generator->cache_id_off);
		if (id_off == id_count) {
			/* try alloc again */
			id_off = raw_bitmap_find_first_zero(id_generator->id_bit_map,
							    id_count);
			if (id_off == id_count) {
				err = E_HM_NOSPC;
			}
		}
		if (err == E_HM_OK) {
			err = raw_bitmap_set_bit(id_generator->id_bit_map,
						 id_count,
						 id_off);
		}
		if (err == E_HM_OK) {
			/* update cache index */
			id_generator->cache_id_off = id_off;
			*id = min_index + id_off;
		}
	}
	return err;
}

static inline bool id_fast_check_valid(const struct id_generator *id_generator,
				       unsigned int id)
{
	return ((id >= id_generator->attr.min_index) &&
		(id <= id_generator->attr.max_index));
}

void id_generator_free(struct id_generator *id_generator,
		       unsigned int id)
{
	int err;
	bool valid_id = false;
	unsigned int min_index, max_index, id_count;

	if (id_generator != NULL) {
		/* fase check */
		valid_id = id_fast_check_valid(id_generator, id);
		if (valid_id) {
			BUG_ON(id_generator->id_bit_map == NULL);

			min_index = id_generator->attr.min_index;
			max_index = id_generator->attr.max_index;
			id_count = (max_index - min_index) + 1U;
			RAW_MUTEX_GUARD(_, &id_generator->mutex);
			err = raw_bitmap_clear_bit(id_generator->id_bit_map,
						   id_count,
						   id - min_index);
			if (err != 0) {
				hm_error("Id_generator free id failed = %s\n", hmstrerror(err));
			}
		}
	}
}

void id_generator_destroy(struct id_generator *id_generator)
{
	if (id_generator != NULL) {
		/* free id bit map */
		free(id_generator->id_bit_map);
		/* free id generator */
		struct_id_generator_free(id_generator);
	}
}

size_t struct_id_generator_size(void)
{
	return sizeof(struct id_generator);
}
