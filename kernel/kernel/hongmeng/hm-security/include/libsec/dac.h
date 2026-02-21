/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: DAC header file
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:18:53 2019
 */
#ifndef ULIBS_LIBSEC_DAC_H
#define ULIBS_LIBSEC_DAC_H

#include <unistd.h>
#include "log.h"
#include "cred.h"
#include "module.h"
#include "method/var.h"
#include "method/para.h"
#include "pac.h"

struct sec_ctx_dac {
	__sec_data(libsec, cntx) uid_t uid;
	__sec_data(libsec, cntx) gid_t gid;
	__sec_data(libsec, cntx) mode_t mode;
};

struct sec_dac_proc_args {
	uid_t proc_uid;
	gid_t proc_gid;
};

#define SEC_CAN_EXEC     ((unsigned short)00001)
#define SEC_CAN_WRITE    ((unsigned short)00002)
#define SEC_CAN_READ     ((unsigned short)00004)
#define SEC_CAN_RWX      ((unsigned short)00007)

#define SEC_SHIFT_FBIT      __U(9)
#define SEC_SHIFT_UBIT      __U(6)
#define SEC_SHIFT_GBIT      __U(3)
#define SEC_SHIFT_OBIT      __U(0)

#define SEC_ISYS_SRVC __U(0200000)
#define SEC_IRWX_UGO  __U(0000777)
#define SEC_IFMT_NATV (SEC_ISYS_SRVC | SEC_IRWX_UGO)

#define SEC_INVALID_UID (uid_t)(-1)
#define SEC_INVALID_GID (uid_t)(-1)

#define DAC_INIT_VAL "dac_init_val"

extern struct sec_mod sec_mod_dac;
void sec_chk_fail_log_dac(const struct sec_cred_dac *proc, const struct sec_cred_basic *basic,
			  const struct sec_ctx_dac *obj, const char *node_name);

int sec_chk_dac_permission(const struct sec_chk_cred *cred,
			   const struct sec_ctx_dac *ctx,
			   const struct sec_dac_proc_args *args,
			   unsigned short acc_mode);

#define SEC_DEFINE_DAC(name, arg)						\
static int _sec_chk_dac_##name(const struct sec_chk_cred *cred,			\
	const struct sec_cred_dac *dac_cred PARA(arg));				\
static int sec_chk_dac_##name(const struct sec_chk_cred *cred PARA(arg))	\
{										\
	struct sec_cred_dac *dac;						\
	int rc;									\
	dac =  dac_cred_of_sec_cred(cred);					\
	rc = _sec_chk_dac_##name(cred, dac VAR(arg));			\
	return rc;								\
}										\
static int _sec_chk_dac_##name(const struct sec_chk_cred *cred,			\
			       const struct sec_cred_dac *dac_cred PARA(arg))

#define SEC_DEFINE_NATV_DAC(name, arg)						\
static int _sec_chk_natv_dac_##name(const struct sec_chk_cred *cred,		\
	const struct sec_cred_dac *dac_cred,					\
	const struct sec_cred_basic *basic_cred PARA(arg));			\
static int sec_chk_dac_##name(const struct sec_chk_cred *cred PARA(arg))	\
{										\
	struct sec_cred_dac *dac;						\
	struct sec_cred_basic *basic;					\
	int rc;									\
	dac = dac_cred_of_sec_cred(cred);					\
	basic = basic_cred_of_sec_cred(cred);				\
	rc = _sec_chk_natv_dac_##name(cred, dac, basic VAR(arg));	\
	return rc;								\
}										\
static int _sec_chk_natv_dac_##name(const struct sec_chk_cred *cred,		\
				    const struct sec_cred_dac *dac_cred,	\
				    const struct sec_cred_basic *basic_cred PARA(arg))

/*
 * initialize dac context of native objects:
 * SEC_ISYS_SRVC flag is also allowed to set by service on chmod.
 * It is discovered that uapps cannot create the object with SEC_ISYS_SRVC flag.
 */
#define SEC_DEFINE_DAC_CREATE_DEFAULT(name, arg, obj_sec_ctx, perm)		\
SEC_DEFINE_NATV_DAC(name, arg)							\
{										\
	int rc = E_HM_OK;							\
	(void)cred;								\
	if ((obj_sec_ctx) != NULL) {						\
		rc = E_HM_POSIX_EXIST;						\
	}									\
	if (rc == E_HM_OK && ((perm) & SEC_ISYS_SRVC) != 0U &&			\
	    (basic_cred->flags & (1U << SEC_BASIC_SYS_SRVC)) == 0U) {		\
		rc = E_HM_PERM;							\
	}									\
	if (rc == E_HM_OK) {							\
		(obj_sec_ctx) = malloc(sizeof(struct sec_ctx_dac));		\
		if ((obj_sec_ctx) == NULL) {					\
			rc = E_HM_NOMEM;					\
		}								\
	}									\
	if (rc == E_HM_OK) {							\
		((struct sec_ctx_dac *)(obj_sec_ctx))->uid = dac_cred->euid;	\
		((struct sec_ctx_dac *)(obj_sec_ctx))->gid = dac_cred->egid;	\
		((struct sec_ctx_dac *)(obj_sec_ctx))->mode = (perm) & SEC_IFMT_NATV;	\
	}									\
	return rc;								\
}

#define sec_define_dac_method(method)						\
	sec_define_chk_method(method, sec_chk_dac_##method)

#endif
