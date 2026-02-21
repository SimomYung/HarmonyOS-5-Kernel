/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Cnode capability types definition
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 12 08:14:44 2018
 */
/*
 * Must choose one GRANT type from ALL/NO/COARSE/FINE
 * Must choose one GRANTMOVE type from ALL/NO/COARSE
 */
#include <hmkernel/feature.h>

#ifdef __UAPI_COMPAT__
#define CAP_CNODE_COMPAT_LLT_METHOD	CAPMETHOD(CNode, CompatLLT)
#else
#define CAP_CNODE_COMPAT_LLT_METHOD
#endif

CAPTYPE(CNode, 1, CAPCOARSEGRANT(CNode)
		  CAPNOGRANTMOVE(CNode)
		  CAPDEFAULTSIZE(CNode),
	CAPMETHOD(CNode, NewCap)
	CAPMETHOD(CNode, DeleteCap)
	CAPMETHOD(CNode, ExtendCTable)
	CAPMETHOD(CNode, ExtendUTable)
	CAPMETHOD(CNode, ExtendRTable)
	CAPMETHOD(CNode, IterateCTableStart)
	CAPMETHOD(CNode, IterateCTableNext)
	CAPMETHOD(CNode, StatCap)
	CAPMETHOD(CNode, SpawnCNode)
	CAPMETHOD(CNode, Destroy)
	CAPMETHOD(CNode, Require)
	CAPMETHOD(CNode, Reject)
	CAPMETHOD(CNode, Grant)
	CAPMETHOD(CNode, Revoke)
	CAPMETHOD(CNode, Export)
	CAPMETHOD(CNode, AcceptGrant)
	CAPMETHOD(CNode, IterateRTableStart)
	CAPMETHOD(CNode, IterateRTableNext)
	CAPMETHOD(CNode, IterateUTableStart)
	CAPMETHOD(CNode, IterateUTableNext)
	CAPMETHOD(CNode, CRef)
	CAPMETHOD(CNode, GrantMove)
	CAPMETHOD(CNode, StatRRef)
	CAPMETHOD(CNode, StatURef)
	CAPMETHOD(CNode, ProxyCall)
	CAPMETHOD(CNode, ExportMove)
	CAPMETHOD(CNode, AcceptMove)
	CAPMETHOD(CNode, SetCLimit)
	CAPMETHOD(CNode, GetCLimit)
	CAPMETHOD(CNode, Stat)
	CAPMETHOD(CNode, BindPermission)
	CAPMETHOD(CNode, UnBindPermission)
	CAPMETHOD(CNode, SetPermission)
	CAPMETHOD(CNode, ExtFastcallBind)
	CAPMETHOD(CNode, ExtFastcallUnbind)
	CAPMETHOD(CNode, AuditContext)
	CAPMETHOD(CNode, ULWTMonitorInit)
	CAPMETHOD(CNode, ULWTMonitorDestroy)
	CAPMETHOD(CNode, ULWTMonitorSynccallLock)
	CAPMETHOD(CNode, ULWTMonitorSynccallUnlock)
	CAPMETHOD(CNode, ULWTKinfoInit)
	CAPMETHOD(CNode, ULWTKinfoDestroy)
	CAPMETHOD(CNode, ULWTDomainPrioSeqUpdate)
	CAP_CNODE_COMPAT_LLT_METHOD

	CAPERRNO(CNode, EINVAL, CNODE_INVAL_CNODE,	"E_HM_CNODE_INVAL_CNODE")
	CAPERRNO(CNode, EHMERROR, CNODE_UTBLSLOT_STATE,	"E_HM_CNODE_UTBLSLOT_STATE")
	CAPERRNO(CNode, EHMERROR, CNODE_RTBLSLOT_STATE,	"E_HM_CNODE_RTBLSLOT_STATE")
	CAPERRNO(CNode, EHMERROR, CNODE_TYPE,			"E_HM_CNODE_TYPE")
	CAPERRNO(CNode, EAGAIN, CNODE_CSPACE_FULL,		"E_HM_CNODE_CSPACE_FULL")
	CAPERRNO(CNode, EAGAIN, CNODE_CTABLE_GROW_LIMIT,	"E_HM_CNODE_CTABLE_GROW_LIMIT")
	CAPERRNO(CNode, EAGAIN, CNODE_UTABLE_GROW_LIMIT,	"E_HM_CNODE_UTABLE_GROW_LIMIT")
	CAPERRNO(CNode, EAGAIN, CNODE_RTABLE_GROW_LIMIT,	"E_HM_CNODE_RTABLE_GROW_LIMIT")
	CAPERRNO(CNode, EHMERROR, CNODE_NEWCAP_TYPE,		"E_HM_CNODE_NEWCAP_TYPE")
	CAPERRNO(CNode, ENOMEM, CNODE_NO_CTBLSLOT,	"E_HM_CNODE_NO_CTBLSLOT")
	CAPERRNO(CNode, ENOMEM, CNODE_NO_UTBLSLOT,	"E_HM_CNODE_NO_UTBLSLOT")
	CAPERRNO(CNode, ENOMEM, CNODE_NO_RTBLSLOT,	"E_HM_CNODE_NO_RTBLSLOT")
	CAPERRNO(CNode, EAGAIN, CNODE_CTBLSLOT_LIMIT,	"E_HM_CNODE_CTBLSLOT_LIMIT")
	CAPERRNO(CNode, EHMERROR, CNODE_WRONG_IDX,		"E_HM_CNODE_WRONG_IDX")
	CAPERRNO(CNode, EHMERROR, CNODE_LOOKUP,		"E_HM_CNODE_LOOKUP")
	CAPERRNO(CNode, EHMERROR, CNODE_LOOKUP_U,		"E_HM_CNODE_LOOKUP_U")
	CAPERRNO(CNode, EHMERROR, CNODE_LOOKUP_R,		"E_HM_CNODE_LOOKUP_R")
	CAPERRNO(CNode, EHMERROR, CNODE_CREF_FORMAT,		"E_HM_CNODE_CREF_FORMAT")
	CAPERRNO(CNode, EHMERROR, CNODE_RREF_FORMAT,		"E_HM_CNODE_RREF_FORMAT")
	CAPERRNO(CNode, EHMERROR, CNODE_UREF_FORMAT,		"E_HM_CNODE_UREF_FORMAT")
	CAPERRNO(CNode, EHMERROR, CNODE_DESTROYING,		"E_HM_CNODE_DESTROYING")
	CAPERRNO(CNode, EHMERROR, CNODE_INVOKE_METHOD,	"E_HM_CNODE_INVOKE_METHOD")
	CAPERRNO(CNode, EPERM, CNODE_INVOKE_NORIGHT,	"E_HM_CNODE_INVOKE_NORIGHT")
	CAPERRNO(CNode, EPERM, CNODE_INVOKE_NOGRANT,	"E_HM_CNODE_INVOKE_NOGRANT")
	CAPERRNO(CNode, EPERM, CNODE_FORBID_GRANT,	"E_HM_CNODE_FORBID_GRANT")
	CAPERRNO(CNode, EPERM, CNODE_FORBID_GRANTMOVE,	"E_HM_CNODE_FORBID_GRANTMOVE")
	CAPERRNO(CNode, EHMERROR, CNODE_GRANT_TOO_MANY,	"E_HM_CNODE_GRANT_TOO_MANY")
	CAPERRNO(CNode, EPERM, CNODE_GRANT_PERM,	"E_HM_CNODE_GRANT_PERM")
	CAPERRNO(CNode, EHMERROR, CNODE_DELETING,		"E_HM_CNODE_DELETING")
	CAPERRNO(CNode, EHMERROR, CNODE_DEL_INVAL,		"E_HM_CNODE_DEL_INVAL")
	CAPERRNO(CNode, EHMERROR, CNODE_DEL_USED,		"E_HM_CNODE_DEL_USED")
	CAPERRNO(CNode, EHMERROR, CNODE_CTABLE_NEED_EXTEND,	"E_HM_CNODE_CTABLE_NEED_EXTEND")
	CAPERRNO(CNode, EHMERROR, CNODE_CTABLE_EXTEND_ENOUGH,	"E_HM_CNODE_CTABLE_EXTEND_ENOUGH")
	CAPERRNO(CNode, EHMERROR, CNODE_UTABLE_EXTEND_ENOUGH,	"E_HM_CNODE_UTABLE_EXTEND_ENOUGH")
	CAPERRNO(CNode, EHMERROR, CNODE_RTABLE_EXTEND_ENOUGH,	"E_HM_CNODE_RTABLE_EXTEND_ENOUGH")
	CAPERRNO(CNode, EHMERROR, CNODE_DEL_CTABLE_STATE,	"E_HM_CNODE_DEL_CTABLE_STATE")
	CAPERRNO(CNode, EHMERROR, CNODE_DEL_UTABLE_STATE,	"E_HM_CNODE_DEL_UTABLE_STATE")
	CAPERRNO(CNode, EHMERROR, CNODE_DEL_RTABLE_STATE,	"E_HM_CNODE_DEL_RTABLE_STATE")
	CAPERRNO(CNode, EPERM, CNODE_REDUCE_CAP_NORIGHT,	"E_HM_CNODE_REDUCE_CAP_NORIGHT")
	CAPERRNO(CNode, EHMERROR, CNODE_KOBJ_IN_USE,	"E_HM_CNODE_KOBJ_IN_USE")
	CAPERRNO(CNode, EHMERROR, CNODE_NOT_ALLOW_RPC,	"E_HM_CNODE_NOT_ALLOW_RPC")
)

#undef CAP_CNODE_COMPAT_LLT_METHOD

#ifdef CAP_EXPORT_API
#ifndef CAP_CNODE_API_EXPORTED
#define CAP_CNODE_API_EXPORTED

#include <hmkernel/types.h>
#include <hmkernel/capability.h>

enum {
	CRIGHT_CNODE_PROXYCALL_BIT,
	CRIGHT_CNODE_GRANT_BIT,
	CRIGHT_CNODE_PREPAREGRANT_BIT,
	CRIGHT_CNODE_INC_CLIMIT_BIT,
	CRIGHT_CNODE_DEC_CLIMIT_BIT,
	CRIGHT_CNODE_SPAWNCNODE_BIT,
	CRIGHT_CNODE_ACCEPT_CNODE_GRANT_BIT,
	CRIGHT_CNODE_AUDITCONTEXT_BIT,
	CRIGHT_CNODE_MAX
};

#define CRIGHT_CNODE_PROXYCALL		(1U << CRIGHT_CNODE_PROXYCALL_BIT)
#define CRIGHT_CNODE_GRANT		(1U << CRIGHT_CNODE_GRANT_BIT)
#define CRIGHT_CNODE_PREPAREGRANT	(1U << CRIGHT_CNODE_PREPAREGRANT_BIT)
#define CRIGHT_CNODE_INC_CLIMIT		(1U << CRIGHT_CNODE_INC_CLIMIT_BIT)
#define CRIGHT_CNODE_DEC_CLIMIT		(1U << CRIGHT_CNODE_DEC_CLIMIT_BIT)
#define CRIGHT_CNODE_SPAWNCNODE		(1U << CRIGHT_CNODE_SPAWNCNODE_BIT)
#define CRIGHT_CNODE_ACCEPT_CNODE_GRANT	(1U << CRIGHT_CNODE_ACCEPT_CNODE_GRANT_BIT)
#define CRIGHT_CNODE_AUDITCONTEXT	(1U << CRIGHT_CNODE_AUDITCONTEXT_BIT)
#define CRIGHT_CNODE_ALL		0xffU

#define CNODE_DEFAULT_FREESLOT_WATER	1
#define __CNODE_PROXY_NEWCAPFLAG(type, rights, water, proxy_created)	\
	(((__u64)(((__u64)(((__u32)(proxy_created) << 16) + ((__u32)(type) << 8)	\
			   + (__u32)(rights))) << 32)) | ((__u32)(water)))
#define __CNODE_NEWCAPFLAG(type, rights, water)	\
	(((__u64)(((__u64)(((__u32)(type) << 8) + (__u32)(rights))) << 32)) | ((__u32)(water)))
#define CNODE_NEWCAPFLAG(type, rights)	__CNODE_NEWCAPFLAG(type, rights, \
		CNODE_DEFAULT_FREESLOT_WATER)

#define CNODE_GRANT_PREPARE_FLAG_AUTO_RELEASE_UREF	0x01UL
#define CNODE_GRANT_PREPARE_FLAG_AUTO_RELEASE_RREF	0x02UL

#define CNODE_GRANT_PREPARE_FLAG_GRANT_CAN_MOVE 	0x04UL

#define CNODE_GRANT_PREPARE_FLAG_QUERY_SRC_CNODE_IDX 	0x08UL
#define CNODE_GRANT_PREPARE_FLAG_QUERY_USLOT_IDX	0x10UL
/*
 * Currently, there is such an authorization management model in sysmgr:
 *
 * ```
 *       client            sysmgr                        server
 *                           |                             |
 *                           |                             V
 *                           |                        create a new
 *                           |                     capability - cref_A
 *                           V
 *        use the client_cnode_rref to proxy require
 *            the grant of the cref_A - rref_A.
 *                           |
 *                           |
 *                           V
 *            use the server_cnode_rref to proxy
 *           grant the cref_A to client - uref_A.
 *                           |
 *                           |
 *                           V
 *        the authorization is established, record and use the
 *        rref_A/uref_A to do something for the client/server.
 *                           |
 *                        .......
 *                           |
 *                           V
 *           proxy reject/revoke the rref_A/uref_A
 * ```
 *
 * Obviously, the rref_A/uref_A could be rejected/revoked by the client/server
 * concurrently. If it happened, the sysmgr would record an invalid rref/uref.
 * More seriously, due to the rref/uref confusion, the sysmgr might record and
 * reject/revoke an unexpected rref/uref which might be using somewhere.
 *
 * To avoid the problem, introduce the flag to indicate the authorization is
 * established by proxy. And only the caller with the CPERMISSION_CNODE_PROXY_GRANT
 * can establish and reject the proxy authorization.
 */
#define CNODE_GRANT_PREPARE_FLAG_PROXY_GRANT		0x20UL

#define CNODE_STATCAP_TYPE		1UL

#define CNODE_STATRREF_TYPE		1UL
#define CNODE_STATRREF_STATE		2UL
#define CNODE_STATRREF_SRC_CNODE_IDX	4UL
#define CNODE_STATRREF_USLOT_IDX	8UL

#define CNODE_STATUREF_TYPE		1UL
#define CNODE_STATUREF_STATE		2UL
#define CNODE_STATUREF_DST_CNODE_IDX	4UL
#define CNODE_STATUREF_CREF		8UL

#define CNODE_CLIMIT_INFINITY	(-1)

#define CNODE_ITERATERTABLE_STATE_GRANTING	1UL
#define CNODE_ITERATERTABLE_STATE_GRANTED	2UL
#define CNODE_ITERATERTABLE_STATE_DEPRECATED	4UL

#define CNODE_ITERATEUTABLE_STATE_GRANTING	1UL
#define CNODE_ITERATEUTABLE_STATE_GRANTED	2UL
#define CNODE_ITERATEUTABLE_STATE_DEPRECATED	4UL

struct sysarg_cnode_statcap {
	__u32 captype;
};

struct sysarg_cnode_statrref {
	__u32 captype;
	__u32 state;
	__u32 src_cnode_idx;
	__u32 uslot_idx;
};

struct sysarg_cnode_staturef {
	__u32 captype;
	__u32 state;
	__u32 dst_cnode_idx;
	cref_t cref;
};

struct sysarg_cnode_climit {
	__s32 climits[hmobj_MAX];
};

#define CNODE_STAT_CACHE_MISS	1UL
#define CNODE_STAT_CACHE_HIT	2UL
#define CNODE_STAT_TABLE_FREE	4UL
#define CNODE_STAT_RUNTIME	8UL

struct sysarg_cnode_stat {
	__u64 cache_hit;
	__u64 cache_miss;
	__u64 runtime;
	__u32 ctable_free;
	__u32 utable_free;
	__u32 rtable_free;
};

#define CNODE_SPAWNFLAGS(rights)	((unsigned long)((rights) & 0xffU))

struct sysarg_cnode_spawn_req_pmem {
	cref_t base_pmem_cref;
	__u32 left;
};

struct sysarg_cnode_spawn_req {
	struct sysarg_cnode_spawn_req_pmem cnode;
	struct sysarg_cnode_spawn_req_pmem ctable;
	struct sysarg_cnode_spawn_req_pmem utable;
	struct sysarg_cnode_spawn_req_pmem rtable;
	__u64 kobj_process_ptr;
	__u8 right;
	__u8 is_system_service;
};

struct sysarg_cnode_spawn_req_page {
	unsigned long vstart;
	unsigned int nrpage;
};

struct sysarg_cnode_spawn_res {
	cref_t cnode_cref;
	rref_t cnode_hdlr_rref;
	uref_t cnode_hdlr_uref;
};

enum sysarg_cnode_rstate {
	RSTATE_GRANTING,
	RSTATE_GRANTED,
	RSTATE_DEPRECATED,
	RSTATE_INVALID,
};

enum sysarg_cnode_ustate {
	USTATE_GRANTING,
	USTATE_GRANTED,
	USTATE_DEPRECATED,
	USTATE_INVALID,
};

#endif
#endif
