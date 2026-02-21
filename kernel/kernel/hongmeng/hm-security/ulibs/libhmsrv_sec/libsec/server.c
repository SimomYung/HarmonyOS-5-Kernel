/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Secure server functions
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:27:35 2019
 */

#include "libsec/server.h"

#include <hongmeng/errno.h>
#include <hmkernel/const.h>
#include "libsec/cred.h"
#include "libsec/module.h"
#include "libsec/component.h"

static inline int server_init_check(struct sec_mod * const *mod, size_t mod_num,
				    struct sec_comp * const *comp, size_t comp_num,
				    const struct sec_server_attr *attr)
{
	int ret = E_HM_OK;
	if ((mod_num != 0u && mod == NULL) ||
	    (comp_num != 0u && comp == NULL) || attr == NULL) {
		ret = E_HM_INVAL;
	}
	return ret;
}

int sec_server_init_with_env(struct sec_mod **mod, size_t mod_num,
			     struct sec_comp **comp, size_t comp_num,
			     const struct sec_server_attr *attr)
{
	int ret;
	size_t mod_i = 0;
	size_t comp_i = 0;

	ret = server_init_check(mod, mod_num, comp, comp_num, attr);
	if (ret != E_HM_OK) {
		hm_error("invalid parameters\n");
	}

	if (ret == E_HM_OK) {
		ret = sec_cred_init(attr->is_creator);
		if (ret != E_HM_OK) {
			hm_error("failed to initialize security credential, err=%s\n", hmstrerror(ret));
		}
	}

	if (ret == E_HM_OK) {
		/*
		 * Loop termination condition:
		 * Following loop terminate when mod_i is mod_num, Since mod_i keep increasing
		 * in loop body, it has a upper bound.
		 * Loop condition is constant and reasonable:
		 * mod_num is less than 4 according to sec_mod, so the loop condition is reasonable.
		 */
		for (mod_i = 0; mod_i < mod_num; mod_i++) {
			ret = sec_mod_init(mod[mod_i], attr->env);
			if (ret < 0) {
				hm_error("failed to initialize security module, mod=%d, err=%s\n",
					 (int)mod_i, hmstrerror(ret));
				goto mod_fini;
			}
		}

		/*
		 * Loop termination condition:
		 * Following loop terminate when comp_i is comp_num, Since comp_i keep increasing
		 * in loop body, it has a upper bound.
		 * Loop condition is constant and reasonable:
		 * comp_num is less than 35 according to sec_comp, so the loop condition is reasonable.
		 */
		for (comp_i = 0; comp_i < comp_num; comp_i++) {
			ret = sec_comp_init(comp[comp_i], attr->env);
			if (ret < 0) {
				hm_error("failed to initialize security components, comp=%d, err=%s\n",
					 (int)comp_i, hmstrerror(ret));
				goto comp_fini;
			}
		}
	}

	goto exit;
comp_fini:
	/*
	 * Loop termination condition:
	 * Following loop terminate when comp_i is 0, Since comp_i keep decreasing
	 * in loop body, it has a lower bound.
	 * Loop condition is constant and reasonable: comp_i is less than comp_num,
	 * comp_num is less than 35 according to sec_comp, so the loop condition is reasonable.
	 */
	while (comp_i > 0U) {
		sec_comp_fini(comp[--comp_i]);
	}
mod_fini:
	/*
	 * Loop termination condition:
	 * Following loop terminate when mod_i is 0, Since mod_i keep decreasing
	 * in loop body, it has a lower bound.
	 * Loop condition is constant and reasonable: mod_i is less than mod_num,
	 * mod_num is less than 4 according to sec_mod, so the loop condition is reasonable.
	 */
	while (mod_i > 0U) {
		sec_mod_fini(mod[--mod_i]);
	}
	sec_cred_fini();
exit:
	return ret;
}

int sec_server_init(struct sec_mod **mod, size_t mod_num,
		    struct sec_comp **comp, size_t comp_num, bool is_creator)
{
	int ret;
	const struct sec_server_attr attr = {
		.is_creator = is_creator,
		.env = NULL
	};
	ret = sec_server_init_with_env(mod, mod_num, comp, comp_num, &attr);
	if (ret != E_HM_OK)
		return ret;
	return ret;
}

void sec_server_fini(struct sec_mod * const *mod, size_t mod_num,
		     struct sec_comp * const *comp, size_t comp_num)
{
	size_t i;

	sec_cred_fini();

	if (comp != NULL) {
		/*
		 * Loop termination condition:
		 * Following loop terminate when i reach 0, Since i keep decreasing
		 * in loop body, it has a upper bound.
		 * Loop condition is constant and reasonable:
		 * comp_num is less than 35 according to sec_comp, so the loop condition is reasonable.
		 */
		i = comp_num;
		while (i > __U(0)) {
			sec_comp_fini(comp[--i]);
		}
	}
	if (mod != NULL) {
		/*
		 * Loop termination condition:
		 * Following loop terminate when i reach 0, Since i keep decreasing
		 * in loop body, it has a upper bound.
		 * Loop condition is constant and reasonable:
		 * mod_num is less than 4 according to sec_mod, so the loop condition is reasonable.
		 */
		i = mod_num;
		while (i > __U(0)) {
			sec_mod_fini(mod[--i]);
		}
	}
}
