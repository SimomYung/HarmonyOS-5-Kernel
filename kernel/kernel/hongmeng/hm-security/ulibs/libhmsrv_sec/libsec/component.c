/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Secure component functions
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:27:30 2019
 */
#include "libsec/component.h"

#include <stddef.h>

#include <hongmeng/errno.h>

#include "libsec/chk.h"

int sec_comp_init(struct sec_comp const *comp, const struct sec_server_env *env)
{
	int ret = E_HM_OK;

	if (comp == NULL || comp->mod == NULL) {
		hm_error("param is invalid\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK && comp->ops.init != NULL) {
		ret = comp->ops.init(env);
	}

	return ret;
}

void sec_comp_fini(struct sec_comp const *comp)
{
	if (comp != NULL &&
	    comp->ops.fini != NULL) {
		comp->ops.fini();
	}
}

void sec_add_method(struct sec_chk_method *method, size_t num, bool high_priority)
{
	size_t i;

	if (method != NULL) {
		/*
		 * Loop termination condition:
		 * Following loop will terminate when i reach num. Since i is monotonic, keep increasing
		 * in loop body, it has an upper bound.
		 * Loop condition is constant and reasonable: num is the count of method, which is limited,
		 * so loop condition is reasonable.
		 */
		for (i = 0; i < num; i++) {
			if (sec_method_is_dummy(&method[i])) {
				continue;
			}
			if (high_priority) {
				dlist_insert(method[i].hook.head, &method[i].hook.node);
			} else {
				dlist_insert_tail(method[i].hook.head, &method[i].hook.node);
			}
		}
	}
}

void sec_del_method(struct sec_chk_method *method, size_t num)
{
	size_t i;

	if (method != NULL) {
		/*
		 * Loop termination condition:
		 * Following loop will terminate when i reach num. Since i is monotonic, keep increasing
		 * in loop body, it has an upper bound.
		 * Loop condition is constant and reasonable: num is the count of method, which is limited,
		 * so loop condition is reasonable.
		 */
		for (i = 0; i < num; i++) {
			if (sec_method_is_dummy(&method[i])) {
				continue;
			}
			dlist_delete(&method[i].hook.node);
		}
	}
}
