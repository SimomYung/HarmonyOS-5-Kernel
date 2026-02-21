/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Define the seharmony security fields in struct
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 28 09:47:37 PM CST 2023
 */
#ifndef __DEVHOST_SECURITY_SEHARMONY_H__
#define __DEVHOST_SECURITY_SEHARMONY_H__

#include <libsec/cred.h>
#include <vsync/atomic.h>

/**
 * fsid: file description sid
 * vsid: sid of associated vnode at open-time
*/
struct dh_file_sel {
	hmsel_sid_t fsid;
	hmsel_sid_t vsid;
};

struct dh_file_sec {
	struct dh_file_sel sel;
	struct device_info_sec *device_info_sec;
};

/**
 * inode initialized state, synchronized from vfs sec_inode_sel_state
 * DEVICE_INFO_SEL_INVALID: device_info seharmony info invalid,
 * DEVICE_INFO_SEL_UNINITIALIZED: device_info seharmony info has been initialized but sid is invalid,
 * DEVICE_INFO_SEL_INITIALIZED: sid is valid.
*/
enum devinfo_sel_state {
	DEVICE_INFO_SEL_INVALID,
	DEVICE_INFO_SEL_UNINITIALIZED,
	DEVICE_INFO_SEL_INITIALIZED,
};

/**
 * devinfo: use to relabel sid
*/
struct device_info_sel {
	vatomic32_t sid;
	hmsel_sid_t task_sid;
	hmsel_class_t sclass;
	enum devinfo_sel_state initialized;
};

struct device_info_sec {
	struct device_info_sel sel;
};

static inline void devinfo_cred_set_sid(struct device_info_sec *devinfo_cred, hmsel_sid_t sid)
{
	vatomic32_write_rlx(&devinfo_cred->sel.sid, sid);
}

static inline hmsel_sid_t devinfo_cred_read_sid(struct device_info_sec *devinfo_cred)
{
	return (hmsel_sid_t)vatomic32_read_rlx(&devinfo_cred->sel.sid);
}

int devhost_seharmony_common_checker(int callno, ...);

#define devhost_seharmony_check(sysif_name, ...) \
	devhost_seharmony_common_checker(__devhost_method_##sysif_name, ##__VA_ARGS__)

#endif /* __DEVHOST_SECURITY_SEHARMONY_H__ */
