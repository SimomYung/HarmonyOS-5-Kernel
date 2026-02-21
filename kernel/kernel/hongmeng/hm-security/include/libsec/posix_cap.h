/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Define posix cap check interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 11 14:10:50 2019
 */

#ifndef ULIBS_LIBSEC_POSIX_CAP_H
#define ULIBS_LIBSEC_POSIX_CAP_H
// hongmeng
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>
#include <hmkernel/strict.h>
// security
#include "log.h"
#include "cred.h"
#include "module.h"
#include "ns_usr.h"
#include "posix_cap_def.h"
#include "ns_cred.h"
#include "method/var.h"
#include "method/para.h"
#include "method/unused_var.h"

extern struct sec_mod sec_mod_posix_cap;
const char *sec_cap_name(unsigned int cap_index);

#define SEC_DEFINE_POSIX_CAP(NAME, _ARG)							\
static int _sec_chk_posix_cap_##NAME(const struct sec_chk_cred *cred,			\
				     const struct sec_cred_caps *posix_cap_cred		\
				     PARA(_ARG));					\
static int sec_chk_posix_cap_##NAME(const struct sec_chk_cred *cred PARA(_ARG))		\
{											\
	struct sec_cred_caps *posix_cap;						\
	int rc;										\
	posix_cap = caps_cred_of_sec_cred(cred);					\
	rc = _sec_chk_posix_cap_##NAME(cred, posix_cap VAR(_ARG));			\
	return rc;									\
}											\
static int _sec_chk_posix_cap_##NAME(const struct sec_chk_cred *cred,			\
				     const struct sec_cred_caps *posix_cap_cred		\
				     PARA(_ARG))

#define SEC_DEFINE_FWD_CRED_POSIX_CAP(NAME, _ARG)					\
static int _sec_chk_posix_cap_##NAME(const struct sec_chk_cred *cred,			\
				     const struct sec_cred_caps *posix_cap_cred		\
				     PARA(_ARG));					\
static int sec_chk_posix_cap_##NAME(struct sec_chk_cred_xact *cred PARA(_ARG))	\
{											\
	struct sec_chk_cred *fwd_cred = NULL;						\
	struct sec_cred_caps *posix_cap;						\
	int rc;										\
	fwd_cred = sec_chk_fwd_cred(cred);						\
	if (fwd_cred == NULL) {								\
		rc = E_HM_SRCH;								\
	} else {									\
		posix_cap = caps_cred_of_sec_cred(fwd_cred);				\
		rc = _sec_chk_posix_cap_##NAME(fwd_cred, posix_cap VAR(_ARG));		\
	}										\
	return rc;									\
}											\
static int _sec_chk_posix_cap_##NAME(const struct sec_chk_cred *cred,			\
				     const struct sec_cred_caps *posix_cap_cred		\
				     PARA(_ARG))

#define sec_define_posix_cap_method(METHOD)				\
	sec_define_chk_method(METHOD, sec_chk_posix_cap_##METHOD)

void sec_chk_fail_log_posix_cap(const struct sec_chk_cred *cred, unsigned int posix_cap);

struct usr_ns_cred *get_root_usr_ns_cred(void);
cred_idx_t get_root_usr_ns_idx_by_shconf(void);
void set_root_usr_ns_cred(struct usr_ns_cred *root_usr_ns_cred);

static inline
struct usr_ns_cred *sec_get_root_usr_ns_cred(void)
{
	struct usr_ns_cred *root_usr_ns_cred = get_root_usr_ns_cred();
	if (root_usr_ns_cred != NULL) {
		return root_usr_ns_cred;
	}
	root_usr_ns_cred = sec_cred_idx_to_ptr(get_root_usr_ns_idx_by_shconf());
	set_root_usr_ns_cred(root_usr_ns_cred);
	return root_usr_ns_cred;
}

static inline
bool posix_cap_issubset(const posix_cap_t set1, const posix_cap_t set2)
{
	bool ret = true;
	if (____IS(set1 & ~set2)) {
		ret = false;
	}

	return ret;
}

static inline
posix_cap_t posix_cap_combine(const posix_cap_t set1, const posix_cap_t set2)
{
	return set1 | set2;
}

static inline
posix_cap_t posix_cap_intersect(const posix_cap_t set1, const posix_cap_t set2)
{
	return set1 & set2;
}

#ifdef SEC_CHK_PERMISSIVE
#define SEC_POSIX_CAP_PERMISSIVE
#endif

#ifdef CONFIG_USER_NS
bool check_ns_posix_cap(const struct sec_chk_cred *cred, posix_cap_t eff,
	struct usr_ns_cred *targ_usr_ns_cred, unsigned int posix_cap);
#endif

static inline bool check_posix_cap(posix_cap_t eff, unsigned int posix_cap)
{
	bool ret = ____IS(eff & posix_cap_to_mask(posix_cap));
#ifdef SEC_POSIX_CAP_PERMISSIVE
	ret = true;
#endif
	return ret;
}

static inline bool sec_check_posix_cap(const struct sec_chk_cred *cred,
				       posix_cap_t eff, unsigned int posix_cap)
{
	bool check_passed = true;

#ifndef SEC_POSIX_CAP_DISABLE
	if (!check_posix_cap(eff, posix_cap)) {
		sec_chk_fail_log_posix_cap(cred, posix_cap);
#ifndef SEC_POSIX_CAP_PERMISSIVE
		check_passed = false;
#endif
	}
#endif
	return check_passed;
}

bool sec_do_capable(const struct sec_chk_cred *cred, posix_cap_t eff, unsigned int cap);
bool sec_do_capable_noaudit(const struct sec_chk_cred *cred, posix_cap_t eff, unsigned int cap);
// sec_capable: check posix capability and SEHarmony capabilit (if SEHarmony enabled)
bool sec_capable(const struct sec_chk_cred *cred, unsigned int cap);
bool sec_capable_noaudit(const struct sec_chk_cred *cred, unsigned int cap);
#ifdef CONFIG_USER_NS
bool sec_ns_capable(const struct sec_chk_cred *cred, struct usr_ns_cred *targ_usr_ns_cred, unsigned int posix_cap);
#endif
// cap_capable: check posix capability
bool cap_capable(const struct sec_chk_cred *cred, unsigned int cap);

#define SEC_DEFINE_POSIX_CAP_DEFAULT(NAME, _ARG, POSIX_CAP)		\
SEC_DEFINE_POSIX_CAP(NAME, _ARG)						\
{									\
	UNUSED_VAR(_ARG);						\
	int rc = E_HM_OK;						\
	if (!sec_do_capable(cred,					\
		posix_cap_cred->eff_cap, POSIX_CAP)) {		\
		rc = E_HM_PERM;						\
	}								\
	return rc;							\
}

// check posix capability of fwd credential in a transaction
#define SEC_DEFINE_FWD_CRED_POSIX_CAP_DEFAULT(NAME, _ARG, POSIX_CAP)	\
SEC_DEFINE_FWD_CRED_POSIX_CAP(NAME, _ARG)				\
{									\
	UNUSED_VAR(_ARG);						\
	int rc = E_HM_OK;						\
	if (!sec_check_posix_cap(cred,					\
		posix_cap_cred->eff_cap, POSIX_CAP)) {		\
		rc = E_HM_PERM;						\
	}								\
	return rc;							\
}

int sec_chk_posix_cap_overide_dac(const struct sec_chk_cred *cred,
				  mode_t obj_mode, unsigned short acc_mode);

typedef int (*cap_check_func)(const struct sec_chk_cred *cred, hmsel_sid_t sid, int cap
	, unsigned int opts, bool initns);
void sec_set_sel_cap_check_func(cap_check_func fun);
#endif
