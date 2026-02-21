/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of entropy
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 22 18:30:34 2020
 */
#include "crypto.h"

#include <libsysif/devhost/server.h>
#include <libentropy/entropy.h>
#include <libhwsecurec/securec.h>

#include "devhost.h"
#include "devhost_api.h"
#include <devhost/log.h>

int hdr_devhost_core_entropy_read(unsigned long long sender,
				  unsigned long credential,
				  unsigned int num)
{
	int ret = E_HM_OK;
	struct __actvret_drvcall_devhost_core_entropy_read *pret =
		actvhdlr_drvhandler_devhost_core_entropy_read_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL);

	if (num > DEVHOST_ENTROPY_DATA_MAX_LEN) {
		return E_HM_INVAL;
	}

	pret->data.len = num;
	ret = entropy_read_randomness(pret->data.buf, pret->data.len,
				      &pret->data.fail_count);
	if (ret < E_HM_OK) {
		dh_error("entropy read randomness failed, %s\n", hmstrerror(ret));
		return ret;
	}
	pret->data.len = (unsigned int)ret;

	return E_HM_OK;
}

PUBLIC_SYMBOL
void devhost_add_randomness(const void *data, unsigned int data_len)
{
	entropy_add_randomness(data, data_len, ENTROPY_DEVHOST_NOISE_SOURCE);
}
