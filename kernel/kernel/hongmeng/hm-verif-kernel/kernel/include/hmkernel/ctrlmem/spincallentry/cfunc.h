/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: Implement spincall body
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 28 11:29:14 2025
 */

#ifndef HMKERNEL_CTRLMEM_SPINCALLENTRY_CFUNC_H
#define HMKERNEL_CTRLMEM_SPINCALLENTRY_CFUNC_H

#include <hmkernel/ctrlmem/spincallentry/naming.h>
#include <hmkernel/ctrlmem/spincallentry.h>
#include <hmkernel/compiler.h>

#define __spincallentry_cfunc_invoke_isvoid_y(rettype, varname, invokebody) invokebody
#define __spincallentry_cfunc_invoke_isvoid_n(rettype, varname, invokebody) rettype varname = invokebody;

#define ______spincallentry_cfunc_invoke(rettype, isvoid, varname, invokebody) \
	__spincallentry_cfunc_invoke_isvoid_##isvoid(rettype, varname, invokebody)

#define ____spincallentry_cfunc_invoke(rettype, isvoid, varname, invokebody) \
	______spincallentry_cfunc_invoke(rettype, isvoid, varname, invokebody)

#define __spincallentry_cfunc_invoke(rettype, varname, invokebody) \
	____spincallentry_cfunc_invoke(rettype, __hmkernel_macro_is_void(rettype), varname, invokebody)

#define __spincallentry_cfunc_return_isvoid_y(varname) do { } while(0)
#define __spincallentry_cfunc_return_isvoid_n(varname) return varname

#define _____spincallentry_cfunc_return(isvoid, varname)\
	__spincallentry_cfunc_return_isvoid_##isvoid(varname)

#define ____spincallentry_cfunc_return(isvoid, varname) \
	_____spincallentry_cfunc_return(isvoid, varname)

#define __spincallentry_cfunc_return(rettype, varname) \
	____spincallentry_cfunc_return(__hmkernel_macro_is_void(rettype), varname)

#define ____SPINCALLENTRY_CFUNC(rettype, funcname, args, prms, attr) \
static inline __always_inline attr rettype SPINCALLENTRY_BDYNAME(funcname)(args); \
attr rettype SPINCALLENTRY_CFNNAME(funcname)(args) \
{ \
	__spincallentry_cfunc_invoke(rettype, r, SPINCALLENTRY_BDYNAME(funcname)(prms)); \
	__spincallentry_cfunc_return(rettype, r); \
} \
static inline __always_inline attr rettype SPINCALLENTRY_BDYNAME(funcname)(args)

#define __SPINCALLENTRY_CFUNC(rettype, funcname, protolist, attr) \
	____SPINCALLENTRY_CFUNC(rettype, funcname, __hmkernel_macro_proto_mkargs(protolist), \
						   __hmkernel_macro_proto_mkprms(protolist), attr)

#define SPINCALLENTRY_CFUNC(rettype, funcname, ...) \
	__SPINCALLENTRY_CFUNC(rettype, funcname, __hmkernel_macro_args(__VA_ARGS__), )

#endif
