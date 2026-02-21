/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Compiler.h for uapi
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 21 18:01:50 2019
 */

#ifndef AARCH64_UAPI_ASM_COMPILER_H
#define AARCH64_UAPI_ASM_COMPILER_H

#ifndef __clang__
# define __opt_noframe		__attribute__((optimize("omit-frame-pointer")))
# define __opt_nonoframe	__attribute__((optimize("no-omit-frame-pointer")))
# define __opt_nostkprot	__attribute__((optimize("no-stack-protector")))
# define __internal_rpccall
# define __internal_rpccapcall
# define __internal_actvcall	__opt_noframe __opt_nostkprot __attribute__((weak))
#else
#if defined(__BISHENG__) || defined (__hongmeng__)
# define __opt_noframe		__attribute__((optimize("omit-frame-pointer")))
# define __opt_nonoframe	__attribute__((optimize("no-omit-frame-pointer")))
#else
# define __opt_noframe
# define __opt_nonoframe
#endif
# define __opt_nostkprot	__attribute__((no_stack_protector))
# define __internal_rpccall	__opt_noframe __opt_nostkprot
# define __internal_rpccapcall	__opt_noframe __opt_nostkprot
# define __internal_actvcall	__opt_nostkprot __attribute__((optnone)) __attribute__((weak))
#endif

#endif
