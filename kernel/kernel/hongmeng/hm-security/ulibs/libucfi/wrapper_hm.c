/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Implementation of wrapped hongmeng functon
 * Author: Huawei OS Kernel Lab
 * Create: Web Mar 30 16:30:11 2022
 */
/* libc */
#include <errno.h>
#include <dlfcn.h>
#include <stdio.h>

/* hongmeng ulibs */
#include <libhmlog/hmlog.h>

/* hm-security */
#include "ucfi.h"

/* direct header */
#include "macro.h"
#include "wrapper_hm.h"


void hmcrt_posthook(void)
{
	int rc;

	rc = ucfi_init();
	if (rc != E_HM_OK) {
		hm_error("failed to initilize ucfi\n");
		return;
	}

	hmcrt_posthook_type ori_hook = dlsym(RTLD_NEXT, "hmcrt_posthook");
	if (ori_hook != NULL) {
		ori_hook();
	}

	return;
}

void *dlopen(const char *file, int mode)
{
	struct dso *p = NULL;

	dlopen_type ori_hook = dlsym(RTLD_NEXT, "dlopen");
	if (ori_hook == NULL) {
		return NULL;
	}

	p = ori_hook(file, mode);
	if (p != NULL) {
		/* update ucfi bitmap */
		ucfi_update(p, true);
	}

	return p;
}

int dlclose(void *p)
{
	int rc = 0;

	/* update ucfi bitmap */
	ucfi_update(p, false);

	dlclose_type ori_hook = dlsym(RTLD_NEXT, "dlclose");
	if (ori_hook != NULL) {
		rc = ori_hook(p);
	}

	return rc;
}
