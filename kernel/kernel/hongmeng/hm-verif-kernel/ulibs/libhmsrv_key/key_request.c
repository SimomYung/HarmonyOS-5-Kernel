/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Native interface of request_key
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 03 16:16:45 2021
 */

#include <stdint.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>
#include <libhmsrv_key/key.h>
#include <libhmsrv_crypt/crypt.h>
#include <libsysif/keymgr/api.h>

long hm_key_request(const char *type, const char *name, const char *callout_info,
		    int32_t link_keyring_id)
{
	int ret = lsyscall_keymgr_request_key(type, name, callout_info, link_keyring_id);
	if (ret < E_HM_OK) {
		hm_debug("failed to request key, err=%s\n", hmstrerror(ret));
	}

	/* return search key id */
	return ret;
}
