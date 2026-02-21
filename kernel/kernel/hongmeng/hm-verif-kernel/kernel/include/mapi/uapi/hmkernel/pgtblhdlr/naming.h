/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Naming convension of pgtblhdlr
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 22 19:27:54 2021
 */

#ifndef MAPI_UAPI_KERNEL_PGTBLHDLR_NAMING_H
#define MAPI_UAPI_KERNEL_PGTBLHDLR_NAMING_H

#ifndef __PGTBLHDLR_NAMING_COMMON_PREFIX_LOWERCASE
# error pgtblhdlr naming.h requires __PGTBLHDLR_NAMING_COMMON_PREFIX_LOWERCASE
#endif

#ifndef __PGTBLHDLR_NAMING_COMMON_PREFIX_UPPERCASE
# error pgtblhdlr naming.h requires __PGTBLHDLR_NAMING_COMMON_PREFIX_UPPERCASE
#endif

#define ______PGTBLHDLR_NAMING_LOWERCASE(basename, commonprefix) \
	__pgtblhdlr_##commonprefix##basename
#define ____PGTBLHDLR_NAMING_LOWERCASE(basename, commonprefix) \
	______PGTBLHDLR_NAMING_LOWERCASE(basename, commonprefix)
#define __PGTBLHDLR_NAMING_LOWERCASE(basename) \
	____PGTBLHDLR_NAMING_LOWERCASE(basename, __PGTBLHDLR_NAMING_COMMON_PREFIX_LOWERCASE)

#define ______PGTBLHDLR_NAMING_UPPERCASE(basename, commonprefix) \
	__PGTBLHDLR_##commonprefix##basename
#define ____PGTBLHDLR_NAMING_UPPERCASE(basename, commonprefix) \
	______PGTBLHDLR_NAMING_UPPERCASE(basename, commonprefix)
#define __PGTBLHDLR_NAMING_UPPERCASE(basename) \
	____PGTBLHDLR_NAMING_UPPERCASE(basename, __PGTBLHDLR_NAMING_COMMON_PREFIX_UPPERCASE)

#endif
