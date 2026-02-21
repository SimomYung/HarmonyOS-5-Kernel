/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for srvmgr
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../ulibs/include/libsysif/srvmgr/api.sysif -I ../ulibs/include --api-dest ../image/usr/include/generated/sysif_client/srvmgr/libsysif/srvmgr/api.h --server-dest ../image/usr/include/generated/sysif_server/srvmgr/libsysif/srvmgr/server.h --dump-callno ../image/usr/include/generated/dump_info/srvmgr/libsysif/srvmgr/l2h_map.h --legacy-mode
* Create: Tue Sep 16 04:31:10 2025
*/
#ifndef LIBSYSIF_SRVMGR_SERVER_H
#define LIBSYSIF_SRVMGR_SERVER_H
#ifndef LIBSYSIF_SRVMGR_COMMON_H
#define LIBSYSIF_SRVMGR_COMMON_H
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

#define SRVMGR_SERVER_PATH		"srvmgr-actv"


#include <hmkernel/drivers/rebootcode.h>
enum {
	HMINIT_EVENT_TYPE_READY,
};

struct reboot_args {
	unsigned int reboot_flag;
	char reboot_reason[REBOOT_STR_MAX_LEN];
};

raw_static_assert(sizeof(pid_t) <= 96,
                    srvcall_srvmgr_set_dh_pid_arg_dh_pid_is_too_large);
raw_static_assert(sizeof(struct reboot_args) <= 96,
                    srvcall_srvmgr_reboot_arg_args_is_too_large);
struct __actvret_srvcall_srvmgr_shutdown {
};

struct __actvret_srvcall_srvmgr_softreboot {
};

struct __actvret_srvcall_srvmgr_notify {
};

struct __actvret_srvcall_srvmgr_set_dh_pid {
};

struct __actvret_srvcall_srvmgr_reboot {
};

struct __actvret_srvcall_srvmgr_kexec {
};

#ifdef __aarch64__
union __actvret_srvcall {
	struct __actvret_srvcall_srvmgr_shutdown srvmgr_shutdown;
	struct __actvret_srvcall_srvmgr_softreboot srvmgr_softreboot;
	struct __actvret_srvcall_srvmgr_notify srvmgr_notify;
	struct __actvret_srvcall_srvmgr_set_dh_pid srvmgr_set_dh_pid;
	struct __actvret_srvcall_srvmgr_reboot srvmgr_reboot;
	struct __actvret_srvcall_srvmgr_kexec srvmgr_kexec;
};

#elif defined(__arm__)
union __actvret_srvcall {
	struct __actvret_srvcall_srvmgr_shutdown srvmgr_shutdown;
	struct __actvret_srvcall_srvmgr_softreboot srvmgr_softreboot;
	struct __actvret_srvcall_srvmgr_notify srvmgr_notify;
	struct __actvret_srvcall_srvmgr_set_dh_pid srvmgr_set_dh_pid;
	struct __actvret_srvcall_srvmgr_reboot srvmgr_reboot;
	struct __actvret_srvcall_srvmgr_kexec srvmgr_kexec;
};

#else
union __actvret_srvcall {
	struct __actvret_srvcall_srvmgr_shutdown srvmgr_shutdown;
	struct __actvret_srvcall_srvmgr_softreboot srvmgr_softreboot;
	struct __actvret_srvcall_srvmgr_notify srvmgr_notify;
	struct __actvret_srvcall_srvmgr_set_dh_pid srvmgr_set_dh_pid;
	struct __actvret_srvcall_srvmgr_reboot srvmgr_reboot;
	struct __actvret_srvcall_srvmgr_kexec srvmgr_kexec;
};

#endif
enum __srvmgr_mgr {
	__srvmgr_INVAL_mgr=0,
	__srvmgr_srvmgr_mgr,
	__srvmgr_MAX_mgr,
};

#define __srvmgr_method_MIN __ACTV_HIGHTABLE_START
#define __srvmgr_method_srvmgr_shutdown (__ACTV_HIGHTABLE_START + 1)
#define __srvmgr_method_srvmgr_softreboot (__ACTV_HIGHTABLE_START + 2)
#define __srvmgr_method_srvmgr_notify (__ACTV_HIGHTABLE_START + 3)
#define __srvmgr_method_srvmgr_set_dh_pid (__ACTV_HIGHTABLE_START + 4)
#define __srvmgr_method_srvmgr_reboot (__ACTV_HIGHTABLE_START + 5)
#define __srvmgr_method_srvmgr_kexec (__ACTV_HIGHTABLE_START + 6)
#define __srvmgr_method_MAX (__ACTV_HIGHTABLE_START + 7)
raw_static_assert(sizeof(struct __actvret_srvcall_srvmgr_shutdown) < 512,
            srvcall_srvmgr_shutdown_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_srvcall_srvmgr_softreboot) < 512,
            srvcall_srvmgr_softreboot_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_srvcall_srvmgr_notify) < 512,
            srvcall_srvmgr_notify_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_srvcall_srvmgr_set_dh_pid) < 512,
            srvcall_srvmgr_set_dh_pid_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_srvcall_srvmgr_reboot) < 512,
            srvcall_srvmgr_reboot_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_srvcall_srvmgr_kexec) < 512,
            srvcall_srvmgr_kexec_too_many_actvret_data);
raw_static_assert(sizeof(union __actvret_srvcall) < 512,
        srvcall_too_many_actvret_data);
extern int __actvcall_srvcall_srvmgr_shutdown(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_srvcall_srvmgr_shutdown(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_srvcall_srvmgr_shutdown(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __srvmgr_method_srvmgr_shutdown;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_srvcall_srvmgr_shutdown(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_srvcall_srvmgr_shutdown(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __srvmgr_method_srvmgr_shutdown;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_srvcall_srvmgr_shutdown(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_srvcall_srvmgr_shutdown(void)
{
	return actvxactcall_srvcall_srvmgr_shutdown(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_srvcall_srvmgr_shutdown(rref_t __rref)
{
	return actvxactcapcall_srvcall_srvmgr_shutdown(false, false, __rref);
}

extern int __actvcall_srvcall_srvmgr_softreboot(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_srvcall_srvmgr_softreboot(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_srvcall_srvmgr_softreboot(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __srvmgr_method_srvmgr_softreboot;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_srvcall_srvmgr_softreboot(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_srvcall_srvmgr_softreboot(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __srvmgr_method_srvmgr_softreboot;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_srvcall_srvmgr_softreboot(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_srvcall_srvmgr_softreboot(void)
{
	return actvxactcall_srvcall_srvmgr_softreboot(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_srvcall_srvmgr_softreboot(rref_t __rref)
{
	return actvxactcapcall_srvcall_srvmgr_softreboot(false, false, __rref);
}

extern int __actvcall_srvcall_srvmgr_notify(unsigned long long __rpc_info, void *__buf, int event);


extern int __actvcapcall_srvcall_srvmgr_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, int event);


static inline int __attribute__((always_inline)) actvxactcall_srvcall_srvmgr_notify(bool is_xact_fwd, bool is_xact_allow_refwd, int event)
{
	const unsigned long __callno = __srvmgr_method_srvmgr_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_srvcall_srvmgr_notify(rpc_info, NULL, event);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_srvcall_srvmgr_notify(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int event)
{
	const unsigned long __callno = __srvmgr_method_srvmgr_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_srvcall_srvmgr_notify(__rref, rpc_info, NULL, event);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_srvcall_srvmgr_notify(int event)
{
	return actvxactcall_srvcall_srvmgr_notify(false, false, event);
}

static inline int __attribute__((always_inline)) actvcapcall_srvcall_srvmgr_notify(rref_t __rref, int event)
{
	return actvxactcapcall_srvcall_srvmgr_notify(false, false, __rref, event);
}

extern int __actvcall_srvcall_srvmgr_set_dh_pid(unsigned long long __rpc_info, void *__buf, pid_t dh_pid);


extern int __actvcapcall_srvcall_srvmgr_set_dh_pid(rref_t __rref, unsigned long long __rpc_info, void *__buf, pid_t dh_pid);


static inline int __attribute__((always_inline)) actvxactcall_srvcall_srvmgr_set_dh_pid(bool is_xact_fwd, bool is_xact_allow_refwd, pid_t dh_pid)
{
	const unsigned long __callno = __srvmgr_method_srvmgr_set_dh_pid;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_srvcall_srvmgr_set_dh_pid(rpc_info, NULL, dh_pid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_srvcall_srvmgr_set_dh_pid(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, pid_t dh_pid)
{
	const unsigned long __callno = __srvmgr_method_srvmgr_set_dh_pid;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_srvcall_srvmgr_set_dh_pid(__rref, rpc_info, NULL, dh_pid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_srvcall_srvmgr_set_dh_pid(pid_t dh_pid)
{
	return actvxactcall_srvcall_srvmgr_set_dh_pid(false, false, dh_pid);
}

static inline int __attribute__((always_inline)) actvcapcall_srvcall_srvmgr_set_dh_pid(rref_t __rref, pid_t dh_pid)
{
	return actvxactcapcall_srvcall_srvmgr_set_dh_pid(false, false, __rref, dh_pid);
}

extern int __actvcall_srvcall_srvmgr_reboot(unsigned long long __rpc_info, void *__buf, struct reboot_args args);


extern int __actvcapcall_srvcall_srvmgr_reboot(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct reboot_args args);


static inline int __attribute__((always_inline)) actvxactcall_srvcall_srvmgr_reboot(bool is_xact_fwd, bool is_xact_allow_refwd, struct reboot_args args)
{
	const unsigned long __callno = __srvmgr_method_srvmgr_reboot;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_args), __arch_actv_rpc_stksz_alignment(struct reboot_args));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_srvcall_srvmgr_reboot(rpc_info, NULL, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_srvcall_srvmgr_reboot(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct reboot_args args)
{
	const unsigned long __callno = __srvmgr_method_srvmgr_reboot;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_args), __arch_actv_rpc_stksz_alignment(struct reboot_args));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_srvcall_srvmgr_reboot(__rref, rpc_info, NULL, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_srvcall_srvmgr_reboot(struct reboot_args args)
{
	return actvxactcall_srvcall_srvmgr_reboot(false, false, args);
}

static inline int __attribute__((always_inline)) actvcapcall_srvcall_srvmgr_reboot(rref_t __rref, struct reboot_args args)
{
	return actvxactcapcall_srvcall_srvmgr_reboot(false, false, __rref, args);
}

extern int __actvcall_srvcall_srvmgr_kexec(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_srvcall_srvmgr_kexec(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_srvcall_srvmgr_kexec(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __srvmgr_method_srvmgr_kexec;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_srvcall_srvmgr_kexec(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_srvcall_srvmgr_kexec(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __srvmgr_method_srvmgr_kexec;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_srvcall_srvmgr_kexec(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_srvcall_srvmgr_kexec(void)
{
	return actvxactcall_srvcall_srvmgr_kexec(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_srvcall_srvmgr_kexec(rref_t __rref)
{
	return actvxactcapcall_srvcall_srvmgr_kexec(false, false, __rref);
}

#endif
#include <libsysif/utils.h>
#include <hmasm/actv_rpc.h>
#include <hmasm/types.h>
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
extern int actvhdlr_srvhandler_srvmgr_shutdown(unsigned long long sender, unsigned long credential);


extern int actvhdlr_srvhandler_srvmgr_softreboot(unsigned long long sender, unsigned long credential);


extern int actvhdlr_srvhandler_srvmgr_notify(unsigned long long sender, unsigned long credential, int event);


extern int actvhdlr_srvhandler_srvmgr_set_dh_pid(unsigned long long sender, unsigned long credential, pid_t dh_pid);


extern int actvhdlr_srvhandler_srvmgr_reboot(unsigned long long sender, unsigned long credential, struct reboot_args args);


extern int actvhdlr_srvhandler_srvmgr_kexec(unsigned long long sender, unsigned long credential);


#else /* aarch64 */
extern int actvhdlr_srvhandler_srvmgr_shutdown(unsigned long long sender, unsigned long credential);


extern int actvhdlr_srvhandler_srvmgr_softreboot(unsigned long long sender, unsigned long credential);


extern int actvhdlr_srvhandler_srvmgr_notify(unsigned long long sender, unsigned long credential, int event);


extern int actvhdlr_srvhandler_srvmgr_set_dh_pid(unsigned long long sender, unsigned long credential, pid_t dh_pid);


extern int actvhdlr_srvhandler_srvmgr_reboot(unsigned long long sender, unsigned long credential, struct reboot_args args);


extern int actvhdlr_srvhandler_srvmgr_kexec(unsigned long long sender, unsigned long credential);


extern int __actvshadowhdlr_srvhandler_srvmgr_shutdown(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_srvhandler_srvmgr_softreboot(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_srvhandler_srvmgr_notify(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_srvhandler_srvmgr_set_dh_pid(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_srvhandler_srvmgr_reboot(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_srvhandler_srvmgr_kexec(unsigned long long sender, unsigned long credential, ...);


#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
static inline struct __actvret_srvcall_srvmgr_shutdown* actvhdlr_srvhandler_srvmgr_shutdown_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_srvcall_srvmgr_shutdown));
	return (struct __actvret_srvcall_srvmgr_shutdown*)__ret;
}

static inline struct __actvret_srvcall_srvmgr_softreboot* actvhdlr_srvhandler_srvmgr_softreboot_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_srvcall_srvmgr_softreboot));
	return (struct __actvret_srvcall_srvmgr_softreboot*)__ret;
}

static inline struct __actvret_srvcall_srvmgr_notify* actvhdlr_srvhandler_srvmgr_notify_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_srvcall_srvmgr_notify));
	return (struct __actvret_srvcall_srvmgr_notify*)__ret;
}

static inline struct __actvret_srvcall_srvmgr_set_dh_pid* actvhdlr_srvhandler_srvmgr_set_dh_pid_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_srvcall_srvmgr_set_dh_pid));
	return (struct __actvret_srvcall_srvmgr_set_dh_pid*)__ret;
}

static inline struct __actvret_srvcall_srvmgr_reboot* actvhdlr_srvhandler_srvmgr_reboot_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_srvcall_srvmgr_reboot));
	return (struct __actvret_srvcall_srvmgr_reboot*)__ret;
}

static inline struct __actvret_srvcall_srvmgr_kexec* actvhdlr_srvhandler_srvmgr_kexec_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_srvcall_srvmgr_kexec));
	return (struct __actvret_srvcall_srvmgr_kexec*)__ret;
}

#ifdef __sysif_server_srvmgr_no_compat32_handlers__
#ifdef __HOST_LLT__
union srvmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_shutdown)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_softreboot)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_notify)(unsigned long long sender, unsigned long credential, int event);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_notify)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int event);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_set_dh_pid)(unsigned long long sender, unsigned long credential, pid_t dh_pid);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t dh_pid);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_reboot)(unsigned long long sender, unsigned long credential, struct reboot_args args);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_reboot)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_args args);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_kexec)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_kexec)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union srvmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_shutdown)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_softreboot)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_notify)(unsigned long long sender, unsigned long credential, int event);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_notify)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int event);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_set_dh_pid)(unsigned long long sender, unsigned long credential, pid_t dh_pid);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t dh_pid);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_reboot)(unsigned long long sender, unsigned long credential, struct reboot_args args);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_reboot)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_args args);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_kexec)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_kexec)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#ifdef __arm__
union srvmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_shutdown)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_softreboot)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_notify)(unsigned long long sender, unsigned long credential, int event);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_notify)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int event);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_set_dh_pid)(unsigned long long sender, unsigned long credential, pid_t dh_pid);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t dh_pid);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_reboot)(unsigned long long sender, unsigned long credential, struct reboot_args args);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_reboot)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_args args);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_kexec)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_kexec)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential);
};

#endif /* arm */
#endif /* not HOST_LLT */
#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif /* not __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#ifdef __HOST_LLT__
union srvmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_shutdown)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_softreboot)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_notify)(unsigned long long sender, unsigned long credential, int event);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_notify)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int event);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_set_dh_pid)(unsigned long long sender, unsigned long credential, pid_t dh_pid);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t dh_pid);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_reboot)(unsigned long long sender, unsigned long credential, struct reboot_args args);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_reboot)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_args args);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_kexec)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_kexec)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_compat_srvhandler_srvmgr_shutdown)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_shutdown)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_srvhandler_srvmgr_softreboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_softreboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_srvhandler_srvmgr_notify)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_notify)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_srvhandler_srvmgr_set_dh_pid)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_set_dh_pid)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_srvhandler_srvmgr_reboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_reboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_srvhandler_srvmgr_kexec)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_kexec)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union srvmgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_shutdown)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_softreboot)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_notify)(unsigned long long sender, unsigned long credential, int event);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_notify)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int event);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_set_dh_pid)(unsigned long long sender, unsigned long credential, pid_t dh_pid);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t dh_pid);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_reboot)(unsigned long long sender, unsigned long credential, struct reboot_args args);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_reboot)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_args args);
	int (*p__actvbridgehdlr_srvhandler_srvmgr_kexec)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_srvhandler_srvmgr_kexec)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_compat_srvhandler_srvmgr_shutdown)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_shutdown)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_srvhandler_srvmgr_softreboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_softreboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_srvhandler_srvmgr_notify)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_notify)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_srvhandler_srvmgr_set_dh_pid)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_set_dh_pid)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_srvhandler_srvmgr_reboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_reboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_srvhandler_srvmgr_kexec)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_kexec)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#endif /* not HOST_LLT */
#endif /* compat32 */

#if defined(__sysif_server_srvmgr_no_compat32_handlers__) && defined(__sysif_server_srvmgr_no_ilp32_handlers__)
#define __srvmgr_method_TIMES 1
#elif !defined(__sysif_server_srvmgr_no_compat32_handlers__) && !defined(__sysif_server_srvmgr_no_ilp32_handlers__)
#define __srvmgr_method_TIMES 3
#else
#define __srvmgr_method_TIMES 2
#endif
extern union srvmgr_hdlrs_union sysif_srvmgr_actvhdlr_table[__srvmgr_method_MAX * __srvmgr_method_TIMES];

extern union srvmgr_hdlrs_union sysif_srvmgr_actvcapcallhdlr_table[__srvmgr_method_MAX * __srvmgr_method_TIMES];

extern void sysif_show_srvmgr_methods(void);


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
static int __opt_nonoframe __actvbridgehdlr_srvhandler_srvmgr_shutdown(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_srvhandler_srvmgr_shutdown(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_srvhandler_srvmgr_shutdown(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_srvhandler_srvmgr_softreboot(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_srvhandler_srvmgr_softreboot(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_srvhandler_srvmgr_softreboot(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_srvhandler_srvmgr_notify(unsigned long long sender, unsigned long credential, int event)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_srvhandler_srvmgr_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, event));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_srvhandler_srvmgr_notify(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int event)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_srvhandler_srvmgr_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, event));
}

static int __opt_nonoframe __actvbridgehdlr_srvhandler_srvmgr_set_dh_pid(unsigned long long sender, unsigned long credential, pid_t dh_pid)
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
	return __actvshadowhdlr_srvhandler_srvmgr_set_dh_pid(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, dh_pid));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t dh_pid)
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
	return __actvshadowhdlr_srvhandler_srvmgr_set_dh_pid(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, dh_pid));
}

static int __opt_nonoframe __actvbridgehdlr_srvhandler_srvmgr_reboot(unsigned long long sender, unsigned long credential, struct reboot_args args)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_args), __arch_actv_rpc_stksz_alignment(struct reboot_args));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_srvhandler_srvmgr_reboot(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct reboot_args, args));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_srvhandler_srvmgr_reboot(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_args args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_args), __arch_actv_rpc_stksz_alignment(struct reboot_args));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_srvhandler_srvmgr_reboot(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct reboot_args, args));
}

static int __opt_nonoframe __actvbridgehdlr_srvhandler_srvmgr_kexec(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_srvhandler_srvmgr_kexec(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_srvhandler_srvmgr_kexec(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_srvhandler_srvmgr_kexec(sender, credential);
}

#ifndef __sysif_server_srvmgr_no_compat32_handlers__
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
static int __opt_nonoframe __actvbridgehdlr_compat_srvhandler_srvmgr_shutdown(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_srvhandler_srvmgr_shutdown(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_srvhandler_srvmgr_shutdown(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_srvhandler_srvmgr_shutdown(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_srvhandler_srvmgr_softreboot(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_srvhandler_srvmgr_softreboot(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_srvhandler_srvmgr_softreboot(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_srvhandler_srvmgr_softreboot(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_srvhandler_srvmgr_notify(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _event;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_event, data, 0, arg_sz, model, type, int, _event)
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
	return __actvshadowhdlr_srvhandler_srvmgr_notify(sender, credential, _event);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_srvhandler_srvmgr_notify(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _event;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_event, data, 0, arg_sz, model, type, int, _event)
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
	return __actvshadowhdlr_srvhandler_srvmgr_notify(sender, credential, _event);
}

static int __opt_nonoframe __actvbridgehdlr_compat_srvhandler_srvmgr_set_dh_pid(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	pid_t __maybe_unused _dh_pid;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(pid_t);
		type = __type_of(pid_t);
		__fetch_one_arg((void*)&_dh_pid, data, 0, arg_sz, model, type, pid_t, _dh_pid)
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
	return __actvshadowhdlr_srvhandler_srvmgr_set_dh_pid(sender, credential, _dh_pid);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_srvhandler_srvmgr_set_dh_pid(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	pid_t __maybe_unused _dh_pid;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(pid_t);
		type = __type_of(pid_t);
		__fetch_one_arg((void*)&_dh_pid, data, 0, arg_sz, model, type, pid_t, _dh_pid)
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
	return __actvshadowhdlr_srvhandler_srvmgr_set_dh_pid(sender, credential, _dh_pid);
}

static int __opt_nonoframe __actvbridgehdlr_compat_srvhandler_srvmgr_reboot(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct reboot_args __maybe_unused _args;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct reboot_args);
		type = __type_of(struct reboot_args);
		__fetch_one_arg((void*)&_args, data, 0, arg_sz, model, type, struct reboot_args, _args)
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
	return __actvshadowhdlr_srvhandler_srvmgr_reboot(sender, credential, _args);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_srvhandler_srvmgr_reboot(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct reboot_args __maybe_unused _args;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct reboot_args);
		type = __type_of(struct reboot_args);
		__fetch_one_arg((void*)&_args, data, 0, arg_sz, model, type, struct reboot_args, _args)
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
	return __actvshadowhdlr_srvhandler_srvmgr_reboot(sender, credential, _args);
}

static int __opt_nonoframe __actvbridgehdlr_compat_srvhandler_srvmgr_kexec(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_srvhandler_srvmgr_kexec(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_srvhandler_srvmgr_kexec(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_srvhandler_srvmgr_kexec(sender, credential);
}

#endif /* compat handlers */
#ifndef __sysif_server_srvmgr_no_ilp32_handlers__
static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_srvhandler_srvmgr_shutdown(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_srvhandler_srvmgr_shutdown(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_shutdown(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_srvhandler_srvmgr_shutdown(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_srvhandler_srvmgr_softreboot(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_srvhandler_srvmgr_softreboot(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_softreboot(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_srvhandler_srvmgr_softreboot(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_srvhandler_srvmgr_notify(unsigned long long sender, unsigned long credential, int event)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, event)
	} while (0);
	return __actvshadowhdlr_srvhandler_srvmgr_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, event));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_notify(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int event)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, event)
	} while (0);
	return __actvshadowhdlr_srvhandler_srvmgr_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, event));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_srvhandler_srvmgr_set_dh_pid(unsigned long long sender, unsigned long credential, pid_t dh_pid)
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
		__adjust_one_arg(pid_t, dh_pid)
	} while (0);
	return __actvshadowhdlr_srvhandler_srvmgr_set_dh_pid(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, dh_pid));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_set_dh_pid(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t dh_pid)
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
		__adjust_one_arg(pid_t, dh_pid)
	} while (0);
	return __actvshadowhdlr_srvhandler_srvmgr_set_dh_pid(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, dh_pid));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_srvhandler_srvmgr_reboot(unsigned long long sender, unsigned long credential, struct reboot_args args)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_args), __arch_actv_rpc_stksz_alignment(struct reboot_args));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct reboot_args, args)
	} while (0);
	return __actvshadowhdlr_srvhandler_srvmgr_reboot(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct reboot_args, args));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_reboot(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_args args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_args), __arch_actv_rpc_stksz_alignment(struct reboot_args));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct reboot_args, args)
	} while (0);
	return __actvshadowhdlr_srvhandler_srvmgr_reboot(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct reboot_args, args));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_srvhandler_srvmgr_kexec(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_srvhandler_srvmgr_kexec(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_kexec(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_srvhandler_srvmgr_kexec(sender, credential);
}

#endif
#else
static int __opt_nonoframe __actvbridgehdlr_srvhandler_srvmgr_shutdown(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_srvhandler_srvmgr_shutdown(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_srvhandler_srvmgr_shutdown(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_srvhandler_srvmgr_softreboot(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_srvhandler_srvmgr_softreboot(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_srvhandler_srvmgr_softreboot(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_srvhandler_srvmgr_notify(unsigned long long sender, unsigned long credential, int event)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_srvhandler_srvmgr_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, event));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_srvhandler_srvmgr_notify(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int event)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_srvhandler_srvmgr_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, event));
}

static int __opt_nonoframe __actvbridgehdlr_srvhandler_srvmgr_set_dh_pid(unsigned long long sender, unsigned long credential, pid_t dh_pid)
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
	return actvhdlr_srvhandler_srvmgr_set_dh_pid(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, dh_pid));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t dh_pid)
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
	return actvhdlr_srvhandler_srvmgr_set_dh_pid(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, dh_pid));
}

static int __opt_nonoframe __actvbridgehdlr_srvhandler_srvmgr_reboot(unsigned long long sender, unsigned long credential, struct reboot_args args)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_args), __arch_actv_rpc_stksz_alignment(struct reboot_args));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_srvhandler_srvmgr_reboot(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct reboot_args, args));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_srvhandler_srvmgr_reboot(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct reboot_args args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_args), __arch_actv_rpc_stksz_alignment(struct reboot_args));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_srvhandler_srvmgr_reboot(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct reboot_args, args));
}

static int __opt_nonoframe __actvbridgehdlr_srvhandler_srvmgr_kexec(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_srvhandler_srvmgr_kexec(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_srvhandler_srvmgr_kexec(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_srvhandler_srvmgr_kexec(sender, credential);
}

#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#if defined(__sysif_server_srvmgr_no_compat32_handlers__) && defined(__sysif_server_srvmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union srvmgr_hdlrs_union sysif_srvmgr_actvhdlr_table[__srvmgr_method_MAX * __srvmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_shutdown = (&__actvbridgehdlr_srvhandler_srvmgr_shutdown)
	},
	[ __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_softreboot = (&__actvbridgehdlr_srvhandler_srvmgr_softreboot)
	},
	[ __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_notify = (&__actvbridgehdlr_srvhandler_srvmgr_notify)
	},
	[ __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_set_dh_pid = (&__actvbridgehdlr_srvhandler_srvmgr_set_dh_pid)
	},
	[ __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_reboot = (&__actvbridgehdlr_srvhandler_srvmgr_reboot)
	},
	[ __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_kexec = (&__actvbridgehdlr_srvhandler_srvmgr_kexec)
	}
};

__ro_after_init_hdlr union srvmgr_hdlrs_union sysif_srvmgr_actvcapcallhdlr_table[__srvmgr_method_MAX * __srvmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown)
	},
	[ __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot)
	},
	[ __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_notify = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_notify)
	},
	[ __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid)
	},
	[ __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_reboot = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_reboot)
	},
	[ __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_kexec = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_kexec)
	}
};

#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif
#if !defined(__sysif_server_srvmgr_no_compat32_handlers__) && defined(__sysif_server_srvmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union srvmgr_hdlrs_union sysif_srvmgr_actvhdlr_table[__srvmgr_method_MAX * __srvmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_shutdown = (&__actvbridgehdlr_srvhandler_srvmgr_shutdown)
	},
	[ __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_softreboot = (&__actvbridgehdlr_srvhandler_srvmgr_softreboot)
	},
	[ __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_notify = (&__actvbridgehdlr_srvhandler_srvmgr_notify)
	},
	[ __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_set_dh_pid = (&__actvbridgehdlr_srvhandler_srvmgr_set_dh_pid)
	},
	[ __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_reboot = (&__actvbridgehdlr_srvhandler_srvmgr_reboot)
	},
	[ __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_kexec = (&__actvbridgehdlr_srvhandler_srvmgr_kexec)
	},
	[__srvmgr_method_MAX ... __srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __srvmgr_method_MAX + __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgehdlr_compat_srvhandler_srvmgr_shutdown = (&__actvbridgehdlr_compat_srvhandler_srvmgr_shutdown)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgehdlr_compat_srvhandler_srvmgr_softreboot = (&__actvbridgehdlr_compat_srvhandler_srvmgr_softreboot)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgehdlr_compat_srvhandler_srvmgr_notify = (&__actvbridgehdlr_compat_srvhandler_srvmgr_notify)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgehdlr_compat_srvhandler_srvmgr_set_dh_pid = (&__actvbridgehdlr_compat_srvhandler_srvmgr_set_dh_pid)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgehdlr_compat_srvhandler_srvmgr_reboot = (&__actvbridgehdlr_compat_srvhandler_srvmgr_reboot)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgehdlr_compat_srvhandler_srvmgr_kexec = (&__actvbridgehdlr_compat_srvhandler_srvmgr_kexec)
	}
};

__ro_after_init_hdlr union srvmgr_hdlrs_union sysif_srvmgr_actvcapcallhdlr_table[__srvmgr_method_MAX * __srvmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown)
	},
	[ __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot)
	},
	[ __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_notify = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_notify)
	},
	[ __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid)
	},
	[ __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_reboot = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_reboot)
	},
	[ __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_kexec = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_kexec)
	},
	[__srvmgr_method_MAX ... __srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __srvmgr_method_MAX + __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_shutdown = (&__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_shutdown)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_softreboot = (&__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_softreboot)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_notify = (&__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_notify)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_set_dh_pid = (&__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_set_dh_pid)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_reboot = (&__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_reboot)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_kexec = (&__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_kexec)
	}
};

#endif /* aarch64 + compat */
#if defined(__sysif_server_srvmgr_no_compat32_handlers__) && !defined(__sysif_server_srvmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union srvmgr_hdlrs_union sysif_srvmgr_actvhdlr_table[__srvmgr_method_MAX * __srvmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_shutdown = (&__actvbridgehdlr_srvhandler_srvmgr_shutdown)
	},
	[ __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_softreboot = (&__actvbridgehdlr_srvhandler_srvmgr_softreboot)
	},
	[ __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_notify = (&__actvbridgehdlr_srvhandler_srvmgr_notify)
	},
	[ __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_set_dh_pid = (&__actvbridgehdlr_srvhandler_srvmgr_set_dh_pid)
	},
	[ __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_reboot = (&__actvbridgehdlr_srvhandler_srvmgr_reboot)
	},
	[ __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_kexec = (&__actvbridgehdlr_srvhandler_srvmgr_kexec)
	},
	[__srvmgr_method_MAX ... __srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __srvmgr_method_MAX + __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_shutdown = (&__actvbridgehdlr_ilp32_srvhandler_srvmgr_shutdown)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_softreboot = (&__actvbridgehdlr_ilp32_srvhandler_srvmgr_softreboot)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_notify = (&__actvbridgehdlr_ilp32_srvhandler_srvmgr_notify)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_set_dh_pid = (&__actvbridgehdlr_ilp32_srvhandler_srvmgr_set_dh_pid)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_reboot = (&__actvbridgehdlr_ilp32_srvhandler_srvmgr_reboot)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_kexec = (&__actvbridgehdlr_ilp32_srvhandler_srvmgr_kexec)
	}
};

__ro_after_init_hdlr union srvmgr_hdlrs_union sysif_srvmgr_actvcapcallhdlr_table[__srvmgr_method_MAX * __srvmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown)
	},
	[ __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot)
	},
	[ __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_notify = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_notify)
	},
	[ __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid)
	},
	[ __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_reboot = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_reboot)
	},
	[ __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_kexec = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_kexec)
	},
	[__srvmgr_method_MAX ... __srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __srvmgr_method_MAX + __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown = (&__actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_shutdown)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot = (&__actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_softreboot)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_notify = (&__actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_notify)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid = (&__actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_set_dh_pid)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_reboot = (&__actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_reboot)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_kexec = (&__actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_kexec)
	}
};

#endif /* aarch64 + ilp32 */
#if !defined(__sysif_server_srvmgr_no_compat32_handlers__) && !defined(__sysif_server_srvmgr_no_ilp32_handlers__)
__ro_after_init_hdlr union srvmgr_hdlrs_union sysif_srvmgr_actvhdlr_table[__srvmgr_method_MAX * __srvmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_shutdown = (&__actvbridgehdlr_srvhandler_srvmgr_shutdown)
	},
	[ __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_softreboot = (&__actvbridgehdlr_srvhandler_srvmgr_softreboot)
	},
	[ __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_notify = (&__actvbridgehdlr_srvhandler_srvmgr_notify)
	},
	[ __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_set_dh_pid = (&__actvbridgehdlr_srvhandler_srvmgr_set_dh_pid)
	},
	[ __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_reboot = (&__actvbridgehdlr_srvhandler_srvmgr_reboot)
	},
	[ __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_kexec = (&__actvbridgehdlr_srvhandler_srvmgr_kexec)
	},
	[__srvmgr_method_MAX ... __srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __srvmgr_method_MAX + __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgehdlr_compat_srvhandler_srvmgr_shutdown = (&__actvbridgehdlr_compat_srvhandler_srvmgr_shutdown)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgehdlr_compat_srvhandler_srvmgr_softreboot = (&__actvbridgehdlr_compat_srvhandler_srvmgr_softreboot)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgehdlr_compat_srvhandler_srvmgr_notify = (&__actvbridgehdlr_compat_srvhandler_srvmgr_notify)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgehdlr_compat_srvhandler_srvmgr_set_dh_pid = (&__actvbridgehdlr_compat_srvhandler_srvmgr_set_dh_pid)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgehdlr_compat_srvhandler_srvmgr_reboot = (&__actvbridgehdlr_compat_srvhandler_srvmgr_reboot)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgehdlr_compat_srvhandler_srvmgr_kexec = (&__actvbridgehdlr_compat_srvhandler_srvmgr_kexec)
	},
	[2 * __srvmgr_method_MAX ... 2 * __srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[2 * __srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... 2 * __srvmgr_method_MAX + __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __srvmgr_method_MAX + __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_shutdown = (&__actvbridgehdlr_ilp32_srvhandler_srvmgr_shutdown)
	},
	[2 * __srvmgr_method_MAX + __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_softreboot = (&__actvbridgehdlr_ilp32_srvhandler_srvmgr_softreboot)
	},
	[2 * __srvmgr_method_MAX + __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_notify = (&__actvbridgehdlr_ilp32_srvhandler_srvmgr_notify)
	},
	[2 * __srvmgr_method_MAX + __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_set_dh_pid = (&__actvbridgehdlr_ilp32_srvhandler_srvmgr_set_dh_pid)
	},
	[2 * __srvmgr_method_MAX + __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_reboot = (&__actvbridgehdlr_ilp32_srvhandler_srvmgr_reboot)
	},
	[2 * __srvmgr_method_MAX + __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgehdlr_srvhandler_srvmgr_kexec = (&__actvbridgehdlr_ilp32_srvhandler_srvmgr_kexec)
	}
};

__ro_after_init_hdlr union srvmgr_hdlrs_union sysif_srvmgr_actvcapcallhdlr_table[__srvmgr_method_MAX * __srvmgr_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown)
	},
	[ __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot)
	},
	[ __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_notify = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_notify)
	},
	[ __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid)
	},
	[ __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_reboot = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_reboot)
	},
	[ __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_kexec = (&__actvbridgecapcallhdlr_srvhandler_srvmgr_kexec)
	},
	[__srvmgr_method_MAX ... __srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __srvmgr_method_MAX + __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_shutdown = (&__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_shutdown)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_softreboot = (&__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_softreboot)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_notify = (&__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_notify)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_set_dh_pid = (&__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_set_dh_pid)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_reboot = (&__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_reboot)
	},
	[__srvmgr_method_MAX + __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_kexec = (&__actvbridgecapcallhdlr_compat_srvhandler_srvmgr_kexec)
	},
	[2 * __srvmgr_method_MAX ... 2 * __srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[2 * __srvmgr_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... 2 * __srvmgr_method_MAX + __srvmgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __srvmgr_method_MAX + __srvmgr_method_srvmgr_shutdown] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_shutdown = (&__actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_shutdown)
	},
	[2 * __srvmgr_method_MAX + __srvmgr_method_srvmgr_softreboot] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_softreboot = (&__actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_softreboot)
	},
	[2 * __srvmgr_method_MAX + __srvmgr_method_srvmgr_notify] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_notify = (&__actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_notify)
	},
	[2 * __srvmgr_method_MAX + __srvmgr_method_srvmgr_set_dh_pid] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_set_dh_pid = (&__actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_set_dh_pid)
	},
	[2 * __srvmgr_method_MAX + __srvmgr_method_srvmgr_reboot] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_reboot = (&__actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_reboot)
	},
	[2 * __srvmgr_method_MAX + __srvmgr_method_srvmgr_kexec] = {
		.p__actvbridgecapcallhdlr_srvhandler_srvmgr_kexec = (&__actvbridgecapcallhdlr_ilp32_srvhandler_srvmgr_kexec)
	}
};

#endif /* aarch64 + compat + ilp32 */
#endif /* arm or aarch64 only */
#ifdef __aarch64__
void sysif_show_srvmgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "shutdown", __srvmgr_method_srvmgr_shutdown);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "shutdown", __srvmgr_method_srvmgr_shutdown);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "softreboot", __srvmgr_method_srvmgr_softreboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "softreboot", __srvmgr_method_srvmgr_softreboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "notify", __srvmgr_method_srvmgr_notify);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "notify", __srvmgr_method_srvmgr_notify);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "set_dh_pid", __srvmgr_method_srvmgr_set_dh_pid);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "set_dh_pid", __srvmgr_method_srvmgr_set_dh_pid);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "reboot", __srvmgr_method_srvmgr_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "reboot", __srvmgr_method_srvmgr_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "kexec", __srvmgr_method_srvmgr_kexec);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "kexec", __srvmgr_method_srvmgr_kexec);
	hm_info("%s_%s, max method num: %d\n", "srvmgr", "method", __srvmgr_method_MAX);
}

#elif defined(__arm__)
void sysif_show_srvmgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "shutdown", __srvmgr_method_srvmgr_shutdown);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "shutdown", __srvmgr_method_srvmgr_shutdown);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "softreboot", __srvmgr_method_srvmgr_softreboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "softreboot", __srvmgr_method_srvmgr_softreboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "notify", __srvmgr_method_srvmgr_notify);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "notify", __srvmgr_method_srvmgr_notify);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "set_dh_pid", __srvmgr_method_srvmgr_set_dh_pid);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "set_dh_pid", __srvmgr_method_srvmgr_set_dh_pid);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "reboot", __srvmgr_method_srvmgr_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "reboot", __srvmgr_method_srvmgr_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "kexec", __srvmgr_method_srvmgr_kexec);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "kexec", __srvmgr_method_srvmgr_kexec);
	hm_info("%s_%s, max method num: %d\n", "srvmgr", "method", __srvmgr_method_MAX);
}

#else
void sysif_show_srvmgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "shutdown", __srvmgr_method_srvmgr_shutdown);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "shutdown", __srvmgr_method_srvmgr_shutdown);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "softreboot", __srvmgr_method_srvmgr_softreboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "softreboot", __srvmgr_method_srvmgr_softreboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "notify", __srvmgr_method_srvmgr_notify);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "notify", __srvmgr_method_srvmgr_notify);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "set_dh_pid", __srvmgr_method_srvmgr_set_dh_pid);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "set_dh_pid", __srvmgr_method_srvmgr_set_dh_pid);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "reboot", __srvmgr_method_srvmgr_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "reboot", __srvmgr_method_srvmgr_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "srvcall", "srvmgr", "kexec", __srvmgr_method_srvmgr_kexec);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "srvhandler", "srvmgr", "kexec", __srvmgr_method_srvmgr_kexec);
	hm_info("%s_%s, max method num: %d\n", "srvmgr", "method", __srvmgr_method_MAX);
}

#endif
int __actvcall_srvcall_srvmgr_shutdown(unsigned long long __rpc_info, void *__buf)
{
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_srvhandler_srvmgr_shutdown((unsigned long long)(uintptr_t)&selfcall, credential);
}

int __actvcapcall_srvcall_srvmgr_shutdown(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	UNUSED(__rref);
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_srvhandler_srvmgr_shutdown((unsigned long long)(uintptr_t)&selfcall, credential);
}

int __actvcall_srvcall_srvmgr_softreboot(unsigned long long __rpc_info, void *__buf)
{
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_srvhandler_srvmgr_softreboot((unsigned long long)(uintptr_t)&selfcall, credential);
}

int __actvcapcall_srvcall_srvmgr_softreboot(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	UNUSED(__rref);
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_srvhandler_srvmgr_softreboot((unsigned long long)(uintptr_t)&selfcall, credential);
}

int __actvcall_srvcall_srvmgr_reboot(unsigned long long __rpc_info, void *__buf, struct reboot_args args)
{
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_srvhandler_srvmgr_reboot((unsigned long long)(uintptr_t)&selfcall, credential, args);
}

int __actvcapcall_srvcall_srvmgr_reboot(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct reboot_args args)
{
	UNUSED(__rref);
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_srvhandler_srvmgr_reboot((unsigned long long)(uintptr_t)&selfcall, credential, args);
}

int __actvcall_srvcall_srvmgr_kexec(unsigned long long __rpc_info, void *__buf)
{
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_srvhandler_srvmgr_kexec((unsigned long long)(uintptr_t)&selfcall, credential);
}

int __actvcapcall_srvcall_srvmgr_kexec(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	UNUSED(__rref);
	unsigned long credential = __ACTV_CRED_SELFCALL;
	struct sysif_actv_selfcall selfcall = {
		NULL,
		0UL,
		NULL,
		0UL,
		__rpc_info
	};
	if (__RPC_INFO_DECODE_FWD_XACT(__rpc_info) == 1) {
		credential = __ACTV_CRED_SELFCALL_XACT;
	}
	return actvhdlr_srvhandler_srvmgr_kexec((unsigned long long)(uintptr_t)&selfcall, credential);
}

#endif
#endif
