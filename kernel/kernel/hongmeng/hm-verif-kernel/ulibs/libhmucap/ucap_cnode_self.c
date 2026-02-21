/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: ops for ucap cnode self
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 14 14:24:56 2023
 */
#include <libhmucap/ucap.h>

#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <hongmeng/hongmeng.h>

static cref_t g_cnode_cref;

cref_t hm_ucap_self_cnode_cref(void)
{
	int err;
	cref_t cref = 0ULL;

	if (g_cnode_cref != 0ULL) {
		cref = g_cnode_cref;
	} else {
		err = syscap_CNodeCRef(0, &cref);
		if (err == E_HM_POSIX_FAULT) {
			/*
			 * with copy on write enabled, the memory region use by
			 * the static variable 'cref' may be set write
			 * protection, when our kernel write to this variable
			 * will get a page fault and return E_HM_POSIX_FAULT, if
			 * this happend, we write touch this variable to handle
			 * this page fault in userspace and try it again
			 */
			cref = 0;
			err = syscap_CNodeCRef(0, &cref);
		}
		if (err < 0) {
			cref = ERR_TO_REF(err);
		} else if (!hm_fetch_fork_allowd_flag()) {
			/* for not allow fork services, we can use g_cnode_cref
			 * cache
			 */
			g_cnode_cref = cref;
		} else {
			/* do nothing */
		}
	}

	return cref;
}

__u32 hm_ucap_self_cnode_idx(void)
{
	__u32 cnode;

	if (g_cnode_cref != 0ULL) {
		cnode = __REF_CNODE_IDX_OF(g_cnode_cref);
	} else {
		cref_t cref;
		cref = hm_ucap_self_cnode_cref();
		if (IS_REF_ERR(cref)) {
			cnode = err_to_cnode_idx((REF_TO_ERR(cref)));
		} else {
			cnode = __REF_CNODE_IDX_OF(cref);
		}
	}

	return cnode;
}
