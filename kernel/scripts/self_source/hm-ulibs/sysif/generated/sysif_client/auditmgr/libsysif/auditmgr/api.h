/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for auditmgr
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../ulibs/include/libsysif/auditmgr/api.sysif -I ../ulibs/include --api-dest ../image/usr/include/generated/sysif_client/auditmgr/libsysif/auditmgr/api.h --server-dest ../image/usr/include/generated/sysif_server/auditmgr/libsysif/auditmgr/server.h --dump-callno ../image/usr/include/generated/dump_info/auditmgr/libsysif/auditmgr/l2h_map.h --legacy-mode
* Create: Tue Sep 16 04:31:10 2025
*/
#ifndef LIBSYSIF_AUDITMGR_API_H
#define LIBSYSIF_AUDITMGR_API_H
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

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_adcall_msg_recv(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_adcall_msg_recv(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_adcall_msg_recv(unsigned long long __rpc_info, void *__buf, unsigned int port, const void *src_data, size_t len)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_port = port;
	__maybe_unused const void *__actv_local_var_src_data = src_data;
	size_t __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcall_adcall_msg_recv(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(port, __actv_local_var_port), __ARCH_ACTV_RPC_PASS_ARG(src_data, __actv_local_var_src_data), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

int __internal_actvcall __actvcapcall_adcall_msg_recv(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int port, const void *src_data, size_t len)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_port = port;
	__maybe_unused const void *__actv_local_var_src_data = src_data;
	size_t __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcapcall_adcall_msg_recv(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(port, __actv_local_var_port), __ARCH_ACTV_RPC_PASS_ARG(src_data, __actv_local_var_src_data), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_adcall_msg_register(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_adcall_msg_register(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_adcall_msg_register(unsigned long long __rpc_info, void *__buf, pid_t pid)
{
	int __ret;
	pid_t __maybe_unused __actv_local_var_pid = pid;
	__ret = ____actvcall_adcall_msg_register(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(pid, __actv_local_var_pid));
	return __ret;
}

int __internal_actvcall __actvcapcall_adcall_msg_register(rref_t __rref, unsigned long long __rpc_info, void *__buf, pid_t pid)
{
	int __ret;
	pid_t __maybe_unused __actv_local_var_pid = pid;
	__ret = ____actvcapcall_adcall_msg_register(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(pid, __actv_local_var_pid));
	return __ret;
}

#endif
#endif
