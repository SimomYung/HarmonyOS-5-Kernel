/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Plugin demo for extension point
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 9 12:18:30 2020
 */

#include <hongmeng/errno.h>
#include <hongmeng/panic.h>

#include <devhost/plugin.h>
#include <devhost/backend.h>
#include <libhmlog/hmlog.h>
#include <libhmactv/actv.h>
#include <libhmucap/ucap.h>
#include <libstrict/strict.h>

#include <libsysif/devmgr/types.h>
#include <libsysif/devhost/server.h>

#include "devhost.h"

static int simple_event_happending(int test_val)
{
	hm_info("simple_event_happending with val %d\n", test_val);
	return E_HM_OK;
}

static int example_event_ep(void *arg)
{
	int test_val = 10;
	int ret = E_HM_OK;

	UNUSED(arg);

	hm_info("test event ep with val %d\n", test_val);
	DECLARE_EVENT_EP(simple_event_test, ret, simple_event_happending, test_val);
	if (ret < 0) {
		hm_info("simple_event_after don't happened\n");
	}
	return E_HM_OK;
}

static int simple_advice_default(int val_a, int val_b)
{
	hm_info("[%d, %d] -> %d\n", val_a, val_b, val_a + val_b);
	return val_a + val_b;
}

DEFINE_ADVICE_EP_PROTOTYPE(simple_advice_test, int, int);

static int example_advice_ep(void *arg)
{
	int val_a = 6;
	int val_b = 7;
	int ret = 0;

	UNUSED(arg);

	hm_info("test advice ep with val [%d, %d]\n", val_a, val_b);
	DECLARE_ADVICE_EP(simple_advice_test, ret, simple_advice_default, val_a, val_b);
	hm_info("test advice result: [%d + %d] * [x] = %d\n", val_a, val_b, ret);

	return E_HM_OK;
}

static int simple_constructor(const struct libdh_builtin_plugin *plugin)
{
	int ret = 0;
	UNUSED(plugin);

	ret = devhost_postinit(example_event_ep, NULL);
	if (ret < 0) {
		return ret;
	}
	ret = devhost_postinit(example_advice_ep, NULL);
	if (ret < 0) {
		return ret;
	}
	return E_HM_OK;
}

/* there is no extension */
DEFINE_BUILTIN_PLUGIN(simple_ep, simple_constructor);
