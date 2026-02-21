/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for uvmm
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../ulibs/include/libsysif/uvmm/api.sysif -I ../ulibs/include --api-dest ../image/usr/include/generated/sysif_client/uvmm/libsysif/uvmm/api.h --server-dest ../image/usr/include/generated/sysif_server/uvmm/libsysif/uvmm/server.h --dump-callno ../image/usr/include/generated/dump_info/uvmm/libsysif/uvmm/l2h_map.h --legacy-mode
* Create: Tue Sep 16 14:00:36 2025
*/
#ifndef LIBSYSIF_UVMM_SERVER_H
#define LIBSYSIF_UVMM_SERVER_H
#ifndef LIBSYSIF_UVMM_COMMON_H
#define LIBSYSIF_UVMM_COMMON_H
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
#include <libbunch/bunch_ipc.h>


#include <limits.h>

#define CHN_NAME_MAX_LEN 32
#define VM_SHUTDOWN_REASON_MAX_LEN 64

#define VM_SHUTDOWN_TIMEOUT_NO_WAIT	UINT_MAX
#define VM_SHUTDOWN_MAX_TIMEOUT_MS	(120U * 1000) /* 120 sec */

enum vminfo_option {
	VMINFO_STATUS,
	VMINFO_MEM_REGION,
	VMINFO_IRQ,
	VMINFO_INVALID,
};

struct vm_attach_chn_arg_s {
	unsigned long long faddr;
	char name[CHN_NAME_MAX_LEN];
	unsigned int flags;
};

struct vm_chn_owner_info_s {
	pid_t pid;
	uid_t uid;
	gid_t gid;
};

struct vm_shutdown_arg_s {
	unsigned int timeout_ms;
	char reason[VM_SHUTDOWN_REASON_MAX_LEN];
};

struct vnotify_msg_s;
struct vnotify_resp_s;

raw_static_assert(sizeof(enum vminfo_option) <= 96,
                    uvmmcall_uvmm_vm_get_info_arg_opt_is_too_large);
raw_static_assert(sizeof(struct bunch_ipc_attr) <= 96,
                    uvmmcall_uvmm_vm_get_info_arg_attr_is_too_large);
raw_static_assert(sizeof(struct vm_attach_chn_arg_s) <= 96,
                    uvmmcall_uvmm_vm_process_attach_chn_arg_args_is_too_large);
raw_static_assert(sizeof(struct vm_shutdown_arg_s) <= 96,
                    uvmmcall_uvmm_vm_shutdown_arg_args_is_too_large);
raw_static_assert(sizeof(struct bunch_ipc_attr) <= 96,
                    uvmmcall_uvmm_vm_vnotify_chns_info_arg_attr_is_too_large);
raw_static_assert(sizeof(struct bunch_ipc_attr) <= 96,
                    uvmmcall_uvmm_vm_trace_stats_arg_attr_is_too_large);
struct __actvret_uvmmcall_uvmm_vm_shm_open {
	unsigned long long shm_id;
	unsigned long len;
	__u64 paddr;
};

struct __actvret_uvmmcall_uvmm_vm_shm_close {
};

struct __actvret_uvmmcall_uvmm_vm_get_info {
};

struct __actvret_uvmmcall_uvmm_vm_process_attach_chn {
};

struct __actvret_uvmmcall_uvmm_vm_process_delete_chn {
};

struct __actvret_uvmmcall_uvmm_vm_process_notify_chn {
};

struct __actvret_uvmmcall_uvmm_vm_process_inject_chn {
};

struct __actvret_uvmmcall_uvmm_vm_process_read_chn_status {
	unsigned int status;
};

struct __actvret_uvmmcall_uvmm_vnotify_attach_fd {
};

struct __actvret_uvmmcall_uvmm_handle_vm_process_exit {
};

struct __actvret_uvmmcall_uvmm_vm_pause {
};

struct __actvret_uvmmcall_uvmm_vm_unpause {
};

struct __actvret_uvmmcall_uvmm_vm_resume {
};

struct __actvret_uvmmcall_uvmm_vm_shutdown {
};

struct __actvret_uvmmcall_uvmm_vm_vnotify_chns_info {
};

struct __actvret_uvmmcall_uvmm_mclone_to_vm {
};

struct __actvret_uvmmcall_uvmm_vnotify_send_ctrl_msg {
};

struct __actvret_uvmmcall_uvmm_vm_process_query_chn_status {
	struct vm_chn_owner_info_s host_info;
	struct vm_chn_owner_info_s guest_info;
};

struct __actvret_uvmmcall_uvmm_vm_cpu_hotplug {
};

struct __actvret_uvmmcall_uvmm_vm_trace_stats {
};

struct __actvret_uvmmcall_uvmm_vnotify_update_futex {
};

struct __actvret_uvmmcall_uvmm_vm_set_trap_wfi {
};

struct __actvret_uvmmcall_uvmm_vm_set_trap_wfe {
};

struct __actvret_uvmmcall_uvmm_vm_set_tickless {
};

struct __actvret_uvmmcall_vsock_vm_rx {
};

#ifdef __aarch64__
union __actvret_uvmmcall {
	struct __actvret_uvmmcall_uvmm_vm_shm_open uvmm_vm_shm_open;
	struct __actvret_uvmmcall_uvmm_vm_shm_close uvmm_vm_shm_close;
	struct __actvret_uvmmcall_uvmm_vm_get_info uvmm_vm_get_info;
	struct __actvret_uvmmcall_uvmm_vm_process_attach_chn uvmm_vm_process_attach_chn;
	struct __actvret_uvmmcall_uvmm_vm_process_delete_chn uvmm_vm_process_delete_chn;
	struct __actvret_uvmmcall_uvmm_vm_process_notify_chn uvmm_vm_process_notify_chn;
	struct __actvret_uvmmcall_uvmm_vm_process_inject_chn uvmm_vm_process_inject_chn;
	struct __actvret_uvmmcall_uvmm_vm_process_read_chn_status uvmm_vm_process_read_chn_status;
	struct __actvret_uvmmcall_uvmm_vnotify_attach_fd uvmm_vnotify_attach_fd;
	struct __actvret_uvmmcall_uvmm_handle_vm_process_exit uvmm_handle_vm_process_exit;
	struct __actvret_uvmmcall_uvmm_vm_pause uvmm_vm_pause;
	struct __actvret_uvmmcall_uvmm_vm_unpause uvmm_vm_unpause;
	struct __actvret_uvmmcall_uvmm_vm_resume uvmm_vm_resume;
	struct __actvret_uvmmcall_uvmm_vm_shutdown uvmm_vm_shutdown;
	struct __actvret_uvmmcall_uvmm_vm_vnotify_chns_info uvmm_vm_vnotify_chns_info;
	struct __actvret_uvmmcall_uvmm_mclone_to_vm uvmm_mclone_to_vm;
	struct __actvret_uvmmcall_uvmm_vnotify_send_ctrl_msg uvmm_vnotify_send_ctrl_msg;
	struct __actvret_uvmmcall_uvmm_vm_process_query_chn_status uvmm_vm_process_query_chn_status;
	struct __actvret_uvmmcall_uvmm_vm_cpu_hotplug uvmm_vm_cpu_hotplug;
	struct __actvret_uvmmcall_uvmm_vm_trace_stats uvmm_vm_trace_stats;
	struct __actvret_uvmmcall_uvmm_vnotify_update_futex uvmm_vnotify_update_futex;
	struct __actvret_uvmmcall_uvmm_vm_set_trap_wfi uvmm_vm_set_trap_wfi;
	struct __actvret_uvmmcall_uvmm_vm_set_trap_wfe uvmm_vm_set_trap_wfe;
	struct __actvret_uvmmcall_uvmm_vm_set_tickless uvmm_vm_set_tickless;
	struct __actvret_uvmmcall_vsock_vm_rx vsock_vm_rx;
};

#elif defined(__arm__)
union __actvret_uvmmcall {
	struct __actvret_uvmmcall_uvmm_vm_shm_open uvmm_vm_shm_open;
	struct __actvret_uvmmcall_uvmm_vm_shm_close uvmm_vm_shm_close;
	struct __actvret_uvmmcall_uvmm_vm_get_info uvmm_vm_get_info;
	struct __actvret_uvmmcall_uvmm_vm_process_attach_chn uvmm_vm_process_attach_chn;
	struct __actvret_uvmmcall_uvmm_vm_process_delete_chn uvmm_vm_process_delete_chn;
	struct __actvret_uvmmcall_uvmm_vm_process_notify_chn uvmm_vm_process_notify_chn;
	struct __actvret_uvmmcall_uvmm_vm_process_inject_chn uvmm_vm_process_inject_chn;
	struct __actvret_uvmmcall_uvmm_vm_process_read_chn_status uvmm_vm_process_read_chn_status;
	struct __actvret_uvmmcall_uvmm_vnotify_attach_fd uvmm_vnotify_attach_fd;
	struct __actvret_uvmmcall_uvmm_handle_vm_process_exit uvmm_handle_vm_process_exit;
	struct __actvret_uvmmcall_uvmm_vm_pause uvmm_vm_pause;
	struct __actvret_uvmmcall_uvmm_vm_unpause uvmm_vm_unpause;
	struct __actvret_uvmmcall_uvmm_vm_resume uvmm_vm_resume;
	struct __actvret_uvmmcall_uvmm_vm_shutdown uvmm_vm_shutdown;
	struct __actvret_uvmmcall_uvmm_vm_vnotify_chns_info uvmm_vm_vnotify_chns_info;
	struct __actvret_uvmmcall_uvmm_mclone_to_vm uvmm_mclone_to_vm;
	struct __actvret_uvmmcall_uvmm_vnotify_send_ctrl_msg uvmm_vnotify_send_ctrl_msg;
	struct __actvret_uvmmcall_uvmm_vm_process_query_chn_status uvmm_vm_process_query_chn_status;
	struct __actvret_uvmmcall_uvmm_vm_cpu_hotplug uvmm_vm_cpu_hotplug;
	struct __actvret_uvmmcall_uvmm_vm_trace_stats uvmm_vm_trace_stats;
	struct __actvret_uvmmcall_uvmm_vnotify_update_futex uvmm_vnotify_update_futex;
	struct __actvret_uvmmcall_uvmm_vm_set_trap_wfi uvmm_vm_set_trap_wfi;
	struct __actvret_uvmmcall_uvmm_vm_set_trap_wfe uvmm_vm_set_trap_wfe;
	struct __actvret_uvmmcall_uvmm_vm_set_tickless uvmm_vm_set_tickless;
	struct __actvret_uvmmcall_vsock_vm_rx vsock_vm_rx;
};

#else
union __actvret_uvmmcall {
	struct __actvret_uvmmcall_uvmm_vm_shm_open uvmm_vm_shm_open;
	struct __actvret_uvmmcall_uvmm_vm_shm_close uvmm_vm_shm_close;
	struct __actvret_uvmmcall_uvmm_vm_get_info uvmm_vm_get_info;
	struct __actvret_uvmmcall_uvmm_vm_process_attach_chn uvmm_vm_process_attach_chn;
	struct __actvret_uvmmcall_uvmm_vm_process_delete_chn uvmm_vm_process_delete_chn;
	struct __actvret_uvmmcall_uvmm_vm_process_notify_chn uvmm_vm_process_notify_chn;
	struct __actvret_uvmmcall_uvmm_vm_process_inject_chn uvmm_vm_process_inject_chn;
	struct __actvret_uvmmcall_uvmm_vm_process_read_chn_status uvmm_vm_process_read_chn_status;
	struct __actvret_uvmmcall_uvmm_vnotify_attach_fd uvmm_vnotify_attach_fd;
	struct __actvret_uvmmcall_uvmm_handle_vm_process_exit uvmm_handle_vm_process_exit;
	struct __actvret_uvmmcall_uvmm_vm_pause uvmm_vm_pause;
	struct __actvret_uvmmcall_uvmm_vm_unpause uvmm_vm_unpause;
	struct __actvret_uvmmcall_uvmm_vm_resume uvmm_vm_resume;
	struct __actvret_uvmmcall_uvmm_vm_shutdown uvmm_vm_shutdown;
	struct __actvret_uvmmcall_uvmm_vm_vnotify_chns_info uvmm_vm_vnotify_chns_info;
	struct __actvret_uvmmcall_uvmm_mclone_to_vm uvmm_mclone_to_vm;
	struct __actvret_uvmmcall_uvmm_vnotify_send_ctrl_msg uvmm_vnotify_send_ctrl_msg;
	struct __actvret_uvmmcall_uvmm_vm_process_query_chn_status uvmm_vm_process_query_chn_status;
	struct __actvret_uvmmcall_uvmm_vm_cpu_hotplug uvmm_vm_cpu_hotplug;
	struct __actvret_uvmmcall_uvmm_vm_trace_stats uvmm_vm_trace_stats;
	struct __actvret_uvmmcall_uvmm_vnotify_update_futex uvmm_vnotify_update_futex;
	struct __actvret_uvmmcall_uvmm_vm_set_trap_wfi uvmm_vm_set_trap_wfi;
	struct __actvret_uvmmcall_uvmm_vm_set_trap_wfe uvmm_vm_set_trap_wfe;
	struct __actvret_uvmmcall_uvmm_vm_set_tickless uvmm_vm_set_tickless;
	struct __actvret_uvmmcall_vsock_vm_rx vsock_vm_rx;
};

#endif
enum __uvmm_mgr {
	__uvmm_INVAL_mgr=0,
	__uvmm_uvmm_mgr,
	__uvmm_vsock_mgr,
	__uvmm_MAX_mgr,
};

#define __uvmm_method_MIN __ACTV_HIGHTABLE_START
#define __uvmm_method_uvmm_vm_shm_open (__ACTV_HIGHTABLE_START + 1)
#define __uvmm_method_uvmm_vm_shm_close (__ACTV_HIGHTABLE_START + 2)
#define __uvmm_method_uvmm_vm_get_info (__ACTV_HIGHTABLE_START + 3)
#define __uvmm_method_uvmm_vm_process_attach_chn (__ACTV_HIGHTABLE_START + 4)
#define __uvmm_method_uvmm_vm_process_delete_chn (__ACTV_HIGHTABLE_START + 5)
#define __uvmm_method_uvmm_vm_process_notify_chn (__ACTV_HIGHTABLE_START + 6)
#define __uvmm_method_uvmm_vm_process_inject_chn (__ACTV_HIGHTABLE_START + 7)
#define __uvmm_method_uvmm_vm_process_read_chn_status (__ACTV_HIGHTABLE_START + 8)
#define __uvmm_method_uvmm_vnotify_attach_fd (__ACTV_HIGHTABLE_START + 9)
#define __uvmm_method_uvmm_handle_vm_process_exit (__ACTV_HIGHTABLE_START + 10)
#define __uvmm_method_uvmm_vm_pause (__ACTV_HIGHTABLE_START + 11)
#define __uvmm_method_uvmm_vm_unpause (__ACTV_HIGHTABLE_START + 12)
#define __uvmm_method_uvmm_vm_resume (__ACTV_HIGHTABLE_START + 13)
#define __uvmm_method_uvmm_vm_shutdown (__ACTV_HIGHTABLE_START + 14)
#define __uvmm_method_uvmm_vm_vnotify_chns_info (__ACTV_HIGHTABLE_START + 15)
#define __uvmm_method_uvmm_mclone_to_vm (__ACTV_HIGHTABLE_START + 16)
#define __uvmm_method_uvmm_vnotify_send_ctrl_msg (__ACTV_HIGHTABLE_START + 17)
#define __uvmm_method_uvmm_vm_process_query_chn_status (__ACTV_HIGHTABLE_START + 18)
#define __uvmm_method_uvmm_vm_cpu_hotplug (__ACTV_HIGHTABLE_START + 19)
#define __uvmm_method_uvmm_vm_trace_stats (__ACTV_HIGHTABLE_START + 20)
#define __uvmm_method_uvmm_vnotify_update_futex (__ACTV_HIGHTABLE_START + 21)
#define __uvmm_method_uvmm_vm_set_trap_wfi (__ACTV_HIGHTABLE_START + 22)
#define __uvmm_method_uvmm_vm_set_trap_wfe (__ACTV_HIGHTABLE_START + 23)
#define __uvmm_method_uvmm_vm_set_tickless (__ACTV_HIGHTABLE_START + 24)
#define __uvmm_method_uvmm_rsvd_8 (__ACTV_HIGHTABLE_START + 25)
#define __uvmm_method_uvmm_rsvd_7 (__ACTV_HIGHTABLE_START + 26)
#define __uvmm_method_uvmm_rsvd_6 (__ACTV_HIGHTABLE_START + 27)
#define __uvmm_method_uvmm_rsvd_5 (__ACTV_HIGHTABLE_START + 28)
#define __uvmm_method_uvmm_rsvd_4 (__ACTV_HIGHTABLE_START + 29)
#define __uvmm_method_uvmm_rsvd_3 (__ACTV_HIGHTABLE_START + 30)
#define __uvmm_method_uvmm_rsvd_2 (__ACTV_HIGHTABLE_START + 31)
#define __uvmm_method_uvmm_rsvd_1 (__ACTV_HIGHTABLE_START + 32)
#define __uvmm_method_vsock_vm_rx (__ACTV_HIGHTABLE_START + 33)
#define __uvmm_method_MAX (__ACTV_HIGHTABLE_START + 34)
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_shm_open) < 512,
            uvmmcall_uvmm_vm_shm_open_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_shm_close) < 512,
            uvmmcall_uvmm_vm_shm_close_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_get_info) < 512,
            uvmmcall_uvmm_vm_get_info_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_process_attach_chn) < 512,
            uvmmcall_uvmm_vm_process_attach_chn_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_process_delete_chn) < 512,
            uvmmcall_uvmm_vm_process_delete_chn_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_process_notify_chn) < 512,
            uvmmcall_uvmm_vm_process_notify_chn_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_process_inject_chn) < 512,
            uvmmcall_uvmm_vm_process_inject_chn_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_process_read_chn_status) < 512,
            uvmmcall_uvmm_vm_process_read_chn_status_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vnotify_attach_fd) < 512,
            uvmmcall_uvmm_vnotify_attach_fd_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_handle_vm_process_exit) < 512,
            uvmmcall_uvmm_handle_vm_process_exit_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_pause) < 512,
            uvmmcall_uvmm_vm_pause_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_unpause) < 512,
            uvmmcall_uvmm_vm_unpause_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_resume) < 512,
            uvmmcall_uvmm_vm_resume_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_shutdown) < 512,
            uvmmcall_uvmm_vm_shutdown_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_vnotify_chns_info) < 512,
            uvmmcall_uvmm_vm_vnotify_chns_info_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_mclone_to_vm) < 512,
            uvmmcall_uvmm_mclone_to_vm_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vnotify_send_ctrl_msg) < 512,
            uvmmcall_uvmm_vnotify_send_ctrl_msg_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_process_query_chn_status) < 512,
            uvmmcall_uvmm_vm_process_query_chn_status_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_cpu_hotplug) < 512,
            uvmmcall_uvmm_vm_cpu_hotplug_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_trace_stats) < 512,
            uvmmcall_uvmm_vm_trace_stats_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vnotify_update_futex) < 512,
            uvmmcall_uvmm_vnotify_update_futex_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_set_trap_wfi) < 512,
            uvmmcall_uvmm_vm_set_trap_wfi_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_set_trap_wfe) < 512,
            uvmmcall_uvmm_vm_set_trap_wfe_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_uvmm_vm_set_tickless) < 512,
            uvmmcall_uvmm_vm_set_tickless_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_uvmmcall_vsock_vm_rx) < 512,
            uvmmcall_vsock_vm_rx_too_many_actvret_data);
raw_static_assert(sizeof(union __actvret_uvmmcall) < 512,
        uvmmcall_too_many_actvret_data);
extern int __actvcall_uvmmcall_uvmm_vm_shm_open(unsigned long long __rpc_info, void *__buf, int shm_id, unsigned int flags, unsigned short acc_mode);


extern int __actvcapcall_uvmmcall_uvmm_vm_shm_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, int shm_id, unsigned int flags, unsigned short acc_mode);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_shm_open(bool is_xact_fwd, bool is_xact_allow_refwd, int shm_id, unsigned int flags, unsigned short acc_mode, struct __actvret_uvmmcall_uvmm_vm_shm_open *__ret)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_shm_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned short), __arch_actv_rpc_stksz_alignment(unsigned short));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_shm_open(rpc_info, __ret, shm_id, flags, acc_mode);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_shm_open(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int shm_id, unsigned int flags, unsigned short acc_mode, struct __actvret_uvmmcall_uvmm_vm_shm_open *__ret)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_shm_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned short), __arch_actv_rpc_stksz_alignment(unsigned short));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_shm_open(__rref, rpc_info, __ret, shm_id, flags, acc_mode);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_shm_open(int shm_id, unsigned int flags, unsigned short acc_mode, struct __actvret_uvmmcall_uvmm_vm_shm_open *__ret)
{
	return actvxactcall_uvmmcall_uvmm_vm_shm_open(false, false, shm_id, flags, acc_mode, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_shm_open(rref_t __rref, int shm_id, unsigned int flags, unsigned short acc_mode, struct __actvret_uvmmcall_uvmm_vm_shm_open *__ret)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_shm_open(false, false, __rref, shm_id, flags, acc_mode, __ret);
}

extern int __actvcall_uvmmcall_uvmm_vm_shm_close(unsigned long long __rpc_info, void *__buf, int shm_id);


extern int __actvcapcall_uvmmcall_uvmm_vm_shm_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, int shm_id);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_shm_close(bool is_xact_fwd, bool is_xact_allow_refwd, int shm_id)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_shm_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_shm_close(rpc_info, NULL, shm_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_shm_close(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int shm_id)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_shm_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_shm_close(__rref, rpc_info, NULL, shm_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_shm_close(int shm_id)
{
	return actvxactcall_uvmmcall_uvmm_vm_shm_close(false, false, shm_id);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_shm_close(rref_t __rref, int shm_id)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_shm_close(false, false, __rref, shm_id);
}

extern int __actvcall_uvmmcall_uvmm_vm_get_info(unsigned long long __rpc_info, void *__buf, enum vminfo_option opt, struct bunch_ipc_attr attr);


extern int __actvcapcall_uvmmcall_uvmm_vm_get_info(rref_t __rref, unsigned long long __rpc_info, void *__buf, enum vminfo_option opt, struct bunch_ipc_attr attr);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_get_info(bool is_xact_fwd, bool is_xact_allow_refwd, enum vminfo_option opt, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_get_info;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum vminfo_option), __arch_actv_rpc_stksz_alignment(enum vminfo_option));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_get_info(rpc_info, NULL, opt, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_get_info(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, enum vminfo_option opt, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_get_info;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum vminfo_option), __arch_actv_rpc_stksz_alignment(enum vminfo_option));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_get_info(__rref, rpc_info, NULL, opt, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_get_info(enum vminfo_option opt, struct bunch_ipc_attr attr)
{
	return actvxactcall_uvmmcall_uvmm_vm_get_info(false, false, opt, attr);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_get_info(rref_t __rref, enum vminfo_option opt, struct bunch_ipc_attr attr)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_get_info(false, false, __rref, opt, attr);
}

extern int __actvcall_uvmmcall_uvmm_vm_process_attach_chn(unsigned long long __rpc_info, void *__buf, struct vm_attach_chn_arg_s args);


extern int __actvcapcall_uvmmcall_uvmm_vm_process_attach_chn(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct vm_attach_chn_arg_s args);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_process_attach_chn(bool is_xact_fwd, bool is_xact_allow_refwd, struct vm_attach_chn_arg_s args)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_process_attach_chn;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_attach_chn_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_attach_chn_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_process_attach_chn(rpc_info, NULL, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_process_attach_chn(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct vm_attach_chn_arg_s args)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_process_attach_chn;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_attach_chn_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_attach_chn_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_process_attach_chn(__rref, rpc_info, NULL, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_process_attach_chn(struct vm_attach_chn_arg_s args)
{
	return actvxactcall_uvmmcall_uvmm_vm_process_attach_chn(false, false, args);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_process_attach_chn(rref_t __rref, struct vm_attach_chn_arg_s args)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_process_attach_chn(false, false, __rref, args);
}

extern int __actvcall_uvmmcall_uvmm_vm_process_delete_chn(unsigned long long __rpc_info, void *__buf, unsigned int chn_id, bool sync, __u64 ts);


extern int __actvcapcall_uvmmcall_uvmm_vm_process_delete_chn(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int chn_id, bool sync, __u64 ts);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_process_delete_chn(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int chn_id, bool sync, __u64 ts)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_process_delete_chn;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u64), __arch_actv_rpc_stksz_alignment(__u64));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_process_delete_chn(rpc_info, NULL, chn_id, sync, ts);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_process_delete_chn(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int chn_id, bool sync, __u64 ts)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_process_delete_chn;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u64), __arch_actv_rpc_stksz_alignment(__u64));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_process_delete_chn(__rref, rpc_info, NULL, chn_id, sync, ts);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_process_delete_chn(unsigned int chn_id, bool sync, __u64 ts)
{
	return actvxactcall_uvmmcall_uvmm_vm_process_delete_chn(false, false, chn_id, sync, ts);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_process_delete_chn(rref_t __rref, unsigned int chn_id, bool sync, __u64 ts)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_process_delete_chn(false, false, __rref, chn_id, sync, ts);
}

extern int __actvcall_uvmmcall_uvmm_vm_process_notify_chn(unsigned long long __rpc_info, void *__buf, unsigned int chn_id);


extern int __actvcapcall_uvmmcall_uvmm_vm_process_notify_chn(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int chn_id);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_process_notify_chn(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int chn_id)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_process_notify_chn;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_process_notify_chn(rpc_info, NULL, chn_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_process_notify_chn(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int chn_id)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_process_notify_chn;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_process_notify_chn(__rref, rpc_info, NULL, chn_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_process_notify_chn(unsigned int chn_id)
{
	return actvxactcall_uvmmcall_uvmm_vm_process_notify_chn(false, false, chn_id);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_process_notify_chn(rref_t __rref, unsigned int chn_id)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_process_notify_chn(false, false, __rref, chn_id);
}

extern int __actvcall_uvmmcall_uvmm_vm_process_inject_chn(unsigned long long __rpc_info, void *__buf, unsigned int chn_id);


extern int __actvcapcall_uvmmcall_uvmm_vm_process_inject_chn(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int chn_id);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_process_inject_chn(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int chn_id)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_process_inject_chn;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_process_inject_chn(rpc_info, NULL, chn_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_process_inject_chn(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int chn_id)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_process_inject_chn;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_process_inject_chn(__rref, rpc_info, NULL, chn_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_process_inject_chn(unsigned int chn_id)
{
	return actvxactcall_uvmmcall_uvmm_vm_process_inject_chn(false, false, chn_id);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_process_inject_chn(rref_t __rref, unsigned int chn_id)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_process_inject_chn(false, false, __rref, chn_id);
}

extern int __actvcall_uvmmcall_uvmm_vm_process_read_chn_status(unsigned long long __rpc_info, void *__buf, unsigned int chn_id);


extern int __actvcapcall_uvmmcall_uvmm_vm_process_read_chn_status(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int chn_id);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_process_read_chn_status(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int chn_id, struct __actvret_uvmmcall_uvmm_vm_process_read_chn_status *__ret)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_process_read_chn_status;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_process_read_chn_status(rpc_info, __ret, chn_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_process_read_chn_status(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int chn_id, struct __actvret_uvmmcall_uvmm_vm_process_read_chn_status *__ret)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_process_read_chn_status;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_process_read_chn_status(__rref, rpc_info, __ret, chn_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_process_read_chn_status(unsigned int chn_id, struct __actvret_uvmmcall_uvmm_vm_process_read_chn_status *__ret)
{
	return actvxactcall_uvmmcall_uvmm_vm_process_read_chn_status(false, false, chn_id, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_process_read_chn_status(rref_t __rref, unsigned int chn_id, struct __actvret_uvmmcall_uvmm_vm_process_read_chn_status *__ret)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_process_read_chn_status(false, false, __rref, chn_id, __ret);
}

extern int __actvcall_uvmmcall_uvmm_vnotify_attach_fd(unsigned long long __rpc_info, void *__buf, unsigned int chn_id);


extern int __actvcapcall_uvmmcall_uvmm_vnotify_attach_fd(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int chn_id);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vnotify_attach_fd(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int chn_id)
{
	const unsigned long __callno = __uvmm_method_uvmm_vnotify_attach_fd;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vnotify_attach_fd(rpc_info, NULL, chn_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vnotify_attach_fd(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int chn_id)
{
	const unsigned long __callno = __uvmm_method_uvmm_vnotify_attach_fd;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vnotify_attach_fd(__rref, rpc_info, NULL, chn_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vnotify_attach_fd(unsigned int chn_id)
{
	return actvxactcall_uvmmcall_uvmm_vnotify_attach_fd(false, false, chn_id);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vnotify_attach_fd(rref_t __rref, unsigned int chn_id)
{
	return actvxactcapcall_uvmmcall_uvmm_vnotify_attach_fd(false, false, __rref, chn_id);
}

extern int __actvcall_uvmmcall_uvmm_handle_vm_process_exit(unsigned long long __rpc_info, void *__buf, __u32 cnode_idx);


extern int __actvcapcall_uvmmcall_uvmm_handle_vm_process_exit(rref_t __rref, unsigned long long __rpc_info, void *__buf, __u32 cnode_idx);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_handle_vm_process_exit(bool is_xact_fwd, bool is_xact_allow_refwd, __u32 cnode_idx)
{
	const unsigned long __callno = __uvmm_method_uvmm_handle_vm_process_exit;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u32), __arch_actv_rpc_stksz_alignment(__u32));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_handle_vm_process_exit(rpc_info, NULL, cnode_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_handle_vm_process_exit(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, __u32 cnode_idx)
{
	const unsigned long __callno = __uvmm_method_uvmm_handle_vm_process_exit;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u32), __arch_actv_rpc_stksz_alignment(__u32));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_handle_vm_process_exit(__rref, rpc_info, NULL, cnode_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_handle_vm_process_exit(__u32 cnode_idx)
{
	return actvxactcall_uvmmcall_uvmm_handle_vm_process_exit(false, false, cnode_idx);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_handle_vm_process_exit(rref_t __rref, __u32 cnode_idx)
{
	return actvxactcapcall_uvmmcall_uvmm_handle_vm_process_exit(false, false, __rref, cnode_idx);
}

extern int __actvcall_uvmmcall_uvmm_vm_pause(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_uvmmcall_uvmm_vm_pause(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_pause(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_pause;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_pause(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_pause(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_pause;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_pause(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_pause(void)
{
	return actvxactcall_uvmmcall_uvmm_vm_pause(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_pause(rref_t __rref)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_pause(false, false, __rref);
}

extern int __actvcall_uvmmcall_uvmm_vm_unpause(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_uvmmcall_uvmm_vm_unpause(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_unpause(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_unpause;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_unpause(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_unpause(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_unpause;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_unpause(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_unpause(void)
{
	return actvxactcall_uvmmcall_uvmm_vm_unpause(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_unpause(rref_t __rref)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_unpause(false, false, __rref);
}

extern int __actvcall_uvmmcall_uvmm_vm_resume(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_uvmmcall_uvmm_vm_resume(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_resume(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_resume;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_resume(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_resume(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_resume;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_resume(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_resume(void)
{
	return actvxactcall_uvmmcall_uvmm_vm_resume(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_resume(rref_t __rref)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_resume(false, false, __rref);
}

extern int __actvcall_uvmmcall_uvmm_vm_shutdown(unsigned long long __rpc_info, void *__buf, struct vm_shutdown_arg_s args);


extern int __actvcapcall_uvmmcall_uvmm_vm_shutdown(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct vm_shutdown_arg_s args);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_shutdown(bool is_xact_fwd, bool is_xact_allow_refwd, struct vm_shutdown_arg_s args)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_shutdown;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_shutdown_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_shutdown_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_shutdown(rpc_info, NULL, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_shutdown(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct vm_shutdown_arg_s args)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_shutdown;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_shutdown_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_shutdown_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_shutdown(__rref, rpc_info, NULL, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_shutdown(struct vm_shutdown_arg_s args)
{
	return actvxactcall_uvmmcall_uvmm_vm_shutdown(false, false, args);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_shutdown(rref_t __rref, struct vm_shutdown_arg_s args)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_shutdown(false, false, __rref, args);
}

extern int __actvcall_uvmmcall_uvmm_vm_vnotify_chns_info(unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr);


extern int __actvcapcall_uvmmcall_uvmm_vm_vnotify_chns_info(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_vnotify_chns_info(bool is_xact_fwd, bool is_xact_allow_refwd, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_vnotify_chns_info;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_vnotify_chns_info(rpc_info, NULL, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_vnotify_chns_info(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct bunch_ipc_attr attr)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_vnotify_chns_info;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_vnotify_chns_info(__rref, rpc_info, NULL, attr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_vnotify_chns_info(struct bunch_ipc_attr attr)
{
	return actvxactcall_uvmmcall_uvmm_vm_vnotify_chns_info(false, false, attr);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_vnotify_chns_info(rref_t __rref, struct bunch_ipc_attr attr)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_vnotify_chns_info(false, false, __rref, attr);
}

extern int __actvcall_uvmmcall_uvmm_mclone_to_vm(unsigned long long __rpc_info, void *__buf, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot);


extern int __actvcapcall_uvmmcall_uvmm_mclone_to_vm(rref_t __rref, unsigned long long __rpc_info, void *__buf, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_mclone_to_vm(bool is_xact_fwd, bool is_xact_allow_refwd, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot)
{
	const unsigned long __callno = __uvmm_method_uvmm_mclone_to_vm;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_mclone_to_vm(rpc_info, NULL, src_va, tgt_gpa, len, prot);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_mclone_to_vm(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot)
{
	const unsigned long __callno = __uvmm_method_uvmm_mclone_to_vm;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_mclone_to_vm(__rref, rpc_info, NULL, src_va, tgt_gpa, len, prot);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_mclone_to_vm(uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot)
{
	return actvxactcall_uvmmcall_uvmm_mclone_to_vm(false, false, src_va, tgt_gpa, len, prot);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_mclone_to_vm(rref_t __rref, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot)
{
	return actvxactcapcall_uvmmcall_uvmm_mclone_to_vm(false, false, __rref, src_va, tgt_gpa, len, prot);
}

extern int __actvcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __uvmm_method_uvmm_vnotify_send_ctrl_msg;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __uvmm_method_uvmm_vnotify_send_ctrl_msg;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(void)
{
	return actvxactcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(rref_t __rref)
{
	return actvxactcapcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(false, false, __rref);
}

extern int __actvcall_uvmmcall_uvmm_vm_process_query_chn_status(unsigned long long __rpc_info, void *__buf, unsigned int chn_id, unsigned int query_type);


extern int __actvcapcall_uvmmcall_uvmm_vm_process_query_chn_status(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int chn_id, unsigned int query_type);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_process_query_chn_status(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int chn_id, unsigned int query_type, struct __actvret_uvmmcall_uvmm_vm_process_query_chn_status *__ret)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_process_query_chn_status;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_process_query_chn_status(rpc_info, __ret, chn_id, query_type);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_process_query_chn_status(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int chn_id, unsigned int query_type, struct __actvret_uvmmcall_uvmm_vm_process_query_chn_status *__ret)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_process_query_chn_status;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_process_query_chn_status(__rref, rpc_info, __ret, chn_id, query_type);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_process_query_chn_status(unsigned int chn_id, unsigned int query_type, struct __actvret_uvmmcall_uvmm_vm_process_query_chn_status *__ret)
{
	return actvxactcall_uvmmcall_uvmm_vm_process_query_chn_status(false, false, chn_id, query_type, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_process_query_chn_status(rref_t __rref, unsigned int chn_id, unsigned int query_type, struct __actvret_uvmmcall_uvmm_vm_process_query_chn_status *__ret)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_process_query_chn_status(false, false, __rref, chn_id, query_type, __ret);
}

extern int __actvcall_uvmmcall_uvmm_vm_cpu_hotplug(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_uvmmcall_uvmm_vm_cpu_hotplug(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_cpu_hotplug(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_cpu_hotplug;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_cpu_hotplug(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_cpu_hotplug(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_cpu_hotplug;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_cpu_hotplug(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_cpu_hotplug(void)
{
	return actvxactcall_uvmmcall_uvmm_vm_cpu_hotplug(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_cpu_hotplug(rref_t __rref)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_cpu_hotplug(false, false, __rref);
}

extern int __actvcall_uvmmcall_uvmm_vm_trace_stats(unsigned long long __rpc_info, void *__buf, int act, struct bunch_ipc_attr attr, uint64_t args);


extern int __actvcapcall_uvmmcall_uvmm_vm_trace_stats(rref_t __rref, unsigned long long __rpc_info, void *__buf, int act, struct bunch_ipc_attr attr, uint64_t args);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_trace_stats(bool is_xact_fwd, bool is_xact_allow_refwd, int act, struct bunch_ipc_attr attr, uint64_t args)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_trace_stats;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_trace_stats(rpc_info, NULL, act, attr, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_trace_stats(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int act, struct bunch_ipc_attr attr, uint64_t args)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_trace_stats;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_trace_stats(__rref, rpc_info, NULL, act, attr, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_trace_stats(int act, struct bunch_ipc_attr attr, uint64_t args)
{
	return actvxactcall_uvmmcall_uvmm_vm_trace_stats(false, false, act, attr, args);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_trace_stats(rref_t __rref, int act, struct bunch_ipc_attr attr, uint64_t args)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_trace_stats(false, false, __rref, act, attr, args);
}

extern int __actvcall_uvmmcall_uvmm_vnotify_update_futex(unsigned long long __rpc_info, void *__buf, unsigned int chn_id, const int *futex, bool shared);


extern int __actvcapcall_uvmmcall_uvmm_vnotify_update_futex(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int chn_id, const int *futex, bool shared);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vnotify_update_futex(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int chn_id, const int *futex, bool shared)
{
	const unsigned long __callno = __uvmm_method_uvmm_vnotify_update_futex;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const int*), __arch_actv_rpc_stksz_alignment(const int*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vnotify_update_futex(rpc_info, NULL, chn_id, futex, shared);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vnotify_update_futex(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int chn_id, const int *futex, bool shared)
{
	const unsigned long __callno = __uvmm_method_uvmm_vnotify_update_futex;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const int*), __arch_actv_rpc_stksz_alignment(const int*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vnotify_update_futex(__rref, rpc_info, NULL, chn_id, futex, shared);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vnotify_update_futex(unsigned int chn_id, const int *futex, bool shared)
{
	return actvxactcall_uvmmcall_uvmm_vnotify_update_futex(false, false, chn_id, futex, shared);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vnotify_update_futex(rref_t __rref, unsigned int chn_id, const int *futex, bool shared)
{
	return actvxactcapcall_uvmmcall_uvmm_vnotify_update_futex(false, false, __rref, chn_id, futex, shared);
}

extern int __actvcall_uvmmcall_uvmm_vm_set_trap_wfi(unsigned long long __rpc_info, void *__buf, uint64_t state);


extern int __actvcapcall_uvmmcall_uvmm_vm_set_trap_wfi(rref_t __rref, unsigned long long __rpc_info, void *__buf, uint64_t state);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_set_trap_wfi(bool is_xact_fwd, bool is_xact_allow_refwd, uint64_t state)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_set_trap_wfi;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_set_trap_wfi(rpc_info, NULL, state);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_set_trap_wfi(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, uint64_t state)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_set_trap_wfi;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_set_trap_wfi(__rref, rpc_info, NULL, state);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_set_trap_wfi(uint64_t state)
{
	return actvxactcall_uvmmcall_uvmm_vm_set_trap_wfi(false, false, state);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_set_trap_wfi(rref_t __rref, uint64_t state)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_set_trap_wfi(false, false, __rref, state);
}

extern int __actvcall_uvmmcall_uvmm_vm_set_trap_wfe(unsigned long long __rpc_info, void *__buf, uint64_t state);


extern int __actvcapcall_uvmmcall_uvmm_vm_set_trap_wfe(rref_t __rref, unsigned long long __rpc_info, void *__buf, uint64_t state);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_set_trap_wfe(bool is_xact_fwd, bool is_xact_allow_refwd, uint64_t state)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_set_trap_wfe;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_set_trap_wfe(rpc_info, NULL, state);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_set_trap_wfe(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, uint64_t state)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_set_trap_wfe;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_set_trap_wfe(__rref, rpc_info, NULL, state);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_set_trap_wfe(uint64_t state)
{
	return actvxactcall_uvmmcall_uvmm_vm_set_trap_wfe(false, false, state);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_set_trap_wfe(rref_t __rref, uint64_t state)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_set_trap_wfe(false, false, __rref, state);
}

extern int __actvcall_uvmmcall_uvmm_vm_set_tickless(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_uvmmcall_uvmm_vm_set_tickless(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_uvmm_vm_set_tickless(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_set_tickless;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_uvmm_vm_set_tickless(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_uvmm_vm_set_tickless(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __uvmm_method_uvmm_vm_set_tickless;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_uvmm_vm_set_tickless(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_uvmm_vm_set_tickless(void)
{
	return actvxactcall_uvmmcall_uvmm_vm_set_tickless(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_uvmm_vm_set_tickless(rref_t __rref)
{
	return actvxactcapcall_uvmmcall_uvmm_vm_set_tickless(false, false, __rref);
}

extern int __actvcall_uvmmcall_vsock_vm_rx(unsigned long long __rpc_info, void *__buf, const void *hdr, size_t hdr_len, const void *data, size_t len);


extern int __actvcapcall_uvmmcall_vsock_vm_rx(rref_t __rref, unsigned long long __rpc_info, void *__buf, const void *hdr, size_t hdr_len, const void *data, size_t len);


static inline int __attribute__((always_inline)) actvxactcall_uvmmcall_vsock_vm_rx(bool is_xact_fwd, bool is_xact_allow_refwd, const void *hdr, size_t hdr_len, const void *data, size_t len)
{
	const unsigned long __callno = __uvmm_method_vsock_vm_rx;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_uvmmcall_vsock_vm_rx(rpc_info, NULL, hdr, hdr_len, data, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_uvmmcall_vsock_vm_rx(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, const void *hdr, size_t hdr_len, const void *data, size_t len)
{
	const unsigned long __callno = __uvmm_method_vsock_vm_rx;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_uvmmcall_vsock_vm_rx(__rref, rpc_info, NULL, hdr, hdr_len, data, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_uvmmcall_vsock_vm_rx(const void *hdr, size_t hdr_len, const void *data, size_t len)
{
	return actvxactcall_uvmmcall_vsock_vm_rx(false, false, hdr, hdr_len, data, len);
}

static inline int __attribute__((always_inline)) actvcapcall_uvmmcall_vsock_vm_rx(rref_t __rref, const void *hdr, size_t hdr_len, const void *data, size_t len)
{
	return actvxactcapcall_uvmmcall_vsock_vm_rx(false, false, __rref, hdr, hdr_len, data, len);
}

#endif
#include <libsysif/utils.h>
#include <hmasm/actv_rpc.h>
#include <hmasm/types.h>
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
extern int actvhdlr_uvmmhandler_uvmm_vm_shm_open(unsigned long long sender, unsigned long credential, int shm_id, unsigned int flags, unsigned short acc_mode);


extern int actvhdlr_uvmmhandler_uvmm_vm_shm_close(unsigned long long sender, unsigned long credential, int shm_id);


extern int actvhdlr_uvmmhandler_uvmm_vm_get_info(unsigned long long sender, unsigned long credential, enum vminfo_option opt, struct bunch_ipc_attr attr);


extern int actvhdlr_uvmmhandler_uvmm_vm_process_attach_chn(unsigned long long sender, unsigned long credential, struct vm_attach_chn_arg_s args);


extern int actvhdlr_uvmmhandler_uvmm_vm_process_delete_chn(unsigned long long sender, unsigned long credential, unsigned int chn_id, bool sync, __u64 ts);


extern int actvhdlr_uvmmhandler_uvmm_vm_process_notify_chn(unsigned long long sender, unsigned long credential, unsigned int chn_id);


extern int actvhdlr_uvmmhandler_uvmm_vm_process_inject_chn(unsigned long long sender, unsigned long credential, unsigned int chn_id);


extern int actvhdlr_uvmmhandler_uvmm_vm_process_read_chn_status(unsigned long long sender, unsigned long credential, unsigned int chn_id);


extern int actvhdlr_uvmmhandler_uvmm_vnotify_attach_fd(unsigned long long sender, unsigned long credential, unsigned int chn_id);


extern int actvhdlr_uvmmhandler_uvmm_handle_vm_process_exit(unsigned long long sender, unsigned long credential, __u32 cnode_idx);


extern int actvhdlr_uvmmhandler_uvmm_vm_pause(unsigned long long sender, unsigned long credential);


extern int actvhdlr_uvmmhandler_uvmm_vm_unpause(unsigned long long sender, unsigned long credential);


extern int actvhdlr_uvmmhandler_uvmm_vm_resume(unsigned long long sender, unsigned long credential);


extern int actvhdlr_uvmmhandler_uvmm_vm_shutdown(unsigned long long sender, unsigned long credential, struct vm_shutdown_arg_s args);


extern int actvhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);


extern int actvhdlr_uvmmhandler_uvmm_mclone_to_vm(unsigned long long sender, unsigned long credential, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot);


extern int actvhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg(unsigned long long sender, unsigned long credential);


extern int actvhdlr_uvmmhandler_uvmm_vm_process_query_chn_status(unsigned long long sender, unsigned long credential, unsigned int chn_id, unsigned int query_type);


extern int actvhdlr_uvmmhandler_uvmm_vm_cpu_hotplug(unsigned long long sender, unsigned long credential);


extern int actvhdlr_uvmmhandler_uvmm_vm_trace_stats(unsigned long long sender, unsigned long credential, int act, struct bunch_ipc_attr attr, uint64_t args);


extern int actvhdlr_uvmmhandler_uvmm_vnotify_update_futex(unsigned long long sender, unsigned long credential, unsigned int chn_id, const int *futex, bool shared);


extern int actvhdlr_uvmmhandler_uvmm_vm_set_trap_wfi(unsigned long long sender, unsigned long credential, uint64_t state);


extern int actvhdlr_uvmmhandler_uvmm_vm_set_trap_wfe(unsigned long long sender, unsigned long credential, uint64_t state);


extern int actvhdlr_uvmmhandler_uvmm_vm_set_tickless(unsigned long long sender, unsigned long credential);


extern int actvhdlr_uvmmhandler_vsock_vm_rx(unsigned long long sender, unsigned long credential, const void *hdr, size_t hdr_len, const void *data, size_t len);


#else /* aarch64 */
extern int actvhdlr_uvmmhandler_uvmm_vm_shm_open(unsigned long long sender, unsigned long credential, int shm_id, unsigned int flags, unsigned short acc_mode);


extern int actvhdlr_uvmmhandler_uvmm_vm_shm_close(unsigned long long sender, unsigned long credential, int shm_id);


extern int actvhdlr_uvmmhandler_uvmm_vm_get_info(unsigned long long sender, unsigned long credential, enum vminfo_option opt, struct bunch_ipc_attr attr);


extern int actvhdlr_uvmmhandler_uvmm_vm_process_attach_chn(unsigned long long sender, unsigned long credential, struct vm_attach_chn_arg_s args);


extern int actvhdlr_uvmmhandler_uvmm_vm_process_delete_chn(unsigned long long sender, unsigned long credential, unsigned int chn_id, bool sync, __u64 ts);


extern int actvhdlr_uvmmhandler_uvmm_vm_process_notify_chn(unsigned long long sender, unsigned long credential, unsigned int chn_id);


extern int actvhdlr_uvmmhandler_uvmm_vm_process_inject_chn(unsigned long long sender, unsigned long credential, unsigned int chn_id);


extern int actvhdlr_uvmmhandler_uvmm_vm_process_read_chn_status(unsigned long long sender, unsigned long credential, unsigned int chn_id);


extern int actvhdlr_uvmmhandler_uvmm_vnotify_attach_fd(unsigned long long sender, unsigned long credential, unsigned int chn_id);


extern int actvhdlr_uvmmhandler_uvmm_handle_vm_process_exit(unsigned long long sender, unsigned long credential, __u32 cnode_idx);


extern int actvhdlr_uvmmhandler_uvmm_vm_pause(unsigned long long sender, unsigned long credential);


extern int actvhdlr_uvmmhandler_uvmm_vm_unpause(unsigned long long sender, unsigned long credential);


extern int actvhdlr_uvmmhandler_uvmm_vm_resume(unsigned long long sender, unsigned long credential);


extern int actvhdlr_uvmmhandler_uvmm_vm_shutdown(unsigned long long sender, unsigned long credential, struct vm_shutdown_arg_s args);


extern int actvhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);


extern int actvhdlr_uvmmhandler_uvmm_mclone_to_vm(unsigned long long sender, unsigned long credential, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot);


extern int actvhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg(unsigned long long sender, unsigned long credential);


extern int actvhdlr_uvmmhandler_uvmm_vm_process_query_chn_status(unsigned long long sender, unsigned long credential, unsigned int chn_id, unsigned int query_type);


extern int actvhdlr_uvmmhandler_uvmm_vm_cpu_hotplug(unsigned long long sender, unsigned long credential);


extern int actvhdlr_uvmmhandler_uvmm_vm_trace_stats(unsigned long long sender, unsigned long credential, int act, struct bunch_ipc_attr attr, uint64_t args);


extern int actvhdlr_uvmmhandler_uvmm_vnotify_update_futex(unsigned long long sender, unsigned long credential, unsigned int chn_id, const int *futex, bool shared);


extern int actvhdlr_uvmmhandler_uvmm_vm_set_trap_wfi(unsigned long long sender, unsigned long credential, uint64_t state);


extern int actvhdlr_uvmmhandler_uvmm_vm_set_trap_wfe(unsigned long long sender, unsigned long credential, uint64_t state);


extern int actvhdlr_uvmmhandler_uvmm_vm_set_tickless(unsigned long long sender, unsigned long credential);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_shm_open(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_shm_close(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_get_info(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_process_attach_chn(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_process_delete_chn(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_process_notify_chn(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_process_inject_chn(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_process_read_chn_status(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vnotify_attach_fd(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_handle_vm_process_exit(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_pause(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_unpause(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_resume(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_shutdown(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_mclone_to_vm(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_process_query_chn_status(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_cpu_hotplug(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_trace_stats(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vnotify_update_futex(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_set_trap_wfi(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_set_trap_wfe(unsigned long long sender, unsigned long credential, ...);


extern int __actvshadowhdlr_uvmmhandler_uvmm_vm_set_tickless(unsigned long long sender, unsigned long credential, ...);


extern int actvhdlr_uvmmhandler_vsock_vm_rx(unsigned long long sender, unsigned long credential, const void *hdr, size_t hdr_len, const void *data, size_t len);


extern int __actvshadowhdlr_uvmmhandler_vsock_vm_rx(unsigned long long sender, unsigned long credential, ...);


#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
static inline struct __actvret_uvmmcall_uvmm_vm_shm_open* actvhdlr_uvmmhandler_uvmm_vm_shm_open_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_shm_open));
	return (struct __actvret_uvmmcall_uvmm_vm_shm_open*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_shm_close* actvhdlr_uvmmhandler_uvmm_vm_shm_close_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_shm_close));
	return (struct __actvret_uvmmcall_uvmm_vm_shm_close*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_get_info* actvhdlr_uvmmhandler_uvmm_vm_get_info_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_get_info));
	return (struct __actvret_uvmmcall_uvmm_vm_get_info*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_process_attach_chn* actvhdlr_uvmmhandler_uvmm_vm_process_attach_chn_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_process_attach_chn));
	return (struct __actvret_uvmmcall_uvmm_vm_process_attach_chn*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_process_delete_chn* actvhdlr_uvmmhandler_uvmm_vm_process_delete_chn_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_process_delete_chn));
	return (struct __actvret_uvmmcall_uvmm_vm_process_delete_chn*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_process_notify_chn* actvhdlr_uvmmhandler_uvmm_vm_process_notify_chn_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_process_notify_chn));
	return (struct __actvret_uvmmcall_uvmm_vm_process_notify_chn*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_process_inject_chn* actvhdlr_uvmmhandler_uvmm_vm_process_inject_chn_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_process_inject_chn));
	return (struct __actvret_uvmmcall_uvmm_vm_process_inject_chn*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_process_read_chn_status* actvhdlr_uvmmhandler_uvmm_vm_process_read_chn_status_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_process_read_chn_status));
	return (struct __actvret_uvmmcall_uvmm_vm_process_read_chn_status*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vnotify_attach_fd* actvhdlr_uvmmhandler_uvmm_vnotify_attach_fd_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vnotify_attach_fd));
	return (struct __actvret_uvmmcall_uvmm_vnotify_attach_fd*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_handle_vm_process_exit* actvhdlr_uvmmhandler_uvmm_handle_vm_process_exit_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_handle_vm_process_exit));
	return (struct __actvret_uvmmcall_uvmm_handle_vm_process_exit*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_pause* actvhdlr_uvmmhandler_uvmm_vm_pause_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_pause));
	return (struct __actvret_uvmmcall_uvmm_vm_pause*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_unpause* actvhdlr_uvmmhandler_uvmm_vm_unpause_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_unpause));
	return (struct __actvret_uvmmcall_uvmm_vm_unpause*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_resume* actvhdlr_uvmmhandler_uvmm_vm_resume_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_resume));
	return (struct __actvret_uvmmcall_uvmm_vm_resume*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_shutdown* actvhdlr_uvmmhandler_uvmm_vm_shutdown_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_shutdown));
	return (struct __actvret_uvmmcall_uvmm_vm_shutdown*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_vnotify_chns_info* actvhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_vnotify_chns_info));
	return (struct __actvret_uvmmcall_uvmm_vm_vnotify_chns_info*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_mclone_to_vm* actvhdlr_uvmmhandler_uvmm_mclone_to_vm_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_mclone_to_vm));
	return (struct __actvret_uvmmcall_uvmm_mclone_to_vm*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vnotify_send_ctrl_msg* actvhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vnotify_send_ctrl_msg));
	return (struct __actvret_uvmmcall_uvmm_vnotify_send_ctrl_msg*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_process_query_chn_status* actvhdlr_uvmmhandler_uvmm_vm_process_query_chn_status_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_process_query_chn_status));
	return (struct __actvret_uvmmcall_uvmm_vm_process_query_chn_status*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_cpu_hotplug* actvhdlr_uvmmhandler_uvmm_vm_cpu_hotplug_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_cpu_hotplug));
	return (struct __actvret_uvmmcall_uvmm_vm_cpu_hotplug*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_trace_stats* actvhdlr_uvmmhandler_uvmm_vm_trace_stats_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_trace_stats));
	return (struct __actvret_uvmmcall_uvmm_vm_trace_stats*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vnotify_update_futex* actvhdlr_uvmmhandler_uvmm_vnotify_update_futex_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vnotify_update_futex));
	return (struct __actvret_uvmmcall_uvmm_vnotify_update_futex*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_set_trap_wfi* actvhdlr_uvmmhandler_uvmm_vm_set_trap_wfi_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_set_trap_wfi));
	return (struct __actvret_uvmmcall_uvmm_vm_set_trap_wfi*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_set_trap_wfe* actvhdlr_uvmmhandler_uvmm_vm_set_trap_wfe_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_set_trap_wfe));
	return (struct __actvret_uvmmcall_uvmm_vm_set_trap_wfe*)__ret;
}

static inline struct __actvret_uvmmcall_uvmm_vm_set_tickless* actvhdlr_uvmmhandler_uvmm_vm_set_tickless_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_uvmm_vm_set_tickless));
	return (struct __actvret_uvmmcall_uvmm_vm_set_tickless*)__ret;
}

static inline struct __actvret_uvmmcall_vsock_vm_rx* actvhdlr_uvmmhandler_vsock_vm_rx_prepare_ret(unsigned long long sender, unsigned long credential)
{
	void *__ret = hm_actvpool_prepare_ret(sender, credential, sizeof(struct __actvret_uvmmcall_vsock_vm_rx));
	return (struct __actvret_uvmmcall_vsock_vm_rx*)__ret;
}

#ifdef __sysif_server_uvmm_no_compat32_handlers__
#ifdef __HOST_LLT__
union uvmm_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open)(unsigned long long sender, unsigned long credential, int shm_id, unsigned int flags, unsigned short acc_mode);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id, unsigned int flags, unsigned short acc_mode);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close)(unsigned long long sender, unsigned long credential, int shm_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_get_info)(unsigned long long sender, unsigned long credential, enum vminfo_option opt, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum vminfo_option opt, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn)(unsigned long long sender, unsigned long credential, struct vm_attach_chn_arg_s args);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_attach_chn_arg_s args);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn)(unsigned long long sender, unsigned long credential, unsigned int chn_id, bool sync, __u64 ts);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, bool sync, __u64 ts);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit)(unsigned long long sender, unsigned long credential, __u32 cnode_idx);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, __u32 cnode_idx);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_pause)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_unpause)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_resume)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown)(unsigned long long sender, unsigned long credential, struct vm_shutdown_arg_s args);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_shutdown_arg_s args);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm)(unsigned long long sender, unsigned long credential, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status)(unsigned long long sender, unsigned long credential, unsigned int chn_id, unsigned int query_type);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, unsigned int query_type);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats)(unsigned long long sender, unsigned long credential, int act, struct bunch_ipc_attr attr, uint64_t args);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int act, struct bunch_ipc_attr attr, uint64_t args);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex)(unsigned long long sender, unsigned long credential, unsigned int chn_id, const int *futex, bool shared);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, const int *futex, bool shared);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi)(unsigned long long sender, unsigned long credential, uint64_t state);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe)(unsigned long long sender, unsigned long credential, uint64_t state);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_vsock_vm_rx)(unsigned long long sender, unsigned long credential, const void *hdr, size_t hdr_len, const void *data, size_t len);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, const void *hdr, size_t hdr_len, const void *data, size_t len);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union uvmm_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open)(unsigned long long sender, unsigned long credential, int shm_id, unsigned int flags, unsigned short acc_mode);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id, unsigned int flags, unsigned short acc_mode);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close)(unsigned long long sender, unsigned long credential, int shm_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_get_info)(unsigned long long sender, unsigned long credential, enum vminfo_option opt, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum vminfo_option opt, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn)(unsigned long long sender, unsigned long credential, struct vm_attach_chn_arg_s args);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_attach_chn_arg_s args);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn)(unsigned long long sender, unsigned long credential, unsigned int chn_id, bool sync, __u64 ts);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, bool sync, __u64 ts);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit)(unsigned long long sender, unsigned long credential, __u32 cnode_idx);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, __u32 cnode_idx);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_pause)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_unpause)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_resume)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown)(unsigned long long sender, unsigned long credential, struct vm_shutdown_arg_s args);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_shutdown_arg_s args);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm)(unsigned long long sender, unsigned long credential, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status)(unsigned long long sender, unsigned long credential, unsigned int chn_id, unsigned int query_type);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, unsigned int query_type);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats)(unsigned long long sender, unsigned long credential, int act, struct bunch_ipc_attr attr, uint64_t args);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int act, struct bunch_ipc_attr attr, uint64_t args);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex)(unsigned long long sender, unsigned long credential, unsigned int chn_id, const int *futex, bool shared);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, const int *futex, bool shared);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi)(unsigned long long sender, unsigned long credential, uint64_t state);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe)(unsigned long long sender, unsigned long credential, uint64_t state);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_vsock_vm_rx)(unsigned long long sender, unsigned long credential, const void *hdr, size_t hdr_len, const void *data, size_t len);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, const void *hdr, size_t hdr_len, const void *data, size_t len);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#ifdef __arm__
union uvmm_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open)(unsigned long long sender, unsigned long credential, int shm_id, unsigned int flags, unsigned short acc_mode);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id, unsigned int flags, unsigned short acc_mode);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close)(unsigned long long sender, unsigned long credential, int shm_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_get_info)(unsigned long long sender, unsigned long credential, enum vminfo_option opt, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum vminfo_option opt, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn)(unsigned long long sender, unsigned long credential, struct vm_attach_chn_arg_s args);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_attach_chn_arg_s args);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn)(unsigned long long sender, unsigned long credential, unsigned int chn_id, bool sync, __u64 ts);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, bool sync, __u64 ts);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit)(unsigned long long sender, unsigned long credential, __u32 cnode_idx);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, __u32 cnode_idx);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_pause)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_unpause)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_resume)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown)(unsigned long long sender, unsigned long credential, struct vm_shutdown_arg_s args);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_shutdown_arg_s args);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm)(unsigned long long sender, unsigned long credential, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status)(unsigned long long sender, unsigned long credential, unsigned int chn_id, unsigned int query_type);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, unsigned int query_type);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats)(unsigned long long sender, unsigned long credential, int act, struct bunch_ipc_attr attr, uint64_t args);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int act, struct bunch_ipc_attr attr, uint64_t args);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex)(unsigned long long sender, unsigned long credential, unsigned int chn_id, const int *futex, bool shared);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, const int *futex, bool shared);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi)(unsigned long long sender, unsigned long credential, uint64_t state);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe)(unsigned long long sender, unsigned long credential, uint64_t state);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_vsock_vm_rx)(unsigned long long sender, unsigned long credential, const void *hdr, size_t hdr_len, const void *data, size_t len);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, const void *hdr, size_t hdr_len, const void *data, size_t len);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential);
};

#endif /* arm */
#endif /* not HOST_LLT */
#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif /* not __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#ifdef __HOST_LLT__
union uvmm_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open)(unsigned long long sender, unsigned long credential, int shm_id, unsigned int flags, unsigned short acc_mode);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id, unsigned int flags, unsigned short acc_mode);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close)(unsigned long long sender, unsigned long credential, int shm_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_get_info)(unsigned long long sender, unsigned long credential, enum vminfo_option opt, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum vminfo_option opt, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn)(unsigned long long sender, unsigned long credential, struct vm_attach_chn_arg_s args);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_attach_chn_arg_s args);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn)(unsigned long long sender, unsigned long credential, unsigned int chn_id, bool sync, __u64 ts);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, bool sync, __u64 ts);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit)(unsigned long long sender, unsigned long credential, __u32 cnode_idx);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, __u32 cnode_idx);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_pause)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_unpause)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_resume)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown)(unsigned long long sender, unsigned long credential, struct vm_shutdown_arg_s args);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_shutdown_arg_s args);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm)(unsigned long long sender, unsigned long credential, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status)(unsigned long long sender, unsigned long credential, unsigned int chn_id, unsigned int query_type);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, unsigned int query_type);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats)(unsigned long long sender, unsigned long credential, int act, struct bunch_ipc_attr attr, uint64_t args);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int act, struct bunch_ipc_attr attr, uint64_t args);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex)(unsigned long long sender, unsigned long credential, unsigned int chn_id, const int *futex, bool shared);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, const int *futex, bool shared);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi)(unsigned long long sender, unsigned long credential, uint64_t state);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe)(unsigned long long sender, unsigned long credential, uint64_t state);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_vsock_vm_rx)(unsigned long long sender, unsigned long credential, const void *hdr, size_t hdr_len, const void *data, size_t len);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, const void *hdr, size_t hdr_len, const void *data, size_t len);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shm_open)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shm_open)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shm_close)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shm_close)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_get_info)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_get_info)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_attach_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_attach_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_delete_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_delete_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_notify_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_notify_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_inject_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_inject_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_read_chn_status)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_read_chn_status)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_attach_fd)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_attach_fd)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_handle_vm_process_exit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_handle_vm_process_exit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_pause)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_pause)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_unpause)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_unpause)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_resume)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_resume)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shutdown)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shutdown)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_vnotify_chns_info)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_vnotify_chns_info)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_mclone_to_vm)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_mclone_to_vm)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_send_ctrl_msg)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_send_ctrl_msg)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_query_chn_status)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_query_chn_status)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_cpu_hotplug)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_cpu_hotplug)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_trace_stats)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_trace_stats)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_update_futex)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_update_futex)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfe)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfe)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_tickless)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_tickless)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_vsock_vm_rx)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_vsock_vm_rx)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(void);
};

#else
#ifdef __aarch64__
union uvmm_hdlrs_union {
	void *p_sysif_actvhdlr_ptr_void;
	int (*p_sysif_default_handler)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open)(unsigned long long sender, unsigned long credential, int shm_id, unsigned int flags, unsigned short acc_mode);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id, unsigned int flags, unsigned short acc_mode);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close)(unsigned long long sender, unsigned long credential, int shm_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_get_info)(unsigned long long sender, unsigned long credential, enum vminfo_option opt, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum vminfo_option opt, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn)(unsigned long long sender, unsigned long credential, struct vm_attach_chn_arg_s args);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_attach_chn_arg_s args);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn)(unsigned long long sender, unsigned long credential, unsigned int chn_id, bool sync, __u64 ts);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, bool sync, __u64 ts);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd)(unsigned long long sender, unsigned long credential, unsigned int chn_id);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit)(unsigned long long sender, unsigned long credential, __u32 cnode_idx);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, __u32 cnode_idx);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_pause)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_unpause)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_resume)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown)(unsigned long long sender, unsigned long credential, struct vm_shutdown_arg_s args);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_shutdown_arg_s args);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm)(unsigned long long sender, unsigned long credential, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status)(unsigned long long sender, unsigned long credential, unsigned int chn_id, unsigned int query_type);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, unsigned int query_type);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats)(unsigned long long sender, unsigned long credential, int act, struct bunch_ipc_attr attr, uint64_t args);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int act, struct bunch_ipc_attr attr, uint64_t args);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex)(unsigned long long sender, unsigned long credential, unsigned int chn_id, const int *futex, bool shared);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, const int *futex, bool shared);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi)(unsigned long long sender, unsigned long credential, uint64_t state);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe)(unsigned long long sender, unsigned long credential, uint64_t state);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state);
	int (*p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless)(unsigned long long sender, unsigned long credential);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS);
	int (*p__actvbridgehdlr_uvmmhandler_vsock_vm_rx)(unsigned long long sender, unsigned long credential, const void *hdr, size_t hdr_len, const void *data, size_t len);
	int (*p__actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx)(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, const void *hdr, size_t hdr_len, const void *data, size_t len);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shm_open)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shm_open)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shm_close)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shm_close)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_get_info)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_get_info)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_attach_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_attach_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_delete_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_delete_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_notify_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_notify_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_inject_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_inject_chn)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_read_chn_status)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_read_chn_status)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_attach_fd)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_attach_fd)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_handle_vm_process_exit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_handle_vm_process_exit)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_pause)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_pause)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_unpause)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_unpause)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_resume)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_resume)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shutdown)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shutdown)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_vnotify_chns_info)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_vnotify_chns_info)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_mclone_to_vm)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_mclone_to_vm)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_send_ctrl_msg)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_send_ctrl_msg)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_query_chn_status)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_query_chn_status)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_cpu_hotplug)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_cpu_hotplug)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_trace_stats)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_trace_stats)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_update_futex)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_update_futex)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfi)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfe)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfe)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_tickless)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_tickless)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgehdlr_compat_uvmmhandler_vsock_vm_rx)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p__actvbridgecapcallhdlr_compat_uvmmhandler_vsock_vm_rx)(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3);
	int (*p_sysif_handle_kcall_entry)(unsigned long long sender, unsigned long credential, __u64 arg1, __u64 arg2, __u64 arg3);
};

#endif /* aarch64 */
#endif /* not HOST_LLT */
#endif /* compat32 */

#if defined(__sysif_server_uvmm_no_compat32_handlers__) && defined(__sysif_server_uvmm_no_ilp32_handlers__)
#define __uvmm_method_TIMES 1
#elif !defined(__sysif_server_uvmm_no_compat32_handlers__) && !defined(__sysif_server_uvmm_no_ilp32_handlers__)
#define __uvmm_method_TIMES 3
#else
#define __uvmm_method_TIMES 2
#endif
extern union uvmm_hdlrs_union sysif_uvmm_actvhdlr_table[__uvmm_method_MAX * __uvmm_method_TIMES];

extern union uvmm_hdlrs_union sysif_uvmm_actvcapcallhdlr_table[__uvmm_method_MAX * __uvmm_method_TIMES];

extern void sysif_show_uvmm_methods(void);


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
static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open(unsigned long long sender, unsigned long credential, int shm_id, unsigned int flags, unsigned short acc_mode)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned short), __arch_actv_rpc_stksz_alignment(unsigned short));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shm_open(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, shm_id), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, flags), __ARCH_ACTV_RPC_RECV_ARG(unsigned short, acc_mode));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id, unsigned int flags, unsigned short acc_mode)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned short), __arch_actv_rpc_stksz_alignment(unsigned short));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shm_open(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, shm_id), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, flags), __ARCH_ACTV_RPC_RECV_ARG(unsigned short, acc_mode));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close(unsigned long long sender, unsigned long credential, int shm_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shm_close(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, shm_id));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shm_close(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, shm_id));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_get_info(unsigned long long sender, unsigned long credential, enum vminfo_option opt, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum vminfo_option), __arch_actv_rpc_stksz_alignment(enum vminfo_option));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_get_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum vminfo_option, opt), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum vminfo_option opt, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum vminfo_option), __arch_actv_rpc_stksz_alignment(enum vminfo_option));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_get_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum vminfo_option, opt), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn(unsigned long long sender, unsigned long credential, struct vm_attach_chn_arg_s args)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_attach_chn_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_attach_chn_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_attach_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct vm_attach_chn_arg_s, args));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_attach_chn_arg_s args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_attach_chn_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_attach_chn_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_attach_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct vm_attach_chn_arg_s, args));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn(unsigned long long sender, unsigned long credential, unsigned int chn_id, bool sync, __u64 ts)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u64), __arch_actv_rpc_stksz_alignment(__u64));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_delete_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(bool, sync), __ARCH_ACTV_RPC_RECV_ARG(__u64, ts));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, bool sync, __u64 ts)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u64), __arch_actv_rpc_stksz_alignment(__u64));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_delete_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(bool, sync), __ARCH_ACTV_RPC_RECV_ARG(__u64, ts));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn(unsigned long long sender, unsigned long credential, unsigned int chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_notify_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_notify_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn(unsigned long long sender, unsigned long credential, unsigned int chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_inject_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_inject_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status(unsigned long long sender, unsigned long credential, unsigned int chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_read_chn_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_read_chn_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd(unsigned long long sender, unsigned long credential, unsigned int chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_attach_fd(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_attach_fd(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit(unsigned long long sender, unsigned long credential, __u32 cnode_idx)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u32), __arch_actv_rpc_stksz_alignment(__u32));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_handle_vm_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(__u32, cnode_idx));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, __u32 cnode_idx)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u32), __arch_actv_rpc_stksz_alignment(__u32));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_handle_vm_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(__u32, cnode_idx));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_pause(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_pause(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_pause(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_unpause(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_unpause(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_unpause(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_resume(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_resume(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_resume(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown(unsigned long long sender, unsigned long credential, struct vm_shutdown_arg_s args)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_shutdown_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_shutdown_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shutdown(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct vm_shutdown_arg_s, args));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_shutdown_arg_s args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_shutdown_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_shutdown_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shutdown(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct vm_shutdown_arg_s, args));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm(unsigned long long sender, unsigned long credential, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_mclone_to_vm(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, src_va), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, tgt_gpa), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, len), __ARCH_ACTV_RPC_RECV_ARG(uint32_t, prot));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_mclone_to_vm(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, src_va), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, tgt_gpa), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, len), __ARCH_ACTV_RPC_RECV_ARG(uint32_t, prot));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status(unsigned long long sender, unsigned long credential, unsigned int chn_id, unsigned int query_type)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_query_chn_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, query_type));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, unsigned int query_type)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_query_chn_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, query_type));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_cpu_hotplug(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_cpu_hotplug(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats(unsigned long long sender, unsigned long credential, int act, struct bunch_ipc_attr attr, uint64_t args)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_trace_stats(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, act), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, args));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int act, struct bunch_ipc_attr attr, uint64_t args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_trace_stats(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, act), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, args));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex(unsigned long long sender, unsigned long credential, unsigned int chn_id, const int *futex, bool shared)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const int*), __arch_actv_rpc_stksz_alignment(const int*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_update_futex(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(const int*, futex), __ARCH_ACTV_RPC_RECV_ARG(bool, shared));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, const int *futex, bool shared)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const int*), __arch_actv_rpc_stksz_alignment(const int*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_update_futex(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(const int*, futex), __ARCH_ACTV_RPC_RECV_ARG(bool, shared));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi(unsigned long long sender, unsigned long credential, uint64_t state)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_trap_wfi(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, state));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_trap_wfi(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, state));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe(unsigned long long sender, unsigned long credential, uint64_t state)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_trap_wfe(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, state));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_trap_wfe(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, state));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_tickless(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_tickless(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_vsock_vm_rx(unsigned long long sender, unsigned long credential, const void *hdr, size_t hdr_len, const void *data, size_t len)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_vsock_vm_rx(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(const void*, hdr), __ARCH_ACTV_RPC_RECV_ARG(size_t, hdr_len), __ARCH_ACTV_RPC_RECV_ARG(const void*, data), __ARCH_ACTV_RPC_RECV_ARG(size_t, len));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, const void *hdr, size_t hdr_len, const void *data, size_t len)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return __actvshadowhdlr_uvmmhandler_vsock_vm_rx(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(const void*, hdr), __ARCH_ACTV_RPC_RECV_ARG(size_t, hdr_len), __ARCH_ACTV_RPC_RECV_ARG(const void*, data), __ARCH_ACTV_RPC_RECV_ARG(size_t, len));
}

#ifndef __sysif_server_uvmm_no_compat32_handlers__
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
static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shm_open(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _shm_id;
	unsigned int __maybe_unused _flags;
	unsigned short __maybe_unused _acc_mode;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_shm_id, data, 0, arg_sz, model, type, int, _shm_id)
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
		__fetch_one_arg((void*)&_flags, data, 1, arg_sz, model, type, unsigned int, _flags)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned short);
		type = __type_of(unsigned short);
		__fetch_one_arg((void*)&_acc_mode, data, 2, arg_sz, model, type, unsigned short, _acc_mode)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shm_open(sender, credential, _shm_id, _flags, _acc_mode);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shm_open(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _shm_id;
	unsigned int __maybe_unused _flags;
	unsigned short __maybe_unused _acc_mode;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_shm_id, data, 0, arg_sz, model, type, int, _shm_id)
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
		__fetch_one_arg((void*)&_flags, data, 1, arg_sz, model, type, unsigned int, _flags)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned short);
		type = __type_of(unsigned short);
		__fetch_one_arg((void*)&_acc_mode, data, 2, arg_sz, model, type, unsigned short, _acc_mode)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shm_open(sender, credential, _shm_id, _flags, _acc_mode);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shm_close(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _shm_id;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_shm_id, data, 0, arg_sz, model, type, int, _shm_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shm_close(sender, credential, _shm_id);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shm_close(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _shm_id;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_shm_id, data, 0, arg_sz, model, type, int, _shm_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shm_close(sender, credential, _shm_id);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_get_info(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	enum vminfo_option __maybe_unused _opt;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(enum vminfo_option);
		type = __type_of(enum vminfo_option);
		__fetch_one_arg((void*)&_opt, data, 0, arg_sz, model, type, enum vminfo_option, _opt)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_get_info(sender, credential, _opt, _attr);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_get_info(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	enum vminfo_option __maybe_unused _opt;
	struct bunch_ipc_attr __maybe_unused _attr;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(enum vminfo_option);
		type = __type_of(enum vminfo_option);
		__fetch_one_arg((void*)&_opt, data, 0, arg_sz, model, type, enum vminfo_option, _opt)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_get_info(sender, credential, _opt, _attr);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_attach_chn(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct vm_attach_chn_arg_s __maybe_unused _args;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct vm_attach_chn_arg_s);
		type = __type_of(struct vm_attach_chn_arg_s);
		__fetch_one_arg((void*)&_args, data, 0, arg_sz, model, type, struct vm_attach_chn_arg_s, _args)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_attach_chn(sender, credential, _args);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_attach_chn(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct vm_attach_chn_arg_s __maybe_unused _args;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct vm_attach_chn_arg_s);
		type = __type_of(struct vm_attach_chn_arg_s);
		__fetch_one_arg((void*)&_args, data, 0, arg_sz, model, type, struct vm_attach_chn_arg_s, _args)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_attach_chn(sender, credential, _args);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_delete_chn(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _chn_id;
	bool __maybe_unused _sync;
	__u64 __maybe_unused _ts;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_chn_id, data, 0, arg_sz, model, type, unsigned int, _chn_id)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(bool);
		type = __type_of(bool);
		__fetch_one_arg((void*)&_sync, data, 1, arg_sz, model, type, bool, _sync)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(__u64);
		type = __type_of(__u64);
		__fetch_one_arg((void*)&_ts, data, 2, arg_sz, model, type, __u64, _ts)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_delete_chn(sender, credential, _chn_id, _sync, _ts);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_delete_chn(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _chn_id;
	bool __maybe_unused _sync;
	__u64 __maybe_unused _ts;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_chn_id, data, 0, arg_sz, model, type, unsigned int, _chn_id)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(bool);
		type = __type_of(bool);
		__fetch_one_arg((void*)&_sync, data, 1, arg_sz, model, type, bool, _sync)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(__u64);
		type = __type_of(__u64);
		__fetch_one_arg((void*)&_ts, data, 2, arg_sz, model, type, __u64, _ts)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_delete_chn(sender, credential, _chn_id, _sync, _ts);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_notify_chn(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _chn_id;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_chn_id, data, 0, arg_sz, model, type, unsigned int, _chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_notify_chn(sender, credential, _chn_id);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_notify_chn(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _chn_id;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_chn_id, data, 0, arg_sz, model, type, unsigned int, _chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_notify_chn(sender, credential, _chn_id);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_inject_chn(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _chn_id;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_chn_id, data, 0, arg_sz, model, type, unsigned int, _chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_inject_chn(sender, credential, _chn_id);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_inject_chn(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _chn_id;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_chn_id, data, 0, arg_sz, model, type, unsigned int, _chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_inject_chn(sender, credential, _chn_id);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_read_chn_status(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _chn_id;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_chn_id, data, 0, arg_sz, model, type, unsigned int, _chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_read_chn_status(sender, credential, _chn_id);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_read_chn_status(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _chn_id;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_chn_id, data, 0, arg_sz, model, type, unsigned int, _chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_read_chn_status(sender, credential, _chn_id);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_attach_fd(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _chn_id;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_chn_id, data, 0, arg_sz, model, type, unsigned int, _chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_attach_fd(sender, credential, _chn_id);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_attach_fd(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _chn_id;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_chn_id, data, 0, arg_sz, model, type, unsigned int, _chn_id)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_attach_fd(sender, credential, _chn_id);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_handle_vm_process_exit(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__u32 __maybe_unused _cnode_idx;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(__u32);
		type = __type_of(__u32);
		__fetch_one_arg((void*)&_cnode_idx, data, 0, arg_sz, model, type, __u32, _cnode_idx)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_handle_vm_process_exit(sender, credential, _cnode_idx);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_handle_vm_process_exit(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__u32 __maybe_unused _cnode_idx;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(__u32);
		type = __type_of(__u32);
		__fetch_one_arg((void*)&_cnode_idx, data, 0, arg_sz, model, type, __u32, _cnode_idx)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_handle_vm_process_exit(sender, credential, _cnode_idx);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_pause(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_pause(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_pause(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_pause(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_unpause(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_unpause(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_unpause(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_unpause(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_resume(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_resume(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_resume(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_resume(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shutdown(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct vm_shutdown_arg_s __maybe_unused _args;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct vm_shutdown_arg_s);
		type = __type_of(struct vm_shutdown_arg_s);
		__fetch_one_arg((void*)&_args, data, 0, arg_sz, model, type, struct vm_shutdown_arg_s, _args)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shutdown(sender, credential, _args);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shutdown(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	struct vm_shutdown_arg_s __maybe_unused _args;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(struct vm_shutdown_arg_s);
		type = __type_of(struct vm_shutdown_arg_s);
		__fetch_one_arg((void*)&_args, data, 0, arg_sz, model, type, struct vm_shutdown_arg_s, _args)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shutdown(sender, credential, _args);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_vnotify_chns_info(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info(sender, credential, _attr);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_vnotify_chns_info(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info(sender, credential, _attr);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_mclone_to_vm(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	uint64_t __maybe_unused _src_va;
	uint64_t __maybe_unused _tgt_gpa;
	uint64_t __maybe_unused _len;
	uint32_t __maybe_unused _prot;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uint64_t);
		type = __type_of(uint64_t);
		__fetch_one_arg((void*)&_src_va, data, 0, arg_sz, model, type, uint64_t, _src_va)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uint64_t);
		type = __type_of(uint64_t);
		__fetch_one_arg((void*)&_tgt_gpa, data, 1, arg_sz, model, type, uint64_t, _tgt_gpa)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uint64_t);
		type = __type_of(uint64_t);
		__fetch_one_arg((void*)&_len, data, 2, arg_sz, model, type, uint64_t, _len)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uint32_t);
		type = __type_of(uint32_t);
		__fetch_one_arg((void*)&_prot, data, 3, arg_sz, model, type, uint32_t, _prot)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_mclone_to_vm(sender, credential, _src_va, _tgt_gpa, _len, _prot);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_mclone_to_vm(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	uint64_t __maybe_unused _src_va;
	uint64_t __maybe_unused _tgt_gpa;
	uint64_t __maybe_unused _len;
	uint32_t __maybe_unused _prot;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uint64_t);
		type = __type_of(uint64_t);
		__fetch_one_arg((void*)&_src_va, data, 0, arg_sz, model, type, uint64_t, _src_va)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uint64_t);
		type = __type_of(uint64_t);
		__fetch_one_arg((void*)&_tgt_gpa, data, 1, arg_sz, model, type, uint64_t, _tgt_gpa)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uint64_t);
		type = __type_of(uint64_t);
		__fetch_one_arg((void*)&_len, data, 2, arg_sz, model, type, uint64_t, _len)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uint32_t);
		type = __type_of(uint32_t);
		__fetch_one_arg((void*)&_prot, data, 3, arg_sz, model, type, uint32_t, _prot)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_mclone_to_vm(sender, credential, _src_va, _tgt_gpa, _len, _prot);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_send_ctrl_msg(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_send_ctrl_msg(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_query_chn_status(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _chn_id;
	unsigned int __maybe_unused _query_type;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_chn_id, data, 0, arg_sz, model, type, unsigned int, _chn_id)
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
		__fetch_one_arg((void*)&_query_type, data, 1, arg_sz, model, type, unsigned int, _query_type)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_query_chn_status(sender, credential, _chn_id, _query_type);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_query_chn_status(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _chn_id;
	unsigned int __maybe_unused _query_type;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_chn_id, data, 0, arg_sz, model, type, unsigned int, _chn_id)
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
		__fetch_one_arg((void*)&_query_type, data, 1, arg_sz, model, type, unsigned int, _query_type)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_query_chn_status(sender, credential, _chn_id, _query_type);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_cpu_hotplug(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_cpu_hotplug(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_cpu_hotplug(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_cpu_hotplug(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_trace_stats(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _act;
	struct bunch_ipc_attr __maybe_unused _attr;
	uint64_t __maybe_unused _args;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_act, data, 0, arg_sz, model, type, int, _act)
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
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uint64_t);
		type = __type_of(uint64_t);
		__fetch_one_arg((void*)&_args, data, 2, arg_sz, model, type, uint64_t, _args)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_trace_stats(sender, credential, _act, _attr, _args);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_trace_stats(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	int __maybe_unused _act;
	struct bunch_ipc_attr __maybe_unused _attr;
	uint64_t __maybe_unused _args;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(int);
		type = __type_of(int);
		__fetch_one_arg((void*)&_act, data, 0, arg_sz, model, type, int, _act)
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
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uint64_t);
		type = __type_of(uint64_t);
		__fetch_one_arg((void*)&_args, data, 2, arg_sz, model, type, uint64_t, _args)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_trace_stats(sender, credential, _act, _attr, _args);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_update_futex(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _chn_id;
	__maybe_unused const int *_futex;
	bool __maybe_unused _shared;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_chn_id, data, 0, arg_sz, model, type, unsigned int, _chn_id)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const int*);
		type = __type_of(const int*);
		__fetch_one_arg((void*)&_futex, data, 1, arg_sz, model, type, const int*, _futex)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(bool);
		type = __type_of(bool);
		__fetch_one_arg((void*)&_shared, data, 2, arg_sz, model, type, bool, _shared)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_update_futex(sender, credential, _chn_id, _futex, _shared);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_update_futex(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	unsigned int __maybe_unused _chn_id;
	__maybe_unused const int *_futex;
	bool __maybe_unused _shared;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(unsigned int);
		type = __type_of(unsigned int);
		__fetch_one_arg((void*)&_chn_id, data, 0, arg_sz, model, type, unsigned int, _chn_id)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const int*);
		type = __type_of(const int*);
		__fetch_one_arg((void*)&_futex, data, 1, arg_sz, model, type, const int*, _futex)
		if (fetch_sz < 0) {
			panic("failed to fetch arg: %s\n", hmstrerror(fetch_sz));
		}
		cursor += fetch_sz;
	} while (0);
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(bool);
		type = __type_of(bool);
		__fetch_one_arg((void*)&_shared, data, 2, arg_sz, model, type, bool, _shared)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_update_futex(sender, credential, _chn_id, _futex, _shared);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfi(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	uint64_t __maybe_unused _state;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uint64_t);
		type = __type_of(uint64_t);
		__fetch_one_arg((void*)&_state, data, 0, arg_sz, model, type, uint64_t, _state)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_trap_wfi(sender, credential, _state);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfi(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	uint64_t __maybe_unused _state;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uint64_t);
		type = __type_of(uint64_t);
		__fetch_one_arg((void*)&_state, data, 0, arg_sz, model, type, uint64_t, _state)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_trap_wfi(sender, credential, _state);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfe(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	uint64_t __maybe_unused _state;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uint64_t);
		type = __type_of(uint64_t);
		__fetch_one_arg((void*)&_state, data, 0, arg_sz, model, type, uint64_t, _state)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_trap_wfe(sender, credential, _state);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfe(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	uint64_t __maybe_unused _state;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(uint64_t);
		type = __type_of(uint64_t);
		__fetch_one_arg((void*)&_state, data, 0, arg_sz, model, type, uint64_t, _state)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_trap_wfe(sender, credential, _state);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_tickless(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_tickless(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_tickless(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_tickless(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_compat_uvmmhandler_vsock_vm_rx(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused const void *_hdr;
	size_t __maybe_unused _hdr_len;
	__maybe_unused const void *_data;
	size_t __maybe_unused _len;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const void*);
		type = __type_of(const void*);
		__fetch_one_arg((void*)&_hdr, data, 0, arg_sz, model, type, const void*, _hdr)
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
		__fetch_one_arg((void*)&_hdr_len, data, 1, arg_sz, model, type, size_t, _hdr_len)
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
		__fetch_one_arg((void*)&_data, data, 2, arg_sz, model, type, const void*, _data)
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
		__fetch_one_arg((void*)&_len, data, 3, arg_sz, model, type, size_t, _len)
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
	return __actvshadowhdlr_uvmmhandler_vsock_vm_rx(sender, credential, _hdr, _hdr_len, _data, _len);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_compat_uvmmhandler_vsock_vm_rx(unsigned long long sender, unsigned long credential, unsigned long sp, unsigned long x3)
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
	__maybe_unused const void *_hdr;
	size_t __maybe_unused _hdr_len;
	__maybe_unused const void *_data;
	size_t __maybe_unused _len;
	do {
		int fetch_sz;
		unsigned long align;
		unsigned long arg_sz = sizeof(const void*);
		type = __type_of(const void*);
		__fetch_one_arg((void*)&_hdr, data, 0, arg_sz, model, type, const void*, _hdr)
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
		__fetch_one_arg((void*)&_hdr_len, data, 1, arg_sz, model, type, size_t, _hdr_len)
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
		__fetch_one_arg((void*)&_data, data, 2, arg_sz, model, type, const void*, _data)
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
		__fetch_one_arg((void*)&_len, data, 3, arg_sz, model, type, size_t, _len)
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
	return __actvshadowhdlr_uvmmhandler_vsock_vm_rx(sender, credential, _hdr, _hdr_len, _data, _len);
}

#endif /* compat handlers */
#ifndef __sysif_server_uvmm_no_ilp32_handlers__
static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_shm_open(unsigned long long sender, unsigned long credential, int shm_id, unsigned int flags, unsigned short acc_mode)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned short), __arch_actv_rpc_stksz_alignment(unsigned short));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, shm_id)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, flags)
	} while (0);
	do {
		__adjust_one_arg(unsigned short, acc_mode)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shm_open(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, shm_id), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, flags), __ARCH_ACTV_RPC_RECV_ARG(unsigned short, acc_mode));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_shm_open(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id, unsigned int flags, unsigned short acc_mode)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned short), __arch_actv_rpc_stksz_alignment(unsigned short));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, shm_id)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, flags)
	} while (0);
	do {
		__adjust_one_arg(unsigned short, acc_mode)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shm_open(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, shm_id), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, flags), __ARCH_ACTV_RPC_RECV_ARG(unsigned short, acc_mode));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_shm_close(unsigned long long sender, unsigned long credential, int shm_id)
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
		__adjust_one_arg(int, shm_id)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shm_close(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, shm_id));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_shm_close(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id)
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
		__adjust_one_arg(int, shm_id)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shm_close(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, shm_id));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_get_info(unsigned long long sender, unsigned long credential, enum vminfo_option opt, struct bunch_ipc_attr attr)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum vminfo_option), __arch_actv_rpc_stksz_alignment(enum vminfo_option));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(enum vminfo_option, opt)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_get_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum vminfo_option, opt), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_get_info(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum vminfo_option opt, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum vminfo_option), __arch_actv_rpc_stksz_alignment(enum vminfo_option));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(enum vminfo_option, opt)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_get_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum vminfo_option, opt), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_attach_chn(unsigned long long sender, unsigned long credential, struct vm_attach_chn_arg_s args)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_attach_chn_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_attach_chn_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct vm_attach_chn_arg_s, args)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_attach_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct vm_attach_chn_arg_s, args));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_attach_chn(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_attach_chn_arg_s args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_attach_chn_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_attach_chn_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct vm_attach_chn_arg_s, args)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_attach_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct vm_attach_chn_arg_s, args));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_delete_chn(unsigned long long sender, unsigned long credential, unsigned int chn_id, bool sync, __u64 ts)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u64), __arch_actv_rpc_stksz_alignment(__u64));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, chn_id)
	} while (0);
	do {
		__adjust_one_arg(bool, sync)
	} while (0);
	do {
		__adjust_one_arg(__u64, ts)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_delete_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(bool, sync), __ARCH_ACTV_RPC_RECV_ARG(__u64, ts));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_delete_chn(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, bool sync, __u64 ts)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u64), __arch_actv_rpc_stksz_alignment(__u64));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, chn_id)
	} while (0);
	do {
		__adjust_one_arg(bool, sync)
	} while (0);
	do {
		__adjust_one_arg(__u64, ts)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_delete_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(bool, sync), __ARCH_ACTV_RPC_RECV_ARG(__u64, ts));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_notify_chn(unsigned long long sender, unsigned long credential, unsigned int chn_id)
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
		__adjust_one_arg(unsigned int, chn_id)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_notify_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_notify_chn(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id)
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
		__adjust_one_arg(unsigned int, chn_id)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_notify_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_inject_chn(unsigned long long sender, unsigned long credential, unsigned int chn_id)
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
		__adjust_one_arg(unsigned int, chn_id)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_inject_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_inject_chn(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id)
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
		__adjust_one_arg(unsigned int, chn_id)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_inject_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_read_chn_status(unsigned long long sender, unsigned long credential, unsigned int chn_id)
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
		__adjust_one_arg(unsigned int, chn_id)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_read_chn_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_read_chn_status(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id)
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
		__adjust_one_arg(unsigned int, chn_id)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_read_chn_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vnotify_attach_fd(unsigned long long sender, unsigned long credential, unsigned int chn_id)
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
		__adjust_one_arg(unsigned int, chn_id)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_attach_fd(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vnotify_attach_fd(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id)
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
		__adjust_one_arg(unsigned int, chn_id)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_attach_fd(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_handle_vm_process_exit(unsigned long long sender, unsigned long credential, __u32 cnode_idx)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u32), __arch_actv_rpc_stksz_alignment(__u32));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(__u32, cnode_idx)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_handle_vm_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(__u32, cnode_idx));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_handle_vm_process_exit(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, __u32 cnode_idx)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u32), __arch_actv_rpc_stksz_alignment(__u32));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(__u32, cnode_idx)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_handle_vm_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(__u32, cnode_idx));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_pause(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_pause(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_pause(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_pause(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_unpause(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_unpause(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_unpause(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_unpause(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_resume(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_resume(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_resume(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_resume(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_shutdown(unsigned long long sender, unsigned long credential, struct vm_shutdown_arg_s args)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_shutdown_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_shutdown_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct vm_shutdown_arg_s, args)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shutdown(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct vm_shutdown_arg_s, args));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_shutdown(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_shutdown_arg_s args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_shutdown_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_shutdown_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(struct vm_shutdown_arg_s, args)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_shutdown(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct vm_shutdown_arg_s, args));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_vnotify_chns_info(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_vnotify_chns_info(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr)
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
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_mclone_to_vm(unsigned long long sender, unsigned long credential, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(uint64_t, src_va)
	} while (0);
	do {
		__adjust_one_arg(uint64_t, tgt_gpa)
	} while (0);
	do {
		__adjust_one_arg(uint64_t, len)
	} while (0);
	do {
		__adjust_one_arg(uint32_t, prot)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_mclone_to_vm(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, src_va), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, tgt_gpa), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, len), __ARCH_ACTV_RPC_RECV_ARG(uint32_t, prot));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_mclone_to_vm(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(uint64_t, src_va)
	} while (0);
	do {
		__adjust_one_arg(uint64_t, tgt_gpa)
	} while (0);
	do {
		__adjust_one_arg(uint64_t, len)
	} while (0);
	do {
		__adjust_one_arg(uint32_t, prot)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_mclone_to_vm(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, src_va), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, tgt_gpa), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, len), __ARCH_ACTV_RPC_RECV_ARG(uint32_t, prot));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vnotify_send_ctrl_msg(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vnotify_send_ctrl_msg(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_query_chn_status(unsigned long long sender, unsigned long credential, unsigned int chn_id, unsigned int query_type)
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
		__adjust_one_arg(unsigned int, chn_id)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, query_type)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_query_chn_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, query_type));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_query_chn_status(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, unsigned int query_type)
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
		__adjust_one_arg(unsigned int, chn_id)
	} while (0);
	do {
		__adjust_one_arg(unsigned int, query_type)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_process_query_chn_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, query_type));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_cpu_hotplug(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_cpu_hotplug(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_cpu_hotplug(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_cpu_hotplug(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_trace_stats(unsigned long long sender, unsigned long credential, int act, struct bunch_ipc_attr attr, uint64_t args)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, act)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	do {
		__adjust_one_arg(uint64_t, args)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_trace_stats(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, act), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, args));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_trace_stats(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int act, struct bunch_ipc_attr attr, uint64_t args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(int, act)
	} while (0);
	do {
		__adjust_one_arg(struct bunch_ipc_attr, attr)
	} while (0);
	do {
		__adjust_one_arg(uint64_t, args)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_trace_stats(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, act), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, args));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vnotify_update_futex(unsigned long long sender, unsigned long credential, unsigned int chn_id, const int *futex, bool shared)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const int*), __arch_actv_rpc_stksz_alignment(const int*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, chn_id)
	} while (0);
	do {
		__adjust_one_arg(const int*, futex)
	} while (0);
	do {
		__adjust_one_arg(bool, shared)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_update_futex(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(const int*, futex), __ARCH_ACTV_RPC_RECV_ARG(bool, shared));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vnotify_update_futex(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, const int *futex, bool shared)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const int*), __arch_actv_rpc_stksz_alignment(const int*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(unsigned int, chn_id)
	} while (0);
	do {
		__adjust_one_arg(const int*, futex)
	} while (0);
	do {
		__adjust_one_arg(bool, shared)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vnotify_update_futex(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(const int*, futex), __ARCH_ACTV_RPC_RECV_ARG(bool, shared));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_set_trap_wfi(unsigned long long sender, unsigned long credential, uint64_t state)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(uint64_t, state)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_trap_wfi(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, state));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_set_trap_wfi(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(uint64_t, state)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_trap_wfi(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, state));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_set_trap_wfe(unsigned long long sender, unsigned long credential, uint64_t state)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(uint64_t, state)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_trap_wfe(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, state));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_set_trap_wfe(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(uint64_t, state)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_trap_wfe(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, state));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_set_tickless(unsigned long long sender, unsigned long credential)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_tickless(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_set_tickless(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return __actvshadowhdlr_uvmmhandler_uvmm_vm_set_tickless(sender, credential);
}

static int __opt_nonoframe __opt_nostkprot __actvbridgehdlr_ilp32_uvmmhandler_vsock_vm_rx(unsigned long long sender, unsigned long credential, const void *hdr, size_t hdr_len, const void *data, size_t len)
{
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(const void*, hdr)
	} while (0);
	do {
		__adjust_one_arg(size_t, hdr_len)
	} while (0);
	do {
		__adjust_one_arg(const void*, data)
	} while (0);
	do {
		__adjust_one_arg(size_t, len)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_vsock_vm_rx(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(const void*, hdr), __ARCH_ACTV_RPC_RECV_ARG(size_t, hdr_len), __ARCH_ACTV_RPC_RECV_ARG(const void*, data), __ARCH_ACTV_RPC_RECV_ARG(size_t, len));
}

static int __opt_nonoframe __opt_nostkprot __actvbridgecapcallhdlr_ilp32_uvmmhandler_vsock_vm_rx(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, const void *hdr, size_t hdr_len, const void *data, size_t len)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused unsigned long *__p_prev_frame = __builtin_frame_address(0);
	unsigned long __maybe_unused __stacksz = __ARCH_ACTV_STACK_ARG_SIZE;
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned int) != __stacksz)) {
		return E_HM_INVAL;
	}
	do {
		__adjust_one_arg(const void*, hdr)
	} while (0);
	do {
		__adjust_one_arg(size_t, hdr_len)
	} while (0);
	do {
		__adjust_one_arg(const void*, data)
	} while (0);
	do {
		__adjust_one_arg(size_t, len)
	} while (0);
	return __actvshadowhdlr_uvmmhandler_vsock_vm_rx(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(const void*, hdr), __ARCH_ACTV_RPC_RECV_ARG(size_t, hdr_len), __ARCH_ACTV_RPC_RECV_ARG(const void*, data), __ARCH_ACTV_RPC_RECV_ARG(size_t, len));
}

#endif
#else
static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open(unsigned long long sender, unsigned long credential, int shm_id, unsigned int flags, unsigned short acc_mode)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned short), __arch_actv_rpc_stksz_alignment(unsigned short));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_shm_open(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, shm_id), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, flags), __ARCH_ACTV_RPC_RECV_ARG(unsigned short, acc_mode));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id, unsigned int flags, unsigned short acc_mode)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned short), __arch_actv_rpc_stksz_alignment(unsigned short));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_shm_open(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, shm_id), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, flags), __ARCH_ACTV_RPC_RECV_ARG(unsigned short, acc_mode));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close(unsigned long long sender, unsigned long credential, int shm_id)
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
	return actvhdlr_uvmmhandler_uvmm_vm_shm_close(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, shm_id));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int shm_id)
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
	return actvhdlr_uvmmhandler_uvmm_vm_shm_close(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, shm_id));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_get_info(unsigned long long sender, unsigned long credential, enum vminfo_option opt, struct bunch_ipc_attr attr)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum vminfo_option), __arch_actv_rpc_stksz_alignment(enum vminfo_option));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_get_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum vminfo_option, opt), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, enum vminfo_option opt, struct bunch_ipc_attr attr)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum vminfo_option), __arch_actv_rpc_stksz_alignment(enum vminfo_option));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_get_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(enum vminfo_option, opt), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn(unsigned long long sender, unsigned long credential, struct vm_attach_chn_arg_s args)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_attach_chn_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_attach_chn_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_process_attach_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct vm_attach_chn_arg_s, args));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_attach_chn_arg_s args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_attach_chn_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_attach_chn_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_process_attach_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct vm_attach_chn_arg_s, args));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn(unsigned long long sender, unsigned long credential, unsigned int chn_id, bool sync, __u64 ts)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u64), __arch_actv_rpc_stksz_alignment(__u64));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_process_delete_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(bool, sync), __ARCH_ACTV_RPC_RECV_ARG(__u64, ts));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, bool sync, __u64 ts)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u64), __arch_actv_rpc_stksz_alignment(__u64));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_process_delete_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(bool, sync), __ARCH_ACTV_RPC_RECV_ARG(__u64, ts));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn(unsigned long long sender, unsigned long credential, unsigned int chn_id)
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
	return actvhdlr_uvmmhandler_uvmm_vm_process_notify_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id)
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
	return actvhdlr_uvmmhandler_uvmm_vm_process_notify_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn(unsigned long long sender, unsigned long credential, unsigned int chn_id)
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
	return actvhdlr_uvmmhandler_uvmm_vm_process_inject_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id)
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
	return actvhdlr_uvmmhandler_uvmm_vm_process_inject_chn(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status(unsigned long long sender, unsigned long credential, unsigned int chn_id)
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
	return actvhdlr_uvmmhandler_uvmm_vm_process_read_chn_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id)
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
	return actvhdlr_uvmmhandler_uvmm_vm_process_read_chn_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd(unsigned long long sender, unsigned long credential, unsigned int chn_id)
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
	return actvhdlr_uvmmhandler_uvmm_vnotify_attach_fd(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id)
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
	return actvhdlr_uvmmhandler_uvmm_vnotify_attach_fd(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit(unsigned long long sender, unsigned long credential, __u32 cnode_idx)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u32), __arch_actv_rpc_stksz_alignment(__u32));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_handle_vm_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(__u32, cnode_idx));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, __u32 cnode_idx)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(__u32), __arch_actv_rpc_stksz_alignment(__u32));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_handle_vm_process_exit(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(__u32, cnode_idx));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_pause(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_uvmmhandler_uvmm_vm_pause(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_uvmmhandler_uvmm_vm_pause(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_unpause(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_uvmmhandler_uvmm_vm_unpause(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_uvmmhandler_uvmm_vm_unpause(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_resume(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_uvmmhandler_uvmm_vm_resume(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_uvmmhandler_uvmm_vm_resume(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown(unsigned long long sender, unsigned long credential, struct vm_shutdown_arg_s args)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_shutdown_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_shutdown_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_shutdown(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct vm_shutdown_arg_s, args));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct vm_shutdown_arg_s args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct vm_shutdown_arg_s), __arch_actv_rpc_stksz_alignment(struct vm_shutdown_arg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_shutdown(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct vm_shutdown_arg_s, args));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info(unsigned long long sender, unsigned long credential, struct bunch_ipc_attr attr)
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
	return actvhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, struct bunch_ipc_attr attr)
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
	return actvhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm(unsigned long long sender, unsigned long credential, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_mclone_to_vm(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, src_va), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, tgt_gpa), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, len), __ARCH_ACTV_RPC_RECV_ARG(uint32_t, prot));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_mclone_to_vm(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, src_va), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, tgt_gpa), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, len), __ARCH_ACTV_RPC_RECV_ARG(uint32_t, prot));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status(unsigned long long sender, unsigned long credential, unsigned int chn_id, unsigned int query_type)
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
	return actvhdlr_uvmmhandler_uvmm_vm_process_query_chn_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, query_type));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, unsigned int query_type)
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
	return actvhdlr_uvmmhandler_uvmm_vm_process_query_chn_status(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(unsigned int, query_type));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_uvmmhandler_uvmm_vm_cpu_hotplug(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_uvmmhandler_uvmm_vm_cpu_hotplug(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats(unsigned long long sender, unsigned long credential, int act, struct bunch_ipc_attr attr, uint64_t args)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_trace_stats(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, act), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, args));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, int act, struct bunch_ipc_attr attr, uint64_t args)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct bunch_ipc_attr), __arch_actv_rpc_stksz_alignment(struct bunch_ipc_attr));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_trace_stats(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(int, act), __ARCH_ACTV_RPC_RECV_ARG(struct bunch_ipc_attr, attr), __ARCH_ACTV_RPC_RECV_ARG(uint64_t, args));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex(unsigned long long sender, unsigned long credential, unsigned int chn_id, const int *futex, bool shared)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const int*), __arch_actv_rpc_stksz_alignment(const int*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vnotify_update_futex(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(const int*, futex), __ARCH_ACTV_RPC_RECV_ARG(bool, shared));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, unsigned int chn_id, const int *futex, bool shared)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const int*), __arch_actv_rpc_stksz_alignment(const int*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vnotify_update_futex(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(unsigned int, chn_id), __ARCH_ACTV_RPC_RECV_ARG(const int*, futex), __ARCH_ACTV_RPC_RECV_ARG(bool, shared));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi(unsigned long long sender, unsigned long credential, uint64_t state)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_set_trap_wfi(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, state));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_set_trap_wfi(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, state));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe(unsigned long long sender, unsigned long credential, uint64_t state)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_set_trap_wfe(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, state));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, uint64_t state)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_uvmm_vm_set_trap_wfe(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(uint64_t, state));
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless(unsigned long long sender, unsigned long credential)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_uvmmhandler_uvmm_vm_set_tickless(sender, credential);
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	return actvhdlr_uvmmhandler_uvmm_vm_set_tickless(sender, credential);
}

static int __opt_nonoframe __actvbridgehdlr_uvmmhandler_vsock_vm_rx(unsigned long long sender, unsigned long credential, const void *hdr, size_t hdr_len, const void *data, size_t len)
{
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_vsock_vm_rx(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(const void*, hdr), __ARCH_ACTV_RPC_RECV_ARG(size_t, hdr_len), __ARCH_ACTV_RPC_RECV_ARG(const void*, data), __ARCH_ACTV_RPC_RECV_ARG(size_t, len));
}

static int __opt_nonoframe __actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx(unsigned long long sender, unsigned long credential, __RPC_CAPCALL_HDLR_UNUSED_ARGS, const void *hdr, size_t hdr_len, const void *data, size_t len)
{
	UNUSED(__RPC_CAPCALL_HDLR_UNUSED_ARGS_STRICT);
	__maybe_unused struct arch_actv_local *__local = hm_actv_current_arch_actv_local();
	unsigned long __nr_ul = __RPC_INFO_DECODE_STK_BUF_NR_UL(__local->rpcinfo);
	unsigned long __maybe_unused __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	if (unlikely(__nr_ul * sizeof(unsigned long) != __stacksz)) {
		return E_HM_INVAL;
	}
	return actvhdlr_uvmmhandler_vsock_vm_rx(sender, credential, __ARCH_ACTV_RPC_RECV_ARG(const void*, hdr), __ARCH_ACTV_RPC_RECV_ARG(size_t, hdr_len), __ARCH_ACTV_RPC_RECV_ARG(const void*, data), __ARCH_ACTV_RPC_RECV_ARG(size_t, len));
}

#endif /* __ARCH_RPC_REQUIRE_BRIDGE_HDLR */
#if defined(__sysif_server_uvmm_no_compat32_handlers__) && defined(__sysif_server_uvmm_no_ilp32_handlers__)
__ro_after_init_hdlr union uvmm_hdlrs_union sysif_uvmm_actvhdlr_table[__uvmm_method_MAX * __uvmm_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open)
	},
	[ __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close)
	},
	[ __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_get_info = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_get_info)
	},
	[ __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[ __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[ __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[ __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[ __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[ __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[ __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[ __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_pause = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_pause)
	},
	[ __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_unpause = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_unpause)
	},
	[ __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_resume = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_resume)
	},
	[ __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown)
	},
	[ __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[ __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm)
	},
	[ __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[ __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[ __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[ __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats)
	},
	[ __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[ __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless)
	},
	[ __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgehdlr_uvmmhandler_vsock_vm_rx = (&__actvbridgehdlr_uvmmhandler_vsock_vm_rx)
	}
};

__ro_after_init_hdlr union uvmm_hdlrs_union sysif_uvmm_actvcapcallhdlr_table[__uvmm_method_MAX * __uvmm_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open)
	},
	[ __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close)
	},
	[ __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info)
	},
	[ __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[ __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[ __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[ __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[ __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[ __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[ __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[ __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause)
	},
	[ __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause)
	},
	[ __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume)
	},
	[ __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown)
	},
	[ __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[ __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm)
	},
	[ __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[ __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[ __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[ __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats)
	},
	[ __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[ __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless)
	},
	[ __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx = (&__actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx)
	}
};

#else
#ifndef __ARCH_RPC_REQUIRE_BRIDGE_HDLR
#error "Only support 64-bit server, build for aarch64 please!"
#endif
#if !defined(__sysif_server_uvmm_no_compat32_handlers__) && defined(__sysif_server_uvmm_no_ilp32_handlers__)
__ro_after_init_hdlr union uvmm_hdlrs_union sysif_uvmm_actvhdlr_table[__uvmm_method_MAX * __uvmm_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open)
	},
	[ __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close)
	},
	[ __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_get_info = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_get_info)
	},
	[ __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[ __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[ __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[ __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[ __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[ __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[ __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[ __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_pause = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_pause)
	},
	[ __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_unpause = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_unpause)
	},
	[ __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_resume = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_resume)
	},
	[ __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown)
	},
	[ __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[ __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm)
	},
	[ __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[ __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[ __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[ __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats)
	},
	[ __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[ __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless)
	},
	[ __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgehdlr_uvmmhandler_vsock_vm_rx = (&__actvbridgehdlr_uvmmhandler_vsock_vm_rx)
	},
	[__uvmm_method_MAX ... __uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __uvmm_method_MAX + __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shm_open)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shm_close)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_get_info = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_get_info)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_pause = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_pause)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_unpause = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_unpause)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_resume = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_resume)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shutdown)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_mclone_to_vm)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_trace_stats)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_tickless)
	},
	[__uvmm_method_MAX + __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgehdlr_compat_uvmmhandler_vsock_vm_rx = (&__actvbridgehdlr_compat_uvmmhandler_vsock_vm_rx)
	}
};

__ro_after_init_hdlr union uvmm_hdlrs_union sysif_uvmm_actvcapcallhdlr_table[__uvmm_method_MAX * __uvmm_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open)
	},
	[ __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close)
	},
	[ __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info)
	},
	[ __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[ __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[ __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[ __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[ __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[ __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[ __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[ __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause)
	},
	[ __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause)
	},
	[ __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume)
	},
	[ __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown)
	},
	[ __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[ __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm)
	},
	[ __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[ __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[ __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[ __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats)
	},
	[ __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[ __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless)
	},
	[ __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx = (&__actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx)
	},
	[__uvmm_method_MAX ... __uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __uvmm_method_MAX + __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shm_open)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shm_close)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_get_info = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_get_info)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_pause = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_pause)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_unpause = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_unpause)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_resume = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_resume)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shutdown)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_mclone_to_vm)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_trace_stats)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_tickless)
	},
	[__uvmm_method_MAX + __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_vsock_vm_rx = (&__actvbridgecapcallhdlr_compat_uvmmhandler_vsock_vm_rx)
	}
};

#endif /* aarch64 + compat */
#if defined(__sysif_server_uvmm_no_compat32_handlers__) && !defined(__sysif_server_uvmm_no_ilp32_handlers__)
__ro_after_init_hdlr union uvmm_hdlrs_union sysif_uvmm_actvhdlr_table[__uvmm_method_MAX * __uvmm_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open)
	},
	[ __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close)
	},
	[ __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_get_info = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_get_info)
	},
	[ __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[ __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[ __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[ __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[ __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[ __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[ __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[ __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_pause = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_pause)
	},
	[ __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_unpause = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_unpause)
	},
	[ __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_resume = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_resume)
	},
	[ __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown)
	},
	[ __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[ __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm)
	},
	[ __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[ __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[ __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[ __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats)
	},
	[ __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[ __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless)
	},
	[ __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgehdlr_uvmmhandler_vsock_vm_rx = (&__actvbridgehdlr_uvmmhandler_vsock_vm_rx)
	},
	[__uvmm_method_MAX ... __uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __uvmm_method_MAX + __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_shm_open)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_shm_close)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_get_info = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_get_info)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_pause = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_pause)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_unpause = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_unpause)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_resume = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_resume)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_shutdown)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_mclone_to_vm)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_trace_stats)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_set_tickless)
	},
	[__uvmm_method_MAX + __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgehdlr_uvmmhandler_vsock_vm_rx = (&__actvbridgehdlr_ilp32_uvmmhandler_vsock_vm_rx)
	}
};

__ro_after_init_hdlr union uvmm_hdlrs_union sysif_uvmm_actvcapcallhdlr_table[__uvmm_method_MAX * __uvmm_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open)
	},
	[ __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close)
	},
	[ __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info)
	},
	[ __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[ __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[ __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[ __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[ __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[ __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[ __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[ __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause)
	},
	[ __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause)
	},
	[ __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume)
	},
	[ __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown)
	},
	[ __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[ __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm)
	},
	[ __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[ __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[ __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[ __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats)
	},
	[ __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[ __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless)
	},
	[ __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx = (&__actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx)
	},
	[__uvmm_method_MAX ... __uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __uvmm_method_MAX + __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_shm_open)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_shm_close)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_get_info)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_pause)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_unpause)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_resume)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_shutdown)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_mclone_to_vm)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_trace_stats)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_set_tickless)
	},
	[__uvmm_method_MAX + __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_vsock_vm_rx)
	}
};

#endif /* aarch64 + ilp32 */
#if !defined(__sysif_server_uvmm_no_compat32_handlers__) && !defined(__sysif_server_uvmm_no_ilp32_handlers__)
__ro_after_init_hdlr union uvmm_hdlrs_union sysif_uvmm_actvhdlr_table[__uvmm_method_MAX * __uvmm_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open)
	},
	[ __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close)
	},
	[ __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_get_info = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_get_info)
	},
	[ __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[ __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[ __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[ __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[ __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[ __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[ __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[ __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_pause = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_pause)
	},
	[ __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_unpause = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_unpause)
	},
	[ __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_resume = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_resume)
	},
	[ __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown)
	},
	[ __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[ __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm)
	},
	[ __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[ __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[ __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[ __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats)
	},
	[ __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[ __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless)
	},
	[ __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgehdlr_uvmmhandler_vsock_vm_rx = (&__actvbridgehdlr_uvmmhandler_vsock_vm_rx)
	},
	[__uvmm_method_MAX ... __uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __uvmm_method_MAX + __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shm_open)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shm_close)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_get_info = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_get_info)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_pause = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_pause)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_unpause = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_unpause)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_resume = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_resume)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_shutdown)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_mclone_to_vm)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_trace_stats)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgehdlr_compat_uvmmhandler_uvmm_vm_set_tickless)
	},
	[__uvmm_method_MAX + __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgehdlr_compat_uvmmhandler_vsock_vm_rx = (&__actvbridgehdlr_compat_uvmmhandler_vsock_vm_rx)
	},
	[2 * __uvmm_method_MAX ... 2 * __uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[2 * __uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... 2 * __uvmm_method_MAX + __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_shm_open)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_shm_close)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_get_info = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_get_info)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_pause = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_pause)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_unpause = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_unpause)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_resume = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_resume)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_shutdown)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_mclone_to_vm)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_trace_stats)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgehdlr_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgehdlr_ilp32_uvmmhandler_uvmm_vm_set_tickless)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgehdlr_uvmmhandler_vsock_vm_rx = (&__actvbridgehdlr_ilp32_uvmmhandler_vsock_vm_rx)
	}
};

__ro_after_init_hdlr union uvmm_hdlrs_union sysif_uvmm_actvcapcallhdlr_table[__uvmm_method_MAX * __uvmm_method_TIMES] = {
	[0 ... __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[ __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open)
	},
	[ __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close)
	},
	[ __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info)
	},
	[ __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[ __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[ __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[ __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[ __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[ __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[ __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[ __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause)
	},
	[ __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause)
	},
	[ __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume)
	},
	[ __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown)
	},
	[ __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[ __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm)
	},
	[ __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[ __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[ __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[ __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats)
	},
	[ __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[ __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[ __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless)
	},
	[ __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx = (&__actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx)
	},
	[__uvmm_method_MAX ... __uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[__uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... __uvmm_method_MAX + __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shm_open)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shm_close)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_get_info = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_get_info)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_pause = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_pause)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_unpause = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_unpause)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_resume = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_resume)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_shutdown)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_mclone_to_vm)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_trace_stats)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[__uvmm_method_MAX + __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgecapcallhdlr_compat_uvmmhandler_uvmm_vm_set_tickless)
	},
	[__uvmm_method_MAX + __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgecapcallhdlr_compat_uvmmhandler_vsock_vm_rx = (&__actvbridgecapcallhdlr_compat_uvmmhandler_vsock_vm_rx)
	},
	[2 * __uvmm_method_MAX ... 2 * __uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE - 1U] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE] = {
		.p_sysif_handle_kcall_entry = (&sysif_actv_pool_handle_grant_change)
	},
	[2 * __uvmm_method_MAX + __ACTV_UPCALL_POOL_GRANT_CHANGE + 1U ... 2 * __uvmm_method_MAX + __uvmm_method_MIN] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_8] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_7] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_6] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_5] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_4] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_3] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_2] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_rsvd_1] = {
		.p_sysif_default_handler = (&sysif_actvhdlr_reserved_default_handler)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_open] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_open = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_shm_open)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_shm_close] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shm_close = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_shm_close)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_get_info] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_get_info = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_get_info)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_process_attach_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_attach_chn = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_attach_chn)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_process_delete_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_delete_chn = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_delete_chn)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_process_notify_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_notify_chn = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_notify_chn)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_process_inject_chn] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_inject_chn = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_inject_chn)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_process_read_chn_status] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_read_chn_status = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_read_chn_status)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vnotify_attach_fd] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_attach_fd = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vnotify_attach_fd)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_handle_vm_process_exit] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_handle_vm_process_exit = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_handle_vm_process_exit)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_pause] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_pause = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_pause)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_unpause] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_unpause = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_unpause)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_resume] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_resume = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_resume)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_shutdown] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_shutdown = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_shutdown)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_vnotify_chns_info] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_vnotify_chns_info = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_vnotify_chns_info)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_mclone_to_vm] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_mclone_to_vm = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_mclone_to_vm)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vnotify_send_ctrl_msg] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_send_ctrl_msg = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vnotify_send_ctrl_msg)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_process_query_chn_status] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_process_query_chn_status = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_process_query_chn_status)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_cpu_hotplug] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_cpu_hotplug = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_cpu_hotplug)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_trace_stats] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_trace_stats = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_trace_stats)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vnotify_update_futex] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vnotify_update_futex = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vnotify_update_futex)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfi] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfi = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_set_trap_wfi)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_set_trap_wfe] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_trap_wfe = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_set_trap_wfe)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_uvmm_vm_set_tickless] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_uvmm_vm_set_tickless = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_uvmm_vm_set_tickless)
	},
	[2 * __uvmm_method_MAX + __uvmm_method_vsock_vm_rx] = {
		.p__actvbridgecapcallhdlr_uvmmhandler_vsock_vm_rx = (&__actvbridgecapcallhdlr_ilp32_uvmmhandler_vsock_vm_rx)
	}
};

#endif /* aarch64 + compat + ilp32 */
#endif /* arm or aarch64 only */
#ifdef __aarch64__
void sysif_show_uvmm_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_shm_open", __uvmm_method_uvmm_vm_shm_open);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_shm_open", __uvmm_method_uvmm_vm_shm_open);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_shm_close", __uvmm_method_uvmm_vm_shm_close);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_shm_close", __uvmm_method_uvmm_vm_shm_close);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_get_info", __uvmm_method_uvmm_vm_get_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_get_info", __uvmm_method_uvmm_vm_get_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_attach_chn", __uvmm_method_uvmm_vm_process_attach_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_attach_chn", __uvmm_method_uvmm_vm_process_attach_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_delete_chn", __uvmm_method_uvmm_vm_process_delete_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_delete_chn", __uvmm_method_uvmm_vm_process_delete_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_notify_chn", __uvmm_method_uvmm_vm_process_notify_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_notify_chn", __uvmm_method_uvmm_vm_process_notify_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_inject_chn", __uvmm_method_uvmm_vm_process_inject_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_inject_chn", __uvmm_method_uvmm_vm_process_inject_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_read_chn_status", __uvmm_method_uvmm_vm_process_read_chn_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_read_chn_status", __uvmm_method_uvmm_vm_process_read_chn_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vnotify_attach_fd", __uvmm_method_uvmm_vnotify_attach_fd);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vnotify_attach_fd", __uvmm_method_uvmm_vnotify_attach_fd);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "handle_vm_process_exit", __uvmm_method_uvmm_handle_vm_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "handle_vm_process_exit", __uvmm_method_uvmm_handle_vm_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_pause", __uvmm_method_uvmm_vm_pause);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_pause", __uvmm_method_uvmm_vm_pause);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_unpause", __uvmm_method_uvmm_vm_unpause);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_unpause", __uvmm_method_uvmm_vm_unpause);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_resume", __uvmm_method_uvmm_vm_resume);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_resume", __uvmm_method_uvmm_vm_resume);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_shutdown", __uvmm_method_uvmm_vm_shutdown);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_shutdown", __uvmm_method_uvmm_vm_shutdown);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_vnotify_chns_info", __uvmm_method_uvmm_vm_vnotify_chns_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_vnotify_chns_info", __uvmm_method_uvmm_vm_vnotify_chns_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "mclone_to_vm", __uvmm_method_uvmm_mclone_to_vm);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "mclone_to_vm", __uvmm_method_uvmm_mclone_to_vm);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vnotify_send_ctrl_msg", __uvmm_method_uvmm_vnotify_send_ctrl_msg);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vnotify_send_ctrl_msg", __uvmm_method_uvmm_vnotify_send_ctrl_msg);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_query_chn_status", __uvmm_method_uvmm_vm_process_query_chn_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_query_chn_status", __uvmm_method_uvmm_vm_process_query_chn_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_cpu_hotplug", __uvmm_method_uvmm_vm_cpu_hotplug);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_cpu_hotplug", __uvmm_method_uvmm_vm_cpu_hotplug);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_trace_stats", __uvmm_method_uvmm_vm_trace_stats);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_trace_stats", __uvmm_method_uvmm_vm_trace_stats);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vnotify_update_futex", __uvmm_method_uvmm_vnotify_update_futex);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vnotify_update_futex", __uvmm_method_uvmm_vnotify_update_futex);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_set_trap_wfi", __uvmm_method_uvmm_vm_set_trap_wfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_set_trap_wfi", __uvmm_method_uvmm_vm_set_trap_wfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_set_trap_wfe", __uvmm_method_uvmm_vm_set_trap_wfe);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_set_trap_wfe", __uvmm_method_uvmm_vm_set_trap_wfe);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_set_tickless", __uvmm_method_uvmm_vm_set_tickless);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_set_tickless", __uvmm_method_uvmm_vm_set_tickless);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "vsock", "vm_rx", __uvmm_method_vsock_vm_rx);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "vsock", "vm_rx", __uvmm_method_vsock_vm_rx);
	hm_info("%s_%s, max method num: %d\n", "uvmm", "method", __uvmm_method_MAX);
}

#elif defined(__arm__)
void sysif_show_uvmm_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_shm_open", __uvmm_method_uvmm_vm_shm_open);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_shm_open", __uvmm_method_uvmm_vm_shm_open);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_shm_close", __uvmm_method_uvmm_vm_shm_close);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_shm_close", __uvmm_method_uvmm_vm_shm_close);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_get_info", __uvmm_method_uvmm_vm_get_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_get_info", __uvmm_method_uvmm_vm_get_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_attach_chn", __uvmm_method_uvmm_vm_process_attach_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_attach_chn", __uvmm_method_uvmm_vm_process_attach_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_delete_chn", __uvmm_method_uvmm_vm_process_delete_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_delete_chn", __uvmm_method_uvmm_vm_process_delete_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_notify_chn", __uvmm_method_uvmm_vm_process_notify_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_notify_chn", __uvmm_method_uvmm_vm_process_notify_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_inject_chn", __uvmm_method_uvmm_vm_process_inject_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_inject_chn", __uvmm_method_uvmm_vm_process_inject_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_read_chn_status", __uvmm_method_uvmm_vm_process_read_chn_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_read_chn_status", __uvmm_method_uvmm_vm_process_read_chn_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vnotify_attach_fd", __uvmm_method_uvmm_vnotify_attach_fd);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vnotify_attach_fd", __uvmm_method_uvmm_vnotify_attach_fd);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "handle_vm_process_exit", __uvmm_method_uvmm_handle_vm_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "handle_vm_process_exit", __uvmm_method_uvmm_handle_vm_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_pause", __uvmm_method_uvmm_vm_pause);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_pause", __uvmm_method_uvmm_vm_pause);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_unpause", __uvmm_method_uvmm_vm_unpause);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_unpause", __uvmm_method_uvmm_vm_unpause);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_resume", __uvmm_method_uvmm_vm_resume);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_resume", __uvmm_method_uvmm_vm_resume);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_shutdown", __uvmm_method_uvmm_vm_shutdown);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_shutdown", __uvmm_method_uvmm_vm_shutdown);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_vnotify_chns_info", __uvmm_method_uvmm_vm_vnotify_chns_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_vnotify_chns_info", __uvmm_method_uvmm_vm_vnotify_chns_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "mclone_to_vm", __uvmm_method_uvmm_mclone_to_vm);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "mclone_to_vm", __uvmm_method_uvmm_mclone_to_vm);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vnotify_send_ctrl_msg", __uvmm_method_uvmm_vnotify_send_ctrl_msg);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vnotify_send_ctrl_msg", __uvmm_method_uvmm_vnotify_send_ctrl_msg);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_query_chn_status", __uvmm_method_uvmm_vm_process_query_chn_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_query_chn_status", __uvmm_method_uvmm_vm_process_query_chn_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_cpu_hotplug", __uvmm_method_uvmm_vm_cpu_hotplug);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_cpu_hotplug", __uvmm_method_uvmm_vm_cpu_hotplug);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_trace_stats", __uvmm_method_uvmm_vm_trace_stats);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_trace_stats", __uvmm_method_uvmm_vm_trace_stats);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vnotify_update_futex", __uvmm_method_uvmm_vnotify_update_futex);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vnotify_update_futex", __uvmm_method_uvmm_vnotify_update_futex);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_set_trap_wfi", __uvmm_method_uvmm_vm_set_trap_wfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_set_trap_wfi", __uvmm_method_uvmm_vm_set_trap_wfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_set_trap_wfe", __uvmm_method_uvmm_vm_set_trap_wfe);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_set_trap_wfe", __uvmm_method_uvmm_vm_set_trap_wfe);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_set_tickless", __uvmm_method_uvmm_vm_set_tickless);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_set_tickless", __uvmm_method_uvmm_vm_set_tickless);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "vsock", "vm_rx", __uvmm_method_vsock_vm_rx);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "vsock", "vm_rx", __uvmm_method_vsock_vm_rx);
	hm_info("%s_%s, max method num: %d\n", "uvmm", "method", __uvmm_method_MAX);
}

#else
void sysif_show_uvmm_methods(void)
{
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_shm_open", __uvmm_method_uvmm_vm_shm_open);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_shm_open", __uvmm_method_uvmm_vm_shm_open);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_shm_close", __uvmm_method_uvmm_vm_shm_close);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_shm_close", __uvmm_method_uvmm_vm_shm_close);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_get_info", __uvmm_method_uvmm_vm_get_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_get_info", __uvmm_method_uvmm_vm_get_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_attach_chn", __uvmm_method_uvmm_vm_process_attach_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_attach_chn", __uvmm_method_uvmm_vm_process_attach_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_delete_chn", __uvmm_method_uvmm_vm_process_delete_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_delete_chn", __uvmm_method_uvmm_vm_process_delete_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_notify_chn", __uvmm_method_uvmm_vm_process_notify_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_notify_chn", __uvmm_method_uvmm_vm_process_notify_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_inject_chn", __uvmm_method_uvmm_vm_process_inject_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_inject_chn", __uvmm_method_uvmm_vm_process_inject_chn);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_read_chn_status", __uvmm_method_uvmm_vm_process_read_chn_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_read_chn_status", __uvmm_method_uvmm_vm_process_read_chn_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vnotify_attach_fd", __uvmm_method_uvmm_vnotify_attach_fd);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vnotify_attach_fd", __uvmm_method_uvmm_vnotify_attach_fd);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "handle_vm_process_exit", __uvmm_method_uvmm_handle_vm_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "handle_vm_process_exit", __uvmm_method_uvmm_handle_vm_process_exit);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_pause", __uvmm_method_uvmm_vm_pause);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_pause", __uvmm_method_uvmm_vm_pause);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_unpause", __uvmm_method_uvmm_vm_unpause);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_unpause", __uvmm_method_uvmm_vm_unpause);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_resume", __uvmm_method_uvmm_vm_resume);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_resume", __uvmm_method_uvmm_vm_resume);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_shutdown", __uvmm_method_uvmm_vm_shutdown);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_shutdown", __uvmm_method_uvmm_vm_shutdown);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_vnotify_chns_info", __uvmm_method_uvmm_vm_vnotify_chns_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_vnotify_chns_info", __uvmm_method_uvmm_vm_vnotify_chns_info);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "mclone_to_vm", __uvmm_method_uvmm_mclone_to_vm);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "mclone_to_vm", __uvmm_method_uvmm_mclone_to_vm);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vnotify_send_ctrl_msg", __uvmm_method_uvmm_vnotify_send_ctrl_msg);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vnotify_send_ctrl_msg", __uvmm_method_uvmm_vnotify_send_ctrl_msg);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_process_query_chn_status", __uvmm_method_uvmm_vm_process_query_chn_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_process_query_chn_status", __uvmm_method_uvmm_vm_process_query_chn_status);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_cpu_hotplug", __uvmm_method_uvmm_vm_cpu_hotplug);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_cpu_hotplug", __uvmm_method_uvmm_vm_cpu_hotplug);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_trace_stats", __uvmm_method_uvmm_vm_trace_stats);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_trace_stats", __uvmm_method_uvmm_vm_trace_stats);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vnotify_update_futex", __uvmm_method_uvmm_vnotify_update_futex);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vnotify_update_futex", __uvmm_method_uvmm_vnotify_update_futex);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_set_trap_wfi", __uvmm_method_uvmm_vm_set_trap_wfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_set_trap_wfi", __uvmm_method_uvmm_vm_set_trap_wfi);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_set_trap_wfe", __uvmm_method_uvmm_vm_set_trap_wfe);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_set_trap_wfe", __uvmm_method_uvmm_vm_set_trap_wfe);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "uvmm", "vm_set_tickless", __uvmm_method_uvmm_vm_set_tickless);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "uvmm", "vm_set_tickless", __uvmm_method_uvmm_vm_set_tickless);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvcall", "uvmmcall", "vsock", "vm_rx", __uvmm_method_vsock_vm_rx);
	hm_debug("method is %s_%s_%s_%s, callnum is %u\n", "actvhdlr", "uvmmhandler", "vsock", "vm_rx", __uvmm_method_vsock_vm_rx);
	hm_info("%s_%s, max method num: %d\n", "uvmm", "method", __uvmm_method_MAX);
}

#endif
#endif
#endif
