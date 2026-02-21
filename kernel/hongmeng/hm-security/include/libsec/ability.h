/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Provide ability check functions
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:18:53 2019
 */
#ifndef ULIBS_LIBSEC_ABILITY_H
#define ULIBS_LIBSEC_ABILITY_H

#include <libhmlog/hmlog.h>
#include <libpolicy_api.h>
#include <policy_def.h>
#include "log.h"
#include "module.h"
#include "cred.h"
#include "method/para.h"
#include "method/var.h"
#include "method/unused_var.h"

extern struct sec_mod sec_mod_ability;

#define SEC_DEFINE_ABILITY(NAME, _ARG)						\
static int _sec_chk_ability_##NAME(const struct sec_chk_cred *cred,		\
				   const struct sec_cred_mac *mac_cred PARA(_ARG));\
static int sec_chk_ability_##NAME(const struct sec_chk_cred *cred PARA(_ARG))	\
{										\
	int rc;									\
	rc = _sec_chk_ability_##NAME(cred, mac_cred_of_sec_cred(cred) VAR(_ARG));		\
	return rc;								\
}										\
static int _sec_chk_ability_##NAME(const struct sec_chk_cred *cred,		\
				   const struct sec_cred_mac *mac_cred PARA(_ARG))

#define SEC_DEFINE_FWD_CRED_ABILITY(NAME, _ARG) SEC_DEFINE_XACT_CRED_ABILITY(NAME, _ARG, fwd)
#define SEC_DEFINE_SRC_CRED_ABILITY(NAME, _ARG) SEC_DEFINE_XACT_CRED_ABILITY(NAME, _ARG, src)
#define SEC_DEFINE_XACT_CRED_ABILITY(NAME, _ARG, CRED)				\
static int _sec_chk_ability_##NAME(const struct sec_chk_cred *cred,		\
				   const struct sec_cred_mac *mac_cred		\
				   PARA(_ARG));					\
static int sec_chk_ability_##NAME(struct sec_chk_cred_xact *cred PARA(_ARG))	\
{										\
	struct sec_chk_cred *CRED##_cred = NULL;				\
	int rc = E_HM_OK;							\
	CRED##_cred = sec_chk_##CRED##_cred(cred);                              \
	if (CRED##_cred == NULL) {                                              \
		rc = E_HM_SRCH;                                                 \
	}                                                                       \
	if (rc == E_HM_OK) {                                                    \
		rc = _sec_chk_ability_##NAME(CRED##_cred, mac_cred_of_sec_cred(CRED##_cred) VAR(_ARG));	\
	}                                                                       \
	return rc;								\
}										\
static int _sec_chk_ability_##NAME(const struct sec_chk_cred *cred,		\
				   const struct sec_cred_mac *mac_cred		\
				   PARA(_ARG))

#define sec_define_ability_method(METHOD)					\
	sec_define_chk_method(METHOD, sec_chk_ability_##METHOD)

void sec_chk_fail_log_ability(const struct sec_chk_cred *cred, uint64_t subj,
			      enum ability_def ability, struct sec_log_extra_info *info);

/* need to be abandoned */
bool sec_check_ability_extra_failed(const struct sec_chk_cred *cred,
				    uint64_t subj, enum ability_def ability,
				    sec_chk_show_extra func, void *arg);

static inline bool sec_check_ability_extra(const struct sec_chk_cred *cred,
					   uint64_t subj, enum ability_def ability,
					   sec_chk_show_extra func, void *arg)
{
	bool check_passed = true;

	if (!check_ability(subj, ability)) {
		check_passed = sec_check_ability_extra_failed(cred, subj, ability, func, arg);
	}

	return check_passed;
}

int sec_check_ability_post_failed(const struct sec_chk_cred *cred,
				  const struct sec_cred_mac *mac_cred,
				  enum ability_def ability,
				  sec_chk_show_extra func, void *arg);

/* internal ability should not give to external subject */
#define check_ab_policy(mac_cred, ability) \
	check_ability(sec_plc_is_internal_ab((ability)) ? (mac_cred)->inner_sid : (mac_cred)->sid, (ability))

static inline int sec_check_ability(const struct sec_chk_cred *cred,
				    const struct sec_cred_mac *mac_cred,
				    enum ability_def ability,
				    sec_chk_show_extra func, void *arg)
{
	int ret = (mac_cred == NULL) ? E_HM_INVAL : E_HM_OK;
	if (ret == E_HM_OK) {
		if (!check_ab_policy(mac_cred, ability)) {
			ret = sec_check_ability_post_failed(cred, mac_cred, ability, func, arg);
		}
	}
	return ret;
}

#define SEC_DEFINE_ABILITY_DEFAULT(NAME, _ARG, ABILITY)				\
SEC_DEFINE_ABILITY(NAME, _ARG)							\
{										\
	UNUSED_VAR(_ARG);							\
	return sec_check_ability(cred, mac_cred, ABILITY,			\
				 sec_chk_show_obj_name, NULL);			\
}

// check ability of fwd credential in a transaction
#define SEC_DEFINE_FWD_CRED_ABILITY_DEFAULT(NAME, _ARG, ABILITY)	\
SEC_DEFINE_FWD_CRED_ABILITY(NAME, _ARG)					\
{									\
	UNUSED_VAR(_ARG);						\
	return sec_check_ability(cred, mac_cred, ABILITY,		\
				 sec_chk_show_obj_name, NULL);		\
}

#define SEC_DEFINE_SRC_CRED_ABILITY_DEFAULT(NAME, _ARG, ABILITY)	\
SEC_DEFINE_SRC_CRED_ABILITY(NAME, _ARG)					\
{									\
	UNUSED_VAR(_ARG);						\
	return sec_check_ability(cred, mac_cred, ABILITY,		\
				 sec_chk_show_obj_name, NULL);		\
}

int sec_check_abilities_failed(const struct sec_chk_cred *cred,
			       const struct sec_cred_mac *mac_cred,
			       const ability_t *missed_ab,
			       sec_chk_show_extra show_proc);

/*
 * caculate the missed abililites from available and expected abilities
 */
static inline void sec_calc_missed_abilities(const ability_t *exp_ablty,
					     const ability_t *avail_ablty,
					     ability_t *missed_ab)
{
	for (unsigned int i = 0; i < ABILITY_WIDTH; i++) {
		missed_ab->ability[i] = (avail_ablty->ability[i] | exp_ablty->ability[i]) ^
					avail_ablty->ability[i];
	}
}

/* need to be abandoned */
static inline int sec_chk_export_abilities(const struct sec_chk_cred *cred,
					   const struct sec_cred_mac *mac_cred,
					   ability_t *exp_ab, ability_t *avail_ab,
					   sec_chk_show_extra show_proc)
{
	int ret = E_HM_OK;

	ability_t log_avail_ab = {0};
	if (!check_and_export_abilities(mac_cred->sid, exp_ab, avail_ab, &log_avail_ab)) {
		ability_t missed_ab = {0};
		sec_calc_missed_abilities(exp_ab, &log_avail_ab, &missed_ab);
		ret = sec_check_abilities_failed(cred, mac_cred, &missed_ab, show_proc);
	}

	return ret;
}

static inline int sec_chk_export_abilities_with_name(const struct sec_chk_cred *cred,
						     ability_t *exp_ablty,
						     ability_t *avail_ablty,
						     sec_chk_show_extra show_proc)
{
	int ret;
	ret = sec_chk_export_abilities(cred, mac_cred_of_sec_cred(cred), exp_ablty, avail_ablty,
					       show_proc);

	return ret;
}

/* check abilities */
int sec_chk_do_check_abilities(const struct sec_chk_cred *cred,
			       const ability_t *exp_ablty, sec_chk_show_extra show_proc);

/* export abilities */
int sec_chk_do_export_abilities(const struct sec_chk_cred *cred, ability_t *avail_ablty);

#endif
