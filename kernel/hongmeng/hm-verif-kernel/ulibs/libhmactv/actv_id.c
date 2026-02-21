/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Operations on activation id
 * Author: Huawei OS Kernel Lab
 * Create: Thi Nov 13 11:43:11 2019
 */
#include <libhmactv/actv_id.h>

#include <hongmeng/errno.h>
#include <hongmeng/panic.h>

#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/bitops/generic.h>
#include <libhwsecurec/securec.h>

struct actv_idpool {
	DEFINE_RAW_BITMAP(actvmap, ACTV_MAX_ID_COUNT);

	unsigned int last;
	struct raw_mutex mutex;
};

static struct actv_idpool idpool = {
	.actvmap[0 ... BITS_TO_LONGLONGS(ACTV_MAX_ID_COUNT) - 1UL] = 0,
	.mutex = RAW_MUTEX_INITIALIZER,
	.last = 0U,
};

struct resv_actv_idpool {
	DEFINE_RAW_BITMAP(actvmap, RESV_ACTV_MAX_ID_COUNT);	/* bitmap array */
	unsigned int last;					/* the last alloced bitmap id, this will be
								 * used as the start value when finding next
								 * bitmap id */
	struct raw_mutex mutex;					/* the mutex protects the whole idpool */
};

static struct resv_actv_idpool resv_idpools[RESV_ACTV_IDPOOL_MAX_NUM];
static bool resv_actv_idpool_inited = false;

static unsigned int
do_bitmap_id_alloc(unsigned long long *bitmap, struct raw_mutex *mutex,
		   unsigned int sz, unsigned int *start)
{
	int err;
	unsigned int rc;
	unsigned int bitmap_id;

	RAW_MUTEX_GUARD(_, mutex);
	bitmap_id = raw_bitmap_find_next_zero(bitmap, sz, *start);
	if (bitmap_id == sz) {
		bitmap_id = raw_bitmap_find_next_zero(bitmap, sz, 0);
	}

	if (bitmap_id == sz) {
		hm_warn("no enough bitmap id exists\n");
		rc = (unsigned int)-1;
	} else {
		err = raw_bitmap_set_bit(bitmap, sz, bitmap_id);
		BUG_ON(err != E_HM_OK);

		*start = bitmap_id;
		rc = bitmap_id;
	}
	return rc;
}

static void
do_bitmap_id_free(unsigned long long *bitmap, struct raw_mutex *mutex,
		  unsigned int sz, unsigned int bitmap_id)
{
	RAW_MUTEX_GUARD(_, mutex);
	if (raw_bitmap_clear_bit(bitmap, sz, bitmap_id) < 0) {
		hm_warn("free bitmap id failed, bitmap_id=%u\n", bitmap_id);
	}
}

unsigned int actv_id_alloc(void)
{
	unsigned int actv_id;
	unsigned int bitmap_id;

	bitmap_id = do_bitmap_id_alloc(idpool.actvmap, &idpool.mutex, ACTV_MAX_ID_COUNT,
				       &idpool.last);
	if (bitmap_id == (unsigned int)-1) {
		actv_id = bitmap_id;
	} else {
		actv_id = actv_bitmapid_to_id(bitmap_id);
	}

	return actv_id;
}

void actv_id_free(unsigned int actv_id)
{
	unsigned int bitmap_id = actv_id_to_bitmapid(actv_id);

	do_bitmap_id_free(idpool.actvmap, &idpool.mutex, ACTV_MAX_ID_COUNT, bitmap_id);
}

unsigned int actv_id_min(void)
{
	return actv_bitmapid_to_id(0U);
}

bool resv_actv_idpool_is_inited(void)
{
	return resv_actv_idpool_inited;
}

void resv_actv_idpool_init(void)
{
	unsigned int pool_id;

	mem_zero_a(resv_idpools);
	for (pool_id = 0U; pool_id < RESV_ACTV_IDPOOL_MAX_NUM; pool_id++) {
		raw_mutex_init(&resv_idpools[pool_id].mutex);
	}

	resv_actv_idpool_inited = true;
}

int actv_pure_id_alloc(unsigned int pool_id, unsigned int *actv_id)
{
	int err = E_HM_OK;
	unsigned int bitmap_id;

	if (pool_id >= RESV_ACTV_IDPOOL_MAX_NUM || actv_id == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		bitmap_id = do_bitmap_id_alloc(resv_idpools[pool_id].actvmap,
					       &resv_idpools[pool_id].mutex,
					       RESV_ACTV_MAX_ID_COUNT,
					       &resv_idpools[pool_id].last);
		if (bitmap_id == (unsigned int)-1) {
			hm_warn("no enough actv_pure id exists\n");
			err = E_HM_NOMEM;
		} else {
			*actv_id = resv_actv_bitmapid_to_id(pool_id, bitmap_id);
		}
	}

	return err;
}

int actv_pure_id_free(unsigned int actv_id)
{
	int err = E_HM_OK;
	unsigned int pool_id;
	unsigned int bitmap_id;

	if ((actv_id < RESV_ACTV_ID_OFFSET) ||
	    (actv_id > resv_actv_bitmapid_to_id(RESV_ACTV_IDPOOL_MAX_NUM - 1,
						RESV_ACTV_MAX_ID_COUNT - 1))) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		pool_id = resv_actv_id_to_poolid(actv_id);
		bitmap_id = resv_actv_id_to_bitmapid(actv_id);
		do_bitmap_id_free(resv_idpools[pool_id].actvmap,
				  &resv_idpools[pool_id].mutex,
				  RESV_ACTV_MAX_ID_COUNT, bitmap_id);
	}

	return err;
}
