/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for freqmgr
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../kernel/hongmeng/hm-verif-kernel/ulibs/include/libsysif/freqmgr/api.sysif -I ../kernel/hongmeng/hm-verif-kernel/ulibs/include --api-dest ../image/usr/include/generated/sysif_client/freqmgr/libsysif/freqmgr/api.h --server-dest ../image/usr/include/generated/sysif_server/freqmgr/libsysif/freqmgr/server.h --dump-callno ../image/usr/include/generated/dump_info/freqmgr/libsysif/freqmgr/l2h_map.h --legacy-mode
* Create: Fri Oct 31 10:27:09 2025
*/
#ifndef LIBSYSIF_FREQMGR_SERVER_H
#define LIBSYSIF_FREQMGR_SERVER_H
#ifndef LIBSYSIF_FREQMGR_COMMON_H
#define LIBSYSIF_FREQMGR_COMMON_H
#include <libsysif/utils_common.h>
#include <hongmeng/compiler.h>
#include <hongmeng/types.h>
#include <hmasm/actv_rpc.h>
#ifndef LIBSYSIF_BASE_API_COMMON_H
#define LIBSYSIF_BASE_API_COMMON_H
#include <stddef.h>
#include <stdint.h>
#include <hmkernel/capability.h>
struct sysif_reply_header {
	int32_t retval;
	uint32_t msgsz;
};

#define SYSIF_REPLY_LEN_MAX 256UL
extern void* hm_actvpool_prepare_ret(unsigned long long sender, unsigned long credential, unsigned long size);


extern struct arch_actv_local* hm_actv_current_arch_actv_local(void);


#endif
#include <libbunch/bunch_ipc.h>
#include <libhmpm/dfc.h>

#define FREQMGR_PATH	"freqmgr_actv"



#define DFC_COMP_NAME_MAX			16
#define DFC_CPU_FREQ_LIMIT_MIN			0U
#define DFC_CPU_FREQ_LIMIT_MAX			1U
#define DFC_CPU_FREQ_QUERY_CUR			0U
#define DFC_CPU_FREQ_QUERY_MAX			1U
#define DFC_CPU_HW_FREQ_QUERY_MAX		2U

struct dfc_component_name {
	char string[DFC_COMP_NAME_MAX];
};


raw_static_assert(sizeof(struct dfc_component_name) <= 96,
                    fmcall_dfc_freq_stat_arg_component_is_too_large);
raw_static_assert(sizeof(struct bunch_ipc_attr) <= 96,
                    fmcall_dfc_freq_stat_arg_attr_is_too_large);
raw_static_assert(sizeof(enum dfc_constraint_reason) <= 96,
                    fmcall_dfc_cpu_freq_limit_arg_reason_is_too_large);
struct __actvret_fmcall_dfc_freq_stat {
};

struct __actvret_fmcall_dfc_cpu_freq_limit {
};

struct __actvret_fmcall_dfc_cpu_freq_query {
	unsigned int v;
};

#ifdef __aarch64__
union __actvret_fmcall {
	struct __actvret_fmcall_dfc_freq_stat dfc_freq_stat;
	struct __actvret_fmcall_dfc_cpu_freq_limit dfc_cpu_freq_limit;
	struct __actvret_fmcall_dfc_cpu_freq_query dfc_cpu_freq_query;
};

#elif defined(__arm__)
union __actvret_fmcall {
	struct __actvret_fmcall_dfc_freq_stat dfc_freq_stat;
	struct __actvret_fmcall_dfc_cpu_freq_limit dfc_cpu_freq_limit;
	struct __actvret_fmcall_dfc_cpu_freq_query dfc_cpu_freq_query;
};

#else
union __actvret_fmcall {
	struct __actvret_fmcall_dfc_freq_stat dfc_freq_stat;
	struct __actvret_fmcall_dfc_cpu_freq_limit dfc_cpu_freq_limit;
	struct __actvret_fmcall_dfc_cpu_freq_query dfc_cpu_freq_query;
};

#endif
enum __freqmgr_mgr {
	__freqmgr_INVAL_mgr=0,
	__freqmgr_dfc_mgr,
	__freqmgr_MAX_mgr,
};

#define __freqmgr_method_MIN __ACTV_HIGHTABLE_START
#define __freqmgr_method_dfc_freq_stat (__ACTV_HIGHTABLE_START + 1)
#define __freqmgr_method_dfc_cpu_freq_limit (__ACTV_HIGHTABLE_START + 2)
#define __freqmgr_method_dfc_cpu_freq_query (__ACTV_HIGHTABLE_START + 3)
#define __freqmgr_method_dfc_rsvd_1 (__ACTV_HIGHTABLE_START + 4)
#define __freqmgr_method_MAX (__ACTV_HIGHTABLE_START + 5)
raw_static_assert(sizeof(struct __actvret_fmcall_dfc_freq_stat) < 512,
            fmcall_dfc_freq_stat_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fmcall_dfc_cpu_freq_limit) < 512,
            fmcall_dfc_cpu_freq_limit_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_fmcall_dfc_cpu_freq_query) < 512,
            fmcall_dfc_cpu_freq_query_too_many_actvret_data);
raw_static_assert(sizeof(union __actvret_fmcall) < 512,
        fmcall_too_many_actvret_data);
extern int __actvcall_fmcall_dfc_freq_stat(unsigned long long __rpc_info, void *__buf, struct dfc_component_name component, struct bunch_ipc_attr attr);


extern int __actvcapcall_fmcall_dfc_freq_stat(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct dfc_component_name component, struct bunch_ipc_attr attr);


static inline int __attribute__((always_inline)) actvxactcall_fmcall_dfc_freq_stat(bool is_xact_fwd, bool is_xact_allow_refwd, struct dfc_component_name component, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __freqmgr_method_dfc_freq_stat;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct dfc_component_name), __arch_actv_rpc_stksz_alignment(struct dfc_component_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fmcall_dfc_freq_stat(rpc_info, NULL, component, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fmcall_dfc_freq_stat(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct dfc_component_name component, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __freqmgr_method_dfc_freq_stat;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct dfc_component_name), __arch_actv_rpc_stksz_alignment(struct dfc_component_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fmcall_dfc_freq_stat(__rref, rpc_info, NULL, component, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fmcall_dfc_freq_stat(struct dfc_component_name component, struct bunch_ipc_attr attr)
{
	return actvxactcall_fmcall_dfc_freq_stat(false, false, component, attr);
}

static inline int __attribute__((always_inline)) actvcapcall_fmcall_dfc_freq_stat(rref_t __rref, struct dfc_component_name component, struct bunch_ipc_attr attr)
{
	return actvxactcapcall_fmcall_dfc_freq_stat(false, false, __rref, component, attr);
}

extern int __actvcall_fmcall_dfc_cpu_freq_limit(unsigned long long __rpc_info, void *__buf, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason);


extern int __actvcapcall_fmcall_dfc_cpu_freq_limit(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason);


static inline int __attribute__((always_inline)) actvxactcall_fmcall_dfc_cpu_freq_limit(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason)
{
	const unsigned long __callno = __freqmgr_method_dfc_cpu_freq_limit;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum dfc_constraint_reason), __arch_actv_rpc_stksz_alignment(enum dfc_constraint_reason));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_fmcall_dfc_cpu_freq_limit(rpc_info, NULL, cpuid, target_freq, limit_type, reason);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fmcall_dfc_cpu_freq_limit(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason)
{
	const unsigned long __callno = __freqmgr_method_dfc_cpu_freq_limit;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum dfc_constraint_reason), __arch_actv_rpc_stksz_alignment(enum dfc_constraint_reason));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fmcall_dfc_cpu_freq_limit(__rref, rpc_info, NULL, cpuid, target_freq, limit_type, reason);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fmcall_dfc_cpu_freq_limit(unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason)
{
	return actvxactcall_fmcall_dfc_cpu_freq_limit(false, false, cpuid, target_freq, limit_type, reason);
}

static inline int __attribute__((always_inline)) actvcapcall_fmcall_dfc_cpu_freq_limit(rref_t __rref, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason)
{
	return actvxactcapcall_fmcall_dfc_cpu_freq_limit(false, false, __rref, cpuid, target_freq, limit_type, reason);
}

extern int __actvcall_fmcall_dfc_cpu_freq_query(unsigned long long __rpc_info, void *__buf, unsigned int cpuid, unsigned int query_type);


extern int __actvcapcall_fmcall_dfc_cpu_freq_query(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cpuid, unsigned int query_type);


static inline int __attribute__((always_inline)) actvxactcall_fmcall_dfc_cpu_freq_query(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cpuid, unsigned int query_type, struct __actvret_fmcall_dfc_cpu_freq_query *__ret)
{
	const unsigned long __callno = __freqmgr_method_dfc_cpu_freq_query;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_fmcall_dfc_cpu_freq_query(rpc_info, __ret, cpuid, query_type);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_fmcall_dfc_cpu_freq_query(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cpuid, unsigned int query_type, struct __actvret_fmcall_dfc_cpu_freq_query *__ret)
{
	const unsigned long __callno = __freqmgr_method_dfc_cpu_freq_query;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_fmcall_dfc_cpu_freq_query(__rref, rpc_info, __ret, cpuid, query_type);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_fmcall_dfc_cpu_freq_query(unsigned int cpuid, unsigned int query_type, struct __actvret_fmcall_dfc_cpu_freq_query *__ret)
{
	return actvxactcall_fmcall_dfc_cpu_freq_query(false, false, cpuid, query_type, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_fmcall_dfc_cpu_freq_query(rref_t __rref, unsigned int cpuid, unsigned int query_type, struct __actvret_fmcall_dfc_cpu_freq_query *__ret)
{
	return actvxactcapcall_fmcall_dfc_cpu_freq_query(false, false, __rref, cpuid, query_type, __ret);
}

#endif
#include <libsysif/utils.h>
#include <hmasm/actv_rpc.h>
#include <hmasm/types.h>
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
extern int actvhdlr_fmhandler_dfc_freq_stat(unsigned long long sender, unsigned long credential, struct dfc_component_name component, struct bunch_ipc_attr attr);


extern int actvhdlr_fmhandler_dfc_cpu_freq_limit(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason);


extern int actvhdlr_fmhandler_dfc_cpu_freq_query(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int query_type);


#else /* aarch64 */
extern int actvhdlr_fmhandler_dfc_freq_stat(unsigned long long sender, unsigned long credential, struct dfc_component_name component, struct bunch_ipc_attr attr);


extern int actvhdlr_fmhandler_dfc_cpu_freq_limit(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason);


extern int actvhdlr_fmhandler_dfc_cpu_freq_query(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int query_type);


extern int __actvshadowhdlr_fmhandler_dfc_freq_stat(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fmhandler_dfc_cpu_freq_limit(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_fmhandler_dfc_cpu_freq_query(unsigned long long sender, unsigned long credential, ...);


#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
static inline struct __actvret_fmcall_dfc_freq_stat* actvhdlr_fmhandler_dfc_freq_stat_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fmcall_dfc_freq_stat));
	return (struct __actvret_fmcall_dfc_freq_stat*)__ret;
}

static inline struct __actvret_fmcall_dfc_cpu_freq_limit* actvhdlr_fmhandler_dfc_cpu_freq_limit_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fmcall_dfc_cpu_freq_limit));
	return (struct __actvret_fmcall_dfc_cpu_freq_limit*)__ret;
}

static inline struct __actvret_fmcall_dfc_cpu_freq_query* actvhdlr_fmhandler_dfc_cpu_freq_query_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_fmcall_dfc_cpu_freq_query));
	return (struct __actvret_fmcall_dfc_cpu_freq_query*)__ret;
}

#ifdef __sysif_server_freqmgr_no_compat32_handlers__
#ifdef __HOST_LLT__
union freqmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_fmhandler_dfc_freq_stat)(unsigned long long sender, unsigned long credential, struct dfc_component_name component, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_fmhandler_dfc_freq_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct dfc_component_name component, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_fmhandler_dfc_cpu_freq_limit)(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason);
	int (*p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason);
	int (*p__actvbridgehdlr_fmhandler_dfc_cpu_freq_query)(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int query_type);
	int (*p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int query_type);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union freqmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_fmhandler_dfc_freq_stat)(unsigned long long sender, unsigned long credential, struct dfc_component_name component, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_fmhandler_dfc_freq_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct dfc_component_name component, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_fmhandler_dfc_cpu_freq_limit)(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason);
	int (*p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason);
	int (*p__actvbridgehdlr_fmhandler_dfc_cpu_freq_query)(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int query_type);
	int (*p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int query_type);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#ifdef __arm__
union freqmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_fmhandler_dfc_freq_stat)(unsigned long long sender, unsigned long credential, struct dfc_component_name component, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_fmhandler_dfc_freq_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct dfc_component_name component, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_fmhandler_dfc_cpu_freq_limit)(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason);
	int (*p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason);
	int (*p__actvbridgehdlr_fmhandler_dfc_cpu_freq_query)(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int query_type);
	int (*p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int query_type);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential);
};

#endif /* arm */
#endif /* not HOST_LLT */
#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif /* not __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#ifdef __HOST_LLT__
union freqmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_fmhandler_dfc_freq_stat)(unsigned long long sender, unsigned long credential, struct dfc_component_name component, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_fmhandler_dfc_freq_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct dfc_component_name component, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_fmhandler_dfc_cpu_freq_limit)(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason);
	int (*p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason);
	int (*p__actvbridgehdlr_fmhandler_dfc_cpu_freq_query)(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int query_type);
	int (*p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int query_type);
	int (*p__actvbridgehdlr_compat_fmhandler_dfc_freq_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fmhandler_dfc_freq_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fmhandler_dfc_cpu_freq_limit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fmhandler_dfc_cpu_freq_limit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fmhandler_dfc_cpu_freq_query)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fmhandler_dfc_cpu_freq_query)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union freqmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_fmhandler_dfc_freq_stat)(unsigned long long sender, unsigned long credential, struct dfc_component_name component, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_fmhandler_dfc_freq_stat)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct dfc_component_name component, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_fmhandler_dfc_cpu_freq_limit)(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason);
	int (*p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason);
	int (*p__actvbridgehdlr_fmhandler_dfc_cpu_freq_query)(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int query_type);
	int (*p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int query_type);
	int (*p__actvbridgehdlr_compat_fmhandler_dfc_freq_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fmhandler_dfc_freq_stat)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fmhandler_dfc_cpu_freq_limit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fmhandler_dfc_cpu_freq_limit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_fmhandler_dfc_cpu_freq_query)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_fmhandler_dfc_cpu_freq_query)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#endif /* not HOST_LLT */
#endif /* compat32 */

#if defined(__sysif_server_freqmgr_no_compat32_handlers__) && defined(__sysif_server_freqmgr_no_ilp32_handlers__)
#define __freqmgr_method_TIMES 1
#elif !defined(__sysif_server_freqmgr_no_compat32_handlers__) && !defined(__sysif_server_freqmgr_no_ilp32_handlers__)
#define __freqmgr_method_TIMES 3
#else
#define __freqmgr_method_TIMES 2
#endif
extern union freqmgr_hdlrs_union sysif_freqmgr_actvhdlr_table[__freqmgr_method_MAX * __freqmgr_method_TIMES];

extern union freqmgr_hdlrs_union sysif_freqmgr_actvcapcallhdlr_table[__freqmgr_method_MAX * __freqmgr_method_TIMES];

extern void sysif_show_freqmgr_methods(void);


#ifdef SYSIF_GEN_IFACE
#include <stdint.h>
#include <hmasm/actv_rpc.h>
#include <hongmeng/syscall.h>
#include <hongmeng/panic.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>
#include <libsysif/base/server_arg_compat.h>
#ifdef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
static int __opt_nonoframe __actvbridgehdlr_fmhandler_dfc_freq_stat(unsigned long long sender, unsigned long credential, struct dfc_component_name component, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct dfc_component_name), __arch_actv_rpc_stksz_alignment(struct dfc_component_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fmhandler_dfc_freq_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct dfc_component_name, component), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fmhandler_dfc_freq_stat(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct dfc_component_name component, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct dfc_component_name), __arch_actv_rpc_stksz_alignment(struct dfc_component_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fmhandler_dfc_freq_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct dfc_component_name, component), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_fmhandler_dfc_cpu_freq_limit(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum dfc_constraint_reason), __arch_actv_rpc_stksz_alignment(enum dfc_constraint_reason));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fmhandler_dfc_cpu_freq_limit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpuid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, target_freq), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, limit_type), __ARCH_ACTV_RPC_RECV_ARG(enum dfc_constraint_reason, reason));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum dfc_constraint_reason), __arch_actv_rpc_stksz_alignment(enum dfc_constraint_reason));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fmhandler_dfc_cpu_freq_limit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpuid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, target_freq), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, limit_type), __ARCH_ACTV_RPC_RECV_ARG(enum dfc_constraint_reason, reason));
}

static int __opt_nonoframe __actvbridgehdlr_fmhandler_dfc_cpu_freq_query(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int query_type)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fmhandler_dfc_cpu_freq_query(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpuid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, query_type));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int query_type)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fmhandler_dfc_cpu_freq_query(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpuid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, query_type));
}

#ifndef __sysif_server_freqmgr_no_compat32_handlers__
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
struct compat_x3 {
	unsigned int reserverd;
	unsigned int w3;
};

#else
struct compat_x3 {
	unsigned int w3;
	unsigned int reserverd;
};

#endif /* CONFIG_BIG_ENDIAN */
static int __opt_nonoframe __actvbridgehdlr_compat_fmhandler_dfc_freq_stat(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct dfc_component_name __maybe_unused _component;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct dfc_component_name);
		type = __type_of(struct dfc_component_name);
		__fetch_one_arg((void*)&_component, data, 0, arg_sz, model, type, struct dfc_component_name, _component)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct bunch_ipc_attr);
		type = __type_of(struct bunch_ipc_attr);
		__fetch_one_arg((void*)&_attr, data, 1, arg_sz, model, type, struct bunch_ipc_attr, _attr)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fmhandler_dfc_freq_stat(sender, credential, _component, _attr);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fmhandler_dfc_freq_stat(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	struct dfc_component_name __maybe_unused _component;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct dfc_component_name);
		type = __type_of(struct dfc_component_name);
		__fetch_one_arg((void*)&_component, data, 0, arg_sz, model, type, struct dfc_component_name, _component)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct bunch_ipc_attr);
		type = __type_of(struct bunch_ipc_attr);
		__fetch_one_arg((void*)&_attr, data, 1, arg_sz, model, type, struct bunch_ipc_attr, _attr)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fmhandler_dfc_freq_stat(sender, credential, _component, _attr);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fmhandler_dfc_cpu_freq_limit(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _cpuid;
	unsigned int __maybe_unused _target_freq;
	unsigned int __maybe_unused _limit_type;
	enum dfc_constraint_reason __maybe_unused _reason;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_cpuid, data, 0, arg_sz, model, type, unsigned int, _cpuid)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_target_freq, data, 1, arg_sz, model, type, unsigned int, _target_freq)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_limit_type, data, 2, arg_sz, model, type, unsigned int, _limit_type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(enum dfc_constraint_reason);
		type = __type_of(enum dfc_constraint_reason);
		__fetch_one_arg((void*)&_reason, data, 3, arg_sz, model, type, enum dfc_constraint_reason, _reason)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fmhandler_dfc_cpu_freq_limit(sender, credential, _cpuid, _target_freq, _limit_type, _reason);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fmhandler_dfc_cpu_freq_limit(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _cpuid;
	unsigned int __maybe_unused _target_freq;
	unsigned int __maybe_unused _limit_type;
	enum dfc_constraint_reason __maybe_unused _reason;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_cpuid, data, 0, arg_sz, model, type, unsigned int, _cpuid)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_target_freq, data, 1, arg_sz, model, type, unsigned int, _target_freq)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_limit_type, data, 2, arg_sz, model, type, unsigned int, _limit_type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(enum dfc_constraint_reason);
		type = __type_of(enum dfc_constraint_reason);
		__fetch_one_arg((void*)&_reason, data, 3, arg_sz, model, type, enum dfc_constraint_reason, _reason)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fmhandler_dfc_cpu_freq_limit(sender, credential, _cpuid, _target_freq, _limit_type, _reason);
}

static int __opt_nonoframe __actvbridgehdlr_compat_fmhandler_dfc_cpu_freq_query(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_NONE;
	bool __maybe_unused is_r3_passing = true;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _cpuid;
	unsigned int __maybe_unused _query_type;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_cpuid, data, 0, arg_sz, model, type, unsigned int, _cpuid)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_query_type, data, 1, arg_sz, model, type, unsigned int, _query_type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fmhandler_dfc_cpu_freq_query(sender, credential, _cpuid, _query_type);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_fmhandler_dfc_cpu_freq_query(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
{
	__maybe_unused union {
		unsigned long x3;
		struct compat_x3 __compat_x3;
	} ___val = {
		0
	};
	___val.x3 = x3;
	unsigned int __maybe_unused ___w3 = ___val.__compat_x3.w3;
	__maybe_unused char *data = (char*)(uintptr_t)sp;
	unsigned int __maybe_unused cursor = __RPC_ACTVCALL_HDLR_UNUSED_ARGS_ONE;
	bool __maybe_unused is_r3_passing = false;
	enum __data_model __maybe_unused model = _DATA_MODEL_ILP32;
	enum __type __maybe_unused type;
	unsigned int __maybe_unused _cpuid;
	unsigned int __maybe_unused _query_type;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_cpuid, data, 0, arg_sz, model, type, unsigned int, _cpuid)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_query_type, data, 1, arg_sz, model, type, unsigned int, _query_type)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	if (unlikely(__nr_ul * sizeof(__u32) != cursor)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_fmhandler_dfc_cpu_freq_query(sender, credential, _cpuid, _query_type);
}

#endif /* compat handlers */
#ifndef __sysif_server_freqmgr_no_ilp32_handlers__
static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fmhandler_dfc_freq_stat(unsigned long long sender, unsigned long credential, struct dfc_component_name component, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct dfc_component_name), __arch_actv_rpc_stksz_alignment(struct dfc_component_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct dfc_component_name, component)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_fmhandler_dfc_freq_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct dfc_component_name, component), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fmhandler_dfc_freq_stat(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct dfc_component_name component, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct dfc_component_name), __arch_actv_rpc_stksz_alignment(struct dfc_component_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct dfc_component_name, component)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_fmhandler_dfc_freq_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct dfc_component_name, component), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fmhandler_dfc_cpu_freq_limit(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum dfc_constraint_reason), __arch_actv_rpc_stksz_alignment(enum dfc_constraint_reason));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, cpuid)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, target_freq)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, limit_type)
	} while (0);
	do {
		__adjust_one_arg(enum dfc_constraint_reason, reason)
	} while (0);
	return __actvshadowhdlr_fmhandler_dfc_cpu_freq_limit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpuid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, target_freq), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, limit_type), __ARCH_ACTV_RPC_RECV_ARG(enum dfc_constraint_reason, reason));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fmhandler_dfc_cpu_freq_limit(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum dfc_constraint_reason), __arch_actv_rpc_stksz_alignment(enum dfc_constraint_reason));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, cpuid)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, target_freq)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, limit_type)
	} while (0);
	do {
		__adjust_one_arg(enum dfc_constraint_reason, reason)
	} while (0);
	return __actvshadowhdlr_fmhandler_dfc_cpu_freq_limit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpuid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, target_freq), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, limit_type), __ARCH_ACTV_RPC_RECV_ARG(enum dfc_constraint_reason, reason));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_fmhandler_dfc_cpu_freq_query(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int query_type)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, cpuid)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, query_type)
	} while (0);
	return __actvshadowhdlr_fmhandler_dfc_cpu_freq_query(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpuid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, query_type));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_fmhandler_dfc_cpu_freq_query(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int query_type)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, cpuid)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, query_type)
	} while (0);
	return __actvshadowhdlr_fmhandler_dfc_cpu_freq_query(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpuid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, query_type));
}

#endif
#else
static int __opt_nonoframe __actvbridgehdlr_fmhandler_dfc_freq_stat(unsigned long long sender, unsigned long credential, struct dfc_component_name component, struct bunch_ipc_attr attr)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct dfc_component_name), __arch_actv_rpc_stksz_alignment(struct dfc_component_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fmhandler_dfc_freq_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct dfc_component_name, component), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fmhandler_dfc_freq_stat(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct dfc_component_name component, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct dfc_component_name), __arch_actv_rpc_stksz_alignment(struct dfc_component_name));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fmhandler_dfc_freq_stat(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct dfc_component_name, component), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_fmhandler_dfc_cpu_freq_limit(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum dfc_constraint_reason), __arch_actv_rpc_stksz_alignment(enum dfc_constraint_reason));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fmhandler_dfc_cpu_freq_limit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpuid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, target_freq), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, limit_type), __ARCH_ACTV_RPC_RECV_ARG(enum dfc_constraint_reason, reason));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum dfc_constraint_reason), __arch_actv_rpc_stksz_alignment(enum dfc_constraint_reason));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fmhandler_dfc_cpu_freq_limit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpuid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, target_freq), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, limit_type), __ARCH_ACTV_RPC_RECV_ARG(enum dfc_constraint_reason, reason));
}

static int __opt_nonoframe __actvbridgehdlr_fmhandler_dfc_cpu_freq_query(unsigned long long sender, unsigned long credential, unsigned int cpuid, unsigned int query_type)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fmhandler_dfc_cpu_freq_query(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpuid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, query_type));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cpuid, unsigned int query_type)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_fmhandler_dfc_cpu_freq_query(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cpuid), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, query_type));
}

#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#if defined(__sysif_server_freqmgr_no_compat32_handlers__) && defined(__sysif_server_freqmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union freqmgr_hdlrs_union sysif_freqmgr_actvhdlr_table[__freqmgr_method_MAX * __freqmgr_method_TIMES] = {
	[0 ... __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgehdlr_fmhandler_dfc_freq_stat = (&__actvbridgehdlr_fmhandler_dfc_freq_stat)
	},
	[ __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgehdlr_fmhandler_dfc_cpu_freq_limit = (&__actvbridgehdlr_fmhandler_dfc_cpu_freq_limit)
	},
	[ __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgehdlr_fmhandler_dfc_cpu_freq_query = (&__actvbridgehdlr_fmhandler_dfc_cpu_freq_query)
	}
};

__ro_after_init_hdlr union freqmgr_hdlrs_union sysif_freqmgr_actvcapcallhdlr_table[__freqmgr_method_MAX * __freqmgr_method_TIMES] = {
	[0 ... __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_freq_stat = (&__actvbridgecapcallhdlr_fmhandler_dfc_freq_stat)
	},
	[ __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit = (&__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit)
	},
	[ __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query = (&__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query)
	}
};

#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif
#if !defined(__sysif_server_freqmgr_no_compat32_handlers__) && defined(__sysif_server_freqmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union freqmgr_hdlrs_union sysif_freqmgr_actvhdlr_table[__freqmgr_method_MAX * __freqmgr_method_TIMES] = {
	[0 ... __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgehdlr_fmhandler_dfc_freq_stat = (&__actvbridgehdlr_fmhandler_dfc_freq_stat)
	},
	[ __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgehdlr_fmhandler_dfc_cpu_freq_limit = (&__actvbridgehdlr_fmhandler_dfc_cpu_freq_limit)
	},
	[ __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgehdlr_fmhandler_dfc_cpu_freq_query = (&__actvbridgehdlr_fmhandler_dfc_cpu_freq_query)
	},
	[__freqmgr_method_MAX ... __freqmgr_method_MAX + __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgehdlr_compat_fmhandler_dfc_freq_stat = (&__actvbridgehdlr_compat_fmhandler_dfc_freq_stat)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgehdlr_compat_fmhandler_dfc_cpu_freq_limit = (&__actvbridgehdlr_compat_fmhandler_dfc_cpu_freq_limit)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgehdlr_compat_fmhandler_dfc_cpu_freq_query = (&__actvbridgehdlr_compat_fmhandler_dfc_cpu_freq_query)
	}
};

__ro_after_init_hdlr union freqmgr_hdlrs_union sysif_freqmgr_actvcapcallhdlr_table[__freqmgr_method_MAX * __freqmgr_method_TIMES] = {
	[0 ... __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_freq_stat = (&__actvbridgecapcallhdlr_fmhandler_dfc_freq_stat)
	},
	[ __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit = (&__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit)
	},
	[ __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query = (&__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query)
	},
	[__freqmgr_method_MAX ... __freqmgr_method_MAX + __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgecapcallhdlr_compat_fmhandler_dfc_freq_stat = (&__actvbridgecapcallhdlr_compat_fmhandler_dfc_freq_stat)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgecapcallhdlr_compat_fmhandler_dfc_cpu_freq_limit = (&__actvbridgecapcallhdlr_compat_fmhandler_dfc_cpu_freq_limit)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgecapcallhdlr_compat_fmhandler_dfc_cpu_freq_query = (&__actvbridgecapcallhdlr_compat_fmhandler_dfc_cpu_freq_query)
	}
};

#endif /* aarch64 + compat */
#if defined(__sysif_server_freqmgr_no_compat32_handlers__) && !defined(__sysif_server_freqmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union freqmgr_hdlrs_union sysif_freqmgr_actvhdlr_table[__freqmgr_method_MAX * __freqmgr_method_TIMES] = {
	[0 ... __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgehdlr_fmhandler_dfc_freq_stat = (&__actvbridgehdlr_fmhandler_dfc_freq_stat)
	},
	[ __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgehdlr_fmhandler_dfc_cpu_freq_limit = (&__actvbridgehdlr_fmhandler_dfc_cpu_freq_limit)
	},
	[ __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgehdlr_fmhandler_dfc_cpu_freq_query = (&__actvbridgehdlr_fmhandler_dfc_cpu_freq_query)
	},
	[__freqmgr_method_MAX ... __freqmgr_method_MAX + __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgehdlr_fmhandler_dfc_freq_stat = (&__actvbridgehdlr_ilp32_fmhandler_dfc_freq_stat)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgehdlr_fmhandler_dfc_cpu_freq_limit = (&__actvbridgehdlr_ilp32_fmhandler_dfc_cpu_freq_limit)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgehdlr_fmhandler_dfc_cpu_freq_query = (&__actvbridgehdlr_ilp32_fmhandler_dfc_cpu_freq_query)
	}
};

__ro_after_init_hdlr union freqmgr_hdlrs_union sysif_freqmgr_actvcapcallhdlr_table[__freqmgr_method_MAX * __freqmgr_method_TIMES] = {
	[0 ... __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_freq_stat = (&__actvbridgecapcallhdlr_fmhandler_dfc_freq_stat)
	},
	[ __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit = (&__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit)
	},
	[ __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query = (&__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query)
	},
	[__freqmgr_method_MAX ... __freqmgr_method_MAX + __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_freq_stat = (&__actvbridgecapcallhdlr_ilp32_fmhandler_dfc_freq_stat)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit = (&__actvbridgecapcallhdlr_ilp32_fmhandler_dfc_cpu_freq_limit)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query = (&__actvbridgecapcallhdlr_ilp32_fmhandler_dfc_cpu_freq_query)
	}
};

#endif /* aarch64 + ilp32 */
#if !defined(__sysif_server_freqmgr_no_compat32_handlers__) && !defined(__sysif_server_freqmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union freqmgr_hdlrs_union sysif_freqmgr_actvhdlr_table[__freqmgr_method_MAX * __freqmgr_method_TIMES] = {
	[0 ... __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgehdlr_fmhandler_dfc_freq_stat = (&__actvbridgehdlr_fmhandler_dfc_freq_stat)
	},
	[ __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgehdlr_fmhandler_dfc_cpu_freq_limit = (&__actvbridgehdlr_fmhandler_dfc_cpu_freq_limit)
	},
	[ __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgehdlr_fmhandler_dfc_cpu_freq_query = (&__actvbridgehdlr_fmhandler_dfc_cpu_freq_query)
	},
	[__freqmgr_method_MAX ... __freqmgr_method_MAX + __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgehdlr_compat_fmhandler_dfc_freq_stat = (&__actvbridgehdlr_compat_fmhandler_dfc_freq_stat)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgehdlr_compat_fmhandler_dfc_cpu_freq_limit = (&__actvbridgehdlr_compat_fmhandler_dfc_cpu_freq_limit)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgehdlr_compat_fmhandler_dfc_cpu_freq_query = (&__actvbridgehdlr_compat_fmhandler_dfc_cpu_freq_query)
	},
	[2 * __freqmgr_method_MAX ... 2 * __freqmgr_method_MAX + __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __freqmgr_method_MAX + __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __freqmgr_method_MAX + __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgehdlr_fmhandler_dfc_freq_stat = (&__actvbridgehdlr_ilp32_fmhandler_dfc_freq_stat)
	},
	[2 * __freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgehdlr_fmhandler_dfc_cpu_freq_limit = (&__actvbridgehdlr_ilp32_fmhandler_dfc_cpu_freq_limit)
	},
	[2 * __freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgehdlr_fmhandler_dfc_cpu_freq_query = (&__actvbridgehdlr_ilp32_fmhandler_dfc_cpu_freq_query)
	}
};

__ro_after_init_hdlr union freqmgr_hdlrs_union sysif_freqmgr_actvcapcallhdlr_table[__freqmgr_method_MAX * __freqmgr_method_TIMES] = {
	[0 ... __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_freq_stat = (&__actvbridgecapcallhdlr_fmhandler_dfc_freq_stat)
	},
	[ __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit = (&__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit)
	},
	[ __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query = (&__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query)
	},
	[__freqmgr_method_MAX ... __freqmgr_method_MAX + __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgecapcallhdlr_compat_fmhandler_dfc_freq_stat = (&__actvbridgecapcallhdlr_compat_fmhandler_dfc_freq_stat)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgecapcallhdlr_compat_fmhandler_dfc_cpu_freq_limit = (&__actvbridgecapcallhdlr_compat_fmhandler_dfc_cpu_freq_limit)
	},
	[__freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgecapcallhdlr_compat_fmhandler_dfc_cpu_freq_query = (&__actvbridgecapcallhdlr_compat_fmhandler_dfc_cpu_freq_query)
	},
	[2 * __freqmgr_method_MAX ... 2 * __freqmgr_method_MAX + __freqmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __freqmgr_method_MAX + __freqmgr_method_dfc_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __freqmgr_method_MAX + __freqmgr_method_dfc_freq_stat] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_freq_stat = (&__actvbridgecapcallhdlr_ilp32_fmhandler_dfc_freq_stat)
	},
	[2 * __freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_limit] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_limit = (&__actvbridgecapcallhdlr_ilp32_fmhandler_dfc_cpu_freq_limit)
	},
	[2 * __freqmgr_method_MAX + __freqmgr_method_dfc_cpu_freq_query] = {
		.p__actvbridgecapcallhdlr_fmhandler_dfc_cpu_freq_query = (&__actvbridgecapcallhdlr_ilp32_fmhandler_dfc_cpu_freq_query)
	}
};

#endif /* aarch64 + compat + ilp32 */
#endif /* arm or aarch64 only */
#ifdef __aarch64__
void sysif_show_freqmgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fmcall", "dfc", "freq_stat", __freqmgr_method_dfc_freq_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fmhandler", "dfc", "freq_stat", __freqmgr_method_dfc_freq_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fmcall", "dfc", "cpu_freq_limit", __freqmgr_method_dfc_cpu_freq_limit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fmhandler", "dfc", "cpu_freq_limit", __freqmgr_method_dfc_cpu_freq_limit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fmcall", "dfc", "cpu_freq_query", __freqmgr_method_dfc_cpu_freq_query);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fmhandler", "dfc", "cpu_freq_query", __freqmgr_method_dfc_cpu_freq_query);
	hm_info("%s_%s, max method num: %d\n", "freqmgr", "method", __freqmgr_method_MAX);
}

#elif defined(__arm__)
void sysif_show_freqmgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fmcall", "dfc", "freq_stat", __freqmgr_method_dfc_freq_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fmhandler", "dfc", "freq_stat", __freqmgr_method_dfc_freq_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fmcall", "dfc", "cpu_freq_limit", __freqmgr_method_dfc_cpu_freq_limit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fmhandler", "dfc", "cpu_freq_limit", __freqmgr_method_dfc_cpu_freq_limit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fmcall", "dfc", "cpu_freq_query", __freqmgr_method_dfc_cpu_freq_query);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fmhandler", "dfc", "cpu_freq_query", __freqmgr_method_dfc_cpu_freq_query);
	hm_info("%s_%s, max method num: %d\n", "freqmgr", "method", __freqmgr_method_MAX);
}

#else
void sysif_show_freqmgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fmcall", "dfc", "freq_stat", __freqmgr_method_dfc_freq_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fmhandler", "dfc", "freq_stat", __freqmgr_method_dfc_freq_stat);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fmcall", "dfc", "cpu_freq_limit", __freqmgr_method_dfc_cpu_freq_limit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fmhandler", "dfc", "cpu_freq_limit", __freqmgr_method_dfc_cpu_freq_limit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "fmcall", "dfc", "cpu_freq_query", __freqmgr_method_dfc_cpu_freq_query);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "fmhandler", "dfc", "cpu_freq_query", __freqmgr_method_dfc_cpu_freq_query);
	hm_info("%s_%s, max method num: %d\n", "freqmgr", "method", __freqmgr_method_MAX);
}

#endif
#endif
#endif
