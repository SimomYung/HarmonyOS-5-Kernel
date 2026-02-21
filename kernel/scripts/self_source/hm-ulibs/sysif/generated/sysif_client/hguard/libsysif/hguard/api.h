/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for hguard
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../ulibs/include/libsysif/hguard/api.sysif -I ../ulibs/include --api-dest ../image/usr/include/generated/sysif_client/hguard/libsysif/hguard/api.h --server-dest ../image/usr/include/generated/sysif_server/hguard/libsysif/hguard/server.h --dump-callno ../image/usr/include/generated/dump_info/hguard/libsysif/hguard/l2h_map.h --legacy-mode
* Create: Tue Sep 16 04:31:10 2025
*/
#ifndef LIBSYSIF_HGUARD_API_H
#define LIBSYSIF_HGUARD_API_H
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
#define __hguard_method_MAX (__ACTV_HIGHTABLE_START + 10)
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

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_hgcall_procfs_get_config(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_hgcall_procfs_get_config(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hgcall_procfs_get_config(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_hgcall_procfs_get_config(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_hgcall_procfs_get_config(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_hgcall_procfs_get_config(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_hgcall_procfs_set_config(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_hgcall_procfs_set_config(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hgcall_procfs_set_config(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_hgcall_procfs_set_config(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_hgcall_procfs_set_config(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_hgcall_procfs_set_config(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hgcall_procfs_get_hungtask(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hgcall_procfs_get_hungtask(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hgcall_procfs_get_hungtask(unsigned long long __rpc_info, void *__buf, struct hguard_config_arg args)
{
	int __ret;
	struct hguard_config_arg __maybe_unused __actv_local_var_args = args;
	__ret = ____actvcall_hgcall_procfs_get_hungtask(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

int __internal_actvcall __actvcapcall_hgcall_procfs_get_hungtask(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hguard_config_arg args)
{
	int __ret;
	struct hguard_config_arg __maybe_unused __actv_local_var_args = args;
	__ret = ____actvcapcall_hgcall_procfs_get_hungtask(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hgcall_procfs_set_hungtask(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hgcall_procfs_set_hungtask(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hgcall_procfs_set_hungtask(unsigned long long __rpc_info, void *__buf, struct hguard_config_arg args)
{
	int __ret;
	struct hguard_config_arg __maybe_unused __actv_local_var_args = args;
	__ret = ____actvcall_hgcall_procfs_set_hungtask(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

int __internal_actvcall __actvcapcall_hgcall_procfs_set_hungtask(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hguard_config_arg args)
{
	int __ret;
	struct hguard_config_arg __maybe_unused __actv_local_var_args = args;
	__ret = ____actvcapcall_hgcall_procfs_set_hungtask(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hgcall_procfs_stop_watchdog_for_reboot(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hgcall_procfs_stop_watchdog_for_reboot(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hgcall_procfs_stop_watchdog_for_reboot(unsigned long long __rpc_info, void *__buf, unsigned int timeout_sec)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_timeout_sec = timeout_sec;
	__ret = ____actvcall_hgcall_procfs_stop_watchdog_for_reboot(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(timeout_sec, __actv_local_var_timeout_sec));
	return __ret;
}

int __internal_actvcall __actvcapcall_hgcall_procfs_stop_watchdog_for_reboot(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int timeout_sec)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_timeout_sec = timeout_sec;
	__ret = ____actvcapcall_hgcall_procfs_stop_watchdog_for_reboot(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(timeout_sec, __actv_local_var_timeout_sec));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hgcall_service_notify(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hgcall_service_notify(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hgcall_service_notify(unsigned long long __rpc_info, void *__buf, unsigned int type)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_type = type;
	__ret = ____actvcall_hgcall_service_notify(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type));
	return __ret;
}

int __internal_actvcall __actvcapcall_hgcall_service_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int type)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_type = type;
	__ret = ____actvcapcall_hgcall_service_notify(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hgcall_service_process_exit(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hgcall_service_process_exit(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hgcall_service_process_exit(unsigned long long __rpc_info, void *__buf, pid_t pid, int status)
{
	int __ret;
	pid_t __maybe_unused __actv_local_var_pid = pid;
	int __maybe_unused __actv_local_var_status = status;
	__ret = ____actvcall_hgcall_service_process_exit(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(pid, __actv_local_var_pid), __ARCH_ACTV_RPC_PASS_ARG(status, __actv_local_var_status));
	return __ret;
}

int __internal_actvcall __actvcapcall_hgcall_service_process_exit(rref_t __rref, unsigned long long __rpc_info, void *__buf, pid_t pid, int status)
{
	int __ret;
	pid_t __maybe_unused __actv_local_var_pid = pid;
	int __maybe_unused __actv_local_var_status = status;
	__ret = ____actvcapcall_hgcall_service_process_exit(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(pid, __actv_local_var_pid), __ARCH_ACTV_RPC_PASS_ARG(status, __actv_local_var_status));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hgcall_service_start(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hgcall_service_start(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hgcall_service_start(unsigned long long __rpc_info, void *__buf, struct hguard_request arg, struct bunch_ipc_attr attr)
{
	int __ret;
	struct hguard_request __maybe_unused __actv_local_var_arg = arg;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcall_hgcall_service_start(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

int __internal_actvcall __actvcapcall_hgcall_service_start(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hguard_request arg, struct bunch_ipc_attr attr)
{
	int __ret;
	struct hguard_request __maybe_unused __actv_local_var_arg = arg;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcapcall_hgcall_service_start(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hgcall_service_subscribe(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hgcall_service_subscribe(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hgcall_service_subscribe(unsigned long long __rpc_info, void *__buf, struct hguard_subscribe_request req)
{
	int __ret;
	struct hguard_subscribe_request __maybe_unused __actv_local_var_req = req;
	__ret = ____actvcall_hgcall_service_subscribe(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(req, __actv_local_var_req));
	return __ret;
}

int __internal_actvcall __actvcapcall_hgcall_service_subscribe(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hguard_subscribe_request req)
{
	int __ret;
	struct hguard_subscribe_request __maybe_unused __actv_local_var_req = req;
	__ret = ____actvcapcall_hgcall_service_subscribe(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(req, __actv_local_var_req));
	return __ret;
}

#endif
#endif
