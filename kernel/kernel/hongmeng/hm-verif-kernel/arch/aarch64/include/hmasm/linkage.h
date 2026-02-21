/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 7 03:10:16 2018
 */
#ifndef AARCH64_ASM_LINKAGE_H
#define AARCH64_ASM_LINKAGE_H

#ifdef __ASSEMBLY__
#include <hmkernel/compiler.h>

#ifndef KCFI_TAG
#error "KCFI_TAG needs to be pre-defined"
#endif

#define BEGIN_FUNC_NO_CFI(name) \
	.global name ; \
	.type name, %function ; \
	name:

#ifndef __hmbuild__
/* Use legacy kconfig option to define morden hmbuild feature macro */
# ifdef CONFIG_KCFI
#  ifndef __hmfeature_cfi__
#   define __hmfeature_cfi__
#  endif
# endif
#endif

#ifndef __hmfeature_cfi__
#define BEGIN_FUNC(name) BEGIN_FUNC_NO_CFI(name)
#else
#define BEGIN_FUNC(name) \
	.global name ; \
	.type name, %function ; \
	.long KCFI_TAG; \
	name:
#endif

#define END_FUNC(name) \
	.size name, .-name

#else
#error "Assembly-specific header is included in C code"
#endif

#endif
