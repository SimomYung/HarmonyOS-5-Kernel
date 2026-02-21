/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for devhost
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../ulibs/include/libsysif/devhost/api.sysif -I ../ulibs/include --api-dest ../image/usr/include/generated/sysif_client/devhost/libsysif/devhost/api.h --server-dest ../image/usr/include/generated/sysif_server/devhost/libsysif/devhost/server.h --dump-callno ../image/usr/include/generated/dump_info/devhost/libsysif/devhost/l2h_map.h --legacy-mode
* Create: Mon Sep 22 11:44:32 2025
*/
#ifndef LIBSYSIF_DEVHOST_API_H
#define LIBSYSIF_DEVHOST_API_H
#ifndef LIBSYSIF_DEVHOST_COMMON_H
#define LIBSYSIF_DEVHOST_COMMON_H
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
#include <libhmpm/shutdown.h>
#include <libhmpm/wl_stat.h>
#include <libsysif/devmgr/types.h>
#include <libhmpm/dubai.h>


#define DEVHOST_ENTROPY_DATA_MAX_LEN	64
struct devhost_entropy_data {
	unsigned char buf[DEVHOST_ENTROPY_DATA_MAX_LEN];
	unsigned int len;
	/* health test fail count */
	unsigned int fail_count;
};




#define DEV_PIO 1U



#include <stdint.h>

#include <hmkernel/types.h>
#include <hmkernel/capability.h>

#include <libsysif/devhost/bd_types.h>



#include <sys/socket.h>




#include <hmsysmgr/eventmgr/event_def.h>



struct kstate_binder_info {
	int caller_pid;
	int callee_pid;
	unsigned int is_freeze;
	unsigned int type;
	unsigned int flags;
	unsigned int callee_level;
};






#define TASK_COMM_LEN 16
#define FENCE_NAME_LEN 32
#define FENCE_ARRAY_LEN 4
#define DMA_BUF_NAME_LEN 32

struct dma_buf_info {
	size_t size;
	unsigned long long magic;
	int allocator_pid;
	char allocator_comm[TASK_COMM_LEN];
};

struct dma_fence_info {
	char fence_name[FENCE_NAME_LEN];
	unsigned long i_ino;
	unsigned int total_num_fences;
	unsigned int num_fences;
};

struct fences_info {
	char timeline_name[FENCE_NAME_LEN];
	char driver_name[FENCE_NAME_LEN];
	int status;
	size_t timestamp;
};

struct sync_file_info {
	struct dma_fence_info dma_fence_info;
	struct fences_info fences_array[FENCE_ARRAY_LEN];
};

struct ion_info {
	size_t size;
	unsigned long i_ino;
	int allocator_tgid;
	int allocator_pid;
	char exp_task_comm[TASK_COMM_LEN];
	char exp_thread_comm[TASK_COMM_LEN];
	char name[DMA_BUF_NAME_LEN];
	char exp_name[DMA_BUF_NAME_LEN];
	bool reclaimable;
	bool is_reclaimed;
};

struct devhost_mem_info {
	unsigned long long ion_total_cache;
	unsigned long long ion_total_used;
	unsigned long dmaheap_total_cache;
	unsigned long dmaheap_total_used;
	unsigned long dmaheap_freelist_cache;
	unsigned long gpu_total_used;
	unsigned long vmalloc_total;
	unsigned long vmalloc_used;
	unsigned long long slab_total;
	unsigned long long slab_unreclaimable;
	unsigned long long slab_reclaimable;
	unsigned long rsv_total_used;
	unsigned long buddy_total;
	unsigned long buddy_normal_total;
	unsigned long buddy_normal_free;
	unsigned long buddy_dma_total;
	unsigned long buddy_dma_free;
	unsigned long ldk_others_total; /* The exclusion of memory other than buddy and ion/dma_buf within LDK */
	unsigned long discretepool_used;
	unsigned long discretepool_free;
	unsigned long vmemmap_rsv_total;
	unsigned long long hisp_total_used;
	unsigned long long vcodec_total_used;
};

struct gpu_info {
	unsigned long size;
};


#include <hmkernel/types.h>
#include <hmasm/ucontext.h>

raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_core_load_driver_arg_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_core_load_driver_arg_args_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_core_create_device_arg_devname_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_core_create_device_arg_dev_compat_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_core_unload_driver_arg_drvname_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_core_bind_device_arg_dev_compat_is_too_large);
raw_static_assert(sizeof(rref_t) <= 96,
                    drvcall_devhost_fops_open_arg_tgt_rref_is_too_large);
raw_static_assert(sizeof(rref_t) <= 96,
                    drvcall_devhost_fops_poll_arg_fdtable_rref_is_too_large);
raw_static_assert(sizeof(cref_t) <= 96,
                    drvcall_devhost_fops_poll_remove_arg_tcb_cref_is_too_large);
raw_static_assert(sizeof(cref_t) <= 96,
                    drvcall_devhost_fops_epoll_remove_arg_ep_cref_is_too_large);
raw_static_assert(sizeof(uref_t) <= 96,
                    drvcall_devhost_block_register_bio_notify_arg_uref_is_too_large);
raw_static_assert(sizeof(rref_t) <= 96,
                    drvcall_devhost_net_init_arg_tgt_rref_is_too_large);
raw_static_assert(sizeof(struct evtcb_thread_msg_s) <= 96,
                    drvcall_devhost_iaware_notify_thread_msg_arg_msg_is_too_large);
raw_static_assert(sizeof(struct reboot_cmd) <= 96,
                    drvcall_devhost_pm_prepare_arg_cmd_is_too_large);
raw_static_assert(sizeof(struct reboot_cmd) <= 96,
                    drvcall_devhost_pm_reboot_arg_cmd_is_too_large);
raw_static_assert(sizeof(struct wl_stat_queue_info) <= 96,
                    drvcall_devhost_pm_wl_stat_arg_queue_info_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_procfs_open_arg_buf_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_procfs_close_arg_buf_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_procfs_read_arg_ubuf_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_procfs_write_arg_ubuf_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_sysfs_read_arg_ubuf_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_sysfs_write_arg_ubuf_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_transfs_mounted_arg_fs_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_transfs_mounted_arg_dev_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_transfs_mounted_arg_mount_opts_is_too_large);
raw_static_assert(sizeof(mode_t) <= 96,
                    drvcall_devhost_transfs_mkdir_arg_mode_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_transfs_mkdir_arg_actv_dname_is_too_large);
raw_static_assert(sizeof(mode_t) <= 96,
                    drvcall_devhost_transfs_create_arg_mode_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_transfs_create_arg_actv_dname_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_transfs_mknod_arg_actv_dname_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_transfs_link_arg_actv_dname_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_transfs_symlink_arg_actv_dname_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_transfs_symlink_arg_actv_path_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_transfs_rename_arg_actv_dname_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_transfs_lookup_arg_actv_dname_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_transfs_getxattr_arg_actv_name_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_transfs_setxattr_arg_actv_name_is_too_large);
raw_static_assert(sizeof(mode_t) <= 96,
                    drvcall_devhost_transfs_tmpfile_arg_mode_is_too_large);
raw_static_assert(sizeof(struct hm_actv_buf) <= 96,
                    drvcall_devhost_transfs_tmpfile_arg_actv_dname_is_too_large);
raw_static_assert(sizeof(pid_t) <= 96,
                    drvcall_devhost_process_revoke_arg_pid_is_too_large);
raw_static_assert(sizeof(uid_t) <= 96,
                    drvcall_devhost_process_revoke_arg_uid_is_too_large);
struct __actvret_drvcall_devhost_core_load_driver {
};

struct __actvret_drvcall_devhost_core_create_device {
	int devid;
};

struct __actvret_drvcall_devhost_core_unload_driver {
};

struct __actvret_drvcall_devhost_core_bind_device {
};

struct __actvret_drvcall_devhost_core_entropy_read {
	struct devhost_entropy_data data;
};

struct __actvret_drvcall_devhost_rpc_export {
	uref_t uref;
};

struct __actvret_drvcall_devhost_rpc_invoke {
	unsigned long long resp_size;
};

struct __actvret_drvcall_devhost_fops_open {
	unsigned long long filp;
	unsigned long long kshm_key;
	unsigned long kobj_offset;
	unsigned int dev_fops_flags;
};

struct __actvret_drvcall_devhost_fops_close {
};

struct __actvret_drvcall_devhost_fops_read {
	unsigned long long nread;
	unsigned long long pos;
};

struct __actvret_drvcall_devhost_fops_write {
	unsigned long long nwrite;
	unsigned long long pos;
};

struct __actvret_drvcall_devhost_fops_read_iter {
	unsigned long long nread;
	unsigned long long pos;
};

struct __actvret_drvcall_devhost_fops_write_iter {
	unsigned long long nwrite;
	unsigned long long pos;
};

struct __actvret_drvcall_devhost_fops_ioctl {
};

struct __actvret_drvcall_devhost_fops_flush {
};

struct __actvret_drvcall_devhost_fops_fcntl {
};

struct __actvret_drvcall_devhost_fops_llseek {
	long long pos;
};

struct __actvret_drvcall_devhost_fops_mmap {
	unsigned long long rvaddr;
	unsigned int iomem_id;
};

struct __actvret_drvcall_devhost_fops_munmap {
};

struct __actvret_drvcall_devhost_fops_do_page_fault {
};

struct __actvret_drvcall_devhost_fops_mm_release {
};

struct __actvret_drvcall_devhost_fops_sysfs_read {
};

struct __actvret_drvcall_devhost_fops_sysfs_write {
};

struct __actvret_drvcall_devhost_fops_poll {
	unsigned int revents;
};

struct __actvret_drvcall_devhost_fops_poll_remove {
};

struct __actvret_drvcall_devhost_fops_epoll_ctl {
};

struct __actvret_drvcall_devhost_fops_epoll_remove {
};

struct __actvret_drvcall_devhost_fops_epoll_recheck {
	unsigned int revents;
};

struct __actvret_drvcall_devhost_fops_sync_ctrlmem_polllist {
};

struct __actvret_drvcall_devhost_fops_file_get {
};

struct __actvret_drvcall_devhost_block_check_feature {
	unsigned long long supported_features;
};

struct __actvret_drvcall_devhost_block_submit_bio {
};

struct __actvret_drvcall_devhost_block_submit_cmd {
};

struct __actvret_drvcall_devhost_block_run_queue {
};

struct __actvret_drvcall_devhost_block_direct_access {
	unsigned long long paddr;
	unsigned long long ret_size;
};

struct __actvret_drvcall_devhost_block_register_bio_notify {
	unsigned long long submit_fd;
	unsigned long long end_fd;
	unsigned long shm_vaddr;
	unsigned long long obj_cnt;
};

struct __actvret_drvcall_devhost_net_send {
};

struct __actvret_drvcall_devhost_net_ioctl {
};

struct __actvret_drvcall_devhost_net_status {
	struct netdev_status status;
};

struct __actvret_drvcall_devhost_net_prepare {
	unsigned int pool_size;
	unsigned int unit_size;
	unsigned long long shm_id;
	unsigned long long rx_uring_shm_id;
};

struct __actvret_drvcall_devhost_net_init {
};

struct __actvret_drvcall_devhost_net_netlink_recv {
};

struct __actvret_drvcall_devhost_net_socket_register {
};

struct __actvret_drvcall_devhost_net_socket_close {
};

struct __actvret_drvcall_devhost_net_socket_connect {
};

struct __actvret_drvcall_devhost_net_socket_bind {
};

struct __actvret_drvcall_devhost_net_socket_listen {
};

struct __actvret_drvcall_devhost_net_socket_accept {
	unsigned long long filp;
	unsigned long long kshm_key;
	unsigned long kobj_offset;
};

struct __actvret_drvcall_devhost_net_socket_shutdown {
};

struct __actvret_drvcall_devhost_net_socket_ioctl {
};

struct __actvret_drvcall_devhost_net_socket_dev_ioctl {
};

struct __actvret_drvcall_devhost_net_socket_fcntl {
};

struct __actvret_drvcall_devhost_net_socket_send {
};

struct __actvret_drvcall_devhost_net_socket_recv {
};

struct __actvret_drvcall_devhost_net_socket_sendmmsg {
};

struct __actvret_drvcall_devhost_net_socket_recvmmsg {
};

struct __actvret_drvcall_devhost_net_socket_sendmsg {
};

struct __actvret_drvcall_devhost_net_socket_recvmsg {
};

struct __actvret_drvcall_devhost_net_socket_sendto {
};

struct __actvret_drvcall_devhost_net_socket_recvfrom {
};

struct __actvret_drvcall_devhost_net_socket_readv {
};

struct __actvret_drvcall_devhost_net_socket_writev {
};

struct __actvret_drvcall_devhost_net_socket_getsockopt {
};

struct __actvret_drvcall_devhost_net_socket_setsockopt {
};

struct __actvret_drvcall_devhost_net_socket_getsockname {
};

struct __actvret_drvcall_devhost_net_socket_getpeername {
};

struct __actvret_drvcall_devhost_net_socket_fchown {
};

struct __actvret_drvcall_devhost_net_socket_fstat {
};

struct __actvret_drvcall_devhost_net_socket_socket {
	unsigned long long filp;
	unsigned long long kshm_key;
	unsigned long kobj_offset;
};

struct __actvret_drvcall_devhost_net_socket_alloc_new_ns {
};

struct __actvret_drvcall_devhost_net_socket_destroy_ns {
};

struct __actvret_drvcall_devhost_net_socket_bpf {
};

struct __actvret_drvcall_devhost_iaware_notify_thread_msg {
};

struct __actvret_drvcall_devhost_kstate_binderinfo_notify {
};

struct __actvret_drvcall_devhost_kstate_killinfo_notify {
};

struct __actvret_drvcall_devhost_kstate_freeze_binder_info_notify {
};

struct __actvret_drvcall_devhost_kstate_request_anco_thaw {
};

struct __actvret_drvcall_devhost_pm_action {
};

struct __actvret_drvcall_devhost_pm_dubai_hwlog {
};

struct __actvret_drvcall_devhost_pm_prepare {
};

struct __actvret_drvcall_devhost_pm_power_off {
};

struct __actvret_drvcall_devhost_pm_reboot {
};

struct __actvret_drvcall_devhost_pm_cpuhp_action {
};

struct __actvret_drvcall_devhost_pm_wl_stat {
	size_t enqueue_cnt;
};

struct __actvret_drvcall_devhost_procfs_open {
	unsigned long long fd;
};

struct __actvret_drvcall_devhost_procfs_close {
};

struct __actvret_drvcall_devhost_procfs_read {
	unsigned long long rsize;
};

struct __actvret_drvcall_devhost_procfs_write {
	unsigned long long wsize;
};

struct __actvret_drvcall_devhost_sysfs_open {
	unsigned long long fd;
};

struct __actvret_drvcall_devhost_sysfs_close {
};

struct __actvret_drvcall_devhost_sysfs_read {
	unsigned long long rsize;
};

struct __actvret_drvcall_devhost_sysfs_write {
	unsigned long long wsize;
};

struct __actvret_drvcall_devhost_sysfs_poll {
};

struct __actvret_drvcall_devhost_clock_settime {
};

struct __actvret_drvcall_devhost_clock_gettime {
};

struct __actvret_drvcall_devhost_clock_getres {
};

struct __actvret_drvcall_devhost_clock_adjtime {
};

struct __actvret_drvcall_devhost_clock_poweroff_alarm {
};

struct __actvret_drvcall_devhost_transfs_mounted {
	struct transfs_m_info_t m_info;
};

struct __actvret_drvcall_devhost_transfs_unmounted {
};

struct __actvret_drvcall_devhost_transfs_drop_caches {
};

struct __actvret_drvcall_devhost_transfs_mkdir {
	struct transfs_dn_info_t dn_info_out;
};

struct __actvret_drvcall_devhost_transfs_rmdir {
};

struct __actvret_drvcall_devhost_transfs_create {
	struct transfs_dn_info_t dn_info_out;
};

struct __actvret_drvcall_devhost_transfs_unlink {
};

struct __actvret_drvcall_devhost_transfs_mknod {
	struct transfs_dn_info_t dn_info_out;
};

struct __actvret_drvcall_devhost_transfs_link {
	struct transfs_dn_info_t dn_info_out;
};

struct __actvret_drvcall_devhost_transfs_symlink {
	struct transfs_dn_info_t dn_info_out;
};

struct __actvret_drvcall_devhost_transfs_statfs {
};

struct __actvret_drvcall_devhost_transfs_rename {
};

struct __actvret_drvcall_devhost_transfs_lookup {
	struct transfs_dn_info_t dn_info_out;
};

struct __actvret_drvcall_devhost_transfs_setattr {
};

struct __actvret_drvcall_devhost_transfs_readlink {
};

struct __actvret_drvcall_devhost_transfs_getattr {
};

struct __actvret_drvcall_devhost_transfs_listxattr {
	long long written;
};

struct __actvret_drvcall_devhost_transfs_getxattr {
	long long nbytes;
};

struct __actvret_drvcall_devhost_transfs_setxattr {
};

struct __actvret_drvcall_devhost_transfs_tmpfile {
	struct transfs_dn_info_t dn_info_out;
};

struct __actvret_drvcall_devhost_transfs_kill_dentry {
};

struct __actvret_drvcall_devhost_transfs_node_put {
};

struct __actvret_drvcall_devhost_transfs_sync {
};

struct __actvret_drvcall_devhost_transfs_write {
	long long nwritten;
};

struct __actvret_drvcall_devhost_transfs_read {
	long long nread;
};

struct __actvret_drvcall_devhost_transfs_write_iter {
	long long nwritten;
};

struct __actvret_drvcall_devhost_transfs_fsync {
};

struct __actvret_drvcall_devhost_transfs_readdir {
	int64_t fpos;
};

struct __actvret_drvcall_devhost_transfs_open {
	unsigned int f_index;
};

struct __actvret_drvcall_devhost_transfs_fallocate {
};

struct __actvret_drvcall_devhost_transfs_close {
};

struct __actvret_drvcall_devhost_transfs_ioctl {
};

struct __actvret_drvcall_devhost_transfs_direct_IO {
	long long nbytes;
};

struct __actvret_drvcall_devhost_transfs_update_opts {
};

struct __actvret_drvcall_devhost_transfs_iommap {
	uintptr_t addr;
	unsigned int iomem_id;
};

struct __actvret_drvcall_devhost_shrinker_query {
};

struct __actvret_drvcall_devhost_shrinker_shrink {
	int cnt;
	int scanned;
};

struct __actvret_drvcall_devhost_process_revoke {
};

struct __actvret_drvcall_devhost_sec_mgmt {
};

struct __actvret_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb {
};

struct __actvret_drvcall_devhost_tests_hkip_protection {
};

struct __actvret_drvcall_devhost_sec_test_hkip_roafter_prot {
};

struct __actvret_drvcall_devhost_sec_sync_fastpath_open {
};

struct __actvret_drvcall_devhost_sec_sync_fastpath_dev {
};

struct __actvret_drvcall_devhost_devfs_read {
};

struct __actvret_drvcall_devhost_devfs_write {
};

struct __actvret_drvcall_devhost_devfs_ioctl {
};

struct __actvret_drvcall_devhost_devfs_readv {
};

struct __actvret_drvcall_devhost_devfs_writev {
};

struct __actvret_drvcall_devhost_devfs_lseek {
};

struct __actvret_drvcall_devhost_devfs_fcntl {
};

#ifdef __arm__
struct __actvret_drvcall_devhost_devfs__llseek {
};

struct __actvret_drvcall_devhost_devfs_fcntl64 {
};

#endif
struct __actvret_drvcall_devhost_trace_enable_disable_ldk_event {
};

struct __actvret_drvcall_devhost_trace_ldk_event_format {
};

struct __actvret_drvcall_devhost_trace_print_ldk_event {
};

struct __actvret_drvcall_devhost_file_info_query_dmabuf {
	struct dma_buf_info dma_buf_info;
};

struct __actvret_drvcall_devhost_file_info_query_fence {
	struct sync_file_info sync_file_info;
};

struct __actvret_drvcall_devhost_file_info_query_ion {
	struct ion_info ion_info;
};

struct __actvret_drvcall_devhost_file_info_query_meminfo {
	struct devhost_mem_info devhost_mem_info;
};

struct __actvret_drvcall_devhost_file_info_lowmem_debug {
};

struct __actvret_drvcall_devhost_file_info_query_gpu {
	struct gpu_info gpu_info;
};

struct __actvret_drvcall_devhost_probe_handle_brk {
};

struct __actvret_drvcall_devhost_probe_handle_ss_brk {
};

struct __actvret_drvcall_devhost_fileguard_message_notify {
};

struct __actvret_drvcall_devhost_posix_vhangup {
};

#ifdef __aarch64__
union __actvret_drvcall_devhost {
	struct __actvret_drvcall_devhost_core_load_driver core_load_driver;
	struct __actvret_drvcall_devhost_core_create_device core_create_device;
	struct __actvret_drvcall_devhost_core_unload_driver core_unload_driver;
	struct __actvret_drvcall_devhost_core_bind_device core_bind_device;
	struct __actvret_drvcall_devhost_core_entropy_read core_entropy_read;
	struct __actvret_drvcall_devhost_rpc_export rpc_export;
	struct __actvret_drvcall_devhost_rpc_invoke rpc_invoke;
	struct __actvret_drvcall_devhost_fops_open fops_open;
	struct __actvret_drvcall_devhost_fops_close fops_close;
	struct __actvret_drvcall_devhost_fops_read fops_read;
	struct __actvret_drvcall_devhost_fops_write fops_write;
	struct __actvret_drvcall_devhost_fops_read_iter fops_read_iter;
	struct __actvret_drvcall_devhost_fops_write_iter fops_write_iter;
	struct __actvret_drvcall_devhost_fops_ioctl fops_ioctl;
	struct __actvret_drvcall_devhost_fops_flush fops_flush;
	struct __actvret_drvcall_devhost_fops_fcntl fops_fcntl;
	struct __actvret_drvcall_devhost_fops_llseek fops_llseek;
	struct __actvret_drvcall_devhost_fops_mmap fops_mmap;
	struct __actvret_drvcall_devhost_fops_munmap fops_munmap;
	struct __actvret_drvcall_devhost_fops_do_page_fault fops_do_page_fault;
	struct __actvret_drvcall_devhost_fops_mm_release fops_mm_release;
	struct __actvret_drvcall_devhost_fops_sysfs_read fops_sysfs_read;
	struct __actvret_drvcall_devhost_fops_sysfs_write fops_sysfs_write;
	struct __actvret_drvcall_devhost_fops_poll fops_poll;
	struct __actvret_drvcall_devhost_fops_poll_remove fops_poll_remove;
	struct __actvret_drvcall_devhost_fops_epoll_ctl fops_epoll_ctl;
	struct __actvret_drvcall_devhost_fops_epoll_remove fops_epoll_remove;
	struct __actvret_drvcall_devhost_fops_epoll_recheck fops_epoll_recheck;
	struct __actvret_drvcall_devhost_fops_sync_ctrlmem_polllist fops_sync_ctrlmem_polllist;
	struct __actvret_drvcall_devhost_fops_file_get fops_file_get;
	struct __actvret_drvcall_devhost_block_check_feature block_check_feature;
	struct __actvret_drvcall_devhost_block_submit_bio block_submit_bio;
	struct __actvret_drvcall_devhost_block_submit_cmd block_submit_cmd;
	struct __actvret_drvcall_devhost_block_run_queue block_run_queue;
	struct __actvret_drvcall_devhost_block_direct_access block_direct_access;
	struct __actvret_drvcall_devhost_block_register_bio_notify block_register_bio_notify;
	struct __actvret_drvcall_devhost_net_send net_send;
	struct __actvret_drvcall_devhost_net_ioctl net_ioctl;
	struct __actvret_drvcall_devhost_net_status net_status;
	struct __actvret_drvcall_devhost_net_prepare net_prepare;
	struct __actvret_drvcall_devhost_net_init net_init;
	struct __actvret_drvcall_devhost_net_netlink_recv net_netlink_recv;
	struct __actvret_drvcall_devhost_net_socket_register net_socket_register;
	struct __actvret_drvcall_devhost_net_socket_close net_socket_close;
	struct __actvret_drvcall_devhost_net_socket_connect net_socket_connect;
	struct __actvret_drvcall_devhost_net_socket_bind net_socket_bind;
	struct __actvret_drvcall_devhost_net_socket_listen net_socket_listen;
	struct __actvret_drvcall_devhost_net_socket_accept net_socket_accept;
	struct __actvret_drvcall_devhost_net_socket_shutdown net_socket_shutdown;
	struct __actvret_drvcall_devhost_net_socket_ioctl net_socket_ioctl;
	struct __actvret_drvcall_devhost_net_socket_dev_ioctl net_socket_dev_ioctl;
	struct __actvret_drvcall_devhost_net_socket_fcntl net_socket_fcntl;
	struct __actvret_drvcall_devhost_net_socket_send net_socket_send;
	struct __actvret_drvcall_devhost_net_socket_recv net_socket_recv;
	struct __actvret_drvcall_devhost_net_socket_sendmmsg net_socket_sendmmsg;
	struct __actvret_drvcall_devhost_net_socket_recvmmsg net_socket_recvmmsg;
	struct __actvret_drvcall_devhost_net_socket_sendmsg net_socket_sendmsg;
	struct __actvret_drvcall_devhost_net_socket_recvmsg net_socket_recvmsg;
	struct __actvret_drvcall_devhost_net_socket_sendto net_socket_sendto;
	struct __actvret_drvcall_devhost_net_socket_recvfrom net_socket_recvfrom;
	struct __actvret_drvcall_devhost_net_socket_readv net_socket_readv;
	struct __actvret_drvcall_devhost_net_socket_writev net_socket_writev;
	struct __actvret_drvcall_devhost_net_socket_getsockopt net_socket_getsockopt;
	struct __actvret_drvcall_devhost_net_socket_setsockopt net_socket_setsockopt;
	struct __actvret_drvcall_devhost_net_socket_getsockname net_socket_getsockname;
	struct __actvret_drvcall_devhost_net_socket_getpeername net_socket_getpeername;
	struct __actvret_drvcall_devhost_net_socket_fchown net_socket_fchown;
	struct __actvret_drvcall_devhost_net_socket_fstat net_socket_fstat;
	struct __actvret_drvcall_devhost_net_socket_socket net_socket_socket;
	struct __actvret_drvcall_devhost_net_socket_alloc_new_ns net_socket_alloc_new_ns;
	struct __actvret_drvcall_devhost_net_socket_destroy_ns net_socket_destroy_ns;
	struct __actvret_drvcall_devhost_net_socket_bpf net_socket_bpf;
	struct __actvret_drvcall_devhost_iaware_notify_thread_msg iaware_notify_thread_msg;
	struct __actvret_drvcall_devhost_kstate_binderinfo_notify kstate_binderinfo_notify;
	struct __actvret_drvcall_devhost_kstate_killinfo_notify kstate_killinfo_notify;
	struct __actvret_drvcall_devhost_kstate_freeze_binder_info_notify kstate_freeze_binder_info_notify;
	struct __actvret_drvcall_devhost_kstate_request_anco_thaw kstate_request_anco_thaw;
	struct __actvret_drvcall_devhost_pm_action pm_action;
	struct __actvret_drvcall_devhost_pm_dubai_hwlog pm_dubai_hwlog;
	struct __actvret_drvcall_devhost_pm_prepare pm_prepare;
	struct __actvret_drvcall_devhost_pm_power_off pm_power_off;
	struct __actvret_drvcall_devhost_pm_reboot pm_reboot;
	struct __actvret_drvcall_devhost_pm_cpuhp_action pm_cpuhp_action;
	struct __actvret_drvcall_devhost_pm_wl_stat pm_wl_stat;
	struct __actvret_drvcall_devhost_procfs_open procfs_open;
	struct __actvret_drvcall_devhost_procfs_close procfs_close;
	struct __actvret_drvcall_devhost_procfs_read procfs_read;
	struct __actvret_drvcall_devhost_procfs_write procfs_write;
	struct __actvret_drvcall_devhost_sysfs_open sysfs_open;
	struct __actvret_drvcall_devhost_sysfs_close sysfs_close;
	struct __actvret_drvcall_devhost_sysfs_read sysfs_read;
	struct __actvret_drvcall_devhost_sysfs_write sysfs_write;
	struct __actvret_drvcall_devhost_sysfs_poll sysfs_poll;
	struct __actvret_drvcall_devhost_clock_settime clock_settime;
	struct __actvret_drvcall_devhost_clock_gettime clock_gettime;
	struct __actvret_drvcall_devhost_clock_getres clock_getres;
	struct __actvret_drvcall_devhost_clock_adjtime clock_adjtime;
	struct __actvret_drvcall_devhost_clock_poweroff_alarm clock_poweroff_alarm;
	struct __actvret_drvcall_devhost_transfs_mounted transfs_mounted;
	struct __actvret_drvcall_devhost_transfs_unmounted transfs_unmounted;
	struct __actvret_drvcall_devhost_transfs_drop_caches transfs_drop_caches;
	struct __actvret_drvcall_devhost_transfs_mkdir transfs_mkdir;
	struct __actvret_drvcall_devhost_transfs_rmdir transfs_rmdir;
	struct __actvret_drvcall_devhost_transfs_create transfs_create;
	struct __actvret_drvcall_devhost_transfs_unlink transfs_unlink;
	struct __actvret_drvcall_devhost_transfs_mknod transfs_mknod;
	struct __actvret_drvcall_devhost_transfs_link transfs_link;
	struct __actvret_drvcall_devhost_transfs_symlink transfs_symlink;
	struct __actvret_drvcall_devhost_transfs_statfs transfs_statfs;
	struct __actvret_drvcall_devhost_transfs_rename transfs_rename;
	struct __actvret_drvcall_devhost_transfs_lookup transfs_lookup;
	struct __actvret_drvcall_devhost_transfs_setattr transfs_setattr;
	struct __actvret_drvcall_devhost_transfs_readlink transfs_readlink;
	struct __actvret_drvcall_devhost_transfs_getattr transfs_getattr;
	struct __actvret_drvcall_devhost_transfs_listxattr transfs_listxattr;
	struct __actvret_drvcall_devhost_transfs_getxattr transfs_getxattr;
	struct __actvret_drvcall_devhost_transfs_setxattr transfs_setxattr;
	struct __actvret_drvcall_devhost_transfs_tmpfile transfs_tmpfile;
	struct __actvret_drvcall_devhost_transfs_kill_dentry transfs_kill_dentry;
	struct __actvret_drvcall_devhost_transfs_node_put transfs_node_put;
	struct __actvret_drvcall_devhost_transfs_sync transfs_sync;
	struct __actvret_drvcall_devhost_transfs_write transfs_write;
	struct __actvret_drvcall_devhost_transfs_read transfs_read;
	struct __actvret_drvcall_devhost_transfs_write_iter transfs_write_iter;
	struct __actvret_drvcall_devhost_transfs_fsync transfs_fsync;
	struct __actvret_drvcall_devhost_transfs_readdir transfs_readdir;
	struct __actvret_drvcall_devhost_transfs_open transfs_open;
	struct __actvret_drvcall_devhost_transfs_fallocate transfs_fallocate;
	struct __actvret_drvcall_devhost_transfs_close transfs_close;
	struct __actvret_drvcall_devhost_transfs_ioctl transfs_ioctl;
	struct __actvret_drvcall_devhost_transfs_direct_IO transfs_direct_IO;
	struct __actvret_drvcall_devhost_transfs_update_opts transfs_update_opts;
	struct __actvret_drvcall_devhost_transfs_iommap transfs_iommap;
	struct __actvret_drvcall_devhost_shrinker_query shrinker_query;
	struct __actvret_drvcall_devhost_shrinker_shrink shrinker_shrink;
	struct __actvret_drvcall_devhost_process_revoke process_revoke;
	struct __actvret_drvcall_devhost_sec_mgmt sec_mgmt;
	struct __actvret_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb sec_test_hkip_seharmony_prot_policydb;
	struct __actvret_drvcall_devhost_tests_hkip_protection tests_hkip_protection;
	struct __actvret_drvcall_devhost_sec_test_hkip_roafter_prot sec_test_hkip_roafter_prot;
	struct __actvret_drvcall_devhost_sec_sync_fastpath_open sec_sync_fastpath_open;
	struct __actvret_drvcall_devhost_sec_sync_fastpath_dev sec_sync_fastpath_dev;
	struct __actvret_drvcall_devhost_devfs_read devfs_read;
	struct __actvret_drvcall_devhost_devfs_write devfs_write;
	struct __actvret_drvcall_devhost_devfs_ioctl devfs_ioctl;
	struct __actvret_drvcall_devhost_devfs_readv devfs_readv;
	struct __actvret_drvcall_devhost_devfs_writev devfs_writev;
	struct __actvret_drvcall_devhost_devfs_lseek devfs_lseek;
	struct __actvret_drvcall_devhost_devfs_fcntl devfs_fcntl;
	struct __actvret_drvcall_devhost_trace_enable_disable_ldk_event trace_enable_disable_ldk_event;
	struct __actvret_drvcall_devhost_trace_ldk_event_format trace_ldk_event_format;
	struct __actvret_drvcall_devhost_trace_print_ldk_event trace_print_ldk_event;
	struct __actvret_drvcall_devhost_file_info_query_dmabuf file_info_query_dmabuf;
	struct __actvret_drvcall_devhost_file_info_query_fence file_info_query_fence;
	struct __actvret_drvcall_devhost_file_info_query_ion file_info_query_ion;
	struct __actvret_drvcall_devhost_file_info_query_meminfo file_info_query_meminfo;
	struct __actvret_drvcall_devhost_file_info_lowmem_debug file_info_lowmem_debug;
	struct __actvret_drvcall_devhost_file_info_query_gpu file_info_query_gpu;
	struct __actvret_drvcall_devhost_probe_handle_brk probe_handle_brk;
	struct __actvret_drvcall_devhost_probe_handle_ss_brk probe_handle_ss_brk;
	struct __actvret_drvcall_devhost_fileguard_message_notify fileguard_message_notify;
	struct __actvret_drvcall_devhost_posix_vhangup posix_vhangup;
};

#elif defined(__arm__)
union __actvret_drvcall_devhost {
	struct __actvret_drvcall_devhost_core_load_driver core_load_driver;
	struct __actvret_drvcall_devhost_core_create_device core_create_device;
	struct __actvret_drvcall_devhost_core_unload_driver core_unload_driver;
	struct __actvret_drvcall_devhost_core_bind_device core_bind_device;
	struct __actvret_drvcall_devhost_core_entropy_read core_entropy_read;
	struct __actvret_drvcall_devhost_rpc_export rpc_export;
	struct __actvret_drvcall_devhost_rpc_invoke rpc_invoke;
	struct __actvret_drvcall_devhost_fops_open fops_open;
	struct __actvret_drvcall_devhost_fops_close fops_close;
	struct __actvret_drvcall_devhost_fops_read fops_read;
	struct __actvret_drvcall_devhost_fops_write fops_write;
	struct __actvret_drvcall_devhost_fops_read_iter fops_read_iter;
	struct __actvret_drvcall_devhost_fops_write_iter fops_write_iter;
	struct __actvret_drvcall_devhost_fops_ioctl fops_ioctl;
	struct __actvret_drvcall_devhost_fops_flush fops_flush;
	struct __actvret_drvcall_devhost_fops_fcntl fops_fcntl;
	struct __actvret_drvcall_devhost_fops_llseek fops_llseek;
	struct __actvret_drvcall_devhost_fops_mmap fops_mmap;
	struct __actvret_drvcall_devhost_fops_munmap fops_munmap;
	struct __actvret_drvcall_devhost_fops_do_page_fault fops_do_page_fault;
	struct __actvret_drvcall_devhost_fops_mm_release fops_mm_release;
	struct __actvret_drvcall_devhost_fops_sysfs_read fops_sysfs_read;
	struct __actvret_drvcall_devhost_fops_sysfs_write fops_sysfs_write;
	struct __actvret_drvcall_devhost_fops_poll fops_poll;
	struct __actvret_drvcall_devhost_fops_poll_remove fops_poll_remove;
	struct __actvret_drvcall_devhost_fops_epoll_ctl fops_epoll_ctl;
	struct __actvret_drvcall_devhost_fops_epoll_remove fops_epoll_remove;
	struct __actvret_drvcall_devhost_fops_epoll_recheck fops_epoll_recheck;
	struct __actvret_drvcall_devhost_fops_sync_ctrlmem_polllist fops_sync_ctrlmem_polllist;
	struct __actvret_drvcall_devhost_fops_file_get fops_file_get;
	struct __actvret_drvcall_devhost_block_check_feature block_check_feature;
	struct __actvret_drvcall_devhost_block_submit_bio block_submit_bio;
	struct __actvret_drvcall_devhost_block_submit_cmd block_submit_cmd;
	struct __actvret_drvcall_devhost_block_run_queue block_run_queue;
	struct __actvret_drvcall_devhost_block_direct_access block_direct_access;
	struct __actvret_drvcall_devhost_block_register_bio_notify block_register_bio_notify;
	struct __actvret_drvcall_devhost_net_send net_send;
	struct __actvret_drvcall_devhost_net_ioctl net_ioctl;
	struct __actvret_drvcall_devhost_net_status net_status;
	struct __actvret_drvcall_devhost_net_prepare net_prepare;
	struct __actvret_drvcall_devhost_net_init net_init;
	struct __actvret_drvcall_devhost_net_netlink_recv net_netlink_recv;
	struct __actvret_drvcall_devhost_net_socket_register net_socket_register;
	struct __actvret_drvcall_devhost_net_socket_close net_socket_close;
	struct __actvret_drvcall_devhost_net_socket_connect net_socket_connect;
	struct __actvret_drvcall_devhost_net_socket_bind net_socket_bind;
	struct __actvret_drvcall_devhost_net_socket_listen net_socket_listen;
	struct __actvret_drvcall_devhost_net_socket_accept net_socket_accept;
	struct __actvret_drvcall_devhost_net_socket_shutdown net_socket_shutdown;
	struct __actvret_drvcall_devhost_net_socket_ioctl net_socket_ioctl;
	struct __actvret_drvcall_devhost_net_socket_dev_ioctl net_socket_dev_ioctl;
	struct __actvret_drvcall_devhost_net_socket_fcntl net_socket_fcntl;
	struct __actvret_drvcall_devhost_net_socket_send net_socket_send;
	struct __actvret_drvcall_devhost_net_socket_recv net_socket_recv;
	struct __actvret_drvcall_devhost_net_socket_sendmmsg net_socket_sendmmsg;
	struct __actvret_drvcall_devhost_net_socket_recvmmsg net_socket_recvmmsg;
	struct __actvret_drvcall_devhost_net_socket_sendmsg net_socket_sendmsg;
	struct __actvret_drvcall_devhost_net_socket_recvmsg net_socket_recvmsg;
	struct __actvret_drvcall_devhost_net_socket_sendto net_socket_sendto;
	struct __actvret_drvcall_devhost_net_socket_recvfrom net_socket_recvfrom;
	struct __actvret_drvcall_devhost_net_socket_readv net_socket_readv;
	struct __actvret_drvcall_devhost_net_socket_writev net_socket_writev;
	struct __actvret_drvcall_devhost_net_socket_getsockopt net_socket_getsockopt;
	struct __actvret_drvcall_devhost_net_socket_setsockopt net_socket_setsockopt;
	struct __actvret_drvcall_devhost_net_socket_getsockname net_socket_getsockname;
	struct __actvret_drvcall_devhost_net_socket_getpeername net_socket_getpeername;
	struct __actvret_drvcall_devhost_net_socket_fchown net_socket_fchown;
	struct __actvret_drvcall_devhost_net_socket_fstat net_socket_fstat;
	struct __actvret_drvcall_devhost_net_socket_socket net_socket_socket;
	struct __actvret_drvcall_devhost_net_socket_alloc_new_ns net_socket_alloc_new_ns;
	struct __actvret_drvcall_devhost_net_socket_destroy_ns net_socket_destroy_ns;
	struct __actvret_drvcall_devhost_net_socket_bpf net_socket_bpf;
	struct __actvret_drvcall_devhost_iaware_notify_thread_msg iaware_notify_thread_msg;
	struct __actvret_drvcall_devhost_kstate_binderinfo_notify kstate_binderinfo_notify;
	struct __actvret_drvcall_devhost_kstate_killinfo_notify kstate_killinfo_notify;
	struct __actvret_drvcall_devhost_kstate_freeze_binder_info_notify kstate_freeze_binder_info_notify;
	struct __actvret_drvcall_devhost_kstate_request_anco_thaw kstate_request_anco_thaw;
	struct __actvret_drvcall_devhost_pm_action pm_action;
	struct __actvret_drvcall_devhost_pm_dubai_hwlog pm_dubai_hwlog;
	struct __actvret_drvcall_devhost_pm_prepare pm_prepare;
	struct __actvret_drvcall_devhost_pm_power_off pm_power_off;
	struct __actvret_drvcall_devhost_pm_reboot pm_reboot;
	struct __actvret_drvcall_devhost_pm_cpuhp_action pm_cpuhp_action;
	struct __actvret_drvcall_devhost_pm_wl_stat pm_wl_stat;
	struct __actvret_drvcall_devhost_procfs_open procfs_open;
	struct __actvret_drvcall_devhost_procfs_close procfs_close;
	struct __actvret_drvcall_devhost_procfs_read procfs_read;
	struct __actvret_drvcall_devhost_procfs_write procfs_write;
	struct __actvret_drvcall_devhost_sysfs_open sysfs_open;
	struct __actvret_drvcall_devhost_sysfs_close sysfs_close;
	struct __actvret_drvcall_devhost_sysfs_read sysfs_read;
	struct __actvret_drvcall_devhost_sysfs_write sysfs_write;
	struct __actvret_drvcall_devhost_sysfs_poll sysfs_poll;
	struct __actvret_drvcall_devhost_clock_settime clock_settime;
	struct __actvret_drvcall_devhost_clock_gettime clock_gettime;
	struct __actvret_drvcall_devhost_clock_getres clock_getres;
	struct __actvret_drvcall_devhost_clock_adjtime clock_adjtime;
	struct __actvret_drvcall_devhost_clock_poweroff_alarm clock_poweroff_alarm;
	struct __actvret_drvcall_devhost_transfs_mounted transfs_mounted;
	struct __actvret_drvcall_devhost_transfs_unmounted transfs_unmounted;
	struct __actvret_drvcall_devhost_transfs_drop_caches transfs_drop_caches;
	struct __actvret_drvcall_devhost_transfs_mkdir transfs_mkdir;
	struct __actvret_drvcall_devhost_transfs_rmdir transfs_rmdir;
	struct __actvret_drvcall_devhost_transfs_create transfs_create;
	struct __actvret_drvcall_devhost_transfs_unlink transfs_unlink;
	struct __actvret_drvcall_devhost_transfs_mknod transfs_mknod;
	struct __actvret_drvcall_devhost_transfs_link transfs_link;
	struct __actvret_drvcall_devhost_transfs_symlink transfs_symlink;
	struct __actvret_drvcall_devhost_transfs_statfs transfs_statfs;
	struct __actvret_drvcall_devhost_transfs_rename transfs_rename;
	struct __actvret_drvcall_devhost_transfs_lookup transfs_lookup;
	struct __actvret_drvcall_devhost_transfs_setattr transfs_setattr;
	struct __actvret_drvcall_devhost_transfs_readlink transfs_readlink;
	struct __actvret_drvcall_devhost_transfs_getattr transfs_getattr;
	struct __actvret_drvcall_devhost_transfs_listxattr transfs_listxattr;
	struct __actvret_drvcall_devhost_transfs_getxattr transfs_getxattr;
	struct __actvret_drvcall_devhost_transfs_setxattr transfs_setxattr;
	struct __actvret_drvcall_devhost_transfs_tmpfile transfs_tmpfile;
	struct __actvret_drvcall_devhost_transfs_kill_dentry transfs_kill_dentry;
	struct __actvret_drvcall_devhost_transfs_node_put transfs_node_put;
	struct __actvret_drvcall_devhost_transfs_sync transfs_sync;
	struct __actvret_drvcall_devhost_transfs_write transfs_write;
	struct __actvret_drvcall_devhost_transfs_read transfs_read;
	struct __actvret_drvcall_devhost_transfs_write_iter transfs_write_iter;
	struct __actvret_drvcall_devhost_transfs_fsync transfs_fsync;
	struct __actvret_drvcall_devhost_transfs_readdir transfs_readdir;
	struct __actvret_drvcall_devhost_transfs_open transfs_open;
	struct __actvret_drvcall_devhost_transfs_fallocate transfs_fallocate;
	struct __actvret_drvcall_devhost_transfs_close transfs_close;
	struct __actvret_drvcall_devhost_transfs_ioctl transfs_ioctl;
	struct __actvret_drvcall_devhost_transfs_direct_IO transfs_direct_IO;
	struct __actvret_drvcall_devhost_transfs_update_opts transfs_update_opts;
	struct __actvret_drvcall_devhost_transfs_iommap transfs_iommap;
	struct __actvret_drvcall_devhost_shrinker_query shrinker_query;
	struct __actvret_drvcall_devhost_shrinker_shrink shrinker_shrink;
	struct __actvret_drvcall_devhost_process_revoke process_revoke;
	struct __actvret_drvcall_devhost_sec_mgmt sec_mgmt;
	struct __actvret_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb sec_test_hkip_seharmony_prot_policydb;
	struct __actvret_drvcall_devhost_tests_hkip_protection tests_hkip_protection;
	struct __actvret_drvcall_devhost_sec_test_hkip_roafter_prot sec_test_hkip_roafter_prot;
	struct __actvret_drvcall_devhost_sec_sync_fastpath_open sec_sync_fastpath_open;
	struct __actvret_drvcall_devhost_sec_sync_fastpath_dev sec_sync_fastpath_dev;
	struct __actvret_drvcall_devhost_devfs_read devfs_read;
	struct __actvret_drvcall_devhost_devfs_write devfs_write;
	struct __actvret_drvcall_devhost_devfs_ioctl devfs_ioctl;
	struct __actvret_drvcall_devhost_devfs_readv devfs_readv;
	struct __actvret_drvcall_devhost_devfs_writev devfs_writev;
	struct __actvret_drvcall_devhost_devfs_lseek devfs_lseek;
	struct __actvret_drvcall_devhost_devfs_fcntl devfs_fcntl;
	struct __actvret_drvcall_devhost_trace_enable_disable_ldk_event trace_enable_disable_ldk_event;
	struct __actvret_drvcall_devhost_trace_ldk_event_format trace_ldk_event_format;
	struct __actvret_drvcall_devhost_trace_print_ldk_event trace_print_ldk_event;
	struct __actvret_drvcall_devhost_file_info_query_dmabuf file_info_query_dmabuf;
	struct __actvret_drvcall_devhost_file_info_query_fence file_info_query_fence;
	struct __actvret_drvcall_devhost_file_info_query_ion file_info_query_ion;
	struct __actvret_drvcall_devhost_file_info_query_meminfo file_info_query_meminfo;
	struct __actvret_drvcall_devhost_file_info_lowmem_debug file_info_lowmem_debug;
	struct __actvret_drvcall_devhost_file_info_query_gpu file_info_query_gpu;
	struct __actvret_drvcall_devhost_probe_handle_brk probe_handle_brk;
	struct __actvret_drvcall_devhost_probe_handle_ss_brk probe_handle_ss_brk;
	struct __actvret_drvcall_devhost_fileguard_message_notify fileguard_message_notify;
	struct __actvret_drvcall_devhost_posix_vhangup posix_vhangup;
	struct __actvret_drvcall_devhost_devfs__llseek devfs__llseek;
	struct __actvret_drvcall_devhost_devfs_fcntl64 devfs_fcntl64;
};

#else
union __actvret_drvcall_devhost {
	struct __actvret_drvcall_devhost_core_load_driver core_load_driver;
	struct __actvret_drvcall_devhost_core_create_device core_create_device;
	struct __actvret_drvcall_devhost_core_unload_driver core_unload_driver;
	struct __actvret_drvcall_devhost_core_bind_device core_bind_device;
	struct __actvret_drvcall_devhost_core_entropy_read core_entropy_read;
	struct __actvret_drvcall_devhost_rpc_export rpc_export;
	struct __actvret_drvcall_devhost_rpc_invoke rpc_invoke;
	struct __actvret_drvcall_devhost_fops_open fops_open;
	struct __actvret_drvcall_devhost_fops_close fops_close;
	struct __actvret_drvcall_devhost_fops_read fops_read;
	struct __actvret_drvcall_devhost_fops_write fops_write;
	struct __actvret_drvcall_devhost_fops_read_iter fops_read_iter;
	struct __actvret_drvcall_devhost_fops_write_iter fops_write_iter;
	struct __actvret_drvcall_devhost_fops_ioctl fops_ioctl;
	struct __actvret_drvcall_devhost_fops_flush fops_flush;
	struct __actvret_drvcall_devhost_fops_fcntl fops_fcntl;
	struct __actvret_drvcall_devhost_fops_llseek fops_llseek;
	struct __actvret_drvcall_devhost_fops_mmap fops_mmap;
	struct __actvret_drvcall_devhost_fops_munmap fops_munmap;
	struct __actvret_drvcall_devhost_fops_do_page_fault fops_do_page_fault;
	struct __actvret_drvcall_devhost_fops_mm_release fops_mm_release;
	struct __actvret_drvcall_devhost_fops_sysfs_read fops_sysfs_read;
	struct __actvret_drvcall_devhost_fops_sysfs_write fops_sysfs_write;
	struct __actvret_drvcall_devhost_fops_poll fops_poll;
	struct __actvret_drvcall_devhost_fops_poll_remove fops_poll_remove;
	struct __actvret_drvcall_devhost_fops_epoll_ctl fops_epoll_ctl;
	struct __actvret_drvcall_devhost_fops_epoll_remove fops_epoll_remove;
	struct __actvret_drvcall_devhost_fops_epoll_recheck fops_epoll_recheck;
	struct __actvret_drvcall_devhost_fops_sync_ctrlmem_polllist fops_sync_ctrlmem_polllist;
	struct __actvret_drvcall_devhost_fops_file_get fops_file_get;
	struct __actvret_drvcall_devhost_block_check_feature block_check_feature;
	struct __actvret_drvcall_devhost_block_submit_bio block_submit_bio;
	struct __actvret_drvcall_devhost_block_submit_cmd block_submit_cmd;
	struct __actvret_drvcall_devhost_block_run_queue block_run_queue;
	struct __actvret_drvcall_devhost_block_direct_access block_direct_access;
	struct __actvret_drvcall_devhost_block_register_bio_notify block_register_bio_notify;
	struct __actvret_drvcall_devhost_net_send net_send;
	struct __actvret_drvcall_devhost_net_ioctl net_ioctl;
	struct __actvret_drvcall_devhost_net_status net_status;
	struct __actvret_drvcall_devhost_net_prepare net_prepare;
	struct __actvret_drvcall_devhost_net_init net_init;
	struct __actvret_drvcall_devhost_net_netlink_recv net_netlink_recv;
	struct __actvret_drvcall_devhost_net_socket_register net_socket_register;
	struct __actvret_drvcall_devhost_net_socket_close net_socket_close;
	struct __actvret_drvcall_devhost_net_socket_connect net_socket_connect;
	struct __actvret_drvcall_devhost_net_socket_bind net_socket_bind;
	struct __actvret_drvcall_devhost_net_socket_listen net_socket_listen;
	struct __actvret_drvcall_devhost_net_socket_accept net_socket_accept;
	struct __actvret_drvcall_devhost_net_socket_shutdown net_socket_shutdown;
	struct __actvret_drvcall_devhost_net_socket_ioctl net_socket_ioctl;
	struct __actvret_drvcall_devhost_net_socket_dev_ioctl net_socket_dev_ioctl;
	struct __actvret_drvcall_devhost_net_socket_fcntl net_socket_fcntl;
	struct __actvret_drvcall_devhost_net_socket_send net_socket_send;
	struct __actvret_drvcall_devhost_net_socket_recv net_socket_recv;
	struct __actvret_drvcall_devhost_net_socket_sendmmsg net_socket_sendmmsg;
	struct __actvret_drvcall_devhost_net_socket_recvmmsg net_socket_recvmmsg;
	struct __actvret_drvcall_devhost_net_socket_sendmsg net_socket_sendmsg;
	struct __actvret_drvcall_devhost_net_socket_recvmsg net_socket_recvmsg;
	struct __actvret_drvcall_devhost_net_socket_sendto net_socket_sendto;
	struct __actvret_drvcall_devhost_net_socket_recvfrom net_socket_recvfrom;
	struct __actvret_drvcall_devhost_net_socket_readv net_socket_readv;
	struct __actvret_drvcall_devhost_net_socket_writev net_socket_writev;
	struct __actvret_drvcall_devhost_net_socket_getsockopt net_socket_getsockopt;
	struct __actvret_drvcall_devhost_net_socket_setsockopt net_socket_setsockopt;
	struct __actvret_drvcall_devhost_net_socket_getsockname net_socket_getsockname;
	struct __actvret_drvcall_devhost_net_socket_getpeername net_socket_getpeername;
	struct __actvret_drvcall_devhost_net_socket_fchown net_socket_fchown;
	struct __actvret_drvcall_devhost_net_socket_fstat net_socket_fstat;
	struct __actvret_drvcall_devhost_net_socket_socket net_socket_socket;
	struct __actvret_drvcall_devhost_net_socket_alloc_new_ns net_socket_alloc_new_ns;
	struct __actvret_drvcall_devhost_net_socket_destroy_ns net_socket_destroy_ns;
	struct __actvret_drvcall_devhost_net_socket_bpf net_socket_bpf;
	struct __actvret_drvcall_devhost_iaware_notify_thread_msg iaware_notify_thread_msg;
	struct __actvret_drvcall_devhost_kstate_binderinfo_notify kstate_binderinfo_notify;
	struct __actvret_drvcall_devhost_kstate_killinfo_notify kstate_killinfo_notify;
	struct __actvret_drvcall_devhost_kstate_freeze_binder_info_notify kstate_freeze_binder_info_notify;
	struct __actvret_drvcall_devhost_kstate_request_anco_thaw kstate_request_anco_thaw;
	struct __actvret_drvcall_devhost_pm_action pm_action;
	struct __actvret_drvcall_devhost_pm_dubai_hwlog pm_dubai_hwlog;
	struct __actvret_drvcall_devhost_pm_prepare pm_prepare;
	struct __actvret_drvcall_devhost_pm_power_off pm_power_off;
	struct __actvret_drvcall_devhost_pm_reboot pm_reboot;
	struct __actvret_drvcall_devhost_pm_cpuhp_action pm_cpuhp_action;
	struct __actvret_drvcall_devhost_pm_wl_stat pm_wl_stat;
	struct __actvret_drvcall_devhost_procfs_open procfs_open;
	struct __actvret_drvcall_devhost_procfs_close procfs_close;
	struct __actvret_drvcall_devhost_procfs_read procfs_read;
	struct __actvret_drvcall_devhost_procfs_write procfs_write;
	struct __actvret_drvcall_devhost_sysfs_open sysfs_open;
	struct __actvret_drvcall_devhost_sysfs_close sysfs_close;
	struct __actvret_drvcall_devhost_sysfs_read sysfs_read;
	struct __actvret_drvcall_devhost_sysfs_write sysfs_write;
	struct __actvret_drvcall_devhost_sysfs_poll sysfs_poll;
	struct __actvret_drvcall_devhost_clock_settime clock_settime;
	struct __actvret_drvcall_devhost_clock_gettime clock_gettime;
	struct __actvret_drvcall_devhost_clock_getres clock_getres;
	struct __actvret_drvcall_devhost_clock_adjtime clock_adjtime;
	struct __actvret_drvcall_devhost_clock_poweroff_alarm clock_poweroff_alarm;
	struct __actvret_drvcall_devhost_transfs_mounted transfs_mounted;
	struct __actvret_drvcall_devhost_transfs_unmounted transfs_unmounted;
	struct __actvret_drvcall_devhost_transfs_drop_caches transfs_drop_caches;
	struct __actvret_drvcall_devhost_transfs_mkdir transfs_mkdir;
	struct __actvret_drvcall_devhost_transfs_rmdir transfs_rmdir;
	struct __actvret_drvcall_devhost_transfs_create transfs_create;
	struct __actvret_drvcall_devhost_transfs_unlink transfs_unlink;
	struct __actvret_drvcall_devhost_transfs_mknod transfs_mknod;
	struct __actvret_drvcall_devhost_transfs_link transfs_link;
	struct __actvret_drvcall_devhost_transfs_symlink transfs_symlink;
	struct __actvret_drvcall_devhost_transfs_statfs transfs_statfs;
	struct __actvret_drvcall_devhost_transfs_rename transfs_rename;
	struct __actvret_drvcall_devhost_transfs_lookup transfs_lookup;
	struct __actvret_drvcall_devhost_transfs_setattr transfs_setattr;
	struct __actvret_drvcall_devhost_transfs_readlink transfs_readlink;
	struct __actvret_drvcall_devhost_transfs_getattr transfs_getattr;
	struct __actvret_drvcall_devhost_transfs_listxattr transfs_listxattr;
	struct __actvret_drvcall_devhost_transfs_getxattr transfs_getxattr;
	struct __actvret_drvcall_devhost_transfs_setxattr transfs_setxattr;
	struct __actvret_drvcall_devhost_transfs_tmpfile transfs_tmpfile;
	struct __actvret_drvcall_devhost_transfs_kill_dentry transfs_kill_dentry;
	struct __actvret_drvcall_devhost_transfs_node_put transfs_node_put;
	struct __actvret_drvcall_devhost_transfs_sync transfs_sync;
	struct __actvret_drvcall_devhost_transfs_write transfs_write;
	struct __actvret_drvcall_devhost_transfs_read transfs_read;
	struct __actvret_drvcall_devhost_transfs_write_iter transfs_write_iter;
	struct __actvret_drvcall_devhost_transfs_fsync transfs_fsync;
	struct __actvret_drvcall_devhost_transfs_readdir transfs_readdir;
	struct __actvret_drvcall_devhost_transfs_open transfs_open;
	struct __actvret_drvcall_devhost_transfs_fallocate transfs_fallocate;
	struct __actvret_drvcall_devhost_transfs_close transfs_close;
	struct __actvret_drvcall_devhost_transfs_ioctl transfs_ioctl;
	struct __actvret_drvcall_devhost_transfs_direct_IO transfs_direct_IO;
	struct __actvret_drvcall_devhost_transfs_update_opts transfs_update_opts;
	struct __actvret_drvcall_devhost_transfs_iommap transfs_iommap;
	struct __actvret_drvcall_devhost_shrinker_query shrinker_query;
	struct __actvret_drvcall_devhost_shrinker_shrink shrinker_shrink;
	struct __actvret_drvcall_devhost_process_revoke process_revoke;
	struct __actvret_drvcall_devhost_sec_mgmt sec_mgmt;
	struct __actvret_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb sec_test_hkip_seharmony_prot_policydb;
	struct __actvret_drvcall_devhost_tests_hkip_protection tests_hkip_protection;
	struct __actvret_drvcall_devhost_sec_test_hkip_roafter_prot sec_test_hkip_roafter_prot;
	struct __actvret_drvcall_devhost_sec_sync_fastpath_open sec_sync_fastpath_open;
	struct __actvret_drvcall_devhost_sec_sync_fastpath_dev sec_sync_fastpath_dev;
	struct __actvret_drvcall_devhost_devfs_read devfs_read;
	struct __actvret_drvcall_devhost_devfs_write devfs_write;
	struct __actvret_drvcall_devhost_devfs_ioctl devfs_ioctl;
	struct __actvret_drvcall_devhost_devfs_readv devfs_readv;
	struct __actvret_drvcall_devhost_devfs_writev devfs_writev;
	struct __actvret_drvcall_devhost_devfs_lseek devfs_lseek;
	struct __actvret_drvcall_devhost_devfs_fcntl devfs_fcntl;
	struct __actvret_drvcall_devhost_trace_enable_disable_ldk_event trace_enable_disable_ldk_event;
	struct __actvret_drvcall_devhost_trace_ldk_event_format trace_ldk_event_format;
	struct __actvret_drvcall_devhost_trace_print_ldk_event trace_print_ldk_event;
	struct __actvret_drvcall_devhost_file_info_query_dmabuf file_info_query_dmabuf;
	struct __actvret_drvcall_devhost_file_info_query_fence file_info_query_fence;
	struct __actvret_drvcall_devhost_file_info_query_ion file_info_query_ion;
	struct __actvret_drvcall_devhost_file_info_query_meminfo file_info_query_meminfo;
	struct __actvret_drvcall_devhost_file_info_lowmem_debug file_info_lowmem_debug;
	struct __actvret_drvcall_devhost_file_info_query_gpu file_info_query_gpu;
	struct __actvret_drvcall_devhost_probe_handle_brk probe_handle_brk;
	struct __actvret_drvcall_devhost_probe_handle_ss_brk probe_handle_ss_brk;
	struct __actvret_drvcall_devhost_fileguard_message_notify fileguard_message_notify;
	struct __actvret_drvcall_devhost_posix_vhangup posix_vhangup;
};

#endif
enum __devhost_mgr {
	__devhost_INVAL_mgr=0,
	__devhost_core_mgr,
	__devhost_rpc_mgr,
	__devhost_fops_mgr,
	__devhost_block_mgr,
	__devhost_net_mgr,
	__devhost_net_socket_mgr,
	__devhost_iaware_notify_mgr,
	__devhost_kstate_mgr,
	__devhost_pm_mgr,
	__devhost_procfs_mgr,
	__devhost_sysfs_mgr,
	__devhost_clock_mgr,
	__devhost_transfs_mgr,
	__devhost_shrinker_mgr,
	__devhost_process_mgr,
	__devhost_sec_mgr,
	__devhost_devfs_mgr,
	__devhost_tracefs_mgr,
	__devhost_file_info_mgr,
	__devhost_probe_mgr,
	__devhost_fileguard_mgr,
	__devhost_posix_mgr,
	__devhost_MAX_mgr,
};

#define __devhost_method_MIN __ACTV_HIGHTABLE_START
#define __devhost_method_devfs_read (__ACTV_HIGHTABLE_START + 1)
#define __devhost_method_devfs_write (__ACTV_HIGHTABLE_START + 2)
#define __devhost_method_devfs_ioctl (__ACTV_HIGHTABLE_START + 3)
#define __devhost_method_devfs_readv (__ACTV_HIGHTABLE_START + 4)
#define __devhost_method_devfs_writev (__ACTV_HIGHTABLE_START + 5)
#define __devhost_method_devfs_lseek (__ACTV_HIGHTABLE_START + 6)
#define __devhost_method_devfs_fcntl (__ACTV_HIGHTABLE_START + 7)
#define __devhost_method_devfs__llseek (__ACTV_HIGHTABLE_START + 8)
#define __devhost_method_devfs_fcntl64 (__ACTV_HIGHTABLE_START + 9)
#define __devhost_method_posix_vhangup (__ACTV_HIGHTABLE_START + 10)
#define __devhost_method_core_load_driver (__ACTV_HIGHTABLE_START + 11)
#define __devhost_method_core_create_device (__ACTV_HIGHTABLE_START + 12)
#define __devhost_method_core_unload_driver (__ACTV_HIGHTABLE_START + 13)
#define __devhost_method_core_bind_device (__ACTV_HIGHTABLE_START + 14)
#define __devhost_method_core_entropy_read (__ACTV_HIGHTABLE_START + 15)
#define __devhost_method_core_rsvd_3 (__ACTV_HIGHTABLE_START + 16)
#define __devhost_method_core_rsvd_2 (__ACTV_HIGHTABLE_START + 17)
#define __devhost_method_core_rsvd_1 (__ACTV_HIGHTABLE_START + 18)
#define __devhost_method_rpc_export (__ACTV_HIGHTABLE_START + 19)
#define __devhost_method_rpc_invoke (__ACTV_HIGHTABLE_START + 20)
#define __devhost_method_rpc_rsvd_2 (__ACTV_HIGHTABLE_START + 21)
#define __devhost_method_rpc_rsvd_1 (__ACTV_HIGHTABLE_START + 22)
#define __devhost_method_fops_open (__ACTV_HIGHTABLE_START + 23)
#define __devhost_method_fops_close (__ACTV_HIGHTABLE_START + 24)
#define __devhost_method_fops_read (__ACTV_HIGHTABLE_START + 25)
#define __devhost_method_fops_write (__ACTV_HIGHTABLE_START + 26)
#define __devhost_method_fops_read_iter (__ACTV_HIGHTABLE_START + 27)
#define __devhost_method_fops_write_iter (__ACTV_HIGHTABLE_START + 28)
#define __devhost_method_fops_ioctl (__ACTV_HIGHTABLE_START + 29)
#define __devhost_method_fops_flush (__ACTV_HIGHTABLE_START + 30)
#define __devhost_method_fops_fcntl (__ACTV_HIGHTABLE_START + 31)
#define __devhost_method_fops_llseek (__ACTV_HIGHTABLE_START + 32)
#define __devhost_method_fops_mmap (__ACTV_HIGHTABLE_START + 33)
#define __devhost_method_fops_munmap (__ACTV_HIGHTABLE_START + 34)
#define __devhost_method_fops_do_page_fault (__ACTV_HIGHTABLE_START + 35)
#define __devhost_method_fops_mm_release (__ACTV_HIGHTABLE_START + 36)
#define __devhost_method_fops_sysfs_read (__ACTV_HIGHTABLE_START + 37)
#define __devhost_method_fops_sysfs_write (__ACTV_HIGHTABLE_START + 38)
#define __devhost_method_fops_poll (__ACTV_HIGHTABLE_START + 39)
#define __devhost_method_fops_poll_remove (__ACTV_HIGHTABLE_START + 40)
#define __devhost_method_fops_epoll_ctl (__ACTV_HIGHTABLE_START + 41)
#define __devhost_method_fops_epoll_remove (__ACTV_HIGHTABLE_START + 42)
#define __devhost_method_fops_epoll_recheck (__ACTV_HIGHTABLE_START + 43)
#define __devhost_method_fops_sync_ctrlmem_polllist (__ACTV_HIGHTABLE_START + 44)
#define __devhost_method_fops_file_get (__ACTV_HIGHTABLE_START + 45)
#define __devhost_method_block_check_feature (__ACTV_HIGHTABLE_START + 46)
#define __devhost_method_block_submit_bio (__ACTV_HIGHTABLE_START + 47)
#define __devhost_method_block_submit_cmd (__ACTV_HIGHTABLE_START + 48)
#define __devhost_method_block_run_queue (__ACTV_HIGHTABLE_START + 49)
#define __devhost_method_block_direct_access (__ACTV_HIGHTABLE_START + 50)
#define __devhost_method_block_register_bio_notify (__ACTV_HIGHTABLE_START + 51)
#define __devhost_method_block_rsvd_3 (__ACTV_HIGHTABLE_START + 52)
#define __devhost_method_block_rsvd_2 (__ACTV_HIGHTABLE_START + 53)
#define __devhost_method_block_rsvd_1 (__ACTV_HIGHTABLE_START + 54)
#define __devhost_method_net_send (__ACTV_HIGHTABLE_START + 55)
#define __devhost_method_net_ioctl (__ACTV_HIGHTABLE_START + 56)
#define __devhost_method_net_status (__ACTV_HIGHTABLE_START + 57)
#define __devhost_method_net_prepare (__ACTV_HIGHTABLE_START + 58)
#define __devhost_method_net_init (__ACTV_HIGHTABLE_START + 59)
#define __devhost_method_net_netlink_recv (__ACTV_HIGHTABLE_START + 60)
#define __devhost_method_net_rsvd_2 (__ACTV_HIGHTABLE_START + 61)
#define __devhost_method_net_rsvd_1 (__ACTV_HIGHTABLE_START + 62)
#define __devhost_method_net_socket_register (__ACTV_HIGHTABLE_START + 63)
#define __devhost_method_net_socket_close (__ACTV_HIGHTABLE_START + 64)
#define __devhost_method_net_socket_connect (__ACTV_HIGHTABLE_START + 65)
#define __devhost_method_net_socket_bind (__ACTV_HIGHTABLE_START + 66)
#define __devhost_method_net_socket_listen (__ACTV_HIGHTABLE_START + 67)
#define __devhost_method_net_socket_accept (__ACTV_HIGHTABLE_START + 68)
#define __devhost_method_net_socket_shutdown (__ACTV_HIGHTABLE_START + 69)
#define __devhost_method_net_socket_ioctl (__ACTV_HIGHTABLE_START + 70)
#define __devhost_method_net_socket_dev_ioctl (__ACTV_HIGHTABLE_START + 71)
#define __devhost_method_net_socket_fcntl (__ACTV_HIGHTABLE_START + 72)
#define __devhost_method_net_socket_send (__ACTV_HIGHTABLE_START + 73)
#define __devhost_method_net_socket_recv (__ACTV_HIGHTABLE_START + 74)
#define __devhost_method_net_socket_sendmmsg (__ACTV_HIGHTABLE_START + 75)
#define __devhost_method_net_socket_recvmmsg (__ACTV_HIGHTABLE_START + 76)
#define __devhost_method_net_socket_sendmsg (__ACTV_HIGHTABLE_START + 77)
#define __devhost_method_net_socket_recvmsg (__ACTV_HIGHTABLE_START + 78)
#define __devhost_method_net_socket_sendto (__ACTV_HIGHTABLE_START + 79)
#define __devhost_method_net_socket_recvfrom (__ACTV_HIGHTABLE_START + 80)
#define __devhost_method_net_socket_readv (__ACTV_HIGHTABLE_START + 81)
#define __devhost_method_net_socket_writev (__ACTV_HIGHTABLE_START + 82)
#define __devhost_method_net_socket_getsockopt (__ACTV_HIGHTABLE_START + 83)
#define __devhost_method_net_socket_setsockopt (__ACTV_HIGHTABLE_START + 84)
#define __devhost_method_net_socket_getsockname (__ACTV_HIGHTABLE_START + 85)
#define __devhost_method_net_socket_getpeername (__ACTV_HIGHTABLE_START + 86)
#define __devhost_method_net_socket_fchown (__ACTV_HIGHTABLE_START + 87)
#define __devhost_method_net_socket_fstat (__ACTV_HIGHTABLE_START + 88)
#define __devhost_method_net_socket_socket (__ACTV_HIGHTABLE_START + 89)
#define __devhost_method_net_socket_alloc_new_ns (__ACTV_HIGHTABLE_START + 90)
#define __devhost_method_net_socket_destroy_ns (__ACTV_HIGHTABLE_START + 91)
#define __devhost_method_net_socket_bpf (__ACTV_HIGHTABLE_START + 92)
#define __devhost_method_net_socket_rsvd_2 (__ACTV_HIGHTABLE_START + 93)
#define __devhost_method_net_socket_rsvd_1 (__ACTV_HIGHTABLE_START + 94)
#define __devhost_method_iaware_notify_thread_msg (__ACTV_HIGHTABLE_START + 95)
#define __devhost_method_iaware_notify_rsvd_4 (__ACTV_HIGHTABLE_START + 96)
#define __devhost_method_iaware_notify_rsvd_3 (__ACTV_HIGHTABLE_START + 97)
#define __devhost_method_iaware_notify_rsvd_2 (__ACTV_HIGHTABLE_START + 98)
#define __devhost_method_iaware_notify_rsvd_1 (__ACTV_HIGHTABLE_START + 99)
#define __devhost_method_kstate_binderinfo_notify (__ACTV_HIGHTABLE_START + 100)
#define __devhost_method_kstate_killinfo_notify (__ACTV_HIGHTABLE_START + 101)
#define __devhost_method_kstate_freeze_binder_info_notify (__ACTV_HIGHTABLE_START + 102)
#define __devhost_method_kstate_request_anco_thaw (__ACTV_HIGHTABLE_START + 103)
#define __devhost_method_kstate_rsvd_1 (__ACTV_HIGHTABLE_START + 104)
#define __devhost_method_pm_action (__ACTV_HIGHTABLE_START + 105)
#define __devhost_method_pm_dubai_hwlog (__ACTV_HIGHTABLE_START + 106)
#define __devhost_method_pm_prepare (__ACTV_HIGHTABLE_START + 107)
#define __devhost_method_pm_power_off (__ACTV_HIGHTABLE_START + 108)
#define __devhost_method_pm_reboot (__ACTV_HIGHTABLE_START + 109)
#define __devhost_method_pm_cpuhp_action (__ACTV_HIGHTABLE_START + 110)
#define __devhost_method_pm_wl_stat (__ACTV_HIGHTABLE_START + 111)
#define __devhost_method_pm_rsvd_2 (__ACTV_HIGHTABLE_START + 112)
#define __devhost_method_pm_rsvd_1 (__ACTV_HIGHTABLE_START + 113)
#define __devhost_method_procfs_open (__ACTV_HIGHTABLE_START + 114)
#define __devhost_method_procfs_close (__ACTV_HIGHTABLE_START + 115)
#define __devhost_method_procfs_read (__ACTV_HIGHTABLE_START + 116)
#define __devhost_method_procfs_write (__ACTV_HIGHTABLE_START + 117)
#define __devhost_method_sysfs_open (__ACTV_HIGHTABLE_START + 118)
#define __devhost_method_sysfs_close (__ACTV_HIGHTABLE_START + 119)
#define __devhost_method_sysfs_read (__ACTV_HIGHTABLE_START + 120)
#define __devhost_method_sysfs_write (__ACTV_HIGHTABLE_START + 121)
#define __devhost_method_sysfs_poll (__ACTV_HIGHTABLE_START + 122)
#define __devhost_method_sysfs_rsvd_3 (__ACTV_HIGHTABLE_START + 123)
#define __devhost_method_sysfs_rsvd_2 (__ACTV_HIGHTABLE_START + 124)
#define __devhost_method_sysfs_rsvd_1 (__ACTV_HIGHTABLE_START + 125)
#define __devhost_method_clock_settime (__ACTV_HIGHTABLE_START + 126)
#define __devhost_method_clock_gettime (__ACTV_HIGHTABLE_START + 127)
#define __devhost_method_clock_getres (__ACTV_HIGHTABLE_START + 128)
#define __devhost_method_clock_adjtime (__ACTV_HIGHTABLE_START + 129)
#define __devhost_method_clock_poweroff_alarm (__ACTV_HIGHTABLE_START + 130)
#define __devhost_method_clock_rsvd_3 (__ACTV_HIGHTABLE_START + 131)
#define __devhost_method_clock_rsvd_2 (__ACTV_HIGHTABLE_START + 132)
#define __devhost_method_clock_rsvd_1 (__ACTV_HIGHTABLE_START + 133)
#define __devhost_method_transfs_mounted (__ACTV_HIGHTABLE_START + 134)
#define __devhost_method_transfs_unmounted (__ACTV_HIGHTABLE_START + 135)
#define __devhost_method_transfs_drop_caches (__ACTV_HIGHTABLE_START + 136)
#define __devhost_method_transfs_mkdir (__ACTV_HIGHTABLE_START + 137)
#define __devhost_method_transfs_rmdir (__ACTV_HIGHTABLE_START + 138)
#define __devhost_method_transfs_create (__ACTV_HIGHTABLE_START + 139)
#define __devhost_method_transfs_unlink (__ACTV_HIGHTABLE_START + 140)
#define __devhost_method_transfs_mknod (__ACTV_HIGHTABLE_START + 141)
#define __devhost_method_transfs_link (__ACTV_HIGHTABLE_START + 142)
#define __devhost_method_transfs_symlink (__ACTV_HIGHTABLE_START + 143)
#define __devhost_method_transfs_statfs (__ACTV_HIGHTABLE_START + 144)
#define __devhost_method_transfs_rename (__ACTV_HIGHTABLE_START + 145)
#define __devhost_method_transfs_lookup (__ACTV_HIGHTABLE_START + 146)
#define __devhost_method_transfs_setattr (__ACTV_HIGHTABLE_START + 147)
#define __devhost_method_transfs_readlink (__ACTV_HIGHTABLE_START + 148)
#define __devhost_method_transfs_getattr (__ACTV_HIGHTABLE_START + 149)
#define __devhost_method_transfs_listxattr (__ACTV_HIGHTABLE_START + 150)
#define __devhost_method_transfs_getxattr (__ACTV_HIGHTABLE_START + 151)
#define __devhost_method_transfs_setxattr (__ACTV_HIGHTABLE_START + 152)
#define __devhost_method_transfs_tmpfile (__ACTV_HIGHTABLE_START + 153)
#define __devhost_method_transfs_kill_dentry (__ACTV_HIGHTABLE_START + 154)
#define __devhost_method_transfs_node_put (__ACTV_HIGHTABLE_START + 155)
#define __devhost_method_transfs_sync (__ACTV_HIGHTABLE_START + 156)
#define __devhost_method_transfs_write (__ACTV_HIGHTABLE_START + 157)
#define __devhost_method_transfs_read (__ACTV_HIGHTABLE_START + 158)
#define __devhost_method_transfs_write_iter (__ACTV_HIGHTABLE_START + 159)
#define __devhost_method_transfs_fsync (__ACTV_HIGHTABLE_START + 160)
#define __devhost_method_transfs_readdir (__ACTV_HIGHTABLE_START + 161)
#define __devhost_method_transfs_open (__ACTV_HIGHTABLE_START + 162)
#define __devhost_method_transfs_fallocate (__ACTV_HIGHTABLE_START + 163)
#define __devhost_method_transfs_close (__ACTV_HIGHTABLE_START + 164)
#define __devhost_method_transfs_ioctl (__ACTV_HIGHTABLE_START + 165)
#define __devhost_method_transfs_direct_IO (__ACTV_HIGHTABLE_START + 166)
#define __devhost_method_transfs_update_opts (__ACTV_HIGHTABLE_START + 167)
#define __devhost_method_transfs_iommap (__ACTV_HIGHTABLE_START + 168)
#define __devhost_method_shrinker_query (__ACTV_HIGHTABLE_START + 169)
#define __devhost_method_shrinker_shrink (__ACTV_HIGHTABLE_START + 170)
#define __devhost_method_shrinker_rsvd_4 (__ACTV_HIGHTABLE_START + 171)
#define __devhost_method_shrinker_rsvd_3 (__ACTV_HIGHTABLE_START + 172)
#define __devhost_method_shrinker_rsvd_2 (__ACTV_HIGHTABLE_START + 173)
#define __devhost_method_shrinker_rsvd_1 (__ACTV_HIGHTABLE_START + 174)
#define __devhost_method_process_revoke (__ACTV_HIGHTABLE_START + 175)
#define __devhost_method_sec_mgmt (__ACTV_HIGHTABLE_START + 176)
#define __devhost_method_sec_test_hkip_seharmony_prot_policydb (__ACTV_HIGHTABLE_START + 177)
#define __devhost_method_tests_hkip_protection (__ACTV_HIGHTABLE_START + 178)
#define __devhost_method_sec_test_hkip_roafter_prot (__ACTV_HIGHTABLE_START + 179)
#define __devhost_method_sec_sync_fastpath_open (__ACTV_HIGHTABLE_START + 180)
#define __devhost_method_sec_sync_fastpath_dev (__ACTV_HIGHTABLE_START + 181)
#define __devhost_method_sec_rsvd_26 (__ACTV_HIGHTABLE_START + 182)
#define __devhost_method_sec_rsvd_25 (__ACTV_HIGHTABLE_START + 183)
#define __devhost_method_sec_rsvd_24 (__ACTV_HIGHTABLE_START + 184)
#define __devhost_method_sec_rsvd_23 (__ACTV_HIGHTABLE_START + 185)
#define __devhost_method_sec_rsvd_22 (__ACTV_HIGHTABLE_START + 186)
#define __devhost_method_sec_rsvd_21 (__ACTV_HIGHTABLE_START + 187)
#define __devhost_method_sec_rsvd_20 (__ACTV_HIGHTABLE_START + 188)
#define __devhost_method_sec_rsvd_19 (__ACTV_HIGHTABLE_START + 189)
#define __devhost_method_sec_rsvd_18 (__ACTV_HIGHTABLE_START + 190)
#define __devhost_method_sec_rsvd_17 (__ACTV_HIGHTABLE_START + 191)
#define __devhost_method_sec_rsvd_16 (__ACTV_HIGHTABLE_START + 192)
#define __devhost_method_sec_rsvd_15 (__ACTV_HIGHTABLE_START + 193)
#define __devhost_method_sec_rsvd_14 (__ACTV_HIGHTABLE_START + 194)
#define __devhost_method_sec_rsvd_13 (__ACTV_HIGHTABLE_START + 195)
#define __devhost_method_sec_rsvd_12 (__ACTV_HIGHTABLE_START + 196)
#define __devhost_method_sec_rsvd_11 (__ACTV_HIGHTABLE_START + 197)
#define __devhost_method_sec_rsvd_10 (__ACTV_HIGHTABLE_START + 198)
#define __devhost_method_sec_rsvd_9 (__ACTV_HIGHTABLE_START + 199)
#define __devhost_method_sec_rsvd_8 (__ACTV_HIGHTABLE_START + 200)
#define __devhost_method_sec_rsvd_7 (__ACTV_HIGHTABLE_START + 201)
#define __devhost_method_sec_rsvd_6 (__ACTV_HIGHTABLE_START + 202)
#define __devhost_method_sec_rsvd_5 (__ACTV_HIGHTABLE_START + 203)
#define __devhost_method_sec_rsvd_4 (__ACTV_HIGHTABLE_START + 204)
#define __devhost_method_sec_rsvd_3 (__ACTV_HIGHTABLE_START + 205)
#define __devhost_method_sec_rsvd_2 (__ACTV_HIGHTABLE_START + 206)
#define __devhost_method_sec_rsvd_1 (__ACTV_HIGHTABLE_START + 207)
#define __devhost_method_devfs_rsvd_2 (__ACTV_HIGHTABLE_START + 208)
#define __devhost_method_devfs_rsvd_1 (__ACTV_HIGHTABLE_START + 209)
#define __devhost_method_trace_enable_disable_ldk_event (__ACTV_HIGHTABLE_START + 210)
#define __devhost_method_trace_ldk_event_format (__ACTV_HIGHTABLE_START + 211)
#define __devhost_method_trace_print_ldk_event (__ACTV_HIGHTABLE_START + 212)
#define __devhost_method_tracefs_rsvd_2 (__ACTV_HIGHTABLE_START + 213)
#define __devhost_method_tracefs_rsvd_1 (__ACTV_HIGHTABLE_START + 214)
#define __devhost_method_file_info_query_dmabuf (__ACTV_HIGHTABLE_START + 215)
#define __devhost_method_file_info_query_fence (__ACTV_HIGHTABLE_START + 216)
#define __devhost_method_file_info_query_ion (__ACTV_HIGHTABLE_START + 217)
#define __devhost_method_file_info_query_meminfo (__ACTV_HIGHTABLE_START + 218)
#define __devhost_method_file_info_lowmem_debug (__ACTV_HIGHTABLE_START + 219)
#define __devhost_method_file_info_query_gpu (__ACTV_HIGHTABLE_START + 220)
#define __devhost_method_probe_handle_brk (__ACTV_HIGHTABLE_START + 221)
#define __devhost_method_probe_handle_ss_brk (__ACTV_HIGHTABLE_START + 222)
#define __devhost_method_fileguard_message_notify (__ACTV_HIGHTABLE_START + 223)
#define __devhost_method_fileguard_rsvd_4 (__ACTV_HIGHTABLE_START + 224)
#define __devhost_method_fileguard_rsvd_3 (__ACTV_HIGHTABLE_START + 225)
#define __devhost_method_fileguard_rsvd_2 (__ACTV_HIGHTABLE_START + 226)
#define __devhost_method_fileguard_rsvd_1 (__ACTV_HIGHTABLE_START + 227)
#define __devhost_method_MAX (__ACTV_HIGHTABLE_START + 228)
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_core_load_driver) < 512,
            drvcall_devhost_core_load_driver_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_core_create_device) < 512,
            drvcall_devhost_core_create_device_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_core_unload_driver) < 512,
            drvcall_devhost_core_unload_driver_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_core_bind_device) < 512,
            drvcall_devhost_core_bind_device_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_core_entropy_read) < 512,
            drvcall_devhost_core_entropy_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_rpc_export) < 512,
            drvcall_devhost_rpc_export_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_rpc_invoke) < 512,
            drvcall_devhost_rpc_invoke_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_open) < 512,
            drvcall_devhost_fops_open_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_close) < 512,
            drvcall_devhost_fops_close_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_read) < 512,
            drvcall_devhost_fops_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_write) < 512,
            drvcall_devhost_fops_write_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_read_iter) < 512,
            drvcall_devhost_fops_read_iter_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_write_iter) < 512,
            drvcall_devhost_fops_write_iter_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_ioctl) < 512,
            drvcall_devhost_fops_ioctl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_flush) < 512,
            drvcall_devhost_fops_flush_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_fcntl) < 512,
            drvcall_devhost_fops_fcntl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_llseek) < 512,
            drvcall_devhost_fops_llseek_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_mmap) < 512,
            drvcall_devhost_fops_mmap_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_munmap) < 512,
            drvcall_devhost_fops_munmap_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_do_page_fault) < 512,
            drvcall_devhost_fops_do_page_fault_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_mm_release) < 512,
            drvcall_devhost_fops_mm_release_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_sysfs_read) < 512,
            drvcall_devhost_fops_sysfs_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_sysfs_write) < 512,
            drvcall_devhost_fops_sysfs_write_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_poll) < 512,
            drvcall_devhost_fops_poll_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_poll_remove) < 512,
            drvcall_devhost_fops_poll_remove_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_epoll_ctl) < 512,
            drvcall_devhost_fops_epoll_ctl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_epoll_remove) < 512,
            drvcall_devhost_fops_epoll_remove_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_epoll_recheck) < 512,
            drvcall_devhost_fops_epoll_recheck_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_sync_ctrlmem_polllist) < 512,
            drvcall_devhost_fops_sync_ctrlmem_polllist_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fops_file_get) < 512,
            drvcall_devhost_fops_file_get_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_block_check_feature) < 512,
            drvcall_devhost_block_check_feature_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_block_submit_bio) < 512,
            drvcall_devhost_block_submit_bio_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_block_submit_cmd) < 512,
            drvcall_devhost_block_submit_cmd_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_block_run_queue) < 512,
            drvcall_devhost_block_run_queue_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_block_direct_access) < 512,
            drvcall_devhost_block_direct_access_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_block_register_bio_notify) < 512,
            drvcall_devhost_block_register_bio_notify_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_send) < 512,
            drvcall_devhost_net_send_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_ioctl) < 512,
            drvcall_devhost_net_ioctl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_status) < 512,
            drvcall_devhost_net_status_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_prepare) < 512,
            drvcall_devhost_net_prepare_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_init) < 512,
            drvcall_devhost_net_init_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_netlink_recv) < 512,
            drvcall_devhost_net_netlink_recv_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_register) < 512,
            drvcall_devhost_net_socket_register_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_close) < 512,
            drvcall_devhost_net_socket_close_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_connect) < 512,
            drvcall_devhost_net_socket_connect_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_bind) < 512,
            drvcall_devhost_net_socket_bind_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_listen) < 512,
            drvcall_devhost_net_socket_listen_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_accept) < 512,
            drvcall_devhost_net_socket_accept_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_shutdown) < 512,
            drvcall_devhost_net_socket_shutdown_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_ioctl) < 512,
            drvcall_devhost_net_socket_ioctl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_dev_ioctl) < 512,
            drvcall_devhost_net_socket_dev_ioctl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_fcntl) < 512,
            drvcall_devhost_net_socket_fcntl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_send) < 512,
            drvcall_devhost_net_socket_send_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_recv) < 512,
            drvcall_devhost_net_socket_recv_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_sendmmsg) < 512,
            drvcall_devhost_net_socket_sendmmsg_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_recvmmsg) < 512,
            drvcall_devhost_net_socket_recvmmsg_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_sendmsg) < 512,
            drvcall_devhost_net_socket_sendmsg_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_recvmsg) < 512,
            drvcall_devhost_net_socket_recvmsg_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_sendto) < 512,
            drvcall_devhost_net_socket_sendto_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_recvfrom) < 512,
            drvcall_devhost_net_socket_recvfrom_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_readv) < 512,
            drvcall_devhost_net_socket_readv_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_writev) < 512,
            drvcall_devhost_net_socket_writev_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_getsockopt) < 512,
            drvcall_devhost_net_socket_getsockopt_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_setsockopt) < 512,
            drvcall_devhost_net_socket_setsockopt_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_getsockname) < 512,
            drvcall_devhost_net_socket_getsockname_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_getpeername) < 512,
            drvcall_devhost_net_socket_getpeername_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_fchown) < 512,
            drvcall_devhost_net_socket_fchown_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_fstat) < 512,
            drvcall_devhost_net_socket_fstat_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_socket) < 512,
            drvcall_devhost_net_socket_socket_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_alloc_new_ns) < 512,
            drvcall_devhost_net_socket_alloc_new_ns_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_destroy_ns) < 512,
            drvcall_devhost_net_socket_destroy_ns_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_net_socket_bpf) < 512,
            drvcall_devhost_net_socket_bpf_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_iaware_notify_thread_msg) < 512,
            drvcall_devhost_iaware_notify_thread_msg_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_kstate_binderinfo_notify) < 512,
            drvcall_devhost_kstate_binderinfo_notify_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_kstate_killinfo_notify) < 512,
            drvcall_devhost_kstate_killinfo_notify_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_kstate_freeze_binder_info_notify) < 512,
            drvcall_devhost_kstate_freeze_binder_info_notify_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_kstate_request_anco_thaw) < 512,
            drvcall_devhost_kstate_request_anco_thaw_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_pm_action) < 512,
            drvcall_devhost_pm_action_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_pm_dubai_hwlog) < 512,
            drvcall_devhost_pm_dubai_hwlog_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_pm_prepare) < 512,
            drvcall_devhost_pm_prepare_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_pm_power_off) < 512,
            drvcall_devhost_pm_power_off_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_pm_reboot) < 512,
            drvcall_devhost_pm_reboot_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_pm_cpuhp_action) < 512,
            drvcall_devhost_pm_cpuhp_action_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_pm_wl_stat) < 512,
            drvcall_devhost_pm_wl_stat_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_procfs_open) < 512,
            drvcall_devhost_procfs_open_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_procfs_close) < 512,
            drvcall_devhost_procfs_close_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_procfs_read) < 512,
            drvcall_devhost_procfs_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_procfs_write) < 512,
            drvcall_devhost_procfs_write_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_sysfs_open) < 512,
            drvcall_devhost_sysfs_open_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_sysfs_close) < 512,
            drvcall_devhost_sysfs_close_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_sysfs_read) < 512,
            drvcall_devhost_sysfs_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_sysfs_write) < 512,
            drvcall_devhost_sysfs_write_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_sysfs_poll) < 512,
            drvcall_devhost_sysfs_poll_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_clock_settime) < 512,
            drvcall_devhost_clock_settime_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_clock_gettime) < 512,
            drvcall_devhost_clock_gettime_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_clock_getres) < 512,
            drvcall_devhost_clock_getres_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_clock_adjtime) < 512,
            drvcall_devhost_clock_adjtime_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_clock_poweroff_alarm) < 512,
            drvcall_devhost_clock_poweroff_alarm_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_mounted) < 512,
            drvcall_devhost_transfs_mounted_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_unmounted) < 512,
            drvcall_devhost_transfs_unmounted_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_drop_caches) < 512,
            drvcall_devhost_transfs_drop_caches_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_mkdir) < 512,
            drvcall_devhost_transfs_mkdir_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_rmdir) < 512,
            drvcall_devhost_transfs_rmdir_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_create) < 512,
            drvcall_devhost_transfs_create_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_unlink) < 512,
            drvcall_devhost_transfs_unlink_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_mknod) < 512,
            drvcall_devhost_transfs_mknod_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_link) < 512,
            drvcall_devhost_transfs_link_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_symlink) < 512,
            drvcall_devhost_transfs_symlink_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_statfs) < 512,
            drvcall_devhost_transfs_statfs_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_rename) < 512,
            drvcall_devhost_transfs_rename_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_lookup) < 512,
            drvcall_devhost_transfs_lookup_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_setattr) < 512,
            drvcall_devhost_transfs_setattr_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_readlink) < 512,
            drvcall_devhost_transfs_readlink_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_getattr) < 512,
            drvcall_devhost_transfs_getattr_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_listxattr) < 512,
            drvcall_devhost_transfs_listxattr_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_getxattr) < 512,
            drvcall_devhost_transfs_getxattr_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_setxattr) < 512,
            drvcall_devhost_transfs_setxattr_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_tmpfile) < 512,
            drvcall_devhost_transfs_tmpfile_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_kill_dentry) < 512,
            drvcall_devhost_transfs_kill_dentry_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_node_put) < 512,
            drvcall_devhost_transfs_node_put_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_sync) < 512,
            drvcall_devhost_transfs_sync_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_write) < 512,
            drvcall_devhost_transfs_write_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_read) < 512,
            drvcall_devhost_transfs_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_write_iter) < 512,
            drvcall_devhost_transfs_write_iter_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_fsync) < 512,
            drvcall_devhost_transfs_fsync_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_readdir) < 512,
            drvcall_devhost_transfs_readdir_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_open) < 512,
            drvcall_devhost_transfs_open_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_fallocate) < 512,
            drvcall_devhost_transfs_fallocate_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_close) < 512,
            drvcall_devhost_transfs_close_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_ioctl) < 512,
            drvcall_devhost_transfs_ioctl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_direct_IO) < 512,
            drvcall_devhost_transfs_direct_IO_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_update_opts) < 512,
            drvcall_devhost_transfs_update_opts_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_transfs_iommap) < 512,
            drvcall_devhost_transfs_iommap_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_shrinker_query) < 512,
            drvcall_devhost_shrinker_query_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_shrinker_shrink) < 512,
            drvcall_devhost_shrinker_shrink_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_process_revoke) < 512,
            drvcall_devhost_process_revoke_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_sec_mgmt) < 512,
            drvcall_devhost_sec_mgmt_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb) < 512,
            drvcall_devhost_sec_test_hkip_seharmony_prot_policydb_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_tests_hkip_protection) < 512,
            drvcall_devhost_tests_hkip_protection_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_sec_test_hkip_roafter_prot) < 512,
            drvcall_devhost_sec_test_hkip_roafter_prot_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_sec_sync_fastpath_open) < 512,
            drvcall_devhost_sec_sync_fastpath_open_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_sec_sync_fastpath_dev) < 512,
            drvcall_devhost_sec_sync_fastpath_dev_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_devfs_read) < 512,
            drvcall_devhost_devfs_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_devfs_write) < 512,
            drvcall_devhost_devfs_write_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_devfs_ioctl) < 512,
            drvcall_devhost_devfs_ioctl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_devfs_readv) < 512,
            drvcall_devhost_devfs_readv_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_devfs_writev) < 512,
            drvcall_devhost_devfs_writev_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_devfs_lseek) < 512,
            drvcall_devhost_devfs_lseek_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_devfs_fcntl) < 512,
            drvcall_devhost_devfs_fcntl_too_many_actvret_data);
#ifdef __arm__
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_devfs__llseek) < 512,
            drvcall_devhost_devfs__llseek_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_devfs_fcntl64) < 512,
            drvcall_devhost_devfs_fcntl64_too_many_actvret_data);
#endif
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_trace_enable_disable_ldk_event) < 512,
            drvcall_devhost_trace_enable_disable_ldk_event_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_trace_ldk_event_format) < 512,
            drvcall_devhost_trace_ldk_event_format_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_trace_print_ldk_event) < 512,
            drvcall_devhost_trace_print_ldk_event_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_file_info_query_dmabuf) < 512,
            drvcall_devhost_file_info_query_dmabuf_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_file_info_query_fence) < 512,
            drvcall_devhost_file_info_query_fence_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_file_info_query_ion) < 512,
            drvcall_devhost_file_info_query_ion_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_file_info_query_meminfo) < 512,
            drvcall_devhost_file_info_query_meminfo_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_file_info_lowmem_debug) < 512,
            drvcall_devhost_file_info_lowmem_debug_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_file_info_query_gpu) < 512,
            drvcall_devhost_file_info_query_gpu_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_probe_handle_brk) < 512,
            drvcall_devhost_probe_handle_brk_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_probe_handle_ss_brk) < 512,
            drvcall_devhost_probe_handle_ss_brk_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_fileguard_message_notify) < 512,
            drvcall_devhost_fileguard_message_notify_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_drvcall_devhost_posix_vhangup) < 512,
            drvcall_devhost_posix_vhangup_too_many_actvret_data);
raw_static_assert(sizeof(union __actvret_drvcall_devhost) < 512,
        drvcall_devhost_too_many_actvret_data);
extern int __actvcall_drvcall_devhost_core_load_driver(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, struct hm_actv_buf args, unsigned long long drv_shm_key, unsigned long size);


extern int __actvcapcall_drvcall_devhost_core_load_driver(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, struct hm_actv_buf args, unsigned long long drv_shm_key, unsigned long size);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_core_load_driver(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf name, struct hm_actv_buf args, unsigned long long drv_shm_key, unsigned long size)
{
	const unsigned long __callno = __devhost_method_core_load_driver;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_core_load_driver(rpc_info, NULL, name, args, drv_shm_key, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_core_load_driver(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf name, struct hm_actv_buf args, unsigned long long drv_shm_key, unsigned long size)
{
	const unsigned long __callno = __devhost_method_core_load_driver;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_core_load_driver(__rref, rpc_info, NULL, name, args, drv_shm_key, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_core_load_driver(struct hm_actv_buf name, struct hm_actv_buf args, unsigned long long drv_shm_key, unsigned long size)
{
	return actvxactcall_drvcall_devhost_core_load_driver(false, false, name, args, drv_shm_key, size);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_core_load_driver(rref_t __rref, struct hm_actv_buf name, struct hm_actv_buf args, unsigned long long drv_shm_key, unsigned long size)
{
	return actvxactcapcall_drvcall_devhost_core_load_driver(false, false, __rref, name, args, drv_shm_key, size);
}

extern int __actvcall_drvcall_devhost_core_create_device(unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, struct hm_actv_buf devname, struct hm_actv_buf dev_compat, int parent_devid, unsigned int resource_count);


extern int __actvcapcall_drvcall_devhost_core_create_device(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, struct hm_actv_buf devname, struct hm_actv_buf dev_compat, int parent_devid, unsigned int resource_count);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_core_create_device(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf devname, struct hm_actv_buf dev_compat, int parent_devid, unsigned int resource_count, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_core_create_device *__ret)
{
	const unsigned long __callno = __devhost_method_core_create_device;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 1);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_core_create_device(rpc_info, ___void_buf, ptr_to_void(__ret), sizeof(*__ret), devname, dev_compat, parent_devid, resource_count);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_core_create_device(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf devname, struct hm_actv_buf dev_compat, int parent_devid, unsigned int resource_count, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_core_create_device *__ret)
{
	const unsigned long __callno = __devhost_method_core_create_device;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 1);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_core_create_device(__rref, rpc_info, ___void_buf, ptr_to_void(__ret), sizeof(*__ret), devname, dev_compat, parent_devid, resource_count);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_core_create_device(struct hm_actv_buf devname, struct hm_actv_buf dev_compat, int parent_devid, unsigned int resource_count, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_core_create_device *__ret)
{
	return actvxactcall_drvcall_devhost_core_create_device(false, false, devname, dev_compat, parent_devid, resource_count, ___void_buf, ___void_buf_sz, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_core_create_device(rref_t __rref, struct hm_actv_buf devname, struct hm_actv_buf dev_compat, int parent_devid, unsigned int resource_count, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_core_create_device *__ret)
{
	return actvxactcapcall_drvcall_devhost_core_create_device(false, false, __rref, devname, dev_compat, parent_devid, resource_count, ___void_buf, ___void_buf_sz, __ret);
}

extern int __actvcall_drvcall_devhost_core_unload_driver(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf drvname);


extern int __actvcapcall_drvcall_devhost_core_unload_driver(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf drvname);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_core_unload_driver(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf drvname)
{
	const unsigned long __callno = __devhost_method_core_unload_driver;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_core_unload_driver(rpc_info, NULL, drvname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_core_unload_driver(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf drvname)
{
	const unsigned long __callno = __devhost_method_core_unload_driver;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_core_unload_driver(__rref, rpc_info, NULL, drvname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_core_unload_driver(struct hm_actv_buf drvname)
{
	return actvxactcall_drvcall_devhost_core_unload_driver(false, false, drvname);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_core_unload_driver(rref_t __rref, struct hm_actv_buf drvname)
{
	return actvxactcapcall_drvcall_devhost_core_unload_driver(false, false, __rref, drvname);
}

extern int __actvcall_drvcall_devhost_core_bind_device(unsigned long long __rpc_info, void *__buf, int devid, struct hm_actv_buf dev_compat);


extern int __actvcapcall_drvcall_devhost_core_bind_device(rref_t __rref, unsigned long long __rpc_info, void *__buf, int devid, struct hm_actv_buf dev_compat);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_core_bind_device(bool is_xact_fwd, bool is_xact_allow_refwd, int devid, struct hm_actv_buf dev_compat)
{
	const unsigned long __callno = __devhost_method_core_bind_device;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_core_bind_device(rpc_info, NULL, devid, dev_compat);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_core_bind_device(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int devid, struct hm_actv_buf dev_compat)
{
	const unsigned long __callno = __devhost_method_core_bind_device;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_core_bind_device(__rref, rpc_info, NULL, devid, dev_compat);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_core_bind_device(int devid, struct hm_actv_buf dev_compat)
{
	return actvxactcall_drvcall_devhost_core_bind_device(false, false, devid, dev_compat);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_core_bind_device(rref_t __rref, int devid, struct hm_actv_buf dev_compat)
{
	return actvxactcapcall_drvcall_devhost_core_bind_device(false, false, __rref, devid, dev_compat);
}

extern int __actvcall_drvcall_devhost_core_entropy_read(unsigned long long __rpc_info, void *__buf, unsigned int num);


extern int __actvcapcall_drvcall_devhost_core_entropy_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int num);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_core_entropy_read(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int num, struct __actvret_drvcall_devhost_core_entropy_read *__ret)
{
	const unsigned long __callno = __devhost_method_core_entropy_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_core_entropy_read(rpc_info, __ret, num);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_core_entropy_read(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int num, struct __actvret_drvcall_devhost_core_entropy_read *__ret)
{
	const unsigned long __callno = __devhost_method_core_entropy_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_core_entropy_read(__rref, rpc_info, __ret, num);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_core_entropy_read(unsigned int num, struct __actvret_drvcall_devhost_core_entropy_read *__ret)
{
	return actvxactcall_drvcall_devhost_core_entropy_read(false, false, num, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_core_entropy_read(rref_t __rref, unsigned int num, struct __actvret_drvcall_devhost_core_entropy_read *__ret)
{
	return actvxactcapcall_drvcall_devhost_core_entropy_read(false, false, __rref, num, __ret);
}

extern int __actvcall_drvcall_devhost_rpc_export(unsigned long long __rpc_info, void *__buf, int devid, unsigned int cnode_idx);


extern int __actvcapcall_drvcall_devhost_rpc_export(rref_t __rref, unsigned long long __rpc_info, void *__buf, int devid, unsigned int cnode_idx);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_rpc_export(bool is_xact_fwd, bool is_xact_allow_refwd, int devid, unsigned int cnode_idx, struct __actvret_drvcall_devhost_rpc_export *__ret)
{
	const unsigned long __callno = __devhost_method_rpc_export;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_rpc_export(rpc_info, __ret, devid, cnode_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_rpc_export(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int devid, unsigned int cnode_idx, struct __actvret_drvcall_devhost_rpc_export *__ret)
{
	const unsigned long __callno = __devhost_method_rpc_export;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_rpc_export(__rref, rpc_info, __ret, devid, cnode_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_rpc_export(int devid, unsigned int cnode_idx, struct __actvret_drvcall_devhost_rpc_export *__ret)
{
	return actvxactcall_drvcall_devhost_rpc_export(false, false, devid, cnode_idx, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_rpc_export(rref_t __rref, int devid, unsigned int cnode_idx, struct __actvret_drvcall_devhost_rpc_export *__ret)
{
	return actvxactcapcall_drvcall_devhost_rpc_export(false, false, __rref, devid, cnode_idx, __ret);
}

extern int __actvcall_drvcall_devhost_rpc_invoke(unsigned long long __rpc_info, void *__buf, int devid, int cmd, unsigned long req_ubuf, unsigned long req_size, unsigned long resp_ubuf, unsigned long resp_size);


extern int __actvcapcall_drvcall_devhost_rpc_invoke(rref_t __rref, unsigned long long __rpc_info, void *__buf, int devid, int cmd, unsigned long req_ubuf, unsigned long req_size, unsigned long resp_ubuf, unsigned long resp_size);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_rpc_invoke(bool is_xact_fwd, bool is_xact_allow_refwd, int devid, int cmd, unsigned long req_ubuf, unsigned long req_size, unsigned long resp_ubuf, unsigned long resp_size, struct __actvret_drvcall_devhost_rpc_invoke *__ret)
{
	const unsigned long __callno = __devhost_method_rpc_invoke;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_rpc_invoke(rpc_info, __ret, devid, cmd, req_ubuf, req_size, resp_ubuf, resp_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_rpc_invoke(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int devid, int cmd, unsigned long req_ubuf, unsigned long req_size, unsigned long resp_ubuf, unsigned long resp_size, struct __actvret_drvcall_devhost_rpc_invoke *__ret)
{
	const unsigned long __callno = __devhost_method_rpc_invoke;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_rpc_invoke(__rref, rpc_info, __ret, devid, cmd, req_ubuf, req_size, resp_ubuf, resp_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_rpc_invoke(int devid, int cmd, unsigned long req_ubuf, unsigned long req_size, unsigned long resp_ubuf, unsigned long resp_size, struct __actvret_drvcall_devhost_rpc_invoke *__ret)
{
	return actvxactcall_drvcall_devhost_rpc_invoke(false, false, devid, cmd, req_ubuf, req_size, resp_ubuf, resp_size, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_rpc_invoke(rref_t __rref, int devid, int cmd, unsigned long req_ubuf, unsigned long req_size, unsigned long resp_ubuf, unsigned long resp_size, struct __actvret_drvcall_devhost_rpc_invoke *__ret)
{
	return actvxactcapcall_drvcall_devhost_rpc_invoke(false, false, __rref, devid, cmd, req_ubuf, req_size, resp_ubuf, resp_size, __ret);
}

extern int __actvcall_drvcall_devhost_fops_open(unsigned long long __rpc_info, void *__buf, unsigned int u_cidx, int devid, rref_t tgt_rref, unsigned int vfs_flags);


extern int __actvcapcall_drvcall_devhost_fops_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int u_cidx, int devid, rref_t tgt_rref, unsigned int vfs_flags);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_open(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int u_cidx, int devid, rref_t tgt_rref, unsigned int vfs_flags, struct __actvret_drvcall_devhost_fops_open *__ret)
{
	const unsigned long __callno = __devhost_method_fops_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(rref_t), __arch_actv_rpc_stksz_alignment(rref_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_open(rpc_info, __ret, u_cidx, devid, tgt_rref, vfs_flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_open(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int u_cidx, int devid, rref_t tgt_rref, unsigned int vfs_flags, struct __actvret_drvcall_devhost_fops_open *__ret)
{
	const unsigned long __callno = __devhost_method_fops_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(rref_t), __arch_actv_rpc_stksz_alignment(rref_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_open(__rref, rpc_info, __ret, u_cidx, devid, tgt_rref, vfs_flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_open(unsigned int u_cidx, int devid, rref_t tgt_rref, unsigned int vfs_flags, struct __actvret_drvcall_devhost_fops_open *__ret)
{
	return actvxactcall_drvcall_devhost_fops_open(false, false, u_cidx, devid, tgt_rref, vfs_flags, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_open(rref_t __rref, unsigned int u_cidx, int devid, rref_t tgt_rref, unsigned int vfs_flags, struct __actvret_drvcall_devhost_fops_open *__ret)
{
	return actvxactcapcall_drvcall_devhost_fops_open(false, false, __rref, u_cidx, devid, tgt_rref, vfs_flags, __ret);
}

extern int __actvcall_drvcall_devhost_fops_close(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id);


extern int __actvcapcall_drvcall_devhost_fops_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_close(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_fops_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_close(rpc_info, NULL, fastpath, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_close(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_fops_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_close(__rref, rpc_info, NULL, fastpath, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_close(unsigned int fastpath, unsigned long long file_id)
{
	return actvxactcall_drvcall_devhost_fops_close(false, false, fastpath, file_id);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_close(rref_t __rref, unsigned int fastpath, unsigned long long file_id)
{
	return actvxactcapcall_drvcall_devhost_fops_close(false, false, __rref, fastpath, file_id);
}

extern int __actvcall_drvcall_devhost_fops_read(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long buf, unsigned long long filep, unsigned long long offset, unsigned long count, unsigned int flags);


extern int __actvcapcall_drvcall_devhost_fops_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long buf, unsigned long long filep, unsigned long long offset, unsigned long count, unsigned int flags);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_read(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long buf, unsigned long long filep, unsigned long long offset, unsigned long count, unsigned int flags, struct __actvret_drvcall_devhost_fops_read *__ret)
{
	const unsigned long __callno = __devhost_method_fops_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_read(rpc_info, __ret, fastpath, buf, filep, offset, count, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_read(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long buf, unsigned long long filep, unsigned long long offset, unsigned long count, unsigned int flags, struct __actvret_drvcall_devhost_fops_read *__ret)
{
	const unsigned long __callno = __devhost_method_fops_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_read(__rref, rpc_info, __ret, fastpath, buf, filep, offset, count, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_read(unsigned int fastpath, unsigned long buf, unsigned long long filep, unsigned long long offset, unsigned long count, unsigned int flags, struct __actvret_drvcall_devhost_fops_read *__ret)
{
	return actvxactcall_drvcall_devhost_fops_read(false, false, fastpath, buf, filep, offset, count, flags, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_read(rref_t __rref, unsigned int fastpath, unsigned long buf, unsigned long long filep, unsigned long long offset, unsigned long count, unsigned int flags, struct __actvret_drvcall_devhost_fops_read *__ret)
{
	return actvxactcapcall_drvcall_devhost_fops_read(false, false, __rref, fastpath, buf, filep, offset, count, flags, __ret);
}

extern int __actvcall_drvcall_devhost_fops_write(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long buf, unsigned long long file_id, unsigned long long offset, unsigned long count, unsigned int flags);


extern int __actvcapcall_drvcall_devhost_fops_write(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long buf, unsigned long long file_id, unsigned long long offset, unsigned long count, unsigned int flags);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_write(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long buf, unsigned long long file_id, unsigned long long offset, unsigned long count, unsigned int flags, struct __actvret_drvcall_devhost_fops_write *__ret)
{
	const unsigned long __callno = __devhost_method_fops_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_write(rpc_info, __ret, fastpath, buf, file_id, offset, count, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_write(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long buf, unsigned long long file_id, unsigned long long offset, unsigned long count, unsigned int flags, struct __actvret_drvcall_devhost_fops_write *__ret)
{
	const unsigned long __callno = __devhost_method_fops_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_write(__rref, rpc_info, __ret, fastpath, buf, file_id, offset, count, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_write(unsigned int fastpath, unsigned long buf, unsigned long long file_id, unsigned long long offset, unsigned long count, unsigned int flags, struct __actvret_drvcall_devhost_fops_write *__ret)
{
	return actvxactcall_drvcall_devhost_fops_write(false, false, fastpath, buf, file_id, offset, count, flags, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_write(rref_t __rref, unsigned int fastpath, unsigned long buf, unsigned long long file_id, unsigned long long offset, unsigned long count, unsigned int flags, struct __actvret_drvcall_devhost_fops_write *__ret)
{
	return actvxactcapcall_drvcall_devhost_fops_write(false, false, __rref, fastpath, buf, file_id, offset, count, flags, __ret);
}

extern int __actvcall_drvcall_devhost_fops_read_iter(unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flag);


extern int __actvcapcall_drvcall_devhost_fops_read_iter(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flag);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_read_iter(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flag, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_fops_read_iter *__ret)
{
	const unsigned long __callno = __devhost_method_fops_read_iter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 1);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_read_iter(rpc_info, ___void_buf, ptr_to_void(__ret), sizeof(*__ret), fastpath, file_id, offset, vec, nr_segs, count, flag);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_read_iter(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flag, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_fops_read_iter *__ret)
{
	const unsigned long __callno = __devhost_method_fops_read_iter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 1);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_read_iter(__rref, rpc_info, ___void_buf, ptr_to_void(__ret), sizeof(*__ret), fastpath, file_id, offset, vec, nr_segs, count, flag);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_read_iter(unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flag, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_fops_read_iter *__ret)
{
	return actvxactcall_drvcall_devhost_fops_read_iter(false, false, fastpath, file_id, offset, vec, nr_segs, count, flag, ___void_buf, ___void_buf_sz, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_read_iter(rref_t __rref, unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flag, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_fops_read_iter *__ret)
{
	return actvxactcapcall_drvcall_devhost_fops_read_iter(false, false, __rref, fastpath, file_id, offset, vec, nr_segs, count, flag, ___void_buf, ___void_buf_sz, __ret);
}

extern int __actvcall_drvcall_devhost_fops_write_iter(unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flags);


extern int __actvcapcall_drvcall_devhost_fops_write_iter(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flags);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_write_iter(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flags, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_fops_write_iter *__ret)
{
	const unsigned long __callno = __devhost_method_fops_write_iter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 1);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_write_iter(rpc_info, ___void_buf, ptr_to_void(__ret), sizeof(*__ret), fastpath, file_id, offset, vec, nr_segs, count, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_write_iter(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flags, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_fops_write_iter *__ret)
{
	const unsigned long __callno = __devhost_method_fops_write_iter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 1);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_write_iter(__rref, rpc_info, ___void_buf, ptr_to_void(__ret), sizeof(*__ret), fastpath, file_id, offset, vec, nr_segs, count, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_write_iter(unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flags, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_fops_write_iter *__ret)
{
	return actvxactcall_drvcall_devhost_fops_write_iter(false, false, fastpath, file_id, offset, vec, nr_segs, count, flags, ___void_buf, ___void_buf_sz, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_write_iter(rref_t __rref, unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flags, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_fops_write_iter *__ret)
{
	return actvxactcapcall_drvcall_devhost_fops_write_iter(false, false, __rref, fastpath, file_id, offset, vec, nr_segs, count, flags, ___void_buf, ___void_buf_sz, __ret);
}

extern int __actvcall_drvcall_devhost_fops_ioctl(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg);


extern int __actvcapcall_drvcall_devhost_fops_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg)
{
	const unsigned long __callno = __devhost_method_fops_ioctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_ioctl(rpc_info, NULL, fastpath, request, file_id, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg)
{
	const unsigned long __callno = __devhost_method_fops_ioctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_ioctl(__rref, rpc_info, NULL, fastpath, request, file_id, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_ioctl(unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg)
{
	return actvxactcall_drvcall_devhost_fops_ioctl(false, false, fastpath, request, file_id, arg);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_ioctl(rref_t __rref, unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg)
{
	return actvxactcapcall_drvcall_devhost_fops_ioctl(false, false, __rref, fastpath, request, file_id, arg);
}

extern int __actvcall_drvcall_devhost_fops_flush(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id);


extern int __actvcapcall_drvcall_devhost_fops_flush(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_flush(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_fops_flush;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_flush(rpc_info, NULL, fastpath, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_flush(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_fops_flush;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_flush(__rref, rpc_info, NULL, fastpath, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_flush(unsigned int fastpath, unsigned long long file_id)
{
	return actvxactcall_drvcall_devhost_fops_flush(false, false, fastpath, file_id);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_flush(rref_t __rref, unsigned int fastpath, unsigned long long file_id)
{
	return actvxactcapcall_drvcall_devhost_fops_flush(false, false, __rref, fastpath, file_id);
}

extern int __actvcall_drvcall_devhost_fops_fcntl(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg);


extern int __actvcapcall_drvcall_devhost_fops_fcntl(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_fcntl(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg)
{
	const unsigned long __callno = __devhost_method_fops_fcntl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_fcntl(rpc_info, NULL, fastpath, request, file_id, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_fcntl(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg)
{
	const unsigned long __callno = __devhost_method_fops_fcntl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_fcntl(__rref, rpc_info, NULL, fastpath, request, file_id, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_fcntl(unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg)
{
	return actvxactcall_drvcall_devhost_fops_fcntl(false, false, fastpath, request, file_id, arg);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_fcntl(rref_t __rref, unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg)
{
	return actvxactcapcall_drvcall_devhost_fops_fcntl(false, false, __rref, fastpath, request, file_id, arg);
}

extern int __actvcall_drvcall_devhost_fops_llseek(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, int whence, unsigned long long file_id, long long offset);


extern int __actvcapcall_drvcall_devhost_fops_llseek(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, int whence, unsigned long long file_id, long long offset);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_llseek(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, int whence, unsigned long long file_id, long long offset, struct __actvret_drvcall_devhost_fops_llseek *__ret)
{
	const unsigned long __callno = __devhost_method_fops_llseek;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_llseek(rpc_info, __ret, fastpath, whence, file_id, offset);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_llseek(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, int whence, unsigned long long file_id, long long offset, struct __actvret_drvcall_devhost_fops_llseek *__ret)
{
	const unsigned long __callno = __devhost_method_fops_llseek;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_llseek(__rref, rpc_info, __ret, fastpath, whence, file_id, offset);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_llseek(unsigned int fastpath, int whence, unsigned long long file_id, long long offset, struct __actvret_drvcall_devhost_fops_llseek *__ret)
{
	return actvxactcall_drvcall_devhost_fops_llseek(false, false, fastpath, whence, file_id, offset, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_llseek(rref_t __rref, unsigned int fastpath, int whence, unsigned long long file_id, long long offset, struct __actvret_drvcall_devhost_fops_llseek *__ret)
{
	return actvxactcapcall_drvcall_devhost_fops_llseek(false, false, __rref, fastpath, whence, file_id, offset, __ret);
}

extern int __actvcall_drvcall_devhost_fops_mmap(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long addr, unsigned long long file_id, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset, bool iomap_prepare_ahead);


extern int __actvcapcall_drvcall_devhost_fops_mmap(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long addr, unsigned long long file_id, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset, bool iomap_prepare_ahead);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_mmap(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long addr, unsigned long long file_id, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset, bool iomap_prepare_ahead, struct __actvret_drvcall_devhost_fops_mmap *__ret)
{
	const unsigned long __callno = __devhost_method_fops_mmap;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_mmap(rpc_info, __ret, fastpath, addr, file_id, length, prot, flags, offset, iomap_prepare_ahead);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_mmap(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long addr, unsigned long long file_id, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset, bool iomap_prepare_ahead, struct __actvret_drvcall_devhost_fops_mmap *__ret)
{
	const unsigned long __callno = __devhost_method_fops_mmap;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_mmap(__rref, rpc_info, __ret, fastpath, addr, file_id, length, prot, flags, offset, iomap_prepare_ahead);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_mmap(unsigned int fastpath, unsigned long addr, unsigned long long file_id, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset, bool iomap_prepare_ahead, struct __actvret_drvcall_devhost_fops_mmap *__ret)
{
	return actvxactcall_drvcall_devhost_fops_mmap(false, false, fastpath, addr, file_id, length, prot, flags, offset, iomap_prepare_ahead, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_mmap(rref_t __rref, unsigned int fastpath, unsigned long addr, unsigned long long file_id, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset, bool iomap_prepare_ahead, struct __actvret_drvcall_devhost_fops_mmap *__ret)
{
	return actvxactcapcall_drvcall_devhost_fops_mmap(false, false, __rref, fastpath, addr, file_id, length, prot, flags, offset, iomap_prepare_ahead, __ret);
}

extern int __actvcall_drvcall_devhost_fops_munmap(unsigned long long __rpc_info, void *__buf, unsigned int key, unsigned long start, unsigned long len, int vs_is_releasing);


extern int __actvcapcall_drvcall_devhost_fops_munmap(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int key, unsigned long start, unsigned long len, int vs_is_releasing);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_munmap(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int key, unsigned long start, unsigned long len, int vs_is_releasing)
{
	const unsigned long __callno = __devhost_method_fops_munmap;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_munmap(rpc_info, NULL, key, start, len, vs_is_releasing);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_munmap(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int key, unsigned long start, unsigned long len, int vs_is_releasing)
{
	const unsigned long __callno = __devhost_method_fops_munmap;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_munmap(__rref, rpc_info, NULL, key, start, len, vs_is_releasing);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_munmap(unsigned int key, unsigned long start, unsigned long len, int vs_is_releasing)
{
	return actvxactcall_drvcall_devhost_fops_munmap(false, false, key, start, len, vs_is_releasing);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_munmap(rref_t __rref, unsigned int key, unsigned long start, unsigned long len, int vs_is_releasing)
{
	return actvxactcapcall_drvcall_devhost_fops_munmap(false, false, __rref, key, start, len, vs_is_releasing);
}

extern int __actvcall_drvcall_devhost_fops_do_page_fault(unsigned long long __rpc_info, void *__buf, unsigned int vma_key, unsigned long long vr_key, unsigned long long addr, unsigned int flags, unsigned long long offset);


extern int __actvcapcall_drvcall_devhost_fops_do_page_fault(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int vma_key, unsigned long long vr_key, unsigned long long addr, unsigned int flags, unsigned long long offset);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_do_page_fault(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int vma_key, unsigned long long vr_key, unsigned long long addr, unsigned int flags, unsigned long long offset)
{
	const unsigned long __callno = __devhost_method_fops_do_page_fault;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_do_page_fault(rpc_info, NULL, vma_key, vr_key, addr, flags, offset);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_do_page_fault(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int vma_key, unsigned long long vr_key, unsigned long long addr, unsigned int flags, unsigned long long offset)
{
	const unsigned long __callno = __devhost_method_fops_do_page_fault;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_do_page_fault(__rref, rpc_info, NULL, vma_key, vr_key, addr, flags, offset);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_do_page_fault(unsigned int vma_key, unsigned long long vr_key, unsigned long long addr, unsigned int flags, unsigned long long offset)
{
	return actvxactcall_drvcall_devhost_fops_do_page_fault(false, false, vma_key, vr_key, addr, flags, offset);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_do_page_fault(rref_t __rref, unsigned int vma_key, unsigned long long vr_key, unsigned long long addr, unsigned int flags, unsigned long long offset)
{
	return actvxactcapcall_drvcall_devhost_fops_do_page_fault(false, false, __rref, vma_key, vr_key, addr, flags, offset);
}

extern int __actvcall_drvcall_devhost_fops_mm_release(unsigned long long __rpc_info, void *__buf, unsigned long long vspace_key);


extern int __actvcapcall_drvcall_devhost_fops_mm_release(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long vspace_key);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_mm_release(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long vspace_key)
{
	const unsigned long __callno = __devhost_method_fops_mm_release;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_mm_release(rpc_info, NULL, vspace_key);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_mm_release(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long vspace_key)
{
	const unsigned long __callno = __devhost_method_fops_mm_release;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_mm_release(__rref, rpc_info, NULL, vspace_key);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_mm_release(unsigned long long vspace_key)
{
	return actvxactcall_drvcall_devhost_fops_mm_release(false, false, vspace_key);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_mm_release(rref_t __rref, unsigned long long vspace_key)
{
	return actvxactcapcall_drvcall_devhost_fops_mm_release(false, false, __rref, vspace_key);
}

extern int __actvcall_drvcall_devhost_fops_sysfs_read(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devhost_fops_sysfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_sysfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devhost_method_fops_sysfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_sysfs_read(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_sysfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devhost_method_fops_sysfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_sysfs_read(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_sysfs_read(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_drvcall_devhost_fops_sysfs_read(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_sysfs_read(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_drvcall_devhost_fops_sysfs_read(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_drvcall_devhost_fops_sysfs_write(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devhost_fops_sysfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_sysfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devhost_method_fops_sysfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_sysfs_write(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_sysfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devhost_method_fops_sysfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_sysfs_write(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_sysfs_write(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_drvcall_devhost_fops_sysfs_write(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_sysfs_write(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_drvcall_devhost_fops_sysfs_write(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_drvcall_devhost_fops_poll(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, unsigned long long tcb_key, rref_t fdtable_rref, unsigned int events, int idx, int fd, int need_add);


extern int __actvcapcall_drvcall_devhost_fops_poll(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, unsigned long long tcb_key, rref_t fdtable_rref, unsigned int events, int idx, int fd, int need_add);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_poll(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long long file_id, unsigned long long tcb_key, rref_t fdtable_rref, unsigned int events, int idx, int fd, int need_add, struct __actvret_drvcall_devhost_fops_poll *__ret)
{
	const unsigned long __callno = __devhost_method_fops_poll;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(rref_t), __arch_actv_rpc_stksz_alignment(rref_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_poll(rpc_info, __ret, fastpath, file_id, tcb_key, fdtable_rref, events, idx, fd, need_add);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_poll(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long long file_id, unsigned long long tcb_key, rref_t fdtable_rref, unsigned int events, int idx, int fd, int need_add, struct __actvret_drvcall_devhost_fops_poll *__ret)
{
	const unsigned long __callno = __devhost_method_fops_poll;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(rref_t), __arch_actv_rpc_stksz_alignment(rref_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_poll(__rref, rpc_info, __ret, fastpath, file_id, tcb_key, fdtable_rref, events, idx, fd, need_add);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_poll(unsigned int fastpath, unsigned long long file_id, unsigned long long tcb_key, rref_t fdtable_rref, unsigned int events, int idx, int fd, int need_add, struct __actvret_drvcall_devhost_fops_poll *__ret)
{
	return actvxactcall_drvcall_devhost_fops_poll(false, false, fastpath, file_id, tcb_key, fdtable_rref, events, idx, fd, need_add, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_poll(rref_t __rref, unsigned int fastpath, unsigned long long file_id, unsigned long long tcb_key, rref_t fdtable_rref, unsigned int events, int idx, int fd, int need_add, struct __actvret_drvcall_devhost_fops_poll *__ret)
{
	return actvxactcapcall_drvcall_devhost_fops_poll(false, false, __rref, fastpath, file_id, tcb_key, fdtable_rref, events, idx, fd, need_add, __ret);
}

extern int __actvcall_drvcall_devhost_fops_poll_remove(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, int fd, cref_t tcb_cref);


extern int __actvcapcall_drvcall_devhost_fops_poll_remove(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, int fd, cref_t tcb_cref);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_poll_remove(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long long file_id, int fd, cref_t tcb_cref)
{
	const unsigned long __callno = __devhost_method_fops_poll_remove;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(cref_t), __arch_actv_rpc_stksz_alignment(cref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_poll_remove(rpc_info, NULL, fastpath, file_id, fd, tcb_cref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_poll_remove(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long long file_id, int fd, cref_t tcb_cref)
{
	const unsigned long __callno = __devhost_method_fops_poll_remove;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(cref_t), __arch_actv_rpc_stksz_alignment(cref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_poll_remove(__rref, rpc_info, NULL, fastpath, file_id, fd, tcb_cref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_poll_remove(unsigned int fastpath, unsigned long long file_id, int fd, cref_t tcb_cref)
{
	return actvxactcall_drvcall_devhost_fops_poll_remove(false, false, fastpath, file_id, fd, tcb_cref);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_poll_remove(rref_t __rref, unsigned int fastpath, unsigned long long file_id, int fd, cref_t tcb_cref)
{
	return actvxactcapcall_drvcall_devhost_fops_poll_remove(false, false, __rref, fastpath, file_id, fd, tcb_cref);
}

extern int __actvcall_drvcall_devhost_fops_epoll_ctl(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, const void *ep_arg);


extern int __actvcapcall_drvcall_devhost_fops_epoll_ctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, const void *ep_arg);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_epoll_ctl(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long long file_id, const void *ep_arg)
{
	const unsigned long __callno = __devhost_method_fops_epoll_ctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_epoll_ctl(rpc_info, NULL, fastpath, file_id, ep_arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_epoll_ctl(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long long file_id, const void *ep_arg)
{
	const unsigned long __callno = __devhost_method_fops_epoll_ctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_epoll_ctl(__rref, rpc_info, NULL, fastpath, file_id, ep_arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_epoll_ctl(unsigned int fastpath, unsigned long long file_id, const void *ep_arg)
{
	return actvxactcall_drvcall_devhost_fops_epoll_ctl(false, false, fastpath, file_id, ep_arg);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_epoll_ctl(rref_t __rref, unsigned int fastpath, unsigned long long file_id, const void *ep_arg)
{
	return actvxactcapcall_drvcall_devhost_fops_epoll_ctl(false, false, __rref, fastpath, file_id, ep_arg);
}

extern int __actvcall_drvcall_devhost_fops_epoll_remove(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, cref_t ep_cref);


extern int __actvcapcall_drvcall_devhost_fops_epoll_remove(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, cref_t ep_cref);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_epoll_remove(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long long file_id, cref_t ep_cref)
{
	const unsigned long __callno = __devhost_method_fops_epoll_remove;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(cref_t), __arch_actv_rpc_stksz_alignment(cref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_epoll_remove(rpc_info, NULL, fastpath, file_id, ep_cref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_epoll_remove(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long long file_id, cref_t ep_cref)
{
	const unsigned long __callno = __devhost_method_fops_epoll_remove;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(cref_t), __arch_actv_rpc_stksz_alignment(cref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_epoll_remove(__rref, rpc_info, NULL, fastpath, file_id, ep_cref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_epoll_remove(unsigned int fastpath, unsigned long long file_id, cref_t ep_cref)
{
	return actvxactcall_drvcall_devhost_fops_epoll_remove(false, false, fastpath, file_id, ep_cref);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_epoll_remove(rref_t __rref, unsigned int fastpath, unsigned long long file_id, cref_t ep_cref)
{
	return actvxactcapcall_drvcall_devhost_fops_epoll_remove(false, false, __rref, fastpath, file_id, ep_cref);
}

extern int __actvcall_drvcall_devhost_fops_epoll_recheck(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, unsigned int events);


extern int __actvcapcall_drvcall_devhost_fops_epoll_recheck(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, unsigned int events);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_epoll_recheck(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long long file_id, unsigned int events, struct __actvret_drvcall_devhost_fops_epoll_recheck *__ret)
{
	const unsigned long __callno = __devhost_method_fops_epoll_recheck;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_epoll_recheck(rpc_info, __ret, fastpath, file_id, events);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_epoll_recheck(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long long file_id, unsigned int events, struct __actvret_drvcall_devhost_fops_epoll_recheck *__ret)
{
	const unsigned long __callno = __devhost_method_fops_epoll_recheck;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_epoll_recheck(__rref, rpc_info, __ret, fastpath, file_id, events);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_epoll_recheck(unsigned int fastpath, unsigned long long file_id, unsigned int events, struct __actvret_drvcall_devhost_fops_epoll_recheck *__ret)
{
	return actvxactcall_drvcall_devhost_fops_epoll_recheck(false, false, fastpath, file_id, events, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_epoll_recheck(rref_t __rref, unsigned int fastpath, unsigned long long file_id, unsigned int events, struct __actvret_drvcall_devhost_fops_epoll_recheck *__ret)
{
	return actvxactcapcall_drvcall_devhost_fops_epoll_recheck(false, false, __rref, fastpath, file_id, events, __ret);
}

extern int __actvcall_drvcall_devhost_fops_sync_ctrlmem_polllist(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id);


extern int __actvcapcall_drvcall_devhost_fops_sync_ctrlmem_polllist(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_sync_ctrlmem_polllist(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_fops_sync_ctrlmem_polllist;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_sync_ctrlmem_polllist(rpc_info, NULL, fastpath, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_sync_ctrlmem_polllist(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_fops_sync_ctrlmem_polllist;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_sync_ctrlmem_polllist(__rref, rpc_info, NULL, fastpath, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_sync_ctrlmem_polllist(unsigned int fastpath, unsigned long long file_id)
{
	return actvxactcall_drvcall_devhost_fops_sync_ctrlmem_polllist(false, false, fastpath, file_id);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_sync_ctrlmem_polllist(rref_t __rref, unsigned int fastpath, unsigned long long file_id)
{
	return actvxactcapcall_drvcall_devhost_fops_sync_ctrlmem_polllist(false, false, __rref, fastpath, file_id);
}

extern int __actvcall_drvcall_devhost_fops_file_get(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id);


extern int __actvcapcall_drvcall_devhost_fops_file_get(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fops_file_get(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_fops_file_get;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fops_file_get(rpc_info, NULL, fastpath, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fops_file_get(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_fops_file_get;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fops_file_get(__rref, rpc_info, NULL, fastpath, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fops_file_get(unsigned int fastpath, unsigned long long file_id)
{
	return actvxactcall_drvcall_devhost_fops_file_get(false, false, fastpath, file_id);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fops_file_get(rref_t __rref, unsigned int fastpath, unsigned long long file_id)
{
	return actvxactcapcall_drvcall_devhost_fops_file_get(false, false, __rref, fastpath, file_id);
}

extern int __actvcall_drvcall_devhost_block_check_feature(unsigned long long __rpc_info, void *__buf, int idx, unsigned long long file_id, unsigned long long features);


extern int __actvcapcall_drvcall_devhost_block_check_feature(rref_t __rref, unsigned long long __rpc_info, void *__buf, int idx, unsigned long long file_id, unsigned long long features);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_block_check_feature(bool is_xact_fwd, bool is_xact_allow_refwd, int idx, unsigned long long file_id, unsigned long long features, struct __actvret_drvcall_devhost_block_check_feature *__ret)
{
	const unsigned long __callno = __devhost_method_block_check_feature;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_block_check_feature(rpc_info, __ret, idx, file_id, features);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_block_check_feature(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int idx, unsigned long long file_id, unsigned long long features, struct __actvret_drvcall_devhost_block_check_feature *__ret)
{
	const unsigned long __callno = __devhost_method_block_check_feature;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_block_check_feature(__rref, rpc_info, __ret, idx, file_id, features);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_block_check_feature(int idx, unsigned long long file_id, unsigned long long features, struct __actvret_drvcall_devhost_block_check_feature *__ret)
{
	return actvxactcall_drvcall_devhost_block_check_feature(false, false, idx, file_id, features, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_block_check_feature(rref_t __rref, int idx, unsigned long long file_id, unsigned long long features, struct __actvret_drvcall_devhost_block_check_feature *__ret)
{
	return actvxactcapcall_drvcall_devhost_block_check_feature(false, false, __rref, idx, file_id, features, __ret);
}

extern int __actvcall_drvcall_devhost_block_submit_bio(unsigned long long __rpc_info, void *__buf, unsigned long long file_id, unsigned long long rw, unsigned long long sector, int vec_cnt, unsigned int priority, unsigned long long bi_crypt_ctx);


extern int __actvcapcall_drvcall_devhost_block_submit_bio(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long file_id, unsigned long long rw, unsigned long long sector, int vec_cnt, unsigned int priority, unsigned long long bi_crypt_ctx);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_block_submit_bio(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long file_id, unsigned long long rw, unsigned long long sector, int vec_cnt, unsigned int priority, unsigned long long bi_crypt_ctx, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devhost_method_block_submit_bio;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_block_submit_bio(rpc_info, ___void_buf, file_id, rw, sector, vec_cnt, priority, bi_crypt_ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_block_submit_bio(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long file_id, unsigned long long rw, unsigned long long sector, int vec_cnt, unsigned int priority, unsigned long long bi_crypt_ctx, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devhost_method_block_submit_bio;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_block_submit_bio(__rref, rpc_info, ___void_buf, file_id, rw, sector, vec_cnt, priority, bi_crypt_ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_block_submit_bio(unsigned long long file_id, unsigned long long rw, unsigned long long sector, int vec_cnt, unsigned int priority, unsigned long long bi_crypt_ctx, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_drvcall_devhost_block_submit_bio(false, false, file_id, rw, sector, vec_cnt, priority, bi_crypt_ctx, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_block_submit_bio(rref_t __rref, unsigned long long file_id, unsigned long long rw, unsigned long long sector, int vec_cnt, unsigned int priority, unsigned long long bi_crypt_ctx, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_drvcall_devhost_block_submit_bio(false, false, __rref, file_id, rw, sector, vec_cnt, priority, bi_crypt_ctx, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_drvcall_devhost_block_submit_cmd(unsigned long long __rpc_info, void *__buf, unsigned long long file_id, unsigned long long cmd, unsigned long long sector, unsigned int size, unsigned int priority);


extern int __actvcapcall_drvcall_devhost_block_submit_cmd(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long file_id, unsigned long long cmd, unsigned long long sector, unsigned int size, unsigned int priority);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_block_submit_cmd(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long file_id, unsigned long long cmd, unsigned long long sector, unsigned int size, unsigned int priority)
{
	const unsigned long __callno = __devhost_method_block_submit_cmd;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_block_submit_cmd(rpc_info, NULL, file_id, cmd, sector, size, priority);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_block_submit_cmd(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long file_id, unsigned long long cmd, unsigned long long sector, unsigned int size, unsigned int priority)
{
	const unsigned long __callno = __devhost_method_block_submit_cmd;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_block_submit_cmd(__rref, rpc_info, NULL, file_id, cmd, sector, size, priority);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_block_submit_cmd(unsigned long long file_id, unsigned long long cmd, unsigned long long sector, unsigned int size, unsigned int priority)
{
	return actvxactcall_drvcall_devhost_block_submit_cmd(false, false, file_id, cmd, sector, size, priority);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_block_submit_cmd(rref_t __rref, unsigned long long file_id, unsigned long long cmd, unsigned long long sector, unsigned int size, unsigned int priority)
{
	return actvxactcapcall_drvcall_devhost_block_submit_cmd(false, false, __rref, file_id, cmd, sector, size, priority);
}

extern int __actvcall_drvcall_devhost_block_run_queue(unsigned long long __rpc_info, void *__buf, unsigned long file_id, unsigned int flags, unsigned int idx);


extern int __actvcapcall_drvcall_devhost_block_run_queue(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long file_id, unsigned int flags, unsigned int idx);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_block_run_queue(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long file_id, unsigned int flags, unsigned int idx)
{
	const unsigned long __callno = __devhost_method_block_run_queue;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_block_run_queue(rpc_info, NULL, file_id, flags, idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_block_run_queue(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long file_id, unsigned int flags, unsigned int idx)
{
	const unsigned long __callno = __devhost_method_block_run_queue;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_block_run_queue(__rref, rpc_info, NULL, file_id, flags, idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_block_run_queue(unsigned long file_id, unsigned int flags, unsigned int idx)
{
	return actvxactcall_drvcall_devhost_block_run_queue(false, false, file_id, flags, idx);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_block_run_queue(rref_t __rref, unsigned long file_id, unsigned int flags, unsigned int idx)
{
	return actvxactcapcall_drvcall_devhost_block_run_queue(false, false, __rref, file_id, flags, idx);
}

extern int __actvcall_drvcall_devhost_block_direct_access(unsigned long long __rpc_info, void *__buf, unsigned long file_id, unsigned long offset, unsigned long size);


extern int __actvcapcall_drvcall_devhost_block_direct_access(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long file_id, unsigned long offset, unsigned long size);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_block_direct_access(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long file_id, unsigned long offset, unsigned long size, struct __actvret_drvcall_devhost_block_direct_access *__ret)
{
	const unsigned long __callno = __devhost_method_block_direct_access;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_block_direct_access(rpc_info, __ret, file_id, offset, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_block_direct_access(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long file_id, unsigned long offset, unsigned long size, struct __actvret_drvcall_devhost_block_direct_access *__ret)
{
	const unsigned long __callno = __devhost_method_block_direct_access;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_block_direct_access(__rref, rpc_info, __ret, file_id, offset, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_block_direct_access(unsigned long file_id, unsigned long offset, unsigned long size, struct __actvret_drvcall_devhost_block_direct_access *__ret)
{
	return actvxactcall_drvcall_devhost_block_direct_access(false, false, file_id, offset, size, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_block_direct_access(rref_t __rref, unsigned long file_id, unsigned long offset, unsigned long size, struct __actvret_drvcall_devhost_block_direct_access *__ret)
{
	return actvxactcapcall_drvcall_devhost_block_direct_access(false, false, __rref, file_id, offset, size, __ret);
}

extern int __actvcall_drvcall_devhost_block_register_bio_notify(unsigned long long __rpc_info, void *__buf, unsigned long file_id, uref_t uref, unsigned int method, size_t bd_shm_size);


extern int __actvcapcall_drvcall_devhost_block_register_bio_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long file_id, uref_t uref, unsigned int method, size_t bd_shm_size);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_block_register_bio_notify(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long file_id, uref_t uref, unsigned int method, size_t bd_shm_size, struct __actvret_drvcall_devhost_block_register_bio_notify *__ret)
{
	const unsigned long __callno = __devhost_method_block_register_bio_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uref_t), __arch_actv_rpc_stksz_alignment(uref_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_block_register_bio_notify(rpc_info, __ret, file_id, uref, method, bd_shm_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_block_register_bio_notify(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long file_id, uref_t uref, unsigned int method, size_t bd_shm_size, struct __actvret_drvcall_devhost_block_register_bio_notify *__ret)
{
	const unsigned long __callno = __devhost_method_block_register_bio_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uref_t), __arch_actv_rpc_stksz_alignment(uref_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_block_register_bio_notify(__rref, rpc_info, __ret, file_id, uref, method, bd_shm_size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_block_register_bio_notify(unsigned long file_id, uref_t uref, unsigned int method, size_t bd_shm_size, struct __actvret_drvcall_devhost_block_register_bio_notify *__ret)
{
	return actvxactcall_drvcall_devhost_block_register_bio_notify(false, false, file_id, uref, method, bd_shm_size, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_block_register_bio_notify(rref_t __rref, unsigned long file_id, uref_t uref, unsigned int method, size_t bd_shm_size, struct __actvret_drvcall_devhost_block_register_bio_notify *__ret)
{
	return actvxactcapcall_drvcall_devhost_block_register_bio_notify(false, false, __rref, file_id, uref, method, bd_shm_size, __ret);
}

extern int __actvcall_drvcall_devhost_net_send(unsigned long long __rpc_info, void *__buf, int ifindex, unsigned int offset, unsigned int size, unsigned int payload_len, unsigned long flags);


extern int __actvcapcall_drvcall_devhost_net_send(rref_t __rref, unsigned long long __rpc_info, void *__buf, int ifindex, unsigned int offset, unsigned int size, unsigned int payload_len, unsigned long flags);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_send(bool is_xact_fwd, bool is_xact_allow_refwd, int ifindex, unsigned int offset, unsigned int size, unsigned int payload_len, unsigned long flags)
{
	const unsigned long __callno = __devhost_method_net_send;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_send(rpc_info, NULL, ifindex, offset, size, payload_len, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_send(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int ifindex, unsigned int offset, unsigned int size, unsigned int payload_len, unsigned long flags)
{
	const unsigned long __callno = __devhost_method_net_send;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_send(__rref, rpc_info, NULL, ifindex, offset, size, payload_len, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_send(int ifindex, unsigned int offset, unsigned int size, unsigned int payload_len, unsigned long flags)
{
	return actvxactcall_drvcall_devhost_net_send(false, false, ifindex, offset, size, payload_len, flags);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_send(rref_t __rref, int ifindex, unsigned int offset, unsigned int size, unsigned int payload_len, unsigned long flags)
{
	return actvxactcapcall_drvcall_devhost_net_send(false, false, __rref, ifindex, offset, size, payload_len, flags);
}

extern int __actvcall_drvcall_devhost_net_ioctl(unsigned long long __rpc_info, void *__buf, int ifindex, int request, unsigned long args);


extern int __actvcapcall_drvcall_devhost_net_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, int ifindex, int request, unsigned long args);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, int ifindex, int request, unsigned long args)
{
	const unsigned long __callno = __devhost_method_net_ioctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_ioctl(rpc_info, NULL, ifindex, request, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int ifindex, int request, unsigned long args)
{
	const unsigned long __callno = __devhost_method_net_ioctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_ioctl(__rref, rpc_info, NULL, ifindex, request, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_ioctl(int ifindex, int request, unsigned long args)
{
	return actvxactcall_drvcall_devhost_net_ioctl(false, false, ifindex, request, args);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_ioctl(rref_t __rref, int ifindex, int request, unsigned long args)
{
	return actvxactcapcall_drvcall_devhost_net_ioctl(false, false, __rref, ifindex, request, args);
}

extern int __actvcall_drvcall_devhost_net_status(unsigned long long __rpc_info, void *__buf, int ifindex);


extern int __actvcapcall_drvcall_devhost_net_status(rref_t __rref, unsigned long long __rpc_info, void *__buf, int ifindex);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_status(bool is_xact_fwd, bool is_xact_allow_refwd, int ifindex, struct __actvret_drvcall_devhost_net_status *__ret)
{
	const unsigned long __callno = __devhost_method_net_status;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_status(rpc_info, __ret, ifindex);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_status(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int ifindex, struct __actvret_drvcall_devhost_net_status *__ret)
{
	const unsigned long __callno = __devhost_method_net_status;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_status(__rref, rpc_info, __ret, ifindex);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_status(int ifindex, struct __actvret_drvcall_devhost_net_status *__ret)
{
	return actvxactcall_drvcall_devhost_net_status(false, false, ifindex, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_status(rref_t __rref, int ifindex, struct __actvret_drvcall_devhost_net_status *__ret)
{
	return actvxactcapcall_drvcall_devhost_net_status(false, false, __rref, ifindex, __ret);
}

extern int __actvcall_drvcall_devhost_net_prepare(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devhost_net_prepare(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_prepare(bool is_xact_fwd, bool is_xact_allow_refwd, struct __actvret_drvcall_devhost_net_prepare *__ret)
{
	const unsigned long __callno = __devhost_method_net_prepare;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_prepare(rpc_info, __ret);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_prepare(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct __actvret_drvcall_devhost_net_prepare *__ret)
{
	const unsigned long __callno = __devhost_method_net_prepare;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_prepare(__rref, rpc_info, __ret);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_prepare(struct __actvret_drvcall_devhost_net_prepare *__ret)
{
	return actvxactcall_drvcall_devhost_net_prepare(false, false, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_prepare(rref_t __rref, struct __actvret_drvcall_devhost_net_prepare *__ret)
{
	return actvxactcapcall_drvcall_devhost_net_prepare(false, false, __rref, __ret);
}

extern int __actvcall_drvcall_devhost_net_init(unsigned long long __rpc_info, void *__buf, int ifindex, rref_t tgt_rref);


extern int __actvcapcall_drvcall_devhost_net_init(rref_t __rref, unsigned long long __rpc_info, void *__buf, int ifindex, rref_t tgt_rref);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_init(bool is_xact_fwd, bool is_xact_allow_refwd, int ifindex, rref_t tgt_rref)
{
	const unsigned long __callno = __devhost_method_net_init;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(rref_t), __arch_actv_rpc_stksz_alignment(rref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_init(rpc_info, NULL, ifindex, tgt_rref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_init(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int ifindex, rref_t tgt_rref)
{
	const unsigned long __callno = __devhost_method_net_init;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(rref_t), __arch_actv_rpc_stksz_alignment(rref_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_init(__rref, rpc_info, NULL, ifindex, tgt_rref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_init(int ifindex, rref_t tgt_rref)
{
	return actvxactcall_drvcall_devhost_net_init(false, false, ifindex, tgt_rref);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_init(rref_t __rref, int ifindex, rref_t tgt_rref)
{
	return actvxactcapcall_drvcall_devhost_net_init(false, false, __rref, ifindex, tgt_rref);
}

extern int __actvcall_drvcall_devhost_net_netlink_recv(unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned long msg_buff, unsigned int msg_len);


extern int __actvcapcall_drvcall_devhost_net_netlink_recv(rref_t __rref, unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned long msg_buff, unsigned int msg_len);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_netlink_recv(bool is_xact_fwd, bool is_xact_allow_refwd, int protocol, unsigned int src_portid, unsigned long msg_buff, unsigned int msg_len)
{
	const unsigned long __callno = __devhost_method_net_netlink_recv;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_netlink_recv(rpc_info, NULL, protocol, src_portid, msg_buff, msg_len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_netlink_recv(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int protocol, unsigned int src_portid, unsigned long msg_buff, unsigned int msg_len)
{
	const unsigned long __callno = __devhost_method_net_netlink_recv;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_netlink_recv(__rref, rpc_info, NULL, protocol, src_portid, msg_buff, msg_len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_netlink_recv(int protocol, unsigned int src_portid, unsigned long msg_buff, unsigned int msg_len)
{
	return actvxactcall_drvcall_devhost_net_netlink_recv(false, false, protocol, src_portid, msg_buff, msg_len);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_netlink_recv(rref_t __rref, int protocol, unsigned int src_portid, unsigned long msg_buff, unsigned int msg_len)
{
	return actvxactcapcall_drvcall_devhost_net_netlink_recv(false, false, __rref, protocol, src_portid, msg_buff, msg_len);
}

extern int __actvcall_drvcall_devhost_net_socket_register(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devhost_net_socket_register(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_register(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __devhost_method_net_socket_register;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_register(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_register(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __devhost_method_net_socket_register;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_register(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_register(void)
{
	return actvxactcall_drvcall_devhost_net_socket_register(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_register(rref_t __rref)
{
	return actvxactcapcall_drvcall_devhost_net_socket_register(false, false, __rref);
}

extern int __actvcall_drvcall_devhost_net_socket_close(unsigned long long __rpc_info, void *__buf, int sockfd);


extern int __actvcapcall_drvcall_devhost_net_socket_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, int sockfd);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_close(bool is_xact_fwd, bool is_xact_allow_refwd, int sockfd)
{
	const unsigned long __callno = __devhost_method_net_socket_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_close(rpc_info, NULL, sockfd);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_close(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int sockfd)
{
	const unsigned long __callno = __devhost_method_net_socket_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_close(__rref, rpc_info, NULL, sockfd);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_close(int sockfd)
{
	return actvxactcall_drvcall_devhost_net_socket_close(false, false, sockfd);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_close(rref_t __rref, int sockfd)
{
	return actvxactcapcall_drvcall_devhost_net_socket_close(false, false, __rref, sockfd);
}

extern int __actvcall_drvcall_devhost_net_socket_connect(unsigned long long __rpc_info, void *__buf, long sockfd, const struct sockaddr *addr, unsigned int addrlen);


extern int __actvcapcall_drvcall_devhost_net_socket_connect(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const struct sockaddr *addr, unsigned int addrlen);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_connect(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, const struct sockaddr *addr, unsigned int addrlen)
{
	const unsigned long __callno = __devhost_method_net_socket_connect;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const struct sockaddr*), __arch_actv_rpc_stksz_alignment(const struct sockaddr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_connect(rpc_info, NULL, sockfd, addr, addrlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_connect(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, const struct sockaddr *addr, unsigned int addrlen)
{
	const unsigned long __callno = __devhost_method_net_socket_connect;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const struct sockaddr*), __arch_actv_rpc_stksz_alignment(const struct sockaddr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_connect(__rref, rpc_info, NULL, sockfd, addr, addrlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_connect(long sockfd, const struct sockaddr *addr, unsigned int addrlen)
{
	return actvxactcall_drvcall_devhost_net_socket_connect(false, false, sockfd, addr, addrlen);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_connect(rref_t __rref, long sockfd, const struct sockaddr *addr, unsigned int addrlen)
{
	return actvxactcapcall_drvcall_devhost_net_socket_connect(false, false, __rref, sockfd, addr, addrlen);
}

extern int __actvcall_drvcall_devhost_net_socket_bind(unsigned long long __rpc_info, void *__buf, long sockfd, const struct sockaddr *addr, unsigned int addrlen);


extern int __actvcapcall_drvcall_devhost_net_socket_bind(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const struct sockaddr *addr, unsigned int addrlen);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_bind(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, const struct sockaddr *addr, unsigned int addrlen)
{
	const unsigned long __callno = __devhost_method_net_socket_bind;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const struct sockaddr*), __arch_actv_rpc_stksz_alignment(const struct sockaddr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_bind(rpc_info, NULL, sockfd, addr, addrlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_bind(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, const struct sockaddr *addr, unsigned int addrlen)
{
	const unsigned long __callno = __devhost_method_net_socket_bind;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const struct sockaddr*), __arch_actv_rpc_stksz_alignment(const struct sockaddr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_bind(__rref, rpc_info, NULL, sockfd, addr, addrlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_bind(long sockfd, const struct sockaddr *addr, unsigned int addrlen)
{
	return actvxactcall_drvcall_devhost_net_socket_bind(false, false, sockfd, addr, addrlen);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_bind(rref_t __rref, long sockfd, const struct sockaddr *addr, unsigned int addrlen)
{
	return actvxactcapcall_drvcall_devhost_net_socket_bind(false, false, __rref, sockfd, addr, addrlen);
}

extern int __actvcall_drvcall_devhost_net_socket_listen(unsigned long long __rpc_info, void *__buf, long sockfd, int backlog);


extern int __actvcapcall_drvcall_devhost_net_socket_listen(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, int backlog);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_listen(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, int backlog)
{
	const unsigned long __callno = __devhost_method_net_socket_listen;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_listen(rpc_info, NULL, sockfd, backlog);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_listen(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, int backlog)
{
	const unsigned long __callno = __devhost_method_net_socket_listen;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_listen(__rref, rpc_info, NULL, sockfd, backlog);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_listen(long sockfd, int backlog)
{
	return actvxactcall_drvcall_devhost_net_socket_listen(false, false, sockfd, backlog);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_listen(rref_t __rref, long sockfd, int backlog)
{
	return actvxactcapcall_drvcall_devhost_net_socket_listen(false, false, __rref, sockfd, backlog);
}

extern int __actvcall_drvcall_devhost_net_socket_accept(unsigned long long __rpc_info, void *__buf, long sockfd, struct sockaddr *addr, unsigned int *addrlen, int flags, unsigned long ino);


extern int __actvcapcall_drvcall_devhost_net_socket_accept(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, struct sockaddr *addr, unsigned int *addrlen, int flags, unsigned long ino);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_accept(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, struct sockaddr *addr, unsigned int *addrlen, int flags, unsigned long ino, struct __actvret_drvcall_devhost_net_socket_accept *__ret)
{
	const unsigned long __callno = __devhost_method_net_socket_accept;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct sockaddr*), __arch_actv_rpc_stksz_alignment(struct sockaddr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int*), __arch_actv_rpc_stksz_alignment(unsigned int*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_accept(rpc_info, __ret, sockfd, addr, addrlen, flags, ino);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_accept(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, struct sockaddr *addr, unsigned int *addrlen, int flags, unsigned long ino, struct __actvret_drvcall_devhost_net_socket_accept *__ret)
{
	const unsigned long __callno = __devhost_method_net_socket_accept;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct sockaddr*), __arch_actv_rpc_stksz_alignment(struct sockaddr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int*), __arch_actv_rpc_stksz_alignment(unsigned int*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_accept(__rref, rpc_info, __ret, sockfd, addr, addrlen, flags, ino);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_accept(long sockfd, struct sockaddr *addr, unsigned int *addrlen, int flags, unsigned long ino, struct __actvret_drvcall_devhost_net_socket_accept *__ret)
{
	return actvxactcall_drvcall_devhost_net_socket_accept(false, false, sockfd, addr, addrlen, flags, ino, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_accept(rref_t __rref, long sockfd, struct sockaddr *addr, unsigned int *addrlen, int flags, unsigned long ino, struct __actvret_drvcall_devhost_net_socket_accept *__ret)
{
	return actvxactcapcall_drvcall_devhost_net_socket_accept(false, false, __rref, sockfd, addr, addrlen, flags, ino, __ret);
}

extern int __actvcall_drvcall_devhost_net_socket_shutdown(unsigned long long __rpc_info, void *__buf, long sockfd, int how);


extern int __actvcapcall_drvcall_devhost_net_socket_shutdown(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, int how);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_shutdown(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, int how)
{
	const unsigned long __callno = __devhost_method_net_socket_shutdown;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_shutdown(rpc_info, NULL, sockfd, how);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_shutdown(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, int how)
{
	const unsigned long __callno = __devhost_method_net_socket_shutdown;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_shutdown(__rref, rpc_info, NULL, sockfd, how);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_shutdown(long sockfd, int how)
{
	return actvxactcall_drvcall_devhost_net_socket_shutdown(false, false, sockfd, how);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_shutdown(rref_t __rref, long sockfd, int how)
{
	return actvxactcapcall_drvcall_devhost_net_socket_shutdown(false, false, __rref, sockfd, how);
}

extern int __actvcall_drvcall_devhost_net_socket_ioctl(unsigned long long __rpc_info, void *__buf, long sockfd, int cmd, void *arg_buf);


extern int __actvcapcall_drvcall_devhost_net_socket_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, int cmd, void *arg_buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, int cmd, void *arg_buf)
{
	const unsigned long __callno = __devhost_method_net_socket_ioctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_ioctl(rpc_info, NULL, sockfd, cmd, arg_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, int cmd, void *arg_buf)
{
	const unsigned long __callno = __devhost_method_net_socket_ioctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_ioctl(__rref, rpc_info, NULL, sockfd, cmd, arg_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_ioctl(long sockfd, int cmd, void *arg_buf)
{
	return actvxactcall_drvcall_devhost_net_socket_ioctl(false, false, sockfd, cmd, arg_buf);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_ioctl(rref_t __rref, long sockfd, int cmd, void *arg_buf)
{
	return actvxactcapcall_drvcall_devhost_net_socket_ioctl(false, false, __rref, sockfd, cmd, arg_buf);
}

extern int __actvcall_drvcall_devhost_net_socket_dev_ioctl(unsigned long long __rpc_info, void *__buf, int cmd, void *arg_buf);


extern int __actvcapcall_drvcall_devhost_net_socket_dev_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, int cmd, void *arg_buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_dev_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, int cmd, void *arg_buf)
{
	const unsigned long __callno = __devhost_method_net_socket_dev_ioctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_dev_ioctl(rpc_info, NULL, cmd, arg_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_dev_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int cmd, void *arg_buf)
{
	const unsigned long __callno = __devhost_method_net_socket_dev_ioctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_dev_ioctl(__rref, rpc_info, NULL, cmd, arg_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_dev_ioctl(int cmd, void *arg_buf)
{
	return actvxactcall_drvcall_devhost_net_socket_dev_ioctl(false, false, cmd, arg_buf);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_dev_ioctl(rref_t __rref, int cmd, void *arg_buf)
{
	return actvxactcapcall_drvcall_devhost_net_socket_dev_ioctl(false, false, __rref, cmd, arg_buf);
}

extern int __actvcall_drvcall_devhost_net_socket_fcntl(unsigned long long __rpc_info, void *__buf, long sockfd, int cmd, int arg);


extern int __actvcapcall_drvcall_devhost_net_socket_fcntl(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, int cmd, int arg);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_fcntl(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, int cmd, int arg)
{
	const unsigned long __callno = __devhost_method_net_socket_fcntl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_fcntl(rpc_info, NULL, sockfd, cmd, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_fcntl(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, int cmd, int arg)
{
	const unsigned long __callno = __devhost_method_net_socket_fcntl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_fcntl(__rref, rpc_info, NULL, sockfd, cmd, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_fcntl(long sockfd, int cmd, int arg)
{
	return actvxactcall_drvcall_devhost_net_socket_fcntl(false, false, sockfd, cmd, arg);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_fcntl(rref_t __rref, long sockfd, int cmd, int arg)
{
	return actvxactcapcall_drvcall_devhost_net_socket_fcntl(false, false, __rref, sockfd, cmd, arg);
}

extern int __actvcall_drvcall_devhost_net_socket_send(unsigned long long __rpc_info, void *__buf, long sockfd, const void *buf, size_t len, int flags);


extern int __actvcapcall_drvcall_devhost_net_socket_send(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const void *buf, size_t len, int flags);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_send(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, const void *buf, size_t len, int flags)
{
	const unsigned long __callno = __devhost_method_net_socket_send;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_send(rpc_info, NULL, sockfd, buf, len, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_send(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, const void *buf, size_t len, int flags)
{
	const unsigned long __callno = __devhost_method_net_socket_send;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_send(__rref, rpc_info, NULL, sockfd, buf, len, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_send(long sockfd, const void *buf, size_t len, int flags)
{
	return actvxactcall_drvcall_devhost_net_socket_send(false, false, sockfd, buf, len, flags);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_send(rref_t __rref, long sockfd, const void *buf, size_t len, int flags)
{
	return actvxactcapcall_drvcall_devhost_net_socket_send(false, false, __rref, sockfd, buf, len, flags);
}

extern int __actvcall_drvcall_devhost_net_socket_recv(unsigned long long __rpc_info, void *__buf, long sockfd, void *buf, size_t len, int flags);


extern int __actvcapcall_drvcall_devhost_net_socket_recv(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, void *buf, size_t len, int flags);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_recv(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, void *buf, size_t len, int flags)
{
	const unsigned long __callno = __devhost_method_net_socket_recv;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_recv(rpc_info, NULL, sockfd, buf, len, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_recv(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, void *buf, size_t len, int flags)
{
	const unsigned long __callno = __devhost_method_net_socket_recv;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_recv(__rref, rpc_info, NULL, sockfd, buf, len, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_recv(long sockfd, void *buf, size_t len, int flags)
{
	return actvxactcall_drvcall_devhost_net_socket_recv(false, false, sockfd, buf, len, flags);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_recv(rref_t __rref, long sockfd, void *buf, size_t len, int flags)
{
	return actvxactcapcall_drvcall_devhost_net_socket_recv(false, false, __rref, sockfd, buf, len, flags);
}

extern int __actvcall_drvcall_devhost_net_socket_sendmmsg(unsigned long long __rpc_info, void *__buf, long sockfd, const void *msgvec, size_t vlen, int flags);


extern int __actvcapcall_drvcall_devhost_net_socket_sendmmsg(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const void *msgvec, size_t vlen, int flags);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_sendmmsg(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, const void *msgvec, size_t vlen, int flags)
{
	const unsigned long __callno = __devhost_method_net_socket_sendmmsg;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_sendmmsg(rpc_info, NULL, sockfd, msgvec, vlen, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_sendmmsg(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, const void *msgvec, size_t vlen, int flags)
{
	const unsigned long __callno = __devhost_method_net_socket_sendmmsg;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_sendmmsg(__rref, rpc_info, NULL, sockfd, msgvec, vlen, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_sendmmsg(long sockfd, const void *msgvec, size_t vlen, int flags)
{
	return actvxactcall_drvcall_devhost_net_socket_sendmmsg(false, false, sockfd, msgvec, vlen, flags);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_sendmmsg(rref_t __rref, long sockfd, const void *msgvec, size_t vlen, int flags)
{
	return actvxactcapcall_drvcall_devhost_net_socket_sendmmsg(false, false, __rref, sockfd, msgvec, vlen, flags);
}

extern int __actvcall_drvcall_devhost_net_socket_recvmmsg(unsigned long long __rpc_info, void *__buf, long sockfd, const void *msgvec, size_t vlen, int flags, void *timeout);


extern int __actvcapcall_drvcall_devhost_net_socket_recvmmsg(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const void *msgvec, size_t vlen, int flags, void *timeout);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_recvmmsg(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, const void *msgvec, size_t vlen, int flags, void *timeout)
{
	const unsigned long __callno = __devhost_method_net_socket_recvmmsg;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_recvmmsg(rpc_info, NULL, sockfd, msgvec, vlen, flags, timeout);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_recvmmsg(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, const void *msgvec, size_t vlen, int flags, void *timeout)
{
	const unsigned long __callno = __devhost_method_net_socket_recvmmsg;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_recvmmsg(__rref, rpc_info, NULL, sockfd, msgvec, vlen, flags, timeout);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_recvmmsg(long sockfd, const void *msgvec, size_t vlen, int flags, void *timeout)
{
	return actvxactcall_drvcall_devhost_net_socket_recvmmsg(false, false, sockfd, msgvec, vlen, flags, timeout);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_recvmmsg(rref_t __rref, long sockfd, const void *msgvec, size_t vlen, int flags, void *timeout)
{
	return actvxactcapcall_drvcall_devhost_net_socket_recvmmsg(false, false, __rref, sockfd, msgvec, vlen, flags, timeout);
}

extern int __actvcall_drvcall_devhost_net_socket_sendmsg(unsigned long long __rpc_info, void *__buf, long sockfd, const struct msghdr *msg, int flags);


extern int __actvcapcall_drvcall_devhost_net_socket_sendmsg(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const struct msghdr *msg, int flags);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_sendmsg(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, const struct msghdr *msg, int flags)
{
	const unsigned long __callno = __devhost_method_net_socket_sendmsg;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const struct msghdr*), __arch_actv_rpc_stksz_alignment(const struct msghdr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_sendmsg(rpc_info, NULL, sockfd, msg, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_sendmsg(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, const struct msghdr *msg, int flags)
{
	const unsigned long __callno = __devhost_method_net_socket_sendmsg;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const struct msghdr*), __arch_actv_rpc_stksz_alignment(const struct msghdr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_sendmsg(__rref, rpc_info, NULL, sockfd, msg, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_sendmsg(long sockfd, const struct msghdr *msg, int flags)
{
	return actvxactcall_drvcall_devhost_net_socket_sendmsg(false, false, sockfd, msg, flags);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_sendmsg(rref_t __rref, long sockfd, const struct msghdr *msg, int flags)
{
	return actvxactcapcall_drvcall_devhost_net_socket_sendmsg(false, false, __rref, sockfd, msg, flags);
}

extern int __actvcall_drvcall_devhost_net_socket_recvmsg(unsigned long long __rpc_info, void *__buf, long sockfd, struct msghdr *msg, int flags);


extern int __actvcapcall_drvcall_devhost_net_socket_recvmsg(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, struct msghdr *msg, int flags);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_recvmsg(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, struct msghdr *msg, int flags)
{
	const unsigned long __callno = __devhost_method_net_socket_recvmsg;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct msghdr*), __arch_actv_rpc_stksz_alignment(struct msghdr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_recvmsg(rpc_info, NULL, sockfd, msg, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_recvmsg(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, struct msghdr *msg, int flags)
{
	const unsigned long __callno = __devhost_method_net_socket_recvmsg;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct msghdr*), __arch_actv_rpc_stksz_alignment(struct msghdr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_recvmsg(__rref, rpc_info, NULL, sockfd, msg, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_recvmsg(long sockfd, struct msghdr *msg, int flags)
{
	return actvxactcall_drvcall_devhost_net_socket_recvmsg(false, false, sockfd, msg, flags);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_recvmsg(rref_t __rref, long sockfd, struct msghdr *msg, int flags)
{
	return actvxactcapcall_drvcall_devhost_net_socket_recvmsg(false, false, __rref, sockfd, msg, flags);
}

extern int __actvcall_drvcall_devhost_net_socket_sendto(unsigned long long __rpc_info, void *__buf, long sockfd, const void *buf, size_t len, int flags, const struct sockaddr *daddr, unsigned int addrlen);


extern int __actvcapcall_drvcall_devhost_net_socket_sendto(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const void *buf, size_t len, int flags, const struct sockaddr *daddr, unsigned int addrlen);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_sendto(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, const void *buf, size_t len, int flags, const struct sockaddr *daddr, unsigned int addrlen)
{
	const unsigned long __callno = __devhost_method_net_socket_sendto;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const struct sockaddr*), __arch_actv_rpc_stksz_alignment(const struct sockaddr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_sendto(rpc_info, NULL, sockfd, buf, len, flags, daddr, addrlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_sendto(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, const void *buf, size_t len, int flags, const struct sockaddr *daddr, unsigned int addrlen)
{
	const unsigned long __callno = __devhost_method_net_socket_sendto;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const struct sockaddr*), __arch_actv_rpc_stksz_alignment(const struct sockaddr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_sendto(__rref, rpc_info, NULL, sockfd, buf, len, flags, daddr, addrlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_sendto(long sockfd, const void *buf, size_t len, int flags, const struct sockaddr *daddr, unsigned int addrlen)
{
	return actvxactcall_drvcall_devhost_net_socket_sendto(false, false, sockfd, buf, len, flags, daddr, addrlen);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_sendto(rref_t __rref, long sockfd, const void *buf, size_t len, int flags, const struct sockaddr *daddr, unsigned int addrlen)
{
	return actvxactcapcall_drvcall_devhost_net_socket_sendto(false, false, __rref, sockfd, buf, len, flags, daddr, addrlen);
}

extern int __actvcall_drvcall_devhost_net_socket_recvfrom(unsigned long long __rpc_info, void *__buf, long sockfd, void *buf, size_t len, int flags, struct sockaddr *daddr, unsigned int *addrlen);


extern int __actvcapcall_drvcall_devhost_net_socket_recvfrom(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, void *buf, size_t len, int flags, struct sockaddr *daddr, unsigned int *addrlen);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_recvfrom(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, void *buf, size_t len, int flags, struct sockaddr *daddr, unsigned int *addrlen)
{
	const unsigned long __callno = __devhost_method_net_socket_recvfrom;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct sockaddr*), __arch_actv_rpc_stksz_alignment(struct sockaddr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int*), __arch_actv_rpc_stksz_alignment(unsigned int*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_recvfrom(rpc_info, NULL, sockfd, buf, len, flags, daddr, addrlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_recvfrom(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, void *buf, size_t len, int flags, struct sockaddr *daddr, unsigned int *addrlen)
{
	const unsigned long __callno = __devhost_method_net_socket_recvfrom;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct sockaddr*), __arch_actv_rpc_stksz_alignment(struct sockaddr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int*), __arch_actv_rpc_stksz_alignment(unsigned int*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_recvfrom(__rref, rpc_info, NULL, sockfd, buf, len, flags, daddr, addrlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_recvfrom(long sockfd, void *buf, size_t len, int flags, struct sockaddr *daddr, unsigned int *addrlen)
{
	return actvxactcall_drvcall_devhost_net_socket_recvfrom(false, false, sockfd, buf, len, flags, daddr, addrlen);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_recvfrom(rref_t __rref, long sockfd, void *buf, size_t len, int flags, struct sockaddr *daddr, unsigned int *addrlen)
{
	return actvxactcapcall_drvcall_devhost_net_socket_recvfrom(false, false, __rref, sockfd, buf, len, flags, daddr, addrlen);
}

extern int __actvcall_drvcall_devhost_net_socket_readv(unsigned long long __rpc_info, void *__buf, long sockfd, const struct iovec *iov, int iovcnt);


extern int __actvcapcall_drvcall_devhost_net_socket_readv(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const struct iovec *iov, int iovcnt);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_readv(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, const struct iovec *iov, int iovcnt)
{
	const unsigned long __callno = __devhost_method_net_socket_readv;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const struct iovec*), __arch_actv_rpc_stksz_alignment(const struct iovec*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_readv(rpc_info, NULL, sockfd, iov, iovcnt);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_readv(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, const struct iovec *iov, int iovcnt)
{
	const unsigned long __callno = __devhost_method_net_socket_readv;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const struct iovec*), __arch_actv_rpc_stksz_alignment(const struct iovec*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_readv(__rref, rpc_info, NULL, sockfd, iov, iovcnt);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_readv(long sockfd, const struct iovec *iov, int iovcnt)
{
	return actvxactcall_drvcall_devhost_net_socket_readv(false, false, sockfd, iov, iovcnt);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_readv(rref_t __rref, long sockfd, const struct iovec *iov, int iovcnt)
{
	return actvxactcapcall_drvcall_devhost_net_socket_readv(false, false, __rref, sockfd, iov, iovcnt);
}

extern int __actvcall_drvcall_devhost_net_socket_writev(unsigned long long __rpc_info, void *__buf, long sockfd, const struct iovec *iov, int iovcnt, int flags);


extern int __actvcapcall_drvcall_devhost_net_socket_writev(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const struct iovec *iov, int iovcnt, int flags);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_writev(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, const struct iovec *iov, int iovcnt, int flags)
{
	const unsigned long __callno = __devhost_method_net_socket_writev;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const struct iovec*), __arch_actv_rpc_stksz_alignment(const struct iovec*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_writev(rpc_info, NULL, sockfd, iov, iovcnt, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_writev(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, const struct iovec *iov, int iovcnt, int flags)
{
	const unsigned long __callno = __devhost_method_net_socket_writev;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const struct iovec*), __arch_actv_rpc_stksz_alignment(const struct iovec*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_writev(__rref, rpc_info, NULL, sockfd, iov, iovcnt, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_writev(long sockfd, const struct iovec *iov, int iovcnt, int flags)
{
	return actvxactcall_drvcall_devhost_net_socket_writev(false, false, sockfd, iov, iovcnt, flags);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_writev(rref_t __rref, long sockfd, const struct iovec *iov, int iovcnt, int flags)
{
	return actvxactcapcall_drvcall_devhost_net_socket_writev(false, false, __rref, sockfd, iov, iovcnt, flags);
}

extern int __actvcall_drvcall_devhost_net_socket_getsockopt(unsigned long long __rpc_info, void *__buf, long sockfd, int level, int optname, void *optval, unsigned int *optlen);


extern int __actvcapcall_drvcall_devhost_net_socket_getsockopt(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, int level, int optname, void *optval, unsigned int *optlen);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_getsockopt(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, int level, int optname, void *optval, unsigned int *optlen)
{
	const unsigned long __callno = __devhost_method_net_socket_getsockopt;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int*), __arch_actv_rpc_stksz_alignment(unsigned int*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_getsockopt(rpc_info, NULL, sockfd, level, optname, optval, optlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_getsockopt(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, int level, int optname, void *optval, unsigned int *optlen)
{
	const unsigned long __callno = __devhost_method_net_socket_getsockopt;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int*), __arch_actv_rpc_stksz_alignment(unsigned int*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_getsockopt(__rref, rpc_info, NULL, sockfd, level, optname, optval, optlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_getsockopt(long sockfd, int level, int optname, void *optval, unsigned int *optlen)
{
	return actvxactcall_drvcall_devhost_net_socket_getsockopt(false, false, sockfd, level, optname, optval, optlen);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_getsockopt(rref_t __rref, long sockfd, int level, int optname, void *optval, unsigned int *optlen)
{
	return actvxactcapcall_drvcall_devhost_net_socket_getsockopt(false, false, __rref, sockfd, level, optname, optval, optlen);
}

extern int __actvcall_drvcall_devhost_net_socket_setsockopt(unsigned long long __rpc_info, void *__buf, long sockfd, int level, int optname, const void *optval, unsigned int optlen);


extern int __actvcapcall_drvcall_devhost_net_socket_setsockopt(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, int level, int optname, const void *optval, unsigned int optlen);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_setsockopt(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, int level, int optname, const void *optval, unsigned int optlen)
{
	const unsigned long __callno = __devhost_method_net_socket_setsockopt;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_setsockopt(rpc_info, NULL, sockfd, level, optname, optval, optlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_setsockopt(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, int level, int optname, const void *optval, unsigned int optlen)
{
	const unsigned long __callno = __devhost_method_net_socket_setsockopt;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_setsockopt(__rref, rpc_info, NULL, sockfd, level, optname, optval, optlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_setsockopt(long sockfd, int level, int optname, const void *optval, unsigned int optlen)
{
	return actvxactcall_drvcall_devhost_net_socket_setsockopt(false, false, sockfd, level, optname, optval, optlen);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_setsockopt(rref_t __rref, long sockfd, int level, int optname, const void *optval, unsigned int optlen)
{
	return actvxactcapcall_drvcall_devhost_net_socket_setsockopt(false, false, __rref, sockfd, level, optname, optval, optlen);
}

extern int __actvcall_drvcall_devhost_net_socket_getsockname(unsigned long long __rpc_info, void *__buf, long sockfd, struct sockaddr *addr, unsigned int *addrlen);


extern int __actvcapcall_drvcall_devhost_net_socket_getsockname(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, struct sockaddr *addr, unsigned int *addrlen);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_getsockname(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, struct sockaddr *addr, unsigned int *addrlen)
{
	const unsigned long __callno = __devhost_method_net_socket_getsockname;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct sockaddr*), __arch_actv_rpc_stksz_alignment(struct sockaddr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int*), __arch_actv_rpc_stksz_alignment(unsigned int*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_getsockname(rpc_info, NULL, sockfd, addr, addrlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_getsockname(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, struct sockaddr *addr, unsigned int *addrlen)
{
	const unsigned long __callno = __devhost_method_net_socket_getsockname;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct sockaddr*), __arch_actv_rpc_stksz_alignment(struct sockaddr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int*), __arch_actv_rpc_stksz_alignment(unsigned int*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_getsockname(__rref, rpc_info, NULL, sockfd, addr, addrlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_getsockname(long sockfd, struct sockaddr *addr, unsigned int *addrlen)
{
	return actvxactcall_drvcall_devhost_net_socket_getsockname(false, false, sockfd, addr, addrlen);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_getsockname(rref_t __rref, long sockfd, struct sockaddr *addr, unsigned int *addrlen)
{
	return actvxactcapcall_drvcall_devhost_net_socket_getsockname(false, false, __rref, sockfd, addr, addrlen);
}

extern int __actvcall_drvcall_devhost_net_socket_getpeername(unsigned long long __rpc_info, void *__buf, long sockfd, struct sockaddr *addr, unsigned int *addrlen);


extern int __actvcapcall_drvcall_devhost_net_socket_getpeername(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, struct sockaddr *addr, unsigned int *addrlen);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_getpeername(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, struct sockaddr *addr, unsigned int *addrlen)
{
	const unsigned long __callno = __devhost_method_net_socket_getpeername;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct sockaddr*), __arch_actv_rpc_stksz_alignment(struct sockaddr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int*), __arch_actv_rpc_stksz_alignment(unsigned int*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_getpeername(rpc_info, NULL, sockfd, addr, addrlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_getpeername(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, struct sockaddr *addr, unsigned int *addrlen)
{
	const unsigned long __callno = __devhost_method_net_socket_getpeername;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct sockaddr*), __arch_actv_rpc_stksz_alignment(struct sockaddr*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int*), __arch_actv_rpc_stksz_alignment(unsigned int*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_getpeername(__rref, rpc_info, NULL, sockfd, addr, addrlen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_getpeername(long sockfd, struct sockaddr *addr, unsigned int *addrlen)
{
	return actvxactcall_drvcall_devhost_net_socket_getpeername(false, false, sockfd, addr, addrlen);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_getpeername(rref_t __rref, long sockfd, struct sockaddr *addr, unsigned int *addrlen)
{
	return actvxactcapcall_drvcall_devhost_net_socket_getpeername(false, false, __rref, sockfd, addr, addrlen);
}

extern int __actvcall_drvcall_devhost_net_socket_fchown(unsigned long long __rpc_info, void *__buf, long sockfd, unsigned int uid, unsigned int gid);


extern int __actvcapcall_drvcall_devhost_net_socket_fchown(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, unsigned int uid, unsigned int gid);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_fchown(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, unsigned int uid, unsigned int gid)
{
	const unsigned long __callno = __devhost_method_net_socket_fchown;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_fchown(rpc_info, NULL, sockfd, uid, gid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_fchown(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, unsigned int uid, unsigned int gid)
{
	const unsigned long __callno = __devhost_method_net_socket_fchown;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_fchown(__rref, rpc_info, NULL, sockfd, uid, gid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_fchown(long sockfd, unsigned int uid, unsigned int gid)
{
	return actvxactcall_drvcall_devhost_net_socket_fchown(false, false, sockfd, uid, gid);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_fchown(rref_t __rref, long sockfd, unsigned int uid, unsigned int gid)
{
	return actvxactcapcall_drvcall_devhost_net_socket_fchown(false, false, __rref, sockfd, uid, gid);
}

extern int __actvcall_drvcall_devhost_net_socket_fstat(unsigned long long __rpc_info, void *__buf, long sockfd, uid_t *uid);


extern int __actvcapcall_drvcall_devhost_net_socket_fstat(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, uid_t *uid);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_fstat(bool is_xact_fwd, bool is_xact_allow_refwd, long sockfd, uid_t *uid)
{
	const unsigned long __callno = __devhost_method_net_socket_fstat;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uid_t*), __arch_actv_rpc_stksz_alignment(uid_t*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_fstat(rpc_info, NULL, sockfd, uid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_fstat(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long sockfd, uid_t *uid)
{
	const unsigned long __callno = __devhost_method_net_socket_fstat;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uid_t*), __arch_actv_rpc_stksz_alignment(uid_t*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_fstat(__rref, rpc_info, NULL, sockfd, uid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_fstat(long sockfd, uid_t *uid)
{
	return actvxactcall_drvcall_devhost_net_socket_fstat(false, false, sockfd, uid);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_fstat(rref_t __rref, long sockfd, uid_t *uid)
{
	return actvxactcapcall_drvcall_devhost_net_socket_fstat(false, false, __rref, sockfd, uid);
}

extern int __actvcall_drvcall_devhost_net_socket_socket(unsigned long long __rpc_info, void *__buf, int domain, int type, int protocol, unsigned long ino);


extern int __actvcapcall_drvcall_devhost_net_socket_socket(rref_t __rref, unsigned long long __rpc_info, void *__buf, int domain, int type, int protocol, unsigned long ino);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_socket(bool is_xact_fwd, bool is_xact_allow_refwd, int domain, int type, int protocol, unsigned long ino, struct __actvret_drvcall_devhost_net_socket_socket *__ret)
{
	const unsigned long __callno = __devhost_method_net_socket_socket;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_socket(rpc_info, __ret, domain, type, protocol, ino);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_socket(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int domain, int type, int protocol, unsigned long ino, struct __actvret_drvcall_devhost_net_socket_socket *__ret)
{
	const unsigned long __callno = __devhost_method_net_socket_socket;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_socket(__rref, rpc_info, __ret, domain, type, protocol, ino);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_socket(int domain, int type, int protocol, unsigned long ino, struct __actvret_drvcall_devhost_net_socket_socket *__ret)
{
	return actvxactcall_drvcall_devhost_net_socket_socket(false, false, domain, type, protocol, ino, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_socket(rref_t __rref, int domain, int type, int protocol, unsigned long ino, struct __actvret_drvcall_devhost_net_socket_socket *__ret)
{
	return actvxactcapcall_drvcall_devhost_net_socket_socket(false, false, __rref, domain, type, protocol, ino, __ret);
}

extern int __actvcall_drvcall_devhost_net_socket_alloc_new_ns(unsigned long long __rpc_info, void *__buf, int nsid);


extern int __actvcapcall_drvcall_devhost_net_socket_alloc_new_ns(rref_t __rref, unsigned long long __rpc_info, void *__buf, int nsid);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_alloc_new_ns(bool is_xact_fwd, bool is_xact_allow_refwd, int nsid)
{
	const unsigned long __callno = __devhost_method_net_socket_alloc_new_ns;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_alloc_new_ns(rpc_info, NULL, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_alloc_new_ns(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int nsid)
{
	const unsigned long __callno = __devhost_method_net_socket_alloc_new_ns;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_alloc_new_ns(__rref, rpc_info, NULL, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_alloc_new_ns(int nsid)
{
	return actvxactcall_drvcall_devhost_net_socket_alloc_new_ns(false, false, nsid);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_alloc_new_ns(rref_t __rref, int nsid)
{
	return actvxactcapcall_drvcall_devhost_net_socket_alloc_new_ns(false, false, __rref, nsid);
}

extern int __actvcall_drvcall_devhost_net_socket_destroy_ns(unsigned long long __rpc_info, void *__buf, int nsid);


extern int __actvcapcall_drvcall_devhost_net_socket_destroy_ns(rref_t __rref, unsigned long long __rpc_info, void *__buf, int nsid);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_destroy_ns(bool is_xact_fwd, bool is_xact_allow_refwd, int nsid)
{
	const unsigned long __callno = __devhost_method_net_socket_destroy_ns;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_destroy_ns(rpc_info, NULL, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_destroy_ns(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int nsid)
{
	const unsigned long __callno = __devhost_method_net_socket_destroy_ns;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_destroy_ns(__rref, rpc_info, NULL, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_destroy_ns(int nsid)
{
	return actvxactcall_drvcall_devhost_net_socket_destroy_ns(false, false, nsid);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_destroy_ns(rref_t __rref, int nsid)
{
	return actvxactcapcall_drvcall_devhost_net_socket_destroy_ns(false, false, __rref, nsid);
}

extern int __actvcall_drvcall_devhost_net_socket_bpf(unsigned long long __rpc_info, void *__buf, int cmd, void *attr, unsigned int size);


extern int __actvcapcall_drvcall_devhost_net_socket_bpf(rref_t __rref, unsigned long long __rpc_info, void *__buf, int cmd, void *attr, unsigned int size);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_net_socket_bpf(bool is_xact_fwd, bool is_xact_allow_refwd, int cmd, void *attr, unsigned int size)
{
	const unsigned long __callno = __devhost_method_net_socket_bpf;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_net_socket_bpf(rpc_info, NULL, cmd, attr, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_net_socket_bpf(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int cmd, void *attr, unsigned int size)
{
	const unsigned long __callno = __devhost_method_net_socket_bpf;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_net_socket_bpf(__rref, rpc_info, NULL, cmd, attr, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_net_socket_bpf(int cmd, void *attr, unsigned int size)
{
	return actvxactcall_drvcall_devhost_net_socket_bpf(false, false, cmd, attr, size);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_net_socket_bpf(rref_t __rref, int cmd, void *attr, unsigned int size)
{
	return actvxactcapcall_drvcall_devhost_net_socket_bpf(false, false, __rref, cmd, attr, size);
}

extern int __actvcall_drvcall_devhost_iaware_notify_thread_msg(unsigned long long __rpc_info, void *__buf, struct evtcb_thread_msg_s msg);


extern int __actvcapcall_drvcall_devhost_iaware_notify_thread_msg(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct evtcb_thread_msg_s msg);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_iaware_notify_thread_msg(bool is_xact_fwd, bool is_xact_allow_refwd, struct evtcb_thread_msg_s msg)
{
	const unsigned long __callno = __devhost_method_iaware_notify_thread_msg;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct evtcb_thread_msg_s), __arch_actv_rpc_stksz_alignment(struct evtcb_thread_msg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_iaware_notify_thread_msg(rpc_info, NULL, msg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_iaware_notify_thread_msg(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct evtcb_thread_msg_s msg)
{
	const unsigned long __callno = __devhost_method_iaware_notify_thread_msg;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct evtcb_thread_msg_s), __arch_actv_rpc_stksz_alignment(struct evtcb_thread_msg_s));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_iaware_notify_thread_msg(__rref, rpc_info, NULL, msg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_iaware_notify_thread_msg(struct evtcb_thread_msg_s msg)
{
	return actvxactcall_drvcall_devhost_iaware_notify_thread_msg(false, false, msg);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_iaware_notify_thread_msg(rref_t __rref, struct evtcb_thread_msg_s msg)
{
	return actvxactcapcall_drvcall_devhost_iaware_notify_thread_msg(false, false, __rref, msg);
}

extern int __actvcall_drvcall_devhost_kstate_binderinfo_notify(unsigned long long __rpc_info, void *__buf, int calling_pid, int called_pid);


extern int __actvcapcall_drvcall_devhost_kstate_binderinfo_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, int calling_pid, int called_pid);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_kstate_binderinfo_notify(bool is_xact_fwd, bool is_xact_allow_refwd, int calling_pid, int called_pid)
{
	const unsigned long __callno = __devhost_method_kstate_binderinfo_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_kstate_binderinfo_notify(rpc_info, NULL, calling_pid, called_pid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_kstate_binderinfo_notify(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int calling_pid, int called_pid)
{
	const unsigned long __callno = __devhost_method_kstate_binderinfo_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_kstate_binderinfo_notify(__rref, rpc_info, NULL, calling_pid, called_pid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_kstate_binderinfo_notify(int calling_pid, int called_pid)
{
	return actvxactcall_drvcall_devhost_kstate_binderinfo_notify(false, false, calling_pid, called_pid);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_kstate_binderinfo_notify(rref_t __rref, int calling_pid, int called_pid)
{
	return actvxactcapcall_drvcall_devhost_kstate_binderinfo_notify(false, false, __rref, calling_pid, called_pid);
}

extern int __actvcall_drvcall_devhost_kstate_killinfo_notify(unsigned long long __rpc_info, void *__buf, int pid, int seg);


extern int __actvcapcall_drvcall_devhost_kstate_killinfo_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, int pid, int seg);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_kstate_killinfo_notify(bool is_xact_fwd, bool is_xact_allow_refwd, int pid, int seg)
{
	const unsigned long __callno = __devhost_method_kstate_killinfo_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_kstate_killinfo_notify(rpc_info, NULL, pid, seg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_kstate_killinfo_notify(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int pid, int seg)
{
	const unsigned long __callno = __devhost_method_kstate_killinfo_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_kstate_killinfo_notify(__rref, rpc_info, NULL, pid, seg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_kstate_killinfo_notify(int pid, int seg)
{
	return actvxactcall_drvcall_devhost_kstate_killinfo_notify(false, false, pid, seg);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_kstate_killinfo_notify(rref_t __rref, int pid, int seg)
{
	return actvxactcapcall_drvcall_devhost_kstate_killinfo_notify(false, false, __rref, pid, seg);
}

extern int __actvcall_drvcall_devhost_kstate_freeze_binder_info_notify(unsigned long long __rpc_info, void *__buf, struct kstate_binder_info *info);


extern int __actvcapcall_drvcall_devhost_kstate_freeze_binder_info_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct kstate_binder_info *info);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_kstate_freeze_binder_info_notify(bool is_xact_fwd, bool is_xact_allow_refwd, struct kstate_binder_info *info)
{
	const unsigned long __callno = __devhost_method_kstate_freeze_binder_info_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct kstate_binder_info*), __arch_actv_rpc_stksz_alignment(struct kstate_binder_info*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_kstate_freeze_binder_info_notify(rpc_info, NULL, info);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_kstate_freeze_binder_info_notify(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct kstate_binder_info *info)
{
	const unsigned long __callno = __devhost_method_kstate_freeze_binder_info_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct kstate_binder_info*), __arch_actv_rpc_stksz_alignment(struct kstate_binder_info*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_kstate_freeze_binder_info_notify(__rref, rpc_info, NULL, info);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_kstate_freeze_binder_info_notify(struct kstate_binder_info *info)
{
	return actvxactcall_drvcall_devhost_kstate_freeze_binder_info_notify(false, false, info);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_kstate_freeze_binder_info_notify(rref_t __rref, struct kstate_binder_info *info)
{
	return actvxactcapcall_drvcall_devhost_kstate_freeze_binder_info_notify(false, false, __rref, info);
}

extern int __actvcall_drvcall_devhost_kstate_request_anco_thaw(unsigned long long __rpc_info, void *__buf, char *reason, unsigned int size);


extern int __actvcapcall_drvcall_devhost_kstate_request_anco_thaw(rref_t __rref, unsigned long long __rpc_info, void *__buf, char *reason, unsigned int size);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_kstate_request_anco_thaw(bool is_xact_fwd, bool is_xact_allow_refwd, char *reason, unsigned int size)
{
	const unsigned long __callno = __devhost_method_kstate_request_anco_thaw;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_kstate_request_anco_thaw(rpc_info, NULL, reason, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_kstate_request_anco_thaw(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, char *reason, unsigned int size)
{
	const unsigned long __callno = __devhost_method_kstate_request_anco_thaw;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_kstate_request_anco_thaw(__rref, rpc_info, NULL, reason, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_kstate_request_anco_thaw(char *reason, unsigned int size)
{
	return actvxactcall_drvcall_devhost_kstate_request_anco_thaw(false, false, reason, size);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_kstate_request_anco_thaw(rref_t __rref, char *reason, unsigned int size)
{
	return actvxactcapcall_drvcall_devhost_kstate_request_anco_thaw(false, false, __rref, reason, size);
}

extern int __actvcall_drvcall_devhost_pm_action(unsigned long long __rpc_info, void *__buf, unsigned int action);


extern int __actvcapcall_drvcall_devhost_pm_action(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int action);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_pm_action(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int action)
{
	const unsigned long __callno = __devhost_method_pm_action;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_pm_action(rpc_info, NULL, action);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_pm_action(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int action)
{
	const unsigned long __callno = __devhost_method_pm_action;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_pm_action(__rref, rpc_info, NULL, action);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_pm_action(unsigned int action)
{
	return actvxactcall_drvcall_devhost_pm_action(false, false, action);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_pm_action(rref_t __rref, unsigned int action)
{
	return actvxactcapcall_drvcall_devhost_pm_action(false, false, __rref, action);
}

extern int __actvcall_drvcall_devhost_pm_dubai_hwlog(unsigned long long __rpc_info, void *__buf, struct pm_dubai_ctrl_log *log_info);


extern int __actvcapcall_drvcall_devhost_pm_dubai_hwlog(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct pm_dubai_ctrl_log *log_info);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_pm_dubai_hwlog(bool is_xact_fwd, bool is_xact_allow_refwd, struct pm_dubai_ctrl_log *log_info)
{
	const unsigned long __callno = __devhost_method_pm_dubai_hwlog;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct pm_dubai_ctrl_log*), __arch_actv_rpc_stksz_alignment(struct pm_dubai_ctrl_log*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_pm_dubai_hwlog(rpc_info, NULL, log_info);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_pm_dubai_hwlog(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct pm_dubai_ctrl_log *log_info)
{
	const unsigned long __callno = __devhost_method_pm_dubai_hwlog;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct pm_dubai_ctrl_log*), __arch_actv_rpc_stksz_alignment(struct pm_dubai_ctrl_log*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_pm_dubai_hwlog(__rref, rpc_info, NULL, log_info);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_pm_dubai_hwlog(struct pm_dubai_ctrl_log *log_info)
{
	return actvxactcall_drvcall_devhost_pm_dubai_hwlog(false, false, log_info);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_pm_dubai_hwlog(rref_t __rref, struct pm_dubai_ctrl_log *log_info)
{
	return actvxactcapcall_drvcall_devhost_pm_dubai_hwlog(false, false, __rref, log_info);
}

extern int __actvcall_drvcall_devhost_pm_prepare(unsigned long long __rpc_info, void *__buf, unsigned int type, struct reboot_cmd cmd);


extern int __actvcapcall_drvcall_devhost_pm_prepare(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int type, struct reboot_cmd cmd);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_pm_prepare(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int type, struct reboot_cmd cmd)
{
	const unsigned long __callno = __devhost_method_pm_prepare;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_cmd), __arch_actv_rpc_stksz_alignment(struct reboot_cmd));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_pm_prepare(rpc_info, NULL, type, cmd);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_pm_prepare(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int type, struct reboot_cmd cmd)
{
	const unsigned long __callno = __devhost_method_pm_prepare;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_cmd), __arch_actv_rpc_stksz_alignment(struct reboot_cmd));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_pm_prepare(__rref, rpc_info, NULL, type, cmd);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_pm_prepare(unsigned int type, struct reboot_cmd cmd)
{
	return actvxactcall_drvcall_devhost_pm_prepare(false, false, type, cmd);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_pm_prepare(rref_t __rref, unsigned int type, struct reboot_cmd cmd)
{
	return actvxactcapcall_drvcall_devhost_pm_prepare(false, false, __rref, type, cmd);
}

extern int __actvcall_drvcall_devhost_pm_power_off(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devhost_pm_power_off(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_pm_power_off(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __devhost_method_pm_power_off;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_pm_power_off(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_pm_power_off(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __devhost_method_pm_power_off;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_pm_power_off(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_pm_power_off(void)
{
	return actvxactcall_drvcall_devhost_pm_power_off(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_pm_power_off(rref_t __rref)
{
	return actvxactcapcall_drvcall_devhost_pm_power_off(false, false, __rref);
}

extern int __actvcall_drvcall_devhost_pm_reboot(unsigned long long __rpc_info, void *__buf, struct reboot_cmd cmd);


extern int __actvcapcall_drvcall_devhost_pm_reboot(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct reboot_cmd cmd);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_pm_reboot(bool is_xact_fwd, bool is_xact_allow_refwd, struct reboot_cmd cmd)
{
	const unsigned long __callno = __devhost_method_pm_reboot;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_cmd), __arch_actv_rpc_stksz_alignment(struct reboot_cmd));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_pm_reboot(rpc_info, NULL, cmd);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_pm_reboot(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct reboot_cmd cmd)
{
	const unsigned long __callno = __devhost_method_pm_reboot;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct reboot_cmd), __arch_actv_rpc_stksz_alignment(struct reboot_cmd));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_pm_reboot(__rref, rpc_info, NULL, cmd);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_pm_reboot(struct reboot_cmd cmd)
{
	return actvxactcall_drvcall_devhost_pm_reboot(false, false, cmd);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_pm_reboot(rref_t __rref, struct reboot_cmd cmd)
{
	return actvxactcapcall_drvcall_devhost_pm_reboot(false, false, __rref, cmd);
}

extern int __actvcall_drvcall_devhost_pm_cpuhp_action(unsigned long long __rpc_info, void *__buf, unsigned int cpu, unsigned int action);


extern int __actvcapcall_drvcall_devhost_pm_cpuhp_action(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cpu, unsigned int action);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_pm_cpuhp_action(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cpu, unsigned int action)
{
	const unsigned long __callno = __devhost_method_pm_cpuhp_action;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_pm_cpuhp_action(rpc_info, NULL, cpu, action);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_pm_cpuhp_action(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cpu, unsigned int action)
{
	const unsigned long __callno = __devhost_method_pm_cpuhp_action;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_pm_cpuhp_action(__rref, rpc_info, NULL, cpu, action);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_pm_cpuhp_action(unsigned int cpu, unsigned int action)
{
	return actvxactcall_drvcall_devhost_pm_cpuhp_action(false, false, cpu, action);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_pm_cpuhp_action(rref_t __rref, unsigned int cpu, unsigned int action)
{
	return actvxactcapcall_drvcall_devhost_pm_cpuhp_action(false, false, __rref, cpu, action);
}

extern int __actvcall_drvcall_devhost_pm_wl_stat(unsigned long long __rpc_info, void *__buf, struct wl_stat_queue_info queue_info, size_t cnt);


extern int __actvcapcall_drvcall_devhost_pm_wl_stat(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct wl_stat_queue_info queue_info, size_t cnt);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_pm_wl_stat(bool is_xact_fwd, bool is_xact_allow_refwd, struct wl_stat_queue_info queue_info, size_t cnt, struct __actvret_drvcall_devhost_pm_wl_stat *__ret)
{
	const unsigned long __callno = __devhost_method_pm_wl_stat;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wl_stat_queue_info), __arch_actv_rpc_stksz_alignment(struct wl_stat_queue_info));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_pm_wl_stat(rpc_info, __ret, queue_info, cnt);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_pm_wl_stat(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct wl_stat_queue_info queue_info, size_t cnt, struct __actvret_drvcall_devhost_pm_wl_stat *__ret)
{
	const unsigned long __callno = __devhost_method_pm_wl_stat;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct wl_stat_queue_info), __arch_actv_rpc_stksz_alignment(struct wl_stat_queue_info));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_pm_wl_stat(__rref, rpc_info, __ret, queue_info, cnt);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_pm_wl_stat(struct wl_stat_queue_info queue_info, size_t cnt, struct __actvret_drvcall_devhost_pm_wl_stat *__ret)
{
	return actvxactcall_drvcall_devhost_pm_wl_stat(false, false, queue_info, cnt, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_pm_wl_stat(rref_t __rref, struct wl_stat_queue_info queue_info, size_t cnt, struct __actvret_drvcall_devhost_pm_wl_stat *__ret)
{
	return actvxactcapcall_drvcall_devhost_pm_wl_stat(false, false, __rref, queue_info, cnt, __ret);
}

extern int __actvcall_drvcall_devhost_procfs_open(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf buf);


extern int __actvcapcall_drvcall_devhost_procfs_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_procfs_open(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf buf, struct __actvret_drvcall_devhost_procfs_open *__ret)
{
	const unsigned long __callno = __devhost_method_procfs_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_procfs_open(rpc_info, __ret, buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_procfs_open(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf buf, struct __actvret_drvcall_devhost_procfs_open *__ret)
{
	const unsigned long __callno = __devhost_method_procfs_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_procfs_open(__rref, rpc_info, __ret, buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_procfs_open(struct hm_actv_buf buf, struct __actvret_drvcall_devhost_procfs_open *__ret)
{
	return actvxactcall_drvcall_devhost_procfs_open(false, false, buf, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_procfs_open(rref_t __rref, struct hm_actv_buf buf, struct __actvret_drvcall_devhost_procfs_open *__ret)
{
	return actvxactcapcall_drvcall_devhost_procfs_open(false, false, __rref, buf, __ret);
}

extern int __actvcall_drvcall_devhost_procfs_close(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf buf);


extern int __actvcapcall_drvcall_devhost_procfs_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_procfs_close(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf buf)
{
	const unsigned long __callno = __devhost_method_procfs_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_procfs_close(rpc_info, NULL, buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_procfs_close(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf buf)
{
	const unsigned long __callno = __devhost_method_procfs_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_procfs_close(__rref, rpc_info, NULL, buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_procfs_close(struct hm_actv_buf buf)
{
	return actvxactcall_drvcall_devhost_procfs_close(false, false, buf);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_procfs_close(rref_t __rref, struct hm_actv_buf buf)
{
	return actvxactcapcall_drvcall_devhost_procfs_close(false, false, __rref, buf);
}

extern int __actvcall_drvcall_devhost_procfs_read(unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx);


extern int __actvcapcall_drvcall_devhost_procfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_procfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_procfs_read *__ret)
{
	const unsigned long __callno = __devhost_method_procfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_procfs_read(rpc_info, __ret, tagid, fd, pos, ubuf, ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_procfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_procfs_read *__ret)
{
	const unsigned long __callno = __devhost_method_procfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_procfs_read(__rref, rpc_info, __ret, tagid, fd, pos, ubuf, ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_procfs_read(unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_procfs_read *__ret)
{
	return actvxactcall_drvcall_devhost_procfs_read(false, false, tagid, fd, pos, ubuf, ctx, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_procfs_read(rref_t __rref, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_procfs_read *__ret)
{
	return actvxactcapcall_drvcall_devhost_procfs_read(false, false, __rref, tagid, fd, pos, ubuf, ctx, __ret);
}

extern int __actvcall_drvcall_devhost_procfs_write(unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx);


extern int __actvcapcall_drvcall_devhost_procfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_procfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_procfs_write *__ret)
{
	const unsigned long __callno = __devhost_method_procfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_procfs_write(rpc_info, __ret, tagid, fd, pos, ubuf, ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_procfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_procfs_write *__ret)
{
	const unsigned long __callno = __devhost_method_procfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_procfs_write(__rref, rpc_info, __ret, tagid, fd, pos, ubuf, ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_procfs_write(unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_procfs_write *__ret)
{
	return actvxactcall_drvcall_devhost_procfs_write(false, false, tagid, fd, pos, ubuf, ctx, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_procfs_write(rref_t __rref, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_procfs_write *__ret)
{
	return actvxactcapcall_drvcall_devhost_procfs_write(false, false, __rref, tagid, fd, pos, ubuf, ctx, __ret);
}

extern int __actvcall_drvcall_devhost_sysfs_open(unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned int flags, unsigned int mode, uintptr_t ctx);


extern int __actvcapcall_drvcall_devhost_sysfs_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned int flags, unsigned int mode, uintptr_t ctx);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_sysfs_open(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int tagid, unsigned int flags, unsigned int mode, uintptr_t ctx, struct __actvret_drvcall_devhost_sysfs_open *__ret)
{
	const unsigned long __callno = __devhost_method_sysfs_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_sysfs_open(rpc_info, __ret, tagid, flags, mode, ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_sysfs_open(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int tagid, unsigned int flags, unsigned int mode, uintptr_t ctx, struct __actvret_drvcall_devhost_sysfs_open *__ret)
{
	const unsigned long __callno = __devhost_method_sysfs_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_sysfs_open(__rref, rpc_info, __ret, tagid, flags, mode, ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_sysfs_open(unsigned int tagid, unsigned int flags, unsigned int mode, uintptr_t ctx, struct __actvret_drvcall_devhost_sysfs_open *__ret)
{
	return actvxactcall_drvcall_devhost_sysfs_open(false, false, tagid, flags, mode, ctx, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_sysfs_open(rref_t __rref, unsigned int tagid, unsigned int flags, unsigned int mode, uintptr_t ctx, struct __actvret_drvcall_devhost_sysfs_open *__ret)
{
	return actvxactcapcall_drvcall_devhost_sysfs_open(false, false, __rref, tagid, flags, mode, ctx, __ret);
}

extern int __actvcall_drvcall_devhost_sysfs_close(unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, uintptr_t ctx);


extern int __actvcapcall_drvcall_devhost_sysfs_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, uintptr_t ctx);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_sysfs_close(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int tagid, unsigned long long fd, uintptr_t ctx)
{
	const unsigned long __callno = __devhost_method_sysfs_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_sysfs_close(rpc_info, NULL, tagid, fd, ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_sysfs_close(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int tagid, unsigned long long fd, uintptr_t ctx)
{
	const unsigned long __callno = __devhost_method_sysfs_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_sysfs_close(__rref, rpc_info, NULL, tagid, fd, ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_sysfs_close(unsigned int tagid, unsigned long long fd, uintptr_t ctx)
{
	return actvxactcall_drvcall_devhost_sysfs_close(false, false, tagid, fd, ctx);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_sysfs_close(rref_t __rref, unsigned int tagid, unsigned long long fd, uintptr_t ctx)
{
	return actvxactcapcall_drvcall_devhost_sysfs_close(false, false, __rref, tagid, fd, ctx);
}

extern int __actvcall_drvcall_devhost_sysfs_read(unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx);


extern int __actvcapcall_drvcall_devhost_sysfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_sysfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_sysfs_read *__ret)
{
	const unsigned long __callno = __devhost_method_sysfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_sysfs_read(rpc_info, __ret, tagid, fd, pos, ubuf, ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_sysfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_sysfs_read *__ret)
{
	const unsigned long __callno = __devhost_method_sysfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_sysfs_read(__rref, rpc_info, __ret, tagid, fd, pos, ubuf, ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_sysfs_read(unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_sysfs_read *__ret)
{
	return actvxactcall_drvcall_devhost_sysfs_read(false, false, tagid, fd, pos, ubuf, ctx, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_sysfs_read(rref_t __rref, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_sysfs_read *__ret)
{
	return actvxactcapcall_drvcall_devhost_sysfs_read(false, false, __rref, tagid, fd, pos, ubuf, ctx, __ret);
}

extern int __actvcall_drvcall_devhost_sysfs_write(unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx);


extern int __actvcapcall_drvcall_devhost_sysfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_sysfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_sysfs_write *__ret)
{
	const unsigned long __callno = __devhost_method_sysfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_sysfs_write(rpc_info, __ret, tagid, fd, pos, ubuf, ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_sysfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_sysfs_write *__ret)
{
	const unsigned long __callno = __devhost_method_sysfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_sysfs_write(__rref, rpc_info, __ret, tagid, fd, pos, ubuf, ctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_sysfs_write(unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_sysfs_write *__ret)
{
	return actvxactcall_drvcall_devhost_sysfs_write(false, false, tagid, fd, pos, ubuf, ctx, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_sysfs_write(rref_t __rref, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx, struct __actvret_drvcall_devhost_sysfs_write *__ret)
{
	return actvxactcapcall_drvcall_devhost_sysfs_write(false, false, __rref, tagid, fd, pos, ubuf, ctx, __ret);
}

extern int __actvcall_drvcall_devhost_sysfs_poll(unsigned long long __rpc_info, void *__buf, unsigned int tagid, uintptr_t ctx, bool pollable);


extern int __actvcapcall_drvcall_devhost_sysfs_poll(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tagid, uintptr_t ctx, bool pollable);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_sysfs_poll(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int tagid, uintptr_t ctx, bool pollable)
{
	const unsigned long __callno = __devhost_method_sysfs_poll;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_sysfs_poll(rpc_info, NULL, tagid, ctx, pollable);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_sysfs_poll(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int tagid, uintptr_t ctx, bool pollable)
{
	const unsigned long __callno = __devhost_method_sysfs_poll;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_sysfs_poll(__rref, rpc_info, NULL, tagid, ctx, pollable);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_sysfs_poll(unsigned int tagid, uintptr_t ctx, bool pollable)
{
	return actvxactcall_drvcall_devhost_sysfs_poll(false, false, tagid, ctx, pollable);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_sysfs_poll(rref_t __rref, unsigned int tagid, uintptr_t ctx, bool pollable)
{
	return actvxactcapcall_drvcall_devhost_sysfs_poll(false, false, __rref, tagid, ctx, pollable);
}

extern int __actvcall_drvcall_devhost_clock_settime(unsigned long long __rpc_info, void *__buf, unsigned long tp, unsigned long long file_id);


extern int __actvcapcall_drvcall_devhost_clock_settime(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long tp, unsigned long long file_id);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_clock_settime(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long tp, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_clock_settime;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_clock_settime(rpc_info, NULL, tp, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_clock_settime(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long tp, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_clock_settime;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_clock_settime(__rref, rpc_info, NULL, tp, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_clock_settime(unsigned long tp, unsigned long long file_id)
{
	return actvxactcall_drvcall_devhost_clock_settime(false, false, tp, file_id);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_clock_settime(rref_t __rref, unsigned long tp, unsigned long long file_id)
{
	return actvxactcapcall_drvcall_devhost_clock_settime(false, false, __rref, tp, file_id);
}

extern int __actvcall_drvcall_devhost_clock_gettime(unsigned long long __rpc_info, void *__buf, unsigned long tp, unsigned long long file_id);


extern int __actvcapcall_drvcall_devhost_clock_gettime(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long tp, unsigned long long file_id);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_clock_gettime(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long tp, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_clock_gettime;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_clock_gettime(rpc_info, NULL, tp, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_clock_gettime(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long tp, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_clock_gettime;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_clock_gettime(__rref, rpc_info, NULL, tp, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_clock_gettime(unsigned long tp, unsigned long long file_id)
{
	return actvxactcall_drvcall_devhost_clock_gettime(false, false, tp, file_id);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_clock_gettime(rref_t __rref, unsigned long tp, unsigned long long file_id)
{
	return actvxactcapcall_drvcall_devhost_clock_gettime(false, false, __rref, tp, file_id);
}

extern int __actvcall_drvcall_devhost_clock_getres(unsigned long long __rpc_info, void *__buf, unsigned long tp, unsigned long long file_id);


extern int __actvcapcall_drvcall_devhost_clock_getres(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long tp, unsigned long long file_id);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_clock_getres(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long tp, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_clock_getres;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_clock_getres(rpc_info, NULL, tp, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_clock_getres(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long tp, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_clock_getres;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_clock_getres(__rref, rpc_info, NULL, tp, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_clock_getres(unsigned long tp, unsigned long long file_id)
{
	return actvxactcall_drvcall_devhost_clock_getres(false, false, tp, file_id);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_clock_getres(rref_t __rref, unsigned long tp, unsigned long long file_id)
{
	return actvxactcapcall_drvcall_devhost_clock_getres(false, false, __rref, tp, file_id);
}

extern int __actvcall_drvcall_devhost_clock_adjtime(unsigned long long __rpc_info, void *__buf, unsigned long tx, unsigned long long file_id);


extern int __actvcapcall_drvcall_devhost_clock_adjtime(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long tx, unsigned long long file_id);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_clock_adjtime(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long tx, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_clock_adjtime;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_clock_adjtime(rpc_info, NULL, tx, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_clock_adjtime(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long tx, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_clock_adjtime;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_clock_adjtime(__rref, rpc_info, NULL, tx, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_clock_adjtime(unsigned long tx, unsigned long long file_id)
{
	return actvxactcall_drvcall_devhost_clock_adjtime(false, false, tx, file_id);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_clock_adjtime(rref_t __rref, unsigned long tx, unsigned long long file_id)
{
	return actvxactcapcall_drvcall_devhost_clock_adjtime(false, false, __rref, tx, file_id);
}

extern int __actvcall_drvcall_devhost_clock_poweroff_alarm(unsigned long long __rpc_info, void *__buf, long tm);


extern int __actvcapcall_drvcall_devhost_clock_poweroff_alarm(rref_t __rref, unsigned long long __rpc_info, void *__buf, long tm);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_clock_poweroff_alarm(bool is_xact_fwd, bool is_xact_allow_refwd, long tm)
{
	const unsigned long __callno = __devhost_method_clock_poweroff_alarm;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_clock_poweroff_alarm(rpc_info, NULL, tm);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_clock_poweroff_alarm(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, long tm)
{
	const unsigned long __callno = __devhost_method_clock_poweroff_alarm;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long), __arch_actv_rpc_stksz_alignment(long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_clock_poweroff_alarm(__rref, rpc_info, NULL, tm);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_clock_poweroff_alarm(long tm)
{
	return actvxactcall_drvcall_devhost_clock_poweroff_alarm(false, false, tm);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_clock_poweroff_alarm(rref_t __rref, long tm)
{
	return actvxactcapcall_drvcall_devhost_clock_poweroff_alarm(false, false, __rref, tm);
}

extern int __actvcall_drvcall_devhost_transfs_mounted(unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, struct hm_actv_buf fs_name, struct hm_actv_buf dev_name, struct hm_actv_buf mount_opts);


extern int __actvcapcall_drvcall_devhost_transfs_mounted(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, struct hm_actv_buf fs_name, struct hm_actv_buf dev_name, struct hm_actv_buf mount_opts);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_mounted(bool is_xact_fwd, bool is_xact_allow_refwd, struct hm_actv_buf fs_name, struct hm_actv_buf dev_name, struct hm_actv_buf mount_opts, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_transfs_mounted *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_mounted;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 1);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_mounted(rpc_info, ___void_buf, ptr_to_void(__ret), sizeof(*__ret), fs_name, dev_name, mount_opts);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_mounted(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct hm_actv_buf fs_name, struct hm_actv_buf dev_name, struct hm_actv_buf mount_opts, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_transfs_mounted *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_mounted;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 1);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_mounted(__rref, rpc_info, ___void_buf, ptr_to_void(__ret), sizeof(*__ret), fs_name, dev_name, mount_opts);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_mounted(struct hm_actv_buf fs_name, struct hm_actv_buf dev_name, struct hm_actv_buf mount_opts, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_transfs_mounted *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_mounted(false, false, fs_name, dev_name, mount_opts, ___void_buf, ___void_buf_sz, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_mounted(rref_t __rref, struct hm_actv_buf fs_name, struct hm_actv_buf dev_name, struct hm_actv_buf mount_opts, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_drvcall_devhost_transfs_mounted *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_mounted(false, false, __rref, fs_name, dev_name, mount_opts, ___void_buf, ___void_buf_sz, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_unmounted(unsigned long long __rpc_info, void *__buf, unsigned int s_index, int flags, unsigned long long file_id);


extern int __actvcapcall_drvcall_devhost_transfs_unmounted(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, int flags, unsigned long long file_id);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_unmounted(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, int flags, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_transfs_unmounted;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_unmounted(rpc_info, NULL, s_index, flags, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_unmounted(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, int flags, unsigned long long file_id)
{
	const unsigned long __callno = __devhost_method_transfs_unmounted;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_unmounted(__rref, rpc_info, NULL, s_index, flags, file_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_unmounted(unsigned int s_index, int flags, unsigned long long file_id)
{
	return actvxactcall_drvcall_devhost_transfs_unmounted(false, false, s_index, flags, file_id);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_unmounted(rref_t __rref, unsigned int s_index, int flags, unsigned long long file_id)
{
	return actvxactcapcall_drvcall_devhost_transfs_unmounted(false, false, __rref, s_index, flags, file_id);
}

extern int __actvcall_drvcall_devhost_transfs_drop_caches(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int mode, int max_nr);


extern int __actvcapcall_drvcall_devhost_transfs_drop_caches(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int mode, int max_nr);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_drop_caches(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int mode, int max_nr)
{
	const unsigned long __callno = __devhost_method_transfs_drop_caches;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_drop_caches(rpc_info, NULL, s_index, mode, max_nr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_drop_caches(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int mode, int max_nr)
{
	const unsigned long __callno = __devhost_method_transfs_drop_caches;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_drop_caches(__rref, rpc_info, NULL, s_index, mode, max_nr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_drop_caches(unsigned int s_index, unsigned int mode, int max_nr)
{
	return actvxactcall_drvcall_devhost_transfs_drop_caches(false, false, s_index, mode, max_nr);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_drop_caches(rref_t __rref, unsigned int s_index, unsigned int mode, int max_nr)
{
	return actvxactcapcall_drvcall_devhost_transfs_drop_caches(false, false, __rref, s_index, mode, max_nr);
}

extern int __actvcall_drvcall_devhost_transfs_mkdir(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname);


extern int __actvcapcall_drvcall_devhost_transfs_mkdir(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_mkdir(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_mkdir *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_mkdir;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_mkdir(rpc_info, __ret, s_index, pd_index, mode, actv_dname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_mkdir(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_mkdir *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_mkdir;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_mkdir(__rref, rpc_info, __ret, s_index, pd_index, mode, actv_dname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_mkdir(unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_mkdir *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_mkdir(false, false, s_index, pd_index, mode, actv_dname, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_mkdir(rref_t __rref, unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_mkdir *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_mkdir(false, false, __rref, s_index, pd_index, mode, actv_dname, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_rmdir(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, unsigned int d_index);


extern int __actvcapcall_drvcall_devhost_transfs_rmdir(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, unsigned int d_index);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_rmdir(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int pd_index, unsigned int d_index)
{
	const unsigned long __callno = __devhost_method_transfs_rmdir;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_rmdir(rpc_info, NULL, s_index, pd_index, d_index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_rmdir(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int pd_index, unsigned int d_index)
{
	const unsigned long __callno = __devhost_method_transfs_rmdir;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_rmdir(__rref, rpc_info, NULL, s_index, pd_index, d_index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_rmdir(unsigned int s_index, unsigned int pd_index, unsigned int d_index)
{
	return actvxactcall_drvcall_devhost_transfs_rmdir(false, false, s_index, pd_index, d_index);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_rmdir(rref_t __rref, unsigned int s_index, unsigned int pd_index, unsigned int d_index)
{
	return actvxactcapcall_drvcall_devhost_transfs_rmdir(false, false, __rref, s_index, pd_index, d_index);
}

extern int __actvcall_drvcall_devhost_transfs_create(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname);


extern int __actvcapcall_drvcall_devhost_transfs_create(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_create(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_create *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_create;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_create(rpc_info, __ret, s_index, pd_index, mode, actv_dname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_create(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_create *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_create;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_create(__rref, rpc_info, __ret, s_index, pd_index, mode, actv_dname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_create(unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_create *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_create(false, false, s_index, pd_index, mode, actv_dname, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_create(rref_t __rref, unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_create *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_create(false, false, __rref, s_index, pd_index, mode, actv_dname, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_unlink(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, unsigned int d_index);


extern int __actvcapcall_drvcall_devhost_transfs_unlink(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, unsigned int d_index);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_unlink(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int pd_index, unsigned int d_index)
{
	const unsigned long __callno = __devhost_method_transfs_unlink;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_unlink(rpc_info, NULL, s_index, pd_index, d_index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_unlink(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int pd_index, unsigned int d_index)
{
	const unsigned long __callno = __devhost_method_transfs_unlink;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_unlink(__rref, rpc_info, NULL, s_index, pd_index, d_index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_unlink(unsigned int s_index, unsigned int pd_index, unsigned int d_index)
{
	return actvxactcall_drvcall_devhost_transfs_unlink(false, false, s_index, pd_index, d_index);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_unlink(rref_t __rref, unsigned int s_index, unsigned int pd_index, unsigned int d_index)
{
	return actvxactcapcall_drvcall_devhost_transfs_unlink(false, false, __rref, s_index, pd_index, d_index);
}

extern int __actvcall_drvcall_devhost_transfs_mknod(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, unsigned int mode, unsigned long long devno, struct hm_actv_buf actv_dname);


extern int __actvcapcall_drvcall_devhost_transfs_mknod(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, unsigned int mode, unsigned long long devno, struct hm_actv_buf actv_dname);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_mknod(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int pd_index, unsigned int mode, unsigned long long devno, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_mknod *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_mknod;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_mknod(rpc_info, __ret, s_index, pd_index, mode, devno, actv_dname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_mknod(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int pd_index, unsigned int mode, unsigned long long devno, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_mknod *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_mknod;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_mknod(__rref, rpc_info, __ret, s_index, pd_index, mode, devno, actv_dname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_mknod(unsigned int s_index, unsigned int pd_index, unsigned int mode, unsigned long long devno, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_mknod *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_mknod(false, false, s_index, pd_index, mode, devno, actv_dname, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_mknod(rref_t __rref, unsigned int s_index, unsigned int pd_index, unsigned int mode, unsigned long long devno, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_mknod *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_mknod(false, false, __rref, s_index, pd_index, mode, devno, actv_dname, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_link(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int old_d_index, unsigned int new_pd_index, struct hm_actv_buf actv_dname);


extern int __actvcapcall_drvcall_devhost_transfs_link(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int old_d_index, unsigned int new_pd_index, struct hm_actv_buf actv_dname);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_link(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int old_d_index, unsigned int new_pd_index, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_link *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_link;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_link(rpc_info, __ret, s_index, old_d_index, new_pd_index, actv_dname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_link(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int old_d_index, unsigned int new_pd_index, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_link *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_link;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_link(__rref, rpc_info, __ret, s_index, old_d_index, new_pd_index, actv_dname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_link(unsigned int s_index, unsigned int old_d_index, unsigned int new_pd_index, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_link *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_link(false, false, s_index, old_d_index, new_pd_index, actv_dname, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_link(rref_t __rref, unsigned int s_index, unsigned int old_d_index, unsigned int new_pd_index, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_link *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_link(false, false, __rref, s_index, old_d_index, new_pd_index, actv_dname, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_symlink(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, struct hm_actv_buf actv_dname, struct hm_actv_buf actv_path);


extern int __actvcapcall_drvcall_devhost_transfs_symlink(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, struct hm_actv_buf actv_dname, struct hm_actv_buf actv_path);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_symlink(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int pd_index, struct hm_actv_buf actv_dname, struct hm_actv_buf actv_path, struct __actvret_drvcall_devhost_transfs_symlink *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_symlink;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_symlink(rpc_info, __ret, s_index, pd_index, actv_dname, actv_path);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_symlink(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int pd_index, struct hm_actv_buf actv_dname, struct hm_actv_buf actv_path, struct __actvret_drvcall_devhost_transfs_symlink *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_symlink;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_symlink(__rref, rpc_info, __ret, s_index, pd_index, actv_dname, actv_path);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_symlink(unsigned int s_index, unsigned int pd_index, struct hm_actv_buf actv_dname, struct hm_actv_buf actv_path, struct __actvret_drvcall_devhost_transfs_symlink *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_symlink(false, false, s_index, pd_index, actv_dname, actv_path, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_symlink(rref_t __rref, unsigned int s_index, unsigned int pd_index, struct hm_actv_buf actv_dname, struct hm_actv_buf actv_path, struct __actvret_drvcall_devhost_transfs_symlink *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_symlink(false, false, __rref, s_index, pd_index, actv_dname, actv_path, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_statfs(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, void *buf);


extern int __actvcapcall_drvcall_devhost_transfs_statfs(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, void *buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_statfs(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int d_index, void *buf)
{
	const unsigned long __callno = __devhost_method_transfs_statfs;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_statfs(rpc_info, NULL, s_index, d_index, buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_statfs(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int d_index, void *buf)
{
	const unsigned long __callno = __devhost_method_transfs_statfs;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_statfs(__rref, rpc_info, NULL, s_index, d_index, buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_statfs(unsigned int s_index, unsigned int d_index, void *buf)
{
	return actvxactcall_drvcall_devhost_transfs_statfs(false, false, s_index, d_index, buf);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_statfs(rref_t __rref, unsigned int s_index, unsigned int d_index, void *buf)
{
	return actvxactcapcall_drvcall_devhost_transfs_statfs(false, false, __rref, s_index, d_index, buf);
}

extern int __actvcall_drvcall_devhost_transfs_rename(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int old_pd_index, unsigned int old_d_index, unsigned int new_pd_index, unsigned int new_d_index, struct hm_actv_buf actv_dname);


extern int __actvcapcall_drvcall_devhost_transfs_rename(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int old_pd_index, unsigned int old_d_index, unsigned int new_pd_index, unsigned int new_d_index, struct hm_actv_buf actv_dname);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_rename(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int sb_index, unsigned int old_pd_index, unsigned int old_d_index, unsigned int new_pd_index, unsigned int new_d_index, struct hm_actv_buf actv_dname)
{
	const unsigned long __callno = __devhost_method_transfs_rename;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_rename(rpc_info, NULL, sb_index, old_pd_index, old_d_index, new_pd_index, new_d_index, actv_dname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_rename(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int sb_index, unsigned int old_pd_index, unsigned int old_d_index, unsigned int new_pd_index, unsigned int new_d_index, struct hm_actv_buf actv_dname)
{
	const unsigned long __callno = __devhost_method_transfs_rename;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_rename(__rref, rpc_info, NULL, sb_index, old_pd_index, old_d_index, new_pd_index, new_d_index, actv_dname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_rename(unsigned int sb_index, unsigned int old_pd_index, unsigned int old_d_index, unsigned int new_pd_index, unsigned int new_d_index, struct hm_actv_buf actv_dname)
{
	return actvxactcall_drvcall_devhost_transfs_rename(false, false, sb_index, old_pd_index, old_d_index, new_pd_index, new_d_index, actv_dname);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_rename(rref_t __rref, unsigned int sb_index, unsigned int old_pd_index, unsigned int old_d_index, unsigned int new_pd_index, unsigned int new_d_index, struct hm_actv_buf actv_dname)
{
	return actvxactcapcall_drvcall_devhost_transfs_rename(false, false, __rref, sb_index, old_pd_index, old_d_index, new_pd_index, new_d_index, actv_dname);
}

extern int __actvcall_drvcall_devhost_transfs_lookup(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int pd_index, struct hm_actv_buf actv_dname, void *meta);


extern int __actvcapcall_drvcall_devhost_transfs_lookup(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int pd_index, struct hm_actv_buf actv_dname, void *meta);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_lookup(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int sb_index, unsigned int pd_index, struct hm_actv_buf actv_dname, void *meta, struct __actvret_drvcall_devhost_transfs_lookup *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_lookup;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_lookup(rpc_info, __ret, sb_index, pd_index, actv_dname, meta);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_lookup(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int sb_index, unsigned int pd_index, struct hm_actv_buf actv_dname, void *meta, struct __actvret_drvcall_devhost_transfs_lookup *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_lookup;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_lookup(__rref, rpc_info, __ret, sb_index, pd_index, actv_dname, meta);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_lookup(unsigned int sb_index, unsigned int pd_index, struct hm_actv_buf actv_dname, void *meta, struct __actvret_drvcall_devhost_transfs_lookup *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_lookup(false, false, sb_index, pd_index, actv_dname, meta, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_lookup(rref_t __rref, unsigned int sb_index, unsigned int pd_index, struct hm_actv_buf actv_dname, void *meta, struct __actvret_drvcall_devhost_transfs_lookup *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_lookup(false, false, __rref, sb_index, pd_index, actv_dname, meta, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_setattr(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, const void *attr, uint64_t size);


extern int __actvcapcall_drvcall_devhost_transfs_setattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, const void *attr, uint64_t size);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_setattr(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int sb_index, unsigned int d_index, const void *attr, uint64_t size)
{
	const unsigned long __callno = __devhost_method_transfs_setattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_setattr(rpc_info, NULL, sb_index, d_index, attr, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_setattr(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int sb_index, unsigned int d_index, const void *attr, uint64_t size)
{
	const unsigned long __callno = __devhost_method_transfs_setattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_setattr(__rref, rpc_info, NULL, sb_index, d_index, attr, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_setattr(unsigned int sb_index, unsigned int d_index, const void *attr, uint64_t size)
{
	return actvxactcall_drvcall_devhost_transfs_setattr(false, false, sb_index, d_index, attr, size);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_setattr(rref_t __rref, unsigned int sb_index, unsigned int d_index, const void *attr, uint64_t size)
{
	return actvxactcapcall_drvcall_devhost_transfs_setattr(false, false, __rref, sb_index, d_index, attr, size);
}

extern int __actvcall_drvcall_devhost_transfs_readlink(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, char *buf, size_t bufsize);


extern int __actvcapcall_drvcall_devhost_transfs_readlink(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, char *buf, size_t bufsize);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_readlink(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int sb_index, unsigned int d_index, char *buf, size_t bufsize)
{
	const unsigned long __callno = __devhost_method_transfs_readlink;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_readlink(rpc_info, NULL, sb_index, d_index, buf, bufsize);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_readlink(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int sb_index, unsigned int d_index, char *buf, size_t bufsize)
{
	const unsigned long __callno = __devhost_method_transfs_readlink;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_readlink(__rref, rpc_info, NULL, sb_index, d_index, buf, bufsize);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_readlink(unsigned int sb_index, unsigned int d_index, char *buf, size_t bufsize)
{
	return actvxactcall_drvcall_devhost_transfs_readlink(false, false, sb_index, d_index, buf, bufsize);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_readlink(rref_t __rref, unsigned int sb_index, unsigned int d_index, char *buf, size_t bufsize)
{
	return actvxactcapcall_drvcall_devhost_transfs_readlink(false, false, __rref, sb_index, d_index, buf, bufsize);
}

extern int __actvcall_drvcall_devhost_transfs_getattr(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, void *meta);


extern int __actvcapcall_drvcall_devhost_transfs_getattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, void *meta);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_getattr(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int sb_index, unsigned int d_index, void *meta)
{
	const unsigned long __callno = __devhost_method_transfs_getattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_getattr(rpc_info, NULL, sb_index, d_index, meta);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_getattr(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int sb_index, unsigned int d_index, void *meta)
{
	const unsigned long __callno = __devhost_method_transfs_getattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_getattr(__rref, rpc_info, NULL, sb_index, d_index, meta);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_getattr(unsigned int sb_index, unsigned int d_index, void *meta)
{
	return actvxactcall_drvcall_devhost_transfs_getattr(false, false, sb_index, d_index, meta);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_getattr(rref_t __rref, unsigned int sb_index, unsigned int d_index, void *meta)
{
	return actvxactcapcall_drvcall_devhost_transfs_getattr(false, false, __rref, sb_index, d_index, meta);
}

extern int __actvcall_drvcall_devhost_transfs_listxattr(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, char *buffer, size_t size);


extern int __actvcapcall_drvcall_devhost_transfs_listxattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, char *buffer, size_t size);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_listxattr(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int d_index, char *buffer, size_t size, struct __actvret_drvcall_devhost_transfs_listxattr *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_listxattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_listxattr(rpc_info, __ret, s_index, d_index, buffer, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_listxattr(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int d_index, char *buffer, size_t size, struct __actvret_drvcall_devhost_transfs_listxattr *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_listxattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_listxattr(__rref, rpc_info, __ret, s_index, d_index, buffer, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_listxattr(unsigned int s_index, unsigned int d_index, char *buffer, size_t size, struct __actvret_drvcall_devhost_transfs_listxattr *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_listxattr(false, false, s_index, d_index, buffer, size, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_listxattr(rref_t __rref, unsigned int s_index, unsigned int d_index, char *buffer, size_t size, struct __actvret_drvcall_devhost_transfs_listxattr *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_listxattr(false, false, __rref, s_index, d_index, buffer, size, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_getxattr(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, char *value, size_t size);


extern int __actvcapcall_drvcall_devhost_transfs_getxattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, char *value, size_t size);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_getxattr(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, char *value, size_t size, struct __actvret_drvcall_devhost_transfs_getxattr *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_getxattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_getxattr(rpc_info, __ret, s_index, d_index, prefix, actv_name, value, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_getxattr(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, char *value, size_t size, struct __actvret_drvcall_devhost_transfs_getxattr *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_getxattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_getxattr(__rref, rpc_info, __ret, s_index, d_index, prefix, actv_name, value, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_getxattr(unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, char *value, size_t size, struct __actvret_drvcall_devhost_transfs_getxattr *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_getxattr(false, false, s_index, d_index, prefix, actv_name, value, size, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_getxattr(rref_t __rref, unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, char *value, size_t size, struct __actvret_drvcall_devhost_transfs_getxattr *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_getxattr(false, false, __rref, s_index, d_index, prefix, actv_name, value, size, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_setxattr(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, const char *value, size_t size, int flags);


extern int __actvcapcall_drvcall_devhost_transfs_setxattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, const char *value, size_t size, int flags);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_setxattr(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, const char *value, size_t size, int flags)
{
	const unsigned long __callno = __devhost_method_transfs_setxattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_setxattr(rpc_info, NULL, s_index, d_index, prefix, actv_name, value, size, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_setxattr(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, const char *value, size_t size, int flags)
{
	const unsigned long __callno = __devhost_method_transfs_setxattr;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_setxattr(__rref, rpc_info, NULL, s_index, d_index, prefix, actv_name, value, size, flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_setxattr(unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, const char *value, size_t size, int flags)
{
	return actvxactcall_drvcall_devhost_transfs_setxattr(false, false, s_index, d_index, prefix, actv_name, value, size, flags);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_setxattr(rref_t __rref, unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, const char *value, size_t size, int flags)
{
	return actvxactcapcall_drvcall_devhost_transfs_setxattr(false, false, __rref, s_index, d_index, prefix, actv_name, value, size, flags);
}

extern int __actvcall_drvcall_devhost_transfs_tmpfile(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_addr, mode_t mode, struct hm_actv_buf actv_dname);


extern int __actvcapcall_drvcall_devhost_transfs_tmpfile(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_addr, mode_t mode, struct hm_actv_buf actv_dname);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_tmpfile(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int pd_addr, mode_t mode, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_tmpfile *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_tmpfile;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_tmpfile(rpc_info, __ret, s_index, pd_addr, mode, actv_dname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_tmpfile(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int pd_addr, mode_t mode, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_tmpfile *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_tmpfile;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(mode_t), __arch_actv_rpc_stksz_alignment(mode_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct hm_actv_buf), __arch_actv_rpc_stksz_alignment(struct hm_actv_buf));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_tmpfile(__rref, rpc_info, __ret, s_index, pd_addr, mode, actv_dname);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_tmpfile(unsigned int s_index, unsigned int pd_addr, mode_t mode, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_tmpfile *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_tmpfile(false, false, s_index, pd_addr, mode, actv_dname, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_tmpfile(rref_t __rref, unsigned int s_index, unsigned int pd_addr, mode_t mode, struct hm_actv_buf actv_dname, struct __actvret_drvcall_devhost_transfs_tmpfile *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_tmpfile(false, false, __rref, s_index, pd_addr, mode, actv_dname, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_kill_dentry(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index);


extern int __actvcapcall_drvcall_devhost_transfs_kill_dentry(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_kill_dentry(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int d_index)
{
	const unsigned long __callno = __devhost_method_transfs_kill_dentry;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_kill_dentry(rpc_info, NULL, s_index, d_index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_kill_dentry(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int d_index)
{
	const unsigned long __callno = __devhost_method_transfs_kill_dentry;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_kill_dentry(__rref, rpc_info, NULL, s_index, d_index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_kill_dentry(unsigned int s_index, unsigned int d_index)
{
	return actvxactcall_drvcall_devhost_transfs_kill_dentry(false, false, s_index, d_index);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_kill_dentry(rref_t __rref, unsigned int s_index, unsigned int d_index)
{
	return actvxactcapcall_drvcall_devhost_transfs_kill_dentry(false, false, __rref, s_index, d_index);
}

extern int __actvcall_drvcall_devhost_transfs_node_put(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int v_index);


extern int __actvcapcall_drvcall_devhost_transfs_node_put(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int v_index);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_node_put(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int v_index)
{
	const unsigned long __callno = __devhost_method_transfs_node_put;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_node_put(rpc_info, NULL, s_index, v_index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_node_put(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int v_index)
{
	const unsigned long __callno = __devhost_method_transfs_node_put;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_node_put(__rref, rpc_info, NULL, s_index, v_index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_node_put(unsigned int s_index, unsigned int v_index)
{
	return actvxactcall_drvcall_devhost_transfs_node_put(false, false, s_index, v_index);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_node_put(rref_t __rref, unsigned int s_index, unsigned int v_index)
{
	return actvxactcapcall_drvcall_devhost_transfs_node_put(false, false, __rref, s_index, v_index);
}

extern int __actvcall_drvcall_devhost_transfs_sync(unsigned long long __rpc_info, void *__buf, unsigned int s_index);


extern int __actvcapcall_drvcall_devhost_transfs_sync(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_sync(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index)
{
	const unsigned long __callno = __devhost_method_transfs_sync;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_sync(rpc_info, NULL, s_index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_sync(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index)
{
	const unsigned long __callno = __devhost_method_transfs_sync;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_sync(__rref, rpc_info, NULL, s_index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_sync(unsigned int s_index)
{
	return actvxactcall_drvcall_devhost_transfs_sync(false, false, s_index);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_sync(rref_t __rref, unsigned int s_index)
{
	return actvxactcapcall_drvcall_devhost_transfs_sync(false, false, __rref, s_index);
}

extern int __actvcall_drvcall_devhost_transfs_write(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len);


extern int __actvcapcall_drvcall_devhost_transfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len, struct __actvret_drvcall_devhost_transfs_write *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_write(rpc_info, __ret, s_index, f_index, pos, src, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len, struct __actvret_drvcall_devhost_transfs_write *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_write(__rref, rpc_info, __ret, s_index, f_index, pos, src, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_write(unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len, struct __actvret_drvcall_devhost_transfs_write *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_write(false, false, s_index, f_index, pos, src, len, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_write(rref_t __rref, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len, struct __actvret_drvcall_devhost_transfs_write *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_write(false, false, __rref, s_index, f_index, pos, src, len, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_read(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len);


extern int __actvcapcall_drvcall_devhost_transfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len, struct __actvret_drvcall_devhost_transfs_read *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_read(rpc_info, __ret, s_index, f_index, pos, src, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len, struct __actvret_drvcall_devhost_transfs_read *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_read(__rref, rpc_info, __ret, s_index, f_index, pos, src, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_read(unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len, struct __actvret_drvcall_devhost_transfs_read *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_read(false, false, s_index, f_index, pos, src, len, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_read(rref_t __rref, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len, struct __actvret_drvcall_devhost_transfs_read *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_read(false, false, __rref, s_index, f_index, pos, src, len, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_write_iter(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *iov, int nr_segs, size_t count);


extern int __actvcapcall_drvcall_devhost_transfs_write_iter(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *iov, int nr_segs, size_t count);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_write_iter(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *iov, int nr_segs, size_t count, struct __actvret_drvcall_devhost_transfs_write_iter *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_write_iter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_write_iter(rpc_info, __ret, s_index, f_index, pos, iov, nr_segs, count);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_write_iter(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *iov, int nr_segs, size_t count, struct __actvret_drvcall_devhost_transfs_write_iter *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_write_iter;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_write_iter(__rref, rpc_info, __ret, s_index, f_index, pos, iov, nr_segs, count);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_write_iter(unsigned int s_index, unsigned int f_index, uint64_t pos, const void *iov, int nr_segs, size_t count, struct __actvret_drvcall_devhost_transfs_write_iter *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_write_iter(false, false, s_index, f_index, pos, iov, nr_segs, count, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_write_iter(rref_t __rref, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *iov, int nr_segs, size_t count, struct __actvret_drvcall_devhost_transfs_write_iter *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_write_iter(false, false, __rref, s_index, f_index, pos, iov, nr_segs, count, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_fsync(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index);


extern int __actvcapcall_drvcall_devhost_transfs_fsync(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_fsync(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int f_index)
{
	const unsigned long __callno = __devhost_method_transfs_fsync;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_fsync(rpc_info, NULL, s_index, f_index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_fsync(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int f_index)
{
	const unsigned long __callno = __devhost_method_transfs_fsync;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_fsync(__rref, rpc_info, NULL, s_index, f_index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_fsync(unsigned int s_index, unsigned int f_index)
{
	return actvxactcall_drvcall_devhost_transfs_fsync(false, false, s_index, f_index);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_fsync(rref_t __rref, unsigned int s_index, unsigned int f_index)
{
	return actvxactcapcall_drvcall_devhost_transfs_fsync(false, false, __rref, s_index, f_index);
}

extern int __actvcall_drvcall_devhost_transfs_readdir(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int f_index, uint64_t pos, void *buf, size_t size);


extern int __actvcapcall_drvcall_devhost_transfs_readdir(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int f_index, uint64_t pos, void *buf, size_t size);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_readdir(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int sb_index, unsigned int f_index, uint64_t pos, void *buf, size_t size, struct __actvret_drvcall_devhost_transfs_readdir *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_readdir;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_readdir(rpc_info, __ret, sb_index, f_index, pos, buf, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_readdir(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int sb_index, unsigned int f_index, uint64_t pos, void *buf, size_t size, struct __actvret_drvcall_devhost_transfs_readdir *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_readdir;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_readdir(__rref, rpc_info, __ret, sb_index, f_index, pos, buf, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_readdir(unsigned int sb_index, unsigned int f_index, uint64_t pos, void *buf, size_t size, struct __actvret_drvcall_devhost_transfs_readdir *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_readdir(false, false, sb_index, f_index, pos, buf, size, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_readdir(rref_t __rref, unsigned int sb_index, unsigned int f_index, uint64_t pos, void *buf, size_t size, struct __actvret_drvcall_devhost_transfs_readdir *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_readdir(false, false, __rref, sb_index, f_index, pos, buf, size, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_open(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, unsigned int f_modes, unsigned int f_flags);


extern int __actvcapcall_drvcall_devhost_transfs_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, unsigned int f_modes, unsigned int f_flags);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_open(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int sb_index, unsigned int d_index, unsigned int f_modes, unsigned int f_flags, struct __actvret_drvcall_devhost_transfs_open *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_open(rpc_info, __ret, sb_index, d_index, f_modes, f_flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_open(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int sb_index, unsigned int d_index, unsigned int f_modes, unsigned int f_flags, struct __actvret_drvcall_devhost_transfs_open *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_open(__rref, rpc_info, __ret, sb_index, d_index, f_modes, f_flags);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_open(unsigned int sb_index, unsigned int d_index, unsigned int f_modes, unsigned int f_flags, struct __actvret_drvcall_devhost_transfs_open *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_open(false, false, sb_index, d_index, f_modes, f_flags, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_open(rref_t __rref, unsigned int sb_index, unsigned int d_index, unsigned int f_modes, unsigned int f_flags, struct __actvret_drvcall_devhost_transfs_open *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_open(false, false, __rref, sb_index, d_index, f_modes, f_flags, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_fallocate(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int f_index, unsigned int mode, uint64_t pos, uint64_t len);


extern int __actvcapcall_drvcall_devhost_transfs_fallocate(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int f_index, unsigned int mode, uint64_t pos, uint64_t len);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_fallocate(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int sb_index, unsigned int f_index, unsigned int mode, uint64_t pos, uint64_t len)
{
	const unsigned long __callno = __devhost_method_transfs_fallocate;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_fallocate(rpc_info, NULL, sb_index, f_index, mode, pos, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_fallocate(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int sb_index, unsigned int f_index, unsigned int mode, uint64_t pos, uint64_t len)
{
	const unsigned long __callno = __devhost_method_transfs_fallocate;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_fallocate(__rref, rpc_info, NULL, sb_index, f_index, mode, pos, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_fallocate(unsigned int sb_index, unsigned int f_index, unsigned int mode, uint64_t pos, uint64_t len)
{
	return actvxactcall_drvcall_devhost_transfs_fallocate(false, false, sb_index, f_index, mode, pos, len);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_fallocate(rref_t __rref, unsigned int sb_index, unsigned int f_index, unsigned int mode, uint64_t pos, uint64_t len)
{
	return actvxactcapcall_drvcall_devhost_transfs_fallocate(false, false, __rref, sb_index, f_index, mode, pos, len);
}

extern int __actvcall_drvcall_devhost_transfs_close(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index);


extern int __actvcapcall_drvcall_devhost_transfs_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_close(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int f_index)
{
	const unsigned long __callno = __devhost_method_transfs_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_close(rpc_info, NULL, s_index, f_index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_close(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int f_index)
{
	const unsigned long __callno = __devhost_method_transfs_close;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_close(__rref, rpc_info, NULL, s_index, f_index);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_close(unsigned int s_index, unsigned int f_index)
{
	return actvxactcall_drvcall_devhost_transfs_close(false, false, s_index, f_index);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_close(rref_t __rref, unsigned int s_index, unsigned int f_index)
{
	return actvxactcapcall_drvcall_devhost_transfs_close(false, false, __rref, s_index, f_index);
}

extern int __actvcall_drvcall_devhost_transfs_ioctl(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, unsigned int cmd, unsigned long arg);


extern int __actvcapcall_drvcall_devhost_transfs_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, unsigned int cmd, unsigned long arg);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int f_index, unsigned int cmd, unsigned long arg)
{
	const unsigned long __callno = __devhost_method_transfs_ioctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_ioctl(rpc_info, NULL, s_index, f_index, cmd, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int f_index, unsigned int cmd, unsigned long arg)
{
	const unsigned long __callno = __devhost_method_transfs_ioctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_ioctl(__rref, rpc_info, NULL, s_index, f_index, cmd, arg);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_ioctl(unsigned int s_index, unsigned int f_index, unsigned int cmd, unsigned long arg)
{
	return actvxactcall_drvcall_devhost_transfs_ioctl(false, false, s_index, f_index, cmd, arg);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_ioctl(rref_t __rref, unsigned int s_index, unsigned int f_index, unsigned int cmd, unsigned long arg)
{
	return actvxactcapcall_drvcall_devhost_transfs_ioctl(false, false, __rref, s_index, f_index, cmd, arg);
}

extern int __actvcall_drvcall_devhost_transfs_direct_IO(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t offset, void *buf, size_t len, unsigned int type);


extern int __actvcapcall_drvcall_devhost_transfs_direct_IO(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t offset, void *buf, size_t len, unsigned int type);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_direct_IO(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int f_index, uint64_t offset, void *buf, size_t len, unsigned int type, struct __actvret_drvcall_devhost_transfs_direct_IO *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_direct_IO;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_direct_IO(rpc_info, __ret, s_index, f_index, offset, buf, len, type);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_direct_IO(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int f_index, uint64_t offset, void *buf, size_t len, unsigned int type, struct __actvret_drvcall_devhost_transfs_direct_IO *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_direct_IO;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint64_t), __arch_actv_rpc_stksz_alignment(uint64_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_direct_IO(__rref, rpc_info, __ret, s_index, f_index, offset, buf, len, type);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_direct_IO(unsigned int s_index, unsigned int f_index, uint64_t offset, void *buf, size_t len, unsigned int type, struct __actvret_drvcall_devhost_transfs_direct_IO *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_direct_IO(false, false, s_index, f_index, offset, buf, len, type, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_direct_IO(rref_t __rref, unsigned int s_index, unsigned int f_index, uint64_t offset, void *buf, size_t len, unsigned int type, struct __actvret_drvcall_devhost_transfs_direct_IO *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_direct_IO(false, false, __rref, s_index, f_index, offset, buf, len, type, __ret);
}

extern int __actvcall_drvcall_devhost_transfs_update_opts(unsigned long long __rpc_info, void *__buf, unsigned int s_index, char *ubuf, size_t buflen);


extern int __actvcapcall_drvcall_devhost_transfs_update_opts(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, char *ubuf, size_t buflen);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_update_opts(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, char *ubuf, size_t buflen)
{
	const unsigned long __callno = __devhost_method_transfs_update_opts;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_update_opts(rpc_info, NULL, s_index, ubuf, buflen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_update_opts(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, char *ubuf, size_t buflen)
{
	const unsigned long __callno = __devhost_method_transfs_update_opts;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_update_opts(__rref, rpc_info, NULL, s_index, ubuf, buflen);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_update_opts(unsigned int s_index, char *ubuf, size_t buflen)
{
	return actvxactcall_drvcall_devhost_transfs_update_opts(false, false, s_index, ubuf, buflen);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_update_opts(rref_t __rref, unsigned int s_index, char *ubuf, size_t buflen)
{
	return actvxactcapcall_drvcall_devhost_transfs_update_opts(false, false, __rref, s_index, ubuf, buflen);
}

extern int __actvcall_drvcall_devhost_transfs_iommap(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, unsigned long addr, unsigned long long vspace_rref, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset);


extern int __actvcapcall_drvcall_devhost_transfs_iommap(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, unsigned long addr, unsigned long long vspace_rref, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_transfs_iommap(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int s_index, unsigned int f_index, unsigned long addr, unsigned long long vspace_rref, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset, struct __actvret_drvcall_devhost_transfs_iommap *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_iommap;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_transfs_iommap(rpc_info, __ret, s_index, f_index, addr, vspace_rref, length, prot, flags, offset);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_transfs_iommap(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int s_index, unsigned int f_index, unsigned long addr, unsigned long long vspace_rref, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset, struct __actvret_drvcall_devhost_transfs_iommap *__ret)
{
	const unsigned long __callno = __devhost_method_transfs_iommap;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_transfs_iommap(__rref, rpc_info, __ret, s_index, f_index, addr, vspace_rref, length, prot, flags, offset);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_transfs_iommap(unsigned int s_index, unsigned int f_index, unsigned long addr, unsigned long long vspace_rref, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset, struct __actvret_drvcall_devhost_transfs_iommap *__ret)
{
	return actvxactcall_drvcall_devhost_transfs_iommap(false, false, s_index, f_index, addr, vspace_rref, length, prot, flags, offset, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_transfs_iommap(rref_t __rref, unsigned int s_index, unsigned int f_index, unsigned long addr, unsigned long long vspace_rref, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset, struct __actvret_drvcall_devhost_transfs_iommap *__ret)
{
	return actvxactcapcall_drvcall_devhost_transfs_iommap(false, false, __rref, s_index, f_index, addr, vspace_rref, length, prot, flags, offset, __ret);
}

extern int __actvcall_drvcall_devhost_shrinker_query(unsigned long long __rpc_info, void *__buf, int type);


extern int __actvcapcall_drvcall_devhost_shrinker_query(rref_t __rref, unsigned long long __rpc_info, void *__buf, int type);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_shrinker_query(bool is_xact_fwd, bool is_xact_allow_refwd, int type)
{
	const unsigned long __callno = __devhost_method_shrinker_query;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_shrinker_query(rpc_info, NULL, type);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_shrinker_query(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int type)
{
	const unsigned long __callno = __devhost_method_shrinker_query;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_shrinker_query(__rref, rpc_info, NULL, type);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_shrinker_query(int type)
{
	return actvxactcall_drvcall_devhost_shrinker_query(false, false, type);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_shrinker_query(rref_t __rref, int type)
{
	return actvxactcapcall_drvcall_devhost_shrinker_query(false, false, __rref, type);
}

extern int __actvcall_drvcall_devhost_shrinker_shrink(unsigned long long __rpc_info, void *__buf, int max_nr);


extern int __actvcapcall_drvcall_devhost_shrinker_shrink(rref_t __rref, unsigned long long __rpc_info, void *__buf, int max_nr);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_shrinker_shrink(bool is_xact_fwd, bool is_xact_allow_refwd, int max_nr, struct __actvret_drvcall_devhost_shrinker_shrink *__ret)
{
	const unsigned long __callno = __devhost_method_shrinker_shrink;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_shrinker_shrink(rpc_info, __ret, max_nr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_shrinker_shrink(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int max_nr, struct __actvret_drvcall_devhost_shrinker_shrink *__ret)
{
	const unsigned long __callno = __devhost_method_shrinker_shrink;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_shrinker_shrink(__rref, rpc_info, __ret, max_nr);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_shrinker_shrink(int max_nr, struct __actvret_drvcall_devhost_shrinker_shrink *__ret)
{
	return actvxactcall_drvcall_devhost_shrinker_shrink(false, false, max_nr, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_shrinker_shrink(rref_t __rref, int max_nr, struct __actvret_drvcall_devhost_shrinker_shrink *__ret)
{
	return actvxactcapcall_drvcall_devhost_shrinker_shrink(false, false, __rref, max_nr, __ret);
}

extern int __actvcall_drvcall_devhost_process_revoke(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, pid_t pid, uid_t uid);


extern int __actvcapcall_drvcall_devhost_process_revoke(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, pid_t pid, uid_t uid);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_process_revoke(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	const unsigned long __callno = __devhost_method_process_revoke;
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
		__res = __actvcall_drvcall_devhost_process_revoke(rpc_info, NULL, cnode_idx, pid, uid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_process_revoke(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	const unsigned long __callno = __devhost_method_process_revoke;
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
		__res = __actvcapcall_drvcall_devhost_process_revoke(__rref, rpc_info, NULL, cnode_idx, pid, uid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_process_revoke(unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	return actvxactcall_drvcall_devhost_process_revoke(false, false, cnode_idx, pid, uid);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_process_revoke(rref_t __rref, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	return actvxactcapcall_drvcall_devhost_process_revoke(false, false, __rref, cnode_idx, pid, uid);
}

extern int __actvcall_drvcall_devhost_sec_mgmt(unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command);


extern int __actvcapcall_drvcall_devhost_sec_mgmt(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_sec_mgmt(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int module, unsigned int command)
{
	const unsigned long __callno = __devhost_method_sec_mgmt;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_sec_mgmt(rpc_info, NULL, module, command);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_sec_mgmt(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int module, unsigned int command)
{
	const unsigned long __callno = __devhost_method_sec_mgmt;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_sec_mgmt(__rref, rpc_info, NULL, module, command);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_sec_mgmt(unsigned int module, unsigned int command)
{
	return actvxactcall_drvcall_devhost_sec_mgmt(false, false, module, command);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_sec_mgmt(rref_t __rref, unsigned int module, unsigned int command)
{
	return actvxactcapcall_drvcall_devhost_sec_mgmt(false, false, __rref, module, command);
}

extern int __actvcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __devhost_method_sec_test_hkip_seharmony_prot_policydb;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __devhost_method_sec_test_hkip_seharmony_prot_policydb;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(void)
{
	return actvxactcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(rref_t __rref)
{
	return actvxactcapcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(false, false, __rref);
}

extern int __actvcall_drvcall_devhost_tests_hkip_protection(unsigned long long __rpc_info, void *__buf, unsigned long test_id);


extern int __actvcapcall_drvcall_devhost_tests_hkip_protection(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long test_id);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_tests_hkip_protection(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long test_id)
{
	const unsigned long __callno = __devhost_method_tests_hkip_protection;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_tests_hkip_protection(rpc_info, NULL, test_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_tests_hkip_protection(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long test_id)
{
	const unsigned long __callno = __devhost_method_tests_hkip_protection;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_tests_hkip_protection(__rref, rpc_info, NULL, test_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_tests_hkip_protection(unsigned long test_id)
{
	return actvxactcall_drvcall_devhost_tests_hkip_protection(false, false, test_id);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_tests_hkip_protection(rref_t __rref, unsigned long test_id)
{
	return actvxactcapcall_drvcall_devhost_tests_hkip_protection(false, false, __rref, test_id);
}

extern int __actvcall_drvcall_devhost_sec_test_hkip_roafter_prot(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devhost_sec_test_hkip_roafter_prot(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_sec_test_hkip_roafter_prot(bool is_xact_fwd, bool is_xact_allow_refwd)
{
	const unsigned long __callno = __devhost_method_sec_test_hkip_roafter_prot;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_sec_test_hkip_roafter_prot(rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_sec_test_hkip_roafter_prot(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref)
{
	const unsigned long __callno = __devhost_method_sec_test_hkip_roafter_prot;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_sec_test_hkip_roafter_prot(__rref, rpc_info, NULL);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_sec_test_hkip_roafter_prot(void)
{
	return actvxactcall_drvcall_devhost_sec_test_hkip_roafter_prot(false, false);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_sec_test_hkip_roafter_prot(rref_t __rref)
{
	return actvxactcapcall_drvcall_devhost_sec_test_hkip_roafter_prot(false, false, __rref);
}

extern int __actvcall_drvcall_devhost_sec_sync_fastpath_open(unsigned long long __rpc_info, void *__buf, unsigned long long filp, unsigned int task_sid, unsigned int state);


extern int __actvcapcall_drvcall_devhost_sec_sync_fastpath_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long filp, unsigned int task_sid, unsigned int state);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_sec_sync_fastpath_open(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long filp, unsigned int task_sid, unsigned int state, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devhost_method_sec_sync_fastpath_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_sec_sync_fastpath_open(rpc_info, ___void_buf, filp, task_sid, state);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_sec_sync_fastpath_open(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long filp, unsigned int task_sid, unsigned int state, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devhost_method_sec_sync_fastpath_open;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_sec_sync_fastpath_open(__rref, rpc_info, ___void_buf, filp, task_sid, state);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_sec_sync_fastpath_open(unsigned long long filp, unsigned int task_sid, unsigned int state, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_drvcall_devhost_sec_sync_fastpath_open(false, false, filp, task_sid, state, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_sec_sync_fastpath_open(rref_t __rref, unsigned long long filp, unsigned int task_sid, unsigned int state, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_drvcall_devhost_sec_sync_fastpath_open(false, false, __rref, filp, task_sid, state, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_drvcall_devhost_sec_sync_fastpath_dev(unsigned long long __rpc_info, void *__buf, int devid, unsigned int task_sid, unsigned int state);


extern int __actvcapcall_drvcall_devhost_sec_sync_fastpath_dev(rref_t __rref, unsigned long long __rpc_info, void *__buf, int devid, unsigned int task_sid, unsigned int state);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_sec_sync_fastpath_dev(bool is_xact_fwd, bool is_xact_allow_refwd, int devid, unsigned int task_sid, unsigned int state, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devhost_method_sec_sync_fastpath_dev;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_sec_sync_fastpath_dev(rpc_info, ___void_buf, devid, task_sid, state);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_sec_sync_fastpath_dev(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int devid, unsigned int task_sid, unsigned int state, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __devhost_method_sec_sync_fastpath_dev;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_sec_sync_fastpath_dev(__rref, rpc_info, ___void_buf, devid, task_sid, state);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_sec_sync_fastpath_dev(int devid, unsigned int task_sid, unsigned int state, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_drvcall_devhost_sec_sync_fastpath_dev(false, false, devid, task_sid, state, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_sec_sync_fastpath_dev(rref_t __rref, int devid, unsigned int task_sid, unsigned int state, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_drvcall_devhost_sec_sync_fastpath_dev(false, false, __rref, devid, task_sid, state, ___void_buf, ___void_buf_sz);
}

#ifdef __arm__
#endif
extern int __actvcall_drvcall_devhost_trace_enable_disable_ldk_event(unsigned long long __rpc_info, void *__buf, unsigned int tp_index, bool enable);


extern int __actvcapcall_drvcall_devhost_trace_enable_disable_ldk_event(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tp_index, bool enable);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_trace_enable_disable_ldk_event(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int tp_index, bool enable)
{
	const unsigned long __callno = __devhost_method_trace_enable_disable_ldk_event;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_trace_enable_disable_ldk_event(rpc_info, NULL, tp_index, enable);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_trace_enable_disable_ldk_event(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int tp_index, bool enable)
{
	const unsigned long __callno = __devhost_method_trace_enable_disable_ldk_event;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_trace_enable_disable_ldk_event(__rref, rpc_info, NULL, tp_index, enable);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_trace_enable_disable_ldk_event(unsigned int tp_index, bool enable)
{
	return actvxactcall_drvcall_devhost_trace_enable_disable_ldk_event(false, false, tp_index, enable);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_trace_enable_disable_ldk_event(rref_t __rref, unsigned int tp_index, bool enable)
{
	return actvxactcapcall_drvcall_devhost_trace_enable_disable_ldk_event(false, false, __rref, tp_index, enable);
}

extern int __actvcall_drvcall_devhost_trace_ldk_event_format(unsigned long long __rpc_info, void *__buf, unsigned int tp_index, char *format);


extern int __actvcapcall_drvcall_devhost_trace_ldk_event_format(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tp_index, char *format);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_trace_ldk_event_format(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int tp_index, char *format)
{
	const unsigned long __callno = __devhost_method_trace_ldk_event_format;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_trace_ldk_event_format(rpc_info, NULL, tp_index, format);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_trace_ldk_event_format(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int tp_index, char *format)
{
	const unsigned long __callno = __devhost_method_trace_ldk_event_format;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_trace_ldk_event_format(__rref, rpc_info, NULL, tp_index, format);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_trace_ldk_event_format(unsigned int tp_index, char *format)
{
	return actvxactcall_drvcall_devhost_trace_ldk_event_format(false, false, tp_index, format);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_trace_ldk_event_format(rref_t __rref, unsigned int tp_index, char *format)
{
	return actvxactcapcall_drvcall_devhost_trace_ldk_event_format(false, false, __rref, tp_index, format);
}

extern int __actvcall_drvcall_devhost_trace_print_ldk_event(unsigned long long __rpc_info, void *__buf, unsigned int tp_index, void *entry, size_t entry_size, char *event_st);


extern int __actvcapcall_drvcall_devhost_trace_print_ldk_event(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tp_index, void *entry, size_t entry_size, char *event_st);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_trace_print_ldk_event(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int tp_index, void *entry, size_t entry_size, char *event_st)
{
	const unsigned long __callno = __devhost_method_trace_print_ldk_event;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_trace_print_ldk_event(rpc_info, NULL, tp_index, entry, entry_size, event_st);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_trace_print_ldk_event(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int tp_index, void *entry, size_t entry_size, char *event_st)
{
	const unsigned long __callno = __devhost_method_trace_print_ldk_event;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(char*), __arch_actv_rpc_stksz_alignment(char*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_trace_print_ldk_event(__rref, rpc_info, NULL, tp_index, entry, entry_size, event_st);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_trace_print_ldk_event(unsigned int tp_index, void *entry, size_t entry_size, char *event_st)
{
	return actvxactcall_drvcall_devhost_trace_print_ldk_event(false, false, tp_index, entry, entry_size, event_st);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_trace_print_ldk_event(rref_t __rref, unsigned int tp_index, void *entry, size_t entry_size, char *event_st)
{
	return actvxactcapcall_drvcall_devhost_trace_print_ldk_event(false, false, __rref, tp_index, entry, entry_size, event_st);
}

extern int __actvcall_drvcall_devhost_file_info_query_dmabuf(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_idx);


extern int __actvcapcall_drvcall_devhost_file_info_query_dmabuf(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_idx);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_file_info_query_dmabuf(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long long file_idx, struct __actvret_drvcall_devhost_file_info_query_dmabuf *__ret)
{
	const unsigned long __callno = __devhost_method_file_info_query_dmabuf;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_file_info_query_dmabuf(rpc_info, __ret, fastpath, file_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_file_info_query_dmabuf(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long long file_idx, struct __actvret_drvcall_devhost_file_info_query_dmabuf *__ret)
{
	const unsigned long __callno = __devhost_method_file_info_query_dmabuf;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_file_info_query_dmabuf(__rref, rpc_info, __ret, fastpath, file_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_file_info_query_dmabuf(unsigned int fastpath, unsigned long long file_idx, struct __actvret_drvcall_devhost_file_info_query_dmabuf *__ret)
{
	return actvxactcall_drvcall_devhost_file_info_query_dmabuf(false, false, fastpath, file_idx, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_file_info_query_dmabuf(rref_t __rref, unsigned int fastpath, unsigned long long file_idx, struct __actvret_drvcall_devhost_file_info_query_dmabuf *__ret)
{
	return actvxactcapcall_drvcall_devhost_file_info_query_dmabuf(false, false, __rref, fastpath, file_idx, __ret);
}

extern int __actvcall_drvcall_devhost_file_info_query_fence(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_idx, unsigned int offset);


extern int __actvcapcall_drvcall_devhost_file_info_query_fence(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_idx, unsigned int offset);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_file_info_query_fence(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long long file_idx, unsigned int offset, struct __actvret_drvcall_devhost_file_info_query_fence *__ret)
{
	const unsigned long __callno = __devhost_method_file_info_query_fence;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_file_info_query_fence(rpc_info, __ret, fastpath, file_idx, offset);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_file_info_query_fence(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long long file_idx, unsigned int offset, struct __actvret_drvcall_devhost_file_info_query_fence *__ret)
{
	const unsigned long __callno = __devhost_method_file_info_query_fence;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_file_info_query_fence(__rref, rpc_info, __ret, fastpath, file_idx, offset);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_file_info_query_fence(unsigned int fastpath, unsigned long long file_idx, unsigned int offset, struct __actvret_drvcall_devhost_file_info_query_fence *__ret)
{
	return actvxactcall_drvcall_devhost_file_info_query_fence(false, false, fastpath, file_idx, offset, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_file_info_query_fence(rref_t __rref, unsigned int fastpath, unsigned long long file_idx, unsigned int offset, struct __actvret_drvcall_devhost_file_info_query_fence *__ret)
{
	return actvxactcapcall_drvcall_devhost_file_info_query_fence(false, false, __rref, fastpath, file_idx, offset, __ret);
}

extern int __actvcall_drvcall_devhost_file_info_query_ion(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_idx);


extern int __actvcapcall_drvcall_devhost_file_info_query_ion(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_idx);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_file_info_query_ion(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int fastpath, unsigned long long file_idx, struct __actvret_drvcall_devhost_file_info_query_ion *__ret)
{
	const unsigned long __callno = __devhost_method_file_info_query_ion;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_file_info_query_ion(rpc_info, __ret, fastpath, file_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_file_info_query_ion(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int fastpath, unsigned long long file_idx, struct __actvret_drvcall_devhost_file_info_query_ion *__ret)
{
	const unsigned long __callno = __devhost_method_file_info_query_ion;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_file_info_query_ion(__rref, rpc_info, __ret, fastpath, file_idx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_file_info_query_ion(unsigned int fastpath, unsigned long long file_idx, struct __actvret_drvcall_devhost_file_info_query_ion *__ret)
{
	return actvxactcall_drvcall_devhost_file_info_query_ion(false, false, fastpath, file_idx, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_file_info_query_ion(rref_t __rref, unsigned int fastpath, unsigned long long file_idx, struct __actvret_drvcall_devhost_file_info_query_ion *__ret)
{
	return actvxactcapcall_drvcall_devhost_file_info_query_ion(false, false, __rref, fastpath, file_idx, __ret);
}

extern int __actvcall_drvcall_devhost_file_info_query_meminfo(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_drvcall_devhost_file_info_query_meminfo(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_file_info_query_meminfo(bool is_xact_fwd, bool is_xact_allow_refwd, struct __actvret_drvcall_devhost_file_info_query_meminfo *__ret)
{
	const unsigned long __callno = __devhost_method_file_info_query_meminfo;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_file_info_query_meminfo(rpc_info, __ret);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_file_info_query_meminfo(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct __actvret_drvcall_devhost_file_info_query_meminfo *__ret)
{
	const unsigned long __callno = __devhost_method_file_info_query_meminfo;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_file_info_query_meminfo(__rref, rpc_info, __ret);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_file_info_query_meminfo(struct __actvret_drvcall_devhost_file_info_query_meminfo *__ret)
{
	return actvxactcall_drvcall_devhost_file_info_query_meminfo(false, false, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_file_info_query_meminfo(rref_t __rref, struct __actvret_drvcall_devhost_file_info_query_meminfo *__ret)
{
	return actvxactcapcall_drvcall_devhost_file_info_query_meminfo(false, false, __rref, __ret);
}

extern int __actvcall_drvcall_devhost_file_info_lowmem_debug(unsigned long long __rpc_info, void *__buf, int oom_score_adj, unsigned long long mem_total, bool is_emergency);


extern int __actvcapcall_drvcall_devhost_file_info_lowmem_debug(rref_t __rref, unsigned long long __rpc_info, void *__buf, int oom_score_adj, unsigned long long mem_total, bool is_emergency);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_file_info_lowmem_debug(bool is_xact_fwd, bool is_xact_allow_refwd, int oom_score_adj, unsigned long long mem_total, bool is_emergency)
{
	const unsigned long __callno = __devhost_method_file_info_lowmem_debug;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_file_info_lowmem_debug(rpc_info, NULL, oom_score_adj, mem_total, is_emergency);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_file_info_lowmem_debug(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int oom_score_adj, unsigned long long mem_total, bool is_emergency)
{
	const unsigned long __callno = __devhost_method_file_info_lowmem_debug;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(bool), __arch_actv_rpc_stksz_alignment(bool));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_file_info_lowmem_debug(__rref, rpc_info, NULL, oom_score_adj, mem_total, is_emergency);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_file_info_lowmem_debug(int oom_score_adj, unsigned long long mem_total, bool is_emergency)
{
	return actvxactcall_drvcall_devhost_file_info_lowmem_debug(false, false, oom_score_adj, mem_total, is_emergency);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_file_info_lowmem_debug(rref_t __rref, int oom_score_adj, unsigned long long mem_total, bool is_emergency)
{
	return actvxactcapcall_drvcall_devhost_file_info_lowmem_debug(false, false, __rref, oom_score_adj, mem_total, is_emergency);
}

extern int __actvcall_drvcall_devhost_file_info_query_gpu(unsigned long long __rpc_info, void *__buf, unsigned long pid);


extern int __actvcapcall_drvcall_devhost_file_info_query_gpu(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long pid);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_file_info_query_gpu(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long pid, struct __actvret_drvcall_devhost_file_info_query_gpu *__ret)
{
	const unsigned long __callno = __devhost_method_file_info_query_gpu;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_file_info_query_gpu(rpc_info, __ret, pid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_file_info_query_gpu(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long pid, struct __actvret_drvcall_devhost_file_info_query_gpu *__ret)
{
	const unsigned long __callno = __devhost_method_file_info_query_gpu;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_file_info_query_gpu(__rref, rpc_info, __ret, pid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_file_info_query_gpu(unsigned long pid, struct __actvret_drvcall_devhost_file_info_query_gpu *__ret)
{
	return actvxactcall_drvcall_devhost_file_info_query_gpu(false, false, pid, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_file_info_query_gpu(rref_t __rref, unsigned long pid, struct __actvret_drvcall_devhost_file_info_query_gpu *__ret)
{
	return actvxactcapcall_drvcall_devhost_file_info_query_gpu(false, false, __rref, pid, __ret);
}

extern int __actvcall_drvcall_devhost_probe_handle_brk(unsigned long long __rpc_info, void *__buf, struct arch_uctx *uctx);


extern int __actvcapcall_drvcall_devhost_probe_handle_brk(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct arch_uctx *uctx);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_probe_handle_brk(bool is_xact_fwd, bool is_xact_allow_refwd, struct arch_uctx *uctx)
{
	const unsigned long __callno = __devhost_method_probe_handle_brk;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct arch_uctx*), __arch_actv_rpc_stksz_alignment(struct arch_uctx*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_probe_handle_brk(rpc_info, NULL, uctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_probe_handle_brk(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct arch_uctx *uctx)
{
	const unsigned long __callno = __devhost_method_probe_handle_brk;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct arch_uctx*), __arch_actv_rpc_stksz_alignment(struct arch_uctx*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_probe_handle_brk(__rref, rpc_info, NULL, uctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_probe_handle_brk(struct arch_uctx *uctx)
{
	return actvxactcall_drvcall_devhost_probe_handle_brk(false, false, uctx);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_probe_handle_brk(rref_t __rref, struct arch_uctx *uctx)
{
	return actvxactcapcall_drvcall_devhost_probe_handle_brk(false, false, __rref, uctx);
}

extern int __actvcall_drvcall_devhost_probe_handle_ss_brk(unsigned long long __rpc_info, void *__buf, struct arch_uctx *uctx);


extern int __actvcapcall_drvcall_devhost_probe_handle_ss_brk(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct arch_uctx *uctx);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_probe_handle_ss_brk(bool is_xact_fwd, bool is_xact_allow_refwd, struct arch_uctx *uctx)
{
	const unsigned long __callno = __devhost_method_probe_handle_ss_brk;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct arch_uctx*), __arch_actv_rpc_stksz_alignment(struct arch_uctx*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_probe_handle_ss_brk(rpc_info, NULL, uctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_probe_handle_ss_brk(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct arch_uctx *uctx)
{
	const unsigned long __callno = __devhost_method_probe_handle_ss_brk;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct arch_uctx*), __arch_actv_rpc_stksz_alignment(struct arch_uctx*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_probe_handle_ss_brk(__rref, rpc_info, NULL, uctx);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_probe_handle_ss_brk(struct arch_uctx *uctx)
{
	return actvxactcall_drvcall_devhost_probe_handle_ss_brk(false, false, uctx);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_probe_handle_ss_brk(rref_t __rref, struct arch_uctx *uctx)
{
	return actvxactcapcall_drvcall_devhost_probe_handle_ss_brk(false, false, __rref, uctx);
}

extern int __actvcall_drvcall_devhost_fileguard_message_notify(unsigned long long __rpc_info, void *__buf, void *message, size_t len);


extern int __actvcapcall_drvcall_devhost_fileguard_message_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *message, size_t len);


static inline int __attribute__((always_inline)) actvxactcall_drvcall_devhost_fileguard_message_notify(bool is_xact_fwd, bool is_xact_allow_refwd, void *message, size_t len)
{
	const unsigned long __callno = __devhost_method_fileguard_message_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_drvcall_devhost_fileguard_message_notify(rpc_info, NULL, message, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_drvcall_devhost_fileguard_message_notify(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *message, size_t len)
{
	const unsigned long __callno = __devhost_method_fileguard_message_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_drvcall_devhost_fileguard_message_notify(__rref, rpc_info, NULL, message, len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_drvcall_devhost_fileguard_message_notify(void *message, size_t len)
{
	return actvxactcall_drvcall_devhost_fileguard_message_notify(false, false, message, len);
}

static inline int __attribute__((always_inline)) actvcapcall_drvcall_devhost_fileguard_message_notify(rref_t __rref, void *message, size_t len)
{
	return actvxactcapcall_drvcall_devhost_fileguard_message_notify(false, false, __rref, message, len);
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

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_core_load_driver(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_core_load_driver(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_core_load_driver(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, struct hm_actv_buf args, unsigned long long drv_shm_key, unsigned long size)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	struct hm_actv_buf __maybe_unused __actv_local_var_args = args;
	unsigned long long __maybe_unused __actv_local_var_drv_shm_key = drv_shm_key;
	unsigned long __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcall_drvcall_devhost_core_load_driver(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args), __ARCH_ACTV_RPC_PASS_ARG(drv_shm_key, __actv_local_var_drv_shm_key), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_core_load_driver(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf name, struct hm_actv_buf args, unsigned long long drv_shm_key, unsigned long size)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_name = name;
	struct hm_actv_buf __maybe_unused __actv_local_var_args = args;
	unsigned long long __maybe_unused __actv_local_var_drv_shm_key = drv_shm_key;
	unsigned long __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcapcall_drvcall_devhost_core_load_driver(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(name, __actv_local_var_name), __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args), __ARCH_ACTV_RPC_PASS_ARG(drv_shm_key, __actv_local_var_drv_shm_key), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_fallback"))) __internal_rpccall ____actvcall_drvcall_devhost_core_create_device(unsigned long long rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, ...);


static int __attribute__((alias("__internal_rpccapcall_fallback"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_core_create_device(rref_t __rref, unsigned long long rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, ...);


int __internal_actvcall __actvcall_drvcall_devhost_core_create_device(unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, struct hm_actv_buf devname, struct hm_actv_buf dev_compat, int parent_devid, unsigned int resource_count)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_devname = devname;
	struct hm_actv_buf __maybe_unused __actv_local_var_dev_compat = dev_compat;
	int __maybe_unused __actv_local_var_parent_devid = parent_devid;
	unsigned int __maybe_unused __actv_local_var_resource_count = resource_count;
	__ret = ____actvcall_drvcall_devhost_core_create_device(__rpc_info, __buf, __rply_buf, rply_buf_sz, __ARCH_ACTV_RPC_PASS_ARG(devname, __actv_local_var_devname), __ARCH_ACTV_RPC_PASS_ARG(dev_compat, __actv_local_var_dev_compat), __ARCH_ACTV_RPC_PASS_ARG(parent_devid, __actv_local_var_parent_devid), __ARCH_ACTV_RPC_PASS_ARG(resource_count, __actv_local_var_resource_count));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_core_create_device(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, struct hm_actv_buf devname, struct hm_actv_buf dev_compat, int parent_devid, unsigned int resource_count)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_devname = devname;
	struct hm_actv_buf __maybe_unused __actv_local_var_dev_compat = dev_compat;
	int __maybe_unused __actv_local_var_parent_devid = parent_devid;
	unsigned int __maybe_unused __actv_local_var_resource_count = resource_count;
	__ret = ____actvcapcall_drvcall_devhost_core_create_device(__rref, __rpc_info, __buf, __rply_buf, rply_buf_sz, __ARCH_ACTV_RPC_PASS_ARG(devname, __actv_local_var_devname), __ARCH_ACTV_RPC_PASS_ARG(dev_compat, __actv_local_var_dev_compat), __ARCH_ACTV_RPC_PASS_ARG(parent_devid, __actv_local_var_parent_devid), __ARCH_ACTV_RPC_PASS_ARG(resource_count, __actv_local_var_resource_count));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_core_unload_driver(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_core_unload_driver(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_core_unload_driver(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf drvname)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_drvname = drvname;
	__ret = ____actvcall_drvcall_devhost_core_unload_driver(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(drvname, __actv_local_var_drvname));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_core_unload_driver(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf drvname)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_drvname = drvname;
	__ret = ____actvcapcall_drvcall_devhost_core_unload_driver(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(drvname, __actv_local_var_drvname));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_core_bind_device(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_core_bind_device(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_core_bind_device(unsigned long long __rpc_info, void *__buf, int devid, struct hm_actv_buf dev_compat)
{
	int __ret;
	int __maybe_unused __actv_local_var_devid = devid;
	struct hm_actv_buf __maybe_unused __actv_local_var_dev_compat = dev_compat;
	__ret = ____actvcall_drvcall_devhost_core_bind_device(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devid, __actv_local_var_devid), __ARCH_ACTV_RPC_PASS_ARG(dev_compat, __actv_local_var_dev_compat));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_core_bind_device(rref_t __rref, unsigned long long __rpc_info, void *__buf, int devid, struct hm_actv_buf dev_compat)
{
	int __ret;
	int __maybe_unused __actv_local_var_devid = devid;
	struct hm_actv_buf __maybe_unused __actv_local_var_dev_compat = dev_compat;
	__ret = ____actvcapcall_drvcall_devhost_core_bind_device(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devid, __actv_local_var_devid), __ARCH_ACTV_RPC_PASS_ARG(dev_compat, __actv_local_var_dev_compat));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_core_entropy_read(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_core_entropy_read(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_core_entropy_read(unsigned long long __rpc_info, void *__buf, unsigned int num)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_num = num;
	__ret = ____actvcall_drvcall_devhost_core_entropy_read(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(num, __actv_local_var_num));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_core_entropy_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int num)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_num = num;
	__ret = ____actvcapcall_drvcall_devhost_core_entropy_read(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(num, __actv_local_var_num));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_rpc_export(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_rpc_export(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_rpc_export(unsigned long long __rpc_info, void *__buf, int devid, unsigned int cnode_idx)
{
	int __ret;
	int __maybe_unused __actv_local_var_devid = devid;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	__ret = ____actvcall_drvcall_devhost_rpc_export(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devid, __actv_local_var_devid), __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_rpc_export(rref_t __rref, unsigned long long __rpc_info, void *__buf, int devid, unsigned int cnode_idx)
{
	int __ret;
	int __maybe_unused __actv_local_var_devid = devid;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	__ret = ____actvcapcall_drvcall_devhost_rpc_export(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devid, __actv_local_var_devid), __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_rpc_invoke(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_rpc_invoke(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_rpc_invoke(unsigned long long __rpc_info, void *__buf, int devid, int cmd, unsigned long req_ubuf, unsigned long req_size, unsigned long resp_ubuf, unsigned long resp_size)
{
	int __ret;
	int __maybe_unused __actv_local_var_devid = devid;
	int __maybe_unused __actv_local_var_cmd = cmd;
	unsigned long __maybe_unused __actv_local_var_req_ubuf = req_ubuf;
	unsigned long __maybe_unused __actv_local_var_req_size = req_size;
	unsigned long __maybe_unused __actv_local_var_resp_ubuf = resp_ubuf;
	unsigned long __maybe_unused __actv_local_var_resp_size = resp_size;
	__ret = ____actvcall_drvcall_devhost_rpc_invoke(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devid, __actv_local_var_devid), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(req_ubuf, __actv_local_var_req_ubuf), __ARCH_ACTV_RPC_PASS_ARG(req_size, __actv_local_var_req_size), __ARCH_ACTV_RPC_PASS_ARG(resp_ubuf, __actv_local_var_resp_ubuf), __ARCH_ACTV_RPC_PASS_ARG(resp_size, __actv_local_var_resp_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_rpc_invoke(rref_t __rref, unsigned long long __rpc_info, void *__buf, int devid, int cmd, unsigned long req_ubuf, unsigned long req_size, unsigned long resp_ubuf, unsigned long resp_size)
{
	int __ret;
	int __maybe_unused __actv_local_var_devid = devid;
	int __maybe_unused __actv_local_var_cmd = cmd;
	unsigned long __maybe_unused __actv_local_var_req_ubuf = req_ubuf;
	unsigned long __maybe_unused __actv_local_var_req_size = req_size;
	unsigned long __maybe_unused __actv_local_var_resp_ubuf = resp_ubuf;
	unsigned long __maybe_unused __actv_local_var_resp_size = resp_size;
	__ret = ____actvcapcall_drvcall_devhost_rpc_invoke(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devid, __actv_local_var_devid), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(req_ubuf, __actv_local_var_req_ubuf), __ARCH_ACTV_RPC_PASS_ARG(req_size, __actv_local_var_req_size), __ARCH_ACTV_RPC_PASS_ARG(resp_ubuf, __actv_local_var_resp_ubuf), __ARCH_ACTV_RPC_PASS_ARG(resp_size, __actv_local_var_resp_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_open(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_open(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_open(unsigned long long __rpc_info, void *__buf, unsigned int u_cidx, int devid, rref_t tgt_rref, unsigned int vfs_flags)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_u_cidx = u_cidx;
	int __maybe_unused __actv_local_var_devid = devid;
	rref_t __maybe_unused __actv_local_var_tgt_rref = tgt_rref;
	unsigned int __maybe_unused __actv_local_var_vfs_flags = vfs_flags;
	__ret = ____actvcall_drvcall_devhost_fops_open(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(u_cidx, __actv_local_var_u_cidx), __ARCH_ACTV_RPC_PASS_ARG(devid, __actv_local_var_devid), __ARCH_ACTV_RPC_PASS_ARG(tgt_rref, __actv_local_var_tgt_rref), __ARCH_ACTV_RPC_PASS_ARG(vfs_flags, __actv_local_var_vfs_flags));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int u_cidx, int devid, rref_t tgt_rref, unsigned int vfs_flags)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_u_cidx = u_cidx;
	int __maybe_unused __actv_local_var_devid = devid;
	rref_t __maybe_unused __actv_local_var_tgt_rref = tgt_rref;
	unsigned int __maybe_unused __actv_local_var_vfs_flags = vfs_flags;
	__ret = ____actvcapcall_drvcall_devhost_fops_open(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(u_cidx, __actv_local_var_u_cidx), __ARCH_ACTV_RPC_PASS_ARG(devid, __actv_local_var_devid), __ARCH_ACTV_RPC_PASS_ARG(tgt_rref, __actv_local_var_tgt_rref), __ARCH_ACTV_RPC_PASS_ARG(vfs_flags, __actv_local_var_vfs_flags));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_close(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_close(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_close(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcall_drvcall_devhost_fops_close(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcapcall_drvcall_devhost_fops_close(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_read(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_read(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_read(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long buf, unsigned long long filep, unsigned long long offset, unsigned long count, unsigned int flags)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long __maybe_unused __actv_local_var_buf = buf;
	unsigned long long __maybe_unused __actv_local_var_filep = filep;
	unsigned long long __maybe_unused __actv_local_var_offset = offset;
	unsigned long __maybe_unused __actv_local_var_count = count;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcall_drvcall_devhost_fops_read(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(filep, __actv_local_var_filep), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(count, __actv_local_var_count), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long buf, unsigned long long filep, unsigned long long offset, unsigned long count, unsigned int flags)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long __maybe_unused __actv_local_var_buf = buf;
	unsigned long long __maybe_unused __actv_local_var_filep = filep;
	unsigned long long __maybe_unused __actv_local_var_offset = offset;
	unsigned long __maybe_unused __actv_local_var_count = count;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcapcall_drvcall_devhost_fops_read(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(filep, __actv_local_var_filep), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(count, __actv_local_var_count), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_write(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_write(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_write(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long buf, unsigned long long file_id, unsigned long long offset, unsigned long count, unsigned int flags)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long __maybe_unused __actv_local_var_buf = buf;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long long __maybe_unused __actv_local_var_offset = offset;
	unsigned long __maybe_unused __actv_local_var_count = count;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcall_drvcall_devhost_fops_write(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(count, __actv_local_var_count), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_write(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long buf, unsigned long long file_id, unsigned long long offset, unsigned long count, unsigned int flags)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long __maybe_unused __actv_local_var_buf = buf;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long long __maybe_unused __actv_local_var_offset = offset;
	unsigned long __maybe_unused __actv_local_var_count = count;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcapcall_drvcall_devhost_fops_write(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(count, __actv_local_var_count), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_fallback"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_read_iter(unsigned long long rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, ...);


static int __attribute__((alias("__internal_rpccapcall_fallback"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_read_iter(rref_t __rref, unsigned long long rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_read_iter(unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flag)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long long __maybe_unused __actv_local_var_offset = offset;
	__maybe_unused const void *__actv_local_var_vec = vec;
	int __maybe_unused __actv_local_var_nr_segs = nr_segs;
	size_t __maybe_unused __actv_local_var_count = count;
	unsigned int __maybe_unused __actv_local_var_flag = flag;
	__ret = ____actvcall_drvcall_devhost_fops_read_iter(__rpc_info, __buf, __rply_buf, rply_buf_sz, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(vec, __actv_local_var_vec), __ARCH_ACTV_RPC_PASS_ARG(nr_segs, __actv_local_var_nr_segs), __ARCH_ACTV_RPC_PASS_ARG(count, __actv_local_var_count), __ARCH_ACTV_RPC_PASS_ARG(flag, __actv_local_var_flag));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_read_iter(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flag)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long long __maybe_unused __actv_local_var_offset = offset;
	__maybe_unused const void *__actv_local_var_vec = vec;
	int __maybe_unused __actv_local_var_nr_segs = nr_segs;
	size_t __maybe_unused __actv_local_var_count = count;
	unsigned int __maybe_unused __actv_local_var_flag = flag;
	__ret = ____actvcapcall_drvcall_devhost_fops_read_iter(__rref, __rpc_info, __buf, __rply_buf, rply_buf_sz, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(vec, __actv_local_var_vec), __ARCH_ACTV_RPC_PASS_ARG(nr_segs, __actv_local_var_nr_segs), __ARCH_ACTV_RPC_PASS_ARG(count, __actv_local_var_count), __ARCH_ACTV_RPC_PASS_ARG(flag, __actv_local_var_flag));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_fallback"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_write_iter(unsigned long long rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, ...);


static int __attribute__((alias("__internal_rpccapcall_fallback"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_write_iter(rref_t __rref, unsigned long long rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_write_iter(unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flags)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long long __maybe_unused __actv_local_var_offset = offset;
	__maybe_unused const void *__actv_local_var_vec = vec;
	int __maybe_unused __actv_local_var_nr_segs = nr_segs;
	size_t __maybe_unused __actv_local_var_count = count;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcall_drvcall_devhost_fops_write_iter(__rpc_info, __buf, __rply_buf, rply_buf_sz, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(vec, __actv_local_var_vec), __ARCH_ACTV_RPC_PASS_ARG(nr_segs, __actv_local_var_nr_segs), __ARCH_ACTV_RPC_PASS_ARG(count, __actv_local_var_count), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_write_iter(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, unsigned int fastpath, unsigned long long file_id, unsigned long long offset, const void *vec, int nr_segs, size_t count, unsigned int flags)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long long __maybe_unused __actv_local_var_offset = offset;
	__maybe_unused const void *__actv_local_var_vec = vec;
	int __maybe_unused __actv_local_var_nr_segs = nr_segs;
	size_t __maybe_unused __actv_local_var_count = count;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcapcall_drvcall_devhost_fops_write_iter(__rref, __rpc_info, __buf, __rply_buf, rply_buf_sz, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(vec, __actv_local_var_vec), __ARCH_ACTV_RPC_PASS_ARG(nr_segs, __actv_local_var_nr_segs), __ARCH_ACTV_RPC_PASS_ARG(count, __actv_local_var_count), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_ioctl(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_ioctl(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_ioctl(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	int __maybe_unused __actv_local_var_request = request;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcall_drvcall_devhost_fops_ioctl(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(request, __actv_local_var_request), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	int __maybe_unused __actv_local_var_request = request;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcapcall_drvcall_devhost_fops_ioctl(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(request, __actv_local_var_request), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_flush(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_flush(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_flush(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcall_drvcall_devhost_fops_flush(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_flush(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcapcall_drvcall_devhost_fops_flush(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_fcntl(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_fcntl(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_fcntl(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	int __maybe_unused __actv_local_var_request = request;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcall_drvcall_devhost_fops_fcntl(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(request, __actv_local_var_request), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_fcntl(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, int request, unsigned long long file_id, unsigned long arg)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	int __maybe_unused __actv_local_var_request = request;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcapcall_drvcall_devhost_fops_fcntl(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(request, __actv_local_var_request), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_llseek(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_llseek(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_llseek(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, int whence, unsigned long long file_id, long long offset)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	int __maybe_unused __actv_local_var_whence = whence;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	long long __maybe_unused __actv_local_var_offset = offset;
	__ret = ____actvcall_drvcall_devhost_fops_llseek(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(whence, __actv_local_var_whence), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_llseek(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, int whence, unsigned long long file_id, long long offset)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	int __maybe_unused __actv_local_var_whence = whence;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	long long __maybe_unused __actv_local_var_offset = offset;
	__ret = ____actvcapcall_drvcall_devhost_fops_llseek(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(whence, __actv_local_var_whence), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_mmap(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_mmap(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_mmap(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long addr, unsigned long long file_id, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset, bool iomap_prepare_ahead)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long __maybe_unused __actv_local_var_addr = addr;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long __maybe_unused __actv_local_var_length = length;
	unsigned long __maybe_unused __actv_local_var_prot = prot;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned long __maybe_unused __actv_local_var_offset = offset;
	bool __maybe_unused __actv_local_var_iomap_prepare_ahead = iomap_prepare_ahead;
	__ret = ____actvcall_drvcall_devhost_fops_mmap(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(length, __actv_local_var_length), __ARCH_ACTV_RPC_PASS_ARG(prot, __actv_local_var_prot), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(iomap_prepare_ahead, __actv_local_var_iomap_prepare_ahead));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_mmap(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long addr, unsigned long long file_id, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset, bool iomap_prepare_ahead)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long __maybe_unused __actv_local_var_addr = addr;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long __maybe_unused __actv_local_var_length = length;
	unsigned long __maybe_unused __actv_local_var_prot = prot;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned long __maybe_unused __actv_local_var_offset = offset;
	bool __maybe_unused __actv_local_var_iomap_prepare_ahead = iomap_prepare_ahead;
	__ret = ____actvcapcall_drvcall_devhost_fops_mmap(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(length, __actv_local_var_length), __ARCH_ACTV_RPC_PASS_ARG(prot, __actv_local_var_prot), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(iomap_prepare_ahead, __actv_local_var_iomap_prepare_ahead));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_munmap(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_munmap(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_munmap(unsigned long long __rpc_info, void *__buf, unsigned int key, unsigned long start, unsigned long len, int vs_is_releasing)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_key = key;
	unsigned long __maybe_unused __actv_local_var_start = start;
	unsigned long __maybe_unused __actv_local_var_len = len;
	int __maybe_unused __actv_local_var_vs_is_releasing = vs_is_releasing;
	__ret = ____actvcall_drvcall_devhost_fops_munmap(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(key, __actv_local_var_key), __ARCH_ACTV_RPC_PASS_ARG(start, __actv_local_var_start), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(vs_is_releasing, __actv_local_var_vs_is_releasing));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_munmap(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int key, unsigned long start, unsigned long len, int vs_is_releasing)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_key = key;
	unsigned long __maybe_unused __actv_local_var_start = start;
	unsigned long __maybe_unused __actv_local_var_len = len;
	int __maybe_unused __actv_local_var_vs_is_releasing = vs_is_releasing;
	__ret = ____actvcapcall_drvcall_devhost_fops_munmap(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(key, __actv_local_var_key), __ARCH_ACTV_RPC_PASS_ARG(start, __actv_local_var_start), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(vs_is_releasing, __actv_local_var_vs_is_releasing));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_do_page_fault(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_do_page_fault(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_do_page_fault(unsigned long long __rpc_info, void *__buf, unsigned int vma_key, unsigned long long vr_key, unsigned long long addr, unsigned int flags, unsigned long long offset)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_vma_key = vma_key;
	unsigned long long __maybe_unused __actv_local_var_vr_key = vr_key;
	unsigned long long __maybe_unused __actv_local_var_addr = addr;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned long long __maybe_unused __actv_local_var_offset = offset;
	__ret = ____actvcall_drvcall_devhost_fops_do_page_fault(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(vma_key, __actv_local_var_vma_key), __ARCH_ACTV_RPC_PASS_ARG(vr_key, __actv_local_var_vr_key), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_do_page_fault(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int vma_key, unsigned long long vr_key, unsigned long long addr, unsigned int flags, unsigned long long offset)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_vma_key = vma_key;
	unsigned long long __maybe_unused __actv_local_var_vr_key = vr_key;
	unsigned long long __maybe_unused __actv_local_var_addr = addr;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned long long __maybe_unused __actv_local_var_offset = offset;
	__ret = ____actvcapcall_drvcall_devhost_fops_do_page_fault(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(vma_key, __actv_local_var_vma_key), __ARCH_ACTV_RPC_PASS_ARG(vr_key, __actv_local_var_vr_key), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_mm_release(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_mm_release(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_mm_release(unsigned long long __rpc_info, void *__buf, unsigned long long vspace_key)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_vspace_key = vspace_key;
	__ret = ____actvcall_drvcall_devhost_fops_mm_release(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(vspace_key, __actv_local_var_vspace_key));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_mm_release(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long vspace_key)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_vspace_key = vspace_key;
	__ret = ____actvcapcall_drvcall_devhost_fops_mm_release(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(vspace_key, __actv_local_var_vspace_key));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_sysfs_read(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_sysfs_read(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_sysfs_read(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devhost_fops_sysfs_read(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_sysfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devhost_fops_sysfs_read(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_sysfs_write(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_sysfs_write(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_sysfs_write(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devhost_fops_sysfs_write(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_sysfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devhost_fops_sysfs_write(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_poll(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_poll(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_poll(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, unsigned long long tcb_key, rref_t fdtable_rref, unsigned int events, int idx, int fd, int need_add)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long long __maybe_unused __actv_local_var_tcb_key = tcb_key;
	rref_t __maybe_unused __actv_local_var_fdtable_rref = fdtable_rref;
	unsigned int __maybe_unused __actv_local_var_events = events;
	int __maybe_unused __actv_local_var_idx = idx;
	int __maybe_unused __actv_local_var_fd = fd;
	int __maybe_unused __actv_local_var_need_add = need_add;
	__ret = ____actvcall_drvcall_devhost_fops_poll(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(tcb_key, __actv_local_var_tcb_key), __ARCH_ACTV_RPC_PASS_ARG(fdtable_rref, __actv_local_var_fdtable_rref), __ARCH_ACTV_RPC_PASS_ARG(events, __actv_local_var_events), __ARCH_ACTV_RPC_PASS_ARG(idx, __actv_local_var_idx), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(need_add, __actv_local_var_need_add));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_poll(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, unsigned long long tcb_key, rref_t fdtable_rref, unsigned int events, int idx, int fd, int need_add)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long long __maybe_unused __actv_local_var_tcb_key = tcb_key;
	rref_t __maybe_unused __actv_local_var_fdtable_rref = fdtable_rref;
	unsigned int __maybe_unused __actv_local_var_events = events;
	int __maybe_unused __actv_local_var_idx = idx;
	int __maybe_unused __actv_local_var_fd = fd;
	int __maybe_unused __actv_local_var_need_add = need_add;
	__ret = ____actvcapcall_drvcall_devhost_fops_poll(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(tcb_key, __actv_local_var_tcb_key), __ARCH_ACTV_RPC_PASS_ARG(fdtable_rref, __actv_local_var_fdtable_rref), __ARCH_ACTV_RPC_PASS_ARG(events, __actv_local_var_events), __ARCH_ACTV_RPC_PASS_ARG(idx, __actv_local_var_idx), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(need_add, __actv_local_var_need_add));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_poll_remove(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_poll_remove(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_poll_remove(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, int fd, cref_t tcb_cref)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	int __maybe_unused __actv_local_var_fd = fd;
	cref_t __maybe_unused __actv_local_var_tcb_cref = tcb_cref;
	__ret = ____actvcall_drvcall_devhost_fops_poll_remove(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(tcb_cref, __actv_local_var_tcb_cref));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_poll_remove(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, int fd, cref_t tcb_cref)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	int __maybe_unused __actv_local_var_fd = fd;
	cref_t __maybe_unused __actv_local_var_tcb_cref = tcb_cref;
	__ret = ____actvcapcall_drvcall_devhost_fops_poll_remove(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(tcb_cref, __actv_local_var_tcb_cref));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_epoll_ctl(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_epoll_ctl(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_epoll_ctl(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, const void *ep_arg)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__maybe_unused const void *__actv_local_var_ep_arg = ep_arg;
	__ret = ____actvcall_drvcall_devhost_fops_epoll_ctl(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(ep_arg, __actv_local_var_ep_arg));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_epoll_ctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, const void *ep_arg)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__maybe_unused const void *__actv_local_var_ep_arg = ep_arg;
	__ret = ____actvcapcall_drvcall_devhost_fops_epoll_ctl(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(ep_arg, __actv_local_var_ep_arg));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_epoll_remove(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_epoll_remove(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_epoll_remove(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, cref_t ep_cref)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	cref_t __maybe_unused __actv_local_var_ep_cref = ep_cref;
	__ret = ____actvcall_drvcall_devhost_fops_epoll_remove(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(ep_cref, __actv_local_var_ep_cref));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_epoll_remove(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, cref_t ep_cref)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	cref_t __maybe_unused __actv_local_var_ep_cref = ep_cref;
	__ret = ____actvcapcall_drvcall_devhost_fops_epoll_remove(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(ep_cref, __actv_local_var_ep_cref));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_epoll_recheck(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_epoll_recheck(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_epoll_recheck(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, unsigned int events)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned int __maybe_unused __actv_local_var_events = events;
	__ret = ____actvcall_drvcall_devhost_fops_epoll_recheck(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(events, __actv_local_var_events));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_epoll_recheck(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id, unsigned int events)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned int __maybe_unused __actv_local_var_events = events;
	__ret = ____actvcapcall_drvcall_devhost_fops_epoll_recheck(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(events, __actv_local_var_events));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_sync_ctrlmem_polllist(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_sync_ctrlmem_polllist(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_sync_ctrlmem_polllist(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcall_drvcall_devhost_fops_sync_ctrlmem_polllist(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_sync_ctrlmem_polllist(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcapcall_drvcall_devhost_fops_sync_ctrlmem_polllist(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fops_file_get(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fops_file_get(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fops_file_get(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcall_drvcall_devhost_fops_file_get(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fops_file_get(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcapcall_drvcall_devhost_fops_file_get(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_block_check_feature(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_block_check_feature(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_block_check_feature(unsigned long long __rpc_info, void *__buf, int idx, unsigned long long file_id, unsigned long long features)
{
	int __ret;
	int __maybe_unused __actv_local_var_idx = idx;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long long __maybe_unused __actv_local_var_features = features;
	__ret = ____actvcall_drvcall_devhost_block_check_feature(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(idx, __actv_local_var_idx), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(features, __actv_local_var_features));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_block_check_feature(rref_t __rref, unsigned long long __rpc_info, void *__buf, int idx, unsigned long long file_id, unsigned long long features)
{
	int __ret;
	int __maybe_unused __actv_local_var_idx = idx;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long long __maybe_unused __actv_local_var_features = features;
	__ret = ____actvcapcall_drvcall_devhost_block_check_feature(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(idx, __actv_local_var_idx), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(features, __actv_local_var_features));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_drvcall_devhost_block_submit_bio(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_block_submit_bio(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_block_submit_bio(unsigned long long __rpc_info, void *__buf, unsigned long long file_id, unsigned long long rw, unsigned long long sector, int vec_cnt, unsigned int priority, unsigned long long bi_crypt_ctx)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long long __maybe_unused __actv_local_var_rw = rw;
	unsigned long long __maybe_unused __actv_local_var_sector = sector;
	int __maybe_unused __actv_local_var_vec_cnt = vec_cnt;
	unsigned int __maybe_unused __actv_local_var_priority = priority;
	unsigned long long __maybe_unused __actv_local_var_bi_crypt_ctx = bi_crypt_ctx;
	__ret = ____actvcall_drvcall_devhost_block_submit_bio(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(rw, __actv_local_var_rw), __ARCH_ACTV_RPC_PASS_ARG(sector, __actv_local_var_sector), __ARCH_ACTV_RPC_PASS_ARG(vec_cnt, __actv_local_var_vec_cnt), __ARCH_ACTV_RPC_PASS_ARG(priority, __actv_local_var_priority), __ARCH_ACTV_RPC_PASS_ARG(bi_crypt_ctx, __actv_local_var_bi_crypt_ctx));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_block_submit_bio(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long file_id, unsigned long long rw, unsigned long long sector, int vec_cnt, unsigned int priority, unsigned long long bi_crypt_ctx)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long long __maybe_unused __actv_local_var_rw = rw;
	unsigned long long __maybe_unused __actv_local_var_sector = sector;
	int __maybe_unused __actv_local_var_vec_cnt = vec_cnt;
	unsigned int __maybe_unused __actv_local_var_priority = priority;
	unsigned long long __maybe_unused __actv_local_var_bi_crypt_ctx = bi_crypt_ctx;
	__ret = ____actvcapcall_drvcall_devhost_block_submit_bio(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(rw, __actv_local_var_rw), __ARCH_ACTV_RPC_PASS_ARG(sector, __actv_local_var_sector), __ARCH_ACTV_RPC_PASS_ARG(vec_cnt, __actv_local_var_vec_cnt), __ARCH_ACTV_RPC_PASS_ARG(priority, __actv_local_var_priority), __ARCH_ACTV_RPC_PASS_ARG(bi_crypt_ctx, __actv_local_var_bi_crypt_ctx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_block_submit_cmd(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_block_submit_cmd(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_block_submit_cmd(unsigned long long __rpc_info, void *__buf, unsigned long long file_id, unsigned long long cmd, unsigned long long sector, unsigned int size, unsigned int priority)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long long __maybe_unused __actv_local_var_cmd = cmd;
	unsigned long long __maybe_unused __actv_local_var_sector = sector;
	unsigned int __maybe_unused __actv_local_var_size = size;
	unsigned int __maybe_unused __actv_local_var_priority = priority;
	__ret = ____actvcall_drvcall_devhost_block_submit_cmd(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(sector, __actv_local_var_sector), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size), __ARCH_ACTV_RPC_PASS_ARG(priority, __actv_local_var_priority));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_block_submit_cmd(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long file_id, unsigned long long cmd, unsigned long long sector, unsigned int size, unsigned int priority)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long long __maybe_unused __actv_local_var_cmd = cmd;
	unsigned long long __maybe_unused __actv_local_var_sector = sector;
	unsigned int __maybe_unused __actv_local_var_size = size;
	unsigned int __maybe_unused __actv_local_var_priority = priority;
	__ret = ____actvcapcall_drvcall_devhost_block_submit_cmd(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(sector, __actv_local_var_sector), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size), __ARCH_ACTV_RPC_PASS_ARG(priority, __actv_local_var_priority));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_block_run_queue(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_block_run_queue(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_block_run_queue(unsigned long long __rpc_info, void *__buf, unsigned long file_id, unsigned int flags, unsigned int idx)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned int __maybe_unused __actv_local_var_idx = idx;
	__ret = ____actvcall_drvcall_devhost_block_run_queue(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(idx, __actv_local_var_idx));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_block_run_queue(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long file_id, unsigned int flags, unsigned int idx)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned int __maybe_unused __actv_local_var_idx = idx;
	__ret = ____actvcapcall_drvcall_devhost_block_run_queue(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(idx, __actv_local_var_idx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_block_direct_access(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_block_direct_access(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_block_direct_access(unsigned long long __rpc_info, void *__buf, unsigned long file_id, unsigned long offset, unsigned long size)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long __maybe_unused __actv_local_var_offset = offset;
	unsigned long __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcall_drvcall_devhost_block_direct_access(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_block_direct_access(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long file_id, unsigned long offset, unsigned long size)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_file_id = file_id;
	unsigned long __maybe_unused __actv_local_var_offset = offset;
	unsigned long __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcapcall_drvcall_devhost_block_direct_access(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_block_register_bio_notify(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_block_register_bio_notify(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_block_register_bio_notify(unsigned long long __rpc_info, void *__buf, unsigned long file_id, uref_t uref, unsigned int method, size_t bd_shm_size)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_file_id = file_id;
	uref_t __maybe_unused __actv_local_var_uref = uref;
	unsigned int __maybe_unused __actv_local_var_method = method;
	size_t __maybe_unused __actv_local_var_bd_shm_size = bd_shm_size;
	__ret = ____actvcall_drvcall_devhost_block_register_bio_notify(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(uref, __actv_local_var_uref), __ARCH_ACTV_RPC_PASS_ARG(method, __actv_local_var_method), __ARCH_ACTV_RPC_PASS_ARG(bd_shm_size, __actv_local_var_bd_shm_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_block_register_bio_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long file_id, uref_t uref, unsigned int method, size_t bd_shm_size)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_file_id = file_id;
	uref_t __maybe_unused __actv_local_var_uref = uref;
	unsigned int __maybe_unused __actv_local_var_method = method;
	size_t __maybe_unused __actv_local_var_bd_shm_size = bd_shm_size;
	__ret = ____actvcapcall_drvcall_devhost_block_register_bio_notify(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id), __ARCH_ACTV_RPC_PASS_ARG(uref, __actv_local_var_uref), __ARCH_ACTV_RPC_PASS_ARG(method, __actv_local_var_method), __ARCH_ACTV_RPC_PASS_ARG(bd_shm_size, __actv_local_var_bd_shm_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_send(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_send(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_send(unsigned long long __rpc_info, void *__buf, int ifindex, unsigned int offset, unsigned int size, unsigned int payload_len, unsigned long flags)
{
	int __ret;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	unsigned int __maybe_unused __actv_local_var_offset = offset;
	unsigned int __maybe_unused __actv_local_var_size = size;
	unsigned int __maybe_unused __actv_local_var_payload_len = payload_len;
	unsigned long __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcall_drvcall_devhost_net_send(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size), __ARCH_ACTV_RPC_PASS_ARG(payload_len, __actv_local_var_payload_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_send(rref_t __rref, unsigned long long __rpc_info, void *__buf, int ifindex, unsigned int offset, unsigned int size, unsigned int payload_len, unsigned long flags)
{
	int __ret;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	unsigned int __maybe_unused __actv_local_var_offset = offset;
	unsigned int __maybe_unused __actv_local_var_size = size;
	unsigned int __maybe_unused __actv_local_var_payload_len = payload_len;
	unsigned long __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcapcall_drvcall_devhost_net_send(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size), __ARCH_ACTV_RPC_PASS_ARG(payload_len, __actv_local_var_payload_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_ioctl(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_ioctl(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_ioctl(unsigned long long __rpc_info, void *__buf, int ifindex, int request, unsigned long args)
{
	int __ret;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	int __maybe_unused __actv_local_var_request = request;
	unsigned long __maybe_unused __actv_local_var_args = args;
	__ret = ____actvcall_drvcall_devhost_net_ioctl(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex), __ARCH_ACTV_RPC_PASS_ARG(request, __actv_local_var_request), __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, int ifindex, int request, unsigned long args)
{
	int __ret;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	int __maybe_unused __actv_local_var_request = request;
	unsigned long __maybe_unused __actv_local_var_args = args;
	__ret = ____actvcapcall_drvcall_devhost_net_ioctl(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex), __ARCH_ACTV_RPC_PASS_ARG(request, __actv_local_var_request), __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_status(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_status(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_status(unsigned long long __rpc_info, void *__buf, int ifindex)
{
	int __ret;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	__ret = ____actvcall_drvcall_devhost_net_status(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_status(rref_t __rref, unsigned long long __rpc_info, void *__buf, int ifindex)
{
	int __ret;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	__ret = ____actvcapcall_drvcall_devhost_net_status(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_prepare(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_prepare(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_prepare(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devhost_net_prepare(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_prepare(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devhost_net_prepare(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_init(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_init(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_init(unsigned long long __rpc_info, void *__buf, int ifindex, rref_t tgt_rref)
{
	int __ret;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	rref_t __maybe_unused __actv_local_var_tgt_rref = tgt_rref;
	__ret = ____actvcall_drvcall_devhost_net_init(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex), __ARCH_ACTV_RPC_PASS_ARG(tgt_rref, __actv_local_var_tgt_rref));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_init(rref_t __rref, unsigned long long __rpc_info, void *__buf, int ifindex, rref_t tgt_rref)
{
	int __ret;
	int __maybe_unused __actv_local_var_ifindex = ifindex;
	rref_t __maybe_unused __actv_local_var_tgt_rref = tgt_rref;
	__ret = ____actvcapcall_drvcall_devhost_net_init(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ifindex, __actv_local_var_ifindex), __ARCH_ACTV_RPC_PASS_ARG(tgt_rref, __actv_local_var_tgt_rref));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_netlink_recv(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_netlink_recv(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_netlink_recv(unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned long msg_buff, unsigned int msg_len)
{
	int __ret;
	int __maybe_unused __actv_local_var_protocol = protocol;
	unsigned int __maybe_unused __actv_local_var_src_portid = src_portid;
	unsigned long __maybe_unused __actv_local_var_msg_buff = msg_buff;
	unsigned int __maybe_unused __actv_local_var_msg_len = msg_len;
	__ret = ____actvcall_drvcall_devhost_net_netlink_recv(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(protocol, __actv_local_var_protocol), __ARCH_ACTV_RPC_PASS_ARG(src_portid, __actv_local_var_src_portid), __ARCH_ACTV_RPC_PASS_ARG(msg_buff, __actv_local_var_msg_buff), __ARCH_ACTV_RPC_PASS_ARG(msg_len, __actv_local_var_msg_len));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_netlink_recv(rref_t __rref, unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned long msg_buff, unsigned int msg_len)
{
	int __ret;
	int __maybe_unused __actv_local_var_protocol = protocol;
	unsigned int __maybe_unused __actv_local_var_src_portid = src_portid;
	unsigned long __maybe_unused __actv_local_var_msg_buff = msg_buff;
	unsigned int __maybe_unused __actv_local_var_msg_len = msg_len;
	__ret = ____actvcapcall_drvcall_devhost_net_netlink_recv(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(protocol, __actv_local_var_protocol), __ARCH_ACTV_RPC_PASS_ARG(src_portid, __actv_local_var_src_portid), __ARCH_ACTV_RPC_PASS_ARG(msg_buff, __actv_local_var_msg_buff), __ARCH_ACTV_RPC_PASS_ARG(msg_len, __actv_local_var_msg_len));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_register(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_register(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_register(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devhost_net_socket_register(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_register(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_register(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_close(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_close(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_close(unsigned long long __rpc_info, void *__buf, int sockfd)
{
	int __ret;
	int __maybe_unused __actv_local_var_sockfd = sockfd;
	__ret = ____actvcall_drvcall_devhost_net_socket_close(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, int sockfd)
{
	int __ret;
	int __maybe_unused __actv_local_var_sockfd = sockfd;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_close(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_connect(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_connect(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_connect(unsigned long long __rpc_info, void *__buf, long sockfd, const struct sockaddr *addr, unsigned int addrlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const struct sockaddr *__actv_local_var_addr = addr;
	unsigned int __maybe_unused __actv_local_var_addrlen = addrlen;
	__ret = ____actvcall_drvcall_devhost_net_socket_connect(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(addrlen, __actv_local_var_addrlen));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_connect(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const struct sockaddr *addr, unsigned int addrlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const struct sockaddr *__actv_local_var_addr = addr;
	unsigned int __maybe_unused __actv_local_var_addrlen = addrlen;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_connect(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(addrlen, __actv_local_var_addrlen));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_bind(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_bind(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_bind(unsigned long long __rpc_info, void *__buf, long sockfd, const struct sockaddr *addr, unsigned int addrlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const struct sockaddr *__actv_local_var_addr = addr;
	unsigned int __maybe_unused __actv_local_var_addrlen = addrlen;
	__ret = ____actvcall_drvcall_devhost_net_socket_bind(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(addrlen, __actv_local_var_addrlen));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_bind(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const struct sockaddr *addr, unsigned int addrlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const struct sockaddr *__actv_local_var_addr = addr;
	unsigned int __maybe_unused __actv_local_var_addrlen = addrlen;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_bind(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(addrlen, __actv_local_var_addrlen));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_listen(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_listen(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_listen(unsigned long long __rpc_info, void *__buf, long sockfd, int backlog)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	int __maybe_unused __actv_local_var_backlog = backlog;
	__ret = ____actvcall_drvcall_devhost_net_socket_listen(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(backlog, __actv_local_var_backlog));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_listen(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, int backlog)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	int __maybe_unused __actv_local_var_backlog = backlog;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_listen(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(backlog, __actv_local_var_backlog));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_accept(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_accept(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_accept(unsigned long long __rpc_info, void *__buf, long sockfd, struct sockaddr *addr, unsigned int *addrlen, int flags, unsigned long ino)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused struct sockaddr *__actv_local_var_addr = addr;
	__maybe_unused unsigned int *__actv_local_var_addrlen = addrlen;
	int __maybe_unused __actv_local_var_flags = flags;
	unsigned long __maybe_unused __actv_local_var_ino = ino;
	__ret = ____actvcall_drvcall_devhost_net_socket_accept(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(addrlen, __actv_local_var_addrlen), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_accept(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, struct sockaddr *addr, unsigned int *addrlen, int flags, unsigned long ino)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused struct sockaddr *__actv_local_var_addr = addr;
	__maybe_unused unsigned int *__actv_local_var_addrlen = addrlen;
	int __maybe_unused __actv_local_var_flags = flags;
	unsigned long __maybe_unused __actv_local_var_ino = ino;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_accept(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(addrlen, __actv_local_var_addrlen), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_shutdown(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_shutdown(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_shutdown(unsigned long long __rpc_info, void *__buf, long sockfd, int how)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	int __maybe_unused __actv_local_var_how = how;
	__ret = ____actvcall_drvcall_devhost_net_socket_shutdown(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(how, __actv_local_var_how));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_shutdown(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, int how)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	int __maybe_unused __actv_local_var_how = how;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_shutdown(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(how, __actv_local_var_how));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_ioctl(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_ioctl(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_ioctl(unsigned long long __rpc_info, void *__buf, long sockfd, int cmd, void *arg_buf)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	int __maybe_unused __actv_local_var_cmd = cmd;
	__maybe_unused void *__actv_local_var_arg_buf = arg_buf;
	__ret = ____actvcall_drvcall_devhost_net_socket_ioctl(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(arg_buf, __actv_local_var_arg_buf));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, int cmd, void *arg_buf)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	int __maybe_unused __actv_local_var_cmd = cmd;
	__maybe_unused void *__actv_local_var_arg_buf = arg_buf;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_ioctl(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(arg_buf, __actv_local_var_arg_buf));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_dev_ioctl(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_dev_ioctl(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_dev_ioctl(unsigned long long __rpc_info, void *__buf, int cmd, void *arg_buf)
{
	int __ret;
	int __maybe_unused __actv_local_var_cmd = cmd;
	__maybe_unused void *__actv_local_var_arg_buf = arg_buf;
	__ret = ____actvcall_drvcall_devhost_net_socket_dev_ioctl(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(arg_buf, __actv_local_var_arg_buf));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_dev_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, int cmd, void *arg_buf)
{
	int __ret;
	int __maybe_unused __actv_local_var_cmd = cmd;
	__maybe_unused void *__actv_local_var_arg_buf = arg_buf;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_dev_ioctl(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(arg_buf, __actv_local_var_arg_buf));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_fcntl(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_fcntl(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_fcntl(unsigned long long __rpc_info, void *__buf, long sockfd, int cmd, int arg)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	int __maybe_unused __actv_local_var_cmd = cmd;
	int __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcall_drvcall_devhost_net_socket_fcntl(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_fcntl(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, int cmd, int arg)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	int __maybe_unused __actv_local_var_cmd = cmd;
	int __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_fcntl(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_send(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_send(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_send(unsigned long long __rpc_info, void *__buf, long sockfd, const void *buf, size_t len, int flags)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_len = len;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcall_drvcall_devhost_net_socket_send(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_send(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const void *buf, size_t len, int flags)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_len = len;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_send(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_recv(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_recv(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_recv(unsigned long long __rpc_info, void *__buf, long sockfd, void *buf, size_t len, int flags)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_len = len;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcall_drvcall_devhost_net_socket_recv(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_recv(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, void *buf, size_t len, int flags)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_len = len;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_recv(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_sendmmsg(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_sendmmsg(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_sendmmsg(unsigned long long __rpc_info, void *__buf, long sockfd, const void *msgvec, size_t vlen, int flags)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const void *__actv_local_var_msgvec = msgvec;
	size_t __maybe_unused __actv_local_var_vlen = vlen;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcall_drvcall_devhost_net_socket_sendmmsg(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(msgvec, __actv_local_var_msgvec), __ARCH_ACTV_RPC_PASS_ARG(vlen, __actv_local_var_vlen), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_sendmmsg(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const void *msgvec, size_t vlen, int flags)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const void *__actv_local_var_msgvec = msgvec;
	size_t __maybe_unused __actv_local_var_vlen = vlen;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_sendmmsg(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(msgvec, __actv_local_var_msgvec), __ARCH_ACTV_RPC_PASS_ARG(vlen, __actv_local_var_vlen), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_recvmmsg(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_recvmmsg(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_recvmmsg(unsigned long long __rpc_info, void *__buf, long sockfd, const void *msgvec, size_t vlen, int flags, void *timeout)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const void *__actv_local_var_msgvec = msgvec;
	size_t __maybe_unused __actv_local_var_vlen = vlen;
	int __maybe_unused __actv_local_var_flags = flags;
	__maybe_unused void *__actv_local_var_timeout = timeout;
	__ret = ____actvcall_drvcall_devhost_net_socket_recvmmsg(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(msgvec, __actv_local_var_msgvec), __ARCH_ACTV_RPC_PASS_ARG(vlen, __actv_local_var_vlen), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(timeout, __actv_local_var_timeout));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_recvmmsg(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const void *msgvec, size_t vlen, int flags, void *timeout)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const void *__actv_local_var_msgvec = msgvec;
	size_t __maybe_unused __actv_local_var_vlen = vlen;
	int __maybe_unused __actv_local_var_flags = flags;
	__maybe_unused void *__actv_local_var_timeout = timeout;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_recvmmsg(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(msgvec, __actv_local_var_msgvec), __ARCH_ACTV_RPC_PASS_ARG(vlen, __actv_local_var_vlen), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(timeout, __actv_local_var_timeout));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_sendmsg(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_sendmsg(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_sendmsg(unsigned long long __rpc_info, void *__buf, long sockfd, const struct msghdr *msg, int flags)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const struct msghdr *__actv_local_var_msg = msg;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcall_drvcall_devhost_net_socket_sendmsg(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(msg, __actv_local_var_msg), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_sendmsg(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const struct msghdr *msg, int flags)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const struct msghdr *__actv_local_var_msg = msg;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_sendmsg(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(msg, __actv_local_var_msg), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_recvmsg(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_recvmsg(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_recvmsg(unsigned long long __rpc_info, void *__buf, long sockfd, struct msghdr *msg, int flags)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused struct msghdr *__actv_local_var_msg = msg;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcall_drvcall_devhost_net_socket_recvmsg(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(msg, __actv_local_var_msg), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_recvmsg(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, struct msghdr *msg, int flags)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused struct msghdr *__actv_local_var_msg = msg;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_recvmsg(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(msg, __actv_local_var_msg), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_sendto(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_sendto(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_sendto(unsigned long long __rpc_info, void *__buf, long sockfd, const void *buf, size_t len, int flags, const struct sockaddr *daddr, unsigned int addrlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_len = len;
	int __maybe_unused __actv_local_var_flags = flags;
	__maybe_unused const struct sockaddr *__actv_local_var_daddr = daddr;
	unsigned int __maybe_unused __actv_local_var_addrlen = addrlen;
	__ret = ____actvcall_drvcall_devhost_net_socket_sendto(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(daddr, __actv_local_var_daddr), __ARCH_ACTV_RPC_PASS_ARG(addrlen, __actv_local_var_addrlen));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_sendto(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const void *buf, size_t len, int flags, const struct sockaddr *daddr, unsigned int addrlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_len = len;
	int __maybe_unused __actv_local_var_flags = flags;
	__maybe_unused const struct sockaddr *__actv_local_var_daddr = daddr;
	unsigned int __maybe_unused __actv_local_var_addrlen = addrlen;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_sendto(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(daddr, __actv_local_var_daddr), __ARCH_ACTV_RPC_PASS_ARG(addrlen, __actv_local_var_addrlen));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_recvfrom(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_recvfrom(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_recvfrom(unsigned long long __rpc_info, void *__buf, long sockfd, void *buf, size_t len, int flags, struct sockaddr *daddr, unsigned int *addrlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_len = len;
	int __maybe_unused __actv_local_var_flags = flags;
	__maybe_unused struct sockaddr *__actv_local_var_daddr = daddr;
	__maybe_unused unsigned int *__actv_local_var_addrlen = addrlen;
	__ret = ____actvcall_drvcall_devhost_net_socket_recvfrom(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(daddr, __actv_local_var_daddr), __ARCH_ACTV_RPC_PASS_ARG(addrlen, __actv_local_var_addrlen));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_recvfrom(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, void *buf, size_t len, int flags, struct sockaddr *daddr, unsigned int *addrlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_len = len;
	int __maybe_unused __actv_local_var_flags = flags;
	__maybe_unused struct sockaddr *__actv_local_var_daddr = daddr;
	__maybe_unused unsigned int *__actv_local_var_addrlen = addrlen;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_recvfrom(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(daddr, __actv_local_var_daddr), __ARCH_ACTV_RPC_PASS_ARG(addrlen, __actv_local_var_addrlen));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_readv(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_readv(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_readv(unsigned long long __rpc_info, void *__buf, long sockfd, const struct iovec *iov, int iovcnt)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const struct iovec *__actv_local_var_iov = iov;
	int __maybe_unused __actv_local_var_iovcnt = iovcnt;
	__ret = ____actvcall_drvcall_devhost_net_socket_readv(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(iov, __actv_local_var_iov), __ARCH_ACTV_RPC_PASS_ARG(iovcnt, __actv_local_var_iovcnt));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_readv(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const struct iovec *iov, int iovcnt)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const struct iovec *__actv_local_var_iov = iov;
	int __maybe_unused __actv_local_var_iovcnt = iovcnt;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_readv(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(iov, __actv_local_var_iov), __ARCH_ACTV_RPC_PASS_ARG(iovcnt, __actv_local_var_iovcnt));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_writev(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_writev(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_writev(unsigned long long __rpc_info, void *__buf, long sockfd, const struct iovec *iov, int iovcnt, int flags)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const struct iovec *__actv_local_var_iov = iov;
	int __maybe_unused __actv_local_var_iovcnt = iovcnt;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcall_drvcall_devhost_net_socket_writev(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(iov, __actv_local_var_iov), __ARCH_ACTV_RPC_PASS_ARG(iovcnt, __actv_local_var_iovcnt), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_writev(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, const struct iovec *iov, int iovcnt, int flags)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused const struct iovec *__actv_local_var_iov = iov;
	int __maybe_unused __actv_local_var_iovcnt = iovcnt;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_writev(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(iov, __actv_local_var_iov), __ARCH_ACTV_RPC_PASS_ARG(iovcnt, __actv_local_var_iovcnt), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_getsockopt(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_getsockopt(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_getsockopt(unsigned long long __rpc_info, void *__buf, long sockfd, int level, int optname, void *optval, unsigned int *optlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	int __maybe_unused __actv_local_var_level = level;
	int __maybe_unused __actv_local_var_optname = optname;
	__maybe_unused void *__actv_local_var_optval = optval;
	__maybe_unused unsigned int *__actv_local_var_optlen = optlen;
	__ret = ____actvcall_drvcall_devhost_net_socket_getsockopt(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(level, __actv_local_var_level), __ARCH_ACTV_RPC_PASS_ARG(optname, __actv_local_var_optname), __ARCH_ACTV_RPC_PASS_ARG(optval, __actv_local_var_optval), __ARCH_ACTV_RPC_PASS_ARG(optlen, __actv_local_var_optlen));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_getsockopt(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, int level, int optname, void *optval, unsigned int *optlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	int __maybe_unused __actv_local_var_level = level;
	int __maybe_unused __actv_local_var_optname = optname;
	__maybe_unused void *__actv_local_var_optval = optval;
	__maybe_unused unsigned int *__actv_local_var_optlen = optlen;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_getsockopt(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(level, __actv_local_var_level), __ARCH_ACTV_RPC_PASS_ARG(optname, __actv_local_var_optname), __ARCH_ACTV_RPC_PASS_ARG(optval, __actv_local_var_optval), __ARCH_ACTV_RPC_PASS_ARG(optlen, __actv_local_var_optlen));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_setsockopt(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_setsockopt(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_setsockopt(unsigned long long __rpc_info, void *__buf, long sockfd, int level, int optname, const void *optval, unsigned int optlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	int __maybe_unused __actv_local_var_level = level;
	int __maybe_unused __actv_local_var_optname = optname;
	__maybe_unused const void *__actv_local_var_optval = optval;
	unsigned int __maybe_unused __actv_local_var_optlen = optlen;
	__ret = ____actvcall_drvcall_devhost_net_socket_setsockopt(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(level, __actv_local_var_level), __ARCH_ACTV_RPC_PASS_ARG(optname, __actv_local_var_optname), __ARCH_ACTV_RPC_PASS_ARG(optval, __actv_local_var_optval), __ARCH_ACTV_RPC_PASS_ARG(optlen, __actv_local_var_optlen));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_setsockopt(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, int level, int optname, const void *optval, unsigned int optlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	int __maybe_unused __actv_local_var_level = level;
	int __maybe_unused __actv_local_var_optname = optname;
	__maybe_unused const void *__actv_local_var_optval = optval;
	unsigned int __maybe_unused __actv_local_var_optlen = optlen;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_setsockopt(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(level, __actv_local_var_level), __ARCH_ACTV_RPC_PASS_ARG(optname, __actv_local_var_optname), __ARCH_ACTV_RPC_PASS_ARG(optval, __actv_local_var_optval), __ARCH_ACTV_RPC_PASS_ARG(optlen, __actv_local_var_optlen));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_getsockname(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_getsockname(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_getsockname(unsigned long long __rpc_info, void *__buf, long sockfd, struct sockaddr *addr, unsigned int *addrlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused struct sockaddr *__actv_local_var_addr = addr;
	__maybe_unused unsigned int *__actv_local_var_addrlen = addrlen;
	__ret = ____actvcall_drvcall_devhost_net_socket_getsockname(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(addrlen, __actv_local_var_addrlen));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_getsockname(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, struct sockaddr *addr, unsigned int *addrlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused struct sockaddr *__actv_local_var_addr = addr;
	__maybe_unused unsigned int *__actv_local_var_addrlen = addrlen;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_getsockname(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(addrlen, __actv_local_var_addrlen));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_getpeername(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_getpeername(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_getpeername(unsigned long long __rpc_info, void *__buf, long sockfd, struct sockaddr *addr, unsigned int *addrlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused struct sockaddr *__actv_local_var_addr = addr;
	__maybe_unused unsigned int *__actv_local_var_addrlen = addrlen;
	__ret = ____actvcall_drvcall_devhost_net_socket_getpeername(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(addrlen, __actv_local_var_addrlen));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_getpeername(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, struct sockaddr *addr, unsigned int *addrlen)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused struct sockaddr *__actv_local_var_addr = addr;
	__maybe_unused unsigned int *__actv_local_var_addrlen = addrlen;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_getpeername(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(addrlen, __actv_local_var_addrlen));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_fchown(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_fchown(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_fchown(unsigned long long __rpc_info, void *__buf, long sockfd, unsigned int uid, unsigned int gid)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	unsigned int __maybe_unused __actv_local_var_uid = uid;
	unsigned int __maybe_unused __actv_local_var_gid = gid;
	__ret = ____actvcall_drvcall_devhost_net_socket_fchown(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(uid, __actv_local_var_uid), __ARCH_ACTV_RPC_PASS_ARG(gid, __actv_local_var_gid));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_fchown(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, unsigned int uid, unsigned int gid)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	unsigned int __maybe_unused __actv_local_var_uid = uid;
	unsigned int __maybe_unused __actv_local_var_gid = gid;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_fchown(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(uid, __actv_local_var_uid), __ARCH_ACTV_RPC_PASS_ARG(gid, __actv_local_var_gid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_fstat(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_fstat(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_fstat(unsigned long long __rpc_info, void *__buf, long sockfd, uid_t *uid)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused uid_t *__actv_local_var_uid = uid;
	__ret = ____actvcall_drvcall_devhost_net_socket_fstat(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(uid, __actv_local_var_uid));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_fstat(rref_t __rref, unsigned long long __rpc_info, void *__buf, long sockfd, uid_t *uid)
{
	int __ret;
	long __maybe_unused __actv_local_var_sockfd = sockfd;
	__maybe_unused uid_t *__actv_local_var_uid = uid;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_fstat(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sockfd, __actv_local_var_sockfd), __ARCH_ACTV_RPC_PASS_ARG(uid, __actv_local_var_uid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_socket(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_socket(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_socket(unsigned long long __rpc_info, void *__buf, int domain, int type, int protocol, unsigned long ino)
{
	int __ret;
	int __maybe_unused __actv_local_var_domain = domain;
	int __maybe_unused __actv_local_var_type = type;
	int __maybe_unused __actv_local_var_protocol = protocol;
	unsigned long __maybe_unused __actv_local_var_ino = ino;
	__ret = ____actvcall_drvcall_devhost_net_socket_socket(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(domain, __actv_local_var_domain), __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type), __ARCH_ACTV_RPC_PASS_ARG(protocol, __actv_local_var_protocol), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_socket(rref_t __rref, unsigned long long __rpc_info, void *__buf, int domain, int type, int protocol, unsigned long ino)
{
	int __ret;
	int __maybe_unused __actv_local_var_domain = domain;
	int __maybe_unused __actv_local_var_type = type;
	int __maybe_unused __actv_local_var_protocol = protocol;
	unsigned long __maybe_unused __actv_local_var_ino = ino;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_socket(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(domain, __actv_local_var_domain), __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type), __ARCH_ACTV_RPC_PASS_ARG(protocol, __actv_local_var_protocol), __ARCH_ACTV_RPC_PASS_ARG(ino, __actv_local_var_ino));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_alloc_new_ns(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_alloc_new_ns(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_alloc_new_ns(unsigned long long __rpc_info, void *__buf, int nsid)
{
	int __ret;
	int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcall_drvcall_devhost_net_socket_alloc_new_ns(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_alloc_new_ns(rref_t __rref, unsigned long long __rpc_info, void *__buf, int nsid)
{
	int __ret;
	int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_alloc_new_ns(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_destroy_ns(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_destroy_ns(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_destroy_ns(unsigned long long __rpc_info, void *__buf, int nsid)
{
	int __ret;
	int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcall_drvcall_devhost_net_socket_destroy_ns(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_destroy_ns(rref_t __rref, unsigned long long __rpc_info, void *__buf, int nsid)
{
	int __ret;
	int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_destroy_ns(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_net_socket_bpf(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_net_socket_bpf(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_net_socket_bpf(unsigned long long __rpc_info, void *__buf, int cmd, void *attr, unsigned int size)
{
	int __ret;
	int __maybe_unused __actv_local_var_cmd = cmd;
	__maybe_unused void *__actv_local_var_attr = attr;
	unsigned int __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcall_drvcall_devhost_net_socket_bpf(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_net_socket_bpf(rref_t __rref, unsigned long long __rpc_info, void *__buf, int cmd, void *attr, unsigned int size)
{
	int __ret;
	int __maybe_unused __actv_local_var_cmd = cmd;
	__maybe_unused void *__actv_local_var_attr = attr;
	unsigned int __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcapcall_drvcall_devhost_net_socket_bpf(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_iaware_notify_thread_msg(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_iaware_notify_thread_msg(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_iaware_notify_thread_msg(unsigned long long __rpc_info, void *__buf, struct evtcb_thread_msg_s msg)
{
	int __ret;
	struct evtcb_thread_msg_s __maybe_unused __actv_local_var_msg = msg;
	__ret = ____actvcall_drvcall_devhost_iaware_notify_thread_msg(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(msg, __actv_local_var_msg));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_iaware_notify_thread_msg(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct evtcb_thread_msg_s msg)
{
	int __ret;
	struct evtcb_thread_msg_s __maybe_unused __actv_local_var_msg = msg;
	__ret = ____actvcapcall_drvcall_devhost_iaware_notify_thread_msg(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(msg, __actv_local_var_msg));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_kstate_binderinfo_notify(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_kstate_binderinfo_notify(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_kstate_binderinfo_notify(unsigned long long __rpc_info, void *__buf, int calling_pid, int called_pid)
{
	int __ret;
	int __maybe_unused __actv_local_var_calling_pid = calling_pid;
	int __maybe_unused __actv_local_var_called_pid = called_pid;
	__ret = ____actvcall_drvcall_devhost_kstate_binderinfo_notify(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(calling_pid, __actv_local_var_calling_pid), __ARCH_ACTV_RPC_PASS_ARG(called_pid, __actv_local_var_called_pid));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_kstate_binderinfo_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, int calling_pid, int called_pid)
{
	int __ret;
	int __maybe_unused __actv_local_var_calling_pid = calling_pid;
	int __maybe_unused __actv_local_var_called_pid = called_pid;
	__ret = ____actvcapcall_drvcall_devhost_kstate_binderinfo_notify(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(calling_pid, __actv_local_var_calling_pid), __ARCH_ACTV_RPC_PASS_ARG(called_pid, __actv_local_var_called_pid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_kstate_killinfo_notify(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_kstate_killinfo_notify(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_kstate_killinfo_notify(unsigned long long __rpc_info, void *__buf, int pid, int seg)
{
	int __ret;
	int __maybe_unused __actv_local_var_pid = pid;
	int __maybe_unused __actv_local_var_seg = seg;
	__ret = ____actvcall_drvcall_devhost_kstate_killinfo_notify(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(pid, __actv_local_var_pid), __ARCH_ACTV_RPC_PASS_ARG(seg, __actv_local_var_seg));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_kstate_killinfo_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, int pid, int seg)
{
	int __ret;
	int __maybe_unused __actv_local_var_pid = pid;
	int __maybe_unused __actv_local_var_seg = seg;
	__ret = ____actvcapcall_drvcall_devhost_kstate_killinfo_notify(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(pid, __actv_local_var_pid), __ARCH_ACTV_RPC_PASS_ARG(seg, __actv_local_var_seg));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_kstate_freeze_binder_info_notify(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_kstate_freeze_binder_info_notify(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_kstate_freeze_binder_info_notify(unsigned long long __rpc_info, void *__buf, struct kstate_binder_info *info)
{
	int __ret;
	__maybe_unused struct kstate_binder_info *__actv_local_var_info = info;
	__ret = ____actvcall_drvcall_devhost_kstate_freeze_binder_info_notify(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(info, __actv_local_var_info));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_kstate_freeze_binder_info_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct kstate_binder_info *info)
{
	int __ret;
	__maybe_unused struct kstate_binder_info *__actv_local_var_info = info;
	__ret = ____actvcapcall_drvcall_devhost_kstate_freeze_binder_info_notify(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(info, __actv_local_var_info));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_kstate_request_anco_thaw(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_kstate_request_anco_thaw(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_kstate_request_anco_thaw(unsigned long long __rpc_info, void *__buf, char *reason, unsigned int size)
{
	int __ret;
	__maybe_unused char *__actv_local_var_reason = reason;
	unsigned int __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcall_drvcall_devhost_kstate_request_anco_thaw(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(reason, __actv_local_var_reason), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_kstate_request_anco_thaw(rref_t __rref, unsigned long long __rpc_info, void *__buf, char *reason, unsigned int size)
{
	int __ret;
	__maybe_unused char *__actv_local_var_reason = reason;
	unsigned int __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcapcall_drvcall_devhost_kstate_request_anco_thaw(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(reason, __actv_local_var_reason), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_pm_action(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_pm_action(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_pm_action(unsigned long long __rpc_info, void *__buf, unsigned int action)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_action = action;
	__ret = ____actvcall_drvcall_devhost_pm_action(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(action, __actv_local_var_action));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_pm_action(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int action)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_action = action;
	__ret = ____actvcapcall_drvcall_devhost_pm_action(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(action, __actv_local_var_action));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_pm_dubai_hwlog(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_pm_dubai_hwlog(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_pm_dubai_hwlog(unsigned long long __rpc_info, void *__buf, struct pm_dubai_ctrl_log *log_info)
{
	int __ret;
	__maybe_unused struct pm_dubai_ctrl_log *__actv_local_var_log_info = log_info;
	__ret = ____actvcall_drvcall_devhost_pm_dubai_hwlog(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(log_info, __actv_local_var_log_info));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_pm_dubai_hwlog(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct pm_dubai_ctrl_log *log_info)
{
	int __ret;
	__maybe_unused struct pm_dubai_ctrl_log *__actv_local_var_log_info = log_info;
	__ret = ____actvcapcall_drvcall_devhost_pm_dubai_hwlog(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(log_info, __actv_local_var_log_info));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_pm_prepare(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_pm_prepare(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_pm_prepare(unsigned long long __rpc_info, void *__buf, unsigned int type, struct reboot_cmd cmd)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_type = type;
	struct reboot_cmd __maybe_unused __actv_local_var_cmd = cmd;
	__ret = ____actvcall_drvcall_devhost_pm_prepare(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_pm_prepare(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int type, struct reboot_cmd cmd)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_type = type;
	struct reboot_cmd __maybe_unused __actv_local_var_cmd = cmd;
	__ret = ____actvcapcall_drvcall_devhost_pm_prepare(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_pm_power_off(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_pm_power_off(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_pm_power_off(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devhost_pm_power_off(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_pm_power_off(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devhost_pm_power_off(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_pm_reboot(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_pm_reboot(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_pm_reboot(unsigned long long __rpc_info, void *__buf, struct reboot_cmd cmd)
{
	int __ret;
	struct reboot_cmd __maybe_unused __actv_local_var_cmd = cmd;
	__ret = ____actvcall_drvcall_devhost_pm_reboot(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_pm_reboot(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct reboot_cmd cmd)
{
	int __ret;
	struct reboot_cmd __maybe_unused __actv_local_var_cmd = cmd;
	__ret = ____actvcapcall_drvcall_devhost_pm_reboot(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_pm_cpuhp_action(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_pm_cpuhp_action(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_pm_cpuhp_action(unsigned long long __rpc_info, void *__buf, unsigned int cpu, unsigned int action)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cpu = cpu;
	unsigned int __maybe_unused __actv_local_var_action = action;
	__ret = ____actvcall_drvcall_devhost_pm_cpuhp_action(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cpu, __actv_local_var_cpu), __ARCH_ACTV_RPC_PASS_ARG(action, __actv_local_var_action));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_pm_cpuhp_action(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cpu, unsigned int action)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cpu = cpu;
	unsigned int __maybe_unused __actv_local_var_action = action;
	__ret = ____actvcapcall_drvcall_devhost_pm_cpuhp_action(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cpu, __actv_local_var_cpu), __ARCH_ACTV_RPC_PASS_ARG(action, __actv_local_var_action));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_pm_wl_stat(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_pm_wl_stat(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_pm_wl_stat(unsigned long long __rpc_info, void *__buf, struct wl_stat_queue_info queue_info, size_t cnt)
{
	int __ret;
	struct wl_stat_queue_info __maybe_unused __actv_local_var_queue_info = queue_info;
	size_t __maybe_unused __actv_local_var_cnt = cnt;
	__ret = ____actvcall_drvcall_devhost_pm_wl_stat(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(queue_info, __actv_local_var_queue_info), __ARCH_ACTV_RPC_PASS_ARG(cnt, __actv_local_var_cnt));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_pm_wl_stat(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct wl_stat_queue_info queue_info, size_t cnt)
{
	int __ret;
	struct wl_stat_queue_info __maybe_unused __actv_local_var_queue_info = queue_info;
	size_t __maybe_unused __actv_local_var_cnt = cnt;
	__ret = ____actvcapcall_drvcall_devhost_pm_wl_stat(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(queue_info, __actv_local_var_queue_info), __ARCH_ACTV_RPC_PASS_ARG(cnt, __actv_local_var_cnt));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_procfs_open(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_procfs_open(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_procfs_open(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf buf)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_buf = buf;
	__ret = ____actvcall_drvcall_devhost_procfs_open(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_procfs_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf buf)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_buf = buf;
	__ret = ____actvcapcall_drvcall_devhost_procfs_open(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_procfs_close(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_procfs_close(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_procfs_close(unsigned long long __rpc_info, void *__buf, struct hm_actv_buf buf)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_buf = buf;
	__ret = ____actvcall_drvcall_devhost_procfs_close(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_procfs_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct hm_actv_buf buf)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_buf = buf;
	__ret = ____actvcapcall_drvcall_devhost_procfs_close(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_procfs_read(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_procfs_read(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_procfs_read(unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_fd = fd;
	unsigned long long __maybe_unused __actv_local_var_pos = pos;
	struct hm_actv_buf __maybe_unused __actv_local_var_ubuf = ubuf;
	uintptr_t __maybe_unused __actv_local_var_ctx = ctx;
	__ret = ____actvcall_drvcall_devhost_procfs_read(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(ubuf, __actv_local_var_ubuf), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_procfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_fd = fd;
	unsigned long long __maybe_unused __actv_local_var_pos = pos;
	struct hm_actv_buf __maybe_unused __actv_local_var_ubuf = ubuf;
	uintptr_t __maybe_unused __actv_local_var_ctx = ctx;
	__ret = ____actvcapcall_drvcall_devhost_procfs_read(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(ubuf, __actv_local_var_ubuf), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_procfs_write(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_procfs_write(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_procfs_write(unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_fd = fd;
	unsigned long long __maybe_unused __actv_local_var_pos = pos;
	struct hm_actv_buf __maybe_unused __actv_local_var_ubuf = ubuf;
	uintptr_t __maybe_unused __actv_local_var_ctx = ctx;
	__ret = ____actvcall_drvcall_devhost_procfs_write(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(ubuf, __actv_local_var_ubuf), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_procfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_fd = fd;
	unsigned long long __maybe_unused __actv_local_var_pos = pos;
	struct hm_actv_buf __maybe_unused __actv_local_var_ubuf = ubuf;
	uintptr_t __maybe_unused __actv_local_var_ctx = ctx;
	__ret = ____actvcapcall_drvcall_devhost_procfs_write(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(ubuf, __actv_local_var_ubuf), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_sysfs_open(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_sysfs_open(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_sysfs_open(unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned int flags, unsigned int mode, uintptr_t ctx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	uintptr_t __maybe_unused __actv_local_var_ctx = ctx;
	__ret = ____actvcall_drvcall_devhost_sysfs_open(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_sysfs_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned int flags, unsigned int mode, uintptr_t ctx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	uintptr_t __maybe_unused __actv_local_var_ctx = ctx;
	__ret = ____actvcapcall_drvcall_devhost_sysfs_open(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_sysfs_close(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_sysfs_close(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_sysfs_close(unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, uintptr_t ctx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_fd = fd;
	uintptr_t __maybe_unused __actv_local_var_ctx = ctx;
	__ret = ____actvcall_drvcall_devhost_sysfs_close(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_sysfs_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, uintptr_t ctx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_fd = fd;
	uintptr_t __maybe_unused __actv_local_var_ctx = ctx;
	__ret = ____actvcapcall_drvcall_devhost_sysfs_close(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_sysfs_read(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_sysfs_read(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_sysfs_read(unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_fd = fd;
	unsigned long long __maybe_unused __actv_local_var_pos = pos;
	struct hm_actv_buf __maybe_unused __actv_local_var_ubuf = ubuf;
	uintptr_t __maybe_unused __actv_local_var_ctx = ctx;
	__ret = ____actvcall_drvcall_devhost_sysfs_read(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(ubuf, __actv_local_var_ubuf), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_sysfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_fd = fd;
	unsigned long long __maybe_unused __actv_local_var_pos = pos;
	struct hm_actv_buf __maybe_unused __actv_local_var_ubuf = ubuf;
	uintptr_t __maybe_unused __actv_local_var_ctx = ctx;
	__ret = ____actvcapcall_drvcall_devhost_sysfs_read(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(ubuf, __actv_local_var_ubuf), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_sysfs_write(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_sysfs_write(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_sysfs_write(unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_fd = fd;
	unsigned long long __maybe_unused __actv_local_var_pos = pos;
	struct hm_actv_buf __maybe_unused __actv_local_var_ubuf = ubuf;
	uintptr_t __maybe_unused __actv_local_var_ctx = ctx;
	__ret = ____actvcall_drvcall_devhost_sysfs_write(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(ubuf, __actv_local_var_ubuf), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_sysfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tagid, unsigned long long fd, unsigned long long pos, struct hm_actv_buf ubuf, uintptr_t ctx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tagid = tagid;
	unsigned long long __maybe_unused __actv_local_var_fd = fd;
	unsigned long long __maybe_unused __actv_local_var_pos = pos;
	struct hm_actv_buf __maybe_unused __actv_local_var_ubuf = ubuf;
	uintptr_t __maybe_unused __actv_local_var_ctx = ctx;
	__ret = ____actvcapcall_drvcall_devhost_sysfs_write(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(ubuf, __actv_local_var_ubuf), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_sysfs_poll(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_sysfs_poll(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_sysfs_poll(unsigned long long __rpc_info, void *__buf, unsigned int tagid, uintptr_t ctx, bool pollable)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tagid = tagid;
	uintptr_t __maybe_unused __actv_local_var_ctx = ctx;
	bool __maybe_unused __actv_local_var_pollable = pollable;
	__ret = ____actvcall_drvcall_devhost_sysfs_poll(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx), __ARCH_ACTV_RPC_PASS_ARG(pollable, __actv_local_var_pollable));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_sysfs_poll(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tagid, uintptr_t ctx, bool pollable)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tagid = tagid;
	uintptr_t __maybe_unused __actv_local_var_ctx = ctx;
	bool __maybe_unused __actv_local_var_pollable = pollable;
	__ret = ____actvcapcall_drvcall_devhost_sysfs_poll(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tagid, __actv_local_var_tagid), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx), __ARCH_ACTV_RPC_PASS_ARG(pollable, __actv_local_var_pollable));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_clock_settime(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_clock_settime(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_clock_settime(unsigned long long __rpc_info, void *__buf, unsigned long tp, unsigned long long file_id)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_tp = tp;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcall_drvcall_devhost_clock_settime(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tp, __actv_local_var_tp), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_clock_settime(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long tp, unsigned long long file_id)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_tp = tp;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcapcall_drvcall_devhost_clock_settime(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tp, __actv_local_var_tp), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_clock_gettime(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_clock_gettime(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_clock_gettime(unsigned long long __rpc_info, void *__buf, unsigned long tp, unsigned long long file_id)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_tp = tp;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcall_drvcall_devhost_clock_gettime(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tp, __actv_local_var_tp), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_clock_gettime(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long tp, unsigned long long file_id)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_tp = tp;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcapcall_drvcall_devhost_clock_gettime(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tp, __actv_local_var_tp), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_clock_getres(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_clock_getres(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_clock_getres(unsigned long long __rpc_info, void *__buf, unsigned long tp, unsigned long long file_id)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_tp = tp;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcall_drvcall_devhost_clock_getres(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tp, __actv_local_var_tp), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_clock_getres(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long tp, unsigned long long file_id)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_tp = tp;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcapcall_drvcall_devhost_clock_getres(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tp, __actv_local_var_tp), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_clock_adjtime(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_clock_adjtime(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_clock_adjtime(unsigned long long __rpc_info, void *__buf, unsigned long tx, unsigned long long file_id)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_tx = tx;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcall_drvcall_devhost_clock_adjtime(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tx, __actv_local_var_tx), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_clock_adjtime(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long tx, unsigned long long file_id)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_tx = tx;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcapcall_drvcall_devhost_clock_adjtime(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tx, __actv_local_var_tx), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_clock_poweroff_alarm(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_clock_poweroff_alarm(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_clock_poweroff_alarm(unsigned long long __rpc_info, void *__buf, long tm)
{
	int __ret;
	long __maybe_unused __actv_local_var_tm = tm;
	__ret = ____actvcall_drvcall_devhost_clock_poweroff_alarm(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tm, __actv_local_var_tm));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_clock_poweroff_alarm(rref_t __rref, unsigned long long __rpc_info, void *__buf, long tm)
{
	int __ret;
	long __maybe_unused __actv_local_var_tm = tm;
	__ret = ____actvcapcall_drvcall_devhost_clock_poweroff_alarm(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tm, __actv_local_var_tm));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_fallback"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_mounted(unsigned long long rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, ...);


static int __attribute__((alias("__internal_rpccapcall_fallback"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_mounted(rref_t __rref, unsigned long long rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_mounted(unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, struct hm_actv_buf fs_name, struct hm_actv_buf dev_name, struct hm_actv_buf mount_opts)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_fs_name = fs_name;
	struct hm_actv_buf __maybe_unused __actv_local_var_dev_name = dev_name;
	struct hm_actv_buf __maybe_unused __actv_local_var_mount_opts = mount_opts;
	__ret = ____actvcall_drvcall_devhost_transfs_mounted(__rpc_info, __buf, __rply_buf, rply_buf_sz, __ARCH_ACTV_RPC_PASS_ARG(fs_name, __actv_local_var_fs_name), __ARCH_ACTV_RPC_PASS_ARG(dev_name, __actv_local_var_dev_name), __ARCH_ACTV_RPC_PASS_ARG(mount_opts, __actv_local_var_mount_opts));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_mounted(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, struct hm_actv_buf fs_name, struct hm_actv_buf dev_name, struct hm_actv_buf mount_opts)
{
	int __ret;
	struct hm_actv_buf __maybe_unused __actv_local_var_fs_name = fs_name;
	struct hm_actv_buf __maybe_unused __actv_local_var_dev_name = dev_name;
	struct hm_actv_buf __maybe_unused __actv_local_var_mount_opts = mount_opts;
	__ret = ____actvcapcall_drvcall_devhost_transfs_mounted(__rref, __rpc_info, __buf, __rply_buf, rply_buf_sz, __ARCH_ACTV_RPC_PASS_ARG(fs_name, __actv_local_var_fs_name), __ARCH_ACTV_RPC_PASS_ARG(dev_name, __actv_local_var_dev_name), __ARCH_ACTV_RPC_PASS_ARG(mount_opts, __actv_local_var_mount_opts));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_unmounted(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_unmounted(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_unmounted(unsigned long long __rpc_info, void *__buf, unsigned int s_index, int flags, unsigned long long file_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	int __maybe_unused __actv_local_var_flags = flags;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcall_drvcall_devhost_transfs_unmounted(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_unmounted(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, int flags, unsigned long long file_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	int __maybe_unused __actv_local_var_flags = flags;
	unsigned long long __maybe_unused __actv_local_var_file_id = file_id;
	__ret = ____actvcapcall_drvcall_devhost_transfs_unmounted(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(file_id, __actv_local_var_file_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_drop_caches(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_drop_caches(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_drop_caches(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int mode, int max_nr)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	int __maybe_unused __actv_local_var_max_nr = max_nr;
	__ret = ____actvcall_drvcall_devhost_transfs_drop_caches(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(max_nr, __actv_local_var_max_nr));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_drop_caches(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int mode, int max_nr)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	int __maybe_unused __actv_local_var_max_nr = max_nr;
	__ret = ____actvcapcall_drvcall_devhost_transfs_drop_caches(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(max_nr, __actv_local_var_max_nr));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_mkdir(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_mkdir(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_mkdir(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_pd_index = pd_index;
	mode_t __maybe_unused __actv_local_var_mode = mode;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	__ret = ____actvcall_drvcall_devhost_transfs_mkdir(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(pd_index, __actv_local_var_pd_index), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_mkdir(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_pd_index = pd_index;
	mode_t __maybe_unused __actv_local_var_mode = mode;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	__ret = ____actvcapcall_drvcall_devhost_transfs_mkdir(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(pd_index, __actv_local_var_pd_index), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_rmdir(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_rmdir(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_rmdir(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, unsigned int d_index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_pd_index = pd_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__ret = ____actvcall_drvcall_devhost_transfs_rmdir(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(pd_index, __actv_local_var_pd_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_rmdir(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, unsigned int d_index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_pd_index = pd_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__ret = ____actvcapcall_drvcall_devhost_transfs_rmdir(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(pd_index, __actv_local_var_pd_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_create(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_create(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_create(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_pd_index = pd_index;
	mode_t __maybe_unused __actv_local_var_mode = mode;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	__ret = ____actvcall_drvcall_devhost_transfs_create(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(pd_index, __actv_local_var_pd_index), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_create(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, mode_t mode, struct hm_actv_buf actv_dname)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_pd_index = pd_index;
	mode_t __maybe_unused __actv_local_var_mode = mode;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	__ret = ____actvcapcall_drvcall_devhost_transfs_create(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(pd_index, __actv_local_var_pd_index), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_unlink(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_unlink(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_unlink(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, unsigned int d_index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_pd_index = pd_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__ret = ____actvcall_drvcall_devhost_transfs_unlink(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(pd_index, __actv_local_var_pd_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_unlink(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, unsigned int d_index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_pd_index = pd_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__ret = ____actvcapcall_drvcall_devhost_transfs_unlink(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(pd_index, __actv_local_var_pd_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_mknod(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_mknod(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_mknod(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, unsigned int mode, unsigned long long devno, struct hm_actv_buf actv_dname)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_pd_index = pd_index;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	unsigned long long __maybe_unused __actv_local_var_devno = devno;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	__ret = ____actvcall_drvcall_devhost_transfs_mknod(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(pd_index, __actv_local_var_pd_index), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(devno, __actv_local_var_devno), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_mknod(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, unsigned int mode, unsigned long long devno, struct hm_actv_buf actv_dname)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_pd_index = pd_index;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	unsigned long long __maybe_unused __actv_local_var_devno = devno;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	__ret = ____actvcapcall_drvcall_devhost_transfs_mknod(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(pd_index, __actv_local_var_pd_index), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(devno, __actv_local_var_devno), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_link(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_link(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_link(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int old_d_index, unsigned int new_pd_index, struct hm_actv_buf actv_dname)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_old_d_index = old_d_index;
	unsigned int __maybe_unused __actv_local_var_new_pd_index = new_pd_index;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	__ret = ____actvcall_drvcall_devhost_transfs_link(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(old_d_index, __actv_local_var_old_d_index), __ARCH_ACTV_RPC_PASS_ARG(new_pd_index, __actv_local_var_new_pd_index), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_link(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int old_d_index, unsigned int new_pd_index, struct hm_actv_buf actv_dname)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_old_d_index = old_d_index;
	unsigned int __maybe_unused __actv_local_var_new_pd_index = new_pd_index;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	__ret = ____actvcapcall_drvcall_devhost_transfs_link(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(old_d_index, __actv_local_var_old_d_index), __ARCH_ACTV_RPC_PASS_ARG(new_pd_index, __actv_local_var_new_pd_index), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_symlink(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_symlink(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_symlink(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, struct hm_actv_buf actv_dname, struct hm_actv_buf actv_path)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_pd_index = pd_index;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_path = actv_path;
	__ret = ____actvcall_drvcall_devhost_transfs_symlink(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(pd_index, __actv_local_var_pd_index), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname), __ARCH_ACTV_RPC_PASS_ARG(actv_path, __actv_local_var_actv_path));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_symlink(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_index, struct hm_actv_buf actv_dname, struct hm_actv_buf actv_path)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_pd_index = pd_index;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_path = actv_path;
	__ret = ____actvcapcall_drvcall_devhost_transfs_symlink(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(pd_index, __actv_local_var_pd_index), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname), __ARCH_ACTV_RPC_PASS_ARG(actv_path, __actv_local_var_actv_path));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_statfs(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_statfs(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_statfs(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, void *buf)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__maybe_unused void *__actv_local_var_buf = buf;
	__ret = ____actvcall_drvcall_devhost_transfs_statfs(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_statfs(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, void *buf)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__maybe_unused void *__actv_local_var_buf = buf;
	__ret = ____actvcapcall_drvcall_devhost_transfs_statfs(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_rename(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_rename(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_rename(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int old_pd_index, unsigned int old_d_index, unsigned int new_pd_index, unsigned int new_d_index, struct hm_actv_buf actv_dname)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_old_pd_index = old_pd_index;
	unsigned int __maybe_unused __actv_local_var_old_d_index = old_d_index;
	unsigned int __maybe_unused __actv_local_var_new_pd_index = new_pd_index;
	unsigned int __maybe_unused __actv_local_var_new_d_index = new_d_index;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	__ret = ____actvcall_drvcall_devhost_transfs_rename(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(old_pd_index, __actv_local_var_old_pd_index), __ARCH_ACTV_RPC_PASS_ARG(old_d_index, __actv_local_var_old_d_index), __ARCH_ACTV_RPC_PASS_ARG(new_pd_index, __actv_local_var_new_pd_index), __ARCH_ACTV_RPC_PASS_ARG(new_d_index, __actv_local_var_new_d_index), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_rename(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int old_pd_index, unsigned int old_d_index, unsigned int new_pd_index, unsigned int new_d_index, struct hm_actv_buf actv_dname)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_old_pd_index = old_pd_index;
	unsigned int __maybe_unused __actv_local_var_old_d_index = old_d_index;
	unsigned int __maybe_unused __actv_local_var_new_pd_index = new_pd_index;
	unsigned int __maybe_unused __actv_local_var_new_d_index = new_d_index;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	__ret = ____actvcapcall_drvcall_devhost_transfs_rename(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(old_pd_index, __actv_local_var_old_pd_index), __ARCH_ACTV_RPC_PASS_ARG(old_d_index, __actv_local_var_old_d_index), __ARCH_ACTV_RPC_PASS_ARG(new_pd_index, __actv_local_var_new_pd_index), __ARCH_ACTV_RPC_PASS_ARG(new_d_index, __actv_local_var_new_d_index), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_lookup(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_lookup(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_lookup(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int pd_index, struct hm_actv_buf actv_dname, void *meta)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_pd_index = pd_index;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	__maybe_unused void *__actv_local_var_meta = meta;
	__ret = ____actvcall_drvcall_devhost_transfs_lookup(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(pd_index, __actv_local_var_pd_index), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname), __ARCH_ACTV_RPC_PASS_ARG(meta, __actv_local_var_meta));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_lookup(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int pd_index, struct hm_actv_buf actv_dname, void *meta)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_pd_index = pd_index;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	__maybe_unused void *__actv_local_var_meta = meta;
	__ret = ____actvcapcall_drvcall_devhost_transfs_lookup(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(pd_index, __actv_local_var_pd_index), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname), __ARCH_ACTV_RPC_PASS_ARG(meta, __actv_local_var_meta));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_setattr(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_setattr(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_setattr(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, const void *attr, uint64_t size)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__maybe_unused const void *__actv_local_var_attr = attr;
	uint64_t __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcall_drvcall_devhost_transfs_setattr(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_setattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, const void *attr, uint64_t size)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__maybe_unused const void *__actv_local_var_attr = attr;
	uint64_t __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcapcall_drvcall_devhost_transfs_setattr(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(attr, __actv_local_var_attr), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_readlink(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_readlink(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_readlink(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, char *buf, size_t bufsize)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__maybe_unused char *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_bufsize = bufsize;
	__ret = ____actvcall_drvcall_devhost_transfs_readlink(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(bufsize, __actv_local_var_bufsize));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_readlink(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, char *buf, size_t bufsize)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__maybe_unused char *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_bufsize = bufsize;
	__ret = ____actvcapcall_drvcall_devhost_transfs_readlink(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(bufsize, __actv_local_var_bufsize));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_getattr(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_getattr(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_getattr(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, void *meta)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__maybe_unused void *__actv_local_var_meta = meta;
	__ret = ____actvcall_drvcall_devhost_transfs_getattr(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(meta, __actv_local_var_meta));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_getattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, void *meta)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__maybe_unused void *__actv_local_var_meta = meta;
	__ret = ____actvcapcall_drvcall_devhost_transfs_getattr(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(meta, __actv_local_var_meta));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_listxattr(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_listxattr(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_listxattr(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, char *buffer, size_t size)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__maybe_unused char *__actv_local_var_buffer = buffer;
	size_t __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcall_drvcall_devhost_transfs_listxattr(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(buffer, __actv_local_var_buffer), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_listxattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, char *buffer, size_t size)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__maybe_unused char *__actv_local_var_buffer = buffer;
	size_t __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcapcall_drvcall_devhost_transfs_listxattr(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(buffer, __actv_local_var_buffer), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_getxattr(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_getxattr(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_getxattr(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, char *value, size_t size)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	int __maybe_unused __actv_local_var_prefix = prefix;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_name = actv_name;
	__maybe_unused char *__actv_local_var_value = value;
	size_t __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcall_drvcall_devhost_transfs_getxattr(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(prefix, __actv_local_var_prefix), __ARCH_ACTV_RPC_PASS_ARG(actv_name, __actv_local_var_actv_name), __ARCH_ACTV_RPC_PASS_ARG(value, __actv_local_var_value), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_getxattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, char *value, size_t size)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	int __maybe_unused __actv_local_var_prefix = prefix;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_name = actv_name;
	__maybe_unused char *__actv_local_var_value = value;
	size_t __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcapcall_drvcall_devhost_transfs_getxattr(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(prefix, __actv_local_var_prefix), __ARCH_ACTV_RPC_PASS_ARG(actv_name, __actv_local_var_actv_name), __ARCH_ACTV_RPC_PASS_ARG(value, __actv_local_var_value), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_setxattr(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_setxattr(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_setxattr(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, const char *value, size_t size, int flags)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	int __maybe_unused __actv_local_var_prefix = prefix;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_name = actv_name;
	__maybe_unused const char *__actv_local_var_value = value;
	size_t __maybe_unused __actv_local_var_size = size;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcall_drvcall_devhost_transfs_setxattr(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(prefix, __actv_local_var_prefix), __ARCH_ACTV_RPC_PASS_ARG(actv_name, __actv_local_var_actv_name), __ARCH_ACTV_RPC_PASS_ARG(value, __actv_local_var_value), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_setxattr(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index, int prefix, struct hm_actv_buf actv_name, const char *value, size_t size, int flags)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	int __maybe_unused __actv_local_var_prefix = prefix;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_name = actv_name;
	__maybe_unused const char *__actv_local_var_value = value;
	size_t __maybe_unused __actv_local_var_size = size;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcapcall_drvcall_devhost_transfs_setxattr(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(prefix, __actv_local_var_prefix), __ARCH_ACTV_RPC_PASS_ARG(actv_name, __actv_local_var_actv_name), __ARCH_ACTV_RPC_PASS_ARG(value, __actv_local_var_value), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_tmpfile(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_tmpfile(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_tmpfile(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_addr, mode_t mode, struct hm_actv_buf actv_dname)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_pd_addr = pd_addr;
	mode_t __maybe_unused __actv_local_var_mode = mode;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	__ret = ____actvcall_drvcall_devhost_transfs_tmpfile(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(pd_addr, __actv_local_var_pd_addr), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_tmpfile(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int pd_addr, mode_t mode, struct hm_actv_buf actv_dname)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_pd_addr = pd_addr;
	mode_t __maybe_unused __actv_local_var_mode = mode;
	struct hm_actv_buf __maybe_unused __actv_local_var_actv_dname = actv_dname;
	__ret = ____actvcapcall_drvcall_devhost_transfs_tmpfile(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(pd_addr, __actv_local_var_pd_addr), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(actv_dname, __actv_local_var_actv_dname));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_kill_dentry(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_kill_dentry(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_kill_dentry(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__ret = ____actvcall_drvcall_devhost_transfs_kill_dentry(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_kill_dentry(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int d_index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	__ret = ____actvcapcall_drvcall_devhost_transfs_kill_dentry(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_node_put(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_node_put(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_node_put(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int v_index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_v_index = v_index;
	__ret = ____actvcall_drvcall_devhost_transfs_node_put(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(v_index, __actv_local_var_v_index));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_node_put(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int v_index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_v_index = v_index;
	__ret = ____actvcapcall_drvcall_devhost_transfs_node_put(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(v_index, __actv_local_var_v_index));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_sync(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_sync(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_sync(unsigned long long __rpc_info, void *__buf, unsigned int s_index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	__ret = ____actvcall_drvcall_devhost_transfs_sync(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_sync(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	__ret = ____actvcapcall_drvcall_devhost_transfs_sync(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_write(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_write(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_write(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	uint64_t __maybe_unused __actv_local_var_pos = pos;
	__maybe_unused const void *__actv_local_var_src = src;
	size_t __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcall_drvcall_devhost_transfs_write(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(src, __actv_local_var_src), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	uint64_t __maybe_unused __actv_local_var_pos = pos;
	__maybe_unused const void *__actv_local_var_src = src;
	size_t __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcapcall_drvcall_devhost_transfs_write(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(src, __actv_local_var_src), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_read(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_read(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_read(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	uint64_t __maybe_unused __actv_local_var_pos = pos;
	__maybe_unused const void *__actv_local_var_src = src;
	size_t __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcall_drvcall_devhost_transfs_read(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(src, __actv_local_var_src), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *src, size_t len)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	uint64_t __maybe_unused __actv_local_var_pos = pos;
	__maybe_unused const void *__actv_local_var_src = src;
	size_t __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcapcall_drvcall_devhost_transfs_read(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(src, __actv_local_var_src), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_write_iter(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_write_iter(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_write_iter(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *iov, int nr_segs, size_t count)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	uint64_t __maybe_unused __actv_local_var_pos = pos;
	__maybe_unused const void *__actv_local_var_iov = iov;
	int __maybe_unused __actv_local_var_nr_segs = nr_segs;
	size_t __maybe_unused __actv_local_var_count = count;
	__ret = ____actvcall_drvcall_devhost_transfs_write_iter(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(iov, __actv_local_var_iov), __ARCH_ACTV_RPC_PASS_ARG(nr_segs, __actv_local_var_nr_segs), __ARCH_ACTV_RPC_PASS_ARG(count, __actv_local_var_count));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_write_iter(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t pos, const void *iov, int nr_segs, size_t count)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	uint64_t __maybe_unused __actv_local_var_pos = pos;
	__maybe_unused const void *__actv_local_var_iov = iov;
	int __maybe_unused __actv_local_var_nr_segs = nr_segs;
	size_t __maybe_unused __actv_local_var_count = count;
	__ret = ____actvcapcall_drvcall_devhost_transfs_write_iter(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(iov, __actv_local_var_iov), __ARCH_ACTV_RPC_PASS_ARG(nr_segs, __actv_local_var_nr_segs), __ARCH_ACTV_RPC_PASS_ARG(count, __actv_local_var_count));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_fsync(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_fsync(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_fsync(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	__ret = ____actvcall_drvcall_devhost_transfs_fsync(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_fsync(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	__ret = ____actvcapcall_drvcall_devhost_transfs_fsync(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_readdir(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_readdir(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_readdir(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int f_index, uint64_t pos, void *buf, size_t size)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	uint64_t __maybe_unused __actv_local_var_pos = pos;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcall_drvcall_devhost_transfs_readdir(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_readdir(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int f_index, uint64_t pos, void *buf, size_t size)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	uint64_t __maybe_unused __actv_local_var_pos = pos;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcapcall_drvcall_devhost_transfs_readdir(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_open(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_open(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_open(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, unsigned int f_modes, unsigned int f_flags)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	unsigned int __maybe_unused __actv_local_var_f_modes = f_modes;
	unsigned int __maybe_unused __actv_local_var_f_flags = f_flags;
	__ret = ____actvcall_drvcall_devhost_transfs_open(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(f_modes, __actv_local_var_f_modes), __ARCH_ACTV_RPC_PASS_ARG(f_flags, __actv_local_var_f_flags));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int d_index, unsigned int f_modes, unsigned int f_flags)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_d_index = d_index;
	unsigned int __maybe_unused __actv_local_var_f_modes = f_modes;
	unsigned int __maybe_unused __actv_local_var_f_flags = f_flags;
	__ret = ____actvcapcall_drvcall_devhost_transfs_open(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(d_index, __actv_local_var_d_index), __ARCH_ACTV_RPC_PASS_ARG(f_modes, __actv_local_var_f_modes), __ARCH_ACTV_RPC_PASS_ARG(f_flags, __actv_local_var_f_flags));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_fallocate(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_fallocate(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_fallocate(unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int f_index, unsigned int mode, uint64_t pos, uint64_t len)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	uint64_t __maybe_unused __actv_local_var_pos = pos;
	uint64_t __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcall_drvcall_devhost_transfs_fallocate(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_fallocate(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int sb_index, unsigned int f_index, unsigned int mode, uint64_t pos, uint64_t len)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_sb_index = sb_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	uint64_t __maybe_unused __actv_local_var_pos = pos;
	uint64_t __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcapcall_drvcall_devhost_transfs_fallocate(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sb_index, __actv_local_var_sb_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode), __ARCH_ACTV_RPC_PASS_ARG(pos, __actv_local_var_pos), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_close(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_close(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_close(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	__ret = ____actvcall_drvcall_devhost_transfs_close(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_close(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	__ret = ____actvcapcall_drvcall_devhost_transfs_close(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_ioctl(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_ioctl(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_ioctl(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, unsigned int cmd, unsigned long arg)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	unsigned int __maybe_unused __actv_local_var_cmd = cmd;
	unsigned long __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcall_drvcall_devhost_transfs_ioctl(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, unsigned int cmd, unsigned long arg)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	unsigned int __maybe_unused __actv_local_var_cmd = cmd;
	unsigned long __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcapcall_drvcall_devhost_transfs_ioctl(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_direct_IO(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_direct_IO(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_direct_IO(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t offset, void *buf, size_t len, unsigned int type)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	uint64_t __maybe_unused __actv_local_var_offset = offset;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_len = len;
	unsigned int __maybe_unused __actv_local_var_type = type;
	__ret = ____actvcall_drvcall_devhost_transfs_direct_IO(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_direct_IO(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, uint64_t offset, void *buf, size_t len, unsigned int type)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	uint64_t __maybe_unused __actv_local_var_offset = offset;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_len = len;
	unsigned int __maybe_unused __actv_local_var_type = type;
	__ret = ____actvcapcall_drvcall_devhost_transfs_direct_IO(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_update_opts(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_update_opts(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_update_opts(unsigned long long __rpc_info, void *__buf, unsigned int s_index, char *ubuf, size_t buflen)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	__maybe_unused char *__actv_local_var_ubuf = ubuf;
	size_t __maybe_unused __actv_local_var_buflen = buflen;
	__ret = ____actvcall_drvcall_devhost_transfs_update_opts(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(ubuf, __actv_local_var_ubuf), __ARCH_ACTV_RPC_PASS_ARG(buflen, __actv_local_var_buflen));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_update_opts(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, char *ubuf, size_t buflen)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	__maybe_unused char *__actv_local_var_ubuf = ubuf;
	size_t __maybe_unused __actv_local_var_buflen = buflen;
	__ret = ____actvcapcall_drvcall_devhost_transfs_update_opts(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(ubuf, __actv_local_var_ubuf), __ARCH_ACTV_RPC_PASS_ARG(buflen, __actv_local_var_buflen));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_transfs_iommap(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_transfs_iommap(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_transfs_iommap(unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, unsigned long addr, unsigned long long vspace_rref, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	unsigned long __maybe_unused __actv_local_var_addr = addr;
	unsigned long long __maybe_unused __actv_local_var_vspace_rref = vspace_rref;
	unsigned long __maybe_unused __actv_local_var_length = length;
	unsigned long __maybe_unused __actv_local_var_prot = prot;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned long __maybe_unused __actv_local_var_offset = offset;
	__ret = ____actvcall_drvcall_devhost_transfs_iommap(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(vspace_rref, __actv_local_var_vspace_rref), __ARCH_ACTV_RPC_PASS_ARG(length, __actv_local_var_length), __ARCH_ACTV_RPC_PASS_ARG(prot, __actv_local_var_prot), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_transfs_iommap(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int s_index, unsigned int f_index, unsigned long addr, unsigned long long vspace_rref, unsigned long length, unsigned long prot, unsigned int flags, unsigned long offset)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_s_index = s_index;
	unsigned int __maybe_unused __actv_local_var_f_index = f_index;
	unsigned long __maybe_unused __actv_local_var_addr = addr;
	unsigned long long __maybe_unused __actv_local_var_vspace_rref = vspace_rref;
	unsigned long __maybe_unused __actv_local_var_length = length;
	unsigned long __maybe_unused __actv_local_var_prot = prot;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	unsigned long __maybe_unused __actv_local_var_offset = offset;
	__ret = ____actvcapcall_drvcall_devhost_transfs_iommap(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(s_index, __actv_local_var_s_index), __ARCH_ACTV_RPC_PASS_ARG(f_index, __actv_local_var_f_index), __ARCH_ACTV_RPC_PASS_ARG(addr, __actv_local_var_addr), __ARCH_ACTV_RPC_PASS_ARG(vspace_rref, __actv_local_var_vspace_rref), __ARCH_ACTV_RPC_PASS_ARG(length, __actv_local_var_length), __ARCH_ACTV_RPC_PASS_ARG(prot, __actv_local_var_prot), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_shrinker_query(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_shrinker_query(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_shrinker_query(unsigned long long __rpc_info, void *__buf, int type)
{
	int __ret;
	int __maybe_unused __actv_local_var_type = type;
	__ret = ____actvcall_drvcall_devhost_shrinker_query(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_shrinker_query(rref_t __rref, unsigned long long __rpc_info, void *__buf, int type)
{
	int __ret;
	int __maybe_unused __actv_local_var_type = type;
	__ret = ____actvcapcall_drvcall_devhost_shrinker_query(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(type, __actv_local_var_type));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_shrinker_shrink(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_shrinker_shrink(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_shrinker_shrink(unsigned long long __rpc_info, void *__buf, int max_nr)
{
	int __ret;
	int __maybe_unused __actv_local_var_max_nr = max_nr;
	__ret = ____actvcall_drvcall_devhost_shrinker_shrink(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(max_nr, __actv_local_var_max_nr));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_shrinker_shrink(rref_t __rref, unsigned long long __rpc_info, void *__buf, int max_nr)
{
	int __ret;
	int __maybe_unused __actv_local_var_max_nr = max_nr;
	__ret = ____actvcapcall_drvcall_devhost_shrinker_shrink(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(max_nr, __actv_local_var_max_nr));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_process_revoke(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_process_revoke(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_process_revoke(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	pid_t __maybe_unused __actv_local_var_pid = pid;
	uid_t __maybe_unused __actv_local_var_uid = uid;
	__ret = ____actvcall_drvcall_devhost_process_revoke(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx), __ARCH_ACTV_RPC_PASS_ARG(pid, __actv_local_var_pid), __ARCH_ACTV_RPC_PASS_ARG(uid, __actv_local_var_uid));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_process_revoke(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, pid_t pid, uid_t uid)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	pid_t __maybe_unused __actv_local_var_pid = pid;
	uid_t __maybe_unused __actv_local_var_uid = uid;
	__ret = ____actvcapcall_drvcall_devhost_process_revoke(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx), __ARCH_ACTV_RPC_PASS_ARG(pid, __actv_local_var_pid), __ARCH_ACTV_RPC_PASS_ARG(uid, __actv_local_var_uid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_sec_mgmt(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_sec_mgmt(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_sec_mgmt(unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_module = module;
	unsigned int __maybe_unused __actv_local_var_command = command;
	__ret = ____actvcall_drvcall_devhost_sec_mgmt(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(module, __actv_local_var_module), __ARCH_ACTV_RPC_PASS_ARG(command, __actv_local_var_command));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_sec_mgmt(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_module = module;
	unsigned int __maybe_unused __actv_local_var_command = command;
	__ret = ____actvcapcall_drvcall_devhost_sec_mgmt(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(module, __actv_local_var_module), __ARCH_ACTV_RPC_PASS_ARG(command, __actv_local_var_command));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devhost_sec_test_hkip_seharmony_prot_policydb(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_tests_hkip_protection(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_tests_hkip_protection(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_tests_hkip_protection(unsigned long long __rpc_info, void *__buf, unsigned long test_id)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_test_id = test_id;
	__ret = ____actvcall_drvcall_devhost_tests_hkip_protection(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(test_id, __actv_local_var_test_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_tests_hkip_protection(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long test_id)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_test_id = test_id;
	__ret = ____actvcapcall_drvcall_devhost_tests_hkip_protection(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(test_id, __actv_local_var_test_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_sec_test_hkip_roafter_prot(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_sec_test_hkip_roafter_prot(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_sec_test_hkip_roafter_prot(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devhost_sec_test_hkip_roafter_prot(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_sec_test_hkip_roafter_prot(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devhost_sec_test_hkip_roafter_prot(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_drvcall_devhost_sec_sync_fastpath_open(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_sec_sync_fastpath_open(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_sec_sync_fastpath_open(unsigned long long __rpc_info, void *__buf, unsigned long long filp, unsigned int task_sid, unsigned int state)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_filp = filp;
	unsigned int __maybe_unused __actv_local_var_task_sid = task_sid;
	unsigned int __maybe_unused __actv_local_var_state = state;
	__ret = ____actvcall_drvcall_devhost_sec_sync_fastpath_open(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(filp, __actv_local_var_filp), __ARCH_ACTV_RPC_PASS_ARG(task_sid, __actv_local_var_task_sid), __ARCH_ACTV_RPC_PASS_ARG(state, __actv_local_var_state));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_sec_sync_fastpath_open(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long filp, unsigned int task_sid, unsigned int state)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_filp = filp;
	unsigned int __maybe_unused __actv_local_var_task_sid = task_sid;
	unsigned int __maybe_unused __actv_local_var_state = state;
	__ret = ____actvcapcall_drvcall_devhost_sec_sync_fastpath_open(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(filp, __actv_local_var_filp), __ARCH_ACTV_RPC_PASS_ARG(task_sid, __actv_local_var_task_sid), __ARCH_ACTV_RPC_PASS_ARG(state, __actv_local_var_state));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_drvcall_devhost_sec_sync_fastpath_dev(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_sec_sync_fastpath_dev(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_sec_sync_fastpath_dev(unsigned long long __rpc_info, void *__buf, int devid, unsigned int task_sid, unsigned int state)
{
	int __ret;
	int __maybe_unused __actv_local_var_devid = devid;
	unsigned int __maybe_unused __actv_local_var_task_sid = task_sid;
	unsigned int __maybe_unused __actv_local_var_state = state;
	__ret = ____actvcall_drvcall_devhost_sec_sync_fastpath_dev(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devid, __actv_local_var_devid), __ARCH_ACTV_RPC_PASS_ARG(task_sid, __actv_local_var_task_sid), __ARCH_ACTV_RPC_PASS_ARG(state, __actv_local_var_state));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_sec_sync_fastpath_dev(rref_t __rref, unsigned long long __rpc_info, void *__buf, int devid, unsigned int task_sid, unsigned int state)
{
	int __ret;
	int __maybe_unused __actv_local_var_devid = devid;
	unsigned int __maybe_unused __actv_local_var_task_sid = task_sid;
	unsigned int __maybe_unused __actv_local_var_state = state;
	__ret = ____actvcapcall_drvcall_devhost_sec_sync_fastpath_dev(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(devid, __actv_local_var_devid), __ARCH_ACTV_RPC_PASS_ARG(task_sid, __actv_local_var_task_sid), __ARCH_ACTV_RPC_PASS_ARG(state, __actv_local_var_state));
	return __ret;
}

#ifdef __arm__
#endif
static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_trace_enable_disable_ldk_event(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_trace_enable_disable_ldk_event(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_trace_enable_disable_ldk_event(unsigned long long __rpc_info, void *__buf, unsigned int tp_index, bool enable)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tp_index = tp_index;
	bool __maybe_unused __actv_local_var_enable = enable;
	__ret = ____actvcall_drvcall_devhost_trace_enable_disable_ldk_event(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tp_index, __actv_local_var_tp_index), __ARCH_ACTV_RPC_PASS_ARG(enable, __actv_local_var_enable));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_trace_enable_disable_ldk_event(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tp_index, bool enable)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tp_index = tp_index;
	bool __maybe_unused __actv_local_var_enable = enable;
	__ret = ____actvcapcall_drvcall_devhost_trace_enable_disable_ldk_event(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tp_index, __actv_local_var_tp_index), __ARCH_ACTV_RPC_PASS_ARG(enable, __actv_local_var_enable));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_trace_ldk_event_format(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_trace_ldk_event_format(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_trace_ldk_event_format(unsigned long long __rpc_info, void *__buf, unsigned int tp_index, char *format)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tp_index = tp_index;
	__maybe_unused char *__actv_local_var_format = format;
	__ret = ____actvcall_drvcall_devhost_trace_ldk_event_format(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tp_index, __actv_local_var_tp_index), __ARCH_ACTV_RPC_PASS_ARG(format, __actv_local_var_format));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_trace_ldk_event_format(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tp_index, char *format)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tp_index = tp_index;
	__maybe_unused char *__actv_local_var_format = format;
	__ret = ____actvcapcall_drvcall_devhost_trace_ldk_event_format(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tp_index, __actv_local_var_tp_index), __ARCH_ACTV_RPC_PASS_ARG(format, __actv_local_var_format));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_trace_print_ldk_event(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_trace_print_ldk_event(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_trace_print_ldk_event(unsigned long long __rpc_info, void *__buf, unsigned int tp_index, void *entry, size_t entry_size, char *event_st)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tp_index = tp_index;
	__maybe_unused void *__actv_local_var_entry = entry;
	size_t __maybe_unused __actv_local_var_entry_size = entry_size;
	__maybe_unused char *__actv_local_var_event_st = event_st;
	__ret = ____actvcall_drvcall_devhost_trace_print_ldk_event(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tp_index, __actv_local_var_tp_index), __ARCH_ACTV_RPC_PASS_ARG(entry, __actv_local_var_entry), __ARCH_ACTV_RPC_PASS_ARG(entry_size, __actv_local_var_entry_size), __ARCH_ACTV_RPC_PASS_ARG(event_st, __actv_local_var_event_st));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_trace_print_ldk_event(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int tp_index, void *entry, size_t entry_size, char *event_st)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_tp_index = tp_index;
	__maybe_unused void *__actv_local_var_entry = entry;
	size_t __maybe_unused __actv_local_var_entry_size = entry_size;
	__maybe_unused char *__actv_local_var_event_st = event_st;
	__ret = ____actvcapcall_drvcall_devhost_trace_print_ldk_event(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(tp_index, __actv_local_var_tp_index), __ARCH_ACTV_RPC_PASS_ARG(entry, __actv_local_var_entry), __ARCH_ACTV_RPC_PASS_ARG(entry_size, __actv_local_var_entry_size), __ARCH_ACTV_RPC_PASS_ARG(event_st, __actv_local_var_event_st));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_file_info_query_dmabuf(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_file_info_query_dmabuf(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_file_info_query_dmabuf(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_idx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_idx = file_idx;
	__ret = ____actvcall_drvcall_devhost_file_info_query_dmabuf(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_idx, __actv_local_var_file_idx));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_file_info_query_dmabuf(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_idx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_idx = file_idx;
	__ret = ____actvcapcall_drvcall_devhost_file_info_query_dmabuf(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_idx, __actv_local_var_file_idx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_file_info_query_fence(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_file_info_query_fence(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_file_info_query_fence(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_idx, unsigned int offset)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_idx = file_idx;
	unsigned int __maybe_unused __actv_local_var_offset = offset;
	__ret = ____actvcall_drvcall_devhost_file_info_query_fence(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_idx, __actv_local_var_file_idx), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_file_info_query_fence(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_idx, unsigned int offset)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_idx = file_idx;
	unsigned int __maybe_unused __actv_local_var_offset = offset;
	__ret = ____actvcapcall_drvcall_devhost_file_info_query_fence(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_idx, __actv_local_var_file_idx), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_file_info_query_ion(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_file_info_query_ion(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_file_info_query_ion(unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_idx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_idx = file_idx;
	__ret = ____actvcall_drvcall_devhost_file_info_query_ion(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_idx, __actv_local_var_file_idx));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_file_info_query_ion(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int fastpath, unsigned long long file_idx)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_fastpath = fastpath;
	unsigned long long __maybe_unused __actv_local_var_file_idx = file_idx;
	__ret = ____actvcapcall_drvcall_devhost_file_info_query_ion(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fastpath, __actv_local_var_fastpath), __ARCH_ACTV_RPC_PASS_ARG(file_idx, __actv_local_var_file_idx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_file_info_query_meminfo(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_file_info_query_meminfo(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_file_info_query_meminfo(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_drvcall_devhost_file_info_query_meminfo(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_file_info_query_meminfo(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_drvcall_devhost_file_info_query_meminfo(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_file_info_lowmem_debug(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_file_info_lowmem_debug(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_file_info_lowmem_debug(unsigned long long __rpc_info, void *__buf, int oom_score_adj, unsigned long long mem_total, bool is_emergency)
{
	int __ret;
	int __maybe_unused __actv_local_var_oom_score_adj = oom_score_adj;
	unsigned long long __maybe_unused __actv_local_var_mem_total = mem_total;
	bool __maybe_unused __actv_local_var_is_emergency = is_emergency;
	__ret = ____actvcall_drvcall_devhost_file_info_lowmem_debug(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(oom_score_adj, __actv_local_var_oom_score_adj), __ARCH_ACTV_RPC_PASS_ARG(mem_total, __actv_local_var_mem_total), __ARCH_ACTV_RPC_PASS_ARG(is_emergency, __actv_local_var_is_emergency));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_file_info_lowmem_debug(rref_t __rref, unsigned long long __rpc_info, void *__buf, int oom_score_adj, unsigned long long mem_total, bool is_emergency)
{
	int __ret;
	int __maybe_unused __actv_local_var_oom_score_adj = oom_score_adj;
	unsigned long long __maybe_unused __actv_local_var_mem_total = mem_total;
	bool __maybe_unused __actv_local_var_is_emergency = is_emergency;
	__ret = ____actvcapcall_drvcall_devhost_file_info_lowmem_debug(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(oom_score_adj, __actv_local_var_oom_score_adj), __ARCH_ACTV_RPC_PASS_ARG(mem_total, __actv_local_var_mem_total), __ARCH_ACTV_RPC_PASS_ARG(is_emergency, __actv_local_var_is_emergency));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_drvcall_devhost_file_info_query_gpu(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_file_info_query_gpu(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_file_info_query_gpu(unsigned long long __rpc_info, void *__buf, unsigned long pid)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_pid = pid;
	__ret = ____actvcall_drvcall_devhost_file_info_query_gpu(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(pid, __actv_local_var_pid));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_file_info_query_gpu(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long pid)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_pid = pid;
	__ret = ____actvcapcall_drvcall_devhost_file_info_query_gpu(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(pid, __actv_local_var_pid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_probe_handle_brk(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_probe_handle_brk(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_probe_handle_brk(unsigned long long __rpc_info, void *__buf, struct arch_uctx *uctx)
{
	int __ret;
	__maybe_unused struct arch_uctx *__actv_local_var_uctx = uctx;
	__ret = ____actvcall_drvcall_devhost_probe_handle_brk(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(uctx, __actv_local_var_uctx));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_probe_handle_brk(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct arch_uctx *uctx)
{
	int __ret;
	__maybe_unused struct arch_uctx *__actv_local_var_uctx = uctx;
	__ret = ____actvcapcall_drvcall_devhost_probe_handle_brk(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(uctx, __actv_local_var_uctx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_probe_handle_ss_brk(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_probe_handle_ss_brk(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_probe_handle_ss_brk(unsigned long long __rpc_info, void *__buf, struct arch_uctx *uctx)
{
	int __ret;
	__maybe_unused struct arch_uctx *__actv_local_var_uctx = uctx;
	__ret = ____actvcall_drvcall_devhost_probe_handle_ss_brk(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(uctx, __actv_local_var_uctx));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_probe_handle_ss_brk(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct arch_uctx *uctx)
{
	int __ret;
	__maybe_unused struct arch_uctx *__actv_local_var_uctx = uctx;
	__ret = ____actvcapcall_drvcall_devhost_probe_handle_ss_brk(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(uctx, __actv_local_var_uctx));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_drvcall_devhost_fileguard_message_notify(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_drvcall_devhost_fileguard_message_notify(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_drvcall_devhost_fileguard_message_notify(unsigned long long __rpc_info, void *__buf, void *message, size_t len)
{
	int __ret;
	__maybe_unused void *__actv_local_var_message = message;
	size_t __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcall_drvcall_devhost_fileguard_message_notify(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(message, __actv_local_var_message), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

int __internal_actvcall __actvcapcall_drvcall_devhost_fileguard_message_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *message, size_t len)
{
	int __ret;
	__maybe_unused void *__actv_local_var_message = message;
	size_t __maybe_unused __actv_local_var_len = len;
	__ret = ____actvcapcall_drvcall_devhost_fileguard_message_notify(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(message, __actv_local_var_message), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len));
	return __ret;
}

#endif
#endif
