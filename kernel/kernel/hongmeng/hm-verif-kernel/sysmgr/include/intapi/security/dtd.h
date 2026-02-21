/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: threat detection scanner
 * Author: Huawei
 * Create: Wen May 17 10:00:10 2024
 */

#ifndef SYSMGR_INTAPI_DTD_H
#define SYSMGR_INTAPI_DTD_H

#include <stdint.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>

#define MAX_KERNEL_EXTRA_LEN 800

struct dtd_measure_result {
	int32_t status;
	int32_t cred;
	char extra[MAX_KERNEL_EXTRA_LEN];
};

void dtd_init_baseline(void);
int dtd_measure_item(int64_t event_id, struct dtd_measure_result *result);
#ifndef CONFIG_SECURITY_DTD
void dtd_init_baseline(void)
{
}

int dtd_measure_item(int64_t event_id, struct dtd_measure_result *result)
{
	UNUSED(event_id);
	UNUSED(result);

	return E_HM_INVAL;
}
#endif

#endif // !SYSMGR_INTAPI_DTD_H
