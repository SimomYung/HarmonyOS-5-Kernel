/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for tsfw
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../kernel/hongmeng/hm-security/tests/test_sec/test_framework/sysif/api.sysif -I ../kernel/hongmeng/hm-security/tests/test_sec/test_framework/sysif --api-dest ../image/usr/include/generated/sysif_client/test_framework/libsysif/test_framework/api.h --server-dest ../image/usr/include/generated/sysif_server/test_framework/libsysif/test_framework/server.h --legacy-mode
* Create: Fri Oct 31 10:38:30 2025
*/
#ifndef LIBSYSIF_TSFW_SERVER_H
#define LIBSYSIF_TSFW_SERVER_H
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
#include <libsysif/utils.h>
#include <hmasm/actv_rpc.h>
#include <hmasm/types.h>
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
extern int actvhdlr_tsfwhandler_tsfw_test_fw_srv0(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);


extern int actvhdlr_tsfwhandler_tsfw_test_fw_srv1(unsigned long long sender, unsigned long credential, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx);


extern int actvhdlr_tsfwhandler_tsfw_test_cocred_srv0(unsigned long long sender, unsigned long credential);


extern int actvhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact(unsigned long long sender, unsigned long credential);


extern int actvhdlr_tsfwhandler_tsfw_test_cocred_srv1(unsigned long long sender, unsigned long credential);


#else /* aarch64 */
extern int actvhdlr_tsfwhandler_tsfw_test_fw_srv0(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);


extern int actvhdlr_tsfwhandler_tsfw_test_fw_srv1(unsigned long long sender, unsigned long credential, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx);


extern int actvhdlr_tsfwhandler_tsfw_test_cocred_srv0(unsigned long long sender, unsigned long credential);


extern int actvhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact(unsigned long long sender, unsigned long credential);


extern int actvhdlr_tsfwhandler_tsfw_test_cocred_srv1(unsigned long long sender, unsigned long credential);


extern int __actvshadowhdlr_tsfwhandler_tsfw_test_fw_srv0(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_tsfwhandler_tsfw_test_fw_srv1(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv0(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv1(unsigned long long sender, unsigned long credential, ...);


#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
static inline struct __actvret_tsfwcall_tsfw_test_fw_srv0* actvhdlr_tsfwhandler_tsfw_test_fw_srv0_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_tsfwcall_tsfw_test_fw_srv0));
	return (struct __actvret_tsfwcall_tsfw_test_fw_srv0*)__ret;
}

static inline struct __actvret_tsfwcall_tsfw_test_fw_srv1* actvhdlr_tsfwhandler_tsfw_test_fw_srv1_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_tsfwcall_tsfw_test_fw_srv1));
	return (struct __actvret_tsfwcall_tsfw_test_fw_srv1*)__ret;
}

static inline struct __actvret_tsfwcall_tsfw_test_cocred_srv0* actvhdlr_tsfwhandler_tsfw_test_cocred_srv0_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_tsfwcall_tsfw_test_cocred_srv0));
	return (struct __actvret_tsfwcall_tsfw_test_cocred_srv0*)__ret;
}

static inline struct __actvret_tsfwcall_tsfw_test_cocred_srv0_xact* actvhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_tsfwcall_tsfw_test_cocred_srv0_xact));
	return (struct __actvret_tsfwcall_tsfw_test_cocred_srv0_xact*)__ret;
}

static inline struct __actvret_tsfwcall_tsfw_test_cocred_srv1* actvhdlr_tsfwhandler_tsfw_test_cocred_srv1_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_tsfwcall_tsfw_test_cocred_srv1));
	return (struct __actvret_tsfwcall_tsfw_test_cocred_srv1*)__ret;
}

#ifdef __sysif_server_tsfw_no_compat32_handlers__
#ifdef __HOST_LLT__
union tsfw_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1)(unsigned long long sender, unsigned long credential, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union tsfw_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1)(unsigned long long sender, unsigned long credential, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#ifdef __arm__
union tsfw_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1)(unsigned long long sender, unsigned long credential, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential);
};

#endif /* arm */
#endif /* not HOST_LLT */
#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif /* not __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#ifdef __HOST_LLT__
union tsfw_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1)(unsigned long long sender, unsigned long credential, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_fw_srv0)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_fw_srv0)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_fw_srv1)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_fw_srv1)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv0)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv0)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv0_xact)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv0_xact)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv1)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv1)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union tsfw_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1)(unsigned long long sender, unsigned long credential, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_fw_srv0)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_fw_srv0)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_fw_srv1)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_fw_srv1)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv0)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv0)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv0_xact)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv0_xact)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv1)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv1)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#endif /* not HOST_LLT */
#endif /* compat32 */

#if defined(__sysif_server_tsfw_no_compat32_handlers__) && defined(__sysif_server_tsfw_no_ilp32_handlers__)
#define __tsfw_method_TIMES 1
#elif !defined(__sysif_server_tsfw_no_compat32_handlers__) && !defined(__sysif_server_tsfw_no_ilp32_handlers__)
#define __tsfw_method_TIMES 3
#else
#define __tsfw_method_TIMES 2
#endif
extern union tsfw_hdlrs_union sysif_tsfw_actvhdlr_table[__tsfw_method_MAX * __tsfw_method_TIMES];

extern union tsfw_hdlrs_union sysif_tsfw_actvcapcallhdlr_table[__tsfw_method_MAX * __tsfw_method_TIMES];

extern void sysif_show_tsfw_methods(void);


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
static int __opt_nonoframe __actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0(unsigned long long sender, unsigned long credential, unsigned int cnode_idx)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_tsfwhandler_tsfw_test_fw_srv0(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_tsfwhandler_tsfw_test_fw_srv0(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

static int __opt_nonoframe __actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1(unsigned long long sender, unsigned long credential, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx)
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
	return __actvshadowhdlr_tsfwhandler_tsfw_test_fw_srv1(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, src_cnode_idx), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, fwd_cnode_idx));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx)
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
	return __actvshadowhdlr_tsfwhandler_tsfw_test_fw_srv1(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, src_cnode_idx), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, fwd_cnode_idx));
}

static int __opt_nonoframe __actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv0(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv0(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv1(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv1(sender, credential);
}

#ifndef __sysif_server_tsfw_no_compat32_handlers__
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
static int __opt_nonoframe __actvbridgehdlr_compat_tsfwhandler_tsfw_test_fw_srv0(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _cnode_idx;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_cnode_idx, data, 0, arg_sz, model, type, unsigned int, _cnode_idx)
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
	return __actvshadowhdlr_tsfwhandler_tsfw_test_fw_srv0(sender, credential, _cnode_idx);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_fw_srv0(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _cnode_idx;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_cnode_idx, data, 0, arg_sz, model, type, unsigned int, _cnode_idx)
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
	return __actvshadowhdlr_tsfwhandler_tsfw_test_fw_srv0(sender, credential, _cnode_idx);
}

static int __opt_nonoframe __actvbridgehdlr_compat_tsfwhandler_tsfw_test_fw_srv1(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _src_cnode_idx;
	unsigned int __maybe_unused _fwd_cnode_idx;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_src_cnode_idx, data, 0, arg_sz, model, type, unsigned int, _src_cnode_idx)
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
		__fetch_one_arg((void*)&_fwd_cnode_idx, data, 1, arg_sz, model, type, unsigned int, _fwd_cnode_idx)
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
	return __actvshadowhdlr_tsfwhandler_tsfw_test_fw_srv1(sender, credential, _src_cnode_idx, _fwd_cnode_idx);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_fw_srv1(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _src_cnode_idx;
	unsigned int __maybe_unused _fwd_cnode_idx;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_src_cnode_idx, data, 0, arg_sz, model, type, unsigned int, _src_cnode_idx)
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
		__fetch_one_arg((void*)&_fwd_cnode_idx, data, 1, arg_sz, model, type, unsigned int, _fwd_cnode_idx)
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
	return __actvshadowhdlr_tsfwhandler_tsfw_test_fw_srv1(sender, credential, _src_cnode_idx, _fwd_cnode_idx);
}

static int __opt_nonoframe __actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv0(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv0(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv0(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv0(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv0_xact(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv0_xact(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv1(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv1(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv1(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv1(sender, credential);
}

#endif /* compat handlers */
#ifndef __sysif_server_tsfw_no_ilp32_handlers__
static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_tsfwhandler_tsfw_test_fw_srv0(unsigned long long sender, unsigned long credential, unsigned int cnode_idx)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, cnode_idx)
	} while (0);
	return __actvshadowhdlr_tsfwhandler_tsfw_test_fw_srv0(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_tsfwhandler_tsfw_test_fw_srv0(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, cnode_idx)
	} while (0);
	return __actvshadowhdlr_tsfwhandler_tsfw_test_fw_srv0(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_tsfwhandler_tsfw_test_fw_srv1(unsigned long long sender, unsigned long credential, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx)
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
		__adjust_one_arg(unsigned int, src_cnode_idx)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, fwd_cnode_idx)
	} while (0);
	return __actvshadowhdlr_tsfwhandler_tsfw_test_fw_srv1(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, src_cnode_idx), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, fwd_cnode_idx));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_tsfwhandler_tsfw_test_fw_srv1(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx)
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
		__adjust_one_arg(unsigned int, src_cnode_idx)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, fwd_cnode_idx)
	} while (0);
	return __actvshadowhdlr_tsfwhandler_tsfw_test_fw_srv1(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, src_cnode_idx), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, fwd_cnode_idx));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv0(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv0(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv0(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv0(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv0_xact(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv0_xact(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv1(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv1(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv1(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_tsfwhandler_tsfw_test_cocred_srv1(sender, credential);
}

#endif
#else
static int __opt_nonoframe __actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0(unsigned long long sender, unsigned long credential, unsigned int cnode_idx)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_tsfwhandler_tsfw_test_fw_srv0(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_tsfwhandler_tsfw_test_fw_srv0(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

static int __opt_nonoframe __actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1(unsigned long long sender, unsigned long credential, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx)
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
	return actvhdlr_tsfwhandler_tsfw_test_fw_srv1(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, src_cnode_idx), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, fwd_cnode_idx));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int src_cnode_idx, unsigned int fwd_cnode_idx)
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
	return actvhdlr_tsfwhandler_tsfw_test_fw_srv1(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, src_cnode_idx), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, fwd_cnode_idx));
}

static int __opt_nonoframe __actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_tsfwhandler_tsfw_test_cocred_srv0(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_tsfwhandler_tsfw_test_cocred_srv0(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_tsfwhandler_tsfw_test_cocred_srv1(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_tsfwhandler_tsfw_test_cocred_srv1(sender, credential);
}

#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#if defined(__sysif_server_tsfw_no_compat32_handlers__) && defined(__sysif_server_tsfw_no_ilp32_handlers__)
__ro_after_init_hdlr union tsfw_hdlrs_union sysif_tsfw_actvhdlr_table[__tsfw_method_MAX * __tsfw_method_TIMES] = {
	[0 ... __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0)
	},
	[ __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[ __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1)
	}
};

__ro_after_init_hdlr union tsfw_hdlrs_union sysif_tsfw_actvcapcallhdlr_table[__tsfw_method_MAX * __tsfw_method_TIMES] = {
	[0 ... __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0)
	},
	[ __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[ __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1)
	}
};

#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif
#if !defined(__sysif_server_tsfw_no_compat32_handlers__) && defined(__sysif_server_tsfw_no_ilp32_handlers__)
__ro_after_init_hdlr union tsfw_hdlrs_union sysif_tsfw_actvhdlr_table[__tsfw_method_MAX * __tsfw_method_TIMES] = {
	[0 ... __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0)
	},
	[ __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[ __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1)
	},
	[__tsfw_method_MAX ... __tsfw_method_MAX + __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgehdlr_compat_tsfwhandler_tsfw_test_fw_srv0)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgehdlr_compat_tsfwhandler_tsfw_test_fw_srv1)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv1)
	}
};

__ro_after_init_hdlr union tsfw_hdlrs_union sysif_tsfw_actvcapcallhdlr_table[__tsfw_method_MAX * __tsfw_method_TIMES] = {
	[0 ... __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0)
	},
	[ __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[ __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1)
	},
	[__tsfw_method_MAX ... __tsfw_method_MAX + __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_fw_srv0)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_fw_srv1)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv1)
	}
};

#endif /* aarch64 + compat */
#if defined(__sysif_server_tsfw_no_compat32_handlers__) && !defined(__sysif_server_tsfw_no_ilp32_handlers__)
__ro_after_init_hdlr union tsfw_hdlrs_union sysif_tsfw_actvhdlr_table[__tsfw_method_MAX * __tsfw_method_TIMES] = {
	[0 ... __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0)
	},
	[ __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[ __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1)
	},
	[__tsfw_method_MAX ... __tsfw_method_MAX + __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgehdlr_ilp32_tsfwhandler_tsfw_test_fw_srv0)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgehdlr_ilp32_tsfwhandler_tsfw_test_fw_srv1)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgehdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgehdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgehdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv1)
	}
};

__ro_after_init_hdlr union tsfw_hdlrs_union sysif_tsfw_actvcapcallhdlr_table[__tsfw_method_MAX * __tsfw_method_TIMES] = {
	[0 ... __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0)
	},
	[ __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[ __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1)
	},
	[__tsfw_method_MAX ... __tsfw_method_MAX + __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgecapcallhdlr_ilp32_tsfwhandler_tsfw_test_fw_srv0)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgecapcallhdlr_ilp32_tsfwhandler_tsfw_test_fw_srv1)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgecapcallhdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgecapcallhdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgecapcallhdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv1)
	}
};

#endif /* aarch64 + ilp32 */
#if !defined(__sysif_server_tsfw_no_compat32_handlers__) && !defined(__sysif_server_tsfw_no_ilp32_handlers__)
__ro_after_init_hdlr union tsfw_hdlrs_union sysif_tsfw_actvhdlr_table[__tsfw_method_MAX * __tsfw_method_TIMES] = {
	[0 ... __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0)
	},
	[ __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[ __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1)
	},
	[__tsfw_method_MAX ... __tsfw_method_MAX + __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgehdlr_compat_tsfwhandler_tsfw_test_fw_srv0)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgehdlr_compat_tsfwhandler_tsfw_test_fw_srv1)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgehdlr_compat_tsfwhandler_tsfw_test_cocred_srv1)
	},
	[2 * __tsfw_method_MAX ... 2 * __tsfw_method_MAX + __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgehdlr_ilp32_tsfwhandler_tsfw_test_fw_srv0)
	},
	[2 * __tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgehdlr_ilp32_tsfwhandler_tsfw_test_fw_srv1)
	},
	[2 * __tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgehdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[2 * __tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgehdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[2 * __tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgehdlr_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgehdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv1)
	}
};

__ro_after_init_hdlr union tsfw_hdlrs_union sysif_tsfw_actvcapcallhdlr_table[__tsfw_method_MAX * __tsfw_method_TIMES] = {
	[0 ... __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0)
	},
	[ __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[ __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[ __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1)
	},
	[__tsfw_method_MAX ... __tsfw_method_MAX + __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_fw_srv0)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_fw_srv1)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[__tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgecapcallhdlr_compat_tsfwhandler_tsfw_test_cocred_srv1)
	},
	[2 * __tsfw_method_MAX ... 2 * __tsfw_method_MAX + __tsfw_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv0] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv0 = (&__actvbridgecapcallhdlr_ilp32_tsfwhandler_tsfw_test_fw_srv0)
	},
	[2 * __tsfw_method_MAX + __tsfw_method_tsfw_test_fw_srv1] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_fw_srv1 = (&__actvbridgecapcallhdlr_ilp32_tsfwhandler_tsfw_test_fw_srv1)
	},
	[2 * __tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0 = (&__actvbridgecapcallhdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv0)
	},
	[2 * __tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv0_xact] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv0_xact = (&__actvbridgecapcallhdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv0_xact)
	},
	[2 * __tsfw_method_MAX + __tsfw_method_tsfw_test_cocred_srv1] = {
		.p__actvbridgecapcallhdlr_tsfwhandler_tsfw_test_cocred_srv1 = (&__actvbridgecapcallhdlr_ilp32_tsfwhandler_tsfw_test_cocred_srv1)
	}
};

#endif /* aarch64 + compat + ilp32 */
#endif /* arm or aarch64 only */
#ifdef __aarch64__
void sysif_show_tsfw_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "tsfwcall", "tsfw", "test_fw_srv0", __tsfw_method_tsfw_test_fw_srv0);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "tsfwhandler", "tsfw", "test_fw_srv0", __tsfw_method_tsfw_test_fw_srv0);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "tsfwcall", "tsfw", "test_fw_srv1", __tsfw_method_tsfw_test_fw_srv1);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "tsfwhandler", "tsfw", "test_fw_srv1", __tsfw_method_tsfw_test_fw_srv1);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "tsfwcall", "tsfw", "test_cocred_srv0", __tsfw_method_tsfw_test_cocred_srv0);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "tsfwhandler", "tsfw", "test_cocred_srv0", __tsfw_method_tsfw_test_cocred_srv0);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "tsfwcall", "tsfw", "test_cocred_srv0_xact", __tsfw_method_tsfw_test_cocred_srv0_xact);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "tsfwhandler", "tsfw", "test_cocred_srv0_xact", __tsfw_method_tsfw_test_cocred_srv0_xact);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "tsfwcall", "tsfw", "test_cocred_srv1", __tsfw_method_tsfw_test_cocred_srv1);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "tsfwhandler", "tsfw", "test_cocred_srv1", __tsfw_method_tsfw_test_cocred_srv1);
	hm_info("%s_%s, max method num: %d\n", "tsfw", "method", __tsfw_method_MAX);
}

#elif defined(__arm__)
void sysif_show_tsfw_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "tsfwcall", "tsfw", "test_fw_srv0", __tsfw_method_tsfw_test_fw_srv0);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "tsfwhandler", "tsfw", "test_fw_srv0", __tsfw_method_tsfw_test_fw_srv0);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "tsfwcall", "tsfw", "test_fw_srv1", __tsfw_method_tsfw_test_fw_srv1);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "tsfwhandler", "tsfw", "test_fw_srv1", __tsfw_method_tsfw_test_fw_srv1);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "tsfwcall", "tsfw", "test_cocred_srv0", __tsfw_method_tsfw_test_cocred_srv0);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "tsfwhandler", "tsfw", "test_cocred_srv0", __tsfw_method_tsfw_test_cocred_srv0);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "tsfwcall", "tsfw", "test_cocred_srv0_xact", __tsfw_method_tsfw_test_cocred_srv0_xact);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "tsfwhandler", "tsfw", "test_cocred_srv0_xact", __tsfw_method_tsfw_test_cocred_srv0_xact);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "tsfwcall", "tsfw", "test_cocred_srv1", __tsfw_method_tsfw_test_cocred_srv1);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "tsfwhandler", "tsfw", "test_cocred_srv1", __tsfw_method_tsfw_test_cocred_srv1);
	hm_info("%s_%s, max method num: %d\n", "tsfw", "method", __tsfw_method_MAX);
}

#else
void sysif_show_tsfw_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "tsfwcall", "tsfw", "test_fw_srv0", __tsfw_method_tsfw_test_fw_srv0);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "tsfwhandler", "tsfw", "test_fw_srv0", __tsfw_method_tsfw_test_fw_srv0);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "tsfwcall", "tsfw", "test_fw_srv1", __tsfw_method_tsfw_test_fw_srv1);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "tsfwhandler", "tsfw", "test_fw_srv1", __tsfw_method_tsfw_test_fw_srv1);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "tsfwcall", "tsfw", "test_cocred_srv0", __tsfw_method_tsfw_test_cocred_srv0);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "tsfwhandler", "tsfw", "test_cocred_srv0", __tsfw_method_tsfw_test_cocred_srv0);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "tsfwcall", "tsfw", "test_cocred_srv0_xact", __tsfw_method_tsfw_test_cocred_srv0_xact);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "tsfwhandler", "tsfw", "test_cocred_srv0_xact", __tsfw_method_tsfw_test_cocred_srv0_xact);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "tsfwcall", "tsfw", "test_cocred_srv1", __tsfw_method_tsfw_test_cocred_srv1);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "tsfwhandler", "tsfw", "test_cocred_srv1", __tsfw_method_tsfw_test_cocred_srv1);
	hm_info("%s_%s, max method num: %d\n", "tsfw", "method", __tsfw_method_MAX);
}

#endif
#endif
#endif
