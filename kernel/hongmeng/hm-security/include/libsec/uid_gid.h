/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Interface for uid & gid
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 12 14:35:21 2023
 */

#ifndef ULIBS_LIBSEC_UID_GID_H
#define ULIBS_LIBSEC_UID_GID_H

#include <libsec/ns_usr.h>
#include <libhmsrv_sys/hm_security.h>

#define INVALID_UID ((uid_t)-1)
#define INVALID_GID ((gid_t)-1)
#define DEFAULT_KERNEL_OVERFLOWUID 65534
#define DEFAULT_KERNEL_OVERFLOWGID 65534
#define MAX_UGID_VAL 65535

static inline bool valid_uid(uid_t uid)
{
	return uid != INVALID_UID;
}

static inline bool valid_gid(gid_t gid)
{
	return gid != INVALID_GID;
}

#ifdef CONFIG_USER_NS

extern uid_t convert_to_global_uid(struct usr_ns_cred *usrns_cred, uid_t inner_uid);
extern gid_t convert_to_global_gid(struct usr_ns_cred *usrns_cred, gid_t inner_gid);
extern uid_t convert_to_inner_uid(struct usr_ns_cred *usrns_cred, uid_t global_uid);
extern gid_t convert_to_inner_gid(struct usr_ns_cred *usrns_cred, gid_t global_gid);
extern uid_t convert_to_inner_uid_munged(struct usr_ns_cred *usrns_cred, uid_t global_uid);
extern gid_t convert_to_inner_gid_munged(struct usr_ns_cred *usrns_cred, uid_t global_gid);
extern uint32_t map_id_range_down(struct ugid_map *map, uint32_t id, uint32_t count);

#else

static inline uid_t convert_to_global_uid(struct usr_ns_cred *usrns_cred, uid_t inner_uid)
{
	UNUSED(usrns_cred);
	return inner_uid;
}

static inline gid_t convert_to_global_gid(struct usr_ns_cred *usrns_cred, gid_t inner_gid)
{
	UNUSED(usrns_cred);
	return inner_gid;
}

static inline uid_t convert_to_inner_uid(struct usr_ns_cred *usrns_cred, uid_t global_uid)
{
	UNUSED(usrns_cred);
	return global_uid;
}

static inline gid_t convert_to_inner_gid(struct usr_ns_cred *usrns_cred, gid_t global_gid)
{
	UNUSED(usrns_cred);
	return global_gid;
}

static inline uid_t convert_to_inner_uid_munged(struct usr_ns_cred *usrns_cred, uid_t global_uid)
{
	uid_t uid = convert_to_inner_uid(usrns_cred, global_uid);
	if (uid == (uid_t)-1) {
		uid = *hm_sec_shconf_kernel_overflowuid();
	}
	return uid;
}

static inline gid_t convert_to_inner_gid_munged(struct usr_ns_cred *usrns_cred, uid_t global_gid)
{
	gid_t gid = convert_to_inner_gid(usrns_cred, global_gid);
	if (gid == (gid_t)-1) {
		gid = *hm_sec_shconf_kernel_overflowgid();
	}
	return gid;
}

#endif

static inline bool global_uid_has_mapping(struct usr_ns_cred *usrns_cred, uid_t global_uid)
{
	return convert_to_inner_uid(usrns_cred, global_uid) != (uid_t)-1;
}

static inline bool global_gid_has_mapping(struct usr_ns_cred *usrns_cred, gid_t global_gid)
{
	return convert_to_inner_gid(usrns_cred, global_gid) != (gid_t)-1;
}

#endif
