/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 09 17:50:59 2021
 */

#ifndef ULIBS_LIBSEC_KEYRING_H
#define ULIBS_LIBSEC_KEYRING_H

#include "module.h"
#include "cred.h"
#include "method/para.h"
#include "method/var.h"

extern struct sec_mod sec_mod_keyring;

#define SEC_DEFINE_KEYRING(NAME, ARG)						\
static int _sec_chk_keyring_##NAME(const struct sec_chk_cred *cred,		\
			       const struct sec_cred_keyring *keyring_cred PARA(ARG));	\
static int sec_chk_keyring_##NAME(const struct sec_chk_cred *cred PARA(ARG))	\
{										\
	struct sec_cred_keyring *keyring;					\
	keyring = keyring_cred_of_sec_cred(cred);				\
	return _sec_chk_keyring_##NAME(cred, keyring VAR(ARG));			\
}										\
static int _sec_chk_keyring_##NAME(const struct sec_chk_cred *cred,		\
				 const struct sec_cred_keyring *keyring_cred PARA(ARG))

#define SEC_DEFINE_KEYRING_METHOD(METHOD)						\
	SEC_DEFINE_CHK_METHOD(METHOD, sec_chk_keyring_##METHOD)

#endif /* ULIBS_LIBSEC_KEYRING_H */
