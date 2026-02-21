/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: MAC secure framework check methods head file
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:18:53 2019
 */
#ifndef ULIBS_LIBSEC_MAC_H
#define ULIBS_LIBSEC_MAC_H

#include <libhmlog/hmlog.h>
#include <libpolicy_api.h>
#include <policy_def.h>
#include "log.h"
#include "module.h"
#include "cred.h"
#include "method/para.h"
#include "method/var.h"
#include "pac.h"

struct sec_ctx_mac {
	__sec_data(libsec, cntx) uint64_t type_id;
	__sec_data(libsec, cntx) uint64_t inner_sid;
};

extern struct sec_mod sec_mod_mac;

#define SEC_DEFINE_MAC(NAME, _ARG)						\
static int _sec_chk_mac_##NAME(const struct sec_chk_cred *cred,			\
			       const struct sec_cred_mac *mac_cred PARA(_ARG));	\
static int sec_chk_mac_##NAME(const struct sec_chk_cred *cred PARA(_ARG))	\
{										\
	int rc;									\
	rc = _sec_chk_mac_##NAME(cred, mac_cred_of_sec_cred(cred) VAR(_ARG));		\
	return rc;								\
}										\
static int _sec_chk_mac_##NAME(const struct sec_chk_cred *cred,			\
			       const struct sec_cred_mac *mac_cred PARA(_ARG))

#define sec_define_mac_method(METHOD)						\
	sec_define_chk_method(METHOD, sec_chk_mac_##METHOD)

bool sec_check_te_policy_failed(const struct sec_chk_cred *cred, uint64_t subj,
				uint64_t obj, int act);

static inline bool sec_check_te_policy(const struct sec_chk_cred *cred,
				       uint64_t subj, uint64_t obj, enum action_def act)
{
	bool check_passed = true;

	if (!check_te_policy(subj, obj, (uint64_t)(unsigned int)act)) {
		check_passed = sec_check_te_policy_failed(cred, subj, obj, (int)act);
	}

	return check_passed;
}

bool sec_check_te_policy_ex_failed(const struct sec_chk_cred *cred,
				   uint64_t subj, uint64_t obj, int act,
				   struct sec_log_extra_info *info);

static inline bool sec_check_te_policy_extra_inner(const struct sec_chk_cred *cred,
					     uint64_t subj, uint64_t obj, enum action_def act,
					     struct sec_log_extra_info info)
{
	bool check_passed = true;

	if (!check_te_policy(subj, obj, (uint64_t)(unsigned int)act)) {
		check_passed = sec_check_te_policy_ex_failed(cred, subj, obj, (int)act, &info);
	}

	return check_passed;
}

static inline void sec_type_transition(uint64_t subj_id, const char *obj,
				       enum action_def action, uint64_t *tgt_id)
{
	uint64_t obj_id = 0;

	if (get_sub_id_by_path(obj, &obj_id) != E_HM_OK) {
		obj_id = subj_id;
	}

	if (!get_type_transition(subj_id, obj_id, action, tgt_id)) {
		*tgt_id = obj_id;
	}
}

#define sec_lofs(STR) (sizeof(STR) - 1U)

#define sec_check_te_policy_extra(cred, subj, obj, act, infunc, inarg)		\
({										\
	struct sec_log_extra_info _info = {.func = (infunc), .arg = (inarg)};	\
	sec_check_te_policy_extra_inner(cred, subj, obj, act, _info);		\
})

#define SEC_DEFINE_MAC_CREATE_TRANSITION_DEFAULT(name, arg, prefix, obj_name,	\
						 name_max, action, obj_sec_ctx)	\
SEC_DEFINE_MAC(name, arg)							\
{										\
	char alias[(sec_lofs(prefix)) + (size_t)(name_max)] = (prefix);	\
	uint64_t tgt_id;							\
	int ret = E_HM_OK;							\
	if ((obj_sec_ctx) != NULL) {						\
		ret = E_HM_POSIX_EXIST;						\
	}									\
	if (ret == E_HM_OK) {							\
		ret = strncpy_s(alias + (sec_lofs(prefix)), (name_max),		\
				(obj_name), (name_max));			\
		if (ret != 0) {							\
			ret = E_HM_INVAL;					\
		}								\
	}									\
	if (ret == E_HM_OK) {							\
		sec_type_transition(mac_cred->sid, alias, (action), &tgt_id);	\
		if (!sec_check_te_policy_extra(cred, mac_cred->sid, tgt_id,	\
					       action, sec_chk_show_obj_name,	\
					       obj_name)) {			\
			ret = E_HM_ACCES;					\
		}								\
	}									\
	if (ret == E_HM_OK) {							\
		(obj_sec_ctx) = malloc(sizeof(struct sec_ctx_mac));		\
		if ((obj_sec_ctx) == NULL) {					\
			ret = E_HM_NOMEM;					\
		} else {							\
			((struct sec_ctx_mac *)(obj_sec_ctx))->type_id = tgt_id;\
		}								\
	}									\
	return ret;								\
}

#define SEC_DEFINE_MAC_DEFAULT(name, arg, action, obj_sec_ctx, obj_name)	\
SEC_DEFINE_MAC(name, arg)							\
{										\
	int ret = E_HM_OK;							\
	char* obj_alias = (obj_name);						\
	struct sec_ctx_mac *ctx = (struct sec_ctx_mac *)(obj_sec_ctx);		\
	if (ctx == NULL) {							\
		ret = E_HM_POSIX_NOENT;						\
	}									\
	if (ret == E_HM_OK) {							\
		if (obj_alias == NULL) {					\
			obj_alias = "anonymous";				\
		}								\
		if (sec_check_te_policy_extra(cred, mac_cred->sid,		\
					      ctx->type_id,			\
					      action,				\
					      sec_chk_show_obj_name,		\
					      obj_alias)) {			\
			ret = E_HM_OK;						\
		} else {							\
			ret = E_HM_ACCES;					\
		}								\
	}									\
	return ret;								\
}

static inline bool sec_cred_mac_sid_is(const struct sec_cred_mac *mac_cred, enum type_def exp_type)
{
	return ((mac_cred->sid == (uint64_t)exp_type) ? true : false);
}

static inline int sec_check_cred_mac_sid(const struct sec_chk_cred *chk_cred,
					 const struct sec_cred_mac *mac_cred,
					 enum type_def exp_type)
{
	int ret = E_HM_OK;

	if (!sec_cred_mac_sid_is(mac_cred, exp_type)) {
		sec_chk_show_obj_name(chk_cred, NULL);
		ret = E_HM_ACCES;
	}

	return ret;
}

#endif
