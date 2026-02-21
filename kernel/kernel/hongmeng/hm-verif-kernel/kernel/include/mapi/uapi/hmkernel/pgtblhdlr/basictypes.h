/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Allow subclass inject basic type to pgtblhdlr
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 23 09:58:07 2022
 */

#ifndef MAPI_UAPI_HMKERNEL_PGTBLHDLR_BASICTYPES_H
#define MAPI_UAPI_HMKERNEL_PGTBLHDLR_BASICTYPES_H

#ifndef __PGTBLHDLR_GENERATOR
# error mapi/uapi/basictypes.h should not be used standalone.
#endif

#include <hmkernel/types.h>
#include <hmkernel/const.h>
#include <hmkernel/compiler.h>
#include <hmkernel/pgtblhdlr/naming.h>

#ifndef __PGTBLHDLR_BASICTYPES_KEY
# error pgtblhdlr basictypes.h requires __PGTBLHDLR_BASICTYPES_KEY
#endif

#ifndef __PGTBLHDLR_BASICTYPES_VAL
# error pgtblhdlr basictypes.h requires __PGTBLHDLR_BASICTYPES_VAL
#endif

#ifndef __PGTBLHDLR_BASICTYPES_INVAL
# error pgtblhdlr basictypes.h requires __PGTBLHDLR_BASICTYPES_INVAL
#endif

/*
 * Do not require __PGTBLHDLR_BASICTYPES_KEY_INVAL because key is always
 * unsigned int compatible type and invalid key is always (__PGTBLHDLR_BASICTYPES_KEY)(0ul)
 */

/*
 * No need to undef following macros because they are usually be
 * defined to same thing. Although expanded into different code,
 * GCC would not warn when multiple #define do same same for a macro.
 */
#define ____TYPE_KEY	__PGTBLHDLR_BASICTYPES_KEY
#define ____KEY_MAX	__PGTBLHDLR_BASICTYPES_KEY_MAX
#define ____TYPE_VAL	__PGTBLHDLR_BASICTYPES_VAL
#define ____VAL_INVAL	__PGTBLHDLR_BASICTYPES_INVAL

/* Undefine guard: this header should be included multiple times and never used standalone. */
#undef MAPI_UAPI_HMKERNEL_PGTBLHDLR_BASICTYPES_H

#endif
