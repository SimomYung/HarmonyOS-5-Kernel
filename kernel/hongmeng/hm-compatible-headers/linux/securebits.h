/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/securebits.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 19:16:49 2021
 */

#ifndef LINUX_SECUREBITS_H
#define LINUX_SECUREBITS_H

#define issecure_mask(X)	(1 << (X))

#define SECURE_NOROOT			0
#define SECURE_NOROOT_LOCKED		1

#define SECBIT_NOROOT			(issecure_mask(SECURE_NOROOT))
#define SECBIT_NOROOT_LOCKED		(issecure_mask(SECURE_NOROOT_LOCKED))

#define SECURE_NO_SETUID_FIXUP		2
#define SECURE_NO_SETUID_FIXUP_LOCKED	3

#define SECBIT_NO_SETUID_FIXUP		(issecure_mask(SECURE_NO_SETUID_FIXUP))
#define SECBIT_NO_SETUID_FIXUP_LOCKED	(issecure_mask(SECURE_NO_SETUID_FIXUP_LOCKED))

#define SECURE_KEEP_CAPS		4
#define SECURE_KEEP_CAPS_LOCKED		5

#define SECBIT_KEEP_CAPS		(issecure_mask(SECURE_KEEP_CAPS))
#define SECBIT_KEEP_CAPS_LOCKED		(issecure_mask(SECURE_KEEP_CAPS_LOCKED))

#endif
