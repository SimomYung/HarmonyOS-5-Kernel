/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: kernel integrity protection for sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 14 21:00:44 2023
 */

#ifndef INTAPI_HKIPSYS_HKIPSYS_H
#define INTAPI_HKIPSYS_HKIPSYS_H

#include <hmkernel/ctrlmem/sysconf.h>
#include <libstrict/strict.h>

extern struct __ctrlmem_sysconf_page_s* g_ctrlmem_sysconf_page;

#ifdef CONFIG_HKIP_PROTECT_SYSMGR
int ctrlmem_sysconf_page_set(struct __ctrlmem_sysconf_page_s* g_addr);
bool hkip_sys_is_enabled(void);
void hkip_sys_get_sec_region_range(uint64_t *start, uint64_t *end);
/* hkip protect apis */
int hkip_sys_protect_ro(const void *dst, unsigned long size);
int hkip_sys_protect_rox(const void *dst, unsigned long size);
int hkip_sys_protect_rowm(const void *dst, unsigned long size);
int hkip_sys_protect_privileged_text(void);

#ifdef CONFIG_HKIP_SUPPORT_REVOKE
int hkip_sys_protect_ro_revocable(const void *dst, unsigned long size);
int hkip_sys_protect_ro_revoke(const void *dst, unsigned long size);
int hkip_sys_protect_rowm_revocable(const void *dst, unsigned long size);
int hkip_sys_protect_rowm_revoke(const void *dst, unsigned long size);
#endif

/* hkip write apis */
int hkip_sys_rowm_cpy(const void *dst, const void *src, size_t size);
int hkip_sys_rowm_set(const void *dst, uint8_t val, size_t size);

int sec_region_sys_protect_sysmgr(const void *dst, unsigned long size);
int sec_region_sys_protect_services(void);
int sec_region_sys_disable_forbidden(void);
#else
static inline int ctrlmem_sysconf_page_set(struct __ctrlmem_sysconf_page_s* g_addr)
{
	UNUSED(g_addr);
	return 0;
}

static inline bool hkip_sys_is_enabled(void)
{
	return false;
}

static inline void hkip_sys_get_sec_region_range(uint64_t *start, uint64_t end)
{
	UNUSED(start, end);
}

static inline int hkip_sys_protect_ro(const void *dst, unsigned long size)
{
	UNUSED(dst, size);
	return 0;
}

static inline int hkip_sys_protect_ro_revocable(const void *dst, unsigned long size)
{
	UNUSED(dst, size);
	return 0;
}

static inline int hkip_sys_protect_ro_revoke(const void *dst, unsigned long size)
{
	UNUSED(dst, size);
	return 0;
}

static inline int hkip_sys_protect_rowm(const void *dst, unsigned long size)
{
	UNUSED(dst, size);
	return 0;
}

static inline int hkip_sys_protect_rowm_revocable(const void *dst, unsigned long size)
{
	UNUSED(dst, size);
	return 0;
}

static inline int hkip_sys_protect_rowm_revoke(const void *dst, unsigned long size)
{
	UNUSED(dst, size);
	return 0;
}

static inline int hkip_sys_protect_rox(const void *dst, unsigned long size)
{
	UNUSED(dst, size);
	return 0;
}
static inline int hkip_sys_protect_privileged_text(void)
{
	return 0;
}

static inline int hkip_sys_rowm_cpy(const void *dst, const void *src, size_t size)
{
	UNUSED(dst, size);
	return 0;
}

static inline int hkip_sys_rowm_set(const void *dst, uint8_t val, size_t size)
{
	UNUSED(dst, size);
	return 0;
}


static inline int sec_region_sys_protect_sysmgr(const void *dst, unsigned long size)
{
	UNUSED(dst, size);
	return 0;
}
static inline int sec_region_sys_protect_services(void)
{
	return 0;
}

static inline int sec_region_sys_disable_forbidden(void)
{
	return 0;
}
#endif
#endif
