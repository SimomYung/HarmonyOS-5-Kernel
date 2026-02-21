/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: Declaration of different layer of spincall
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 28 08:55:56 2025
 */

#ifndef HMKERNEL_CTRLMEM_SPINCALLENTRY_DECLARATION_H
#define HMKERNEL_CTRLMEM_SPINCALLENTRY_DECLARATION_H

#include <hmkernel/ctrlmem/spincallentry/naming.h>
#include <hmkernel/macro.h>

#define __SPINCALLENTRY_DECLARATION(rettype, funcname, args, attr) \
extern attr rettype SPINCALLENTRY_SYMNAME(funcname)(args); \
extern attr rettype SPINCALLENTRY_CFNNAME(funcname)(args);

#define SPINCALLENTRY_DECLARATION(rettype, funcname, protolist, attr)	\
	__SPINCALLENTRY_DECLARATION(rettype, funcname, __hmkernel_macro_proto_mkargs(protolist), attr)

#endif
