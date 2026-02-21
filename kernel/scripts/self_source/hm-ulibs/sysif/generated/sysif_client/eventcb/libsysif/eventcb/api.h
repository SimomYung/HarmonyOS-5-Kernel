/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for eventcb
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../kernel/hongmeng/hm-verif-kernel/ulibs/include/libsysif/eventcb/api.sysif -I ../kernel/hongmeng/hm-verif-kernel/ulibs/include --api-dest ../image/usr/include/generated/sysif_client/eventcb/libsysif/eventcb/api.h --server-dest ../image/usr/include/generated/sysif_server/eventcb/libsysif/eventcb/server.h --dump-callno ../image/usr/include/generated/dump_info/eventcb/libsysif/eventcb/l2h_map.h --legacy-mode --no-selfcall
* Create: Fri Oct 31 10:38:30 2025
*/
#ifndef LIBSYSIF_EVENTCB_API_H
#define LIBSYSIF_EVENTCB_API_H
#ifndef LIBSYSIF_EVENTCB_COMMON_H
#define LIBSYSIF_EVENTCB_COMMON_H
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
#include <libhwsecurec/securec.h>

#include <hmsysmgr/eventmgr/event_def.h>

raw_static_assert(sizeof(struct evtcb_thread_msg_s) <= 96,
                    eventcbcall_eventcb_thread_msg_ctrl_arg_msg_is_too_large);
raw_static_assert(sizeof(pid_t) <= 96,
                    eventcbcall_eventcb_process_exit_arg_pid_is_too_large);
raw_static_assert(sizeof(uid_t) <= 96,
                    eventcbcall_eventcb_process_exit_arg_uid_is_too_large);
struct __actvret_eventcbcall_eventcb_thread_msg_ctrl {
};

struct __actvret_eventcbcall_eventcb_process_exit {
};

struct __actvret_eventcbcall_eventcb_path_clean_entry {
};

#ifdef __aarch64__
union __actvret_eventcbcall {
	struct __actvret_eventcbcall_eventcb_thread_msg_ctrl eventcb_thread_msg_ctrl;
	struct __actvret_eventcbcall_eventcb_process_exit eventcb_process_exit;
	struct __actvret_eventcbcall_eventcb_path_clean_entry eventcb_path_clean_entry;
};

#elif defined(__arm__)
union __actvret_eventcbcall {
	struct __actvret_eventcbcall_eventcb_thread_msg_ctrl eventcb_thread_msg_ctrl;
	struct __actvret_eventcbcall_eventcb_process_exit eventcb_process_exit;
	struct __actvret_eventcbcall_eventcb_path_clean_entry eventcb_path_clean_entry;
};

#else
union __actvret_eventcbcall {
	struct __actvret_eventcbcall_eventcb_thread_msg_ctrl eventcb_thread_msg_ctrl;
	struct __actvret_eventcbcall_eventcb_process_exit eventcb_process_exit;
	struct __actvret_eventcbcall_eventcb_path_clean_entry eventcb_path_clean_entry;
};

#endif
enum __eventcb_mgr {
	__eventcb_INVAL_mgr=0,
	__eventcb_eventcb_mgr,
	__eventcb_MAX_mgr,
};

#define __eventcb_method_MIN __ACTV_HIGHTABLE_START
#define __eventcb_method_eventcb_thread_msg_ctrl (__ACTV_HIGHTABLE_START + 1)
#define __eventcb_method_eventcb_process_exit (__ACTV_HIGHTABLE_START + 2)
#define __eventcb_method_eventcb_path_clean_entry (__ACTV_HIGHTABLE_START + 3)
#define __eventcb_method_eventcb_rsvd_5 (__ACTV_HIGHTABLE_START + 4)
#define __eventcb_method_eventcb_rsvd_4 (__ACTV_HIGHTABLE_START + 5)
#define __eventcb_method_eventcb_rsvd_3 (__ACTV_HIGHTABLE_START + 6)
#define __eventcb_method_eventcb_rsvd_2 (__ACTV_HIGHTABLE_START + 7)
#define __eventcb_method_eventcb_rsvd_1 (__ACTV_HIGHTABLE_START + 8)
#define __eventcb_method_MAX (__ACTV_HIGHTABLE_START + 9)
raw_static_assert(sizeof(struct __actvret_eventcbcall_eventcb_thread_msg_ctrl) < 512,
            eventcbcall_eventcb_thread_msg_ctrl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_eventcbcall_eventcb_process_exit) < 512,
            eventcbcall_eventcb_process_exit_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_eventcbcall_eventcb_path_clean_entry) < 512,
            eventcbcall_eventcb_path_clean_entry_too_many_actvret_data);
raw_static_assert(sizeof(union __actvret_eventcbcall) < 512,
        eventcbcall_too_many_actvret_data);
extern int __actvcall_eventcbcall_eventcb_thread_msg_ctrl(unsigned long long __rpc_info, void *__buf, struct evtcb_thread_msg_s msg);


extern int __actvcapcall_eventcbcall_eventcb_thread_msg_ctrl(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct evtcb_thread_msg_s msg);


static inline int __attribute__((always_inline)) actvxactcall_eventcbcall_eventcb_thread_msg_ctrl(bool is_xact_fwd, bool is_xact_allow_refwd, struct evtcb_thread_msg_s msg)
{
	const unsigned long __callno = __eventcb_method_eventcb_thread_msg_ctrl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct evtcb_thread_msg_s), __arch_actv_rpc_stksz_alignment(struct evtcb_thread_msg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_eventcbcall_eventcb_thread_msg_ctrl(rpc_info, NULL, msg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_eventcbcall_eventcb_thread_msg_ctrl(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct evtcb_thread_msg_s msg)
{
	const unsigned long __callno = __eventcb_method_eventcb_thread_msg_ctrl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct evtcb_thread_msg_s), __arch_actv_rpc_stksz_alignment(struct evtcb_thread_msg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_eventcbcall_eventcb_thread_msg_ctrl(__rref, rpc_info, NULL, msg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_eventcbcall_eventcb_thread_msg_ctrl(struct evtcb_thread_msg_s msg)
{
	return actvxactcall_eventcbcall_eventcb_thread_msg_ctrl(false, false, msg);
}

static inline int __attribute__((always_inline)) actvcapcall_eventcbcall_eventcb_thread_msg_ctrl(rref_t __rref, struct evtcb_thread_msg_s msg)
{
	return actvxactcapcall_eventcbcall_eventcb_thread_msg_ctrl(false, false, __rref, msg);
}

extern int __actvcall_eventcbcall_eventcb_process_exit(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, pid_t pid, uid_t uid);


extern int __actvcapcall_eventcbcall_eventcb_process_exit(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, pid_t pid, uid_t uid);


static inline int __attribute__((always_inline)) actvxactcall_eventcbcall_eventcb_process_exit(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	const unsigned long __callno = __eventcb_method_eventcb_process_exit;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uid_t), __arch_actv_rpc_stksz_alignment(uid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_eventcbcall_eventcb_process_exit(rpc_info, NULL, cnode_idx, pid, uid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_eventcbcall_eventcb_process_exit(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	const unsigned long __callno = __eventcb_method_eventcb_process_exit;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uid_t), __arch_actv_rpc_stksz_alignment(uid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_eventcbcall_eventcb_process_exit(__rref, rpc_info, NULL, cnode_idx, pid, uid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_eventcbcall_eventcb_process_exit(unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	return actvxactcall_eventcbcall_eventcb_process_exit(false, false, cnode_idx, pid, uid);
}

static inline int __attribute__((always_inline)) actvcapcall_eventcbcall_eventcb_process_exit(rref_t __rref, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	return actvxactcapcall_eventcbcall_eventcb_process_exit(false, false, __rref, cnode_idx, pid, uid);
}

extern int __actvcall_eventcbcall_eventcb_path_clean_entry(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx);


extern int __actvcapcall_eventcbcall_eventcb_path_clean_entry(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx);


static inline int __attribute__((always_inline)) actvxactcall_eventcbcall_eventcb_path_clean_entry(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cnode_idx, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __eventcb_method_eventcb_path_clean_entry;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_eventcbcall_eventcb_path_clean_entry(rpc_info, ___void_buf, cnode_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_eventcbcall_eventcb_path_clean_entry(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cnode_idx, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __eventcb_method_eventcb_path_clean_entry;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_eventcbcall_eventcb_path_clean_entry(__rref, rpc_info, ___void_buf, cnode_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_eventcbcall_eventcb_path_clean_entry(unsigned int cnode_idx, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_eventcbcall_eventcb_path_clean_entry(false, false, cnode_idx, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_eventcbcall_eventcb_path_clean_entry(rref_t __rref, unsigned int cnode_idx, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_eventcbcall_eventcb_path_clean_entry(false, false, __rref, cnode_idx, ___void_buf, ___void_buf_sz);
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

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_eventcbcall_eventcb_thread_msg_ctrl(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_eventcbcall_eventcb_thread_msg_ctrl(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_eventcbcall_eventcb_thread_msg_ctrl(unsigned long long __rpc_info, void *__buf, struct evtcb_thread_msg_s msg)
{
	int __ret;
	struct evtcb_thread_msg_s __maybe_unused __actv_local_var_msg = msg;
	__ret = ____actvcall_eventcbcall_eventcb_thread_msg_ctrl(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(msg, __actv_local_var_msg));
	return __ret;
}

int __internal_actvcall __actvcapcall_eventcbcall_eventcb_thread_msg_ctrl(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct evtcb_thread_msg_s msg)
{
	int __ret;
	struct evtcb_thread_msg_s __maybe_unused __actv_local_var_msg = msg;
	__ret = ____actvcapcall_eventcbcall_eventcb_thread_msg_ctrl(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(msg, __actv_local_var_msg));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_eventcbcall_eventcb_process_exit(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_eventcbcall_eventcb_process_exit(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_eventcbcall_eventcb_process_exit(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	pid_t __maybe_unused __actv_local_var_pid = pid;
	uid_t __maybe_unused __actv_local_var_uid = uid;
	__ret = ____actvcall_eventcbcall_eventcb_process_exit(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx), __ARCH_ACTV_RPC_PASS_ARG(pid, __actv_local_var_pid), __ARCH_ACTV_RPC_PASS_ARG(uid, __actv_local_var_uid));
	return __ret;
}

int __internal_actvcall __actvcapcall_eventcbcall_eventcb_process_exit(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	pid_t __maybe_unused __actv_local_var_pid = pid;
	uid_t __maybe_unused __actv_local_var_uid = uid;
	__ret = ____actvcapcall_eventcbcall_eventcb_process_exit(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx), __ARCH_ACTV_RPC_PASS_ARG(pid, __actv_local_var_pid), __ARCH_ACTV_RPC_PASS_ARG(uid, __actv_local_var_uid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_eventcbcall_eventcb_path_clean_entry(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_eventcbcall_eventcb_path_clean_entry(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_eventcbcall_eventcb_path_clean_entry(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	__ret = ____actvcall_eventcbcall_eventcb_path_clean_entry(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx));
	return __ret;
}

int __internal_actvcall __actvcapcall_eventcbcall_eventcb_path_clean_entry(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	__ret = ____actvcapcall_eventcbcall_eventcb_path_clean_entry(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx));
	return __ret;
}

#endif
#endif
