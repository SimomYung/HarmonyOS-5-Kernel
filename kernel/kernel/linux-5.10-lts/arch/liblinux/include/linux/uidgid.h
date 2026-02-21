/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBLINUX_UIDGID_H__
#define __LIBLINUX_UIDGID_H__

#define __kuid_val ____kuid_val
#define __kgid_val ____kgid_val
#define from_kuid __from_kuid
#define from_kuid_munged __from_kuid_munged
#define uid_gt _uid_gt
#define uid_lt _uid_lt
#define uid_gte _uid_gte
#define uid_lte _uid_lte
#define gid_gte _gid_gte
#define gid_lte _gid_lte
#include_next <linux/uidgid.h>
#undef __kuid_val
#undef __kgid_val
#undef from_kuid
#undef from_kuid_munged
#undef uid_gt
#undef uid_lt
#undef uid_gte
#undef uid_lte
#undef gid_gte
#undef gid_lte

static inline uid_t __kuid_val(kuid_t uid)
{
	return uid.val;
}

static inline gid_t __kgid_val(kgid_t gid)
{
	return gid.val;
}

static inline bool uid_gt(kuid_t left, kuid_t right)
{
	return __kuid_val(left) > __kuid_val(right);
}
 
static inline bool uid_gte(kuid_t left, kuid_t right)
{
	return __kuid_val(left) >= __kuid_val(right);
}
 
static inline bool uid_lt(kuid_t left, kuid_t right)
{
	return __kuid_val(left) < __kuid_val(right);
}
 
static inline bool uid_lte(kuid_t left, kuid_t right)
{
	return __kuid_val(left) <= __kuid_val(right);
}
 
static inline bool gid_gte(kgid_t left, kgid_t right)
{
	return __kgid_val(left) >= __kgid_val(right);
}
 
static inline bool gid_lte(kgid_t left, kgid_t right)
{
	return __kgid_val(left) <= __kgid_val(right);
}

static inline uid_t from_kuid(struct user_namespace *to, kuid_t kuid)
{
	return __kuid_val(kuid);
}

static inline uid_t from_kuid_munged(struct user_namespace *to, kuid_t kuid)
{
	uid_t uid = from_kuid(to, kuid);
	if (uid == (uid_t)-1)
		uid = overflowuid;
	return uid;
}
#endif	/* __LIBLINUX_UIDGID_H__ */
