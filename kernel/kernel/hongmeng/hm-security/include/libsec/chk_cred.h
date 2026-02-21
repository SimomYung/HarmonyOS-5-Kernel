/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Secure check credential head file
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 30 15:05:25 2019
 */
#ifndef ULIBS_LIBSEC_CHK_CRED_H
#define ULIBS_LIBSEC_CHK_CRED_H

#include <librequester/requester.h>
#include "cred_common.h"

struct sec_chk_cred {
	uint32_t cnode_idx;
	/* cred is sh_cred */
	__sec_data(libsec, ptr) void *cred;
	__sec_data(libsec, ptr) void *base_cred;
	cred_idx_t cred_idx;
	uint32_t local_mem_flag;
};

#define SEC_CHK_CRED_RESET { 0 }

static inline void sec_chk_cred_init(struct sec_chk_cred *cred,
				    uint32_t cnode_idx)
{
	cred->cnode_idx = cnode_idx;
}

static inline void sec_chk_cocred_init(struct sec_chk_cred *cred,
				      uint32_t cnode_idx, struct requester *req)
{
	cred->cnode_idx = cnode_idx;
	cred->cred = req->fwd_sh_cred;
	cred->base_cred = req->fwd_base_cred;
#ifdef ENABLE_PAC_HARD_CODE
	cred->cred_idx = req->fwd_cred_idx;
	cred->local_mem_flag = req->local_mem_flag;
#endif
}

static inline void sec_chk_cocred_init_src(struct sec_chk_cred *src_cred,
					   uint32_t cnode_idx, struct requester *req)
{
	src_cred->cnode_idx = cnode_idx;
	src_cred->cred = req->src_sh_cred;
	src_cred->base_cred = req->src_base_cred;
#ifdef ENABLE_PAC_HARD_CODE
	src_cred->cred_idx = req->src_cred_idx;
	src_cred->local_mem_flag = req->local_mem_flag;
#endif
}

static inline void sec_chk_cocred_init_with_cred(struct sec_chk_cred *cred,
						uint32_t cnode_idx, void *sh_cred, void *base_cred,
						cred_idx_t cred_idx)
{
	cred->cnode_idx = cnode_idx;
	if (base_cred != NULL) {
		cred->cred = sh_cred;
		cred->base_cred = base_cred;
	} else {
		void *share_cred = cred->cred;
		void *b_cred = cred->base_cred;
		/* only src cred use cache when init xact cred */
		sec_cred_idx_to_src_cred(cred_idx, &b_cred, &share_cred);
		cred->base_cred = b_cred;
		cred->cred = share_cred;
	}
#ifdef ENABLE_PAC_HARD_CODE
	cred->cred_idx = cred_idx;
#endif
}


int sec_chk_cred_init_requester(struct sec_chk_cred *cred);

int sec_chk_cred_init_requester_simple(struct sec_chk_cred *cred);
int sec_chk_src_cred_init_requester_simple(struct sec_chk_cred *src_cred);

struct sec_chk_cred_xact {
	struct requester _req;
	struct sec_chk_cred _src_cred;
	struct sec_chk_cred _fwd_cred;
};

#define SEC_CHK_CRED_XACT_RESET {			\
		._req = {0},				\
		._src_cred = SEC_CHK_CRED_RESET,	\
		._fwd_cred = SEC_CHK_CRED_RESET,	\
	}

static inline int sec_chk_cred_xact_init(struct sec_chk_cred_xact *cred)
{
	struct sec_chk_cred_xact c = SEC_CHK_CRED_XACT_RESET;
	int rc;

	rc = requester_init(&c._req);
	if (rc == E_HM_OK) {
		*cred = c;
	}

	return rc;
}

static inline
struct sec_chk_cred *_sec_chk_cred_xact_init(struct sec_chk_cred *cred,
					     uint32_t cnode_idx, void *sh_cred, void *base_cred,
					     cred_idx_t cred_idx)
{
	struct sec_chk_cred *ret = cred;
	/* we need not initialize the credential again */
	if (is_cnode_idx_valid(cred->cnode_idx) == 0) {
		sec_chk_cocred_init_with_cred(cred, cnode_idx, sh_cred, base_cred, cred_idx);
	}
	return ret;
}

static inline
struct sec_chk_cred *sec_chk_fwd_cred(struct sec_chk_cred_xact *cred)
{
#ifdef ENABLE_PAC_HARD_CODE
	cred->_fwd_cred.local_mem_flag = cred->_req.local_mem_flag;
#endif
	return _sec_chk_cred_xact_init(&cred->_fwd_cred,
				       cred->_req.fwd_cnode_idx, cred->_req.fwd_sh_cred,
				       cred->_req.fwd_base_cred, cred->_req.fwd_cred_idx);
}

static inline
struct sec_chk_cred *sec_chk_src_cred(struct sec_chk_cred_xact *cred)
{
	struct sec_chk_cred *ret = NULL;
	/* only use one credential for performance if we can */
	if (cred->_req.src_cnode_idx == cred->_req.fwd_cnode_idx) {
		ret = sec_chk_fwd_cred(cred);
	} else {
		ret = _sec_chk_cred_xact_init(&cred->_src_cred,
					      cred->_req.src_cnode_idx, cred->_req.src_sh_cred,
					      cred->_req.src_base_cred, cred->_req.src_cred_idx);
	}
#ifdef ENABLE_PAC_HARD_CODE
	cred->_src_cred.local_mem_flag = cred->_req.local_mem_flag;
#endif
	return ret;
}

#endif
