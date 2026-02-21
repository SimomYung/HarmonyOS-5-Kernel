/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Credential define head file
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 10:49:14 2019
 */
#ifndef ULIBS_LIBSEC_CRED_H
#define ULIBS_LIBSEC_CRED_H
// hongmeng
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_security.h>
#include <libhmsync/raw_scopedptr.h>
// security
#include "cred_common.h"
#include "chk.h"
#include "posix_cap_def.h"
#include "utility.h"
#include "property_props.h"
#ifndef SEC_PLC_PERMISSIVE_CACHE
#define SEC_PLC_PERMISSIVE_CACHE
#endif

#define SEC_SUPPORT_MAC_BIT 1U
#define SEC_SUPPORT_DAC_BIT 2U
#define SEC_SUPPORT_CAP_BIT 4U
#define SEC_SUPPORT_AUDITBIT 8U
#define SEC_SUPPORT_SEHARMONY_BIT 16U
#define SEC_SUPPORT_OVERLAY_BIT 32U
#define SEC_SUPPORT_SECCOMP_BIT 64U
#define SEC_SUPPORT_DEC_BIT 128U
#define SEC_SUPPORT_ENCAPS_BIT 256U
#define SEC_SUPPORT_ISOLATE_BIT 512U
#define SEC_SUPPORT_HKIDS_BIT 1024U

#define SEC_BASIC_SYS_SRVC   1u
#define SEC_BASIC_LOG_ENABLE 2u
#define SEC_BASIC_LOG_TRACE  3u

#define SEC_DEFAULT_UID __U(0)
#define SEC_DEFAULT_GID __U(0)

#define sec_cred_flag_mask_of(f)	(1U << ((unsigned int)(f)))
#define sec_cred_is_flag_set(x, f)	(((x) & sec_cred_flag_mask_of(f)) != 0U)
#define sec_cred_set_flag(x, f)		((x) |= sec_cred_flag_mask_of(f))
#define sec_cred_clear_flag(x, f)	((x) &= ~sec_cred_flag_mask_of(f))

#define SEC_HKIP_PREFIX "hkip:"

#define SEC_INNER_MAC_PREFIX "inner_mac_obj:"
#define SEC_INNER_DEFAULT_SID inner_default_t

enum cred_role {
	CRED_OWNER,
	CRED_USER,
};

struct proc_cred {
	struct sec_cred_mac mac_cred;
	struct sec_cred_seharmony sel_cred;
	struct sec_cred_caps cred_caps;
	struct sec_cred_dac dac_cred;
	struct sec_cred_property prop;
	uint8_t dumpable;
};

#define PROC_CRED_FLAGS_MAC	0x01
#define PROC_CRED_FLAGS_DAC	0x02
#define PROC_CRED_FLAGS_CAP	0x04
#define PROC_CRED_FLAGS_SEHARMONY	0x08
#define PROC_CRED_FLAGS_DUMP	0x10
#define PROC_CRED_FLAGS_PROP	0x20

/* PROC ID used to get cred by id */
#define PROC_ID_FLAGS_CNODE_IDX	0x10000
#define PROC_ID_FLAGS_PID	0x20000
#define PROC_ID_FLAGS_TID	0x40000
#define PROC_ID_MASK		0x70000

#define MAC_OF_SH_CRED(sh_cred) ((sh_cred)->cred_mac)
#define DUMP_OF_SH_CRED(sh_cred) ((sh_cred)->dumpable)
#define UMASK_OF_SH_CRED(sh_cred) ((sh_cred)->umask)
#define BASIC_OF_SH_CRED(sh_cred) ((sh_cred)->cred_basic)
#define PROP_OF_SH_CRED(sh_cred) ((sh_cred)->prop)
#define AUDIT_OF_SH_CRED(sh_cred) ((sh_cred)->cred_audit)

/* get cred from base cred */
#define DAC_OF_CRED(cred) ((cred)->cred_dac)
#define SEL_OF_CRED(cred) ((cred)->cred_sel)
#define USR_NS_OF_CRED(cred) ((cred)->usr_ns_cred)
#define MAC_OF_CRED(cred) MAC_OF_SH_CRED((cred)->sh_cred)
#define DUMP_OF_CRED(cred) DUMP_OF_SH_CRED((cred)->sh_cred)
#define CAPS_OF_CRED(cred) ((cred)->cred_caps)
#define KEYRING_OF_CRED(cred) ((cred)->cred_key)
#define ENCAPS_OF_CRED(cred) (*(cred)->cred_encaps)

#ifdef ENABLE_PAC_HARD_CODE
#define MAC_ADR_OF_SH_CRED(sh_cred) ({ \
	hm_pacdb_shcred_check((sh_cred), SEC_PAC_SHARED_MAC, (sh_cred)->idx, PAC_CHECK_NO_RETRY); \
	(&(sh_cred)->cred_mac); \
})

#define BASIC_ADR_OF_SH_CRED(sh_cred) ({ \
	hm_pacdb_shcred_check((sh_cred), SEC_PAC_SHARED_BASIC, (sh_cred)->idx, PAC_CHECK_NO_RETRY); \
	(&(sh_cred)->cred_basic); \
})

#define PROP_ADR_OF_SH_CRED(sh_cred) ({ \
	hm_pacdb_shcred_check((sh_cred), SEC_PAC_SHARED_PROPERTY, (sh_cred)->idx, PAC_CHECK_NO_RETRY); \
	(&(sh_cred)->prop); \
})

#define AUDIT_ADR_OF_SH_CRED(sh_cred) ({ \
	hm_pacdb_shcred_check((sh_cred), SEC_PAC_SHARED_AUDIT, (sh_cred)->idx, PAC_CHECK_NO_RETRY); \
	(&(sh_cred)->cred_audit); \
})

/* get cred from base cred */
#define MAC_ADR_OF_CRED(b_cred) MAC_ADR_OF_SH_CRED((b_cred)->sh_cred)

#ifdef PAC_DFI_CHECK_CRED_DAC_WITH_LOCK
#define PAC_CHECK_DAC_ADR_OF_CRED hm_pac_bcred_check_with_lock
#else
#define PAC_CHECK_DAC_ADR_OF_CRED hm_pac_bcred_check_nolock
#endif

#define DAC_ADR_OF_CRED(b_cred) ({ \
	PAC_CHECK_DAC_ADR_OF_CRED((b_cred), SEC_PAC_BASE_DAC); \
	(&(b_cred)->cred_dac); \
})

#ifdef PAC_DFI_CHECK_CRED_SEHARMANY_WITH_LOCK
#define PAC_CHECK_SEL_ADR_OF_CRED hm_pac_bcred_check_with_lock
#else
#define PAC_CHECK_SEL_ADR_OF_CRED hm_pac_bcred_check_nolock
#endif

#define SEL_ADR_OF_CRED(b_cred) ({ \
	PAC_CHECK_SEL_ADR_OF_CRED((b_cred), SEC_PAC_BASE_SEHARMONY); \
	(&(b_cred)->cred_sel); \
})

#ifdef PAC_DFI_CHECK_CRED_CAPS_WITH_LOCK
#define PAC_CHECK_CAPS_ADR_OF_CRED hm_pac_bcred_check_with_lock
#else
#define PAC_CHECK_CAPS_ADR_OF_CRED hm_pac_bcred_check_nolock
#endif

#define CAPS_ADR_OF_CRED(b_cred) ({ \
	PAC_CHECK_CAPS_ADR_OF_CRED((b_cred), SEC_PAC_BASE_CAPS); \
	(&(b_cred)->cred_caps); \
})

#ifdef PAC_DFI_CHECK_CRED_KEYRING_WITH_LOCK
#define PAC_CHECK_KEYRING_ADR_OF_CRED hm_pac_bcred_check_with_lock
#else
#define PAC_CHECK_KEYRING_ADR_OF_CRED hm_pac_bcred_check_nolock
#endif

#define KEYRING_ADR_OF_CRED(b_cred) ({ \
	PAC_CHECK_KEYRING_ADR_OF_CRED((b_cred), SEC_PAC_BASE_CAPS); \
	(&(b_cred)->cred_key); \
})

#ifdef PAC_DFI_CHECK_CRED_NSID_WITH_LOCK
#define PAC_CHECK_NSID_GROUP_ADR_OF_CRED hm_pac_bcred_check_with_lock
#else
#define PAC_CHECK_NSID_GROUP_ADR_OF_CRED hm_pac_bcred_check_nolock
#endif

#define NSID_GROUP_ADR_OF_CRED(b_cred) ({ \
	PAC_CHECK_NSID_GROUP_ADR_OF_CRED((b_cred), SEC_PAC_BASE_NSID_GROUP); \
	(&(b_cred)->nsid_group); \
})

#ifdef PAC_DFI_CHECK_CRED_ENCAPS_WITH_LOCK
#define PAC_CHECK_ENCAPS_ADR_OF_CRED hm_pac_bcred_check_with_lock
#else
#define PAC_CHECK_ENCAPS_ADR_OF_CRED hm_pac_bcred_check_nolock
#endif

#define ENCAPS_ADR_OF_CRED(b_cred) ({ \
	PAC_CHECK_ENCAPS_ADR_OF_CRED((b_cred), SEC_PAC_BASE_ENCAPS); \
	(&(b_cred)->cred_encaps); \
})
#else
#define MAC_ADR_OF_SH_CRED(sh_cred) (&(sh_cred)->cred_mac)
#define BASIC_ADR_OF_SH_CRED(sh_cred) (&(sh_cred)->cred_basic)
#define PROP_ADR_OF_SH_CRED(sh_cred) (&(sh_cred)->prop)
#define AUDIT_ADR_OF_SH_CRED(sh_cred) (&(sh_cred)->cred_audit)

/* get cred from base cred */
#define MAC_ADR_OF_CRED(b_cred) MAC_ADR_OF_SH_CRED((b_cred)->sh_cred)
#define DAC_ADR_OF_CRED(b_cred) (&(b_cred)->cred_dac)
#define SEL_ADR_OF_CRED(b_cred) (&(b_cred)->cred_sel)
#define CAPS_ADR_OF_CRED(b_cred) (&(b_cred)->cred_caps)
#define KEYRING_ADR_OF_CRED(b_cred) (&(b_cred)->cred_key)
#define NSID_GROUP_ADR_OF_CRED(b_cred) (&(b_cred)->nsid_group)
#define ENCAPS_ADR_OF_CRED(b_cred) (&(b_cred)->cred_encaps)
#endif

/* shared cred helpers */
#define current_sh_cred() (requester_current_frame()->requester.fwd_sh_cred)
#define current_inter_sh_cred() (requester_current_frame()->requester.src_sh_cred)

#define current_base_cred() (requester_current_frame()->requester.fwd_base_cred)
#define current_inter_base_cred() (requester_current_frame()->requester.src_base_cred)

int sec_cocred_init(enum cred_role role);
int sec_cred_alloc(size_t sz, cred_idx_t *idx);
void sec_cred_free(cred_idx_t idx);
int sec_cred_dup(const void *src_ptr, cred_idx_t *new, size_t src_sz, size_t dst_sz);
struct base_cred *sec_base_cred_alloc(cred_idx_t *idx_t);
struct shared_cred *sec_shared_cred_alloc(cred_idx_t *idx_t);
struct usr_ns_cred *sec_usr_ns_cred_alloc(cred_idx_t *idx_t);
struct str_cred *sec_str_cred_alloc(cred_idx_t *idx_t);
struct sec_cred_encaps *sec_cred_encaps_alloc(cred_idx_t *idx_t);
void sec_shared_cred_ref_release(struct raw_refcnt *ref);
void sec_base_cred_ref_release(struct raw_refcnt *ref);
void sec_str_cred_ref_release(struct raw_refcnt *ref);
void sec_usr_ns_cred_ref_release(struct raw_refcnt *ref);
void sec_cred_encaps_ref_release(struct raw_refcnt *ref);
bool sec_cocred_is_inited(void);
int sec_get_mac_cred_by_pid_nsid(struct proc_cred *cred, pid_t pid, unsigned int nsid);
int sec_get_dac_cred_dump_by_pid_nsid(struct proc_cred *cred, pid_t pid, unsigned int nsid);
int sec_get_proc_cred_dump_by_pid_nsid(struct proc_cred *cred, pid_t pid, unsigned int nsid);

static inline void sec_base_cred_get(struct base_cred *cred)
{
	(void)raw_refcnt_get(&cred->cred_refcnt);
}

static inline void sec_base_cred_put(struct base_cred *cred)
{
	(void)raw_refcnt_put(&cred->cred_refcnt);
}

static inline void sec_str_cred_get(struct str_cred *cred)
{
	(void)raw_refcnt_get(&cred->cred_refcnt);
}

static inline void sec_str_cred_put(struct str_cred *cred)
{
	(void)raw_refcnt_put(&cred->cred_refcnt);
}

static inline void sec_usr_ns_cred_get(struct usr_ns_cred *cred)
{
	(void)raw_refcnt_get(&cred->refcnt);
}

static inline void sec_usr_ns_cred_put(struct usr_ns_cred *cred)
{
	(void)raw_refcnt_put(&cred->refcnt);
}

static inline void sec_cred_encaps_get(struct sec_cred_encaps *cred)
{
	(void)raw_refcnt_get(&cred->cred_refcnt);
}

static inline void sec_cred_encaps_put(struct sec_cred_encaps *cred)
{
	(void)raw_refcnt_put(&cred->cred_refcnt);
}

#define DAC_GROUP_INITIALIZED ((void *)-1)
int sec_copy_groups(struct sec_cred_dac *dac_cred, uint32_t group_size, gid_t *group_list);

int sec_copy_encaps_list(struct sec_cred_encaps *encaps_cred, uint32_t count, struct sec_encaps *encaps_list);

static inline void copy_shared_cred_for_base_cred(struct base_cred *dst_cred, struct base_cred *src_cred)
{
	dst_cred->sh_idx = src_cred->sh_idx;
	dst_cred->sh_cred = src_cred->sh_cred;
	(void)raw_refcnt_get(&dst_cred->sh_cred->cred_refcnt);
}

static inline int copy_dac_cred_for_base_cred(struct base_cred *dst_cred, struct base_cred *src_cred)
{
	dst_cred->cred_dac = src_cred->cred_dac;
	dst_cred->cred_dac.groups.pgrp = NULL;
	dst_cred->cred_dac.groups.idx = 0;
	dst_cred->cred_dac.groups.size = 0;
	return sec_copy_groups(&dst_cred->cred_dac, src_cred->cred_dac.groups.size,
			src_cred->cred_dac.groups.pgrp);
}

static inline void copy_encaps_cred_for_base_cred(struct base_cred *dst_cred, struct base_cred *src_cred)
{
	dst_cred->cred_encaps = src_cred->cred_encaps;
	dst_cred->encaps_idx = src_cred->encaps_idx;
	if (dst_cred->cred_encaps != NULL) {
		raw_refcnt_get(&src_cred->cred_encaps->cred_refcnt);
	}
}

static inline void copy_usr_ns_from_base_cred(struct base_cred *dst_cred, struct base_cred *src_cred)
{
	struct usr_ns_cred *dst_usr_ns_cred = dst_cred->usr_ns_cred;
	raw_refcnt_get(&src_cred->usr_ns_cred->refcnt);
	dst_cred->usr_ns_idx = src_cred->usr_ns_idx;
	dst_cred->usr_ns_cred = src_cred->usr_ns_cred;
	if (dst_usr_ns_cred != NULL) {
		raw_refcnt_put(&dst_usr_ns_cred->refcnt);
	}
}

static inline int sec_copy_base_cred(struct base_cred *dst_cred, struct base_cred *src_cred)
{
	int rc;
	copy_shared_cred_for_base_cred(dst_cred, src_cred);
#ifdef CONFIG_SEHARMONY
	dst_cred->cred_sel = src_cred->cred_sel;
#endif
#ifdef CONFIG_POSIX_CAPS
	dst_cred->cred_caps = src_cred->cred_caps;
#endif
#ifdef CONFIG_KEYMGR
	dst_cred->cred_key = src_cred->cred_key;
#endif
#ifdef CONFIG_NET_NS
	dst_cred->nsid_group.netns_id = src_cred->nsid_group.netns_id;
#endif
#ifdef CONFIG_ENCAPS
	copy_encaps_cred_for_base_cred(dst_cred, src_cred);
#endif
	rc = copy_dac_cred_for_base_cred(dst_cred, src_cred);
#ifdef CONFIG_USER_NS
	if (rc == E_HM_OK) {
		copy_usr_ns_from_base_cred(dst_cred, src_cred);
	}
#endif
#ifdef ENABLE_PAC_HARD_CODE
	hm_pacdb_bcred_mem(dst_cred);
#endif
	return rc;
}

static inline struct base_cred *sec_base_cred_dup(struct base_cred *old)
{
	int ret = E_HM_OK;
	cred_idx_t idx;
	struct base_cred *new = sec_base_cred_alloc(&idx);
	if (new != NULL) {
		sec_base_cred_get(old);
#ifdef ENABLE_PAC_HARD_CODE
		hm_pacga_bcred(new, SEC_PAC_BASE_IDX);
#endif
		ret = sec_copy_base_cred(new, old);
		if (ret != E_HM_OK) {
			sec_base_cred_put(new);
		}
		sec_base_cred_put(old);
	}
	return ret == E_HM_OK ? new : NULL;
}

int sec_get_mac_cred_by_cidx(struct proc_cred *cred, uint32_t cnode_idx);
int sec_get_seharmony_cred_by_cidx(struct proc_cred *cred, uint32_t cnode_idx);
int sec_get_mac_cred_by_pid(struct proc_cred *cred, pid_t pid);
int sec_get_seharmony_cred_by_pid(struct proc_cred *cred, pid_t pid);
int sec_get_dac_cred_by_pid(struct proc_cred *cred, pid_t pid);
int sec_get_dac_cred_dump_by_pid(struct proc_cred *cred, pid_t pid);
int sec_get_prop_by_pid(struct proc_cred *cred, pid_t pid);

int sec_cred_copy_from_req(struct sec_local_cred *ret_cred);

int sec_cred_groups_search(const gid_t *group_list, uint32_t size, gid_t gid);

#include "mac_cred.h"
#include "seharmony_cred.h"
#include "dac_cred.h"
#include "ns_cred.h"
#include "basic_cred.h"
#include "caps_cred.h"
#include "keyring_cred.h"
#include "audit_cred.h"
#include "encaps_cred.h"

/* Check whether the gid is in the group of the dac */
int sec_cred_dac_in_group_check(const struct sec_cred_dac *dac_cred, gid_t gid);

static inline int sec_cred_check_dac_groups(const struct sec_chk_cred *chk,
					   gid_t obj_gid, gid_t proc_gid)
{
	int rc;
	struct sec_cred_dac *dac_cred = dac_cred_of_sec_cred(chk);
	rc = in_group(dac_cred, obj_gid, proc_gid);
	return rc;
}

static inline int sec_sysfast_actv_cred(__u64 *src_idx)
{
	int ret = E_HM_OK;
	*src_idx = sysif_callinfo.self_cred_idx;
	if (unlikely(*src_idx == 0)) {
		ret = sysfast_actv_cred(src_idx);
		sysif_callinfo.self_cred_idx = *src_idx;
	}
	return ret;
}
#endif /* ifndef ULIBS_LIBSEC_CRED_H */
