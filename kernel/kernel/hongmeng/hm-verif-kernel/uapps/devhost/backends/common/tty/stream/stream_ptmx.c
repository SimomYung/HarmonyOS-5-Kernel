/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Stream ptmx init functions
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */

#include "stream.h"
#include "stream_core.h"
#include "../tty_api.h"

int stream_ptmx_init(struct stream_control_device *ptmx,
		     struct tty_chrdrv *chrdrv,
		     const struct tty_backend_operations *bops)
{
	struct tty_chrdev *chrdev = NULL;
	int ret;

	if (ptmx == NULL || chrdrv == NULL || bops == NULL) {
		return -EINVAL;
	}

	/* create chrdev ptmx in stream_control_device */
	chrdev = tty_mem_zalloc(sizeof(struct tty_chrdev));
	if (chrdev == NULL) {
		return -ENOMEM;
	}
	/* currently we set ptmx info and create it */
	ret = snprintf_s(chrdev->name, CHRDEV_MAX_NAME_LEN, CHRDEV_MAX_NAME_LEN - 1,
			 "%s", chrdrv->dev_base_name);
	if (ret < 0) {
		tty_mem_free(chrdev);
		return ret;
	}
	chrdev->dev_no = MKDEV(chrdrv->major, chrdrv->minor_start);

	ret = tty_chrdev_create(chrdev, chrdrv, MAX_DEVICE_COUNT);
	if (ret < 0) {
		tty_warn("tty Stream: tty_chrdev_create failed, err: %s\n", strerror(-ret));
		tty_mem_free(chrdev);
		return ret;
	}

	/* init struct stream_control_device ptmx */
	ptmx->tty_bops = bops;
	tty_mutex_init(&ptmx->mutex);
	ptmx->max_pair = MAX_DEVICE_COUNT;
	ptmx->chrdev = chrdev;
	idr_init(&ptmx->pair_map);

	return 0;
}

void stream_ptmx_release(struct stream_control_device *ptmx)
{
	if (ptmx == NULL || ptmx->chrdev == NULL) {
		return;
	}
	ptmx->tty_bops = NULL;
	tty_chrdev_destroy(ptmx->chrdev);
	tty_mem_free(ptmx->chrdev);
	ptmx->chrdev = NULL;
	/* When driver removed is supported, release all the pairs in ptmx */
}

static struct stream_device_pair *
stream_pop_first_free_pair(struct stream_control_device *ptmx)
{
	unsigned int pair_id = 0;
	void *pair = NULL;

	/* Try to Find first released pair, set it as used, return pair */
	idr_search_forward(&ptmx->pair_map, pair, pair_id) {
		struct stream_device_pair *p = pair;
		/* Find one free tty and try recycled it */
		if (p->tty != NULL && tty_struct_try_recycle(p->tty)) {
			return p;
		}
	}
	return (struct stream_device_pair *)pair;
}

/* insert pair to idr and return idr id */
static int stream_insert_pair(struct stream_device_pair *pair)
{
	int ret;
	struct stream_control_device *ptmx = NULL;
	ptmx = pair->ptmx;
	ret = idr_alloc(&ptmx->pair_map, (void *)pair);
	if (ret < 0) {
		tty_warn("tty: idr alloc failed, err: %s\n", strerror(-ret));
		return ret;
	}
	return ret;
}

static void stream_delete_pair(struct stream_device_pair *pair)
{
	struct stream_control_device *ptmx = NULL;
	ptmx = pair->ptmx;
	(void)idr_remove(&ptmx->pair_map, pair->slave_id);
}

static struct stream_device_pair *
stream_device_pair_alloc(struct stream_control_device *ptmx)
{
	struct stream_device_pair *pair = NULL;
	int ret;

	/* alloc for stream_device_pair and init it */
	pair = (struct stream_device_pair *)
	       tty_mem_zalloc(sizeof(struct stream_device_pair));
	if (pair == NULL) {
		tty_warn("tty: Stream: tty_mem_zalloc return NULL\n");
		return NULL;
	}
	pair->ptmx = ptmx;
	tty_poll_wq_init(&pair->wq);

	(void)tty_mutex_lock(&ptmx->mutex);
	ret = stream_insert_pair(pair);
	if (ret < 0) {
		tty_warn("tty: Stream: insert pair failed, err: %s\n", strerror(-ret));
		tty_mem_free(pair);
		(void)tty_mutex_unlock(&ptmx->mutex);
		return NULL;
	}
	pair->slave_id = (unsigned int)ret;
	/* Here we limit pair id up to ptmx->max_pair */
	if (pair->slave_id >= ptmx->max_pair) {
		tty_warn("tty: Stream: ptmx pair num %u overflow\n", pair->slave_id);
		stream_delete_pair(pair);
		tty_mem_free(pair);
		(void)tty_mutex_unlock(&ptmx->mutex);
		return NULL;
	}
	(void)tty_mutex_unlock(&ptmx->mutex);

	/* register one tty device */
	ret = stream_register_tty(pair);
	if (ret < 0) {
		tty_warn("tty: Stream: tty_register failed, err: %s\n", strerror(-ret));
		(void)tty_mutex_lock(&ptmx->mutex);
		stream_delete_pair(pair);
		(void)tty_mutex_unlock(&ptmx->mutex);
		tty_mem_free(pair);
		return NULL;
	}
	return pair;
}

/*
 * Since we do not support remove device now, we will
 * recycle the free pair from pair_map before really
 * alloc one pair
 */
struct stream_device_pair *
stream_device_pair_create(struct stream_control_device *ptmx)
{
	struct stream_device_pair *pair = NULL;

	if (ptmx == NULL) {
		return NULL;
	}

	(void)tty_mutex_lock(&ptmx->mutex);
	pair = stream_pop_first_free_pair(ptmx);
	(void)tty_mutex_unlock(&ptmx->mutex);
	if (pair != NULL) {
		tty_debug("tty: stream Recycle using pts, slave_id=%u\n", pair->slave_id);
		return pair;
	}
	return stream_device_pair_alloc(ptmx);
}

void stream_device_pair_release(struct stream_device_pair *pair)
{
	if (pair == NULL || pair->tty == NULL) {
		return;
	}
	/*
	 * Instead of rm device which is not supported now,
	 * we will add pair to pair_map and set it as free.
	 * Recycle it next time.
	 */
	tty_struct_set_removed(pair->tty);
}
