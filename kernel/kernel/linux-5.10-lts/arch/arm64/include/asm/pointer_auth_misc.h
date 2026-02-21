// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 */

#ifndef __POINTER_AUTH_MISC_H
#define __POINTER_AUTH_MISC_H

#include <linux/irqflags.h>
#ifdef CONFIG_ARM64_PTR_AUTH_EXT
#include <asm/pointer_auth_common.h>
#endif
#ifdef CONFIG_ARM64_PTR_AUTH_DATA_FIELD
#include <asm/pointer_auth_context.h>
#include <asm/pointer_auth_data.h>
#endif

#ifndef CONFIG_ARM64_PTR_AUTH_DATA_FIELD

#define resign_compat_exception_context_start(regs)
#define resign_compat_exception_context_end(regs)

#define resign_exception_context_start(regs)
#define resign_exception_context_end(regs)

#define sign_exception_context_start(regs)
#define sign_exception_context_end(regs)

#endif

#endif
