/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for freqmgr
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../ulibs/include/libsysif/freqmgr/api.sysif -I ../ulibs/include --api-dest ../image/usr/include/generated/sysif_client/freqmgr/libsysif/freqmgr/api.h --server-dest ../image/usr/include/generated/sysif_server/freqmgr/libsysif/freqmgr/server.h --dump-callno ../image/usr/include/generated/dump_info/freqmgr/libsysif/freqmgr/l2h_map.h --legacy-mode
* Create: Mon Sep 22 11:44:31 2025
*/
#ifndef LIBSYSIF_FREQMGR_API_H
#define LIBSYSIF_FREQMGR_API_H
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
#ifdef SYSIF_GEN_IFACE
#include <hmkernel/compiler.h>
#include <hmasm/actv_rpc.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <hongmeng/panic.h>
#include <libstrict/strict.h>
#include <hmasm/lsyscall.h>
static int __used __noinline __opt_noframe __internal_rpccall_nobuf(void)
{
	hm_panic("calling unimplemented actvcall. Use actvcapcall or ebbcall instead\n");
	return E_HM_NOSYS;
}

static int __used __noinline __opt_noframe __internal_rpccall_buf(void)
{
	hm_panic("calling unimplemented actvcall. Use actvcapcall or ebbcall instead\n");
	return E_HM_NOSYS;
}

static int __used __noinline __opt_noframe __internal_rpccall_retbuf(void)
{
	hm_panic("calling unimplemented actvcall. Use actvcapcall or ebbcall instead\n");
	return E_HM_NOSYS;
}

static int __used __noinline __opt_noframe __internal_rpccall_fallback(void)
{
	hm_panic("calling unimplemented actvcall. Use actvcapcall or ebbcall instead\n");
	return E_HM_NOSYS;
}

static int __used __noinline __opt_noframe __internal_rpccapcall_nobuf(void)
{
	__RPC_DEF_RETVAR(int, ret);
	__RPC_CAP_SYSCALL(NoBuf, ret);
	return ret;
}

static int __used __noinline __opt_noframe __internal_rpccapcall_buf(void)
{
	__RPC_DEF_RETVAR(int, ret);
	__RPC_CAP_SYSCALL(SendBuf, ret);
	return ret;
}

static int __used __noinline __opt_noframe __internal_rpccapcall_retbuf(void)
{
	__RPC_DEF_RETVAR(int, ret);
	__RPC_CAP_SYSCALL(RetBuf, ret);
	return ret;
}

static int __used __noinline __opt_noframe __internal_rpccapcall_fallback(void)
{
	__RPC_DEF_RETVAR(int, ret);
	__RPC_CAP_SYSCALL(, ret);
	return ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fmcall_dfc_freq_stat(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fmcall_dfc_freq_stat(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fmcall_dfc_freq_stat(unsigned long long __rpc_info, void *__buf, struct dfc_component_name component, struct bunch_ipc_attr attr)
{
	int __ret;
	struct dfc_component_name __maybe_unused __actv_local_var_component = component;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcall_fmcall_dfc_freq_stat(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(component, __actv_local_var_component), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

int __internal_actvcall __actvcapcall_fmcall_dfc_freq_stat(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct dfc_component_name component, struct bunch_ipc_attr attr)
{
	int __ret;
	struct dfc_component_name __maybe_unused __actv_local_var_component = component;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcapcall_fmcall_dfc_freq_stat(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(component, __actv_local_var_component), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_fmcall_dfc_cpu_freq_limit(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_fmcall_dfc_cpu_freq_limit(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fmcall_dfc_cpu_freq_limit(unsigned long long __rpc_info, void *__buf, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cpuid = cpuid;
	unsigned int __maybe_unused __actv_local_var_target_freq = target_freq;
	unsigned int __maybe_unused __actv_local_var_limit_type = limit_type;
	enum dfc_constraint_reason __maybe_unused __actv_local_var_reason = reason;
	__ret = ____actvcall_fmcall_dfc_cpu_freq_limit(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cpuid, __actv_local_var_cpuid), __ARCH_ACTV_RPC_PASS_ARG(target_freq, __actv_local_var_target_freq), __ARCH_ACTV_RPC_PASS_ARG(limit_type, __actv_local_var_limit_type), __ARCH_ACTV_RPC_PASS_ARG(reason, __actv_local_var_reason));
	return __ret;
}

int __internal_actvcall __actvcapcall_fmcall_dfc_cpu_freq_limit(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cpuid, unsigned int target_freq, unsigned int limit_type, enum dfc_constraint_reason reason)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cpuid = cpuid;
	unsigned int __maybe_unused __actv_local_var_target_freq = target_freq;
	unsigned int __maybe_unused __actv_local_var_limit_type = limit_type;
	enum dfc_constraint_reason __maybe_unused __actv_local_var_reason = reason;
	__ret = ____actvcapcall_fmcall_dfc_cpu_freq_limit(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cpuid, __actv_local_var_cpuid), __ARCH_ACTV_RPC_PASS_ARG(target_freq, __actv_local_var_target_freq), __ARCH_ACTV_RPC_PASS_ARG(limit_type, __actv_local_var_limit_type), __ARCH_ACTV_RPC_PASS_ARG(reason, __actv_local_var_reason));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_fmcall_dfc_cpu_freq_query(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_fmcall_dfc_cpu_freq_query(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_fmcall_dfc_cpu_freq_query(unsigned long long __rpc_info, void *__buf, unsigned int cpuid, unsigned int query_type)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cpuid = cpuid;
	unsigned int __maybe_unused __actv_local_var_query_type = query_type;
	__ret = ____actvcall_fmcall_dfc_cpu_freq_query(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cpuid, __actv_local_var_cpuid), __ARCH_ACTV_RPC_PASS_ARG(query_type, __actv_local_var_query_type));
	return __ret;
}

int __internal_actvcall __actvcapcall_fmcall_dfc_cpu_freq_query(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cpuid, unsigned int query_type)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cpuid = cpuid;
	unsigned int __maybe_unused __actv_local_var_query_type = query_type;
	__ret = ____actvcapcall_fmcall_dfc_cpu_freq_query(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cpuid, __actv_local_var_cpuid), __ARCH_ACTV_RPC_PASS_ARG(query_type, __actv_local_var_query_type));
	return __ret;
}

#endif
#endif
