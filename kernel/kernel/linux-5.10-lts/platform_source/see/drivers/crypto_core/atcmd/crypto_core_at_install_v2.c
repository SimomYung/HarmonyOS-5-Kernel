/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Crypto Core at command install function.
 * Create: 2023/07/06
 */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <securec.h>
#include "crypto_core_errno.h"
#include "crypto_core_factory.h"
#include "crypto_core_smc.h"
#include "crypto_core_fs.h"

bool mspc_at_install_bypass(void)
{
	return false;
}

int32_t mspc_install_func(void)
{
#ifdef CRYPTO_CORE_PUB_SSD
	return MSPC_UNSUPPORT_PUBSSD_ERROR;
#else
	return mspc_total_manufacture_func();
#endif
}

int32_t mspc_at_get_install(const char *data, int32_t (*resp)(const char *buf, size_t len))
{
	int32_t ret;
	int32_t install_result = 0;
	int32_t default_value = 0;
	struct mspc_access_info access_info;

	if (!resp)
		return MSPC_AT_RSP_CALLBACK_NULL_ERROR;

	(void)memset_s(&access_info, sizeof(access_info), 0, sizeof(access_info));
	access_info.access_type = MSPC_INSTALL_RESULT_READ;
	ret = mspc_access_partition((int8_t *)&install_result, sizeof(install_result), &access_info);
	if (ret != MSPC_OK) {
		pr_err("%s: read install result failed! ret:%x\n", __func__, ret);
		(void)resp("FAIL", sizeof("FAIL"));
		return ret;
	}

	pr_err("%s: result = 0x%x\n", __func__, install_result);
	if (install_result != MSPC_OK) {
		(void)resp("FAIL", sizeof("FAIL"));
		return install_result;
	}

	/* if install_result is MSPC_OK, clear install result in hisee_img */
	(void)memset_s(&access_info, sizeof(access_info), 0, sizeof(access_info));
	access_info.access_type = MSPC_INSTALL_RESULT_WRITE;
	pr_err("%s:start clear result\n", __func__);
	ret = mspc_access_partition((int8_t *)&default_value, sizeof(default_value), &access_info);
	if (ret != MSPC_OK) {
		pr_err("%s: clear result failed! ret:%x\n", __func__, ret);
		(void)resp("FAIL", sizeof("FAIL"));
		return ret;
	}

	return MSPC_OK;
}
