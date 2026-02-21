/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: Declaration of spincalls
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 28 09:15:40 2025
 */

#ifndef HMKERNEL_CTRLMEM_SPINCALLENTRY_H
#define HMKERNEL_CTRLMEM_SPINCALLENTRY_H

#include <hmkernel/compiler.h>
#include <hmkernel/ctrlmem/spincallentry/declaration.h>

#define __SPINCALLENTRY_DESC(rettype, funcname, protolist, attr, need_irqsave) \
	SPINCALLENTRY_DECLARATION(rettype, funcname, __hmkernel_macro_args(protolist), attr)

#define SPINCALLENTRY_DESC(rettype, funcname, ...) \
	__SPINCALLENTRY_DESC(rettype, funcname, __hmkernel_macro_args(__VA_ARGS__), , y)

#include <hmkernel/ctrlmem/spincalls_list.h>
#ifdef CONFIG_SPINHDLR_ENABLED
#include <hmkernel/ctrlmem/spinhdlr_spincalls_list.h>
#endif

#undef __SPINCALLENTRY_DESC
#undef SPINCALLENTRY_DESC

#endif
