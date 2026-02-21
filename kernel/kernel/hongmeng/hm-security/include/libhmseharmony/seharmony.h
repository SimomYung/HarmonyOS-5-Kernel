/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: seharmony secure framework check methods head file
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 06 11:18:53 2023
 */
#ifndef ULIBS_LIBSEC_SEHARMONY_H
#define ULIBS_LIBSEC_SEHARMONY_H

#include <libhmlog/hmlog.h>
#include <libsec/log.h>
#include <libsec/module.h>
#include <libsec/cred.h>
#include <libsec/pac.h>
#include <libsec/method/para.h>
#include <libsec/method/var.h>
#include <libhmseharmony/services.h>

#define HMSEL_AUDIT_NONE 0
#define HMSEL_AUDIT_DENTRY 1
#define HMSEL_AUDIT_IOCTL 2
#define HMSEL_AUDIT_PATH 3
#define HMSEL_AUDIT_FILE 4
#define HMSEL_AUDIT_INODE 5

struct hmsel_audit_info {
	char *buf;
	size_t len;
};

static inline void ai_update(int len, struct hmsel_audit_info *ai)
{
	size_t size;
	if (len > 0) {
		size = (size_t)(unsigned int)len;
		ai->buf = ai->buf + size;
		ai->len = ai->len - size;
	}
}

typedef void (*sec_sel_show)(const void *cred, const void *arg, struct hmsel_audit_info *ai);
struct sec_sel_audit {
	sec_sel_show func;
	const void *cred;
	const void *data;
};

extern struct sec_mod sec_mod_seharmony;

struct sec_ctx_sel {
	__sec_data(libsec, cntx) hmsel_sid_t sid;
	uint32_t user;
	uint32_t role;
	uint32_t type;
};

struct sec_ipc_ctx_sel {
	__sec_data(libsec, cntx) hmsel_sid_t sid;
	__sec_data(libsec, cntx) hmsel_class_t sclass;
};

struct sec_perf_ctx_sel {
	__sec_data(libsec, cntx) hmsel_sid_t sid;
};

#define SEC_DEFINE_SEHARMONY(NAME, _ARG)						\
static int _sec_chk_seharmony_##NAME(const struct sec_chk_cred *cred PARA(_ARG));	\
static int sec_chk_seharmony_##NAME(const struct sec_chk_cred *cred PARA(_ARG))	\
{										\
	int rc;									\
	rc = _sec_chk_seharmony_##NAME(cred VAR(_ARG));				\
	return rc;								\
}										\
static int _sec_chk_seharmony_##NAME(const struct sec_chk_cred *cred PARA(_ARG))

#define sec_define_seharmony_method(METHOD)					\
	sec_define_chk_method(METHOD, sec_chk_seharmony_##METHOD)

#define SEC_DEFINE_SEHARMONY_DEFAULT(NAME, _ARG)					\
static int _sec_chk_seharmony_##NAME(const struct sec_chk_cred *cred,		\
	const struct sec_cred_seharmony *sel_cred PARA(_ARG));			\
static int sec_chk_seharmony_##NAME(const struct sec_chk_cred *cred PARA(_ARG))	\
{										\
	int rc;									\
	struct sec_cred_seharmony *sel;					\
										\
	sel = sel_cred_of_sec_cred(cred);	\
	rc = _sec_chk_seharmony_##NAME(cred, sel VAR(_ARG));		\
	return rc;								\
}										\
static int _sec_chk_seharmony_##NAME(const struct sec_chk_cred *cred,		\
	const struct sec_cred_seharmony *sel_cred PARA(_ARG))

int sel_unload_policy(void);
int sel_load_policy(const void *data, size_t size, unsigned int service);
int sel_load_policy_remote(const void *data, size_t size, unsigned int service);
typedef void (*sel_load_policy_custom_ft)(void);
void sel_load_policy_custom(sel_load_policy_custom_ft fn, unsigned int command);
int hmsel_proc_avc_check(const struct sec_chk_cred *cred, hmsel_sid_t tsid,
		hmsel_class_t tclass, hmsel_av_t av_perm);
static inline int hmsel_proc_avc_check_self(const struct sec_chk_cred *cred,
		hmsel_class_t tclass, hmsel_av_t av_perm)
{
	return hmsel_proc_avc_check(cred, -1, tclass, av_perm); // -1 indicates using ssid as tsid
}
#endif
