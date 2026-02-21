/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for uvmm
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../kernel/hongmeng/hm-verif-kernel/ulibs/include/libsysif/uvmm/api.sysif -I ../kernel/hongmeng/hm-verif-kernel/ulibs/include --api-dest ../image/usr/include/generated/sysif_client/uvmm/libsysif/uvmm/api.h --server-dest ../image/usr/include/generated/sysif_server/uvmm/libsysif/uvmm/server.h --dump-callno ../image/usr/include/generated/dump_info/uvmm/libsysif/uvmm/l2h_map.h --legacy-mode
* Create: Fri Oct 31 10:38:30 2025
*/
#ifndef LIBSYSIF_UVMM_API_H
#define LIBSYSIF_UVMM_API_H
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

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_shm_open(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_shm_open(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_shm_open(unsigned long long __rpc_info, void *__buf, int shm_id, unsigned int flags, unsigned short acc_mode)
{
	int __ret;
	int __maybe_unused __actv_local_var_shm_id = shm_id;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned short __maybe_unused __actv_local_var_acc_mode = acc_mode;
	__ret = ____actvcall_uvmmcall_uvmm_vm_shm_open(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(shm_id, __actv_local_var_shm_id), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(acc_mode, __actv_local_var_acc_mode));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_shm_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, int shm_id, unsigned int flags, unsigned short acc_mode)
{
	int __ret;
	int __maybe_unused __actv_local_var_shm_id = shm_id;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned short __maybe_unused __actv_local_var_acc_mode = acc_mode;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_shm_open(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(shm_id, __actv_local_var_shm_id), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(acc_mode, __actv_local_var_acc_mode));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_shm_close(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_shm_close(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_shm_close(unsigned long long __rpc_info, void *__buf, int shm_id)
{
	int __ret;
	int __maybe_unused __actv_local_var_shm_id = shm_id;
	__ret = ____actvcall_uvmmcall_uvmm_vm_shm_close(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(shm_id, __actv_local_var_shm_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_shm_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, int shm_id)
{
	int __ret;
	int __maybe_unused __actv_local_var_shm_id = shm_id;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_shm_close(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(shm_id, __actv_local_var_shm_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_get_info(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_get_info(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_get_info(unsigned long long __rpc_info, void *__buf, enum vminfo_option opt, struct bunch_ipc_attr attr)
{
	int __ret;
	enum vminfo_option __maybe_unused __actv_local_var_opt = opt;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcall_uvmmcall_uvmm_vm_get_info(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(opt, __actv_local_var_opt), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_get_info(rref_t __rref, unsigned long long __rpc_info, void *__buf, enum vminfo_option opt, struct bunch_ipc_attr attr)
{
	int __ret;
	enum vminfo_option __maybe_unused __actv_local_var_opt = opt;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_get_info(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(opt, __actv_local_var_opt), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_process_attach_chn(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_process_attach_chn(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_process_attach_chn(unsigned long long __rpc_info, void *__buf, struct vm_attach_chn_arg_s args)
{
	int __ret;
	struct vm_attach_chn_arg_s __maybe_unused __actv_local_var_args = args;
	__ret = ____actvcall_uvmmcall_uvmm_vm_process_attach_chn(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_process_attach_chn(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct vm_attach_chn_arg_s args)
{
	int __ret;
	struct vm_attach_chn_arg_s __maybe_unused __actv_local_var_args = args;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_process_attach_chn(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_process_delete_chn(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_process_delete_chn(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_process_delete_chn(unsigned long long __rpc_info, void *__buf, unsigned int chn_id, bool sync, __u64 ts)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_chn_id = chn_id;
	bool __maybe_unused __actv_local_var_sync = sync;
	__u64 __maybe_unused __actv_local_var_ts = ts;
	__ret = ____actvcall_uvmmcall_uvmm_vm_process_delete_chn(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(chn_id, __actv_local_var_chn_id), __ARCH_ACTV_RPC_PASS_ARG(sync, __actv_local_var_sync), __ARCH_ACTV_RPC_PASS_ARG(ts, __actv_local_var_ts));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_process_delete_chn(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int chn_id, bool sync, __u64 ts)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_chn_id = chn_id;
	bool __maybe_unused __actv_local_var_sync = sync;
	__u64 __maybe_unused __actv_local_var_ts = ts;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_process_delete_chn(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(chn_id, __actv_local_var_chn_id), __ARCH_ACTV_RPC_PASS_ARG(sync, __actv_local_var_sync), __ARCH_ACTV_RPC_PASS_ARG(ts, __actv_local_var_ts));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_process_notify_chn(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_process_notify_chn(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_process_notify_chn(unsigned long long __rpc_info, void *__buf, unsigned int chn_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_chn_id = chn_id;
	__ret = ____actvcall_uvmmcall_uvmm_vm_process_notify_chn(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(chn_id, __actv_local_var_chn_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_process_notify_chn(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int chn_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_chn_id = chn_id;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_process_notify_chn(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(chn_id, __actv_local_var_chn_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_process_inject_chn(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_process_inject_chn(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_process_inject_chn(unsigned long long __rpc_info, void *__buf, unsigned int chn_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_chn_id = chn_id;
	__ret = ____actvcall_uvmmcall_uvmm_vm_process_inject_chn(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(chn_id, __actv_local_var_chn_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_process_inject_chn(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int chn_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_chn_id = chn_id;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_process_inject_chn(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(chn_id, __actv_local_var_chn_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_process_read_chn_status(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_process_read_chn_status(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_process_read_chn_status(unsigned long long __rpc_info, void *__buf, unsigned int chn_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_chn_id = chn_id;
	__ret = ____actvcall_uvmmcall_uvmm_vm_process_read_chn_status(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(chn_id, __actv_local_var_chn_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_process_read_chn_status(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int chn_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_chn_id = chn_id;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_process_read_chn_status(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(chn_id, __actv_local_var_chn_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vnotify_attach_fd(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vnotify_attach_fd(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vnotify_attach_fd(unsigned long long __rpc_info, void *__buf, unsigned int chn_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_chn_id = chn_id;
	__ret = ____actvcall_uvmmcall_uvmm_vnotify_attach_fd(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(chn_id, __actv_local_var_chn_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vnotify_attach_fd(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int chn_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_chn_id = chn_id;
	__ret = ____actvcapcall_uvmmcall_uvmm_vnotify_attach_fd(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(chn_id, __actv_local_var_chn_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_handle_vm_process_exit(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_handle_vm_process_exit(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_handle_vm_process_exit(unsigned long long __rpc_info, void *__buf, __u32 cnode_idx)
{
	int __ret;
	__u32 __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	__ret = ____actvcall_uvmmcall_uvmm_handle_vm_process_exit(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_handle_vm_process_exit(rref_t __rref, unsigned long long __rpc_info, void *__buf, __u32 cnode_idx)
{
	int __ret;
	__u32 __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	__ret = ____actvcapcall_uvmmcall_uvmm_handle_vm_process_exit(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_pause(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_pause(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_pause(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_uvmmcall_uvmm_vm_pause(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_pause(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_pause(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_unpause(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_unpause(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_unpause(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_uvmmcall_uvmm_vm_unpause(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_unpause(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_unpause(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_resume(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_resume(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_resume(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_uvmmcall_uvmm_vm_resume(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_resume(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_resume(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_shutdown(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_shutdown(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_shutdown(unsigned long long __rpc_info, void *__buf, struct vm_shutdown_arg_s args)
{
	int __ret;
	struct vm_shutdown_arg_s __maybe_unused __actv_local_var_args = args;
	__ret = ____actvcall_uvmmcall_uvmm_vm_shutdown(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_shutdown(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct vm_shutdown_arg_s args)
{
	int __ret;
	struct vm_shutdown_arg_s __maybe_unused __actv_local_var_args = args;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_shutdown(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_vnotify_chns_info(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_vnotify_chns_info(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_vnotify_chns_info(unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr)
{
	int __ret;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcall_uvmmcall_uvmm_vm_vnotify_chns_info(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_vnotify_chns_info(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct bunch_ipc_attr attr)
{
	int __ret;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_vnotify_chns_info(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_mclone_to_vm(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_mclone_to_vm(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_mclone_to_vm(unsigned long long __rpc_info, void *__buf, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot)
{
	int __ret;
	uint64_t __maybe_unused __actv_local_var_src_va = src_va;
	uint64_t __maybe_unused __actv_local_var_tgt_gpa = tgt_gpa;
	uint64_t __maybe_unused __actv_local_var_len = len;
	uint32_t __maybe_unused __actv_local_var_prot = prot;
	__ret = ____actvcall_uvmmcall_uvmm_mclone_to_vm(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(src_va, __actv_local_var_src_va), __ARCH_ACTV_RPC_PASS_ARG(tgt_gpa, __actv_local_var_tgt_gpa), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(prot, __actv_local_var_prot));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_mclone_to_vm(rref_t __rref, unsigned long long __rpc_info, void *__buf, uint64_t src_va, uint64_t tgt_gpa, uint64_t len, uint32_t prot)
{
	int __ret;
	uint64_t __maybe_unused __actv_local_var_src_va = src_va;
	uint64_t __maybe_unused __actv_local_var_tgt_gpa = tgt_gpa;
	uint64_t __maybe_unused __actv_local_var_len = len;
	uint32_t __maybe_unused __actv_local_var_prot = prot;
	__ret = ____actvcapcall_uvmmcall_uvmm_mclone_to_vm(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(src_va, __actv_local_var_src_va), __ARCH_ACTV_RPC_PASS_ARG(tgt_gpa, __actv_local_var_tgt_gpa), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(prot, __actv_local_var_prot));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_uvmmcall_uvmm_vnotify_send_ctrl_msg(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_process_query_chn_status(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_process_query_chn_status(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_process_query_chn_status(unsigned long long __rpc_info, void *__buf, unsigned int chn_id, unsigned int query_type)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_chn_id = chn_id;
	unsigned int __maybe_unused __actv_local_var_query_type = query_type;
	__ret = ____actvcall_uvmmcall_uvmm_vm_process_query_chn_status(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(chn_id, __actv_local_var_chn_id), __ARCH_ACTV_RPC_PASS_ARG(query_type, __actv_local_var_query_type));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_process_query_chn_status(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int chn_id, unsigned int query_type)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_chn_id = chn_id;
	unsigned int __maybe_unused __actv_local_var_query_type = query_type;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_process_query_chn_status(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(chn_id, __actv_local_var_chn_id), __ARCH_ACTV_RPC_PASS_ARG(query_type, __actv_local_var_query_type));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_cpu_hotplug(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_cpu_hotplug(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_cpu_hotplug(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_uvmmcall_uvmm_vm_cpu_hotplug(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_cpu_hotplug(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_cpu_hotplug(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_trace_stats(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_trace_stats(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_trace_stats(unsigned long long __rpc_info, void *__buf, int act, struct bunch_ipc_attr attr, uint64_t args)
{
	int __ret;
	int __maybe_unused __actv_local_var_act = act;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	uint64_t __maybe_unused __actv_local_var_args = args;
	__ret = ____actvcall_uvmmcall_uvmm_vm_trace_stats(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(act, __actv_local_var_act), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr), __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_trace_stats(rref_t __rref, unsigned long long __rpc_info, void *__buf, int act, struct bunch_ipc_attr attr, uint64_t args)
{
	int __ret;
	int __maybe_unused __actv_local_var_act = act;
	struct bunch_ipc_attr __maybe_unused __actv_local_var_attr = attr;
	uint64_t __maybe_unused __actv_local_var_args = args;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_trace_stats(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(act, __actv_local_var_act), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr), __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vnotify_update_futex(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vnotify_update_futex(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vnotify_update_futex(unsigned long long __rpc_info, void *__buf, unsigned int chn_id, const int *futex, bool shared)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_chn_id = chn_id;
	__maybe_unused const int *__actv_local_var_futex = futex;
	bool __maybe_unused __actv_local_var_shared = shared;
	__ret = ____actvcall_uvmmcall_uvmm_vnotify_update_futex(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(chn_id, __actv_local_var_chn_id), __ARCH_ACTV_RPC_PASS_ARG(futex, __actv_local_var_futex), __ARCH_ACTV_RPC_PASS_ARG(shared, __actv_local_var_shared));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vnotify_update_futex(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int chn_id, const int *futex, bool shared)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_chn_id = chn_id;
	__maybe_unused const int *__actv_local_var_futex = futex;
	bool __maybe_unused __actv_local_var_shared = shared;
	__ret = ____actvcapcall_uvmmcall_uvmm_vnotify_update_futex(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(chn_id, __actv_local_var_chn_id), __ARCH_ACTV_RPC_PASS_ARG(futex, __actv_local_var_futex), __ARCH_ACTV_RPC_PASS_ARG(shared, __actv_local_var_shared));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_set_trap_wfi(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_set_trap_wfi(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_set_trap_wfi(unsigned long long __rpc_info, void *__buf, uint64_t state)
{
	int __ret;
	uint64_t __maybe_unused __actv_local_var_state = state;
	__ret = ____actvcall_uvmmcall_uvmm_vm_set_trap_wfi(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(state, __actv_local_var_state));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_set_trap_wfi(rref_t __rref, unsigned long long __rpc_info, void *__buf, uint64_t state)
{
	int __ret;
	uint64_t __maybe_unused __actv_local_var_state = state;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_set_trap_wfi(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(state, __actv_local_var_state));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_set_trap_wfe(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_set_trap_wfe(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_set_trap_wfe(unsigned long long __rpc_info, void *__buf, uint64_t state)
{
	int __ret;
	uint64_t __maybe_unused __actv_local_var_state = state;
	__ret = ____actvcall_uvmmcall_uvmm_vm_set_trap_wfe(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(state, __actv_local_var_state));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_set_trap_wfe(rref_t __rref, unsigned long long __rpc_info, void *__buf, uint64_t state)
{
	int __ret;
	uint64_t __maybe_unused __actv_local_var_state = state;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_set_trap_wfe(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(state, __actv_local_var_state));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_uvmm_vm_set_tickless(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_uvmm_vm_set_tickless(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_uvmm_vm_set_tickless(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_uvmmcall_uvmm_vm_set_tickless(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_uvmm_vm_set_tickless(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_uvmmcall_uvmm_vm_set_tickless(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_uvmmcall_vsock_vm_rx(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_uvmmcall_vsock_vm_rx(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_uvmmcall_vsock_vm_rx(unsigned long long __rpc_info, void *__buf, const void *hdr, size_t hdr_len, const void *data, size_t len)
{
	int __ret;
	__maybe_unused const void *__actv_local_var_hdr = hdr;
	size_t __maybe_unused __actv_local_var_hdr_len = hdr_len;
	__maybe_unused const void *__actv_local_var_data = data;
	size_t __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcall_uvmmcall_vsock_vm_rx(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(hdr, __actv_local_var_hdr), __ARCH_ACTV_RPC_PASS_ARG(hdr_len, __actv_local_var_hdr_len), __ARCH_ACTV_RPC_PASS_ARG(data, __actv_local_var_data), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

int __internal_actvcall __actvcapcall_uvmmcall_vsock_vm_rx(rref_t __rref, unsigned long long __rpc_info, void *__buf, const void *hdr, size_t hdr_len, const void *data, size_t len)
{
	int __ret;
	__maybe_unused const void *__actv_local_var_hdr = hdr;
	size_t __maybe_unused __actv_local_var_hdr_len = hdr_len;
	__maybe_unused const void *__actv_local_var_data = data;
	size_t __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcapcall_uvmmcall_vsock_vm_rx(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(hdr, __actv_local_var_hdr), __ARCH_ACTV_RPC_PASS_ARG(hdr_len, __actv_local_var_hdr_len), __ARCH_ACTV_RPC_PASS_ARG(data, __actv_local_var_data), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

#endif
#endif
