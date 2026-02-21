/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Provide common secure credential operation functions
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 30 14:51:46 2019
 */
#ifndef ULIBS_LIBSEC_CRED_COMMON_STRUCT_H
#define ULIBS_LIBSEC_CRED_COMMON_STRUCT_H
// sys
#include <sys/types.h>
#include <libhmsync/raw_rwlock.h>
// hongmeng
#include <hongmeng/errno.h>
#include <libsysif/utils.h>
// security
#include <libshlist/shlist.h>
#include <libseqlock/seqlock.h>
#include <libhmsrv_sys/hm_coobj.h>
#include "ns_usr.h"
#include "posix_cap_def.h"
#include "property_props.h"
#include "pac.h"
#include "libsec/pac_func.h"

typedef uintptr_t cred_ator;
extern cred_ator g_sec_cred_ator;

int sec_cred_init(bool is_creator);
void sec_cred_fini(void);
typedef size_t cred_idx_t;

struct nsid_group {
	uint32_t netns_id;
};

struct sec_cred_basic {
	pid_t pid;
	pid_t ppid;
	uint32_t flags;
};

struct sec_cred_mac {
	uint64_t sid;
	uint64_t inner_sid;
};

struct sec_cred_caps {
	posix_cap_t inher_cap;
	posix_cap_t perm_cap;
	posix_cap_t eff_cap;
	posix_cap_t bset_cap;
	posix_cap_t amb_cap;
	unsigned secure_bits;
};

struct sec_cred_groups {
	cred_idx_t idx;
	gid_t *pgrp;
	uint32_t size;
};

enum encaps_type {
	BOOL_VALUE = 1,
	INT_VALUE,
};

enum encaps_proc_type {
	ENCAPS_UNKNOWN_PROC,
	ENCAPS_NO_SET_PROC,
	ENCAPS_KERNEL_PROC,
	ENCAPS_SYS_PROC,
	ENCAPS_HAP_PROC,
};

struct sec_encaps {
	enum encaps_type type;
	uint32_t key;
	uint32_t value;
};

struct cred_encaps {
	uint32_t version;
	__sec_data(libsec, cntx) enum encaps_proc_type proc_type;
	__sec_data(libsec, ptr) struct sec_encaps *encaps_list;
	uint32_t count;
};

struct sec_cred_encaps {
	/* don't change field order because of hm_pacga_encaps_mem */
	cred_idx_t idx; /* will be protected */
	enum encaps_proc_type proc_type; /* will be protected */
	uint32_t count;
	cred_idx_t cred_idx;
	uint32_t version;
	struct sec_encaps *encaps_list; /* we use idx to calculate encaps_list so that this ptr is not need to be protected */
	struct raw_rwlock rwlock;
	struct raw_refcnt cred_refcnt;
};

struct sec_cred_dac {
	uid_t			ruid;
	gid_t			rgid;
	uid_t			euid;
	gid_t			egid;
	uid_t			suid;
	gid_t			sgid;
	uid_t			fsuid;
	gid_t			fsgid;
	struct sec_cred_groups	groups;
};

enum cred_type {
	CRED_BASE = 1,
	CRED_SHARED,
	CRED_STR
};

struct str_cred {
	enum cred_type type;
	struct raw_refcnt cred_refcnt;
	cred_idx_t idx;
	cred_idx_t str_idx;
	char *pstr;
	uint32_t size;
};

static inline char *str_addr_of(struct str_cred *cred)
{
	return cred->pstr;
}

typedef struct str_cred cmdline_node_t;
typedef struct str_cred tty_node_t;

struct sec_cred_audit {
	uid_t	login_uid;
	uid_t	session_id;
	cred_idx_t cmdline_idx;
	cred_idx_t tty_idx;
};

struct sec_cred_keyring {
	void *ses_keyring_addr;
};

struct sec_cred_property {
	struct property_props props;
};

typedef uint32_t hmsel_sid_t;
typedef uint16_t hmsel_class_t;

struct sec_cred_seharmony {
	hmsel_sid_t osid;
	hmsel_sid_t sid;
	hmsel_sid_t exec_sid;
	hmsel_sid_t create_sid;
	hmsel_sid_t keycreate_sid;
	hmsel_sid_t sockcreate_sid;
};

struct group_search_args {
	gid_t gid;      /* object gid, such as files and native objects */
	gid_t proc_gid; /* gid of the process which will be searched */
};

enum sec_pac_shared_id {
	SEC_PAC_SHARED_IDX = 0,
	SEC_PAC_SHARED_MAC,
	SEC_PAC_SHARED_BASIC,
	SEC_PAC_SHARED_PROPERTY,
	SEC_PAC_SHARED_AUDIT,
	SEC_PAC_SHARED_COUNT,
};

enum sec_pac_base_id {
	SEC_PAC_BASE_IDX = 0,
	SEC_PAC_BASE_SHIDX,
	SEC_PAC_BASE_DAC,
	SEC_PAC_BASE_SEHARMONY,
	SEC_PAC_BASE_CAPS,
	SEC_PAC_BASE_KEYRING,
	SEC_PAC_BASE_NSID_GROUP,
	SEC_PAC_BASE_ENCAPS,
	SEC_PAC_BASE_COUNT,
};

/*
 * shared credentials means process global credentials,
 * you can put cred in shared credentials if and only if
 * it is constant during process lifetime or it can be atomicly updated(like process keyring
 * and sessiong keyring), and shared cred may be modified concurrently, caller should get mutex
 * lock before update shared cred.
 */
struct shared_cred {
	enum cred_type type;
	struct raw_refcnt cred_refcnt;
	cred_idx_t idx;
	uint8_t	dumpable;
	mode_t umask;
	struct sec_cred_mac cred_mac;
	struct sec_cred_basic cred_basic;
	struct sec_cred_property prop;
	struct sec_cred_audit cred_audit;
	uint32_t pac[SEC_PAC_SHARED_COUNT];
};

/* per thread credential */
struct base_cred {
	struct raw_refcnt cred_refcnt;
	cred_idx_t idx;
	cred_idx_t sh_idx;
	__sec_data(libsec, ptr) struct shared_cred *sh_cred;
	struct sec_cred_dac cred_dac;
	struct sec_cred_seharmony cred_sel;
	struct sec_cred_caps cred_caps;
	struct sec_cred_keyring cred_key;
	enum cred_type type;
	struct nsid_group nsid_group;
	cred_idx_t usr_ns_idx;
	__sec_data(libsec, ptr) struct usr_ns_cred *usr_ns_cred;
	cred_idx_t encaps_idx;
	__sec_data(libsec, ptr) struct sec_cred_encaps *cred_encaps;
	uint32_t pac[SEC_PAC_BASE_COUNT];
#ifdef PAC_DFI_SHARED_WITH_LOCK
	uint32_t pac_seq_lock[SEC_PAC_BASE_COUNT];
#endif
};

#define LOCAL_CRED_FLAG 0xffffffff
struct sec_local_cred {
	struct shared_cred fwd_sh_cred;
	struct base_cred fwd_base_cred;
	cred_idx_t fwd_cred_idx;
	uint32_t fwd_cnode_idx;
	struct shared_cred src_sh_cred;
	struct base_cred src_base_cred;
	cred_idx_t src_cred_idx;
	uint32_t src_cnode_idx;
	uint32_t local_mem_flag;
};

/* cred cache for sysif actv handler */
struct cred_context  {
	struct base_cred *src_base_cred;
	struct shared_cred *src_shared_cred;
	struct base_cred *fwd_base_cred;
	struct shared_cred *fwd_shared_cred;
};
extern __thread struct cred_context sysif_cred_cache;

struct sec_chk_cred {
        uint32_t cnode_idx;
        /* cred is sh_cred */
        __sec_data(libsec, ptr) void *cred;
        __sec_data(libsec, ptr) void *base_cred;
        cred_idx_t cred_idx;
        uint32_t local_mem_flag;
};

static inline void *sec_cred_idx_to_ptr(cred_idx_t idx)
{
#ifdef __aarch64__
	return coobj_offset_to_ptr(idx);
#else
	return coobj_idx_to_ptr(g_sec_cred_ator, idx);
#endif
}

static inline void sec_cred_idx_to_cred_simple(cred_idx_t idx, void **b_cred)
{
	struct base_cred *tmp_b_cred = sec_cred_idx_to_ptr(idx);
	*b_cred = tmp_b_cred;
}

static inline void update_fwd_cred_cache(struct base_cred *b_cred, struct shared_cred *sh_cred)
{
	sysif_cred_cache.fwd_base_cred = b_cred;
	sysif_cred_cache.fwd_shared_cred = sh_cred;
}

static inline void update_src_cred_cache(struct base_cred *b_cred, struct shared_cred *sh_cred)
{
	sysif_cred_cache.src_base_cred = b_cred;
	sysif_cred_cache.src_shared_cred = sh_cred;
}

#endif
