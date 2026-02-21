/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Common structures and interfaces of audit module
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 25 11:38:55 2020
 */
#ifndef ULIBS_LIBSEC_AUDIT_H
#define ULIBS_LIBSEC_AUDIT_H

#include <stdbool.h>
#include <stdint.h>
#include <libshlist/shlist.h>

#include "module.h"
#include "cred.h"
#include "method/para.h"
#include "method/var.h"

extern struct sec_mod sec_mod_audit;

#define SEC_DEFINE_AUDIT(NAME, ARG)						\
static int _sec_chk_audit_##NAME(const struct sec_chk_cred *cred,		\
			       const struct sec_cred_audit *audit_cred PARA(ARG));	\
static int sec_chk_audit_##NAME(const struct sec_chk_cred *cred PARA(ARG))	\
{										\
	struct sec_cred_audit *audit;						\
	int rc;									\
	audit = audit_cred_of_sec_cred(cred);					\
	return _sec_chk_audit_##NAME(cred, audit VAR(ARG));			\
}										\
static int _sec_chk_audit_##NAME(const struct sec_chk_cred *cred,		\
				 const struct sec_cred_audit *audit_cred PARA(ARG))

#define sec_define_audit_method(METHOD)						\
	sec_define_chk_method(METHOD, sec_chk_audit_##METHOD)

#if defined(__HOST_UT__)
struct str_cred *sec_audit_str_alloc(size_t size);
#else
static struct str_cred *sec_audit_str_alloc(size_t size)
{
	int ret = E_HM_OK;
	cred_idx_t node_idx, str_idx;

	struct str_cred *str_node = sec_str_cred_alloc(&node_idx);
	if (str_node == NULL) {
	       ret = E_HM_NOMEM;
	} else if (size != 0u) {
		char *pstr = NULL;
		ret = sec_cred_alloc(size, &str_idx);
		if (ret == E_HM_OK) {
			pstr = (char *)sec_cred_idx_to_ptr(str_idx);
			if (pstr == NULL) {
				sec_cred_free(str_idx);
				ret = E_HM_NOMEM;
			}
		}
		if (ret == E_HM_OK) {
			str_node->size = size;
			str_node->pstr = pstr;
			str_node->str_idx = str_idx;
		} else {
			sec_str_cred_put(str_node);
		}
	} else {
		/* do nothing */
	}
	return ret == E_HM_OK ? str_node : NULL;
}
#endif

static inline cmdline_node_t *sec_audit_cmdline_node_alloc(size_t size)
{
	return sec_audit_str_alloc(size);
}

static inline tty_node_t *sec_audit_tty_node_alloc(size_t size)
{
	return sec_audit_str_alloc(size);
}

static inline void sec_audit_cmdline_node_get(cmdline_node_t *cmdline_node)
{
	sec_str_cred_get(cmdline_node);
}

static inline void sec_audit_tty_node_get(tty_node_t *tty_node)
{
	sec_str_cred_get(tty_node);
}

static inline void sec_audit_cmdline_node_put(cmdline_node_t *cmdline_node)
{
	sec_str_cred_put(cmdline_node);
}

static inline void sec_audit_tty_node_put(tty_node_t *tty_node)
{
	sec_str_cred_put(tty_node);
}

#endif /* ifndef ULIBS_LIBSEC_AUDIT_H */
