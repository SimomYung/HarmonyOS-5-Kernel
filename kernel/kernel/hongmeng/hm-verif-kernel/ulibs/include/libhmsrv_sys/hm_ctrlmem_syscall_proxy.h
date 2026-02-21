/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Ctrlmem syscall proxy
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 29 15:07:11 2023
 */

#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_CTRLMEM_SYSCALLS_SYSCALL_PROXY_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_CTRLMEM_SYSCALLS_SYSCALL_PROXY_H

#include <libstrict/strict.h>

#define CTRLMEM_ARG_LIST0
#define CTRLMEM_ARG_LIST1	, unsigned long arg1
#define CTRLMEM_ARG_LIST2	CTRLMEM_ARG_LIST1, unsigned long arg2
#define CTRLMEM_ARG_LIST3	CTRLMEM_ARG_LIST2, unsigned long arg3
#define CTRLMEM_ARG_LIST4	CTRLMEM_ARG_LIST3, unsigned long arg4
#define CTRLMEM_ARG_LIST5	CTRLMEM_ARG_LIST4, unsigned long arg5
#define CTRLMEM_ARG_LIST6	CTRLMEM_ARG_LIST5, unsigned long arg6
#define CTRLMEM_ARG_LIST7	CTRLMEM_ARG_LIST6, unsigned long arg7

#define CTRLMEM_PARAM_LIST0
#define CTRLMEM_PARAM_LIST1	, arg1
#define CTRLMEM_PARAM_LIST2	CTRLMEM_PARAM_LIST1, arg2
#define CTRLMEM_PARAM_LIST3	CTRLMEM_PARAM_LIST2, arg3
#define CTRLMEM_PARAM_LIST4	CTRLMEM_PARAM_LIST3, arg4
#define CTRLMEM_PARAM_LIST5	CTRLMEM_PARAM_LIST4, arg5
#define CTRLMEM_PARAM_LIST6	CTRLMEM_PARAM_LIST5, arg6
#define CTRLMEM_PARAM_LIST7	CTRLMEM_PARAM_LIST6, arg7

#define define_ctrlmem_syscall_proxy_group(x) \
static inline int sysproxy_ctrlmem_write_r32_v32_arg##x(unsigned long callno, \
							void *dest, unsigned long v CTRLMEM_ARG_LIST##x) \
{ \
	UNUSED(callno); \
	return sysfast___ctrlmem_write_r32_v32_arg##x(dest, v CTRLMEM_PARAM_LIST##x); \
} \
static inline int sysproxy_ctrlmem_write_r32_v64_arg##x(unsigned long callno, \
							void *dest, unsigned long v_lo, \
							unsigned long v_hi CTRLMEM_ARG_LIST##x) \
{ \
	UNUSED(callno); \
	return sysfast___ctrlmem_write_r32_v64_arg##x(dest, v_lo, v_hi CTRLMEM_PARAM_LIST##x); \
} \
static inline int sysproxy_ctrlmem_write_r64_v32_arg##x(unsigned long callno, \
							void *dest, unsigned long v CTRLMEM_ARG_LIST##x) \
{ \
	UNUSED(callno); \
	return sysfast___ctrlmem_write_r64_v32_arg##x(dest, v CTRLMEM_PARAM_LIST##x); \
} \
static inline int sysproxy_ctrlmem_write_r64_v64_arg##x(unsigned long callno, \
							void *dest, unsigned long v CTRLMEM_ARG_LIST##x) \
{ \
	UNUSED(callno); \
	return sysfast___ctrlmem_write_r64_v64_arg##x(dest, v CTRLMEM_PARAM_LIST##x); \
} \
static inline int sysproxy_ctrlmem_cas_r32_v32_arg##x(unsigned long callno, \
						      void *dest, unsigned long ori_v, \
						      unsigned long v CTRLMEM_ARG_LIST##x) \
{ \
	UNUSED(callno); \
	return sysfast___ctrlmem_cas_r32_v32_arg##x(dest, ori_v, v CTRLMEM_PARAM_LIST##x); \
} \
static inline int sysproxy_ctrlmem_cas_r32_v64_arg##x(unsigned long callno, \
						      void *dest, unsigned long ori_v_lo, unsigned long ori_v_hi, \
						      unsigned long v_lo, unsigned long v_hi CTRLMEM_ARG_LIST##x) \
{ \
	UNUSED(callno); \
	return sysfast___ctrlmem_cas_r32_v64_arg##x(dest, ori_v_lo, ori_v_hi, v_lo, v_hi CTRLMEM_PARAM_LIST##x); \
} \
static inline int sysproxy_ctrlmem_cas_r64_v32_arg##x(unsigned long callno, \
						      void *dest, unsigned long ori_v, \
						      unsigned long v CTRLMEM_ARG_LIST##x) \
{ \
	UNUSED(callno); \
	return sysfast___ctrlmem_cas_r64_v32_arg##x(dest, ori_v, v CTRLMEM_PARAM_LIST##x); \
} \
static inline int sysproxy_ctrlmem_cas_r64_v64_arg##x(unsigned long callno, \
						      void *dest, unsigned long ori_v, \
						      unsigned long v CTRLMEM_ARG_LIST##x) \
{ \
	UNUSED(callno); \
	return sysfast___ctrlmem_cas_r64_v64_arg##x(dest, ori_v, v CTRLMEM_PARAM_LIST##x); \
}

define_ctrlmem_syscall_proxy_group(0)
define_ctrlmem_syscall_proxy_group(1)
define_ctrlmem_syscall_proxy_group(2)
define_ctrlmem_syscall_proxy_group(3)
define_ctrlmem_syscall_proxy_group(4)
define_ctrlmem_syscall_proxy_group(5)

#endif
