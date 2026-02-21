/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes for devhost security APIs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#ifndef __DEVHOST_API_SECURITY_H__
#define __DEVHOST_API_SECURITY_H__

/*
 * CAUSION: This file will be used in linux source,
 *          **DO NOT** include any HMOS headers.
 */

/* for `current` */
int devhost_current_pid(void);
int devhost_current_pgid(void);

/* for procmgr */
int devhost_mysid(void);
int devhost_getsid(int pid);
int devhost_getpgid(int pid);
int devhost_getfgpgrp(int sid);
int devhost_setfgpgrp(int sid, int pgrp);
int devhost_session_set_ttyinfo(int sid, const char *name, unsigned int devno);

/* for caller security check */
enum dh_ability {
	DH_ABILITY_TTY = 0,
	DH_ABILITY_SERIAL_TOGGLE = 1,
	DH_ABILITY_NR
};

enum dh_capability {
	DH_CAP_ADMIN = 0,
	DH_CAP_TTY_CONFIG = 1,
	DH_CAP_NR
};

int devhost_caller_has_ability(int ability);
int devhost_caller_has_capability(int cap);
int devhost_caller_has_ab_and_cap(int ability, int cap);

#endif /* __DEVHOST_API_SECURITY_H__ */
