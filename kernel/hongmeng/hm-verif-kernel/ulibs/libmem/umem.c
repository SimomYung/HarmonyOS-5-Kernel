/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Privilged services access uapps memory headers on SPIN.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 5 10:33:25 2020
 */
#include <string.h>
#include <limits.h>

#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <hmkernel/errno.h>
#include <libsysif/sysmgr/api.h>

#include <libmem/utils.h>
#include <libmem/umem.h>
#include <libhmactv/actv.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsync/raw_thread.h>

static uint32_t g_spin_flags __ro_after_init = 0U;
static uintptr_t g_spin_priv_start __ro_after_init = 0UL;

static inline bool enable_safe_copy(void)
{
	return ((g_spin_flags & SPIN_MEM_SAFE_COPY) != 0U);
}

/* will provide to privilege service in spin mode. */
void hm_setup_spin_flags(void)
{
	int ret;

	BUG_ON(g_spin_flags != 0U);

	if (g_spin_priv_start == 0UL) {
		ret = hm_sysctrl_read_conf(__SYSCONF_PRIVILEGE_VA_START,
					   (void *)&g_spin_priv_start,
					   (unsigned int)sizeof(g_spin_priv_start));
		BUG_ON(ret < 0, "hm_sysctrl_read_conf failed, %s\n", hmstrerror(ret));
	}

	/*
	 * If any address of this process is larger than privilege_va_start,
	 * this process is a privilege process,
	 * so safe_copy can be enable in copy_from/to_ser
	 */
	if (((uintptr_t)&g_spin_priv_start) >= g_spin_priv_start) {
		g_spin_flags = SPIN_MEM_SAFE_COPY;
	}
}

#ifdef CONFIG_HARDENED_USERCOPY
static inline bool addr_is_legal_stack_addr(const void *addr, const size_t size)
{
	struct raw_thread *thread = raw_thread_self();
	unsigned long stack_base, sp;
	bool is_legal_addr = false;

	sp = __RPC_CURR_SP;
	stack_base = raw_thread_stack_base_of(thread);

	is_legal_addr = !hm_addr_cross_border((uintptr_t)addr, size, sp,
			stack_base, true);

	return is_legal_addr;
}

#ifdef __hmbuild_target_is_static__
__attribute__((__weak__)) bool hm_addr_clear_of_text(const void *addr,
		const size_t size)
{
	UNUSED(addr);
	UNUSED(size);

	return true;
}
#else
static hm_addr_clear_of_text_t __addr_clear_of_text;

void hm_register_addr_clear_of_text_check_func(hm_addr_clear_of_text_t func)
{
	__addr_clear_of_text = func;
}

static inline bool hm_addr_clear_of_text(const void *addr, const size_t size)
{
	bool ret = true;

	if (__addr_clear_of_text != NULL) {
		ret = __addr_clear_of_text(addr, size);
	}

	return ret;
}
#endif /* __hmbuild_target_is_static__ */
#endif /* CONFIG_HARDENED_USERCOPY */

static bool is_legal_copy_addr(const void *addr, const size_t size)
{
#ifdef CONFIG_HARDENED_USERCOPY
	bool is_legal_addr = true;

	if (ptr_to_ulong(addr) > ULONG_MAX - size) {
		is_legal_addr = false;
	}

	if (is_legal_addr) {
		is_legal_addr = addr_is_legal_stack_addr(addr, size);
	}

	if (is_legal_addr) {
		is_legal_addr = hm_addr_clear_of_text(addr, size);
	}

	return is_legal_addr;
#else
	UNUSED(addr);
	UNUSED(size);
	return true;
#endif
}

int hm_copy_to_caller(void *dst_ori, const void *src_ori, size_t bytes)
{
	int ret = E_HM_POSIX_FAULT;
	void *dst = untagged_addr(dst_ori);
	const void *src = untagged_addr(src_ori);

	ret = hm_actv_write_caller_vm(dst, src, bytes);

	return ret;
}

int hm_copy_from_caller(void *dst_ori, const void *src_ori, size_t bytes)
{
	int ret = E_HM_POSIX_FAULT;
	void *dst = untagged_addr(dst_ori);
	const void *src = untagged_addr(src_ori);

	ret = hm_actv_read_caller_vm(dst, src, bytes);
	return ret;
}

int hm_copy_to_user(void *dst_ori, const void *src_ori, size_t bytes)
{
	int ret = E_HM_POSIX_FAULT;
	void *dst = untagged_addr(dst_ori);
	const void *src = untagged_addr(src_ori);

	if (!is_legal_copy_addr(src, bytes)) {
		hm_panic("Copying to user with illegal address!\n");
	}

	if (enable_safe_copy()) {
		if ((ptr_to_ulong(dst) < ((size_t)ULONG_MAX - bytes)) &&
			(ptr_to_ulong(dst) + bytes) < g_spin_priv_start) {
			/* safe_copy will return -EFAULT only if failed */
			ret = safe_copy_with_pan(dst, src, bytes);
			if (ret < 0) {
				ret = posix2hmerrno(-ret);
			}
		}
	}

	if (ret != E_HM_OK) {
		ret = hm_actv_write_xact_vm(dst, src, bytes);
	}

	return ret;
}

int hm_copy_from_user(void *dst_ori, const void *src_ori, size_t bytes)
{
	int ret = E_HM_POSIX_FAULT;
	void *dst = untagged_addr(dst_ori);
	const void *src = untagged_addr(src_ori);

	if (!is_legal_copy_addr(dst, bytes)) {
		hm_panic("Copying from user with illegal address!\n");
	}

	if (enable_safe_copy()) {
		if ((ptr_to_ulong(src) < ((size_t)ULONG_MAX - bytes)) &&
			(ptr_to_ulong(src) + bytes) < g_spin_priv_start) {
			/* safe_copy will return -EFAULT only if failed */
			ret = safe_copy_with_pan(dst, src, bytes);
			if (ret < 0) {
				ret = posix2hmerrno(-ret);
			}
		}
	}

	if (ret != E_HM_OK) {
		ret = hm_actv_read_xact_vm(dst, src, bytes);
	}
	return ret;
}

int hm_umem_get_user_pages(bool is_fwd, uintptr_t vaddr, size_t len,
			   unsigned long long *ret_pa_array, size_t array_size)
{
	int rc;

	rc = actvxactcall_hmcall_mem_devhost_get_user_pages(is_fwd, false, vaddr, len,
							    ret_pa_array, array_size);
	return rc;
}

int hm_umem_put_user_pages(bool is_fwd, uintptr_t vaddr, size_t len,
			   unsigned long long *ret_pa_array, size_t array_size)
{
	int rc;

	rc = actvxactcall_hmcall_mem_devhost_put_user_pages(is_fwd, false, vaddr, len,
							    ret_pa_array, array_size);
	return rc;
}

int hm_umem_charge_mlock(unsigned long mlock_size)
{
	return actvxactcall_hmcall_mem_devhost_charge_mlock(true, true, mlock_size);
}

int hm_umem_uncharge_mlock(unsigned long mlock_size)
{
	return actvxactcall_hmcall_mem_devhost_uncharge_mlock(true, true, mlock_size);
}
