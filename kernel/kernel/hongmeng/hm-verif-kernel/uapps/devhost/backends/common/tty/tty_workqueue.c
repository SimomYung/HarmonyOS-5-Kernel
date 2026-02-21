/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: TTY utils for async workqueue
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 28 20:06:14 2020
 */

#include "tty_wrap.h"
#include "tty_workqueue.h"

#define TTY_WQ_TNAME			"tty-wq"
/* default max active value 256 when 0 is specified */
#define TTY_MAX_ACTIVE_DEFAULT		0

static struct workqueue_struct *g_tty_wq = NULL;

struct tty_async_work {
	struct work_struct work;
	tty_async_func_t func;
	void *data;
};

int tty_workq_init(void)
{
	g_tty_wq = alloc_workqueue(TTY_WQ_TNAME, WQ_UNBOUND,
				   TTY_MAX_ACTIVE_DEFAULT);
	if (g_tty_wq == NULL) {
		tty_err("tty: failed to alloc workqueue\n");
		return -ENOMEM;
	}

	return 0;
}

void tty_workq_exit(void)
{
	if (g_tty_wq != NULL) {
		destroy_workqueue(g_tty_wq);
		g_tty_wq = NULL;
	}
}

static void __handle_async_work(struct work_struct *work)
{
	struct tty_async_work *awork = NULL;

	if (work == NULL) {
		tty_err("tty: invalid work\n");
		return;
	}
	awork = container_of(work, struct tty_async_work, work);

	(*awork->func)(awork->data);
	tty_mem_free(awork);

	return;
}

void tty_async_schedule(tty_async_func_t func, void *data)
{
	struct tty_async_work *awork = NULL;

	if (func == NULL) {
		tty_warn("tty: invalid async work func\n");
		return;
	}

	awork = (struct tty_async_work *)tty_mem_zalloc(sizeof(*awork));
	if (awork == NULL) {
		tty_warn("tty: alloc async work failed\n");
		func(data);
		return;
	}

	INIT_WORK(&awork->work, __handle_async_work);
	awork->func = func;
	awork->data = data;

	queue_work(g_tty_wq, &awork->work);

	return;
}
tty_export_symbol(tty_async_schedule);
