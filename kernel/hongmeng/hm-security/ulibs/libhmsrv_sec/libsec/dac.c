/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: DAC module init and fini functions
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:27:14 2019
 */
#include "libsec/dac.h"

#include <hongmeng/errno.h>
#include <libhmsync/raw_scopedptr.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsrv_sys/hm_security.h>

#include "libsec/server_env.h"

void sec_chk_fail_log_dac(const struct sec_cred_dac *proc, const struct sec_cred_basic *basic,
			  const struct sec_ctx_dac *obj, const char *node_name)
{
	if (hm_sec_debug_is_enabled() == false) {
		return;
	}
	sec_deny_log_info_init(log_info, NULL, basic,
			       "perm denied, proc: uid %u, gid %u, system_service 0x%x, "
			       "obj: '%s', uid %u, gid %u, mode(oct) 0%o\n",
			       false, true);
	const char *obj_name = (node_name == NULL) ? "unknown" : node_name;
	sec_deny_log_debug(&log_info, proc->euid, proc->egid,
			   sec_cred_is_flag_set(basic->flags, SEC_BASIC_SYS_SRVC),
			   obj_name, obj->uid, obj->gid, obj->mode);
}

static unsigned int dac_get_ctx_mode(unsigned short acc_mode, mode_t mode, unsigned short shift_bit)
{
	return acc_mode & (mode >> shift_bit);
}

static int check_dac_groups_permission(const struct sec_chk_cred *cred, const struct sec_ctx_dac *ctx,
				       const struct sec_dac_proc_args *args, unsigned short acc_mode)
{
	int rc;
	int result = E_HM_ACCES;

	rc = sec_cred_check_dac_groups(cred, ctx->gid, args->proc_gid);
	if (rc == E_HM_OK) {
		if (dac_get_ctx_mode(acc_mode, ctx->mode, SEC_SHIFT_GBIT) == acc_mode) {
			result = E_HM_OK;
		} else {
			result = E_HM_ACCES;
		}
	} else if (rc == E_HM_POSIX_NOENT && dac_get_ctx_mode(acc_mode, ctx->mode, SEC_SHIFT_OBIT) == acc_mode) {
		result = E_HM_OK;
	} else {
		result = E_HM_ACCES;
	}
	return result;
}

int sec_chk_dac_permission(const struct sec_chk_cred *cred,
			   const struct sec_ctx_dac *ctx,
			   const struct sec_dac_proc_args *args,
			   unsigned short acc_mode)
{
	int rc = E_HM_OK;
	uid_t proc_uid;

	if (cred == NULL || ctx == NULL || args == NULL) {
		hm_error("invalid param\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		proc_uid = args->proc_uid;
		if (proc_uid == ctx->uid) {
			if (dac_get_ctx_mode(acc_mode, ctx->mode, SEC_SHIFT_UBIT) == acc_mode) {
				rc = E_HM_OK;
			} else {
				rc = E_HM_ACCES;
			}
		} else {
			rc = check_dac_groups_permission(cred, ctx, args, acc_mode);
		}
	}
	return rc;
}


static int dac_init(const struct sec_server_env *env)
{
	UNUSED(env);
	return E_HM_OK;
}

static void dac_fini(void)
{
	return;
}

struct sec_mod sec_mod_dac = {
	.name = "dac",
	.ref_cnt = SEC_MOD_REFCNT_NR_INITIALIZER,
	.lock = RAW_MUTEX_INITIALIZER,
	.ops = {
		.init = dac_init,
		.fini = dac_fini,
	},
};
