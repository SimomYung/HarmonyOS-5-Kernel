/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation of ulibs actv read/write vm interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 20 11:43:11 2023
 */

#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libsysif/utils.h>
#include <libhmactv/actv.h>
#include <libmem/utils.h>
#include <hmasm/page.h>

static bool xact_owner_is_myself(void)
{
	return sysif_actv_fwd_cnode_idx() == hm_ucap_self_cnode_idx();
}

static bool caller_is_myself(void)
{
	return sysif_actv_src_cnode_idx() == hm_ucap_self_cnode_idx();
}

/*
 * wrapper for safe_copy, transform posix errno to hm errno.
 */
static int __safe_copy(void *dst, const void *src, unsigned long size)
{
	int ret;

	ret = safe_copy(dst, src, size);
	if (ret < 0) {
		ret = E_HM_FAULT;
	}

	return ret;
}

/*
 * wrapper for safe_strncpy, transform posix errno to hm errno.
 */
static int __safe_strncpy(char *dst, const char *src, unsigned long size)
{
	int ret;

	ret = safe_strncpy(dst, src, size);
	if (ret < 0) {
		ret = E_HM_FAULT;
	}

	return ret;
}

/*
 * hm_actv_read_xact_vm - read data from @src of current transaction owner
 *			  vspace to @dst of current vspace.
 *
 * @dst: address of current vspace
 * @src: address of transaction owner vspace
 * @size: copy size
 */
int hm_actv_read_xact_vm(void *dst, const void *src, unsigned long size)
{
	if (xact_owner_is_myself()) {
		return __safe_copy(dst, src, size);
	} else {
		return sysfast_rpc_read_xact_vm(dst, src, size);
	}
}

/*
 * hm_actv_write_xact_vm - write data from @src of current vspace to @dst of
 *			   transaction owner vspace.
 *
 * @dst: address of transaction owner vspace
 * @src: address of current vspace
 * @size: copy size
 */
int hm_actv_write_xact_vm(void *dst, const void *src, unsigned long size)
{
	if (xact_owner_is_myself()) {
		return __safe_copy(dst, src, size);
	} else {
		return sysfast_rpc_write_xact_vm(dst, src, size);
	}
}

/*
 * hm_actv_zero_xact_vm - set zero to @dst of transaction owner vspace.
 *
 * @dst: address of transaction owner vspace
 * @size: set zero size, allow max size: PAGE_SIZE
 */
int hm_actv_zero_xact_vm(void *dst, unsigned long size)
{
	int err = E_HM_OK;
	if (size > PAGE_SIZE) {
		return E_HM_TOOMANY;
	}

	if (xact_owner_is_myself()) {
		mem_zero_b(dst, size);
	} else {
		err = sysfast_rpc_zero_xact_vm(dst, size);
	}

	return err;
}

/*
 * hm_actv_copy_in_xact_vm - write data from @src to @dst both of
 *				transaction owner vspace.
 *
 * @dst: address of transaction owner vspace
 * @src: address of transaction owner vspace
 * @size: copy size
 */
int hm_actv_copy_in_xact_vm(void *dst, const void *src, unsigned long size)
{
	if (xact_owner_is_myself()) {
		return __safe_copy(dst, src, size);
	} else {
		return sysfast_rpc_copy_in_xact_vm(dst, src, size);
	}
}

/*
 * hm_actv_strcpy_xact_vm - read string from @src of current transaction owner
 *			  vspace to @dst of current vspace.
 *
 * @dst: address of current vspace
 * @src: address of transaction owner vspace
 * @size: max dst buffer size
 */
int hm_actv_strcpy_xact_vm(void *dst, const void *src, unsigned long size)
{
	int ret = E_HM_OK;
	char *dst_str = void_to_ptr(dst, char);

	if (xact_owner_is_myself()) {
		ret = __safe_strncpy(dst, src, size);
	} else {
		ret = sysfast_rpc_strcpy_xact_vm(dst, src, size);
	}
	/* reserved for end of string */
	if (size > 0) {
		dst_str[size - 1] = '\0';
	}
	return ret;
}

int hm_actv_read_caller_vm(void *dst, const void *src, unsigned long size)
{
#ifdef __HOST_LLT__
	/*
	 * The `errno` is positive number if error occurs in our sec lib, make
	 * the behaviour same as the native syscall, i.e., indicate failer using
	 * negative number.
	 */
	return (memcpy_s(dst, size, src, size) == 0) ? 0 : E_HM_POSIX_FAULT;
#else
	if (caller_is_myself()) {
		return __safe_copy(dst, src, size);
	} else {
		return sysfast_rpc_read_caller_vm(dst, src, size);
	}
#endif
}

int hm_actv_write_caller_vm(void *dst, const void *src, unsigned long size)
{
	if (caller_is_myself()) {
		return __safe_copy(dst, src, size);
	} else {
		return sysfast_rpc_write_caller_vm(dst, src, size);
	}
}

/*
 * hm_actv_strcpy_caller_vm - read string from @src of caller's vspace to @dst of current vspace.
 *
 * @dst: address of current vspace
 * @src: address of caller's vspace
 * @size: max dst buffer size
 */
int hm_actv_strcpy_caller_vm(void *dst, const void *src, unsigned long size)
{
	int ret = E_HM_OK;
	char *dst_str = void_to_ptr(dst, char);

	if (caller_is_myself()) {
		ret = __safe_strncpy(dst_str, (const char *)src, size);
	} else {
		ret = sysfast_rpc_strcpy_caller_vm(dst, src, size);
	}
	/* reserved for end of string */
	if (size > 0) {
		dst_str[size - 1] = '\0';
	}
	return ret;
}
