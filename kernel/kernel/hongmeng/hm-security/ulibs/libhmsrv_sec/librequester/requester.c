/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Requester frame functions
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 31 18:01:24 2019
 */
/* direct header */
#include "librequester/requester.h"

/* libc */
#include <stdlib.h>

/* hongmeng ulibs */
#include <libhmlog/hmlog.h>
#include <libsec/cred.h>
#include <hongmeng/syscall.h>
#include <libhmactv/actv.h>

static inline void requester_assign_cred_idx(struct requester *req, cred_idx_t fwd_idx, cred_idx_t src_idx)
{
#ifdef ENABLE_PAC_HARD_CODE
	req->fwd_cred_idx = fwd_idx;
	req->src_cred_idx = src_idx;
#else
	UNUSED(req, fwd_idx, src_idx);
#endif
}

static int __requester_assign_cred(struct requester *req, bool sysif_self_call)
{
	int err = E_HM_OK;
	cred_idx_t src_idx, fwd_idx;
	__u64 tmp_idx = 0;
	void *sh_cred;
	void *b_cred;

	if (unlikely(sysif_self_call)) {
		err = sec_sysfast_actv_cred(&tmp_idx);
		if (err != E_HM_OK || tmp_idx == 0) {
			hm_error("self cred idx is invalid \n");
			return E_HM_INVAL;
		}

		/* if from selfcall , src must be self */
		sec_cred_idx_to_cred((cred_idx_t)tmp_idx,
				     &b_cred, &sh_cred);
		req->src_base_cred = b_cred;
		req->src_sh_cred = sh_cred;
		if (sysif_callinfo.cred_flags.selfcall_fwd_use_self == SEC_CRED_SELFCALL_FWD_USE_SELF) {
			req->fwd_base_cred = req->src_base_cred;
			req->fwd_sh_cred = req->src_sh_cred;
			requester_assign_cred_idx(req, (cred_idx_t)tmp_idx, (cred_idx_t)tmp_idx);
		} else {
			/* if fwd is not self , get cred idx from local area */
			fwd_idx = (cred_idx_t)hm_actv_local_actv_cred_of();
			if (fwd_idx == 0) {
				hm_error("fwd idx is invalid \n");
				return E_HM_INVAL;
			}
			sec_cred_idx_to_cred(fwd_idx,
					     &b_cred, &sh_cred);
			req->fwd_base_cred = b_cred;
			req->fwd_sh_cred = sh_cred;
			requester_assign_cred_idx(req, fwd_idx, (cred_idx_t)tmp_idx);
		}
	} else {
		fwd_idx = (cred_idx_t)hm_actv_local_actv_cred_of();
		src_idx = (cred_idx_t)hm_actv_local_actv_xcred_of();
		if (fwd_idx == 0) {
			hm_error("fwd idx is invalid \n");
			return E_HM_INVAL;
		}
		sec_cred_idx_to_fwd_cred(fwd_idx,
					 &b_cred, &sh_cred);
		req->fwd_base_cred = b_cred;
		req->fwd_sh_cred = sh_cred;
		if (src_idx == 0) {
			hm_error("src idx is invalid \n");
			return E_HM_INVAL;
		}
		req->src_cred_idx = src_idx;
#ifdef ENABLE_PAC_HARD_CODE
		req->fwd_cred_idx = fwd_idx;
#endif
	}

	return err;
}

static inline int __requester_assign_cred_simple(struct requester *req)
{
	cred_idx_t fwd_idx = (cred_idx_t)hm_actv_local_actv_cred_of();
	if (fwd_idx == 0) {
		hm_error("fwd idx is invalid \n");
		return E_HM_INVAL;
	}
	void *fwd_base_cred = req->fwd_base_cred;
	sec_cred_idx_to_fwd_cred_simple(fwd_idx, &fwd_base_cred);
	req->fwd_base_cred = fwd_base_cred;
#ifdef ENABLE_PAC_HARD_CODE
	req->fwd_cred_idx = fwd_idx;
#endif

	return E_HM_OK;
}

static inline int __requester_assign_cred_simple_src(struct requester *req)
{
	cred_idx_t src_idx = (cred_idx_t)hm_actv_local_actv_xcred_of();
	if (src_idx == 0) {
		hm_error("src idx is invalid \n");
		return E_HM_INVAL;
	}
	void *sh_cred = req->src_sh_cred;
	void *b_cred = req->src_base_cred;
	sec_cred_idx_to_src_cred_simple(src_idx, &b_cred, &sh_cred);
	req->src_sh_cred = sh_cred;
	req->src_base_cred = b_cred;

#ifdef ENABLE_PAC_HARD_CODE
	req->src_cred_idx = src_idx;
#endif
	return E_HM_OK;
}

int requester_init(struct requester *req)
{
	int err = E_HM_OK;
	struct requester_frame *req_frame = requester_current_frame();
	uint32_t cnode_idx;

	/* workque or sec init context get from requester_frame */
	if (requester_frame_is_avail(req_frame)) {
		*req = req_frame->requester;
	} else {
		/* get cred info from tls var on ebb or actv context */
		struct sec_cred *cred_info = sysif_actv_current_callinfo();
		if ((cred_info->selfcall_type == __SEC_CRED_ACTV_SELFCALL) && (cred_info->sender == 0)) {
			cnode_idx = hm_ucap_self_cnode_idx();
			if (is_cnode_idx_valid(cnode_idx) == 0) {
				hm_error("failed to get self cnode index, cnode_idx=%d\n", cnode_idx_to_err(cnode_idx));
				err = cnode_idx_to_err(cnode_idx);
			}
			if (err == E_HM_OK) {
				req->src_cnode_idx = cnode_idx;
				req->fwd_cnode_idx = cnode_idx;
				err = __requester_assign_cred(req, true);
			}
		} else {
			req->src_cnode_idx = sysif_actv_src_cnode_idx();
			req->fwd_cnode_idx = sysif_actv_fwd_cnode_idx();
			err = __requester_assign_cred(req,
						      (cred_info->selfcall_type == __SEC_CRED_ACTV_SELFCALL));
		}
	}
	return err;
}

inline int requester_init_simple(struct requester *req)
{
	return  __requester_assign_cred_simple(req);
}

inline int requester_init_simple_src(struct requester *req)
{
	return  __requester_assign_cred_simple_src(req);
}

__thread struct requester_current_frame t_requester_current_frame;

int requester_frame_set_init_frame(void)
{
	uint32_t cnode_idx;
	struct requester_frame *req = NULL;
	int err = E_HM_OK;

	cnode_idx = hm_ucap_self_cnode_idx();
	if (is_cnode_idx_valid(cnode_idx) == 0) {
		hm_error("get cnode index failed. (err_message=%s)\n",
			 hmstrerror(cnode_idx_to_err(cnode_idx)));
		err = cnode_idx_to_err(cnode_idx);
	}
	if (err == E_HM_OK) {
		req = malloc(sizeof(*req));
		if (req == NULL) {
			hm_error("failed to allocate memory\n");
			err = E_HM_NOMEM;
		}
		if (err == E_HM_OK) {
			mem_zero_s(*req);
			req->requester.src_cnode_idx = cnode_idx;
			req->requester.fwd_cnode_idx = cnode_idx;

			/* security mac_init used */
			req->requester_state = REQUESTER_INIT;

			err = __requester_assign_cred(&req->requester, true);
			if (err == E_HM_OK) {
				(void)requester_frame_replace(req);
			} else {
				free(req);
			}
		}
	}

	return err;
}

void requester_frame_rst_init_frame(void)
{
	free(requester_frame_replace(NULL));
}

static int assign_spec_cred(struct requester *req, struct sec_local_cred *cred)
{
	if (cred == NULL) {
		return E_HM_INVAL;
	}
	req->fwd_base_cred = &cred->fwd_base_cred;
	req->fwd_sh_cred = &cred->fwd_sh_cred;
	req->fwd_cnode_idx = cred->fwd_cnode_idx;
	req->src_base_cred = &cred->src_base_cred;
	req->src_sh_cred = &cred->src_sh_cred;
	req->src_cnode_idx = cred->src_cnode_idx;

#ifdef ENABLE_PAC_HARD_CODE
	req->fwd_cred_idx = cred->fwd_cred_idx;
	req->src_cred_idx = cred->src_cred_idx;
	cred->local_mem_flag = LOCAL_CRED_FLAG;
	req->local_mem_flag = LOCAL_CRED_FLAG;

	hm_pacga_bcred_mem(req->fwd_base_cred);
	hm_pacga_shcred_mem(req->fwd_sh_cred);

	hm_pacga_bcred_mem(req->src_base_cred);
	hm_pacga_shcred_mem(req->src_sh_cred);
#endif
	return E_HM_OK;
}

int requester_assign_cred_with_spec(struct requester *req, enum cred_source_type cred_s, struct sec_local_cred *cred)
{
	int err = E_HM_OK;
	cred_idx_t src_idx, fwd_idx;
	__u64 tmp_idx;
	void *sh_cred;
	void *b_cred;
	switch (cred_s) {
	case CRED_SOURCE_FROM_SELF:
		err = sec_sysfast_actv_cred(&tmp_idx);
		if ((err != E_HM_OK || tmp_idx == 0)) {
			hm_error("self cred idx is invalid\n");
			return E_HM_INVAL;
		}
		sec_cred_idx_to_cred((cred_idx_t)tmp_idx,
				     &b_cred, &sh_cred);
		req->src_base_cred = b_cred;
		req->src_sh_cred = sh_cred;
		req->fwd_base_cred = req->src_base_cred;
		req->fwd_sh_cred = req->src_sh_cred;
		requester_assign_cred_idx(req, (cred_idx_t)tmp_idx, (cred_idx_t)tmp_idx);
		break;
	case CRED_SOURCE_POP_SRC:
		src_idx = (cred_idx_t)hm_actv_local_actv_xcred_of();
		if (src_idx == 0) {
			hm_error("src idx is invalid \n");
			return E_HM_INVAL;
		}
		sec_cred_idx_to_cred(src_idx,
				     &b_cred, &sh_cred);
		req->src_base_cred = b_cred;
		req->src_sh_cred = sh_cred;
		req->fwd_base_cred = req->src_base_cred;
		req->fwd_sh_cred = req->src_sh_cred;
		requester_assign_cred_idx(req, src_idx, src_idx);
		break;
	case CRED_SOURCE_POP_FWD:
		fwd_idx = (cred_idx_t)hm_actv_local_actv_cred_of();
		if (fwd_idx == 0) {
			hm_error("fwd idx is invalid \n");
			return E_HM_INVAL;
		}
		sec_cred_idx_to_cred(fwd_idx,
					     &b_cred, &sh_cred);
		req->fwd_base_cred = b_cred;
		req->fwd_sh_cred = sh_cred;
		req->src_base_cred = req->fwd_base_cred;
		req->src_sh_cred = req->fwd_sh_cred;
		requester_assign_cred_idx(req, fwd_idx, fwd_idx);
		break;
	case CRED_SOURCE_FROM_CALLER:
		err = __requester_assign_cred(req, false);
		break;
	case CRED_SOURCE_SPEC_CRED:
		err = assign_spec_cred(req, cred);
		break;
	default:
		err = E_HM_INVAL;
	}
	return err;
}

int requester_assign_cred(struct requester *req, enum cred_source_type cred_s)
{
	return requester_assign_cred_with_spec(req, cred_s, NULL);
}
