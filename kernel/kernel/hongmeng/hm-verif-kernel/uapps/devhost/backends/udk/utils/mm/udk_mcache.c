/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Mcache for UDK
 * Author: Huawei OS Kernel Lab
 * Create: Thur Dec 14 19:34:05
 */

#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>

#include <udk/delay.h>
#include <udk/iolib.h>
#include <udk/log.h>
#include <udk/mcache.h>
#include <udk/mutex.h>

#define UDK_MCACHE_NAME_SIZE	16U
struct udk_mcache_s {
	unsigned long long *bitmap;
	unsigned int num;
	unsigned int obj_size;
	unsigned int busy_cnt;
	char *pool;
	char name[UDK_MCACHE_NAME_SIZE];
	struct udk_mutex lock;
};

struct udk_mcache_s *udk_mcache_create(char *name, unsigned int num, unsigned int obj_size)
{
	struct udk_mcache_s *mcache;
	int ret;

	if (name == NULL) {
		return NULL;
	}

	mcache = (struct udk_mcache_s *)udk_malloc(sizeof(struct udk_mcache_s));
	if (mcache == NULL) {
		return NULL;
	}

	mem_zero_s(*mcache);
	mcache->num = num;
	mcache->obj_size = obj_size;
	udk_mutex_init(&mcache->lock);

	ret = strncpy_s(mcache->name, UDK_MCACHE_NAME_SIZE, name, UDK_MCACHE_NAME_SIZE - 1U);
	if (ret != 0) {
		goto err;
	}

	mcache->pool = (char *)udk_malloc(num * obj_size);
	if (mcache->pool == NULL) {
		goto err;
	}
	mcache->bitmap = (unsigned long long *)udk_malloc(
			BITS_TO_LONGLONGS(num) * sizeof(unsigned long long));
	if (mcache->bitmap == NULL) {
		goto err;
	}
	mem_zero_b(mcache->bitmap, BITS_TO_LONGLONGS(num) * sizeof(unsigned long long));

	return mcache;
err:
	udk_mcache_destroy(mcache);
	return NULL;
}

void udk_mcache_destroy(struct udk_mcache_s *mcache)
{
	BUG_ON(mcache == NULL);

	if (mcache->pool != NULL) {
		udk_free(mcache->pool);
	}
	if (mcache->bitmap != NULL) {
		udk_free(mcache->bitmap);
	}
	mcache->pool = NULL;
	mcache->bitmap = NULL;
	udk_free(mcache);
}

void *udk_mcache_alloc(struct udk_mcache_s *mcache)
{
	unsigned int pos;
	int print = 0;

	BUG_ON(mcache == NULL);
#define UDK_MCACHE_DELAY	5UL
#define UDK_MCACHE_BUSY_LIMIT	1000U
retry:
	udk_mutex_lock(&mcache->lock);
	pos = raw_bitmap_find_first_zero(mcache->bitmap, mcache->num);
	if (pos >= mcache->num) {
		mcache->busy_cnt++;
		if (mcache->busy_cnt > UDK_MCACHE_NAME_SIZE) {
			mcache->busy_cnt = 0U;
			print = 1;
		}
		udk_mutex_unlock(&mcache->lock);
		udk_mdelay(UDK_MCACHE_DELAY);
		if (print != 0) {
			udk_info("mcache alloc busy, name=%s size=%u\n",
				 mcache->name, mcache->obj_size);
			print = 0;
		}
		goto retry;
	}
	NOFAIL(raw_bitmap_set_bit(mcache->bitmap, mcache->num, pos) != 0);
	udk_mutex_unlock(&mcache->lock);

	return (void *)(mcache->pool + (pos * mcache->obj_size));
}

void udk_mcache_free(struct udk_mcache_s *mcache, void *va)
{
	unsigned int pos;

	BUG_ON(mcache == NULL || va == NULL);
	pos = (ptr_to_ulong(va) - ptr_to_ulong(mcache->pool)) / mcache->obj_size;

	udk_mutex_lock(&mcache->lock);
	BUG_ON(raw_bitmap_test_bit(mcache->bitmap, mcache->num, pos) == 0);
	NOFAIL(raw_bitmap_clear_bit(mcache->bitmap, mcache->num, pos) != 0);
	udk_mutex_unlock(&mcache->lock);
}

int udk_mcache_foreach(struct udk_mcache_s *mcache, udk_mcache_foreach_func f)
{
	int ret = 0;

	BUG_ON(mcache == NULL || f == NULL);
	for(unsigned int i = 0; i < mcache->num; ++i) {
		ret = f((void *)(mcache->pool + mcache->obj_size * i));
		if (ret < 0) {
			return ret;
		}
	}

	return ret;
}
