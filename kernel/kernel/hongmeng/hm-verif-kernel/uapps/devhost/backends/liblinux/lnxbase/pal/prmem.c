/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Pal for seharmony
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 06 14:46:06 2023
 */
#include <liblinux/pal.h>
#include <devhost/log.h>
#include <libhmkip/prmem.h>
#include <libsysif/sysmgr/api.h>

void *liblinux_pal_prmem_malloc(void *pool, size_t size, int flags)
{
	if (!pool) {
		hm_error("Ldk using prmem pool is Null.\n");
		return NULL;
	}
	return pmalloc((struct prmem_pool *)pool, size, (enum prmem_flags)flags);
}

void *liblinux_pal_prmem_calloc(void *pool, size_t n, size_t size, int flags)
{
	if (!pool) {
		hm_error("Ldk using prmem pool is Null.\n");
		return NULL;
	}
	return pcalloc((struct prmem_pool *)pool, n, size, (enum prmem_flags)flags);
}

char *liblinux_pal_prmem_strdup(void *pool, const char *s, int flags)
{
	if (!pool) {
		hm_error("Ldk using prmem pool is Null.\n");
		return NULL;
	}
	return pstrdup((struct prmem_pool *)pool, s, (enum prmem_flags)flags);
}

void liblinux_pal_prmem_protect_pool(void *pool)
{
	if (!pool) {
		hm_error("Ldk using prmem pool is Null.\n");
		return;
	}
	prmem_protect_pool((struct prmem_pool *)pool);
}

void liblinux_pal_prmem_sec_region_protect(void)
{
	actvcall_hmcall_sec_region_protect_services();
}
