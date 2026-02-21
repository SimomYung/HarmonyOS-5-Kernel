/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for eventcb
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../ulibs/include/libsysif/eventcb/api.sysif -I ../ulibs/include --api-dest ../image/usr/include/generated/sysif_client/eventcb/libsysif/eventcb/api.h --server-dest ../image/usr/include/generated/sysif_server/eventcb/libsysif/eventcb/server.h --dump-callno ../image/usr/include/generated/dump_info/eventcb/libsysif/eventcb/l2h_map.h --legacy-mode --no-selfcall
* Create: Tue Sep 16 14:00:36 2025
*/
#ifndef LIBSYSIF_EVENTCB_SERVER_H
#define LIBSYSIF_EVENTCB_SERVER_H
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
#include <libsysif/utils.h>
#include <hmasm/actv_rpc.h>
#include <hmasm/types.h>
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
extern int actvhdlr_eventcbhandler_eventcb_thread_msg_ctrl(unsigned long long sender, unsigned long credential, struct evtcb_thread_msg_s msg);


extern int actvhdlr_eventcbhandler_eventcb_process_exit(unsigned long long sender, unsigned long credential, unsigned int cnode_idx, pid_t pid, uid_t uid);


extern int actvhdlr_eventcbhandler_eventcb_path_clean_entry(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);


#else /* aarch64 */
extern int actvhdlr_eventcbhandler_eventcb_thread_msg_ctrl(unsigned long long sender, unsigned long credential, struct evtcb_thread_msg_s msg);


extern int actvhdlr_eventcbhandler_eventcb_process_exit(unsigned long long sender, unsigned long credential, unsigned int cnode_idx, pid_t pid, uid_t uid);


extern int actvhdlr_eventcbhandler_eventcb_path_clean_entry(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);


extern int __actvshadowhdlr_eventcbhandler_eventcb_thread_msg_ctrl(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_eventcbhandler_eventcb_process_exit(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_eventcbhandler_eventcb_path_clean_entry(unsigned long long sender, unsigned long credential, ...);


#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
static inline struct __actvret_eventcbcall_eventcb_thread_msg_ctrl* actvhdlr_eventcbhandler_eventcb_thread_msg_ctrl_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_eventcbcall_eventcb_thread_msg_ctrl));
	return (struct __actvret_eventcbcall_eventcb_thread_msg_ctrl*)__ret;
}

static inline struct __actvret_eventcbcall_eventcb_process_exit* actvhdlr_eventcbhandler_eventcb_process_exit_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_eventcbcall_eventcb_process_exit));
	return (struct __actvret_eventcbcall_eventcb_process_exit*)__ret;
}

static inline struct __actvret_eventcbcall_eventcb_path_clean_entry* actvhdlr_eventcbhandler_eventcb_path_clean_entry_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_eventcbcall_eventcb_path_clean_entry));
	return (struct __actvret_eventcbcall_eventcb_path_clean_entry*)__ret;
}

#ifdef __sysif_server_eventcb_no_compat32_handlers__
#ifdef __HOST_LLT__
union eventcb_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl)(unsigned long long sender, unsigned long credential, struct evtcb_thread_msg_s msg);
	int (*p__actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct evtcb_thread_msg_s msg);
	int (*p__actvbridgehdlr_eventcbhandler_eventcb_process_exit)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx, pid_t pid, uid_t uid);
	int (*p__actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx, pid_t pid, uid_t uid);
	int (*p__actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);
	int (*p__actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union eventcb_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl)(unsigned long long sender, unsigned long credential, struct evtcb_thread_msg_s msg);
	int (*p__actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct evtcb_thread_msg_s msg);
	int (*p__actvbridgehdlr_eventcbhandler_eventcb_process_exit)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx, pid_t pid, uid_t uid);
	int (*p__actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx, pid_t pid, uid_t uid);
	int (*p__actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);
	int (*p__actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#ifdef __arm__
union eventcb_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl)(unsigned long long sender, unsigned long credential, struct evtcb_thread_msg_s msg);
	int (*p__actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct evtcb_thread_msg_s msg);
	int (*p__actvbridgehdlr_eventcbhandler_eventcb_process_exit)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx, pid_t pid, uid_t uid);
	int (*p__actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx, pid_t pid, uid_t uid);
	int (*p__actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);
	int (*p__actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential);
};

#endif /* arm */
#endif /* not HOST_LLT */
#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif /* not __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#ifdef __HOST_LLT__
union eventcb_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl)(unsigned long long sender, unsigned long credential, struct evtcb_thread_msg_s msg);
	int (*p__actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct evtcb_thread_msg_s msg);
	int (*p__actvbridgehdlr_eventcbhandler_eventcb_process_exit)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx, pid_t pid, uid_t uid);
	int (*p__actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx, pid_t pid, uid_t uid);
	int (*p__actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);
	int (*p__actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx);
	int (*p__actvbridgehdlr_compat_eventcbhandler_eventcb_thread_msg_ctrl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_thread_msg_ctrl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_eventcbhandler_eventcb_process_exit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_process_exit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_eventcbhandler_eventcb_path_clean_entry)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_path_clean_entry)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union eventcb_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl)(unsigned long long sender, unsigned long credential, struct evtcb_thread_msg_s msg);
	int (*p__actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct evtcb_thread_msg_s msg);
	int (*p__actvbridgehdlr_eventcbhandler_eventcb_process_exit)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx, pid_t pid, uid_t uid);
	int (*p__actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx, pid_t pid, uid_t uid);
	int (*p__actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry)(unsigned long long sender, unsigned long credential, unsigned int cnode_idx);
	int (*p__actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx);
	int (*p__actvbridgehdlr_compat_eventcbhandler_eventcb_thread_msg_ctrl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_thread_msg_ctrl)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_eventcbhandler_eventcb_process_exit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_process_exit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_eventcbhandler_eventcb_path_clean_entry)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_path_clean_entry)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#endif /* not HOST_LLT */
#endif /* compat32 */

#if defined(__sysif_server_eventcb_no_compat32_handlers__) && defined(__sysif_server_eventcb_no_ilp32_handlers__)
#define __eventcb_method_TIMES 1
#elif !defined(__sysif_server_eventcb_no_compat32_handlers__) && !defined(__sysif_server_eventcb_no_ilp32_handlers__)
#define __eventcb_method_TIMES 3
#else
#define __eventcb_method_TIMES 2
#endif
extern union eventcb_hdlrs_union sysif_eventcb_actvhdlr_table[__eventcb_method_MAX * __eventcb_method_TIMES];

extern union eventcb_hdlrs_union sysif_eventcb_actvcapcallhdlr_table[__eventcb_method_MAX * __eventcb_method_TIMES];

extern void sysif_show_eventcb_methods(void);


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
static int __opt_nonoframe __actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl(unsigned long long sender, unsigned long credential, struct evtcb_thread_msg_s msg)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct evtcb_thread_msg_s), __arch_actv_rpc_stksz_alignment(struct evtcb_thread_msg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_eventcbhandler_eventcb_thread_msg_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct evtcb_thread_msg_s, msg));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct evtcb_thread_msg_s msg)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct evtcb_thread_msg_s), __arch_actv_rpc_stksz_alignment(struct evtcb_thread_msg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_eventcbhandler_eventcb_thread_msg_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct evtcb_thread_msg_s, msg));
}

static int __opt_nonoframe __actvbridgehdlr_eventcbhandler_eventcb_process_exit(unsigned long long sender, unsigned long credential, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uid_t), __arch_actv_rpc_stksz_alignment(uid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_eventcbhandler_eventcb_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx), __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid), __ARCH_ACTV_RPC_RECV_ARG(uid_t, uid));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uid_t), __arch_actv_rpc_stksz_alignment(uid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_eventcbhandler_eventcb_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx), __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid), __ARCH_ACTV_RPC_RECV_ARG(uid_t, uid));
}

static int __opt_nonoframe __actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry(unsigned long long sender, unsigned long credential, unsigned int cnode_idx)
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
	return __actvshadowhdlr_eventcbhandler_eventcb_path_clean_entry(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx)
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
	return __actvshadowhdlr_eventcbhandler_eventcb_path_clean_entry(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

#ifndef __sysif_server_eventcb_no_compat32_handlers__
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
static int __opt_nonoframe __actvbridgehdlr_compat_eventcbhandler_eventcb_thread_msg_ctrl(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct evtcb_thread_msg_s __maybe_unused _msg;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct evtcb_thread_msg_s);
		type = __type_of(struct evtcb_thread_msg_s);
		__fetch_one_arg((void*)&_msg, data, 0, arg_sz, model, type, struct evtcb_thread_msg_s, _msg)
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
	return __actvshadowhdlr_eventcbhandler_eventcb_thread_msg_ctrl(sender, credential, _msg);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_thread_msg_ctrl(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct evtcb_thread_msg_s __maybe_unused _msg;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct evtcb_thread_msg_s);
		type = __type_of(struct evtcb_thread_msg_s);
		__fetch_one_arg((void*)&_msg, data, 0, arg_sz, model, type, struct evtcb_thread_msg_s, _msg)
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
	return __actvshadowhdlr_eventcbhandler_eventcb_thread_msg_ctrl(sender, credential, _msg);
}

static int __opt_nonoframe __actvbridgehdlr_compat_eventcbhandler_eventcb_process_exit(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	pid_t __maybe_unused _pid;
	uid_t __maybe_unused _uid;
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
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(pid_t);
		type = __type_of(pid_t);
		__fetch_one_arg((void*)&_pid, data, 1, arg_sz, model, type, pid_t, _pid)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uid_t);
		type = __type_of(uid_t);
		__fetch_one_arg((void*)&_uid, data, 2, arg_sz, model, type, uid_t, _uid)
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
	return __actvshadowhdlr_eventcbhandler_eventcb_process_exit(sender, credential, _cnode_idx, _pid, _uid);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_process_exit(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	pid_t __maybe_unused _pid;
	uid_t __maybe_unused _uid;
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
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(pid_t);
		type = __type_of(pid_t);
		__fetch_one_arg((void*)&_pid, data, 1, arg_sz, model, type, pid_t, _pid)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uid_t);
		type = __type_of(uid_t);
		__fetch_one_arg((void*)&_uid, data, 2, arg_sz, model, type, uid_t, _uid)
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
	return __actvshadowhdlr_eventcbhandler_eventcb_process_exit(sender, credential, _cnode_idx, _pid, _uid);
}

static int __opt_nonoframe __actvbridgehdlr_compat_eventcbhandler_eventcb_path_clean_entry(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_eventcbhandler_eventcb_path_clean_entry(sender, credential, _cnode_idx);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_path_clean_entry(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_eventcbhandler_eventcb_path_clean_entry(sender, credential, _cnode_idx);
}

#endif /* compat handlers */
#ifndef __sysif_server_eventcb_no_ilp32_handlers__
static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_eventcbhandler_eventcb_thread_msg_ctrl(unsigned long long sender, unsigned long credential, struct evtcb_thread_msg_s msg)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct evtcb_thread_msg_s), __arch_actv_rpc_stksz_alignment(struct evtcb_thread_msg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct evtcb_thread_msg_s, msg)
	} while (0);
	return __actvshadowhdlr_eventcbhandler_eventcb_thread_msg_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct evtcb_thread_msg_s, msg));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_eventcbhandler_eventcb_thread_msg_ctrl(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct evtcb_thread_msg_s msg)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct evtcb_thread_msg_s), __arch_actv_rpc_stksz_alignment(struct evtcb_thread_msg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct evtcb_thread_msg_s, msg)
	} while (0);
	return __actvshadowhdlr_eventcbhandler_eventcb_thread_msg_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct evtcb_thread_msg_s, msg));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_eventcbhandler_eventcb_process_exit(unsigned long long sender, unsigned long credential, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uid_t), __arch_actv_rpc_stksz_alignment(uid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, cnode_idx)
	} while (0);
	do {
		__adjust_one_arg(pid_t, pid)
	} while (0);
	do {
		__adjust_one_arg(uid_t, uid)
	} while (0);
	return __actvshadowhdlr_eventcbhandler_eventcb_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx), __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid), __ARCH_ACTV_RPC_RECV_ARG(uid_t, uid));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_eventcbhandler_eventcb_process_exit(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uid_t), __arch_actv_rpc_stksz_alignment(uid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, cnode_idx)
	} while (0);
	do {
		__adjust_one_arg(pid_t, pid)
	} while (0);
	do {
		__adjust_one_arg(uid_t, uid)
	} while (0);
	return __actvshadowhdlr_eventcbhandler_eventcb_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx), __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid), __ARCH_ACTV_RPC_RECV_ARG(uid_t, uid));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_eventcbhandler_eventcb_path_clean_entry(unsigned long long sender, unsigned long credential, unsigned int cnode_idx)
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
	return __actvshadowhdlr_eventcbhandler_eventcb_path_clean_entry(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_eventcbhandler_eventcb_path_clean_entry(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx)
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
	return __actvshadowhdlr_eventcbhandler_eventcb_path_clean_entry(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

#endif
#else
static int __opt_nonoframe __actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl(unsigned long long sender, unsigned long credential, struct evtcb_thread_msg_s msg)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct evtcb_thread_msg_s), __arch_actv_rpc_stksz_alignment(struct evtcb_thread_msg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_eventcbhandler_eventcb_thread_msg_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct evtcb_thread_msg_s, msg));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct evtcb_thread_msg_s msg)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct evtcb_thread_msg_s), __arch_actv_rpc_stksz_alignment(struct evtcb_thread_msg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_eventcbhandler_eventcb_thread_msg_ctrl(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct evtcb_thread_msg_s, msg));
}

static int __opt_nonoframe __actvbridgehdlr_eventcbhandler_eventcb_process_exit(unsigned long long sender, unsigned long credential, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uid_t), __arch_actv_rpc_stksz_alignment(uid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_eventcbhandler_eventcb_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx), __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid), __ARCH_ACTV_RPC_RECV_ARG(uid_t, uid));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(pid_t), __arch_actv_rpc_stksz_alignment(pid_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uid_t), __arch_actv_rpc_stksz_alignment(uid_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_eventcbhandler_eventcb_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx), __ARCH_ACTV_RPC_RECV_ARG(pid_t, pid), __ARCH_ACTV_RPC_RECV_ARG(uid_t, uid));
}

static int __opt_nonoframe __actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry(unsigned long long sender, unsigned long credential, unsigned int cnode_idx)
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
	return actvhdlr_eventcbhandler_eventcb_path_clean_entry(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int cnode_idx)
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
	return actvhdlr_eventcbhandler_eventcb_path_clean_entry(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, cnode_idx));
}

#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#if defined(__sysif_server_eventcb_no_compat32_handlers__) && defined(__sysif_server_eventcb_no_ilp32_handlers__)
__ro_after_init_hdlr union eventcb_hdlrs_union sysif_eventcb_actvhdlr_table[__eventcb_method_MAX * __eventcb_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[ __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_process_exit = (&__actvbridgehdlr_eventcbhandler_eventcb_process_exit)
	},
	[ __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry)
	}
};

__ro_after_init_hdlr union eventcb_hdlrs_union sysif_eventcb_actvcapcallhdlr_table[__eventcb_method_MAX * __eventcb_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[ __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit = (&__actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit)
	},
	[ __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry)
	}
};

#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif
#if !defined(__sysif_server_eventcb_no_compat32_handlers__) && defined(__sysif_server_eventcb_no_ilp32_handlers__)
__ro_after_init_hdlr union eventcb_hdlrs_union sysif_eventcb_actvhdlr_table[__eventcb_method_MAX * __eventcb_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[ __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_process_exit = (&__actvbridgehdlr_eventcbhandler_eventcb_process_exit)
	},
	[ __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry)
	},
	[__eventcb_method_MAX ... __eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __eventcb_method_MAX + __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgehdlr_compat_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgehdlr_compat_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgehdlr_compat_eventcbhandler_eventcb_process_exit = (&__actvbridgehdlr_compat_eventcbhandler_eventcb_process_exit)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgehdlr_compat_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgehdlr_compat_eventcbhandler_eventcb_path_clean_entry)
	}
};

__ro_after_init_hdlr union eventcb_hdlrs_union sysif_eventcb_actvcapcallhdlr_table[__eventcb_method_MAX * __eventcb_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[ __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit = (&__actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit)
	},
	[ __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry)
	},
	[__eventcb_method_MAX ... __eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __eventcb_method_MAX + __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_process_exit = (&__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_process_exit)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_path_clean_entry)
	}
};

#endif /* aarch64 + compat */
#if defined(__sysif_server_eventcb_no_compat32_handlers__) && !defined(__sysif_server_eventcb_no_ilp32_handlers__)
__ro_after_init_hdlr union eventcb_hdlrs_union sysif_eventcb_actvhdlr_table[__eventcb_method_MAX * __eventcb_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[ __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_process_exit = (&__actvbridgehdlr_eventcbhandler_eventcb_process_exit)
	},
	[ __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry)
	},
	[__eventcb_method_MAX ... __eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __eventcb_method_MAX + __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgehdlr_ilp32_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_process_exit = (&__actvbridgehdlr_ilp32_eventcbhandler_eventcb_process_exit)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgehdlr_ilp32_eventcbhandler_eventcb_path_clean_entry)
	}
};

__ro_after_init_hdlr union eventcb_hdlrs_union sysif_eventcb_actvcapcallhdlr_table[__eventcb_method_MAX * __eventcb_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[ __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit = (&__actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit)
	},
	[ __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry)
	},
	[__eventcb_method_MAX ... __eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __eventcb_method_MAX + __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgecapcallhdlr_ilp32_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit = (&__actvbridgecapcallhdlr_ilp32_eventcbhandler_eventcb_process_exit)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgecapcallhdlr_ilp32_eventcbhandler_eventcb_path_clean_entry)
	}
};

#endif /* aarch64 + ilp32 */
#if !defined(__sysif_server_eventcb_no_compat32_handlers__) && !defined(__sysif_server_eventcb_no_ilp32_handlers__)
__ro_after_init_hdlr union eventcb_hdlrs_union sysif_eventcb_actvhdlr_table[__eventcb_method_MAX * __eventcb_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[ __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_process_exit = (&__actvbridgehdlr_eventcbhandler_eventcb_process_exit)
	},
	[ __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry)
	},
	[__eventcb_method_MAX ... __eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __eventcb_method_MAX + __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgehdlr_compat_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgehdlr_compat_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgehdlr_compat_eventcbhandler_eventcb_process_exit = (&__actvbridgehdlr_compat_eventcbhandler_eventcb_process_exit)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgehdlr_compat_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgehdlr_compat_eventcbhandler_eventcb_path_clean_entry)
	},
	[2 * __eventcb_method_MAX ... 2 * __eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[2 * __eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... 2 * __eventcb_method_MAX + __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgehdlr_ilp32_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_process_exit = (&__actvbridgehdlr_ilp32_eventcbhandler_eventcb_process_exit)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgehdlr_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgehdlr_ilp32_eventcbhandler_eventcb_path_clean_entry)
	}
};

__ro_after_init_hdlr union eventcb_hdlrs_union sysif_eventcb_actvcapcallhdlr_table[__eventcb_method_MAX * __eventcb_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[ __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit = (&__actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit)
	},
	[ __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry)
	},
	[__eventcb_method_MAX ... __eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __eventcb_method_MAX + __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_process_exit = (&__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_process_exit)
	},
	[__eventcb_method_MAX + __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgecapcallhdlr_compat_eventcbhandler_eventcb_path_clean_entry)
	},
	[2 * __eventcb_method_MAX ... 2 * __eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[2 * __eventcb_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... 2 * __eventcb_method_MAX + __eventcb_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_thread_msg_ctrl] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_thread_msg_ctrl = (&__actvbridgecapcallhdlr_ilp32_eventcbhandler_eventcb_thread_msg_ctrl)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_process_exit] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_process_exit = (&__actvbridgecapcallhdlr_ilp32_eventcbhandler_eventcb_process_exit)
	},
	[2 * __eventcb_method_MAX + __eventcb_method_eventcb_path_clean_entry] = {
		.p__actvbridgecapcallhdlr_eventcbhandler_eventcb_path_clean_entry = (&__actvbridgecapcallhdlr_ilp32_eventcbhandler_eventcb_path_clean_entry)
	}
};

#endif /* aarch64 + compat + ilp32 */
#endif /* arm or aarch64 only */
#ifdef __aarch64__
void sysif_show_eventcb_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "eventcbcall", "eventcb", "thread_msg_ctrl", __eventcb_method_eventcb_thread_msg_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "eventcbhandler", "eventcb", "thread_msg_ctrl", __eventcb_method_eventcb_thread_msg_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "eventcbcall", "eventcb", "process_exit", __eventcb_method_eventcb_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "eventcbhandler", "eventcb", "process_exit", __eventcb_method_eventcb_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "eventcbcall", "eventcb", "path_clean_entry", __eventcb_method_eventcb_path_clean_entry);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "eventcbhandler", "eventcb", "path_clean_entry", __eventcb_method_eventcb_path_clean_entry);
	hm_info("%s_%s, max method num: %d\n", "eventcb", "method", __eventcb_method_MAX);
}

#elif defined(__arm__)
void sysif_show_eventcb_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "eventcbcall", "eventcb", "thread_msg_ctrl", __eventcb_method_eventcb_thread_msg_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "eventcbhandler", "eventcb", "thread_msg_ctrl", __eventcb_method_eventcb_thread_msg_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "eventcbcall", "eventcb", "process_exit", __eventcb_method_eventcb_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "eventcbhandler", "eventcb", "process_exit", __eventcb_method_eventcb_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "eventcbcall", "eventcb", "path_clean_entry", __eventcb_method_eventcb_path_clean_entry);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "eventcbhandler", "eventcb", "path_clean_entry", __eventcb_method_eventcb_path_clean_entry);
	hm_info("%s_%s, max method num: %d\n", "eventcb", "method", __eventcb_method_MAX);
}

#else
void sysif_show_eventcb_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "eventcbcall", "eventcb", "thread_msg_ctrl", __eventcb_method_eventcb_thread_msg_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "eventcbhandler", "eventcb", "thread_msg_ctrl", __eventcb_method_eventcb_thread_msg_ctrl);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "eventcbcall", "eventcb", "process_exit", __eventcb_method_eventcb_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "eventcbhandler", "eventcb", "process_exit", __eventcb_method_eventcb_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "eventcbcall", "eventcb", "path_clean_entry", __eventcb_method_eventcb_path_clean_entry);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "eventcbhandler", "eventcb", "path_clean_entry", __eventcb_method_eventcb_path_clean_entry);
	hm_info("%s_%s, max method num: %d\n", "eventcb", "method", __eventcb_method_MAX);
}

#endif
#endif
#endif
