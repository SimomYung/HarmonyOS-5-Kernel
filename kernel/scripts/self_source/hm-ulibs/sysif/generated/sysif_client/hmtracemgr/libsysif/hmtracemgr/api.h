/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for hmtracemgr
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../ulibs/include/libsysif/hmtracemgr/api.sysif -I ../ulibs/include --api-dest ../image/usr/include/generated/sysif_client/hmtracemgr/libsysif/hmtracemgr/api.h --server-dest ../image/usr/include/generated/sysif_server/hmtracemgr/libsysif/hmtracemgr/server.h --dump-callno ../image/usr/include/generated/dump_info/hmtracemgr/libsysif/hmtracemgr/l2h_map.h --legacy-mode --no-selfcall
* Create: Tue Sep 16 04:31:10 2025
*/
#ifndef LIBSYSIF_HMTRACEMGR_API_H
#define LIBSYSIF_HMTRACEMGR_API_H
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

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_get_registered_class_list(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_get_registered_class_list(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_get_registered_class_list(unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr)
{
	int __ret;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcall_hmtracecall_hmtrace_get_registered_class_list(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_get_registered_class_list(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr)
{
	int __ret;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcapcall_hmtracecall_hmtrace_get_registered_class_list(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_get_registered_events_list(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_get_registered_events_list(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_get_registered_events_list(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcall_hmtracecall_hmtrace_get_registered_events_list(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_get_registered_events_list(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcapcall_hmtracecall_hmtrace_get_registered_events_list(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_get_available_events_list(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_get_available_events_list(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_get_available_events_list(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcall_hmtracecall_hmtrace_get_available_events_list(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_get_available_events_list(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct bunch_ipc_attr attr)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcapcall_hmtracecall_hmtrace_get_available_events_list(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_register_event(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_register_event(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_register_event(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, unsigned long long key)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	unsigned long long __maybe_unused __actv_local_var_key = key;
	__ret = ____actvcall_hmtracecall_hmtrace_register_event(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(key, __actv_local_var_key));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_register_event(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, unsigned long long key)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	unsigned long long __maybe_unused __actv_local_var_key = key;
	__ret = ____actvcapcall_hmtracecall_hmtrace_register_event(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(key, __actv_local_var_key));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_unregister_event(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_unregister_event(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_unregister_event(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, unsigned long long key)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	unsigned long long __maybe_unused __actv_local_var_key = key;
	__ret = ____actvcall_hmtracecall_hmtrace_unregister_event(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(key, __actv_local_var_key));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_unregister_event(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, unsigned long long key)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	unsigned long long __maybe_unused __actv_local_var_key = key;
	__ret = ____actvcapcall_hmtracecall_hmtrace_unregister_event(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(key, __actv_local_var_key));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_set_event_tracing_on(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_set_event_tracing_on(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_set_event_tracing_on(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	struct hmtrace_eventname __maybe_unused __actv_local_var_event_data = event_data;
	__ret = ____actvcall_hmtracecall_hmtrace_set_event_tracing_on(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(event_data, __actv_local_var_event_data));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_set_event_tracing_on(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	struct hmtrace_eventname __maybe_unused __actv_local_var_event_data = event_data;
	__ret = ____actvcapcall_hmtracecall_hmtrace_set_event_tracing_on(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(event_data, __actv_local_var_event_data));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_set_event_tracing_off(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_set_event_tracing_off(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_set_event_tracing_off(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	struct hmtrace_eventname __maybe_unused __actv_local_var_event_data = event_data;
	__ret = ____actvcall_hmtracecall_hmtrace_set_event_tracing_off(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(event_data, __actv_local_var_event_data));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_set_event_tracing_off(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	struct hmtrace_eventname __maybe_unused __actv_local_var_event_data = event_data;
	__ret = ____actvcapcall_hmtracecall_hmtrace_set_event_tracing_off(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(event_data, __actv_local_var_event_data));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_events_tracing_on(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_events_tracing_on(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_events_tracing_on(unsigned long long __rpc_info, void *__buf, unsigned int status)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_status = status;
	__ret = ____actvcall_hmtracecall_hmtrace_events_tracing_on(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(status, __actv_local_var_status));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_events_tracing_on(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int status)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_status = status;
	__ret = ____actvcapcall_hmtracecall_hmtrace_events_tracing_on(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(status, __actv_local_var_status));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_get_event_tracing_status(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_get_event_tracing_status(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_get_event_tracing_status(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	struct hmtrace_eventname __maybe_unused __actv_local_var_event_data = event_data;
	__ret = ____actvcall_hmtracecall_hmtrace_get_event_tracing_status(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(event_data, __actv_local_var_event_data));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_get_event_tracing_status(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	struct hmtrace_eventname __maybe_unused __actv_local_var_event_data = event_data;
	__ret = ____actvcapcall_hmtracecall_hmtrace_get_event_tracing_status(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(event_data, __actv_local_var_event_data));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_get_rb_info(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_get_rb_info(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_get_rb_info(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	__ret = ____actvcall_hmtracecall_hmtrace_get_rb_info(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_get_rb_info(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	__ret = ____actvcapcall_hmtracecall_hmtrace_get_rb_info(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_ipc_buf_open(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_ipc_buf_open(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_ipc_buf_open(unsigned long long __rpc_info, void *__buf, struct hmtrace_data ipc_data)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_ipc_data = ipc_data;
	__ret = ____actvcall_hmtracecall_hmtrace_ipc_buf_open(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ipc_data, __actv_local_var_ipc_data));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_ipc_buf_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data ipc_data)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_ipc_data = ipc_data;
	__ret = ____actvcapcall_hmtracecall_hmtrace_ipc_buf_open(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ipc_data, __actv_local_var_ipc_data));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_ipc_buf_read(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_ipc_buf_read(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_ipc_buf_read(unsigned long long __rpc_info, void *__buf, int handle, unsigned long long key)
{
	int __ret;
	int __maybe_unused __actv_local_var_handle = handle;
	unsigned long long __maybe_unused __actv_local_var_key = key;
	__ret = ____actvcall_hmtracecall_hmtrace_ipc_buf_read(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(handle, __actv_local_var_handle), __ARCH_ACTV_RPC_PASS_ARG(key, __actv_local_var_key));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_ipc_buf_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, int handle, unsigned long long key)
{
	int __ret;
	int __maybe_unused __actv_local_var_handle = handle;
	unsigned long long __maybe_unused __actv_local_var_key = key;
	__ret = ____actvcapcall_hmtracecall_hmtrace_ipc_buf_read(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(handle, __actv_local_var_handle), __ARCH_ACTV_RPC_PASS_ARG(key, __actv_local_var_key));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_ipc_buf_close(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_ipc_buf_close(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_ipc_buf_close(unsigned long long __rpc_info, void *__buf, int handle)
{
	int __ret;
	int __maybe_unused __actv_local_var_handle = handle;
	__ret = ____actvcall_hmtracecall_hmtrace_ipc_buf_close(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(handle, __actv_local_var_handle));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_ipc_buf_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, int handle)
{
	int __ret;
	int __maybe_unused __actv_local_var_handle = handle;
	__ret = ____actvcapcall_hmtracecall_hmtrace_ipc_buf_close(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(handle, __actv_local_var_handle));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_get_event_tracing_eid(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_get_event_tracing_eid(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_get_event_tracing_eid(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	struct hmtrace_eventname __maybe_unused __actv_local_var_event_data = event_data;
	__ret = ____actvcall_hmtracecall_hmtrace_get_event_tracing_eid(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(event_data, __actv_local_var_event_data));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_get_event_tracing_eid(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, struct hmtrace_eventname event_data)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	struct hmtrace_eventname __maybe_unused __actv_local_var_event_data = event_data;
	__ret = ____actvcapcall_hmtracecall_hmtrace_get_event_tracing_eid(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(event_data, __actv_local_var_event_data));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_realloc_trace_buffer(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_realloc_trace_buffer(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_realloc_trace_buffer(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, unsigned long buffer_size)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	unsigned long __maybe_unused __actv_local_var_buffer_size = buffer_size;
	__ret = ____actvcall_hmtracecall_hmtrace_realloc_trace_buffer(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(buffer_size, __actv_local_var_buffer_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_realloc_trace_buffer(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data, unsigned long buffer_size)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	unsigned long __maybe_unused __actv_local_var_buffer_size = buffer_size;
	__ret = ____actvcapcall_hmtracecall_hmtrace_realloc_trace_buffer(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data), __ARCH_ACTV_RPC_PASS_ARG(buffer_size, __actv_local_var_buffer_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_hmtracecall_hmtrace_trace_buffer_size(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_hmtracecall_hmtrace_trace_buffer_size(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_hmtracecall_hmtrace_trace_buffer_size(unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	__ret = ____actvcall_hmtracecall_hmtrace_trace_buffer_size(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data));
	return __ret;
}

int __internal_actvcall __actvcapcall_hmtracecall_hmtrace_trace_buffer_size(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hmtrace_data class_data)
{
	int __ret;
	struct hmtrace_data __maybe_unused __actv_local_var_class_data = class_data;
	__ret = ____actvcapcall_hmtracecall_hmtrace_trace_buffer_size(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(class_data, __actv_local_var_class_data));
	return __ret;
}

#endif
#endif
