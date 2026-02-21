/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for tsfw
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../tests/test_sec/test_framework/sysif/api.sysif -I ../tests/test_sec/test_framework/sysif --api-dest ../image/usr/include/generated/sysif_client/test_framework/libsysif/test_framework/api.h --server-dest ../image/usr/include/generated/sysif_server/test_framework/libsysif/test_framework/server.h --legacy-mode
* Create: Tue Sep 16 14:00:36 2025
*/
#ifndef LIBSYSIF_TSFW_API_H
#define LIBSYSIF_TSFW_API_H
#ifndef LIBSYSIF_TSFW_COMMON_H
#define LIBSYSIF_TSFW_COMMON_H
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
struct __actvret_tsfwcall_tsfw_test_fw_srv0 {
};

struct __actvret_tsfwcall_tsfw_test_fw_srv1 {
};

struct __actvret_tsfwcall_tsfw_test_cocred_srv0 {
};

struct __actvret_tsfwcall_tsfw_test_cocred_srv0_xact {
};

struct __actvret_tsfwcall_tsfw_test_cocred_srv1 {
};

#ifdef __aarch64__
union __actvret_tsfwcall {
	struct __actvret_tsfwcall_tsfw_test_fw_srv0 tsfw_test_fw_srv0;
	struct __actvret_tsfwcall_tsfw_test_fw_srv1 tsfw_test_fw_srv1;
	struct __actvret_tsfwcall_tsfw_test_cocred_srv0 tsfw_test_cocred_srv0;
	struct __actvret_tsfwcall_tsfw_test_cocred_srv0_xact tsfw_test_cocred_srv0_xact;
	struct __actvret_tsfwcall_tsfw_test_cocred_srv1 tsfw_test_cocred_srv1;
};

#elif defined(__arm__)
union __actvret_tsfwcall {
	struct __actvret_tsfwcall_tsfw_test_fw_srv0 tsfw_test_fw_srv0;
	struct __actvret_tsfwcall_tsfw_test_fw_srv1 tsfw_test_fw_srv1;
	struct __actvret_tsfwcall_tsfw_test_cocred_srv0 tsfw_test_cocred_srv0;
	struct __actvret_tsfwcall_tsfw_test_cocred_srv0_xact tsfw_test_cocred_srv0_xact;
	struct __actvret_tsfwcall_tsfw_test_cocred_srv1 tsfw_test_cocred_srv1;
};

#else
union __actvret_tsfwcall {
	struct __actvret_tsfwcall_tsfw_test_fw_srv0 tsfw_test_fw_srv0;
	struct __actvret_tsfwcall_tsfw_test_fw_srv1 tsfw_test_fw_srv1;
	struct __actvret_tsfwcall_tsfw_test_cocred_srv0 tsfw_test_cocred_srv0;
	struct __actvret_tsfwcall_tsfw_test_cocred_srv0_xact tsfw_test_cocred_srv0_xact;
	struct __actvret_tsfwcall_tsfw_test_cocred_srv1 tsfw_test_cocred_srv1;
};

#endif
enum __tsfw_mgr {
	__tsfw_INVAL_mgr=0,
	__tsfw_tsfw_mgr,
	__tsfw_MAX_mgr,
};

#define __tsfw_method_MIN __ACTV_HIGHTABLE_START
#define __tsfw_method_tsfw_test_fw_srv0 (__ACTV_HIGHTABLE_START + 1)
#define __tsfw_method_tsfw_test_fw_srv1 (__ACTV_HIGHTABLE_START + 2)
#define __tsfw_method_tsfw_test_cocred_srv0 (__ACTV_HIGHTABLE_START + 3)
#define __tsfw_method_tsfw_test_cocred_srv0_xact (__ACTV_HIGHTABLE_START + 4)
#define __tsfw_method_tsfw_test_cocred_srv1 (__ACTV_HIGHTABLE_START + 5)
#define __tsfw_method_MAX (__ACTV_HIGHTABLE_START + 6)
raw_static_assert(sizeof(struct __actvret_tsfwcall_tsfw_test_fw_srv0) < 512,
            tsfwcall_tsfw_test_fw_srv0_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_tsfwcall_tsfw_test_fw_srv1) < 512,
            tsfwcall_tsfw_test_fw_srv1_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_tsfwcall_tsfw_test_cocred_srv0) < 512,
            tsfwcall_tsfw_test_cocred_srv0_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_tsfwcall_tsfw_test_cocred_srv0_xact) < 512,
            tsfwcall_tsfw_test_cocred_srv0_xact_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_tsfwcall_tsfw_test_cocred_srv1) < 512,
            tsfwcall_tsfw_test_cocred_srv1_too_many_actvret_data);
raw_static_assert(sizeof(union __actvret_tsfwcall) < 512,
        tsfwcall_too_many_actvret_data);
extern int __actvcall_tsfwcall_tsfw_test_fw_srv0(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx);


extern int __actvcapcall_tsfwcall_tsfw_test_fw_srv0(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx);


static inline int __attribute__((always_inline)) actvxactcall_tsfwcall_tsfw_test_fw_srv0(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cnode_idx)
{
	const unsigned long __callno = __tsfw_method_tsfw_test_fw_srv0;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_tsfwcall_tsfw_test_fw_srv0(rpc_info, NULL, cnode_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_tsfwcall_tsfw_test_fw_srv0(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cnode_idx)
{
	const unsigned long __callno = __tsfw_method_tsfw_test_fw_srv0;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_tsfwcall_tsfw_test_fw_srv0(__rref, rpc_info, NULL, cnode_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_tsfwcall_tsfw_test_fw_srv0(unsigned int cnode_idx)
{
	return actvxactcall_tsfwcall_tsfw_test_fw_srv0(false, false, cnode_idx);
}

static inline int __attribute__((always_inline)) actvcapcall_tsfwcall_tsfw_test_fw_srv0(rref_t __rref, unsigned int cnode_idx)
{
	return actvxactcapcall_tsfwcall_tsfw_test_fw_srv0(false, false, __rref, cnode_idx);
}

extern int __actvcall_tsfwcall_tsfw_test_fw_srv1(unsigned long long __rpc_info, void *__buf, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx);


extern int __actvcapcall_tsfwcall_tsfw_test_fw_srv1(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx);


static inline int __attribute__((always_inline)) actvxactcall_tsfwcall_tsfw_test_fw_srv1(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx)
{
	const unsigned long __callno = __tsfw_method_tsfw_test_fw_srv1;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_tsfwcall_tsfw_test_fw_srv1(rpc_info, NULL, src_cnode_idx, fwd_cnode_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_tsfwcall_tsfw_test_fw_srv1(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx)
{
	const unsigned long __callno = __tsfw_method_tsfw_test_fw_srv1;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_tsfwcall_tsfw_test_fw_srv1(__rref, rpc_info, NULL, src_cnode_idx, fwd_cnode_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_tsfwcall_tsfw_test_fw_srv1(unsigned int src_cnode_idx, unsigned int fwd_cnode_idx)
{
	return actvxactcall_tsfwcall_tsfw_test_fw_srv1(false, false, src_cnode_idx, fwd_cnode_idx);
}

static inline int __attribute__((always_inline)) actvcapcall_tsfwcall_tsfw_test_fw_srv1(rref_t __rref, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx)
{
	return actvxactcapcall_tsfwcall_tsfw_test_fw_srv1(false, false, __rref, src_cnode_idx, fwd_cnode_idx);
}

extern int __actvcall_tsfwcall_tsfw_test_cocred_srv0(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_tsfwcall_tsfw_test_cocred_srv0(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_tsfwcall_tsfw_test_cocred_srv0(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __tsfw_method_tsfw_test_cocred_srv0;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_tsfwcall_tsfw_test_cocred_srv0(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_tsfwcall_tsfw_test_cocred_srv0(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __tsfw_method_tsfw_test_cocred_srv0;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_tsfwcall_tsfw_test_cocred_srv0(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_tsfwcall_tsfw_test_cocred_srv0(void)
{
	return actvxactcall_tsfwcall_tsfw_test_cocred_srv0(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_tsfwcall_tsfw_test_cocred_srv0(rref_t __rref)
{
	return actvxactcapcall_tsfwcall_tsfw_test_cocred_srv0(false, false, __rref);
}

extern int __actvcall_tsfwcall_tsfw_test_cocred_srv0_xact(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_tsfwcall_tsfw_test_cocred_srv0_xact(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_tsfwcall_tsfw_test_cocred_srv0_xact(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __tsfw_method_tsfw_test_cocred_srv0_xact;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_tsfwcall_tsfw_test_cocred_srv0_xact(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_tsfwcall_tsfw_test_cocred_srv0_xact(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __tsfw_method_tsfw_test_cocred_srv0_xact;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_tsfwcall_tsfw_test_cocred_srv0_xact(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_tsfwcall_tsfw_test_cocred_srv0_xact(void)
{
	return actvxactcall_tsfwcall_tsfw_test_cocred_srv0_xact(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_tsfwcall_tsfw_test_cocred_srv0_xact(rref_t __rref)
{
	return actvxactcapcall_tsfwcall_tsfw_test_cocred_srv0_xact(false, false, __rref);
}

extern int __actvcall_tsfwcall_tsfw_test_cocred_srv1(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_tsfwcall_tsfw_test_cocred_srv1(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_tsfwcall_tsfw_test_cocred_srv1(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __tsfw_method_tsfw_test_cocred_srv1;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_tsfwcall_tsfw_test_cocred_srv1(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_tsfwcall_tsfw_test_cocred_srv1(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __tsfw_method_tsfw_test_cocred_srv1;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_tsfwcall_tsfw_test_cocred_srv1(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_tsfwcall_tsfw_test_cocred_srv1(void)
{
	return actvxactcall_tsfwcall_tsfw_test_cocred_srv1(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_tsfwcall_tsfw_test_cocred_srv1(rref_t __rref)
{
	return actvxactcapcall_tsfwcall_tsfw_test_cocred_srv1(false, false, __rref);
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

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_tsfwcall_tsfw_test_fw_srv0(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_tsfwcall_tsfw_test_fw_srv0(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_tsfwcall_tsfw_test_fw_srv0(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	__ret = ____actvcall_tsfwcall_tsfw_test_fw_srv0(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx));
	return __ret;
}

int __internal_actvcall __actvcapcall_tsfwcall_tsfw_test_fw_srv0(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	__ret = ____actvcapcall_tsfwcall_tsfw_test_fw_srv0(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_tsfwcall_tsfw_test_fw_srv1(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_tsfwcall_tsfw_test_fw_srv1(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_tsfwcall_tsfw_test_fw_srv1(unsigned long long __rpc_info, void *__buf, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_src_cnode_idx = src_cnode_idx;
	unsigned int __maybe_unused __actv_local_var_fwd_cnode_idx = fwd_cnode_idx;
	__ret = ____actvcall_tsfwcall_tsfw_test_fw_srv1(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(src_cnode_idx, __actv_local_var_src_cnode_idx), __ARCH_ACTV_RPC_PASS_ARG(fwd_cnode_idx, __actv_local_var_fwd_cnode_idx));
	return __ret;
}

int __internal_actvcall __actvcapcall_tsfwcall_tsfw_test_fw_srv1(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_src_cnode_idx = src_cnode_idx;
	unsigned int __maybe_unused __actv_local_var_fwd_cnode_idx = fwd_cnode_idx;
	__ret = ____actvcapcall_tsfwcall_tsfw_test_fw_srv1(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(src_cnode_idx, __actv_local_var_src_cnode_idx), __ARCH_ACTV_RPC_PASS_ARG(fwd_cnode_idx, __actv_local_var_fwd_cnode_idx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_tsfwcall_tsfw_test_cocred_srv0(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_tsfwcall_tsfw_test_cocred_srv0(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_tsfwcall_tsfw_test_cocred_srv0(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_tsfwcall_tsfw_test_cocred_srv0(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_tsfwcall_tsfw_test_cocred_srv0(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_tsfwcall_tsfw_test_cocred_srv0(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_tsfwcall_tsfw_test_cocred_srv0_xact(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_tsfwcall_tsfw_test_cocred_srv0_xact(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_tsfwcall_tsfw_test_cocred_srv0_xact(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_tsfwcall_tsfw_test_cocred_srv0_xact(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_tsfwcall_tsfw_test_cocred_srv0_xact(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_tsfwcall_tsfw_test_cocred_srv0_xact(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_tsfwcall_tsfw_test_cocred_srv1(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_tsfwcall_tsfw_test_cocred_srv1(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_tsfwcall_tsfw_test_cocred_srv1(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_tsfwcall_tsfw_test_cocred_srv1(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_tsfwcall_tsfw_test_cocred_srv1(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_tsfwcall_tsfw_test_cocred_srv1(__rref, __rpc_info, __buf);
	return __ret;
}

#endif
#endif
