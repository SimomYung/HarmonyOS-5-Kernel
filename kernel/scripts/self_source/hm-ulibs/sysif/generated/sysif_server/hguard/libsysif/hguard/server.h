/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for hguard
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../kernel/hongmeng/hm-verif-kernel/ulibs/include/libsysif/hguard/api.sysif -I ../kernel/hongmeng/hm-verif-kernel/ulibs/include --api-dest ../image/usr/include/generated/sysif_client/hguard/libsysif/hguard/api.h --server-dest ../image/usr/include/generated/sysif_server/hguard/libsysif/hguard/server.h --dump-callno ../image/usr/include/generated/dump_info/hguard/libsysif/hguard/l2h_map.h --legacy-mode
* Create: Fri Oct 31 10:38:30 2025
*/
#ifndef LIBSYSIF_HGUARD_SERVER_H
#define LIBSYSIF_HGUARD_SERVER_H
#ifndef LIBSYSIF_HGUARD_COMMON_H
#define LIBSYSIF_HGUARD_COMMON_H
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

#define HGUARD_SERVER_PATH		"hguard-actv"


#include <unistd.h>

struct hguard_config_arg {
	int id;
	char *buf;
	int size;
};



#include <unistd.h>
#include <libbunch/bunch_ipc.h>

#define HGUARD_SERVICE_NAME_MAX 64U /* including ending NULL */
#define HGUARD_REBOOT_FLAG_SAFE   0
#define HGUARD_REBOOT_FLAG_FORCE  1

enum hguard_notify_type {
	HGUARD_NOTIFY_HEARTBEAT,
	HGUARD_NOTIFY_READY,
	HGUARD_NOTIFY_INVALID,
};

enum hguard_action_type {
	HGUARD_ACTION_START,
	HGUARD_ACTION_STOP,
	HGUARD_ACTION_RESTART,
	HGUARD_ACTION_RELOAD,
	HGUARD_ACTION_STATUS,
	HGUARD_ACTION_DUMP_STARTUP_TIME,
	HGUARD_ACTION_DAEMON_RELOAD,
	HGUARD_ACTION_REBOOT,
	HGUARD_ACTION_REBOOT_FORCE,
	HGUARD_ACTION_ALL_SOCKET_SERVICE_NAMES,
	HGUARD_ACTION_SERVICE_STATE,
	HGUARD_ACTION_SERVICE_MEMORY_CURRENT,
	HGUARD_ACTION_DELIVER_START,
	HGUARD_ACTION_MAX,
};

struct hguard_request {
	enum hguard_action_type action;
	char name[HGUARD_SERVICE_NAME_MAX];
	struct {
		int size;
		void *ptr;
	} buf;
};

enum hguard_subscribe_action_type {
	HGUARD_SUBSCRIBE_RECORD,
	HGUARD_SUBSCRIBE_REGISTER,
	HGUARD_SUBSCRIBE_FETCHALL,
	HGUARD_SUBSCRIBE_CLEAR,
};

struct hguard_subscribe_request {
	enum hguard_subscribe_action_type action;
	union {
		int pid;
		int signum;
		struct {
			int size;
			void *ptr;
		} buf;
	} arg;
};


raw_static_assert(sizeof(struct hguard_config_arg) <= 96,
                    hgcall_procfs_get_hungtask_arg_args_is_too_large);
raw_static_assert(sizeof(struct hguard_config_arg) <= 96,
                    hgcall_procfs_set_hungtask_arg_args_is_too_large);
raw_static_assert(sizeof(pid_t) <= 96,
                    hgcall_service_process_exit_arg_pid_is_too_large);
raw_static_assert(sizeof(struct hguard_request) <= 96,
                    hgcall_service_start_arg_arg_is_too_large);
raw_static_assert(sizeof(struct bunch_ipc_attr) <= 96,
                    hgcall_service_start_arg_attr_is_too_large);
raw_static_assert(sizeof(struct hguard_subscribe_request) <= 96,
                    hgcall_service_subscribe_arg_req_is_too_large);
struct __actvret_hgcall_procfs_get_config {
};

struct __actvret_hgcall_procfs_set_config {
};

struct __actvret_hgcall_procfs_get_hungtask {
};

struct __actvret_hgcall_procfs_set_hungtask {
};

struct __actvret_hgcall_procfs_stop_watchdog_for_reboot {
};

struct __actvret_hgcall_service_notify {
};

struct __actvret_hgcall_service_process_exit {
};

struct __actvret_hgcall_service_start {
};

struct __actvret_hgcall_service_subscribe {
};

struct __actvret_hgcall_pac_cfi {
};

#ifdef __aarch64__
union __actvret_hgcall {
	struct __actvret_hgcall_procfs_get_config procfs_get_config;
	struct __actvret_hgcall_procfs_set_config procfs_set_config;
	struct __actvret_hgcall_procfs_get_hungtask procfs_get_hungtask;
	struct __actvret_hgcall_procfs_set_hungtask procfs_set_hungtask;
	struct __actvret_hgcall_procfs_stop_watchdog_for_reboot procfs_stop_watchdog_for_reboot;
	struct __actvret_hgcall_service_notify service_notify;
	struct __actvret_hgcall_service_process_exit service_process_exit;
	struct __actvret_hgcall_service_start service_start;
	struct __actvret_hgcall_service_subscribe service_subscribe;
	struct __actvret_hgcall_pac_cfi pac_cfi;
};

#elif defined(__arm__)
union __actvret_hgcall {
	struct __actvret_hgcall_procfs_get_config procfs_get_config;
	struct __actvret_hgcall_procfs_set_config procfs_set_config;
	struct __actvret_hgcall_procfs_get_hungtask procfs_get_hungtask;
	struct __actvret_hgcall_procfs_set_hungtask procfs_set_hungtask;
	struct __actvret_hgcall_procfs_stop_watchdog_for_reboot procfs_stop_watchdog_for_reboot;
	struct __actvret_hgcall_service_notify service_notify;
	struct __actvret_hgcall_service_process_exit service_process_exit;
	struct __actvret_hgcall_service_start service_start;
	struct __actvret_hgcall_service_subscribe service_subscribe;
	struct __actvret_hgcall_pac_cfi pac_cfi;
};

#else
union __actvret_hgcall {
	struct __actvret_hgcall_procfs_get_config procfs_get_config;
	struct __actvret_hgcall_procfs_set_config procfs_set_config;
	struct __actvret_hgcall_procfs_get_hungtask procfs_get_hungtask;
	struct __actvret_hgcall_procfs_set_hungtask procfs_set_hungtask;
	struct __actvret_hgcall_procfs_stop_watchdog_for_reboot procfs_stop_watchdog_for_reboot;
	struct __actvret_hgcall_service_notify service_notify;
	struct __actvret_hgcall_service_process_exit service_process_exit;
	struct __actvret_hgcall_service_start service_start;
	struct __actvret_hgcall_service_subscribe service_subscribe;
	struct __actvret_hgcall_pac_cfi pac_cfi;
};

#endif
enum __hguard_mgr {
	__hguard_INVAL_mgr=0,
	__hguard_procfs_mgr,
	__hguard_service_mgr,
	__hguard_MAX_mgr,
};

#define __hguard_method_MIN __ACTV_HIGHTABLE_START
#define __hguard_method_procfs_get_config (__ACTV_HIGHTABLE_START + 1)
#define __hguard_method_procfs_set_config (__ACTV_HIGHTABLE_START + 2)
#define __hguard_method_procfs_get_hungtask (__ACTV_HIGHTABLE_START + 3)
#define __hguard_method_procfs_set_hungtask (__ACTV_HIGHTABLE_START + 4)
#define __hguard_method_procfs_stop_watchdog_for_reboot (__ACTV_HIGHTABLE_START + 5)
#define __hguard_method_service_notify (__ACTV_HIGHTABLE_START + 6)
#define __hguard_method_service_process_exit (__ACTV_HIGHTABLE_START + 7)
#define __hguard_method_service_start (__ACTV_HIGHTABLE_START + 8)
#define __hguard_method_service_subscribe (__ACTV_HIGHTABLE_START + 9)
#define __hguard_method_pac_cfi (__ACTV_HIGHTABLE_START + 10)
#define __hguard_method_MAX (__ACTV_HIGHTABLE_START + 11)
raw_static_assert(sizeof(struct __actvret_hgcall_procfs_get_config) < 512,
            hgcall_procfs_get_config_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hgcall_procfs_set_config) < 512,
            hgcall_procfs_set_config_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hgcall_procfs_get_hungtask) < 512,
            hgcall_procfs_get_hungtask_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hgcall_procfs_set_hungtask) < 512,
            hgcall_procfs_set_hungtask_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hgcall_procfs_stop_watchdog_for_reboot) < 512,
            hgcall_procfs_stop_watchdog_for_reboot_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hgcall_service_notify) < 512,
            hgcall_service_notify_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hgcall_service_process_exit) < 512,
            hgcall_service_process_exit_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hgcall_service_start) < 512,
            hgcall_service_start_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hgcall_service_subscribe) < 512,
            hgcall_service_subscribe_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hgcall_pac_cfi) < 512,
            hgcall_pac_cfi_too_many_actvret_data);
raw_static_assert(sizeof(union __actvret_hgcall) < 512,
        hgcall_too_many_actvret_data);
extern int __actvcall_hgcall_procfs_get_config(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_hgcall_procfs_get_config(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_hgcall_procfs_get_config(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hguard_method_procfs_get_config;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hgcall_procfs_get_config(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hgcall_procfs_get_config(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hguard_method_procfs_get_config;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hgcall_procfs_get_config(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hgcall_procfs_get_config(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_hgcall_procfs_get_config(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_hgcall_procfs_get_config(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_hgcall_procfs_get_config(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_hgcall_procfs_set_config(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_hgcall_procfs_set_config(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_hgcall_procfs_set_config(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hguard_method_procfs_set_config;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hgcall_procfs_set_config(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hgcall_procfs_set_config(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hguard_method_procfs_set_config;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hgcall_procfs_set_config(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hgcall_procfs_set_config(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_hgcall_procfs_set_config(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_hgcall_procfs_set_config(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_hgcall_procfs_set_config(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_hgcall_procfs_get_hungtask(unsigned long long __rpc_info, void *__buf, struct hguard_config_arg args);


extern int __actvcapcall_hgcall_procfs_get_hungtask(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hguard_config_arg args);


static inline int __attribute__((always_inline)) actvxactcall_hgcall_procfs_get_hungtask(bool is_xact_fwd, bool is_xact_allow_refwd, struct hguard_config_arg args)
{
	const unsigned long __callno = __hguard_method_procfs_get_hungtask;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hgcall_procfs_get_hungtask(rpc_info, NULL, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hgcall_procfs_get_hungtask(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hguard_config_arg args)
{
	const unsigned long __callno = __hguard_method_procfs_get_hungtask;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hgcall_procfs_get_hungtask(__rref, rpc_info, NULL, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hgcall_procfs_get_hungtask(struct hguard_config_arg args)
{
	return actvxactcall_hgcall_procfs_get_hungtask(false, false, args);
}

static inline int __attribute__((always_inline)) actvcapcall_hgcall_procfs_get_hungtask(rref_t __rref, struct hguard_config_arg args)
{
	return actvxactcapcall_hgcall_procfs_get_hungtask(false, false, __rref, args);
}

extern int __actvcall_hgcall_procfs_set_hungtask(unsigned long long __rpc_info, void *__buf, struct hguard_config_arg args);


extern int __actvcapcall_hgcall_procfs_set_hungtask(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hguard_config_arg args);


static inline int __attribute__((always_inline)) actvxactcall_hgcall_procfs_set_hungtask(bool is_xact_fwd, bool is_xact_allow_refwd, struct hguard_config_arg args)
{
	const unsigned long __callno = __hguard_method_procfs_set_hungtask;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hgcall_procfs_set_hungtask(rpc_info, NULL, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hgcall_procfs_set_hungtask(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hguard_config_arg args)
{
	const unsigned long __callno = __hguard_method_procfs_set_hungtask;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hgcall_procfs_set_hungtask(__rref, rpc_info, NULL, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hgcall_procfs_set_hungtask(struct hguard_config_arg args)
{
	return actvxactcall_hgcall_procfs_set_hungtask(false, false, args);
}

static inline int __attribute__((always_inline)) actvcapcall_hgcall_procfs_set_hungtask(rref_t __rref, struct hguard_config_arg args)
{
	return actvxactcapcall_hgcall_procfs_set_hungtask(false, false, __rref, args);
}

extern int __actvcall_hgcall_procfs_stop_watchdog_for_reboot(unsigned long long __rpc_info, void *__buf, unsigned int timeout_sec);


extern int __actvcapcall_hgcall_procfs_stop_watchdog_for_reboot(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int timeout_sec);


static inline int __attribute__((always_inline)) actvxactcall_hgcall_procfs_stop_watchdog_for_reboot(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int timeout_sec)
{
	const unsigned long __callno = __hguard_method_procfs_stop_watchdog_for_reboot;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hgcall_procfs_stop_watchdog_for_reboot(rpc_info, NULL, timeout_sec);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hgcall_procfs_stop_watchdog_for_reboot(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int timeout_sec)
{
	const unsigned long __callno = __hguard_method_procfs_stop_watchdog_for_reboot;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hgcall_procfs_stop_watchdog_for_reboot(__rref, rpc_info, NULL, timeout_sec);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hgcall_procfs_stop_watchdog_for_reboot(unsigned int timeout_sec)
{
	return actvxactcall_hgcall_procfs_stop_watchdog_for_reboot(false, false, timeout_sec);
}

static inline int __attribute__((always_inline)) actvcapcall_hgcall_procfs_stop_watchdog_for_reboot(rref_t __rref, unsigned int timeout_sec)
{
	return actvxactcapcall_hgcall_procfs_stop_watchdog_for_reboot(false, false, __rref, timeout_sec);
}

extern int __actvcall_hgcall_service_notify(unsigned long long __rpc_info, void *__buf, unsigned int type);


extern int __actvcapcall_hgcall_service_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int type);


static inline int __attribute__((always_inline)) actvxactcall_hgcall_service_notify(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int type)
{
	const unsigned long __callno = __hguard_method_service_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hgcall_service_notify(rpc_info, NULL, type);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hgcall_service_notify(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int type)
{
	const unsigned long __callno = __hguard_method_service_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hgcall_service_notify(__rref, rpc_info, NULL, type);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hgcall_service_notify(unsigned int type)
{
	return actvxactcall_hgcall_service_notify(false, false, type);
}

static inline int __attribute__((always_inline)) actvcapcall_hgcall_service_notify(rref_t __rref, unsigned int type)
{
	return actvxactcapcall_hgcall_service_notify(false, false, __rref, type);
}

extern int __actvcall_hgcall_service_process_exit(unsigned long long __rpc_info, void *__buf, pid_t pid, int status);


extern int __actvcapcall_hgcall_service_process_exit(rref_t __rref, unsigned long long __rpc_info, void *__buf, pid_t pid, int status);


static inline int __attribute__((always_inline)) actvxactcall_hgcall_service_process_exit(bool is_xact_fwd, bool is_xact_allow_refwd, pid_t pid, int status)
{
	const unsigned long __callno = __hguard_method_service_process_exit;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hgcall_service_process_exit(rpc_info, NULL, pid, status);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hgcall_service_process_exit(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, pid_t pid, int status)
{
	const unsigned long __callno = __hguard_method_service_process_exit;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hgcall_service_process_exit(__rref, rpc_info, NULL, pid, status);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hgcall_service_process_exit(pid_t pid, int status)
{
	return actvxactcall_hgcall_service_process_exit(false, false, pid, status);
}

static inline int __attribute__((always_inline)) actvcapcall_hgcall_service_process_exit(rref_t __rref, pid_t pid, int status)
{
	return actvxactcapcall_hgcall_service_process_exit(false, false, __rref, pid, status);
}

extern int __actvcall_hgcall_service_start(unsigned long long __rpc_info, void *__buf, struct hguard_request arg, struct bunch_ipc_attr attr);


extern int __actvcapcall_hgcall_service_start(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hguard_request arg, struct bunch_ipc_attr attr);


static inline int __attribute__((always_inline)) actvxactcall_hgcall_service_start(bool is_xact_fwd, bool is_xact_allow_refwd, struct hguard_request arg, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __hguard_method_service_start;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_request), __arch_actv_rpc_stksz_alignment(struct hguard_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hgcall_service_start(rpc_info, NULL, arg, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hgcall_service_start(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hguard_request arg, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __hguard_method_service_start;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_request), __arch_actv_rpc_stksz_alignment(struct hguard_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hgcall_service_start(__rref, rpc_info, NULL, arg, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hgcall_service_start(struct hguard_request arg, struct bunch_ipc_attr attr)
{
	return actvxactcall_hgcall_service_start(false, false, arg, attr);
}

static inline int __attribute__((always_inline)) actvcapcall_hgcall_service_start(rref_t __rref, struct hguard_request arg, struct bunch_ipc_attr attr)
{
	return actvxactcapcall_hgcall_service_start(false, false, __rref, arg, attr);
}

extern int __actvcall_hgcall_service_subscribe(unsigned long long __rpc_info, void *__buf, struct hguard_subscribe_request req);


extern int __actvcapcall_hgcall_service_subscribe(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hguard_subscribe_request req);


static inline int __attribute__((always_inline)) actvxactcall_hgcall_service_subscribe(bool is_xact_fwd, bool is_xact_allow_refwd, struct hguard_subscribe_request req)
{
	const unsigned long __callno = __hguard_method_service_subscribe;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_subscribe_request), __arch_actv_rpc_stksz_alignment(struct hguard_subscribe_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hgcall_service_subscribe(rpc_info, NULL, req);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hgcall_service_subscribe(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hguard_subscribe_request req)
{
	const unsigned long __callno = __hguard_method_service_subscribe;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_subscribe_request), __arch_actv_rpc_stksz_alignment(struct hguard_subscribe_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hgcall_service_subscribe(__rref, rpc_info, NULL, req);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hgcall_service_subscribe(struct hguard_subscribe_request req)
{
	return actvxactcall_hgcall_service_subscribe(false, false, req);
}

static inline int __attribute__((always_inline)) actvcapcall_hgcall_service_subscribe(rref_t __rref, struct hguard_subscribe_request req)
{
	return actvxactcapcall_hgcall_service_subscribe(false, false, __rref, req);
}

extern int __actvcall_hgcall_pac_cfi(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_hgcall_pac_cfi(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_hgcall_pac_cfi(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __hguard_method_pac_cfi;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hgcall_pac_cfi(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hgcall_pac_cfi(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __hguard_method_pac_cfi;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hgcall_pac_cfi(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hgcall_pac_cfi(void)
{
	return actvxactcall_hgcall_pac_cfi(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_hgcall_pac_cfi(rref_t __rref)
{
	return actvxactcapcall_hgcall_pac_cfi(false, false, __rref);
}

#endif
#include <libsysif/utils.h>
#include <hmasm/actv_rpc.h>
#include <hmasm/types.h>
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
extern int actvhdlr_hghandler_procfs_get_config(unsigned long long sender, unsigned long credential);


extern int actvhdlr_hghandler_procfs_set_config(unsigned long long sender, unsigned long credential);


extern int actvhdlr_hghandler_procfs_get_hungtask(unsigned long long sender, unsigned long credential, struct hguard_config_arg args);


extern int actvhdlr_hghandler_procfs_set_hungtask(unsigned long long sender, unsigned long credential, struct hguard_config_arg args);


extern int actvhdlr_hghandler_procfs_stop_watchdog_for_reboot(unsigned long long sender, unsigned long credential, unsigned int timeout_sec);


extern int actvhdlr_hghandler_service_notify(unsigned long long sender, unsigned long credential, unsigned int type);


extern int actvhdlr_hghandler_service_process_exit(unsigned long long sender, unsigned long credential, pid_t pid, int status);


extern int actvhdlr_hghandler_service_start(unsigned long long sender, unsigned long credential, struct hguard_request arg, struct bunch_ipc_attr attr);


extern int actvhdlr_hghandler_service_subscribe(unsigned long long sender, unsigned long credential, struct hguard_subscribe_request req);


extern int actvhdlr_hghandler_pac_cfi(unsigned long long sender, unsigned long credential);


#else /* aarch64 */
extern int actvhdlr_hghandler_procfs_get_config(unsigned long long sender, unsigned long credential);


extern int actvhdlr_hghandler_procfs_set_config(unsigned long long sender, unsigned long credential);


extern int actvhdlr_hghandler_procfs_get_hungtask(unsigned long long sender, unsigned long credential, struct hguard_config_arg args);


extern int actvhdlr_hghandler_procfs_set_hungtask(unsigned long long sender, unsigned long credential, struct hguard_config_arg args);


extern int actvhdlr_hghandler_procfs_stop_watchdog_for_reboot(unsigned long long sender, unsigned long credential, unsigned int timeout_sec);


extern int __actvshadowhdlr_hghandler_procfs_get_config(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hghandler_procfs_set_config(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hghandler_procfs_get_hungtask(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hghandler_procfs_set_hungtask(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hghandler_procfs_stop_watchdog_for_reboot(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_hghandler_service_notify(unsigned long long sender, unsigned long credential, unsigned int type);


extern int actvhdlr_hghandler_service_process_exit(unsigned long long sender, unsigned long credential, pid_t pid, int status);


extern int actvhdlr_hghandler_service_start(unsigned long long sender, unsigned long credential, struct hguard_request arg, struct bunch_ipc_attr attr);


extern int actvhdlr_hghandler_service_subscribe(unsigned long long sender, unsigned long credential, struct hguard_subscribe_request req);


extern int actvhdlr_hghandler_pac_cfi(unsigned long long sender, unsigned long credential);


extern int __actvshadowhdlr_hghandler_service_notify(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hghandler_service_process_exit(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hghandler_service_start(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hghandler_service_subscribe(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hghandler_pac_cfi(unsigned long long sender, unsigned long credential, ...);


#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
static inline struct __actvret_hgcall_procfs_get_config* actvhdlr_hghandler_procfs_get_config_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hgcall_procfs_get_config));
	return (struct __actvret_hgcall_procfs_get_config*)__ret;
}

static inline struct __actvret_hgcall_procfs_set_config* actvhdlr_hghandler_procfs_set_config_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hgcall_procfs_set_config));
	return (struct __actvret_hgcall_procfs_set_config*)__ret;
}

static inline struct __actvret_hgcall_procfs_get_hungtask* actvhdlr_hghandler_procfs_get_hungtask_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hgcall_procfs_get_hungtask));
	return (struct __actvret_hgcall_procfs_get_hungtask*)__ret;
}

static inline struct __actvret_hgcall_procfs_set_hungtask* actvhdlr_hghandler_procfs_set_hungtask_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hgcall_procfs_set_hungtask));
	return (struct __actvret_hgcall_procfs_set_hungtask*)__ret;
}

static inline struct __actvret_hgcall_procfs_stop_watchdog_for_reboot* actvhdlr_hghandler_procfs_stop_watchdog_for_reboot_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hgcall_procfs_stop_watchdog_for_reboot));
	return (struct __actvret_hgcall_procfs_stop_watchdog_for_reboot*)__ret;
}

static inline struct __actvret_hgcall_service_notify* actvhdlr_hghandler_service_notify_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hgcall_service_notify));
	return (struct __actvret_hgcall_service_notify*)__ret;
}

static inline struct __actvret_hgcall_service_process_exit* actvhdlr_hghandler_service_process_exit_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hgcall_service_process_exit));
	return (struct __actvret_hgcall_service_process_exit*)__ret;
}

static inline struct __actvret_hgcall_service_start* actvhdlr_hghandler_service_start_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hgcall_service_start));
	return (struct __actvret_hgcall_service_start*)__ret;
}

static inline struct __actvret_hgcall_service_subscribe* actvhdlr_hghandler_service_subscribe_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hgcall_service_subscribe));
	return (struct __actvret_hgcall_service_subscribe*)__ret;
}

static inline struct __actvret_hgcall_pac_cfi* actvhdlr_hghandler_pac_cfi_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hgcall_pac_cfi));
	return (struct __actvret_hgcall_pac_cfi*)__ret;
}

#ifdef __sysif_server_hguard_no_compat32_handlers__
#ifdef __HOST_LLT__
union hguard_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_hghandler_procfs_get_config)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_get_config)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_hghandler_procfs_set_config)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_set_config)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_hghandler_procfs_get_hungtask)(unsigned long long sender, unsigned long credential, struct hguard_config_arg args);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_get_hungtask)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args);
	int (*p__actvbridgehdlr_hghandler_procfs_set_hungtask)(unsigned long long sender, unsigned long credential, struct hguard_config_arg args);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_set_hungtask)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args);
	int (*p__actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot)(unsigned long long sender, unsigned long credential, unsigned int timeout_sec);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int timeout_sec);
	int (*p__actvbridgehdlr_hghandler_service_notify)(unsigned long long sender, unsigned long credential, unsigned int type);
	int (*p__actvbridgecapcallhdlr_hghandler_service_notify)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type);
	int (*p__actvbridgehdlr_hghandler_service_process_exit)(unsigned long long sender, unsigned long credential, pid_t pid, int status);
	int (*p__actvbridgecapcallhdlr_hghandler_service_process_exit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid, int status);
	int (*p__actvbridgehdlr_hghandler_service_start)(unsigned long long sender, unsigned long credential, struct hguard_request arg, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hghandler_service_start)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_request arg, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hghandler_service_subscribe)(unsigned long long sender, unsigned long credential, struct hguard_subscribe_request req);
	int (*p__actvbridgecapcallhdlr_hghandler_service_subscribe)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_subscribe_request req);
	int (*p__actvbridgehdlr_hghandler_pac_cfi)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_hghandler_pac_cfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union hguard_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_hghandler_procfs_get_config)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_get_config)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_hghandler_procfs_set_config)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_set_config)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_hghandler_procfs_get_hungtask)(unsigned long long sender, unsigned long credential, struct hguard_config_arg args);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_get_hungtask)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args);
	int (*p__actvbridgehdlr_hghandler_procfs_set_hungtask)(unsigned long long sender, unsigned long credential, struct hguard_config_arg args);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_set_hungtask)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args);
	int (*p__actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot)(unsigned long long sender, unsigned long credential, unsigned int timeout_sec);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int timeout_sec);
	int (*p__actvbridgehdlr_hghandler_service_notify)(unsigned long long sender, unsigned long credential, unsigned int type);
	int (*p__actvbridgecapcallhdlr_hghandler_service_notify)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type);
	int (*p__actvbridgehdlr_hghandler_service_process_exit)(unsigned long long sender, unsigned long credential, pid_t pid, int status);
	int (*p__actvbridgecapcallhdlr_hghandler_service_process_exit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid, int status);
	int (*p__actvbridgehdlr_hghandler_service_start)(unsigned long long sender, unsigned long credential, struct hguard_request arg, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hghandler_service_start)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_request arg, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hghandler_service_subscribe)(unsigned long long sender, unsigned long credential, struct hguard_subscribe_request req);
	int (*p__actvbridgecapcallhdlr_hghandler_service_subscribe)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_subscribe_request req);
	int (*p__actvbridgehdlr_hghandler_pac_cfi)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_hghandler_pac_cfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#ifdef __arm__
union hguard_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_hghandler_procfs_get_config)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_get_config)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_hghandler_procfs_set_config)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_set_config)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_hghandler_procfs_get_hungtask)(unsigned long long sender, unsigned long credential, struct hguard_config_arg args);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_get_hungtask)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args);
	int (*p__actvbridgehdlr_hghandler_procfs_set_hungtask)(unsigned long long sender, unsigned long credential, struct hguard_config_arg args);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_set_hungtask)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args);
	int (*p__actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot)(unsigned long long sender, unsigned long credential, unsigned int timeout_sec);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int timeout_sec);
	int (*p__actvbridgehdlr_hghandler_service_notify)(unsigned long long sender, unsigned long credential, unsigned int type);
	int (*p__actvbridgecapcallhdlr_hghandler_service_notify)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type);
	int (*p__actvbridgehdlr_hghandler_service_process_exit)(unsigned long long sender, unsigned long credential, pid_t pid, int status);
	int (*p__actvbridgecapcallhdlr_hghandler_service_process_exit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid, int status);
	int (*p__actvbridgehdlr_hghandler_service_start)(unsigned long long sender, unsigned long credential, struct hguard_request arg, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hghandler_service_start)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_request arg, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hghandler_service_subscribe)(unsigned long long sender, unsigned long credential, struct hguard_subscribe_request req);
	int (*p__actvbridgecapcallhdlr_hghandler_service_subscribe)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_subscribe_request req);
	int (*p__actvbridgehdlr_hghandler_pac_cfi)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_hghandler_pac_cfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential);
};

#endif /* arm */
#endif /* not HOST_LLT */
#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif /* not __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#ifdef __HOST_LLT__
union hguard_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_hghandler_procfs_get_config)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_get_config)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_hghandler_procfs_set_config)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_set_config)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_hghandler_procfs_get_hungtask)(unsigned long long sender, unsigned long credential, struct hguard_config_arg args);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_get_hungtask)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args);
	int (*p__actvbridgehdlr_hghandler_procfs_set_hungtask)(unsigned long long sender, unsigned long credential, struct hguard_config_arg args);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_set_hungtask)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args);
	int (*p__actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot)(unsigned long long sender, unsigned long credential, unsigned int timeout_sec);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int timeout_sec);
	int (*p__actvbridgehdlr_hghandler_service_notify)(unsigned long long sender, unsigned long credential, unsigned int type);
	int (*p__actvbridgecapcallhdlr_hghandler_service_notify)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type);
	int (*p__actvbridgehdlr_hghandler_service_process_exit)(unsigned long long sender, unsigned long credential, pid_t pid, int status);
	int (*p__actvbridgecapcallhdlr_hghandler_service_process_exit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid, int status);
	int (*p__actvbridgehdlr_hghandler_service_start)(unsigned long long sender, unsigned long credential, struct hguard_request arg, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hghandler_service_start)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_request arg, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hghandler_service_subscribe)(unsigned long long sender, unsigned long credential, struct hguard_subscribe_request req);
	int (*p__actvbridgecapcallhdlr_hghandler_service_subscribe)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_subscribe_request req);
	int (*p__actvbridgehdlr_hghandler_pac_cfi)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_hghandler_pac_cfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_compat_hghandler_procfs_get_config)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_procfs_get_config)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_procfs_set_config)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_procfs_set_config)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_procfs_get_hungtask)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_procfs_get_hungtask)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_procfs_set_hungtask)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_procfs_set_hungtask)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_procfs_stop_watchdog_for_reboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_procfs_stop_watchdog_for_reboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_service_notify)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_service_notify)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_service_process_exit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_service_process_exit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_service_start)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_service_start)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_service_subscribe)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_service_subscribe)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_pac_cfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_pac_cfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union hguard_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_hghandler_procfs_get_config)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_get_config)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_hghandler_procfs_set_config)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_set_config)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_hghandler_procfs_get_hungtask)(unsigned long long sender, unsigned long credential, struct hguard_config_arg args);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_get_hungtask)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args);
	int (*p__actvbridgehdlr_hghandler_procfs_set_hungtask)(unsigned long long sender, unsigned long credential, struct hguard_config_arg args);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_set_hungtask)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args);
	int (*p__actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot)(unsigned long long sender, unsigned long credential, unsigned int timeout_sec);
	int (*p__actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int timeout_sec);
	int (*p__actvbridgehdlr_hghandler_service_notify)(unsigned long long sender, unsigned long credential, unsigned int type);
	int (*p__actvbridgecapcallhdlr_hghandler_service_notify)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type);
	int (*p__actvbridgehdlr_hghandler_service_process_exit)(unsigned long long sender, unsigned long credential, pid_t pid, int status);
	int (*p__actvbridgecapcallhdlr_hghandler_service_process_exit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid, int status);
	int (*p__actvbridgehdlr_hghandler_service_start)(unsigned long long sender, unsigned long credential, struct hguard_request arg, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hghandler_service_start)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_request arg, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hghandler_service_subscribe)(unsigned long long sender, unsigned long credential, struct hguard_subscribe_request req);
	int (*p__actvbridgecapcallhdlr_hghandler_service_subscribe)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_subscribe_request req);
	int (*p__actvbridgehdlr_hghandler_pac_cfi)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_hghandler_pac_cfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_compat_hghandler_procfs_get_config)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_procfs_get_config)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_procfs_set_config)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_procfs_set_config)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_procfs_get_hungtask)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_procfs_get_hungtask)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_procfs_set_hungtask)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_procfs_set_hungtask)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_procfs_stop_watchdog_for_reboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_procfs_stop_watchdog_for_reboot)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_service_notify)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_service_notify)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_service_process_exit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_service_process_exit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_service_start)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_service_start)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_service_subscribe)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_service_subscribe)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hghandler_pac_cfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hghandler_pac_cfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#endif /* not HOST_LLT */
#endif /* compat32 */

#if defined(__sysif_server_hguard_no_compat32_handlers__) && defined(__sysif_server_hguard_no_ilp32_handlers__)
#define __hguard_method_TIMES 1
#elif !defined(__sysif_server_hguard_no_compat32_handlers__) && !defined(__sysif_server_hguard_no_ilp32_handlers__)
#define __hguard_method_TIMES 3
#else
#define __hguard_method_TIMES 2
#endif
extern union hguard_hdlrs_union sysif_hguard_actvhdlr_table[__hguard_method_MAX * __hguard_method_TIMES];

extern union hguard_hdlrs_union sysif_hguard_actvcapcallhdlr_table[__hguard_method_MAX * __hguard_method_TIMES];

extern void sysif_show_hguard_methods(void);


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
static int __opt_nonoframe __actvbridgehdlr_hghandler_procfs_get_config(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_hghandler_procfs_get_config(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_procfs_get_config(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_hghandler_procfs_get_config(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_procfs_set_config(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_hghandler_procfs_set_config(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_procfs_set_config(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_hghandler_procfs_set_config(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_procfs_get_hungtask(unsigned long long sender, unsigned long credential, struct hguard_config_arg args)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hghandler_procfs_get_hungtask(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_config_arg, args));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_procfs_get_hungtask(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hghandler_procfs_get_hungtask(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_config_arg, args));
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_procfs_set_hungtask(unsigned long long sender, unsigned long credential, struct hguard_config_arg args)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hghandler_procfs_set_hungtask(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_config_arg, args));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_procfs_set_hungtask(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hghandler_procfs_set_hungtask(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_config_arg, args));
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot(unsigned long long sender, unsigned long credential, unsigned int timeout_sec)
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
	return __actvshadowhdlr_hghandler_procfs_stop_watchdog_for_reboot(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, timeout_sec));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int timeout_sec)
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
	return __actvshadowhdlr_hghandler_procfs_stop_watchdog_for_reboot(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, timeout_sec));
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_service_notify(unsigned long long sender, unsigned long credential, unsigned int type)
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
	return __actvshadowhdlr_hghandler_service_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_service_notify(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type)
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
	return __actvshadowhdlr_hghandler_service_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type));
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_service_process_exit(unsigned long long sender, unsigned long credential, pid_t pid, int status)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hghandler_service_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid), __ARCH_ACTV_RPC_RECV_ARG(int, status));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_service_process_exit(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid, int status)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hghandler_service_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid), __ARCH_ACTV_RPC_RECV_ARG(int, status));
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_service_start(unsigned long long sender, unsigned long credential, struct hguard_request arg, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_request), __arch_actv_rpc_stksz_alignment(struct hguard_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hghandler_service_start(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_request, arg), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_service_start(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_request arg, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_request), __arch_actv_rpc_stksz_alignment(struct hguard_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hghandler_service_start(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_request, arg), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_service_subscribe(unsigned long long sender, unsigned long credential, struct hguard_subscribe_request req)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_subscribe_request), __arch_actv_rpc_stksz_alignment(struct hguard_subscribe_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hghandler_service_subscribe(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_subscribe_request, req));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_service_subscribe(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_subscribe_request req)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_subscribe_request), __arch_actv_rpc_stksz_alignment(struct hguard_subscribe_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hghandler_service_subscribe(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_subscribe_request, req));
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_pac_cfi(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_hghandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_pac_cfi(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_hghandler_pac_cfi(sender, credential);
}

#ifndef __sysif_server_hguard_no_compat32_handlers__
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
static int __opt_nonoframe __actvbridgehdlr_compat_hghandler_procfs_get_config(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_hghandler_procfs_get_config(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hghandler_procfs_get_config(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_hghandler_procfs_get_config(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hghandler_procfs_set_config(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_hghandler_procfs_set_config(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hghandler_procfs_set_config(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_hghandler_procfs_set_config(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hghandler_procfs_get_hungtask(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hguard_config_arg __maybe_unused _args;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hguard_config_arg);
		type = __type_of(struct hguard_config_arg);
		__fetch_one_arg((void*)&_args, data, 0, arg_sz, model, type, struct hguard_config_arg, _args)
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
	return __actvshadowhdlr_hghandler_procfs_get_hungtask(sender, credential, _args);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hghandler_procfs_get_hungtask(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hguard_config_arg __maybe_unused _args;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hguard_config_arg);
		type = __type_of(struct hguard_config_arg);
		__fetch_one_arg((void*)&_args, data, 0, arg_sz, model, type, struct hguard_config_arg, _args)
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
	return __actvshadowhdlr_hghandler_procfs_get_hungtask(sender, credential, _args);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hghandler_procfs_set_hungtask(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hguard_config_arg __maybe_unused _args;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hguard_config_arg);
		type = __type_of(struct hguard_config_arg);
		__fetch_one_arg((void*)&_args, data, 0, arg_sz, model, type, struct hguard_config_arg, _args)
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
	return __actvshadowhdlr_hghandler_procfs_set_hungtask(sender, credential, _args);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hghandler_procfs_set_hungtask(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hguard_config_arg __maybe_unused _args;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hguard_config_arg);
		type = __type_of(struct hguard_config_arg);
		__fetch_one_arg((void*)&_args, data, 0, arg_sz, model, type, struct hguard_config_arg, _args)
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
	return __actvshadowhdlr_hghandler_procfs_set_hungtask(sender, credential, _args);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hghandler_procfs_stop_watchdog_for_reboot(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _timeout_sec;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_timeout_sec, data, 0, arg_sz, model, type, unsigned int, _timeout_sec)
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
	return __actvshadowhdlr_hghandler_procfs_stop_watchdog_for_reboot(sender, credential, _timeout_sec);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hghandler_procfs_stop_watchdog_for_reboot(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _timeout_sec;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_timeout_sec, data, 0, arg_sz, model, type, unsigned int, _timeout_sec)
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
	return __actvshadowhdlr_hghandler_procfs_stop_watchdog_for_reboot(sender, credential, _timeout_sec);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hghandler_service_notify(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _type;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_type, data, 0, arg_sz, model, type, unsigned int, _type)
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
	return __actvshadowhdlr_hghandler_service_notify(sender, credential, _type);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hghandler_service_notify(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _type;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_type, data, 0, arg_sz, model, type, unsigned int, _type)
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
	return __actvshadowhdlr_hghandler_service_notify(sender, credential, _type);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hghandler_service_process_exit(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _status;
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
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_status, data, 1, arg_sz, model, type, int, _status)
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
	return __actvshadowhdlr_hghandler_service_process_exit(sender, credential, _pid, _status);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hghandler_service_process_exit(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _status;
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
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_status, data, 1, arg_sz, model, type, int, _status)
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
	return __actvshadowhdlr_hghandler_service_process_exit(sender, credential, _pid, _status);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hghandler_service_start(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hguard_request __maybe_unused _arg;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hguard_request);
		type = __type_of(struct hguard_request);
		__fetch_one_arg((void*)&_arg, data, 0, arg_sz, model, type, struct hguard_request, _arg)
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
	return __actvshadowhdlr_hghandler_service_start(sender, credential, _arg, _attr);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hghandler_service_start(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hguard_request __maybe_unused _arg;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hguard_request);
		type = __type_of(struct hguard_request);
		__fetch_one_arg((void*)&_arg, data, 0, arg_sz, model, type, struct hguard_request, _arg)
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
	return __actvshadowhdlr_hghandler_service_start(sender, credential, _arg, _attr);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hghandler_service_subscribe(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hguard_subscribe_request __maybe_unused _req;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hguard_subscribe_request);
		type = __type_of(struct hguard_subscribe_request);
		__fetch_one_arg((void*)&_req, data, 0, arg_sz, model, type, struct hguard_subscribe_request, _req)
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
	return __actvshadowhdlr_hghandler_service_subscribe(sender, credential, _req);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hghandler_service_subscribe(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hguard_subscribe_request __maybe_unused _req;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hguard_subscribe_request);
		type = __type_of(struct hguard_subscribe_request);
		__fetch_one_arg((void*)&_req, data, 0, arg_sz, model, type, struct hguard_subscribe_request, _req)
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
	return __actvshadowhdlr_hghandler_service_subscribe(sender, credential, _req);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hghandler_pac_cfi(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_hghandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hghandler_pac_cfi(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_hghandler_pac_cfi(sender, credential);
}

#endif /* compat handlers */
#ifndef __sysif_server_hguard_no_ilp32_handlers__
static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hghandler_procfs_get_config(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_hghandler_procfs_get_config(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hghandler_procfs_get_config(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_hghandler_procfs_get_config(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hghandler_procfs_set_config(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_hghandler_procfs_set_config(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hghandler_procfs_set_config(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_hghandler_procfs_set_config(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hghandler_procfs_get_hungtask(unsigned long long sender, unsigned long credential, struct hguard_config_arg args)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hguard_config_arg, args)
	} while (0);
	return __actvshadowhdlr_hghandler_procfs_get_hungtask(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_config_arg, args));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hghandler_procfs_get_hungtask(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hguard_config_arg, args)
	} while (0);
	return __actvshadowhdlr_hghandler_procfs_get_hungtask(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_config_arg, args));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hghandler_procfs_set_hungtask(unsigned long long sender, unsigned long credential, struct hguard_config_arg args)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hguard_config_arg, args)
	} while (0);
	return __actvshadowhdlr_hghandler_procfs_set_hungtask(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_config_arg, args));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hghandler_procfs_set_hungtask(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hguard_config_arg, args)
	} while (0);
	return __actvshadowhdlr_hghandler_procfs_set_hungtask(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_config_arg, args));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hghandler_procfs_stop_watchdog_for_reboot(unsigned long long sender, unsigned long credential, unsigned int timeout_sec)
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
		__adjust_one_arg(unsigned int, timeout_sec)
	} while (0);
	return __actvshadowhdlr_hghandler_procfs_stop_watchdog_for_reboot(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, timeout_sec));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hghandler_procfs_stop_watchdog_for_reboot(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int timeout_sec)
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
		__adjust_one_arg(unsigned int, timeout_sec)
	} while (0);
	return __actvshadowhdlr_hghandler_procfs_stop_watchdog_for_reboot(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, timeout_sec));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hghandler_service_notify(unsigned long long sender, unsigned long credential, unsigned int type)
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
		__adjust_one_arg(unsigned int, type)
	} while (0);
	return __actvshadowhdlr_hghandler_service_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hghandler_service_notify(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type)
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
		__adjust_one_arg(unsigned int, type)
	} while (0);
	return __actvshadowhdlr_hghandler_service_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hghandler_service_process_exit(unsigned long long sender, unsigned long credential, pid_t pid, int status)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(pid_t, pid)
	} while (0);
	do {
		__adjust_one_arg(int, status)
	} while (0);
	return __actvshadowhdlr_hghandler_service_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid), __ARCH_ACTV_RPC_RECV_ARG(int, status));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hghandler_service_process_exit(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid, int status)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(pid_t, pid)
	} while (0);
	do {
		__adjust_one_arg(int, status)
	} while (0);
	return __actvshadowhdlr_hghandler_service_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid), __ARCH_ACTV_RPC_RECV_ARG(int, status));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hghandler_service_start(unsigned long long sender, unsigned long credential, struct hguard_request arg, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_request), __arch_actv_rpc_stksz_alignment(struct hguard_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hguard_request, arg)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_hghandler_service_start(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_request, arg), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hghandler_service_start(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_request arg, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_request), __arch_actv_rpc_stksz_alignment(struct hguard_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hguard_request, arg)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_hghandler_service_start(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_request, arg), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hghandler_service_subscribe(unsigned long long sender, unsigned long credential, struct hguard_subscribe_request req)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_subscribe_request), __arch_actv_rpc_stksz_alignment(struct hguard_subscribe_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hguard_subscribe_request, req)
	} while (0);
	return __actvshadowhdlr_hghandler_service_subscribe(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_subscribe_request, req));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hghandler_service_subscribe(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_subscribe_request req)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_subscribe_request), __arch_actv_rpc_stksz_alignment(struct hguard_subscribe_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hguard_subscribe_request, req)
	} while (0);
	return __actvshadowhdlr_hghandler_service_subscribe(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_subscribe_request, req));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hghandler_pac_cfi(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_hghandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hghandler_pac_cfi(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_hghandler_pac_cfi(sender, credential);
}

#endif
#else
static int __opt_nonoframe __actvbridgehdlr_hghandler_procfs_get_config(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_hghandler_procfs_get_config(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_procfs_get_config(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_hghandler_procfs_get_config(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_procfs_set_config(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_hghandler_procfs_set_config(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_procfs_set_config(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_hghandler_procfs_set_config(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_procfs_get_hungtask(unsigned long long sender, unsigned long credential, struct hguard_config_arg args)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hghandler_procfs_get_hungtask(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_config_arg, args));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_procfs_get_hungtask(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hghandler_procfs_get_hungtask(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_config_arg, args));
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_procfs_set_hungtask(unsigned long long sender, unsigned long credential, struct hguard_config_arg args)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hghandler_procfs_set_hungtask(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_config_arg, args));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_procfs_set_hungtask(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_config_arg args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_config_arg), __arch_actv_rpc_stksz_alignment(struct hguard_config_arg));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hghandler_procfs_set_hungtask(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_config_arg, args));
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot(unsigned long long sender, unsigned long credential, unsigned int timeout_sec)
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
	return actvhdlr_hghandler_procfs_stop_watchdog_for_reboot(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, timeout_sec));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int timeout_sec)
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
	return actvhdlr_hghandler_procfs_stop_watchdog_for_reboot(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, timeout_sec));
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_service_notify(unsigned long long sender, unsigned long credential, unsigned int type)
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
	return actvhdlr_hghandler_service_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_service_notify(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int type)
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
	return actvhdlr_hghandler_service_notify(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, type));
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_service_process_exit(unsigned long long sender, unsigned long credential, pid_t pid, int status)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hghandler_service_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid), __ARCH_ACTV_RPC_RECV_ARG(int, status));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_service_process_exit(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, pid_t pid, int status)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hghandler_service_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid), __ARCH_ACTV_RPC_RECV_ARG(int, status));
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_service_start(unsigned long long sender, unsigned long credential, struct hguard_request arg, struct bunch_ipc_attr attr)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_request), __arch_actv_rpc_stksz_alignment(struct hguard_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hghandler_service_start(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_request, arg), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_service_start(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_request arg, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_request), __arch_actv_rpc_stksz_alignment(struct hguard_request));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hghandler_service_start(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_request, arg), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_service_subscribe(unsigned long long sender, unsigned long credential, struct hguard_subscribe_request req)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_subscribe_request), __arch_actv_rpc_stksz_alignment(struct hguard_subscribe_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hghandler_service_subscribe(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_subscribe_request, req));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_service_subscribe(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hguard_subscribe_request req)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hguard_subscribe_request), __arch_actv_rpc_stksz_alignment(struct hguard_subscribe_request));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hghandler_service_subscribe(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hguard_subscribe_request, req));
}

static int __opt_nonoframe __actvbridgehdlr_hghandler_pac_cfi(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_hghandler_pac_cfi(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hghandler_pac_cfi(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_hghandler_pac_cfi(sender, credential);
}

#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#if defined(__sysif_server_hguard_no_compat32_handlers__) && defined(__sysif_server_hguard_no_ilp32_handlers__)
__ro_after_init_hdlr union hguard_hdlrs_union sysif_hguard_actvhdlr_table[__hguard_method_MAX * __hguard_method_TIMES] = {
	[0 ... __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hguard_method_procfs_get_config] = {
		.p__actvbridgehdlr_hghandler_procfs_get_config = (&__actvbridgehdlr_hghandler_procfs_get_config)
	},
	[ __hguard_method_procfs_set_config] = {
		.p__actvbridgehdlr_hghandler_procfs_set_config = (&__actvbridgehdlr_hghandler_procfs_set_config)
	},
	[ __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgehdlr_hghandler_procfs_get_hungtask = (&__actvbridgehdlr_hghandler_procfs_get_hungtask)
	},
	[ __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgehdlr_hghandler_procfs_set_hungtask = (&__actvbridgehdlr_hghandler_procfs_set_hungtask)
	},
	[ __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[ __hguard_method_service_notify] = {
		.p__actvbridgehdlr_hghandler_service_notify = (&__actvbridgehdlr_hghandler_service_notify)
	},
	[ __hguard_method_service_process_exit] = {
		.p__actvbridgehdlr_hghandler_service_process_exit = (&__actvbridgehdlr_hghandler_service_process_exit)
	},
	[ __hguard_method_service_start] = {
		.p__actvbridgehdlr_hghandler_service_start = (&__actvbridgehdlr_hghandler_service_start)
	},
	[ __hguard_method_service_subscribe] = {
		.p__actvbridgehdlr_hghandler_service_subscribe = (&__actvbridgehdlr_hghandler_service_subscribe)
	},
	[ __hguard_method_pac_cfi] = {
		.p__actvbridgehdlr_hghandler_pac_cfi = (&__actvbridgehdlr_hghandler_pac_cfi)
	}
};

__ro_after_init_hdlr union hguard_hdlrs_union sysif_hguard_actvcapcallhdlr_table[__hguard_method_MAX * __hguard_method_TIMES] = {
	[0 ... __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hguard_method_procfs_get_config] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_get_config = (&__actvbridgecapcallhdlr_hghandler_procfs_get_config)
	},
	[ __hguard_method_procfs_set_config] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_set_config = (&__actvbridgecapcallhdlr_hghandler_procfs_set_config)
	},
	[ __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_get_hungtask = (&__actvbridgecapcallhdlr_hghandler_procfs_get_hungtask)
	},
	[ __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_set_hungtask = (&__actvbridgecapcallhdlr_hghandler_procfs_set_hungtask)
	},
	[ __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[ __hguard_method_service_notify] = {
		.p__actvbridgecapcallhdlr_hghandler_service_notify = (&__actvbridgecapcallhdlr_hghandler_service_notify)
	},
	[ __hguard_method_service_process_exit] = {
		.p__actvbridgecapcallhdlr_hghandler_service_process_exit = (&__actvbridgecapcallhdlr_hghandler_service_process_exit)
	},
	[ __hguard_method_service_start] = {
		.p__actvbridgecapcallhdlr_hghandler_service_start = (&__actvbridgecapcallhdlr_hghandler_service_start)
	},
	[ __hguard_method_service_subscribe] = {
		.p__actvbridgecapcallhdlr_hghandler_service_subscribe = (&__actvbridgecapcallhdlr_hghandler_service_subscribe)
	},
	[ __hguard_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_hghandler_pac_cfi = (&__actvbridgecapcallhdlr_hghandler_pac_cfi)
	}
};

#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif
#if !defined(__sysif_server_hguard_no_compat32_handlers__) && defined(__sysif_server_hguard_no_ilp32_handlers__)
__ro_after_init_hdlr union hguard_hdlrs_union sysif_hguard_actvhdlr_table[__hguard_method_MAX * __hguard_method_TIMES] = {
	[0 ... __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hguard_method_procfs_get_config] = {
		.p__actvbridgehdlr_hghandler_procfs_get_config = (&__actvbridgehdlr_hghandler_procfs_get_config)
	},
	[ __hguard_method_procfs_set_config] = {
		.p__actvbridgehdlr_hghandler_procfs_set_config = (&__actvbridgehdlr_hghandler_procfs_set_config)
	},
	[ __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgehdlr_hghandler_procfs_get_hungtask = (&__actvbridgehdlr_hghandler_procfs_get_hungtask)
	},
	[ __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgehdlr_hghandler_procfs_set_hungtask = (&__actvbridgehdlr_hghandler_procfs_set_hungtask)
	},
	[ __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[ __hguard_method_service_notify] = {
		.p__actvbridgehdlr_hghandler_service_notify = (&__actvbridgehdlr_hghandler_service_notify)
	},
	[ __hguard_method_service_process_exit] = {
		.p__actvbridgehdlr_hghandler_service_process_exit = (&__actvbridgehdlr_hghandler_service_process_exit)
	},
	[ __hguard_method_service_start] = {
		.p__actvbridgehdlr_hghandler_service_start = (&__actvbridgehdlr_hghandler_service_start)
	},
	[ __hguard_method_service_subscribe] = {
		.p__actvbridgehdlr_hghandler_service_subscribe = (&__actvbridgehdlr_hghandler_service_subscribe)
	},
	[ __hguard_method_pac_cfi] = {
		.p__actvbridgehdlr_hghandler_pac_cfi = (&__actvbridgehdlr_hghandler_pac_cfi)
	},
	[__hguard_method_MAX ... __hguard_method_MAX + __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__hguard_method_MAX + __hguard_method_procfs_get_config] = {
		.p__actvbridgehdlr_compat_hghandler_procfs_get_config = (&__actvbridgehdlr_compat_hghandler_procfs_get_config)
	},
	[__hguard_method_MAX + __hguard_method_procfs_set_config] = {
		.p__actvbridgehdlr_compat_hghandler_procfs_set_config = (&__actvbridgehdlr_compat_hghandler_procfs_set_config)
	},
	[__hguard_method_MAX + __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgehdlr_compat_hghandler_procfs_get_hungtask = (&__actvbridgehdlr_compat_hghandler_procfs_get_hungtask)
	},
	[__hguard_method_MAX + __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgehdlr_compat_hghandler_procfs_set_hungtask = (&__actvbridgehdlr_compat_hghandler_procfs_set_hungtask)
	},
	[__hguard_method_MAX + __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgehdlr_compat_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgehdlr_compat_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[__hguard_method_MAX + __hguard_method_service_notify] = {
		.p__actvbridgehdlr_compat_hghandler_service_notify = (&__actvbridgehdlr_compat_hghandler_service_notify)
	},
	[__hguard_method_MAX + __hguard_method_service_process_exit] = {
		.p__actvbridgehdlr_compat_hghandler_service_process_exit = (&__actvbridgehdlr_compat_hghandler_service_process_exit)
	},
	[__hguard_method_MAX + __hguard_method_service_start] = {
		.p__actvbridgehdlr_compat_hghandler_service_start = (&__actvbridgehdlr_compat_hghandler_service_start)
	},
	[__hguard_method_MAX + __hguard_method_service_subscribe] = {
		.p__actvbridgehdlr_compat_hghandler_service_subscribe = (&__actvbridgehdlr_compat_hghandler_service_subscribe)
	},
	[__hguard_method_MAX + __hguard_method_pac_cfi] = {
		.p__actvbridgehdlr_compat_hghandler_pac_cfi = (&__actvbridgehdlr_compat_hghandler_pac_cfi)
	}
};

__ro_after_init_hdlr union hguard_hdlrs_union sysif_hguard_actvcapcallhdlr_table[__hguard_method_MAX * __hguard_method_TIMES] = {
	[0 ... __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hguard_method_procfs_get_config] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_get_config = (&__actvbridgecapcallhdlr_hghandler_procfs_get_config)
	},
	[ __hguard_method_procfs_set_config] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_set_config = (&__actvbridgecapcallhdlr_hghandler_procfs_set_config)
	},
	[ __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_get_hungtask = (&__actvbridgecapcallhdlr_hghandler_procfs_get_hungtask)
	},
	[ __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_set_hungtask = (&__actvbridgecapcallhdlr_hghandler_procfs_set_hungtask)
	},
	[ __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[ __hguard_method_service_notify] = {
		.p__actvbridgecapcallhdlr_hghandler_service_notify = (&__actvbridgecapcallhdlr_hghandler_service_notify)
	},
	[ __hguard_method_service_process_exit] = {
		.p__actvbridgecapcallhdlr_hghandler_service_process_exit = (&__actvbridgecapcallhdlr_hghandler_service_process_exit)
	},
	[ __hguard_method_service_start] = {
		.p__actvbridgecapcallhdlr_hghandler_service_start = (&__actvbridgecapcallhdlr_hghandler_service_start)
	},
	[ __hguard_method_service_subscribe] = {
		.p__actvbridgecapcallhdlr_hghandler_service_subscribe = (&__actvbridgecapcallhdlr_hghandler_service_subscribe)
	},
	[ __hguard_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_hghandler_pac_cfi = (&__actvbridgecapcallhdlr_hghandler_pac_cfi)
	},
	[__hguard_method_MAX ... __hguard_method_MAX + __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__hguard_method_MAX + __hguard_method_procfs_get_config] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_procfs_get_config = (&__actvbridgecapcallhdlr_compat_hghandler_procfs_get_config)
	},
	[__hguard_method_MAX + __hguard_method_procfs_set_config] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_procfs_set_config = (&__actvbridgecapcallhdlr_compat_hghandler_procfs_set_config)
	},
	[__hguard_method_MAX + __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_procfs_get_hungtask = (&__actvbridgecapcallhdlr_compat_hghandler_procfs_get_hungtask)
	},
	[__hguard_method_MAX + __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_procfs_set_hungtask = (&__actvbridgecapcallhdlr_compat_hghandler_procfs_set_hungtask)
	},
	[__hguard_method_MAX + __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgecapcallhdlr_compat_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[__hguard_method_MAX + __hguard_method_service_notify] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_service_notify = (&__actvbridgecapcallhdlr_compat_hghandler_service_notify)
	},
	[__hguard_method_MAX + __hguard_method_service_process_exit] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_service_process_exit = (&__actvbridgecapcallhdlr_compat_hghandler_service_process_exit)
	},
	[__hguard_method_MAX + __hguard_method_service_start] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_service_start = (&__actvbridgecapcallhdlr_compat_hghandler_service_start)
	},
	[__hguard_method_MAX + __hguard_method_service_subscribe] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_service_subscribe = (&__actvbridgecapcallhdlr_compat_hghandler_service_subscribe)
	},
	[__hguard_method_MAX + __hguard_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_pac_cfi = (&__actvbridgecapcallhdlr_compat_hghandler_pac_cfi)
	}
};

#endif /* aarch64 + compat */
#if defined(__sysif_server_hguard_no_compat32_handlers__) && !defined(__sysif_server_hguard_no_ilp32_handlers__)
__ro_after_init_hdlr union hguard_hdlrs_union sysif_hguard_actvhdlr_table[__hguard_method_MAX * __hguard_method_TIMES] = {
	[0 ... __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hguard_method_procfs_get_config] = {
		.p__actvbridgehdlr_hghandler_procfs_get_config = (&__actvbridgehdlr_hghandler_procfs_get_config)
	},
	[ __hguard_method_procfs_set_config] = {
		.p__actvbridgehdlr_hghandler_procfs_set_config = (&__actvbridgehdlr_hghandler_procfs_set_config)
	},
	[ __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgehdlr_hghandler_procfs_get_hungtask = (&__actvbridgehdlr_hghandler_procfs_get_hungtask)
	},
	[ __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgehdlr_hghandler_procfs_set_hungtask = (&__actvbridgehdlr_hghandler_procfs_set_hungtask)
	},
	[ __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[ __hguard_method_service_notify] = {
		.p__actvbridgehdlr_hghandler_service_notify = (&__actvbridgehdlr_hghandler_service_notify)
	},
	[ __hguard_method_service_process_exit] = {
		.p__actvbridgehdlr_hghandler_service_process_exit = (&__actvbridgehdlr_hghandler_service_process_exit)
	},
	[ __hguard_method_service_start] = {
		.p__actvbridgehdlr_hghandler_service_start = (&__actvbridgehdlr_hghandler_service_start)
	},
	[ __hguard_method_service_subscribe] = {
		.p__actvbridgehdlr_hghandler_service_subscribe = (&__actvbridgehdlr_hghandler_service_subscribe)
	},
	[ __hguard_method_pac_cfi] = {
		.p__actvbridgehdlr_hghandler_pac_cfi = (&__actvbridgehdlr_hghandler_pac_cfi)
	},
	[__hguard_method_MAX ... __hguard_method_MAX + __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__hguard_method_MAX + __hguard_method_procfs_get_config] = {
		.p__actvbridgehdlr_hghandler_procfs_get_config = (&__actvbridgehdlr_ilp32_hghandler_procfs_get_config)
	},
	[__hguard_method_MAX + __hguard_method_procfs_set_config] = {
		.p__actvbridgehdlr_hghandler_procfs_set_config = (&__actvbridgehdlr_ilp32_hghandler_procfs_set_config)
	},
	[__hguard_method_MAX + __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgehdlr_hghandler_procfs_get_hungtask = (&__actvbridgehdlr_ilp32_hghandler_procfs_get_hungtask)
	},
	[__hguard_method_MAX + __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgehdlr_hghandler_procfs_set_hungtask = (&__actvbridgehdlr_ilp32_hghandler_procfs_set_hungtask)
	},
	[__hguard_method_MAX + __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgehdlr_ilp32_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[__hguard_method_MAX + __hguard_method_service_notify] = {
		.p__actvbridgehdlr_hghandler_service_notify = (&__actvbridgehdlr_ilp32_hghandler_service_notify)
	},
	[__hguard_method_MAX + __hguard_method_service_process_exit] = {
		.p__actvbridgehdlr_hghandler_service_process_exit = (&__actvbridgehdlr_ilp32_hghandler_service_process_exit)
	},
	[__hguard_method_MAX + __hguard_method_service_start] = {
		.p__actvbridgehdlr_hghandler_service_start = (&__actvbridgehdlr_ilp32_hghandler_service_start)
	},
	[__hguard_method_MAX + __hguard_method_service_subscribe] = {
		.p__actvbridgehdlr_hghandler_service_subscribe = (&__actvbridgehdlr_ilp32_hghandler_service_subscribe)
	},
	[__hguard_method_MAX + __hguard_method_pac_cfi] = {
		.p__actvbridgehdlr_hghandler_pac_cfi = (&__actvbridgehdlr_ilp32_hghandler_pac_cfi)
	}
};

__ro_after_init_hdlr union hguard_hdlrs_union sysif_hguard_actvcapcallhdlr_table[__hguard_method_MAX * __hguard_method_TIMES] = {
	[0 ... __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hguard_method_procfs_get_config] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_get_config = (&__actvbridgecapcallhdlr_hghandler_procfs_get_config)
	},
	[ __hguard_method_procfs_set_config] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_set_config = (&__actvbridgecapcallhdlr_hghandler_procfs_set_config)
	},
	[ __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_get_hungtask = (&__actvbridgecapcallhdlr_hghandler_procfs_get_hungtask)
	},
	[ __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_set_hungtask = (&__actvbridgecapcallhdlr_hghandler_procfs_set_hungtask)
	},
	[ __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[ __hguard_method_service_notify] = {
		.p__actvbridgecapcallhdlr_hghandler_service_notify = (&__actvbridgecapcallhdlr_hghandler_service_notify)
	},
	[ __hguard_method_service_process_exit] = {
		.p__actvbridgecapcallhdlr_hghandler_service_process_exit = (&__actvbridgecapcallhdlr_hghandler_service_process_exit)
	},
	[ __hguard_method_service_start] = {
		.p__actvbridgecapcallhdlr_hghandler_service_start = (&__actvbridgecapcallhdlr_hghandler_service_start)
	},
	[ __hguard_method_service_subscribe] = {
		.p__actvbridgecapcallhdlr_hghandler_service_subscribe = (&__actvbridgecapcallhdlr_hghandler_service_subscribe)
	},
	[ __hguard_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_hghandler_pac_cfi = (&__actvbridgecapcallhdlr_hghandler_pac_cfi)
	},
	[__hguard_method_MAX ... __hguard_method_MAX + __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__hguard_method_MAX + __hguard_method_procfs_get_config] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_get_config = (&__actvbridgecapcallhdlr_ilp32_hghandler_procfs_get_config)
	},
	[__hguard_method_MAX + __hguard_method_procfs_set_config] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_set_config = (&__actvbridgecapcallhdlr_ilp32_hghandler_procfs_set_config)
	},
	[__hguard_method_MAX + __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_get_hungtask = (&__actvbridgecapcallhdlr_ilp32_hghandler_procfs_get_hungtask)
	},
	[__hguard_method_MAX + __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_set_hungtask = (&__actvbridgecapcallhdlr_ilp32_hghandler_procfs_set_hungtask)
	},
	[__hguard_method_MAX + __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgecapcallhdlr_ilp32_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[__hguard_method_MAX + __hguard_method_service_notify] = {
		.p__actvbridgecapcallhdlr_hghandler_service_notify = (&__actvbridgecapcallhdlr_ilp32_hghandler_service_notify)
	},
	[__hguard_method_MAX + __hguard_method_service_process_exit] = {
		.p__actvbridgecapcallhdlr_hghandler_service_process_exit = (&__actvbridgecapcallhdlr_ilp32_hghandler_service_process_exit)
	},
	[__hguard_method_MAX + __hguard_method_service_start] = {
		.p__actvbridgecapcallhdlr_hghandler_service_start = (&__actvbridgecapcallhdlr_ilp32_hghandler_service_start)
	},
	[__hguard_method_MAX + __hguard_method_service_subscribe] = {
		.p__actvbridgecapcallhdlr_hghandler_service_subscribe = (&__actvbridgecapcallhdlr_ilp32_hghandler_service_subscribe)
	},
	[__hguard_method_MAX + __hguard_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_hghandler_pac_cfi = (&__actvbridgecapcallhdlr_ilp32_hghandler_pac_cfi)
	}
};

#endif /* aarch64 + ilp32 */
#if !defined(__sysif_server_hguard_no_compat32_handlers__) && !defined(__sysif_server_hguard_no_ilp32_handlers__)
__ro_after_init_hdlr union hguard_hdlrs_union sysif_hguard_actvhdlr_table[__hguard_method_MAX * __hguard_method_TIMES] = {
	[0 ... __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hguard_method_procfs_get_config] = {
		.p__actvbridgehdlr_hghandler_procfs_get_config = (&__actvbridgehdlr_hghandler_procfs_get_config)
	},
	[ __hguard_method_procfs_set_config] = {
		.p__actvbridgehdlr_hghandler_procfs_set_config = (&__actvbridgehdlr_hghandler_procfs_set_config)
	},
	[ __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgehdlr_hghandler_procfs_get_hungtask = (&__actvbridgehdlr_hghandler_procfs_get_hungtask)
	},
	[ __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgehdlr_hghandler_procfs_set_hungtask = (&__actvbridgehdlr_hghandler_procfs_set_hungtask)
	},
	[ __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[ __hguard_method_service_notify] = {
		.p__actvbridgehdlr_hghandler_service_notify = (&__actvbridgehdlr_hghandler_service_notify)
	},
	[ __hguard_method_service_process_exit] = {
		.p__actvbridgehdlr_hghandler_service_process_exit = (&__actvbridgehdlr_hghandler_service_process_exit)
	},
	[ __hguard_method_service_start] = {
		.p__actvbridgehdlr_hghandler_service_start = (&__actvbridgehdlr_hghandler_service_start)
	},
	[ __hguard_method_service_subscribe] = {
		.p__actvbridgehdlr_hghandler_service_subscribe = (&__actvbridgehdlr_hghandler_service_subscribe)
	},
	[ __hguard_method_pac_cfi] = {
		.p__actvbridgehdlr_hghandler_pac_cfi = (&__actvbridgehdlr_hghandler_pac_cfi)
	},
	[__hguard_method_MAX ... __hguard_method_MAX + __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__hguard_method_MAX + __hguard_method_procfs_get_config] = {
		.p__actvbridgehdlr_compat_hghandler_procfs_get_config = (&__actvbridgehdlr_compat_hghandler_procfs_get_config)
	},
	[__hguard_method_MAX + __hguard_method_procfs_set_config] = {
		.p__actvbridgehdlr_compat_hghandler_procfs_set_config = (&__actvbridgehdlr_compat_hghandler_procfs_set_config)
	},
	[__hguard_method_MAX + __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgehdlr_compat_hghandler_procfs_get_hungtask = (&__actvbridgehdlr_compat_hghandler_procfs_get_hungtask)
	},
	[__hguard_method_MAX + __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgehdlr_compat_hghandler_procfs_set_hungtask = (&__actvbridgehdlr_compat_hghandler_procfs_set_hungtask)
	},
	[__hguard_method_MAX + __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgehdlr_compat_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgehdlr_compat_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[__hguard_method_MAX + __hguard_method_service_notify] = {
		.p__actvbridgehdlr_compat_hghandler_service_notify = (&__actvbridgehdlr_compat_hghandler_service_notify)
	},
	[__hguard_method_MAX + __hguard_method_service_process_exit] = {
		.p__actvbridgehdlr_compat_hghandler_service_process_exit = (&__actvbridgehdlr_compat_hghandler_service_process_exit)
	},
	[__hguard_method_MAX + __hguard_method_service_start] = {
		.p__actvbridgehdlr_compat_hghandler_service_start = (&__actvbridgehdlr_compat_hghandler_service_start)
	},
	[__hguard_method_MAX + __hguard_method_service_subscribe] = {
		.p__actvbridgehdlr_compat_hghandler_service_subscribe = (&__actvbridgehdlr_compat_hghandler_service_subscribe)
	},
	[__hguard_method_MAX + __hguard_method_pac_cfi] = {
		.p__actvbridgehdlr_compat_hghandler_pac_cfi = (&__actvbridgehdlr_compat_hghandler_pac_cfi)
	},
	[2 * __hguard_method_MAX ... 2 * __hguard_method_MAX + __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __hguard_method_MAX + __hguard_method_procfs_get_config] = {
		.p__actvbridgehdlr_hghandler_procfs_get_config = (&__actvbridgehdlr_ilp32_hghandler_procfs_get_config)
	},
	[2 * __hguard_method_MAX + __hguard_method_procfs_set_config] = {
		.p__actvbridgehdlr_hghandler_procfs_set_config = (&__actvbridgehdlr_ilp32_hghandler_procfs_set_config)
	},
	[2 * __hguard_method_MAX + __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgehdlr_hghandler_procfs_get_hungtask = (&__actvbridgehdlr_ilp32_hghandler_procfs_get_hungtask)
	},
	[2 * __hguard_method_MAX + __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgehdlr_hghandler_procfs_set_hungtask = (&__actvbridgehdlr_ilp32_hghandler_procfs_set_hungtask)
	},
	[2 * __hguard_method_MAX + __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgehdlr_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgehdlr_ilp32_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[2 * __hguard_method_MAX + __hguard_method_service_notify] = {
		.p__actvbridgehdlr_hghandler_service_notify = (&__actvbridgehdlr_ilp32_hghandler_service_notify)
	},
	[2 * __hguard_method_MAX + __hguard_method_service_process_exit] = {
		.p__actvbridgehdlr_hghandler_service_process_exit = (&__actvbridgehdlr_ilp32_hghandler_service_process_exit)
	},
	[2 * __hguard_method_MAX + __hguard_method_service_start] = {
		.p__actvbridgehdlr_hghandler_service_start = (&__actvbridgehdlr_ilp32_hghandler_service_start)
	},
	[2 * __hguard_method_MAX + __hguard_method_service_subscribe] = {
		.p__actvbridgehdlr_hghandler_service_subscribe = (&__actvbridgehdlr_ilp32_hghandler_service_subscribe)
	},
	[2 * __hguard_method_MAX + __hguard_method_pac_cfi] = {
		.p__actvbridgehdlr_hghandler_pac_cfi = (&__actvbridgehdlr_ilp32_hghandler_pac_cfi)
	}
};

__ro_after_init_hdlr union hguard_hdlrs_union sysif_hguard_actvcapcallhdlr_table[__hguard_method_MAX * __hguard_method_TIMES] = {
	[0 ... __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hguard_method_procfs_get_config] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_get_config = (&__actvbridgecapcallhdlr_hghandler_procfs_get_config)
	},
	[ __hguard_method_procfs_set_config] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_set_config = (&__actvbridgecapcallhdlr_hghandler_procfs_set_config)
	},
	[ __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_get_hungtask = (&__actvbridgecapcallhdlr_hghandler_procfs_get_hungtask)
	},
	[ __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_set_hungtask = (&__actvbridgecapcallhdlr_hghandler_procfs_set_hungtask)
	},
	[ __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[ __hguard_method_service_notify] = {
		.p__actvbridgecapcallhdlr_hghandler_service_notify = (&__actvbridgecapcallhdlr_hghandler_service_notify)
	},
	[ __hguard_method_service_process_exit] = {
		.p__actvbridgecapcallhdlr_hghandler_service_process_exit = (&__actvbridgecapcallhdlr_hghandler_service_process_exit)
	},
	[ __hguard_method_service_start] = {
		.p__actvbridgecapcallhdlr_hghandler_service_start = (&__actvbridgecapcallhdlr_hghandler_service_start)
	},
	[ __hguard_method_service_subscribe] = {
		.p__actvbridgecapcallhdlr_hghandler_service_subscribe = (&__actvbridgecapcallhdlr_hghandler_service_subscribe)
	},
	[ __hguard_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_hghandler_pac_cfi = (&__actvbridgecapcallhdlr_hghandler_pac_cfi)
	},
	[__hguard_method_MAX ... __hguard_method_MAX + __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__hguard_method_MAX + __hguard_method_procfs_get_config] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_procfs_get_config = (&__actvbridgecapcallhdlr_compat_hghandler_procfs_get_config)
	},
	[__hguard_method_MAX + __hguard_method_procfs_set_config] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_procfs_set_config = (&__actvbridgecapcallhdlr_compat_hghandler_procfs_set_config)
	},
	[__hguard_method_MAX + __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_procfs_get_hungtask = (&__actvbridgecapcallhdlr_compat_hghandler_procfs_get_hungtask)
	},
	[__hguard_method_MAX + __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_procfs_set_hungtask = (&__actvbridgecapcallhdlr_compat_hghandler_procfs_set_hungtask)
	},
	[__hguard_method_MAX + __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgecapcallhdlr_compat_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[__hguard_method_MAX + __hguard_method_service_notify] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_service_notify = (&__actvbridgecapcallhdlr_compat_hghandler_service_notify)
	},
	[__hguard_method_MAX + __hguard_method_service_process_exit] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_service_process_exit = (&__actvbridgecapcallhdlr_compat_hghandler_service_process_exit)
	},
	[__hguard_method_MAX + __hguard_method_service_start] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_service_start = (&__actvbridgecapcallhdlr_compat_hghandler_service_start)
	},
	[__hguard_method_MAX + __hguard_method_service_subscribe] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_service_subscribe = (&__actvbridgecapcallhdlr_compat_hghandler_service_subscribe)
	},
	[__hguard_method_MAX + __hguard_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_compat_hghandler_pac_cfi = (&__actvbridgecapcallhdlr_compat_hghandler_pac_cfi)
	},
	[2 * __hguard_method_MAX ... 2 * __hguard_method_MAX + __hguard_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __hguard_method_MAX + __hguard_method_procfs_get_config] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_get_config = (&__actvbridgecapcallhdlr_ilp32_hghandler_procfs_get_config)
	},
	[2 * __hguard_method_MAX + __hguard_method_procfs_set_config] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_set_config = (&__actvbridgecapcallhdlr_ilp32_hghandler_procfs_set_config)
	},
	[2 * __hguard_method_MAX + __hguard_method_procfs_get_hungtask] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_get_hungtask = (&__actvbridgecapcallhdlr_ilp32_hghandler_procfs_get_hungtask)
	},
	[2 * __hguard_method_MAX + __hguard_method_procfs_set_hungtask] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_set_hungtask = (&__actvbridgecapcallhdlr_ilp32_hghandler_procfs_set_hungtask)
	},
	[2 * __hguard_method_MAX + __hguard_method_procfs_stop_watchdog_for_reboot] = {
		.p__actvbridgecapcallhdlr_hghandler_procfs_stop_watchdog_for_reboot = (&__actvbridgecapcallhdlr_ilp32_hghandler_procfs_stop_watchdog_for_reboot)
	},
	[2 * __hguard_method_MAX + __hguard_method_service_notify] = {
		.p__actvbridgecapcallhdlr_hghandler_service_notify = (&__actvbridgecapcallhdlr_ilp32_hghandler_service_notify)
	},
	[2 * __hguard_method_MAX + __hguard_method_service_process_exit] = {
		.p__actvbridgecapcallhdlr_hghandler_service_process_exit = (&__actvbridgecapcallhdlr_ilp32_hghandler_service_process_exit)
	},
	[2 * __hguard_method_MAX + __hguard_method_service_start] = {
		.p__actvbridgecapcallhdlr_hghandler_service_start = (&__actvbridgecapcallhdlr_ilp32_hghandler_service_start)
	},
	[2 * __hguard_method_MAX + __hguard_method_service_subscribe] = {
		.p__actvbridgecapcallhdlr_hghandler_service_subscribe = (&__actvbridgecapcallhdlr_ilp32_hghandler_service_subscribe)
	},
	[2 * __hguard_method_MAX + __hguard_method_pac_cfi] = {
		.p__actvbridgecapcallhdlr_hghandler_pac_cfi = (&__actvbridgecapcallhdlr_ilp32_hghandler_pac_cfi)
	}
};

#endif /* aarch64 + compat + ilp32 */
#endif /* arm or aarch64 only */
#ifdef __aarch64__
void sysif_show_hguard_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "procfs", "get_config", __hguard_method_procfs_get_config);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "procfs", "get_config", __hguard_method_procfs_get_config);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "procfs", "set_config", __hguard_method_procfs_set_config);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "procfs", "set_config", __hguard_method_procfs_set_config);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "procfs", "get_hungtask", __hguard_method_procfs_get_hungtask);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "procfs", "get_hungtask", __hguard_method_procfs_get_hungtask);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "procfs", "set_hungtask", __hguard_method_procfs_set_hungtask);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "procfs", "set_hungtask", __hguard_method_procfs_set_hungtask);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "procfs", "stop_watchdog_for_reboot", __hguard_method_procfs_stop_watchdog_for_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "procfs", "stop_watchdog_for_reboot", __hguard_method_procfs_stop_watchdog_for_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "service", "notify", __hguard_method_service_notify);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "service", "notify", __hguard_method_service_notify);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "service", "process_exit", __hguard_method_service_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "service", "process_exit", __hguard_method_service_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "service", "start", __hguard_method_service_start);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "service", "start", __hguard_method_service_start);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "service", "subscribe", __hguard_method_service_subscribe);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "service", "subscribe", __hguard_method_service_subscribe);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "pac", "cfi", __hguard_method_pac_cfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "pac", "cfi", __hguard_method_pac_cfi);
	hm_info("%s_%s, max method num: %d\n", "hguard", "method", __hguard_method_MAX);
}

#elif defined(__arm__)
void sysif_show_hguard_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "procfs", "get_config", __hguard_method_procfs_get_config);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "procfs", "get_config", __hguard_method_procfs_get_config);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "procfs", "set_config", __hguard_method_procfs_set_config);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "procfs", "set_config", __hguard_method_procfs_set_config);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "procfs", "get_hungtask", __hguard_method_procfs_get_hungtask);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "procfs", "get_hungtask", __hguard_method_procfs_get_hungtask);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "procfs", "set_hungtask", __hguard_method_procfs_set_hungtask);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "procfs", "set_hungtask", __hguard_method_procfs_set_hungtask);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "procfs", "stop_watchdog_for_reboot", __hguard_method_procfs_stop_watchdog_for_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "procfs", "stop_watchdog_for_reboot", __hguard_method_procfs_stop_watchdog_for_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "service", "notify", __hguard_method_service_notify);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "service", "notify", __hguard_method_service_notify);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "service", "process_exit", __hguard_method_service_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "service", "process_exit", __hguard_method_service_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "service", "start", __hguard_method_service_start);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "service", "start", __hguard_method_service_start);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "service", "subscribe", __hguard_method_service_subscribe);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "service", "subscribe", __hguard_method_service_subscribe);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "pac", "cfi", __hguard_method_pac_cfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "pac", "cfi", __hguard_method_pac_cfi);
	hm_info("%s_%s, max method num: %d\n", "hguard", "method", __hguard_method_MAX);
}

#else
void sysif_show_hguard_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "procfs", "get_config", __hguard_method_procfs_get_config);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "procfs", "get_config", __hguard_method_procfs_get_config);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "procfs", "set_config", __hguard_method_procfs_set_config);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "procfs", "set_config", __hguard_method_procfs_set_config);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "procfs", "get_hungtask", __hguard_method_procfs_get_hungtask);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "procfs", "get_hungtask", __hguard_method_procfs_get_hungtask);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "procfs", "set_hungtask", __hguard_method_procfs_set_hungtask);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "procfs", "set_hungtask", __hguard_method_procfs_set_hungtask);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "procfs", "stop_watchdog_for_reboot", __hguard_method_procfs_stop_watchdog_for_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "procfs", "stop_watchdog_for_reboot", __hguard_method_procfs_stop_watchdog_for_reboot);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "service", "notify", __hguard_method_service_notify);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "service", "notify", __hguard_method_service_notify);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "service", "process_exit", __hguard_method_service_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "service", "process_exit", __hguard_method_service_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "service", "start", __hguard_method_service_start);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "service", "start", __hguard_method_service_start);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "service", "subscribe", __hguard_method_service_subscribe);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "service", "subscribe", __hguard_method_service_subscribe);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hgcall", "pac", "cfi", __hguard_method_pac_cfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hghandler", "pac", "cfi", __hguard_method_pac_cfi);
	hm_info("%s_%s, max method num: %d\n", "hguard", "method", __hguard_method_MAX);
}

#endif
#endif
#endif
