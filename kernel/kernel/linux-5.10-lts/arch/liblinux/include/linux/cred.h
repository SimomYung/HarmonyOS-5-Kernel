/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef __LIBLINUX_LINUX_CRED_H__
#define __LIBLINUX_LINUX_CRED_H__

#include_next <linux/cred.h>
#undef current_cred

extern void liblinux_get_cred_dac(struct cred **cred);
extern struct cred init_cred;
static inline const struct cred * current_cred(void) {
	if (!(current->flags & PF_KTHREAD) && (current->cred == &init_cred)) {
		struct cred *cred = NULL;

		liblinux_get_cred_dac(&cred);
		current->cred = cred;
	}
	return rcu_dereference_protected(current->cred, 1);
}

#endif /* __LIBLINUX_LINUX_CRED_H__ */
