/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Devhost dso-extension demo
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 15 11:40:37 2021
 */

#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libhmlog/hmlog.h>
#include <libhmactv/actv.h>
#include <libhmucap/ucap.h>

#include <lnxbase/export.h>
#include <liblinux/pal.h>

static int dso_simple_export_test(void)
{
	hm_info("enter dso exported test symbol\n");
	return 0;
}
EXPORT_SYMBOL(dso_simple_export_test);
