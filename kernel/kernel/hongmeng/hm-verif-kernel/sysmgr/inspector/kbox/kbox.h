/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Header file for kbox_init
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 11 14:26:15 2019
 */

#ifndef SYSMGR_INSPECTOR_KBOX_H
#define SYSMGR_INSPECTOR_KBOX_H

#include <stdint.h>
#include <libkbox/device.h>
#include <libstrict/strict.h>
#include <hmasm/pgtbl/pgtblhdlr.h>

#ifdef __HMPGTBL_ARM_LPAE__
typedef uint64_t kbox_paddr_t;
#else
typedef uintptr_t kbox_paddr_t;
#endif

struct kbox_region_param {
	char name[KBOX_REGION_NAME_SIZE];
	kbox_paddr_t paddr;
	size_t size;
	bool dynamic;
	int refcnt;
};

struct kbox_device_param {
	kbox_paddr_t paddr;
	size_t total_size;
	size_t free_size;
	uint32_t region_count;
	struct kbox_region_param regions[KBOX_REGION_COUNT_MAX];
};

__noinline int kbox_log_transaction(int region_id, const char *msg, const char *label);

/**
 * only use nolock path when sysmgr snapshot
 */
int kbox_log_transaction_nolock(int region_id, const char *msg, const char *label);

#ifdef CONFIG_KBOX

int kbox_init(void);
int kbox_gparam_foreach(int (*op)(const char *, size_t));
int kbox_query_device(unsigned long long sender,
		      unsigned long credential);
int kbox_query_region(unsigned long long sender,
		      unsigned long credential,
		      unsigned int index);
int kbox_open_region(unsigned long long sender,
		     unsigned long credential,
		     const char *name);
int kbox_close_region(unsigned long credential, const char *name,
		      uintptr_t vaddr, size_t size);
int kbox_create_region(const char *name, size_t size);
int kbox_delete_region(const char *name);
int kbox_check_region_dynamic(unsigned long long sender,
			      unsigned long credential,
			      const char *name);

#else /* CONFIG_KBOX */

static inline int kbox_init(void)
{
	return E_HM_OK;
}

static inline int kbox_gparam_foreach(int (*op)(const char *, size_t))
{
	UNUSED(op);
	return E_HM_NOSYS;
}

static inline int kbox_query_device(unsigned long long sender,
				    unsigned long credential)
{
	UNUSED(sender, credential);
	return E_HM_NOSYS;
}

static inline int kbox_query_region(unsigned long long sender,
				    unsigned long credential,
				    unsigned int index)
{
	UNUSED(sender, credential, index);
	return E_HM_NOSYS;
}

static inline int kbox_open_region(unsigned long long sender,
				   unsigned long credential,
				   const char *name)
{
	UNUSED(sender, credential, name);
	return E_HM_NOSYS;
}

static inline int kbox_close_region(unsigned long credential, const char *name,
				    uintptr_t vaddr, size_t size)
{
	UNUSED(credential, name, vaddr, size);
	return E_HM_NOSYS;
}

static inline int kbox_create_region(const char *name, size_t size)
{
	UNUSED(name, size);
	return E_HM_NOSYS;
}

static inline int kbox_delete_region(void)
{
	return E_HM_NOSYS;
}

static inline int kbox_check_region_dynamic(unsigned long long sender,
					    unsigned long credential,
					    const char *name)
{
	UNUSED(sender, credential, name);
	return E_HM_NOSYS;
}

#endif /* CONFIG_KBOX */

#ifdef CONFIG_KBOX_BACKUP_RAW

void kbox_backup(const void *data, size_t size);
void kbox_backup_dump(void);

#else /* CONFIG_KBOX_BACKUP_RAW */

static inline void kbox_backup(const void *data, size_t size)
{
	UNUSED(data, size);
}

static inline void kbox_backup_dump(void) { }

#endif /* CONFIG_KBOX_BACKUP_RAW */

#ifdef __HOST_LLT__

/* reset LLT enrionment */
void kbox_llt_reset(void);

#endif

#endif /* SYSMGR_INSPECTOR_KBOX_H */
