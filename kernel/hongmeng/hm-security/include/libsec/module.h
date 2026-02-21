/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Secure module init and finish head file
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:21:52 2019
 */
#ifndef ULIBS_LIBSEC_MODULE_H
#define ULIBS_LIBSEC_MODULE_H
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_scopedptr.h>
#include "server_env.h"

typedef int (*sec_mod_init_ft)(const struct sec_server_env *);
typedef void (*sec_mod_fini_ft)(void);
struct sec_mod_ops {
	sec_mod_init_ft init;
	sec_mod_fini_ft fini;
};

struct sec_mod {
	const char *name;
	struct sec_mod_ops ops;
	struct raw_refcnt_nr ref_cnt;
	struct raw_mutex lock;
};

/* When the input count of the refcnt_nr_put interface is 1, the release flag is set
 * for the reference counting. An error is reported when the reference count operation
 * is performed later. This semantic corresponds to the release process of dynamically
 * created resources. However, sec mod is a global variable. When no user uses it,
 * resources are not released. In addition, modules may be dynamically loaded and
 * unloaded in the future. Therefore, the initial reference count of sec mod is set
 * to 1.
 */
#define SEC_MOD_UNINITIALIZED	1
#define SEC_MOD_REFCNT_NR_INITIALIZER	__RAW_REFCNT_NR_INITIALIZER(SEC_MOD_UNINITIALIZED)

int sec_mod_init(struct sec_mod *mod, const struct sec_server_env *env);
void sec_mod_fini(struct sec_mod *mod);

#define HM_SEC_MODULE_MAC		1u
#define HM_SEC_MODULE_DAC		2u
#define HM_SEC_MODULE_CAPABILITY	3u
#define HM_SEC_MODULE_SEHARMONY		4u
#define HM_SEC_MODULE_AUDIT		5u
#define HM_SEC_MODULE_MAX HM_SEC_MODULE_AUDIT

#endif
