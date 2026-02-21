/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/audit.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 11 22:41:28 2021
 */

#ifndef LINUX_AUDIT_H
#define LINUX_AUDIT_H

#include <libhmsrv_audit/audit.h>
#include <linux/elf-em.h>

#define AUDIT_ARCH_I386		(EM_386|__AUDIT_ARCH_LE)
#define AUDIT_ARCH_PPC		(EM_PPC)
#define AUDIT_ARCH_PPC64	(EM_PPC64|__AUDIT_ARCH_64BIT)
#define AUDIT_ARCH_PPC64LE	(EM_PPC64|__AUDIT_ARCH_64BIT|__AUDIT_ARCH_LE)
#define AUDIT_ARCH_S390		(EM_S390)
#define AUDIT_ARCH_S390X	(EM_S390|__AUDIT_ARCH_64BIT)
#define AUDIT_ARCH_X86_64	(EM_X86_64|__AUDIT_ARCH_64BIT|__AUDIT_ARCH_LE)

#endif

