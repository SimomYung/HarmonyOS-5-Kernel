/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: kernel integrity protection for hm
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 19 21:00:44 2023
 */

#include <hmkernel/errno.h>
#include <libhmsrv_sys/hm_hkip.h>

#if defined(CONFIG_DEBUG_BUILD) && defined(CONFIG_HKIP)

int hm_hkip_protect_ro(const void *dst, size_t size)
{
	return actvcall_hmcall_hkip_sys_protect(__HKIP_RO_REGISTER, dst, size);
}

int hm_hkip_protect_ro_revocable(const void *dst, size_t size)
{
	return actvcall_hmcall_hkip_sys_protect(__HKIP_RO_REGISTER_REVOCABLE, dst, size);
}

int hm_hkip_protect_ro_revoke(const void *dst, size_t size)
{
	return actvcall_hmcall_hkip_sys_protect(__HKIP_RO_REVOKE, dst, size);
}

int hm_hkip_protect_rowm(const void *dst, size_t size)
{
	return actvcall_hmcall_hkip_sys_protect(__HKIP_ROWM_REGISTER, dst, size);
}

int hm_hkip_protect_rowm_revocable(const void *dst, size_t size)
{
	return actvcall_hmcall_hkip_sys_protect(__HKIP_ROWM_REGISTER_REVOCABLE, dst, size);
}

int hm_hkip_protect_rowm_revoke(const void *dst, size_t size)
{
	return actvcall_hmcall_hkip_sys_protect(__HKIP_ROWM_REVOKE, dst, size);
}

int hm_hkip_protect_rox(const void *dst, size_t size)
{
	return actvcall_hmcall_hkip_sys_protect_rox(dst, size);
}
#else

int hm_hkip_protect_ro(const void *dst, size_t size)
{
	UNUSED(dst, size);
	return E_HM_NOSYS;
}
int hm_hkip_protect_ro_revocable(const void *dst, size_t size)
{
	UNUSED(dst, size);
	return E_HM_NOSYS;
}

int hm_hkip_protect_ro_revoke(const void *dst, size_t size)
{
	UNUSED(dst, size);
	return E_HM_NOSYS;
}

int hm_hkip_protect_rox(const void *dst, size_t size)
{
	UNUSED(dst, size);
	return E_HM_NOSYS;
}

int hm_hkip_protect_rowm(const void *dst, size_t size)
{
	UNUSED(dst, size);
	return E_HM_NOSYS;
}

int hm_hkip_protect_rowm_revocable(const void *dst, size_t size)
{
	UNUSED(dst, size);
	return E_HM_NOSYS;
}

int hm_hkip_protect_rowm_revoke(const void *dst, size_t size)
{
	UNUSED(dst, size);
	return E_HM_NOSYS;
}

#endif

#ifdef CONFIG_HKIP

int hm_hkip_rowm_cpy(const void *dst, const void *src, size_t size)
{
	return actvcall_hmcall_hkip_sys_rowm_cpy(dst, src, size);
}

int hm_hkip_rowm_set(const void *dst, uint8_t val, size_t size)
{
	return actvcall_hmcall_hkip_sys_rowm_set(dst, val, size);
}

int hm_hkip_get_status(void)
{
	return actvcall_hmcall_hkip_get_status();
}

int hm_hkip_protect_proc(void)
{
	return actvcall_hmcall_hkip_protect_proc();
}
#else

int hm_hkip_rowm_cpy(const void *dst, const void *src, size_t size)
{
	UNUSED(dst, src, size);
	return E_HM_NOSYS;
}

int hm_hkip_rowm_set(const void *dst, uint8_t val, size_t size)
{
	UNUSED(dst, val, size);
	return E_HM_NOSYS;
}

int hm_hkip_get_status(void)
{
	return E_HM_NOSYS;
}

int hm_hkip_protect_proc(void)
{
	return E_HM_NOSYS;
}
#endif

#ifdef CONFIG_SYSMGR_FAULT_INJECT
int hm_hkip_write_evil_data(const void *vaddr, int value)
{
	return actvcall_hmcall_test_hkip_write_evil_data(vaddr, value);
}
#endif
