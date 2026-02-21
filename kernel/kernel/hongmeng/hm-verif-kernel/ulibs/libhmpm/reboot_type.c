/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 * Description: Get reboot type
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 18 10:36:32 2021
 */

#include <hmkernel/capability.h>
#include <hmkernel/drivers/rebootcode.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmlog/hmlog.h>
#include <libhmpm/reboot_type.h>
#include <libhmpm/shutdown.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhwsecurec/securec.h>
#include <libsysif/srvmgr/api.h>

#define DTB_PATH_SIZE		30U
#define REBOOT_TYPE_PATH_SIZE	(((unsigned int)(REBOOT_STR_MAX_LEN)) + (DTB_PATH_SIZE))

int hm_reboot_cmd_to_type(const char *cmd, unsigned int *ret_type)
{
	int ret = E_HM_OK;
	uint32_t dtb_reason_val = 0U;
	char config_path[REBOOT_TYPE_PATH_SIZE];

	if (cmd == NULL || strnlen(cmd, REBOOT_STR_MAX_LEN) == 0 || ret_type == NULL) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK &&
	    strnlen(cmd, REBOOT_STR_MAX_LEN) == REBOOT_STR_MAX_LEN) {
		hm_warn("reboot reason is too long\n");
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		mem_zero_a(config_path);
		/* build config path */
		ret = sprintf_s(config_path, sizeof(config_path),
				"/hmmicrokernel/reboot_type/%s", cmd);
	}
	if (ret >= 0) {
		/* query reboot reason detail */
		ret = hm_sysctrl_read_dtb(config_path, strlen(config_path),
					  &dtb_reason_val, sizeof(uint32_t));
		if (ret < 0) {
			hm_warn("Read dtb to get reboot type failed\n");
		} else {
			*ret_type = (int)u32_to_cpu(dtb_reason_val);
			ret = E_HM_OK;
		}
	}

	return ret;
}
