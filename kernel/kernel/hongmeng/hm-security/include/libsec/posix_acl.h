/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Inner interfaces of POSIX access control list
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 23 05:50:25 2021
 */
#ifndef __POSIX_ACL_H
#define __POSIX_ACL_H

#include <stdbool.h>
#include <sys/types.h>
#include <libhmebr/ebr.h>
#include "posix_acl_def.h"

#define ACL_UNCACHED ((void *)(-1))

struct sec_cred_dac;
struct sec_chk_cred;
struct sec_posix_acl_obj {
	uid_t uid;
	gid_t gid;
	struct sec_posix_acl *acl;
};
struct sec_hm_acl_obj {
	uid_t uid;
	gid_t gid;
	uid_t cuid;
	gid_t cgid;
	struct sec_posix_acl *acl;
};

struct sec_posix_acl *sec_posix_acl_alloc(size_t entry_num);
int sec_posix_acl_check(const struct sec_chk_cred *cred,
			const struct sec_cred_dac *dac_cred,
			const struct sec_posix_acl_obj *obj,
			unsigned short mask);
int sec_hm_acl_check(const struct sec_chk_cred *cred,
		     const struct sec_cred_dac *dac_cred,
		     const struct sec_hm_acl_obj *obj,
		     unsigned short mask);
int sec_posix_acl_from_xattr(const char *xattr, size_t size, struct sec_posix_acl **acl);
ssize_t sec_posix_acl_to_xattr(const struct sec_posix_acl *acl, char *xattr, size_t size);
struct sec_posix_acl *sec_posix_acl_dup(const struct sec_posix_acl *acl);
void sec_posix_acl_free_ebr(struct sec_posix_acl *acl, struct epoch_set *epoch);
void sec_posix_acl_free(struct sec_posix_acl *acl);
int sec_posix_acl_get(struct sec_posix_acl *acl);
void sec_posix_acl_put(struct sec_posix_acl *acl);
void sec_posix_acl_put_ebr(struct sec_posix_acl *acl, struct epoch_set *epoch);
/*
 * from, to
 * true, true: fix acl and mode
 * true, false: acl <- mode
 * false, false: acl -> mode
 * false, false: invalid
 *
 * Only permission bits of modes may be modified.
 *
 * + SEC_POSIX_ACL_TRIVIAL: convert success and `acl` is equivalent with `mode`.
 * + E_HM_OK: convert success and `acl` is not equivalent with `mode`.
 * + E_HM_INVAL: unexpected arguments or combination of arguments.
 */
int sec_posix_acl_convert_mode(struct sec_posix_acl *acl, mode_t *mode,
			       bool from_mode, bool to_mode);

/* acl might be ACL_UNCACHED or sentinel */
static inline bool sec_posix_acl_is_not_init(const struct sec_posix_acl *acl)
{
	return (long)acl & 1;
}

static inline bool sec_posix_acl_is_invalid(const struct sec_posix_acl *acl)
{
	return (acl == NULL || acl == ACL_UNCACHED);
}
#endif /* ifndef __POSIX_ACL_H */
