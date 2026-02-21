/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Common utils for sysif
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 31 19:24:56 2021
 */
#ifndef LIBSYSIF_UTILS_COMMON_H
#define LIBSYSIF_UTILS_COMMON_H

#include <libsysif/base/macro.h>
#include <hmkernel/capability.h>
#include <hmasm/activation.h>
#include <hmasm/activation/upcall.h>

#include <hongmeng/syscall.h>
#include <hongmeng/errno.h>

#include <hmasm/actv_rpc.h>
#include <hmasm/libsysif/utils.h>

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmsrv_sys/hmsrv_sys.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libsecure/sec_cred.h>
#include <libhmseccomp/seccomp.h>
#include <hmkernel/extensions/lsyscall.h>
#include <libhmactv/actv.h>
#include <hongmeng/macro.h>
#include <hongmeng/types.h>

#define ULIBS_NEW_SYSIF_MASK_ALL
void sysif_mask_all(uint64_t *old);

void sysif_mask_restore(const uint64_t *old);

struct sysif_actv_selfcall {
	void *data;
	unsigned long data_sz;
	void *buf;
	unsigned long buf_sz;
	unsigned long long rpc_info;
};

#define __ACTV_CRED_SELFCALL			__ARCH_ACTV_CRED_INVAL_SELFCALL
#define __ACTV_CRED_UNDEFINED			__ARCH_ACTV_CRED_UNDEFINED
#define __ACTV_CRED_FWD_XACT			__ARCH_ACTV_CRED_INVAL_FWD_XACT
#define __ACTV_CRED_SELFCALL_XACT		__ARCH_ACTV_CRED_INVAL_SELFCALL_XACT

/*
 * Check if a sysif actvcall is selfcall.
 * Selfcall set credential be __ACTV_CRED_SELFCALL.
 */
static inline bool
sysif_actv_is_selfcall(unsigned long cred)
{
	return ((cred == __ACTV_CRED_SELFCALL) || (cred == __ACTV_CRED_SELFCALL_XACT)) ? true : false;
}

/*
 * Check whether cred is selfcall.
 */
static inline bool
sysif_cred_is_selfcall(unsigned long cred)
{
	return (cred == __ACTV_CRED_SELFCALL) ? true : false;
}

/*
 * Check whether cred is selfcall_xact.
 */
static inline bool
sysif_cred_is_selfcall_xact(unsigned long cred)
{
	return (cred == __ACTV_CRED_SELFCALL_XACT) ? true : false;
}

/* Check whether it is an undefined cred in sysif actv handler. */
static inline bool
sysif_cred_is_undefined(unsigned long cred)
{
	return (cred == __ACTV_CRED_UNDEFINED) ? true : false;
}

/* Check whether it is an invalid cred used for forwarding transaction. */
static inline bool
sysif_cred_is_fwd_xact(unsigned long cred)
{
	return (cred == __ACTV_CRED_FWD_XACT) ? true : false;
}

static inline bool
sysif_actv_is_fwd_xact(unsigned long long sender)
{
	return (__RPC_SENDER_DECODE_FWD_CIDX(sender) !=
		__RPC_SENDER_DECODE_SRC_CIDX(sender));
}

static inline bool
sysif_cred_is_invalid(unsigned long cred)
{
	return (cred >= __ARCH_ACTV_CRED_INVAL_MIN) ? true : false;
}

extern __thread struct sec_cred sysif_callinfo;

static inline struct sec_cred* sysif_actv_current_callinfo(void)
{
	return &sysif_callinfo;
}

static inline __u32 sysif_actv_src_cnode_idx(void)
{
	return __RPC_SENDER_DECODE_SRC_CIDX(sysif_actv_current_callinfo()->sender);
}

static inline __u32 sysif_actv_fwd_cnode_idx(void)
{
	return __RPC_SENDER_DECODE_FWD_CIDX(sysif_actv_current_callinfo()->sender);
}

/*
 * Simplify selfcall logic, only update sender for actvcall
 * this function can't be used for actvxactcall.
 * There are two senarios,
 * 1) sysmgr call vfs:
 *	vfs use credential to store client data, but if src_cnode_idx == sysmgr,
 *	vfs use global client data instead of credential, so it's safe if we don't
 *	update credential.
 * 2) vfs call sysmgr:
 *	To be supported, we can also use src_cnode_idx to judge whether it's a selfcall.
 *
 * In the feature, we will combine sysmgr and vfs actvpool, so credential will
 * have the same meanings, so ignoring credential difference is reasonable temporarily.
 *
 * fwd controls whether we reserve fwd_cnode_idx in directcall. VFS depends on fwd_cnode_idx
 * to get cwd, root and readlink /proc/self/exe, so in scenarios such as open_execat/openat,
 * we should reserve fwd_cnode_idx.
 */
static inline unsigned long long sysif_actv_set_directcall(bool fwd)
{
	__u32 src_idx = hm_ucap_self_cnode_idx();
	__u32 fwd_idx = src_idx;
	__u64 old_sender = sysif_callinfo.sender;
	if (fwd) {
		fwd_idx = __RPC_SENDER_DECODE_FWD_CIDX(old_sender);
	}
	sysif_callinfo.sender = src_idx + ((unsigned long long)fwd_idx << 32ULL);

	return old_sender;
}

static inline void sysif_actv_restore_directcall(unsigned long long *sender_ptr)
{
	sysif_callinfo.sender = *sender_ptr;
}
/*
 * (void)__sender is used to avoid warnings
 * __sender is used in cleanup function.
 */
#define SYSIF_DIRECTCALL_GUARD()	\
	unsigned long long __sender __attribute__ ((cleanup(sysif_actv_restore_directcall)))	\
				    = sysif_actv_set_directcall(false);	\
	(void)__sender

#define SYSIF_DIRECTCALL_FWD_GUARD()	\
	unsigned long long __sender __attribute__ ((cleanup(sysif_actv_restore_directcall)))	\
				    = sysif_actv_set_directcall(true);	\
	(void)__sender


#define SYSIF_SENDER_GUARD(target_sender)							\
	unsigned long long __sender __attribute__ ((cleanup(sysif_actv_restore_directcall)))	\
				    = sysif_callinfo.sender;					\
	sysif_callinfo.sender = (target_sender);						\
	(void)__sender

static inline void sysif_actv_update_callinfo(unsigned long long sender,
					      unsigned long credential)
{
	struct sec_cred *info = sysif_actv_current_callinfo();
	info->sender = sender;
	info->credential = credential;
	info->u_cred_flags = 0;
}

static inline void sysif_actv_clear_cred_flags(void)
{
	struct sec_cred *info = sysif_actv_current_callinfo();
	info->u_cred_flags = 0;
}

static inline void sysif_actv_clear_base_cred_cache(void)
{
	struct sec_cred *info = sysif_actv_current_callinfo();
	info->cred_flags.fwd_base_cred_valid = SEC_CRED_INVALID;
	info->cred_flags.src_base_cred_valid = SEC_CRED_INVALID;
}

/* Check if a selfcall is a fwd call */
static inline bool
sysif_selfcall_is_fwd_xact(unsigned long long sender)
{
	struct sysif_actv_selfcall *pselfcall = (struct sysif_actv_selfcall *)(uintptr_t)(sender);
	return __RPC_INFO_DECODE_FWD_XACT(pselfcall->rpc_info) == 1UL ? true: false;
}

static inline void
sysif_selfcall_update_cred_info(__u32 *fwd_idx, __u64 old_sender, unsigned long long sender)
{
	bool is_fwd_xact = sysif_selfcall_is_fwd_xact(sender);
	if (is_fwd_xact) {
		*fwd_idx = __RPC_SENDER_DECODE_FWD_CIDX(old_sender);
	} else {
		if (sysif_callinfo.cred_flags.selfcall_fwd_use_self == SEC_CRED_SELFCALL_FWD_NOT_USE_SELF) {
			sysif_callinfo.cred_flags.selfcall_fwd_use_self = SEC_CRED_SELFCALL_FWD_USE_SELF;
		}
	}
}

/* Update selfcall sender to emulate kernel IPC */
static inline void
sysif_selfcall_update_sender(unsigned long long sender)
{
	__u64 old_sender = sysif_callinfo.sender;
	__u32 src_idx = hm_ucap_self_cnode_idx();
	__u32 fwd_idx = src_idx;
	if (old_sender != 0UL) {
		sysif_selfcall_update_cred_info(&fwd_idx, old_sender, sender);
	}
	sysif_callinfo.sender = src_idx + ((unsigned long long)fwd_idx << 32ULL);
}

struct actv;
struct actv_attr;
struct actv_pure_attr;

#endif
