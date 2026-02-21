/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for auditmgr
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../ulibs/include/libsysif/auditmgr/api.sysif -I ../ulibs/include --api-dest ../image/usr/include/generated/sysif_client/auditmgr/libsysif/auditmgr/api.h --server-dest ../image/usr/include/generated/sysif_server/auditmgr/libsysif/auditmgr/server.h --dump-callno ../image/usr/include/generated/dump_info/auditmgr/libsysif/auditmgr/l2h_map.h --legacy-mode
* Create: Mon Sep 22 11:44:32 2025
*/
#ifndef LIBSYSIF_AUDITMGR_SERVER_H
#define LIBSYSIF_AUDITMGR_SERVER_H
#ifndef LIBSYSIF_AUDITMGR_COMMON_H
#define LIBSYSIF_AUDITMGR_COMMON_H
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
#include <unistd.h>

#define AUDITMGR_PATH	"auditmgr.actv"

raw_static_assert(sizeof(pid_t) <= 96,
                    adcall_msg_register_arg_pid_is_too_large);
struct __actvret_adcall_msg_recv {
};

struct __actvret_adcall_msg_register {
	unsigned int serial_header;
};

#ifdef __aarch64__
union __actvret_adcall {
	struct __actvret_adcall_msg_recv msg_recv;
	struct __actvret_adcall_msg_register msg_register;
};

#elif defined(__arm__)
union __actvret_adcall {
	struct __actvret_adcall_msg_recv msg_recv;
	struct __actvret_adcall_msg_register msg_register;
};

#else
union __actvret_adcall {
	struct __actvret_adcall_msg_recv msg_recv;
	struct __actvret_adcall_msg_register msg_register;
};

#endif
enum __auditmgr_mgr {
	__auditmgr_INVAL_mgr=0,
	__auditmgr_msg_mgr,
	__auditmgr_MAX_mgr,
};

#define __auditmgr_method_MIN __ACTV_HIGHTABLE_START
#define __auditmgr_method_msg_recv (__ACTV_HIGHTABLE_START + 1)
#define __auditmgr_method_msg_register (__ACTV_HIGHTABLE_START + 2)
#define __auditmgr_method_msg_rsvd_2 (__ACTV_HIGHTABLE_START + 3)
#define __auditmgr_method_msg_rsvd_1 (__ACTV_HIGHTABLE_START + 4)
#define __auditmgr_method_MAX (__ACTV_HIGHTABLE_START + 5)
raw_static_assert(sizeof(struct __actvret_adcall_msg_recv) < 512,
            adcall_msg_recv_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_adcall_msg_register) < 512,
            adcall_msg_register_too_many_actvret_data);
raw_static_assert(sizeof(union __actvret_adcall) < 512,
        adcall_too_many_actvret_data);
extern int __actvcall_adcall_msg_recv(unsigned long long __rpc_info, void *__buf, unsigned int port, const void *src_data, size_t len);


extern int __actvcapcall_adcall_msg_recv(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int port, const void *src_data, size_t len);


static inline int __attribute__((always_inline)) actvxactcall_adcall_msg_recv(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int port, const void *src_data, size_t len)
{
	const unsigned long __callno = __auditmgr_method_msg_recv;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_adcall_msg_recv(rpc_info, NULL, port, src_data, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_adcall_msg_recv(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int port, const void *src_data, size_t len)
{
	const unsigned long __callno = __auditmgr_method_msg_recv;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_adcall_msg_recv(__rref, rpc_info, NULL, port, src_data, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_adcall_msg_recv(unsigned int port, const void *src_data, size_t len)
{
	return actvxactcall_adcall_msg_recv(false, false, port, src_data, len);
}

static inline int __attribute__((always_inline)) actvcapcall_adcall_msg_recv(rref_t __rref, unsigned int port, const void *src_data, size_t len)
{
	return actvxactcapcall_adcall_msg_recv(false, false, __rref, port, src_data, len);
}

extern int __actvcall_adcall_msg_register(unsigned long long __rpc_info, void *__buf, pid_t pid);


extern int __actvcapcall_adcall_msg_register(rref_t __rref, unsigned long long __rpc_info, void *__buf, pid_t pid);


static inline int __attribute__((always_inline)) actvxactcall_adcall_msg_register(bool is_xact_fwd, bool is_xact_allow_refwd, pid_t pid, struct __actvret_adcall_msg_register *__ret)
{
	const unsigned long __callno = __auditmgr_method_msg_register;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_adcall_msg_register(rpc_info, __ret, pid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_adcall_msg_register(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, pid_t pid, struct __actvret_adcall_msg_register *__ret)
{
	const unsigned long __callno = __auditmgr_method_msg_register;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_adcall_msg_register(__rref, rpc_info, __ret, pid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_adcall_msg_register(pid_t pid, struct __actvret_adcall_msg_register *__ret)
{
	return actvxactcall_adcall_msg_register(false, false, pid, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_adcall_msg_register(rref_t __rref, pid_t pid, struct __actvret_adcall_msg_register *__ret)
{
	return actvxactcapcall_adcall_msg_register(false, false, __rref, pid, __ret);
}

#endif
#include <libsysif/utils.h>
#include <hmasm/actv_rpc.h>
#include <hmasm/types.h>
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
extern int actvhdlr_adhandler_msg_recv(unsigned long long sender, unsigned long credential, unsigned int port, const void *src_data, size_t len);


extern int actvhdlr_adhandler_msg_register(unsigned long long sender, unsigned long credential, pid_t pid);


#else /* aarch64 */
extern int actvhdlr_adhandler_msg_recv(unsigned long long sender, unsigned long credential, unsigned int port, const void *src_data, size_t len);


extern int actvhdlr_adhandler_msg_register(unsigned long long sender, unsigned long credential, pid_t pid);


extern int __actvshadowhdlr_adhandler_msg_recv(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_adhandler_msg_register(unsigned long long sender, unsigned long credential, ...);


#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
static inline struct __actvret_adcall_msg_recv* actvhdlr_adhandler_msg_recv_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_adcall_msg_recv));
	return (struct __actvret_adcall_msg_recv*)__ret;
}

static inline struct __actvret_adcall_msg_register* actvhdlr_adhandler_msg_register_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_adcall_msg_register));
	return (struct __actvret_adcall_msg_register*)__ret;
}

#ifdef __sysif_server_auditmgr_no_compat32_handlers__
#ifdef __HOST_LLT__
union auditmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_adhandler_msg_recv)(unsigned long long sender, unsigned long credential, unsigned int port, const void *src_data, size_t len);
	int (*p__actvbridgecapcallhdlr_adhandler_msg_recv)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int port, const void *src_data, size_t len);
	int (*p__actvbridgehdlr_adhandler_msg_register)(unsigned long long sender, unsigned long credential, pid_t pid);
	int (*p__actvbridgecapcallhdlr_adhandler_msg_register)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union auditmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_adhandler_msg_recv)(unsigned long long sender, unsigned long credential, unsigned int port, const void *src_data, size_t len);
	int (*p__actvbridgecapcallhdlr_adhandler_msg_recv)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int port, const void *src_data, size_t len);
	int (*p__actvbridgehdlr_adhandler_msg_register)(unsigned long long sender, unsigned long credential, pid_t pid);
	int (*p__actvbridgecapcallhdlr_adhandler_msg_register)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#ifdef __arm__
union auditmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_adhandler_msg_recv)(unsigned long long sender, unsigned long credential, unsigned int port, const void *src_data, size_t len);
	int (*p__actvbridgecapcallhdlr_adhandler_msg_recv)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int port, const void *src_data, size_t len);
	int (*p__actvbridgehdlr_adhandler_msg_register)(unsigned long long sender, unsigned long credential, pid_t pid);
	int (*p__actvbridgecapcallhdlr_adhandler_msg_register)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential);
};

#endif /* arm */
#endif /* not HOST_LLT */
#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif /* not __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#ifdef __HOST_LLT__
union auditmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_adhandler_msg_recv)(unsigned long long sender, unsigned long credential, unsigned int port, const void *src_data, size_t len);
	int (*p__actvbridgecapcallhdlr_adhandler_msg_recv)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int port, const void *src_data, size_t len);
	int (*p__actvbridgehdlr_adhandler_msg_register)(unsigned long long sender, unsigned long credential, pid_t pid);
	int (*p__actvbridgecapcallhdlr_adhandler_msg_register)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid);
	int (*p__actvbridgehdlr_compat_adhandler_msg_recv)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_adhandler_msg_recv)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_adhandler_msg_register)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_adhandler_msg_register)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union auditmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_adhandler_msg_recv)(unsigned long long sender, unsigned long credential, unsigned int port, const void *src_data, size_t len);
	int (*p__actvbridgecapcallhdlr_adhandler_msg_recv)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int port, const void *src_data, size_t len);
	int (*p__actvbridgehdlr_adhandler_msg_register)(unsigned long long sender, unsigned long credential, pid_t pid);
	int (*p__actvbridgecapcallhdlr_adhandler_msg_register)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid);
	int (*p__actvbridgehdlr_compat_adhandler_msg_recv)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_adhandler_msg_recv)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_adhandler_msg_register)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_adhandler_msg_register)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#endif /* not HOST_LLT */
#endif /* compat32 */

#if defined(__sysif_server_auditmgr_no_compat32_handlers__) && defined(__sysif_server_auditmgr_no_ilp32_handlers__)
#define __auditmgr_method_TIMES 1
#elif !defined(__sysif_server_auditmgr_no_compat32_handlers__) && !defined(__sysif_server_auditmgr_no_ilp32_handlers__)
#define __auditmgr_method_TIMES 3
#else
#define __auditmgr_method_TIMES 2
#endif
extern union auditmgr_hdlrs_union sysif_auditmgr_actvhdlr_table[__auditmgr_method_MAX * __auditmgr_method_TIMES];

extern union auditmgr_hdlrs_union sysif_auditmgr_actvcapcallhdlr_table[__auditmgr_method_MAX * __auditmgr_method_TIMES];

extern void sysif_show_auditmgr_methods(void);


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
static int __opt_nonoframe __actvbridgehdlr_adhandler_msg_recv(unsigned long long sender, unsigned long credential, unsigned int port, const void *src_data, size_t len)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_adhandler_msg_recv(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, port), __ARCH_ACTV_RPC_RECV_ARG(const void*, src_data), __ARCH_ACTV_RPC_RECV_ARG(size_t, len));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_adhandler_msg_recv(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int port, const void *src_data, size_t len)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_adhandler_msg_recv(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, port), __ARCH_ACTV_RPC_RECV_ARG(const void*, src_data), __ARCH_ACTV_RPC_RECV_ARG(size_t, len));
}

static int __opt_nonoframe __actvbridgehdlr_adhandler_msg_register(unsigned long long sender, unsigned long credential, pid_t pid)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_adhandler_msg_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_adhandler_msg_register(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_adhandler_msg_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid));
}

#ifndef __sysif_server_auditmgr_no_compat32_handlers__
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
static int __opt_nonoframe __actvbridgehdlr_compat_adhandler_msg_recv(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _port;
	__maybe_unused const void *_src_data;
	size_t __maybe_unused _len;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_port, data, 0, arg_sz, model, type, unsigned int, _port)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const void*);
		type = __type_of(const void*);
		__fetch_one_arg((void*)&_src_data, data, 1, arg_sz, model, type, const void*, _src_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_len, data, 2, arg_sz, model, type, size_t, _len)
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
	return __actvshadowhdlr_adhandler_msg_recv(sender, credential, _port, _src_data, _len);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_adhandler_msg_recv(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _port;
	__maybe_unused const void *_src_data;
	size_t __maybe_unused _len;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_port, data, 0, arg_sz, model, type, unsigned int, _port)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const void*);
		type = __type_of(const void*);
		__fetch_one_arg((void*)&_src_data, data, 1, arg_sz, model, type, const void*, _src_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(size_t);
		type = __type_of(size_t);
		__fetch_one_arg((void*)&_len, data, 2, arg_sz, model, type, size_t, _len)
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
	return __actvshadowhdlr_adhandler_msg_recv(sender, credential, _port, _src_data, _len);
}

static int __opt_nonoframe __actvbridgehdlr_compat_adhandler_msg_register(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	pid_t __maybe_unused _pid;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(pid_t);
		type = __type_of(pid_t);
		__fetch_one_arg((void*)&_pid, data, 0, arg_sz, model, type, pid_t, _pid)
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
	return __actvshadowhdlr_adhandler_msg_register(sender, credential, _pid);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_adhandler_msg_register(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	pid_t __maybe_unused _pid;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(pid_t);
		type = __type_of(pid_t);
		__fetch_one_arg((void*)&_pid, data, 0, arg_sz, model, type, pid_t, _pid)
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
	return __actvshadowhdlr_adhandler_msg_register(sender, credential, _pid);
}

#endif /* compat handlers */
#ifndef __sysif_server_auditmgr_no_ilp32_handlers__
static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_adhandler_msg_recv(unsigned long long sender, unsigned long credential, unsigned int port, const void *src_data, size_t len)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, port)
	} while (0);
	do {
		__adjust_one_arg(const void*, src_data)
	} while (0);
	do {
		__adjust_one_arg(size_t, len)
	} while (0);
	return __actvshadowhdlr_adhandler_msg_recv(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, port), __ARCH_ACTV_RPC_RECV_ARG(const void*, src_data), __ARCH_ACTV_RPC_RECV_ARG(size_t, len));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_adhandler_msg_recv(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int port, const void *src_data, size_t len)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, port)
	} while (0);
	do {
		__adjust_one_arg(const void*, src_data)
	} while (0);
	do {
		__adjust_one_arg(size_t, len)
	} while (0);
	return __actvshadowhdlr_adhandler_msg_recv(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, port), __ARCH_ACTV_RPC_RECV_ARG(const void*, src_data), __ARCH_ACTV_RPC_RECV_ARG(size_t, len));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_adhandler_msg_register(unsigned long long sender, unsigned long credential, pid_t pid)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(pid_t, pid)
	} while (0);
	return __actvshadowhdlr_adhandler_msg_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_adhandler_msg_register(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(pid_t, pid)
	} while (0);
	return __actvshadowhdlr_adhandler_msg_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid));
}

#endif
#else
static int __opt_nonoframe __actvbridgehdlr_adhandler_msg_recv(unsigned long long sender, unsigned long credential, unsigned int port, const void *src_data, size_t len)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_adhandler_msg_recv(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, port), __ARCH_ACTV_RPC_RECV_ARG(const void*, src_data), __ARCH_ACTV_RPC_RECV_ARG(size_t, len));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_adhandler_msg_recv(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int port, const void *src_data, size_t len)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_adhandler_msg_recv(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, port), __ARCH_ACTV_RPC_RECV_ARG(const void*, src_data), __ARCH_ACTV_RPC_RECV_ARG(size_t, len));
}

static int __opt_nonoframe __actvbridgehdlr_adhandler_msg_register(unsigned long long sender, unsigned long credential, pid_t pid)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_adhandler_msg_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_adhandler_msg_register(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_adhandler_msg_register(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid));
}

#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#if defined(__sysif_server_auditmgr_no_compat32_handlers__) && defined(__sysif_server_auditmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union auditmgr_hdlrs_union sysif_auditmgr_actvhdlr_table[__auditmgr_method_MAX * __auditmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_recv] = {
		.p__actvbridgehdlr_adhandler_msg_recv = (&__actvbridgehdlr_adhandler_msg_recv)
	},
	[ __auditmgr_method_msg_register] = {
		.p__actvbridgehdlr_adhandler_msg_register = (&__actvbridgehdlr_adhandler_msg_register)
	}
};

__ro_after_init_hdlr union auditmgr_hdlrs_union sysif_auditmgr_actvcapcallhdlr_table[__auditmgr_method_MAX * __auditmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_recv] = {
		.p__actvbridgecapcallhdlr_adhandler_msg_recv = (&__actvbridgecapcallhdlr_adhandler_msg_recv)
	},
	[ __auditmgr_method_msg_register] = {
		.p__actvbridgecapcallhdlr_adhandler_msg_register = (&__actvbridgecapcallhdlr_adhandler_msg_register)
	}
};

#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif
#if !defined(__sysif_server_auditmgr_no_compat32_handlers__) && defined(__sysif_server_auditmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union auditmgr_hdlrs_union sysif_auditmgr_actvhdlr_table[__auditmgr_method_MAX * __auditmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_recv] = {
		.p__actvbridgehdlr_adhandler_msg_recv = (&__actvbridgehdlr_adhandler_msg_recv)
	},
	[ __auditmgr_method_msg_register] = {
		.p__actvbridgehdlr_adhandler_msg_register = (&__actvbridgehdlr_adhandler_msg_register)
	},
	[__auditmgr_method_MAX ... __auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __auditmgr_method_MAX + __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_recv] = {
		.p__actvbridgehdlr_compat_adhandler_msg_recv = (&__actvbridgehdlr_compat_adhandler_msg_recv)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_register] = {
		.p__actvbridgehdlr_compat_adhandler_msg_register = (&__actvbridgehdlr_compat_adhandler_msg_register)
	}
};

__ro_after_init_hdlr union auditmgr_hdlrs_union sysif_auditmgr_actvcapcallhdlr_table[__auditmgr_method_MAX * __auditmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_recv] = {
		.p__actvbridgecapcallhdlr_adhandler_msg_recv = (&__actvbridgecapcallhdlr_adhandler_msg_recv)
	},
	[ __auditmgr_method_msg_register] = {
		.p__actvbridgecapcallhdlr_adhandler_msg_register = (&__actvbridgecapcallhdlr_adhandler_msg_register)
	},
	[__auditmgr_method_MAX ... __auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __auditmgr_method_MAX + __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_recv] = {
		.p__actvbridgecapcallhdlr_compat_adhandler_msg_recv = (&__actvbridgecapcallhdlr_compat_adhandler_msg_recv)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_register] = {
		.p__actvbridgecapcallhdlr_compat_adhandler_msg_register = (&__actvbridgecapcallhdlr_compat_adhandler_msg_register)
	}
};

#endif /* aarch64 + compat */
#if defined(__sysif_server_auditmgr_no_compat32_handlers__) && !defined(__sysif_server_auditmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union auditmgr_hdlrs_union sysif_auditmgr_actvhdlr_table[__auditmgr_method_MAX * __auditmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_recv] = {
		.p__actvbridgehdlr_adhandler_msg_recv = (&__actvbridgehdlr_adhandler_msg_recv)
	},
	[ __auditmgr_method_msg_register] = {
		.p__actvbridgehdlr_adhandler_msg_register = (&__actvbridgehdlr_adhandler_msg_register)
	},
	[__auditmgr_method_MAX ... __auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __auditmgr_method_MAX + __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_recv] = {
		.p__actvbridgehdlr_adhandler_msg_recv = (&__actvbridgehdlr_ilp32_adhandler_msg_recv)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_register] = {
		.p__actvbridgehdlr_adhandler_msg_register = (&__actvbridgehdlr_ilp32_adhandler_msg_register)
	}
};

__ro_after_init_hdlr union auditmgr_hdlrs_union sysif_auditmgr_actvcapcallhdlr_table[__auditmgr_method_MAX * __auditmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_recv] = {
		.p__actvbridgecapcallhdlr_adhandler_msg_recv = (&__actvbridgecapcallhdlr_adhandler_msg_recv)
	},
	[ __auditmgr_method_msg_register] = {
		.p__actvbridgecapcallhdlr_adhandler_msg_register = (&__actvbridgecapcallhdlr_adhandler_msg_register)
	},
	[__auditmgr_method_MAX ... __auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __auditmgr_method_MAX + __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_recv] = {
		.p__actvbridgecapcallhdlr_adhandler_msg_recv = (&__actvbridgecapcallhdlr_ilp32_adhandler_msg_recv)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_register] = {
		.p__actvbridgecapcallhdlr_adhandler_msg_register = (&__actvbridgecapcallhdlr_ilp32_adhandler_msg_register)
	}
};

#endif /* aarch64 + ilp32 */
#if !defined(__sysif_server_auditmgr_no_compat32_handlers__) && !defined(__sysif_server_auditmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union auditmgr_hdlrs_union sysif_auditmgr_actvhdlr_table[__auditmgr_method_MAX * __auditmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_recv] = {
		.p__actvbridgehdlr_adhandler_msg_recv = (&__actvbridgehdlr_adhandler_msg_recv)
	},
	[ __auditmgr_method_msg_register] = {
		.p__actvbridgehdlr_adhandler_msg_register = (&__actvbridgehdlr_adhandler_msg_register)
	},
	[__auditmgr_method_MAX ... __auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __auditmgr_method_MAX + __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_recv] = {
		.p__actvbridgehdlr_compat_adhandler_msg_recv = (&__actvbridgehdlr_compat_adhandler_msg_recv)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_register] = {
		.p__actvbridgehdlr_compat_adhandler_msg_register = (&__actvbridgehdlr_compat_adhandler_msg_register)
	},
	[2 * __auditmgr_method_MAX ... 2 * __auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[2 * __auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... 2 * __auditmgr_method_MAX + __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __auditmgr_method_MAX + __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __auditmgr_method_MAX + __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __auditmgr_method_MAX + __auditmgr_method_msg_recv] = {
		.p__actvbridgehdlr_adhandler_msg_recv = (&__actvbridgehdlr_ilp32_adhandler_msg_recv)
	},
	[2 * __auditmgr_method_MAX + __auditmgr_method_msg_register] = {
		.p__actvbridgehdlr_adhandler_msg_register = (&__actvbridgehdlr_ilp32_adhandler_msg_register)
	}
};

__ro_after_init_hdlr union auditmgr_hdlrs_union sysif_auditmgr_actvcapcallhdlr_table[__auditmgr_method_MAX * __auditmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __auditmgr_method_msg_recv] = {
		.p__actvbridgecapcallhdlr_adhandler_msg_recv = (&__actvbridgecapcallhdlr_adhandler_msg_recv)
	},
	[ __auditmgr_method_msg_register] = {
		.p__actvbridgecapcallhdlr_adhandler_msg_register = (&__actvbridgecapcallhdlr_adhandler_msg_register)
	},
	[__auditmgr_method_MAX ... __auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __auditmgr_method_MAX + __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_recv] = {
		.p__actvbridgecapcallhdlr_compat_adhandler_msg_recv = (&__actvbridgecapcallhdlr_compat_adhandler_msg_recv)
	},
	[__auditmgr_method_MAX + __auditmgr_method_msg_register] = {
		.p__actvbridgecapcallhdlr_compat_adhandler_msg_register = (&__actvbridgecapcallhdlr_compat_adhandler_msg_register)
	},
	[2 * __auditmgr_method_MAX ... 2 * __auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[2 * __auditmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... 2 * __auditmgr_method_MAX + __auditmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __auditmgr_method_MAX + __auditmgr_method_msg_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __auditmgr_method_MAX + __auditmgr_method_msg_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __auditmgr_method_MAX + __auditmgr_method_msg_recv] = {
		.p__actvbridgecapcallhdlr_adhandler_msg_recv = (&__actvbridgecapcallhdlr_ilp32_adhandler_msg_recv)
	},
	[2 * __auditmgr_method_MAX + __auditmgr_method_msg_register] = {
		.p__actvbridgecapcallhdlr_adhandler_msg_register = (&__actvbridgecapcallhdlr_ilp32_adhandler_msg_register)
	}
};

#endif /* aarch64 + compat + ilp32 */
#endif /* arm or aarch64 only */
#ifdef __aarch64__
void sysif_show_auditmgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "adcall", "msg", "recv", __auditmgr_method_msg_recv);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "adhandler", "msg", "recv", __auditmgr_method_msg_recv);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "adcall", "msg", "register", __auditmgr_method_msg_register);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "adhandler", "msg", "register", __auditmgr_method_msg_register);
	hm_info("%s_%s, max method num: %d\n", "auditmgr", "method", __auditmgr_method_MAX);
}

#elif defined(__arm__)
void sysif_show_auditmgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "adcall", "msg", "recv", __auditmgr_method_msg_recv);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "adhandler", "msg", "recv", __auditmgr_method_msg_recv);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "adcall", "msg", "register", __auditmgr_method_msg_register);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "adhandler", "msg", "register", __auditmgr_method_msg_register);
	hm_info("%s_%s, max method num: %d\n", "auditmgr", "method", __auditmgr_method_MAX);
}

#else
void sysif_show_auditmgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "adcall", "msg", "recv", __auditmgr_method_msg_recv);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "adhandler", "msg", "recv", __auditmgr_method_msg_recv);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "adcall", "msg", "register", __auditmgr_method_msg_register);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "adhandler", "msg", "register", __auditmgr_method_msg_register);
	hm_info("%s_%s, max method num: %d\n", "auditmgr", "method", __auditmgr_method_MAX);
}

#endif
#endif
#endif
