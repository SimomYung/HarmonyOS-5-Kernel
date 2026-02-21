/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Help to process signal in activation handler
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 13 17:44:07 2020
 */
#include <hmasm/page.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmpm/actv_helper.h>
#include <libhmsrv_sys/hm_signal.h>
#include <libhmsync/raw_thread.h>
#include <libsysif/utils.h>

#define THREAD_STACK_SIZE (2 * PAGE_SIZE)

struct actv_helper_context {
	struct actv_helper_obj *helper;
	int retval;
};

static void *handle_actv_helper(void *arg)
{
	int err;
	struct actv_helper_context *ctx = NULL;

	ctx = (struct actv_helper_context *)arg;
	err = ctx->helper->callback(ctx->helper->data);
	ctx->retval = err;

	return arg;
}

static int prepare_actv_helper_attr(raw_thread_attr_t *attr, int policy, int prio)
{
	int err = E_HM_OK;
	struct raw_sched_param param;

	err = raw_thread_attr_init(attr);
	if (err != E_HM_OK) {
		hm_error("failed to init helper thread attr: %s\n",
			 hmstrerror(err));
		return err;
	}
	err = raw_thread_attr_setinheritsched(attr, RAW_THREAD_EXPLICIT_SCHED);
	if (err != E_HM_OK) {
		hm_error("failed to set helper thread attr inherit, err=%s\n",
			 hmstrerror(err));
		return err;
	}
	err = raw_thread_attr_setschedpolicy(attr, policy);
	if (err != E_HM_OK) {
		hm_error("failed to set helper thread attr policy, err=%s\n",
			 hmstrerror(err));
		return err;
	}
	mem_zero_s(param);
	param.priority = (unsigned int)prio;
	err = raw_thread_attr_setschedparam(attr, &param);
	if (err != E_HM_OK) {
		hm_error("failed to set helper thread param, err=%s\n",
			 hmstrerror(err));
		return err;
	}
	err = raw_thread_attr_setstacksize(attr, THREAD_STACK_SIZE);
	if (err != E_HM_OK) {
		hm_error("failed to set helper thread stack size: %s\n",
			 hmstrerror(err));
		return err;
	}

	return err;
}

int actv_helper_init(struct actv_helper_obj *helper)
{
	int err;
	void *res = NULL;
	raw_thread_attr_t attr;
	raw_thread_t thread = NULL;
	struct actv_helper_context ctx;

	if (helper == NULL || helper->callback == NULL) {
		hm_error("invalid actv helper\n");
		return E_HM_INVAL;
	}

	err = prepare_actv_helper_attr(&attr, helper->policy, helper->prio);
	if (err != E_HM_OK) {
		return err;
	}

	ctx.helper = helper;
	ctx.retval = E_HM_OK;
	err = raw_thread_create(&thread, &attr, handle_actv_helper, &ctx);
	if (err != E_HM_OK) {
		hm_error("create helper thread failed: %s\n", hmstrerror(err));
		return err;
	}
	err = raw_thread_join(thread, &res);
	if (err != E_HM_OK) {
		hm_error("join helper thread failed: %s\n", hmstrerror(err));
		return err;
	}
	if (res != NULL) {
		return ((struct actv_helper_context *)res)->retval;
	}

	return E_HM_HMSIGINTR;
}
