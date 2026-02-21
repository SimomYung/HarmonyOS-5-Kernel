/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description:  OSAL(OS Abstraction Layer) header file
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 5 15:48:34 2023
 */

#ifndef __NETD_OSAL_H__
#define __NETD_OSAL_H__

#include <stdlib.h>

#include <libsysif/utils.h>
#include <libhmactv/actv.h>
#include <libmem/umem.h>
#include "ux_fusion_ops.h"

static inline void *osal_malloc(size_t size)
{
	return malloc(size);
}

static inline void osal_free(void *ptr)
{
	if (ptr != NULL) {
		free(ptr);
	}
}

static inline void *osal_calloc(size_t nmemb, size_t size)
{
	return calloc(nmemb, size);
}

static inline int osal_copy_from_user(void *dest, const void *src, unsigned long n)
{
#ifndef CONFIG_NETFS_FUSION
	if (is_fusion_ops_enable()) {
		return fusion_copy_from_user(dest, src, n);
	}
	return hm_actv_read_caller_vm(dest, src, n);
#else
	return hm_copy_from_user(dest, src, n);
#endif
}

static inline int osal_copy_to_user(void *dest, const void *src, unsigned long n)
{
#ifndef CONFIG_NETFS_FUSION
	if (is_fusion_ops_enable()) {
		return fusion_copy_to_user(dest, src, n);
	}
	return hm_actv_write_caller_vm(dest, src, n);
#else
	return hm_copy_to_user(dest, src, n);
#endif
}

static inline int osal_read_xact_vm(void *dest, const void *src, unsigned long n)
{
#ifndef CONFIG_NETFS_FUSION
	return hm_actv_read_xact_vm(dest, src, n);
#else
	return hm_copy_from_user(dest, src, n);
#endif
}

static inline int osal_write_xact_vm(void *dest, const void *src, unsigned long n)
{
#ifndef CONFIG_NETFS_FUSION
	return hm_actv_write_xact_vm(dest, src, n);
#else
	return hm_copy_to_user(dest, src, n);
#endif
}

/* the following two functions can only be used in actv-handler context */
uint32_t osal_hm_actv_src_cnode(void);
uint32_t osal_hm_actv_fwd_cnode(void);

/*
 * once cred be set to owner,
 * we can get cred in actv context by callinfo,
 * not traverse owner-tree
 */
unsigned long osal_hm_actv_cred(void);

void osal_hm_send_signal(int signo);

/**
 * get pid of current thread in root pid namespace
 */
pid_t osal_hm_root_ns_pid(void);
pid_t osal_hm_convert_pid_to_pidns(pid_t root_ns_pid, int pid_ns_id);
int osal_hm_lock_pid(pid_t root_ns_pid);
void osal_hm_unlock_pid(pid_t root_ns_pid);
int osal_current_pid_nsid(void);

/**
 * On success return pid in current pid namespace;
 * On fail return 0;
 */
pid_t osal_hm_try_convert_pid_to_current_pidns(pid_t root_ns_pid);
#endif /* __NETD_OSAL_H__ */
