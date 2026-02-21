/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for hmtracemgr
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../kernel/hongmeng/hm-verif-kernel/ulibs/include/libsysif/hmtracemgr/api.sysif -I ../kernel/hongmeng/hm-verif-kernel/ulibs/include --api-dest ../image/usr/include/generated/sysif_client/hmtracemgr/libsysif/hmtracemgr/api.h --server-dest ../image/usr/include/generated/sysif_server/hmtracemgr/libsysif/hmtracemgr/server.h --dump-callno ../image/usr/include/generated/dump_info/hmtracemgr/libsysif/hmtracemgr/l2h_map.h --legacy-mode --no-selfcall
* Create: Fri Oct 31 10:27:09 2025
*/
#ifndef LIBSYSIF_HMTRACEMGR_SERVER_H
#define LIBSYSIF_HMTRACEMGR_SERVER_H
#ifndef LIBSYSIF_HMTRACEMGR_COMMON_H
#define LIBSYSIF_HMTRACEMGR_COMMON_H
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

#include <libhmtrace/hmtrace.h>
#include <libhmtrace/ring_buffer/ring_buffer.h>
#include <libbunch/bunch_ipc.h>

#define HMTRACE_DATA_SIZE HMTRACE_MAX_CLASS_NAME_SIZE
struct hmtrace_data {
	char buffer[HMTRACE_DATA_SIZE];
};

struct hmtrace_eventname {
	char buffer[HMTRACE_MAX_EVENT_NAME_SIZE_RESTRICTED];
};

raw_static_assert(sizeof(struct bunch_ipc_attr) <= 96,
                    hmtracecall_hmtrace_get_registered_class_list_arg_attr_is_too_large);
raw_static_assert(sizeof(struct hmtrace_data) <= 96,
                    hmtracecall_hmtrace_get_registered_events_list_arg_class_data_is_too_large);
raw_static_assert(sizeof(struct bunch_ipc_attr) <= 96,
                    hmtracecall_hmtrace_get_registered_events_list_arg_attr_is_too_large);
raw_static_assert(sizeof(struct hmtrace_data) <= 96,
                    hmtracecall_hmtrace_get_available_events_list_arg_class_data_is_too_large);
raw_static_assert(sizeof(struct bunch_ipc_attr) <= 96,
                    hmtracecall_hmtrace_get_available_events_list_arg_attr_is_too_large);
raw_static_assert(sizeof(struct hmtrace_data) <= 96,
                    hmtracecall_hmtrace_register_event_arg_class_data_is_too_large);
raw_static_assert(sizeof(struct hmtrace_data) <= 96,
                    hmtracecall_hmtrace_unregister_event_arg_class_data_is_too_large);
raw_static_assert(sizeof(struct hmtrace_data) <= 96,
                    hmtracecall_hmtrace_set_event_tracing_on_arg_class_data_is_too_large);
raw_static_assert(sizeof(struct hmtrace_eventname) <= 96,
                    hmtracecall_hmtrace_set_event_tracing_on_arg_event_data_is_too_large);
raw_static_assert(sizeof(struct hmtrace_data) <= 96,
                    hmtracecall_hmtrace_set_event_tracing_off_arg_class_data_is_too_large);
raw_static_assert(sizeof(struct hmtrace_eventname) <= 96,
                    hmtracecall_hmtrace_set_event_tracing_off_arg_event_data_is_too_large);
raw_static_assert(sizeof(struct hmtrace_data) <= 96,
                    hmtracecall_hmtrace_get_event_tracing_status_arg_class_data_is_too_large);
raw_static_assert(sizeof(struct hmtrace_eventname) <= 96,
                    hmtracecall_hmtrace_get_event_tracing_status_arg_event_data_is_too_large);
raw_static_assert(sizeof(struct hmtrace_data) <= 96,
                    hmtracecall_hmtrace_get_rb_info_arg_class_data_is_too_large);
raw_static_assert(sizeof(struct hmtrace_data) <= 96,
                    hmtracecall_hmtrace_ipc_buf_open_arg_ipc_data_is_too_large);
raw_static_assert(sizeof(struct hmtrace_data) <= 96,
                    hmtracecall_hmtrace_get_event_tracing_eid_arg_class_data_is_too_large);
raw_static_assert(sizeof(struct hmtrace_eventname) <= 96,
                    hmtracecall_hmtrace_get_event_tracing_eid_arg_event_data_is_too_large);
raw_static_assert(sizeof(struct hmtrace_data) <= 96,
                    hmtracecall_hmtrace_realloc_trace_buffer_arg_class_data_is_too_large);
raw_static_assert(sizeof(struct hmtrace_data) <= 96,
                    hmtracecall_hmtrace_trace_buffer_size_arg_class_data_is_too_large);
struct __actvret_hmtracecall_hmtrace_get_registered_class_list {
};

struct __actvret_hmtracecall_hmtrace_get_registered_events_list {
};

struct __actvret_hmtracecall_hmtrace_get_available_events_list {
};

struct __actvret_hmtracecall_hmtrace_register_event {
};

struct __actvret_hmtracecall_hmtrace_unregister_event {
};

struct __actvret_hmtracecall_hmtrace_set_event_tracing_on {
};

struct __actvret_hmtracecall_hmtrace_set_event_tracing_off {
};

struct __actvret_hmtracecall_hmtrace_events_tracing_on {
};

struct __actvret_hmtracecall_hmtrace_get_event_tracing_status {
	int status;
};

struct __actvret_hmtracecall_hmtrace_get_rb_info {
	struct rb rb;
};

struct __actvret_hmtracecall_hmtrace_ipc_buf_open {
};

struct __actvret_hmtracecall_hmtrace_ipc_buf_read {
};

struct __actvret_hmtracecall_hmtrace_ipc_buf_close {
};

struct __actvret_hmtracecall_hmtrace_get_event_tracing_eid {
	int eid;
};

struct __actvret_hmtracecall_hmtrace_realloc_trace_buffer {
};

struct __actvret_hmtracecall_hmtrace_trace_buffer_size {
	unsigned long buffer_size;
};

#ifdef __aarch64__
union __actvret_hmtracecall {
	struct __actvret_hmtracecall_hmtrace_get_registered_class_list hmtrace_get_registered_class_list;
	struct __actvret_hmtracecall_hmtrace_get_registered_events_list hmtrace_get_registered_events_list;
	struct __actvret_hmtracecall_hmtrace_get_available_events_list hmtrace_get_available_events_list;
	struct __actvret_hmtracecall_hmtrace_register_event hmtrace_register_event;
	struct __actvret_hmtracecall_hmtrace_unregister_event hmtrace_unregister_event;
	struct __actvret_hmtracecall_hmtrace_set_event_tracing_on hmtrace_set_event_tracing_on;
	struct __actvret_hmtracecall_hmtrace_set_event_tracing_off hmtrace_set_event_tracing_off;
	struct __actvret_hmtracecall_hmtrace_events_tracing_on hmtrace_events_tracing_on;
	struct __actvret_hmtracecall_hmtrace_get_event_tracing_status hmtrace_get_event_tracing_status;
	struct __actvret_hmtracecall_hmtrace_get_rb_info hmtrace_get_rb_info;
	struct __actvret_hmtracecall_hmtrace_ipc_buf_open hmtrace_ipc_buf_open;
	struct __actvret_hmtracecall_hmtrace_ipc_buf_read hmtrace_ipc_buf_read;
	struct __actvret_hmtracecall_hmtrace_ipc_buf_close hmtrace_ipc_buf_close;
	struct __actvret_hmtracecall_hmtrace_get_event_tracing_eid hmtrace_get_event_tracing_eid;
	struct __actvret_hmtracecall_hmtrace_realloc_trace_buffer hmtrace_realloc_trace_buffer;
	struct __actvret_hmtracecall_hmtrace_trace_buffer_size hmtrace_trace_buffer_size;
};

#elif defined(__arm__)
union __actvret_hmtracecall {
	struct __actvret_hmtracecall_hmtrace_get_registered_class_list hmtrace_get_registered_class_list;
	struct __actvret_hmtracecall_hmtrace_get_registered_events_list hmtrace_get_registered_events_list;
	struct __actvret_hmtracecall_hmtrace_get_available_events_list hmtrace_get_available_events_list;
	struct __actvret_hmtracecall_hmtrace_register_event hmtrace_register_event;
	struct __actvret_hmtracecall_hmtrace_unregister_event hmtrace_unregister_event;
	struct __actvret_hmtracecall_hmtrace_set_event_tracing_on hmtrace_set_event_tracing_on;
	struct __actvret_hmtracecall_hmtrace_set_event_tracing_off hmtrace_set_event_tracing_off;
	struct __actvret_hmtracecall_hmtrace_events_tracing_on hmtrace_events_tracing_on;
	struct __actvret_hmtracecall_hmtrace_get_event_tracing_status hmtrace_get_event_tracing_status;
	struct __actvret_hmtracecall_hmtrace_get_rb_info hmtrace_get_rb_info;
	struct __actvret_hmtracecall_hmtrace_ipc_buf_open hmtrace_ipc_buf_open;
	struct __actvret_hmtracecall_hmtrace_ipc_buf_read hmtrace_ipc_buf_read;
	struct __actvret_hmtracecall_hmtrace_ipc_buf_close hmtrace_ipc_buf_close;
	struct __actvret_hmtracecall_hmtrace_get_event_tracing_eid hmtrace_get_event_tracing_eid;
	struct __actvret_hmtracecall_hmtrace_realloc_trace_buffer hmtrace_realloc_trace_buffer;
	struct __actvret_hmtracecall_hmtrace_trace_buffer_size hmtrace_trace_buffer_size;
};

#else
union __actvret_hmtracecall {
	struct __actvret_hmtracecall_hmtrace_get_registered_class_list hmtrace_get_registered_class_list;
	struct __actvret_hmtracecall_hmtrace_get_registered_events_list hmtrace_get_registered_events_list;
	struct __actvret_hmtracecall_hmtrace_get_available_events_list hmtrace_get_available_events_list;
	struct __actvret_hmtracecall_hmtrace_register_event hmtrace_register_event;
	struct __actvret_hmtracecall_hmtrace_unregister_event hmtrace_unregister_event;
	struct __actvret_hmtracecall_hmtrace_set_event_tracing_on hmtrace_set_event_tracing_on;
	struct __actvret_hmtracecall_hmtrace_set_event_tracing_off hmtrace_set_event_tracing_off;
	struct __actvret_hmtracecall_hmtrace_events_tracing_on hmtrace_events_tracing_on;
	struct __actvret_hmtracecall_hmtrace_get_event_tracing_status hmtrace_get_event_tracing_status;
	struct __actvret_hmtracecall_hmtrace_get_rb_info hmtrace_get_rb_info;
	struct __actvret_hmtracecall_hmtrace_ipc_buf_open hmtrace_ipc_buf_open;
	struct __actvret_hmtracecall_hmtrace_ipc_buf_read hmtrace_ipc_buf_read;
	struct __actvret_hmtracecall_hmtrace_ipc_buf_close hmtrace_ipc_buf_close;
	struct __actvret_hmtracecall_hmtrace_get_event_tracing_eid hmtrace_get_event_tracing_eid;
	struct __actvret_hmtracecall_hmtrace_realloc_trace_buffer hmtrace_realloc_trace_buffer;
	struct __actvret_hmtracecall_hmtrace_trace_buffer_size hmtrace_trace_buffer_size;
};

#endif
enum __hmtracemgr_mgr {
	__hmtracemgr_INVAL_mgr=0,
	__hmtracemgr_hmtrace_mgr,
	__hmtracemgr_MAX_mgr,
};

#define __hmtracemgr_method_MIN __ACTV_HIGHTABLE_START
#define __hmtracemgr_method_hmtrace_get_registered_class_list (__ACTV_HIGHTABLE_START + 1)
#define __hmtracemgr_method_hmtrace_get_registered_events_list (__ACTV_HIGHTABLE_START + 2)
#define __hmtracemgr_method_hmtrace_get_available_events_list (__ACTV_HIGHTABLE_START + 3)
#define __hmtracemgr_method_hmtrace_register_event (__ACTV_HIGHTABLE_START + 4)
#define __hmtracemgr_method_hmtrace_unregister_event (__ACTV_HIGHTABLE_START + 5)
#define __hmtracemgr_method_hmtrace_set_event_tracing_on (__ACTV_HIGHTABLE_START + 6)
#define __hmtracemgr_method_hmtrace_set_event_tracing_off (__ACTV_HIGHTABLE_START + 7)
#define __hmtracemgr_method_hmtrace_events_tracing_on (__ACTV_HIGHTABLE_START + 8)
#define __hmtracemgr_method_hmtrace_get_event_tracing_status (__ACTV_HIGHTABLE_START + 9)
#define __hmtracemgr_method_hmtrace_get_rb_info (__ACTV_HIGHTABLE_START + 10)
#define __hmtracemgr_method_hmtrace_ipc_buf_open (__ACTV_HIGHTABLE_START + 11)
#define __hmtracemgr_method_hmtrace_ipc_buf_read (__ACTV_HIGHTABLE_START + 12)
#define __hmtracemgr_method_hmtrace_ipc_buf_close (__ACTV_HIGHTABLE_START + 13)
#define __hmtracemgr_method_hmtrace_get_event_tracing_eid (__ACTV_HIGHTABLE_START + 14)
#define __hmtracemgr_method_hmtrace_realloc_trace_buffer (__ACTV_HIGHTABLE_START + 15)
#define __hmtracemgr_method_hmtrace_trace_buffer_size (__ACTV_HIGHTABLE_START + 16)
#define __hmtracemgr_method_MAX (__ACTV_HIGHTABLE_START + 17)
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_get_registered_class_list) < 512,
            hmtracecall_hmtrace_get_registered_class_list_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_get_registered_events_list) < 512,
            hmtracecall_hmtrace_get_registered_events_list_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_get_available_events_list) < 512,
            hmtracecall_hmtrace_get_available_events_list_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_register_event) < 512,
            hmtracecall_hmtrace_register_event_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_unregister_event) < 512,
            hmtracecall_hmtrace_unregister_event_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_set_event_tracing_on) < 512,
            hmtracecall_hmtrace_set_event_tracing_on_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_set_event_tracing_off) < 512,
            hmtracecall_hmtrace_set_event_tracing_off_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_events_tracing_on) < 512,
            hmtracecall_hmtrace_events_tracing_on_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_get_event_tracing_status) < 512,
            hmtracecall_hmtrace_get_event_tracing_status_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_get_rb_info) < 512,
            hmtracecall_hmtrace_get_rb_info_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_ipc_buf_open) < 512,
            hmtracecall_hmtrace_ipc_buf_open_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_ipc_buf_read) < 512,
            hmtracecall_hmtrace_ipc_buf_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_ipc_buf_close) < 512,
            hmtracecall_hmtrace_ipc_buf_close_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_get_event_tracing_eid) < 512,
            hmtracecall_hmtrace_get_event_tracing_eid_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_realloc_trace_buffer) < 512,
            hmtracecall_hmtrace_realloc_trace_buffer_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_hmtracecall_hmtrace_trace_buffer_size) < 512,
            hmtracecall_hmtrace_trace_buffer_size_too_many_actvret_data);
raw_static_assert(sizeof(union __actvret_hmtracecall) < 512,
        hmtracecall_too_many_actvret_data);
extern int __actvcall_hmtracecall_hmtrace_get_registered_class_list(unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr);


extern int __actvcapcall_hmtracecall_hmtrace_get_registered_class_list(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_get_registered_class_list(bool is_xact_fwd, bool is_xact_allow_refwd, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_get_registered_class_list;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_get_registered_class_list(rpc_info, NULL, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_get_registered_class_list(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_get_registered_class_list;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_get_registered_class_list(__rref, rpc_info, NULL, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_get_registered_class_list(struct bunch_ipc_attr attr)
{
	return actvxactcall_hmtracecall_hmtrace_get_registered_class_list(false, false, attr);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_get_registered_class_list(rref_t __rref, struct bunch_ipc_attr attr)
{
	return actvxactcapcall_hmtracecall_hmtrace_get_registered_class_list(false, false, __rref, attr);
}

extern int __actvcall_hmtracecall_hmtrace_get_registered_events_list(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct bunch_ipc_attr attr);


extern int __actvcapcall_hmtracecall_hmtrace_get_registered_events_list(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct bunch_ipc_attr attr);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_get_registered_events_list(bool is_xact_fwd, bool is_xact_allow_refwd, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_get_registered_events_list;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_get_registered_events_list(rpc_info, NULL, class_data, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_get_registered_events_list(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_get_registered_events_list;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_get_registered_events_list(__rref, rpc_info, NULL, class_data, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_get_registered_events_list(struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	return actvxactcall_hmtracecall_hmtrace_get_registered_events_list(false, false, class_data, attr);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_get_registered_events_list(rref_t __rref, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	return actvxactcapcall_hmtracecall_hmtrace_get_registered_events_list(false, false, __rref, class_data, attr);
}

extern int __actvcall_hmtracecall_hmtrace_get_available_events_list(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct bunch_ipc_attr attr);


extern int __actvcapcall_hmtracecall_hmtrace_get_available_events_list(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct bunch_ipc_attr attr);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_get_available_events_list(bool is_xact_fwd, bool is_xact_allow_refwd, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_get_available_events_list;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_get_available_events_list(rpc_info, NULL, class_data, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_get_available_events_list(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_get_available_events_list;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_get_available_events_list(__rref, rpc_info, NULL, class_data, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_get_available_events_list(struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	return actvxactcall_hmtracecall_hmtrace_get_available_events_list(false, false, class_data, attr);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_get_available_events_list(rref_t __rref, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	return actvxactcapcall_hmtracecall_hmtrace_get_available_events_list(false, false, __rref, class_data, attr);
}

extern int __actvcall_hmtracecall_hmtrace_register_event(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, unsigned long long key);


extern int __actvcapcall_hmtracecall_hmtrace_register_event(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, unsigned long long key);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_register_event(bool is_xact_fwd, bool is_xact_allow_refwd, struct hmtrace_data class_data, unsigned long long key)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_register_event;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_register_event(rpc_info, NULL, class_data, key);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_register_event(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hmtrace_data class_data, unsigned long long key)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_register_event;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_register_event(__rref, rpc_info, NULL, class_data, key);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_register_event(struct hmtrace_data class_data, unsigned long long key)
{
	return actvxactcall_hmtracecall_hmtrace_register_event(false, false, class_data, key);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_register_event(rref_t __rref, struct hmtrace_data class_data, unsigned long long key)
{
	return actvxactcapcall_hmtracecall_hmtrace_register_event(false, false, __rref, class_data, key);
}

extern int __actvcall_hmtracecall_hmtrace_unregister_event(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, unsigned long long key);


extern int __actvcapcall_hmtracecall_hmtrace_unregister_event(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, unsigned long long key);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_unregister_event(bool is_xact_fwd, bool is_xact_allow_refwd, struct hmtrace_data class_data, unsigned long long key)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_unregister_event;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_unregister_event(rpc_info, NULL, class_data, key);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_unregister_event(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hmtrace_data class_data, unsigned long long key)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_unregister_event;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_unregister_event(__rref, rpc_info, NULL, class_data, key);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_unregister_event(struct hmtrace_data class_data, unsigned long long key)
{
	return actvxactcall_hmtracecall_hmtrace_unregister_event(false, false, class_data, key);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_unregister_event(rref_t __rref, struct hmtrace_data class_data, unsigned long long key)
{
	return actvxactcapcall_hmtracecall_hmtrace_unregister_event(false, false, __rref, class_data, key);
}

extern int __actvcall_hmtracecall_hmtrace_set_event_tracing_on(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


extern int __actvcapcall_hmtracecall_hmtrace_set_event_tracing_on(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_set_event_tracing_on(bool is_xact_fwd, bool is_xact_allow_refwd, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_set_event_tracing_on;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_set_event_tracing_on(rpc_info, NULL, class_data, event_data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_set_event_tracing_on(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_set_event_tracing_on;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_set_event_tracing_on(__rref, rpc_info, NULL, class_data, event_data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_set_event_tracing_on(struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	return actvxactcall_hmtracecall_hmtrace_set_event_tracing_on(false, false, class_data, event_data);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_set_event_tracing_on(rref_t __rref, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	return actvxactcapcall_hmtracecall_hmtrace_set_event_tracing_on(false, false, __rref, class_data, event_data);
}

extern int __actvcall_hmtracecall_hmtrace_set_event_tracing_off(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


extern int __actvcapcall_hmtracecall_hmtrace_set_event_tracing_off(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_set_event_tracing_off(bool is_xact_fwd, bool is_xact_allow_refwd, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_set_event_tracing_off;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_set_event_tracing_off(rpc_info, NULL, class_data, event_data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_set_event_tracing_off(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_set_event_tracing_off;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_set_event_tracing_off(__rref, rpc_info, NULL, class_data, event_data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_set_event_tracing_off(struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	return actvxactcall_hmtracecall_hmtrace_set_event_tracing_off(false, false, class_data, event_data);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_set_event_tracing_off(rref_t __rref, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	return actvxactcapcall_hmtracecall_hmtrace_set_event_tracing_off(false, false, __rref, class_data, event_data);
}

extern int __actvcall_hmtracecall_hmtrace_events_tracing_on(unsigned long long __rpc_info, void *__buf, unsigned int status);


extern int __actvcapcall_hmtracecall_hmtrace_events_tracing_on(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int status);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_events_tracing_on(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int status)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_events_tracing_on;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_events_tracing_on(rpc_info, NULL, status);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_events_tracing_on(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int status)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_events_tracing_on;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_events_tracing_on(__rref, rpc_info, NULL, status);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_events_tracing_on(unsigned int status)
{
	return actvxactcall_hmtracecall_hmtrace_events_tracing_on(false, false, status);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_events_tracing_on(rref_t __rref, unsigned int status)
{
	return actvxactcapcall_hmtracecall_hmtrace_events_tracing_on(false, false, __rref, status);
}

extern int __actvcall_hmtracecall_hmtrace_get_event_tracing_status(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


extern int __actvcapcall_hmtracecall_hmtrace_get_event_tracing_status(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_get_event_tracing_status(bool is_xact_fwd, bool is_xact_allow_refwd, struct hmtrace_data class_data, struct hmtrace_eventname event_data, struct __actvret_hmtracecall_hmtrace_get_event_tracing_status *__ret)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_get_event_tracing_status;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_get_event_tracing_status(rpc_info, __ret, class_data, event_data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_get_event_tracing_status(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hmtrace_data class_data, struct hmtrace_eventname event_data, struct __actvret_hmtracecall_hmtrace_get_event_tracing_status *__ret)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_get_event_tracing_status;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_get_event_tracing_status(__rref, rpc_info, __ret, class_data, event_data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_get_event_tracing_status(struct hmtrace_data class_data, struct hmtrace_eventname event_data, struct __actvret_hmtracecall_hmtrace_get_event_tracing_status *__ret)
{
	return actvxactcall_hmtracecall_hmtrace_get_event_tracing_status(false, false, class_data, event_data, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_get_event_tracing_status(rref_t __rref, struct hmtrace_data class_data, struct hmtrace_eventname event_data, struct __actvret_hmtracecall_hmtrace_get_event_tracing_status *__ret)
{
	return actvxactcapcall_hmtracecall_hmtrace_get_event_tracing_status(false, false, __rref, class_data, event_data, __ret);
}

extern int __actvcall_hmtracecall_hmtrace_get_rb_info(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data);


extern int __actvcapcall_hmtracecall_hmtrace_get_rb_info(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_get_rb_info(bool is_xact_fwd, bool is_xact_allow_refwd, struct hmtrace_data class_data, struct __actvret_hmtracecall_hmtrace_get_rb_info *__ret)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_get_rb_info;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_get_rb_info(rpc_info, __ret, class_data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_get_rb_info(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hmtrace_data class_data, struct __actvret_hmtracecall_hmtrace_get_rb_info *__ret)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_get_rb_info;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_get_rb_info(__rref, rpc_info, __ret, class_data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_get_rb_info(struct hmtrace_data class_data, struct __actvret_hmtracecall_hmtrace_get_rb_info *__ret)
{
	return actvxactcall_hmtracecall_hmtrace_get_rb_info(false, false, class_data, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_get_rb_info(rref_t __rref, struct hmtrace_data class_data, struct __actvret_hmtracecall_hmtrace_get_rb_info *__ret)
{
	return actvxactcapcall_hmtracecall_hmtrace_get_rb_info(false, false, __rref, class_data, __ret);
}

extern int __actvcall_hmtracecall_hmtrace_ipc_buf_open(unsigned long long __rpc_info, void *__buf, struct hmtrace_data ipc_data);


extern int __actvcapcall_hmtracecall_hmtrace_ipc_buf_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data ipc_data);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_ipc_buf_open(bool is_xact_fwd, bool is_xact_allow_refwd, struct hmtrace_data ipc_data)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_ipc_buf_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_ipc_buf_open(rpc_info, NULL, ipc_data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_ipc_buf_open(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hmtrace_data ipc_data)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_ipc_buf_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_ipc_buf_open(__rref, rpc_info, NULL, ipc_data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_ipc_buf_open(struct hmtrace_data ipc_data)
{
	return actvxactcall_hmtracecall_hmtrace_ipc_buf_open(false, false, ipc_data);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_ipc_buf_open(rref_t __rref, struct hmtrace_data ipc_data)
{
	return actvxactcapcall_hmtracecall_hmtrace_ipc_buf_open(false, false, __rref, ipc_data);
}

extern int __actvcall_hmtracecall_hmtrace_ipc_buf_read(unsigned long long __rpc_info, void *__buf, int handle, unsigned long long key);


extern int __actvcapcall_hmtracecall_hmtrace_ipc_buf_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, int handle, unsigned long long key);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_ipc_buf_read(bool is_xact_fwd, bool is_xact_allow_refwd, int handle, unsigned long long key)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_ipc_buf_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_ipc_buf_read(rpc_info, NULL, handle, key);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_ipc_buf_read(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int handle, unsigned long long key)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_ipc_buf_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_ipc_buf_read(__rref, rpc_info, NULL, handle, key);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_ipc_buf_read(int handle, unsigned long long key)
{
	return actvxactcall_hmtracecall_hmtrace_ipc_buf_read(false, false, handle, key);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_ipc_buf_read(rref_t __rref, int handle, unsigned long long key)
{
	return actvxactcapcall_hmtracecall_hmtrace_ipc_buf_read(false, false, __rref, handle, key);
}

extern int __actvcall_hmtracecall_hmtrace_ipc_buf_close(unsigned long long __rpc_info, void *__buf, int handle);


extern int __actvcapcall_hmtracecall_hmtrace_ipc_buf_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, int handle);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_ipc_buf_close(bool is_xact_fwd, bool is_xact_allow_refwd, int handle)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_ipc_buf_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_ipc_buf_close(rpc_info, NULL, handle);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_ipc_buf_close(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int handle)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_ipc_buf_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_ipc_buf_close(__rref, rpc_info, NULL, handle);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_ipc_buf_close(int handle)
{
	return actvxactcall_hmtracecall_hmtrace_ipc_buf_close(false, false, handle);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_ipc_buf_close(rref_t __rref, int handle)
{
	return actvxactcapcall_hmtracecall_hmtrace_ipc_buf_close(false, false, __rref, handle);
}

extern int __actvcall_hmtracecall_hmtrace_get_event_tracing_eid(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


extern int __actvcapcall_hmtracecall_hmtrace_get_event_tracing_eid(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_get_event_tracing_eid(bool is_xact_fwd, bool is_xact_allow_refwd, struct hmtrace_data class_data, struct hmtrace_eventname event_data, struct __actvret_hmtracecall_hmtrace_get_event_tracing_eid *__ret)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_get_event_tracing_eid;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_get_event_tracing_eid(rpc_info, __ret, class_data, event_data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_get_event_tracing_eid(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hmtrace_data class_data, struct hmtrace_eventname event_data, struct __actvret_hmtracecall_hmtrace_get_event_tracing_eid *__ret)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_get_event_tracing_eid;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_get_event_tracing_eid(__rref, rpc_info, __ret, class_data, event_data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_get_event_tracing_eid(struct hmtrace_data class_data, struct hmtrace_eventname event_data, struct __actvret_hmtracecall_hmtrace_get_event_tracing_eid *__ret)
{
	return actvxactcall_hmtracecall_hmtrace_get_event_tracing_eid(false, false, class_data, event_data, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_get_event_tracing_eid(rref_t __rref, struct hmtrace_data class_data, struct hmtrace_eventname event_data, struct __actvret_hmtracecall_hmtrace_get_event_tracing_eid *__ret)
{
	return actvxactcapcall_hmtracecall_hmtrace_get_event_tracing_eid(false, false, __rref, class_data, event_data, __ret);
}

extern int __actvcall_hmtracecall_hmtrace_realloc_trace_buffer(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, unsigned long buffer_size);


extern int __actvcapcall_hmtracecall_hmtrace_realloc_trace_buffer(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, unsigned long buffer_size);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_realloc_trace_buffer(bool is_xact_fwd, bool is_xact_allow_refwd, struct hmtrace_data class_data, unsigned long buffer_size)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_realloc_trace_buffer;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_realloc_trace_buffer(rpc_info, NULL, class_data, buffer_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_realloc_trace_buffer(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hmtrace_data class_data, unsigned long buffer_size)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_realloc_trace_buffer;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_realloc_trace_buffer(__rref, rpc_info, NULL, class_data, buffer_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_realloc_trace_buffer(struct hmtrace_data class_data, unsigned long buffer_size)
{
	return actvxactcall_hmtracecall_hmtrace_realloc_trace_buffer(false, false, class_data, buffer_size);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_realloc_trace_buffer(rref_t __rref, struct hmtrace_data class_data, unsigned long buffer_size)
{
	return actvxactcapcall_hmtracecall_hmtrace_realloc_trace_buffer(false, false, __rref, class_data, buffer_size);
}

extern int __actvcall_hmtracecall_hmtrace_trace_buffer_size(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data);


extern int __actvcapcall_hmtracecall_hmtrace_trace_buffer_size(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data);


static inline int __attribute__((always_inline)) actvxactcall_hmtracecall_hmtrace_trace_buffer_size(bool is_xact_fwd, bool is_xact_allow_refwd, struct hmtrace_data class_data, struct __actvret_hmtracecall_hmtrace_trace_buffer_size *__ret)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_trace_buffer_size;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_hmtracecall_hmtrace_trace_buffer_size(rpc_info, __ret, class_data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_hmtracecall_hmtrace_trace_buffer_size(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hmtrace_data class_data, struct __actvret_hmtracecall_hmtrace_trace_buffer_size *__ret)
{
	const unsigned long __callno = __hmtracemgr_method_hmtrace_trace_buffer_size;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_hmtracecall_hmtrace_trace_buffer_size(__rref, rpc_info, __ret, class_data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_hmtracecall_hmtrace_trace_buffer_size(struct hmtrace_data class_data, struct __actvret_hmtracecall_hmtrace_trace_buffer_size *__ret)
{
	return actvxactcall_hmtracecall_hmtrace_trace_buffer_size(false, false, class_data, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_hmtracecall_hmtrace_trace_buffer_size(rref_t __rref, struct hmtrace_data class_data, struct __actvret_hmtracecall_hmtrace_trace_buffer_size *__ret)
{
	return actvxactcapcall_hmtracecall_hmtrace_trace_buffer_size(false, false, __rref, class_data, __ret);
}

#endif
#include <libsysif/utils.h>
#include <hmasm/actv_rpc.h>
#include <hmasm/types.h>
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
extern int actvhdlr_hmtracehandler_hmtrace_get_registered_class_list(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);


extern int actvhdlr_hmtracehandler_hmtrace_get_registered_events_list(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr);


extern int actvhdlr_hmtracehandler_hmtrace_get_available_events_list(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr);


extern int actvhdlr_hmtracehandler_hmtrace_register_event(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key);


extern int actvhdlr_hmtracehandler_hmtrace_unregister_event(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key);


extern int actvhdlr_hmtracehandler_hmtrace_set_event_tracing_on(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


extern int actvhdlr_hmtracehandler_hmtrace_set_event_tracing_off(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


extern int actvhdlr_hmtracehandler_hmtrace_events_tracing_on(unsigned long long sender, unsigned long credential, unsigned int status);


extern int actvhdlr_hmtracehandler_hmtrace_get_event_tracing_status(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


extern int actvhdlr_hmtracehandler_hmtrace_get_rb_info(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data);


extern int actvhdlr_hmtracehandler_hmtrace_ipc_buf_open(unsigned long long sender, unsigned long credential, struct hmtrace_data ipc_data);


extern int actvhdlr_hmtracehandler_hmtrace_ipc_buf_read(unsigned long long sender, unsigned long credential, int handle, unsigned long long key);


extern int actvhdlr_hmtracehandler_hmtrace_ipc_buf_close(unsigned long long sender, unsigned long credential, int handle);


extern int actvhdlr_hmtracehandler_hmtrace_get_event_tracing_eid(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


extern int actvhdlr_hmtracehandler_hmtrace_realloc_trace_buffer(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long buffer_size);


extern int actvhdlr_hmtracehandler_hmtrace_trace_buffer_size(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data);


#else /* aarch64 */
extern int actvhdlr_hmtracehandler_hmtrace_get_registered_class_list(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);


extern int actvhdlr_hmtracehandler_hmtrace_get_registered_events_list(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr);


extern int actvhdlr_hmtracehandler_hmtrace_get_available_events_list(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr);


extern int actvhdlr_hmtracehandler_hmtrace_register_event(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key);


extern int actvhdlr_hmtracehandler_hmtrace_unregister_event(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key);


extern int actvhdlr_hmtracehandler_hmtrace_set_event_tracing_on(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


extern int actvhdlr_hmtracehandler_hmtrace_set_event_tracing_off(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


extern int actvhdlr_hmtracehandler_hmtrace_events_tracing_on(unsigned long long sender, unsigned long credential, unsigned int status);


extern int actvhdlr_hmtracehandler_hmtrace_get_event_tracing_status(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


extern int actvhdlr_hmtracehandler_hmtrace_get_rb_info(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data);


extern int actvhdlr_hmtracehandler_hmtrace_ipc_buf_open(unsigned long long sender, unsigned long credential, struct hmtrace_data ipc_data);


extern int actvhdlr_hmtracehandler_hmtrace_ipc_buf_read(unsigned long long sender, unsigned long credential, int handle, unsigned long long key);


extern int actvhdlr_hmtracehandler_hmtrace_ipc_buf_close(unsigned long long sender, unsigned long credential, int handle);


extern int actvhdlr_hmtracehandler_hmtrace_get_event_tracing_eid(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);


extern int actvhdlr_hmtracehandler_hmtrace_realloc_trace_buffer(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long buffer_size);


extern int actvhdlr_hmtracehandler_hmtrace_trace_buffer_size(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_get_registered_class_list(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_get_registered_events_list(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_get_available_events_list(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_register_event(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_unregister_event(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_set_event_tracing_on(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_set_event_tracing_off(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_events_tracing_on(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_get_event_tracing_status(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_get_rb_info(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_open(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_read(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_close(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_get_event_tracing_eid(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_realloc_trace_buffer(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_hmtracehandler_hmtrace_trace_buffer_size(unsigned long long sender, unsigned long credential, ...);


#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
static inline struct __actvret_hmtracecall_hmtrace_get_registered_class_list* actvhdlr_hmtracehandler_hmtrace_get_registered_class_list_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_get_registered_class_list));
	return (struct __actvret_hmtracecall_hmtrace_get_registered_class_list*)__ret;
}

static inline struct __actvret_hmtracecall_hmtrace_get_registered_events_list* actvhdlr_hmtracehandler_hmtrace_get_registered_events_list_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_get_registered_events_list));
	return (struct __actvret_hmtracecall_hmtrace_get_registered_events_list*)__ret;
}

static inline struct __actvret_hmtracecall_hmtrace_get_available_events_list* actvhdlr_hmtracehandler_hmtrace_get_available_events_list_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_get_available_events_list));
	return (struct __actvret_hmtracecall_hmtrace_get_available_events_list*)__ret;
}

static inline struct __actvret_hmtracecall_hmtrace_register_event* actvhdlr_hmtracehandler_hmtrace_register_event_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_register_event));
	return (struct __actvret_hmtracecall_hmtrace_register_event*)__ret;
}

static inline struct __actvret_hmtracecall_hmtrace_unregister_event* actvhdlr_hmtracehandler_hmtrace_unregister_event_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_unregister_event));
	return (struct __actvret_hmtracecall_hmtrace_unregister_event*)__ret;
}

static inline struct __actvret_hmtracecall_hmtrace_set_event_tracing_on* actvhdlr_hmtracehandler_hmtrace_set_event_tracing_on_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_set_event_tracing_on));
	return (struct __actvret_hmtracecall_hmtrace_set_event_tracing_on*)__ret;
}

static inline struct __actvret_hmtracecall_hmtrace_set_event_tracing_off* actvhdlr_hmtracehandler_hmtrace_set_event_tracing_off_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_set_event_tracing_off));
	return (struct __actvret_hmtracecall_hmtrace_set_event_tracing_off*)__ret;
}

static inline struct __actvret_hmtracecall_hmtrace_events_tracing_on* actvhdlr_hmtracehandler_hmtrace_events_tracing_on_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_events_tracing_on));
	return (struct __actvret_hmtracecall_hmtrace_events_tracing_on*)__ret;
}

static inline struct __actvret_hmtracecall_hmtrace_get_event_tracing_status* actvhdlr_hmtracehandler_hmtrace_get_event_tracing_status_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_get_event_tracing_status));
	return (struct __actvret_hmtracecall_hmtrace_get_event_tracing_status*)__ret;
}

static inline struct __actvret_hmtracecall_hmtrace_get_rb_info* actvhdlr_hmtracehandler_hmtrace_get_rb_info_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_get_rb_info));
	return (struct __actvret_hmtracecall_hmtrace_get_rb_info*)__ret;
}

static inline struct __actvret_hmtracecall_hmtrace_ipc_buf_open* actvhdlr_hmtracehandler_hmtrace_ipc_buf_open_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_ipc_buf_open));
	return (struct __actvret_hmtracecall_hmtrace_ipc_buf_open*)__ret;
}

static inline struct __actvret_hmtracecall_hmtrace_ipc_buf_read* actvhdlr_hmtracehandler_hmtrace_ipc_buf_read_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_ipc_buf_read));
	return (struct __actvret_hmtracecall_hmtrace_ipc_buf_read*)__ret;
}

static inline struct __actvret_hmtracecall_hmtrace_ipc_buf_close* actvhdlr_hmtracehandler_hmtrace_ipc_buf_close_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_ipc_buf_close));
	return (struct __actvret_hmtracecall_hmtrace_ipc_buf_close*)__ret;
}

static inline struct __actvret_hmtracecall_hmtrace_get_event_tracing_eid* actvhdlr_hmtracehandler_hmtrace_get_event_tracing_eid_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_get_event_tracing_eid));
	return (struct __actvret_hmtracecall_hmtrace_get_event_tracing_eid*)__ret;
}

static inline struct __actvret_hmtracecall_hmtrace_realloc_trace_buffer* actvhdlr_hmtracehandler_hmtrace_realloc_trace_buffer_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_realloc_trace_buffer));
	return (struct __actvret_hmtracecall_hmtrace_realloc_trace_buffer*)__ret;
}

static inline struct __actvret_hmtracecall_hmtrace_trace_buffer_size* actvhdlr_hmtracehandler_hmtrace_trace_buffer_size_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_hmtracecall_hmtrace_trace_buffer_size));
	return (struct __actvret_hmtracecall_hmtrace_trace_buffer_size*)__ret;
}

#ifdef __sysif_server_hmtracemgr_no_compat32_handlers__
#ifdef __HOST_LLT__
union hmtracemgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list)(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_register_event)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_unregister_event)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on)(unsigned long long sender, unsigned long credential, unsigned int status);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int status);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open)(unsigned long long sender, unsigned long credential, struct hmtrace_data ipc_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data ipc_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read)(unsigned long long sender, unsigned long credential, int handle, unsigned long long key);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle, unsigned long long key);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close)(unsigned long long sender, unsigned long credential, int handle);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long buffer_size);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long buffer_size);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union hmtracemgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list)(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_register_event)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_unregister_event)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on)(unsigned long long sender, unsigned long credential, unsigned int status);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int status);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open)(unsigned long long sender, unsigned long credential, struct hmtrace_data ipc_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data ipc_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read)(unsigned long long sender, unsigned long credential, int handle, unsigned long long key);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle, unsigned long long key);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close)(unsigned long long sender, unsigned long credential, int handle);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long buffer_size);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long buffer_size);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#ifdef __arm__
union hmtracemgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list)(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_register_event)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_unregister_event)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on)(unsigned long long sender, unsigned long credential, unsigned int status);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int status);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open)(unsigned long long sender, unsigned long credential, struct hmtrace_data ipc_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data ipc_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read)(unsigned long long sender, unsigned long credential, int handle, unsigned long long key);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle, unsigned long long key);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close)(unsigned long long sender, unsigned long credential, int handle);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long buffer_size);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long buffer_size);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential);
};

#endif /* arm */
#endif /* not HOST_LLT */
#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif /* not __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#ifdef __HOST_LLT__
union hmtracemgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list)(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_register_event)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_unregister_event)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on)(unsigned long long sender, unsigned long credential, unsigned int status);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int status);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open)(unsigned long long sender, unsigned long credential, struct hmtrace_data ipc_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data ipc_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read)(unsigned long long sender, unsigned long credential, int handle, unsigned long long key);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle, unsigned long long key);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close)(unsigned long long sender, unsigned long credential, int handle);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long buffer_size);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long buffer_size);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_registered_class_list)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_registered_class_list)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_registered_events_list)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_registered_events_list)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_available_events_list)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_available_events_list)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_register_event)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_register_event)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_unregister_event)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_unregister_event)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_set_event_tracing_on)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_set_event_tracing_on)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_set_event_tracing_off)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_set_event_tracing_off)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_events_tracing_on)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_events_tracing_on)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_event_tracing_status)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_event_tracing_status)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_rb_info)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_rb_info)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_open)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_open)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_close)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_close)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_event_tracing_eid)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_event_tracing_eid)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_realloc_trace_buffer)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_realloc_trace_buffer)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_trace_buffer_size)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_trace_buffer_size)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union hmtracemgr_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list)(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_register_event)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_unregister_event)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on)(unsigned long long sender, unsigned long credential, unsigned int status);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int status);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open)(unsigned long long sender, unsigned long credential, struct hmtrace_data ipc_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data ipc_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read)(unsigned long long sender, unsigned long credential, int handle, unsigned long long key);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle, unsigned long long key);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close)(unsigned long long sender, unsigned long credential, int handle);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long buffer_size);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long buffer_size);
	int (*p__actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size)(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data);
	int (*p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_registered_class_list)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_registered_class_list)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_registered_events_list)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_registered_events_list)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_available_events_list)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_available_events_list)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_register_event)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_register_event)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_unregister_event)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_unregister_event)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_set_event_tracing_on)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_set_event_tracing_on)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_set_event_tracing_off)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_set_event_tracing_off)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_events_tracing_on)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_events_tracing_on)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_event_tracing_status)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_event_tracing_status)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_rb_info)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_rb_info)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_open)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_open)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_read)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_close)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_close)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_event_tracing_eid)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_event_tracing_eid)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_realloc_trace_buffer)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_realloc_trace_buffer)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_hmtracehandler_hmtrace_trace_buffer_size)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_trace_buffer_size)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#endif /* not HOST_LLT */
#endif /* compat32 */

#if defined(__sysif_server_hmtracemgr_no_compat32_handlers__) && defined(__sysif_server_hmtracemgr_no_ilp32_handlers__)
#define __hmtracemgr_method_TIMES 1
#elif !defined(__sysif_server_hmtracemgr_no_compat32_handlers__) && !defined(__sysif_server_hmtracemgr_no_ilp32_handlers__)
#define __hmtracemgr_method_TIMES 3
#else
#define __hmtracemgr_method_TIMES 2
#endif
extern union hmtracemgr_hdlrs_union sysif_hmtracemgr_actvhdlr_table[__hmtracemgr_method_MAX * __hmtracemgr_method_TIMES];

extern union hmtracemgr_hdlrs_union sysif_hmtracemgr_actvcapcallhdlr_table[__hmtracemgr_method_MAX * __hmtracemgr_method_TIMES];

extern void sysif_show_hmtracemgr_methods(void);


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
static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_registered_class_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_registered_class_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_registered_events_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_registered_events_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_available_events_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_available_events_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_register_event(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_register_event(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_register_event(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_unregister_event(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_unregister_event(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_unregister_event(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_set_event_tracing_on(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_set_event_tracing_on(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_set_event_tracing_off(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_set_event_tracing_off(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on(unsigned long long sender, unsigned long credential, unsigned int status)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_events_tracing_on(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, status));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int status)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_events_tracing_on(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, status));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_event_tracing_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_event_tracing_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_rb_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_rb_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open(unsigned long long sender, unsigned long credential, struct hmtrace_data ipc_data)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_open(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, ipc_data));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data ipc_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_open(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, ipc_data));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read(unsigned long long sender, unsigned long credential, int handle, unsigned long long key)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, handle), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle, unsigned long long key)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, handle), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close(unsigned long long sender, unsigned long credential, int handle)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_close(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, handle));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_close(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, handle));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_event_tracing_eid(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_event_tracing_eid(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long buffer_size)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_realloc_trace_buffer(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, buffer_size));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long buffer_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_realloc_trace_buffer(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, buffer_size));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_trace_buffer_size(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_hmtracehandler_hmtrace_trace_buffer_size(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data));
}

#ifndef __sysif_server_hmtracemgr_no_compat32_handlers__
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
static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_get_registered_class_list(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct bunch_ipc_attr);
		type = __type_of(struct bunch_ipc_attr);
		__fetch_one_arg((void*)&_attr, data, 0, arg_sz, model, type, struct bunch_ipc_attr, _attr)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_registered_class_list(sender, credential, _attr);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_registered_class_list(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct bunch_ipc_attr);
		type = __type_of(struct bunch_ipc_attr);
		__fetch_one_arg((void*)&_attr, data, 0, arg_sz, model, type, struct bunch_ipc_attr, _attr)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_registered_class_list(sender, credential, _attr);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_get_registered_events_list(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_registered_events_list(sender, credential, _class_data, _attr);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_registered_events_list(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_registered_events_list(sender, credential, _class_data, _attr);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_get_available_events_list(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_available_events_list(sender, credential, _class_data, _attr);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_available_events_list(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_available_events_list(sender, credential, _class_data, _attr);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_register_event(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	unsigned long long __maybe_unused _key;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_key, data, 1, arg_sz, model, type, unsigned long long, _key)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_register_event(sender, credential, _class_data, _key);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_register_event(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	unsigned long long __maybe_unused _key;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_key, data, 1, arg_sz, model, type, unsigned long long, _key)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_register_event(sender, credential, _class_data, _key);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_unregister_event(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	unsigned long long __maybe_unused _key;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_key, data, 1, arg_sz, model, type, unsigned long long, _key)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_unregister_event(sender, credential, _class_data, _key);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_unregister_event(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	unsigned long long __maybe_unused _key;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_key, data, 1, arg_sz, model, type, unsigned long long, _key)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_unregister_event(sender, credential, _class_data, _key);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_set_event_tracing_on(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	struct hmtrace_eventname __maybe_unused _event_data;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_eventname);
		type = __type_of(struct hmtrace_eventname);
		__fetch_one_arg((void*)&_event_data, data, 1, arg_sz, model, type, struct hmtrace_eventname, _event_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_set_event_tracing_on(sender, credential, _class_data, _event_data);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_set_event_tracing_on(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	struct hmtrace_eventname __maybe_unused _event_data;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_eventname);
		type = __type_of(struct hmtrace_eventname);
		__fetch_one_arg((void*)&_event_data, data, 1, arg_sz, model, type, struct hmtrace_eventname, _event_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_set_event_tracing_on(sender, credential, _class_data, _event_data);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_set_event_tracing_off(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	struct hmtrace_eventname __maybe_unused _event_data;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_eventname);
		type = __type_of(struct hmtrace_eventname);
		__fetch_one_arg((void*)&_event_data, data, 1, arg_sz, model, type, struct hmtrace_eventname, _event_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_set_event_tracing_off(sender, credential, _class_data, _event_data);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_set_event_tracing_off(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	struct hmtrace_eventname __maybe_unused _event_data;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_eventname);
		type = __type_of(struct hmtrace_eventname);
		__fetch_one_arg((void*)&_event_data, data, 1, arg_sz, model, type, struct hmtrace_eventname, _event_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_set_event_tracing_off(sender, credential, _class_data, _event_data);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_events_tracing_on(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _status;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_status, data, 0, arg_sz, model, type, unsigned int, _status)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_events_tracing_on(sender, credential, _status);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_events_tracing_on(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _status;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_status, data, 0, arg_sz, model, type, unsigned int, _status)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_events_tracing_on(sender, credential, _status);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_get_event_tracing_status(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	struct hmtrace_eventname __maybe_unused _event_data;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_eventname);
		type = __type_of(struct hmtrace_eventname);
		__fetch_one_arg((void*)&_event_data, data, 1, arg_sz, model, type, struct hmtrace_eventname, _event_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_event_tracing_status(sender, credential, _class_data, _event_data);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_event_tracing_status(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	struct hmtrace_eventname __maybe_unused _event_data;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_eventname);
		type = __type_of(struct hmtrace_eventname);
		__fetch_one_arg((void*)&_event_data, data, 1, arg_sz, model, type, struct hmtrace_eventname, _event_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_event_tracing_status(sender, credential, _class_data, _event_data);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_get_rb_info(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_rb_info(sender, credential, _class_data);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_rb_info(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_rb_info(sender, credential, _class_data);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_open(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _ipc_data;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_ipc_data, data, 0, arg_sz, model, type, struct hmtrace_data, _ipc_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_open(sender, credential, _ipc_data);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_open(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _ipc_data;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_ipc_data, data, 0, arg_sz, model, type, struct hmtrace_data, _ipc_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_open(sender, credential, _ipc_data);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_read(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _handle;
	unsigned long long __maybe_unused _key;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_handle, data, 0, arg_sz, model, type, int, _handle)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_key, data, 1, arg_sz, model, type, unsigned long long, _key)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_read(sender, credential, _handle, _key);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_read(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _handle;
	unsigned long long __maybe_unused _key;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_handle, data, 0, arg_sz, model, type, int, _handle)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long long);
		type = __type_of(unsigned long long);
		__fetch_one_arg((void*)&_key, data, 1, arg_sz, model, type, unsigned long long, _key)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_read(sender, credential, _handle, _key);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_close(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _handle;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_handle, data, 0, arg_sz, model, type, int, _handle)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_close(sender, credential, _handle);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_close(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _handle;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_handle, data, 0, arg_sz, model, type, int, _handle)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_close(sender, credential, _handle);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_get_event_tracing_eid(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	struct hmtrace_eventname __maybe_unused _event_data;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_eventname);
		type = __type_of(struct hmtrace_eventname);
		__fetch_one_arg((void*)&_event_data, data, 1, arg_sz, model, type, struct hmtrace_eventname, _event_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_event_tracing_eid(sender, credential, _class_data, _event_data);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_event_tracing_eid(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	struct hmtrace_eventname __maybe_unused _event_data;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_eventname);
		type = __type_of(struct hmtrace_eventname);
		__fetch_one_arg((void*)&_event_data, data, 1, arg_sz, model, type, struct hmtrace_eventname, _event_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_event_tracing_eid(sender, credential, _class_data, _event_data);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_realloc_trace_buffer(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	unsigned long __maybe_unused _buffer_size;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long);
		type = __type_of(unsigned long);
		__fetch_one_arg((void*)&_buffer_size, data, 1, arg_sz, model, type, unsigned long, _buffer_size)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_realloc_trace_buffer(sender, credential, _class_data, _buffer_size);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_realloc_trace_buffer(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	unsigned long __maybe_unused _buffer_size;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned long);
		type = __type_of(unsigned long);
		__fetch_one_arg((void*)&_buffer_size, data, 1, arg_sz, model, type, unsigned long, _buffer_size)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_realloc_trace_buffer(sender, credential, _class_data, _buffer_size);
}

static int __opt_nonoframe __actvbridgehdlr_compat_hmtracehandler_hmtrace_trace_buffer_size(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_trace_buffer_size(sender, credential, _class_data);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_trace_buffer_size(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct hmtrace_data __maybe_unused _class_data;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct hmtrace_data);
		type = __type_of(struct hmtrace_data);
		__fetch_one_arg((void*)&_class_data, data, 0, arg_sz, model, type, struct hmtrace_data, _class_data)
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
	return __actvshadowhdlr_hmtracehandler_hmtrace_trace_buffer_size(sender, credential, _class_data);
}

#endif /* compat handlers */
#ifndef __sysif_server_hmtracemgr_no_ilp32_handlers__
static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_registered_class_list(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_registered_class_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_registered_class_list(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_registered_class_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_registered_events_list(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_registered_events_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_registered_events_list(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_registered_events_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_available_events_list(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_available_events_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_available_events_list(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_available_events_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_register_event(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, key)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_register_event(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_register_event(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, key)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_register_event(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_unregister_event(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, key)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_unregister_event(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_unregister_event(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, key)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_unregister_event(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_set_event_tracing_on(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(struct hmtrace_eventname, event_data)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_set_event_tracing_on(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_set_event_tracing_on(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(struct hmtrace_eventname, event_data)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_set_event_tracing_on(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_set_event_tracing_off(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(struct hmtrace_eventname, event_data)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_set_event_tracing_off(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_set_event_tracing_off(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(struct hmtrace_eventname, event_data)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_set_event_tracing_off(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_events_tracing_on(unsigned long long sender, unsigned long credential, unsigned int status)
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
		__adjust_one_arg(unsigned int, status)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_events_tracing_on(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, status));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_events_tracing_on(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int status)
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
		__adjust_one_arg(unsigned int, status)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_events_tracing_on(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, status));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_event_tracing_status(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(struct hmtrace_eventname, event_data)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_event_tracing_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_event_tracing_status(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(struct hmtrace_eventname, event_data)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_event_tracing_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_rb_info(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_rb_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_rb_info(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_rb_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_open(unsigned long long sender, unsigned long credential, struct hmtrace_data ipc_data)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, ipc_data)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_open(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, ipc_data));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_open(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data ipc_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, ipc_data)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_open(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, ipc_data));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_read(unsigned long long sender, unsigned long credential, int handle, unsigned long long key)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, handle)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, key)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, handle), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_read(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle, unsigned long long key)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, handle)
	} while (0);
	do {
		__adjust_one_arg(unsigned long long, key)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, handle), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_close(unsigned long long sender, unsigned long credential, int handle)
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
		__adjust_one_arg(int, handle)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_close(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, handle));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_close(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle)
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
		__adjust_one_arg(int, handle)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_ipc_buf_close(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, handle));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_event_tracing_eid(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(struct hmtrace_eventname, event_data)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_event_tracing_eid(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_event_tracing_eid(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(struct hmtrace_eventname, event_data)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_get_event_tracing_eid(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_realloc_trace_buffer(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long buffer_size)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, buffer_size)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_realloc_trace_buffer(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, buffer_size));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_realloc_trace_buffer(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long buffer_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	do {
		__adjust_one_arg(unsigned long, buffer_size)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_realloc_trace_buffer(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, buffer_size));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_hmtracehandler_hmtrace_trace_buffer_size(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_trace_buffer_size(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_trace_buffer_size(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct hmtrace_data, class_data)
	} while (0);
	return __actvshadowhdlr_hmtracehandler_hmtrace_trace_buffer_size(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data));
}

#endif
#else
static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_get_registered_class_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_get_registered_class_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_get_registered_events_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_get_registered_events_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_get_available_events_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_get_available_events_list(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_register_event(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_register_event(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_register_event(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_unregister_event(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long long key)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_unregister_event(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long long key)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_unregister_event(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_set_event_tracing_on(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_set_event_tracing_on(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_set_event_tracing_off(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_set_event_tracing_off(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on(unsigned long long sender, unsigned long credential, unsigned int status)
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
	return actvhdlr_hmtracehandler_hmtrace_events_tracing_on(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, status));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int status)
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
	return actvhdlr_hmtracehandler_hmtrace_events_tracing_on(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, status));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_get_event_tracing_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_get_event_tracing_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_get_rb_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_get_rb_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open(unsigned long long sender, unsigned long credential, struct hmtrace_data ipc_data)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_ipc_buf_open(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, ipc_data));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data ipc_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_ipc_buf_open(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, ipc_data));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read(unsigned long long sender, unsigned long credential, int handle, unsigned long long key)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_ipc_buf_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, handle), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle, unsigned long long key)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_ipc_buf_read(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, handle), __ARCH_ACTV_RPC_RECV_ARG(unsigned long long, key));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close(unsigned long long sender, unsigned long credential, int handle)
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
	return actvhdlr_hmtracehandler_hmtrace_ipc_buf_close(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, handle));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int handle)
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
	return actvhdlr_hmtracehandler_hmtrace_ipc_buf_close(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, handle));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_get_event_tracing_eid(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_eventname), __arch_actv_rpc_stksz_alignment(struct hmtrace_eventname));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_get_event_tracing_eid(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_eventname, event_data));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data, unsigned long buffer_size)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_realloc_trace_buffer(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, buffer_size));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data, unsigned long buffer_size)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_realloc_trace_buffer(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data), __ARCH_ACTV_RPC_RECV_ARG(unsigned long, buffer_size));
}

static int __opt_nonoframe __actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size(unsigned long long sender, unsigned long credential, struct hmtrace_data class_data)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_trace_buffer_size(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct hmtrace_data class_data)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hmtrace_data), __arch_actv_rpc_stksz_alignment(struct hmtrace_data));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_hmtracehandler_hmtrace_trace_buffer_size(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct hmtrace_data, class_data));
}

#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#if defined(__sysif_server_hmtracemgr_no_compat32_handlers__) && defined(__sysif_server_hmtracemgr_no_ilp32_handlers__)
__ro_after_init_hdlr union hmtracemgr_hdlrs_union sysif_hmtracemgr_actvhdlr_table[__hmtracemgr_method_MAX * __hmtracemgr_method_TIMES] = {
	[0 ... __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[ __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list)
	},
	[ __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_register_event = (&__actvbridgehdlr_hmtracehandler_hmtrace_register_event)
	},
	[ __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_unregister_event = (&__actvbridgehdlr_hmtracehandler_hmtrace_unregister_event)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[ __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[ __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[ __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[ __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size)
	}
};

__ro_after_init_hdlr union hmtracemgr_hdlrs_union sysif_hmtracemgr_actvcapcallhdlr_table[__hmtracemgr_method_MAX * __hmtracemgr_method_TIMES] = {
	[0 ... __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[ __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list)
	},
	[ __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event)
	},
	[ __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[ __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[ __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[ __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[ __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size)
	}
};

#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif
#if !defined(__sysif_server_hmtracemgr_no_compat32_handlers__) && defined(__sysif_server_hmtracemgr_no_ilp32_handlers__)
__ro_after_init_hdlr union hmtracemgr_hdlrs_union sysif_hmtracemgr_actvhdlr_table[__hmtracemgr_method_MAX * __hmtracemgr_method_TIMES] = {
	[0 ... __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[ __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list)
	},
	[ __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_register_event = (&__actvbridgehdlr_hmtracehandler_hmtrace_register_event)
	},
	[ __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_unregister_event = (&__actvbridgehdlr_hmtracehandler_hmtrace_unregister_event)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[ __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[ __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[ __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[ __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size)
	},
	[__hmtracemgr_method_MAX ... __hmtracemgr_method_MAX + __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_available_events_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_register_event = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_register_event)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_unregister_event = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_unregister_event)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_events_tracing_on)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_rb_info)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_trace_buffer_size)
	}
};

__ro_after_init_hdlr union hmtracemgr_hdlrs_union sysif_hmtracemgr_actvcapcallhdlr_table[__hmtracemgr_method_MAX * __hmtracemgr_method_TIMES] = {
	[0 ... __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[ __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list)
	},
	[ __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event)
	},
	[ __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[ __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[ __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[ __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[ __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size)
	},
	[__hmtracemgr_method_MAX ... __hmtracemgr_method_MAX + __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_available_events_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_register_event = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_register_event)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_unregister_event = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_unregister_event)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_events_tracing_on)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_rb_info)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_trace_buffer_size)
	}
};

#endif /* aarch64 + compat */
#if defined(__sysif_server_hmtracemgr_no_compat32_handlers__) && !defined(__sysif_server_hmtracemgr_no_ilp32_handlers__)
__ro_after_init_hdlr union hmtracemgr_hdlrs_union sysif_hmtracemgr_actvhdlr_table[__hmtracemgr_method_MAX * __hmtracemgr_method_TIMES] = {
	[0 ... __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[ __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list)
	},
	[ __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_register_event = (&__actvbridgehdlr_hmtracehandler_hmtrace_register_event)
	},
	[ __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_unregister_event = (&__actvbridgehdlr_hmtracehandler_hmtrace_unregister_event)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[ __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[ __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[ __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[ __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size)
	},
	[__hmtracemgr_method_MAX ... __hmtracemgr_method_MAX + __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_available_events_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_register_event = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_register_event)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_unregister_event = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_unregister_event)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_events_tracing_on)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_rb_info)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_trace_buffer_size)
	}
};

__ro_after_init_hdlr union hmtracemgr_hdlrs_union sysif_hmtracemgr_actvcapcallhdlr_table[__hmtracemgr_method_MAX * __hmtracemgr_method_TIMES] = {
	[0 ... __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[ __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list)
	},
	[ __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event)
	},
	[ __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[ __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[ __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[ __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[ __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size)
	},
	[__hmtracemgr_method_MAX ... __hmtracemgr_method_MAX + __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_available_events_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_register_event)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_unregister_event)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_events_tracing_on)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_rb_info)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_trace_buffer_size)
	}
};

#endif /* aarch64 + ilp32 */
#if !defined(__sysif_server_hmtracemgr_no_compat32_handlers__) && !defined(__sysif_server_hmtracemgr_no_ilp32_handlers__)
__ro_after_init_hdlr union hmtracemgr_hdlrs_union sysif_hmtracemgr_actvhdlr_table[__hmtracemgr_method_MAX * __hmtracemgr_method_TIMES] = {
	[0 ... __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[ __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list)
	},
	[ __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_register_event = (&__actvbridgehdlr_hmtracehandler_hmtrace_register_event)
	},
	[ __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_unregister_event = (&__actvbridgehdlr_hmtracehandler_hmtrace_unregister_event)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[ __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[ __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[ __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[ __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size)
	},
	[__hmtracemgr_method_MAX ... __hmtracemgr_method_MAX + __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_available_events_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_register_event = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_register_event)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_unregister_event = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_unregister_event)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_events_tracing_on)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_rb_info)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgehdlr_compat_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgehdlr_compat_hmtracehandler_hmtrace_trace_buffer_size)
	},
	[2 * __hmtracemgr_method_MAX ... 2 * __hmtracemgr_method_MAX + __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_available_events_list)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_register_event = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_register_event)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_unregister_event = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_unregister_event)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_events_tracing_on)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_rb_info)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgehdlr_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgehdlr_ilp32_hmtracehandler_hmtrace_trace_buffer_size)
	}
};

__ro_after_init_hdlr union hmtracemgr_hdlrs_union sysif_hmtracemgr_actvcapcallhdlr_table[__hmtracemgr_method_MAX * __hmtracemgr_method_TIMES] = {
	[0 ... __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[ __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[ __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list)
	},
	[ __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event)
	},
	[ __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[ __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[ __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[ __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[ __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[ __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[ __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size)
	},
	[__hmtracemgr_method_MAX ... __hmtracemgr_method_MAX + __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_available_events_list)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_register_event = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_register_event)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_unregister_event = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_unregister_event)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_events_tracing_on)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_rb_info)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[__hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgecapcallhdlr_compat_hmtracehandler_hmtrace_trace_buffer_size)
	},
	[2 * __hmtracemgr_method_MAX ... 2 * __hmtracemgr_method_MAX + __hmtracemgr_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_class_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_class_list = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_registered_class_list)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_registered_events_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_registered_events_list = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_registered_events_list)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_available_events_list] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_available_events_list = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_available_events_list)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_register_event] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_register_event = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_register_event)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_unregister_event] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_unregister_event = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_unregister_event)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_on] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_on = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_set_event_tracing_on)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_set_event_tracing_off] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_set_event_tracing_off = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_set_event_tracing_off)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_events_tracing_on] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_events_tracing_on = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_events_tracing_on)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_status] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_status = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_event_tracing_status)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_rb_info] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_rb_info = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_rb_info)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_open] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_open = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_open)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_read] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_read = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_read)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_ipc_buf_close] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_ipc_buf_close = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_ipc_buf_close)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_get_event_tracing_eid] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_get_event_tracing_eid = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_get_event_tracing_eid)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_realloc_trace_buffer] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_realloc_trace_buffer = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_realloc_trace_buffer)
	},
	[2 * __hmtracemgr_method_MAX + __hmtracemgr_method_hmtrace_trace_buffer_size] = {
		.p__actvbridgecapcallhdlr_hmtracehandler_hmtrace_trace_buffer_size = (&__actvbridgecapcallhdlr_ilp32_hmtracehandler_hmtrace_trace_buffer_size)
	}
};

#endif /* aarch64 + compat + ilp32 */
#endif /* arm or aarch64 only */
#ifdef __aarch64__
void sysif_show_hmtracemgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_registered_class_list", __hmtracemgr_method_hmtrace_get_registered_class_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_registered_class_list", __hmtracemgr_method_hmtrace_get_registered_class_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_registered_events_list", __hmtracemgr_method_hmtrace_get_registered_events_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_registered_events_list", __hmtracemgr_method_hmtrace_get_registered_events_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_available_events_list", __hmtracemgr_method_hmtrace_get_available_events_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_available_events_list", __hmtracemgr_method_hmtrace_get_available_events_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "register_event", __hmtracemgr_method_hmtrace_register_event);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "register_event", __hmtracemgr_method_hmtrace_register_event);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "unregister_event", __hmtracemgr_method_hmtrace_unregister_event);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "unregister_event", __hmtracemgr_method_hmtrace_unregister_event);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "set_event_tracing_on", __hmtracemgr_method_hmtrace_set_event_tracing_on);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "set_event_tracing_on", __hmtracemgr_method_hmtrace_set_event_tracing_on);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "set_event_tracing_off", __hmtracemgr_method_hmtrace_set_event_tracing_off);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "set_event_tracing_off", __hmtracemgr_method_hmtrace_set_event_tracing_off);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "events_tracing_on", __hmtracemgr_method_hmtrace_events_tracing_on);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "events_tracing_on", __hmtracemgr_method_hmtrace_events_tracing_on);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_event_tracing_status", __hmtracemgr_method_hmtrace_get_event_tracing_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_event_tracing_status", __hmtracemgr_method_hmtrace_get_event_tracing_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_rb_info", __hmtracemgr_method_hmtrace_get_rb_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_rb_info", __hmtracemgr_method_hmtrace_get_rb_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "ipc_buf_open", __hmtracemgr_method_hmtrace_ipc_buf_open);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "ipc_buf_open", __hmtracemgr_method_hmtrace_ipc_buf_open);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "ipc_buf_read", __hmtracemgr_method_hmtrace_ipc_buf_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "ipc_buf_read", __hmtracemgr_method_hmtrace_ipc_buf_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "ipc_buf_close", __hmtracemgr_method_hmtrace_ipc_buf_close);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "ipc_buf_close", __hmtracemgr_method_hmtrace_ipc_buf_close);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_event_tracing_eid", __hmtracemgr_method_hmtrace_get_event_tracing_eid);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_event_tracing_eid", __hmtracemgr_method_hmtrace_get_event_tracing_eid);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "realloc_trace_buffer", __hmtracemgr_method_hmtrace_realloc_trace_buffer);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "realloc_trace_buffer", __hmtracemgr_method_hmtrace_realloc_trace_buffer);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "trace_buffer_size", __hmtracemgr_method_hmtrace_trace_buffer_size);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "trace_buffer_size", __hmtracemgr_method_hmtrace_trace_buffer_size);
	hm_info("%s_%s, max method num: %d\n", "hmtracemgr", "method", __hmtracemgr_method_MAX);
}

#elif defined(__arm__)
void sysif_show_hmtracemgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_registered_class_list", __hmtracemgr_method_hmtrace_get_registered_class_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_registered_class_list", __hmtracemgr_method_hmtrace_get_registered_class_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_registered_events_list", __hmtracemgr_method_hmtrace_get_registered_events_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_registered_events_list", __hmtracemgr_method_hmtrace_get_registered_events_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_available_events_list", __hmtracemgr_method_hmtrace_get_available_events_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_available_events_list", __hmtracemgr_method_hmtrace_get_available_events_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "register_event", __hmtracemgr_method_hmtrace_register_event);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "register_event", __hmtracemgr_method_hmtrace_register_event);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "unregister_event", __hmtracemgr_method_hmtrace_unregister_event);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "unregister_event", __hmtracemgr_method_hmtrace_unregister_event);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "set_event_tracing_on", __hmtracemgr_method_hmtrace_set_event_tracing_on);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "set_event_tracing_on", __hmtracemgr_method_hmtrace_set_event_tracing_on);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "set_event_tracing_off", __hmtracemgr_method_hmtrace_set_event_tracing_off);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "set_event_tracing_off", __hmtracemgr_method_hmtrace_set_event_tracing_off);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "events_tracing_on", __hmtracemgr_method_hmtrace_events_tracing_on);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "events_tracing_on", __hmtracemgr_method_hmtrace_events_tracing_on);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_event_tracing_status", __hmtracemgr_method_hmtrace_get_event_tracing_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_event_tracing_status", __hmtracemgr_method_hmtrace_get_event_tracing_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_rb_info", __hmtracemgr_method_hmtrace_get_rb_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_rb_info", __hmtracemgr_method_hmtrace_get_rb_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "ipc_buf_open", __hmtracemgr_method_hmtrace_ipc_buf_open);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "ipc_buf_open", __hmtracemgr_method_hmtrace_ipc_buf_open);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "ipc_buf_read", __hmtracemgr_method_hmtrace_ipc_buf_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "ipc_buf_read", __hmtracemgr_method_hmtrace_ipc_buf_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "ipc_buf_close", __hmtracemgr_method_hmtrace_ipc_buf_close);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "ipc_buf_close", __hmtracemgr_method_hmtrace_ipc_buf_close);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_event_tracing_eid", __hmtracemgr_method_hmtrace_get_event_tracing_eid);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_event_tracing_eid", __hmtracemgr_method_hmtrace_get_event_tracing_eid);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "realloc_trace_buffer", __hmtracemgr_method_hmtrace_realloc_trace_buffer);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "realloc_trace_buffer", __hmtracemgr_method_hmtrace_realloc_trace_buffer);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "trace_buffer_size", __hmtracemgr_method_hmtrace_trace_buffer_size);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "trace_buffer_size", __hmtracemgr_method_hmtrace_trace_buffer_size);
	hm_info("%s_%s, max method num: %d\n", "hmtracemgr", "method", __hmtracemgr_method_MAX);
}

#else
void sysif_show_hmtracemgr_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_registered_class_list", __hmtracemgr_method_hmtrace_get_registered_class_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_registered_class_list", __hmtracemgr_method_hmtrace_get_registered_class_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_registered_events_list", __hmtracemgr_method_hmtrace_get_registered_events_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_registered_events_list", __hmtracemgr_method_hmtrace_get_registered_events_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_available_events_list", __hmtracemgr_method_hmtrace_get_available_events_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_available_events_list", __hmtracemgr_method_hmtrace_get_available_events_list);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "register_event", __hmtracemgr_method_hmtrace_register_event);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "register_event", __hmtracemgr_method_hmtrace_register_event);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "unregister_event", __hmtracemgr_method_hmtrace_unregister_event);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "unregister_event", __hmtracemgr_method_hmtrace_unregister_event);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "set_event_tracing_on", __hmtracemgr_method_hmtrace_set_event_tracing_on);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "set_event_tracing_on", __hmtracemgr_method_hmtrace_set_event_tracing_on);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "set_event_tracing_off", __hmtracemgr_method_hmtrace_set_event_tracing_off);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "set_event_tracing_off", __hmtracemgr_method_hmtrace_set_event_tracing_off);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "events_tracing_on", __hmtracemgr_method_hmtrace_events_tracing_on);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "events_tracing_on", __hmtracemgr_method_hmtrace_events_tracing_on);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_event_tracing_status", __hmtracemgr_method_hmtrace_get_event_tracing_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_event_tracing_status", __hmtracemgr_method_hmtrace_get_event_tracing_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_rb_info", __hmtracemgr_method_hmtrace_get_rb_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_rb_info", __hmtracemgr_method_hmtrace_get_rb_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "ipc_buf_open", __hmtracemgr_method_hmtrace_ipc_buf_open);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "ipc_buf_open", __hmtracemgr_method_hmtrace_ipc_buf_open);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "ipc_buf_read", __hmtracemgr_method_hmtrace_ipc_buf_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "ipc_buf_read", __hmtracemgr_method_hmtrace_ipc_buf_read);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "ipc_buf_close", __hmtracemgr_method_hmtrace_ipc_buf_close);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "ipc_buf_close", __hmtracemgr_method_hmtrace_ipc_buf_close);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "get_event_tracing_eid", __hmtracemgr_method_hmtrace_get_event_tracing_eid);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "get_event_tracing_eid", __hmtracemgr_method_hmtrace_get_event_tracing_eid);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "realloc_trace_buffer", __hmtracemgr_method_hmtrace_realloc_trace_buffer);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "realloc_trace_buffer", __hmtracemgr_method_hmtrace_realloc_trace_buffer);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "hmtracecall", "hmtrace", "trace_buffer_size", __hmtracemgr_method_hmtrace_trace_buffer_size);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "hmtracehandler", "hmtrace", "trace_buffer_size", __hmtracemgr_method_hmtrace_trace_buffer_size);
	hm_info("%s_%s, max method num: %d\n", "hmtracemgr", "method", __hmtracemgr_method_MAX);
}

#endif
#endif
#endif
