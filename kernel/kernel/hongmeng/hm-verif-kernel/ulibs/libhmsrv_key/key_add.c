/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Native interface of add_key
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 02 16:15:39 2021
 */

#include <stdint.h>
#include <libmem/utils.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>
#include <libsysif/keymgr/api.h>
#include <libhmsrv_key/key.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_crypt/crypt.h>

long hm_key_add(const char *type, const char *name, const void *payload,
		size_t plen, int link_keyring_id)
{
	int ret = lsyscall_keymgr_add_key(type, name, payload, plen, link_keyring_id);
	if (ret < E_HM_OK) {
		hm_debug("failed to add key, err=%s\n", hmstrerror(ret));
	}

	/* return new key id */
	return ret;
}
