/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Plugin demo for extension
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 9 12:18:30 2020
 */

#include <hongmeng/errno.h>
#include <hongmeng/panic.h>

#include <devhost/plugin.h>
#include <libhmlog/hmlog.h>
#include <libhmactv/actv.h>
#include <libhmucap/ucap.h>

#include <libsysif/devmgr/types.h>
#include <libsysif/devhost/server.h>

#include "devhost.h"

#define AROUND_ONE_STEP	10
static void simple_ev_before(int test_val)
{
	hm_info("imple before event happened with val %d\n", test_val);
}

static void simple_ev_after(int test_val)
{
	hm_info("simple after event happened with val %d\n", test_val);
}

static void simple_ad_before(int val_a, int val_b)
{
	hm_info("advice before event happened with val [%d, %d]\n", val_a, val_b);
}

static int simple_ad_around(struct advice_ctx *context, int val_a, int val_b)
{
	int result = 0;
	result = context->call_next(context);
	if (context->call_next_args(context, val_a + 1, val_b + 1) < 0) {
		hm_info("next is only called once\n");
	} else {
		hm_error("something go wrong !\n");
	}

	hm_info("next around result is %d\n", result);
	return result * AROUND_ONE_STEP;
}

static void simple_ad_after(int around_ret, int val_a, int val_b)
{
	hm_info("advice after event happened with val [%d, %d]->[%d]\n", val_a, val_b, around_ret);
}

static int simple_ad_around1(struct advice_ctx *context, int val_a, int val_b)
{
	int result = 0;
	result = context->call_next_args(context, val_a + 1, val_b + 1);
	if (context->call_next(context) < 0) {
		hm_info("next is only called once\n");
	} else {
		hm_error("something go wrong !\n");
	}

	hm_info("default around result is %d\n", result);
	return result * AROUND_ONE_STEP;
}

DEFINE_EVENT_EXTENSION(simple_ev_ext, simple_ev_before, simple_ev_after);
DEFINE_ADVICE_EXTENSION(simple_ad_ext, simple_ad_before, simple_ad_around, simple_ad_after);
DEFINE_ADVICE_EXTENSION(simple_ad_ext1, NULL, simple_ad_around1, NULL);

DEFINE_BUILTIN_PLUGIN(simple, NULL,
	IMPORT_EVENT_EXTENSION(simple_event_test, simple_ev_ext),
	IMPORT_ADVICE_EXTENSION(simple_advice_test, simple_ad_ext),
	IMPORT_ADVICE_EXTENSION(simple_advice_test, simple_ad_ext1)
);
