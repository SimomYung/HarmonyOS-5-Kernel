/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Function implementation of raw conuter
 * Author: Huawei CBG OS Lab
 * Create: Wed Mar 20 2024
 */
#include <hongmeng/errno.h>

#include <libhmsync/raw_completion.h>

int raw_completion_wait(struct raw_completion *comp)
{
	int rc;

	BUG_ON_D(comp == NULL);

	do {
		rc = raw_counter_wait(&comp->done);
	} while (rc == E_HM_INTR);

	return rc;
}
