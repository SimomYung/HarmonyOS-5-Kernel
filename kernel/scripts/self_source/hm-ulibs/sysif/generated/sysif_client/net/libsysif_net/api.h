/*
* Automatically generated file; DO NOT EDIT.
* Automatically generated sysif apis for hmnet
* Using command: ../recipe-sysroot-native/usr/bin/hm-dsl/hm-dsl sysif ../kernel/hongmeng/hm-network/ulibs/libevent/include/libsysif_net/api.sysif -I ../kernel/hongmeng/hm-network/ulibs/libevent/include --api-dest ../image/usr/include/generated/sysif_client/net/libsysif_net/api.h --server-dest ../image/usr/include/generated/sysif_server/net/libsysif_net/service.h --dump-callno ../image/usr/include/generated/dump_info/net/libsysif_net/l2h_map.h --legacy-mode
* Create: Fri Oct 31 10:27:10 2025
*/
#ifndef LIBSYSIF_HMNET_API_H
#define LIBSYSIF_HMNET_API_H
#ifndef LIBSYSIF_HMNET_COMMON_H
#define LIBSYSIF_HMNET_COMMON_H
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
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <net/if.h>


#define NETTEST_BUF_LEN		64
struct sysif_nettest {
	char buf[NETTEST_BUF_LEN];
};
struct ux_socket;



#include <netinet/in.h>
#include <net/if.h>

#define PROTOCOL_NAME_LEN_MAX	16
struct sysif_netif {
	char ifname[IF_NAMESIZE];
};

struct sysif_protocol {
	char protocol[PROTOCOL_NAME_LEN_MAX];
};




#include <net/if.h>
#include <hmnet/hmnet_devices.h>

struct netdev_ifname {
	char name[IF_NAMESIZE];
};




enum vm_notify_cmd {
	VM_NOTIFY_VM_TX = 1,
	VM_NOTIFY_VQ_READY = 2,
};


raw_static_assert(sizeof(struct sysif_protocol) <= 96,
                    netcall_cfg_show_stats_arg_ss_protocol_is_too_large);
raw_static_assert(sizeof(enum hm_netdev_event) <= 96,
                    netcall_netdev_notify_arg_event_is_too_large);
raw_static_assert(sizeof(struct netdev_ifname) <= 96,
                    netcall_netdev_notify_arg_ifname_is_too_large);
struct __actvret_netcall_socket_revoke {
};

struct __actvret_netcall_socket_fork {
};

struct __actvret_netcall_socket_dev_ioctl {
};

struct __actvret_netcall_socket_send_fd {
	int recv_fd;
};

struct __actvret_netcall_socket_put_send_fd {
};

struct __actvret_netcall_socket_proc_unauthoried_files {
};

struct __actvret_netcall_socket_query_info {
};

struct __actvret_netcall_socket_sys_read {
};

struct __actvret_netcall_socket_sys_write {
};

struct __actvret_netcall_socket_sys_ioctl {
};

struct __actvret_netcall_socket_mmap {
	uintptr_t rvaddr;
};

struct __actvret_netcall_socket_attach {
};

struct __actvret_netcall_socket_get {
	void *sock;
};

struct __actvret_netcall_socket_put {
};

struct __actvret_netcall_socket_socket {
};

struct __actvret_netcall_socket_socketpair {
};

struct __actvret_netcall_socket_close {
};

struct __actvret_netcall_socket_shutdown {
};

struct __actvret_netcall_socket_bind {
};

struct __actvret_netcall_socket_listen {
};

struct __actvret_netcall_socket_accept {
};

struct __actvret_netcall_socket_accept4 {
};

struct __actvret_netcall_socket_connect {
};

struct __actvret_netcall_socket_sendmmsg {
};

struct __actvret_netcall_socket_recvmmsg {
};

struct __actvret_netcall_socket_read {
};

struct __actvret_netcall_socket_write {
};

struct __actvret_netcall_socket_sendmsg {
};

struct __actvret_netcall_socket_recvmsg {
};

struct __actvret_netcall_socket_sendto {
};

struct __actvret_netcall_socket_recvfrom {
};

struct __actvret_netcall_socket_readv {
};

struct __actvret_netcall_socket_writev {
};

struct __actvret_netcall_socket_getsockopt {
};

struct __actvret_netcall_socket_setsockopt {
};

struct __actvret_netcall_socket_getsockname {
};

struct __actvret_netcall_socket_getpeername {
};

struct __actvret_netcall_socket_ioctl {
};

struct __actvret_netcall_socket_fcntl {
};

struct __actvret_netcall_socket_fchown {
};

struct __actvret_netcall_socket_dup {
};

struct __actvret_netcall_socket_dup3 {
};

struct __actvret_netcall_socket_fstat {
};

#ifdef __arm__
struct __actvret_netcall_socket_send {
};

struct __actvret_netcall_socket_recv {
};

struct __actvret_netcall_socket_fcntl64 {
};

struct __actvret_netcall_socket_fchown32 {
};

struct __actvret_netcall_socket_dup2 {
};

struct __actvret_netcall_socket_fstat64 {
};

#endif
struct __actvret_netcall_epoll_recheck_revents {
};

struct __actvret_netcall_cfg_loglevel {
};

struct __actvret_netcall_cfg_show_stats {
};

struct __actvret_netcall_cfg_getifaddrs {
};

struct __actvret_netcall_netdev_rx {
};

struct __actvret_netcall_netdev_notify {
};

struct __actvret_netcall_netdev_rx_ex_slowdata {
};

struct __actvret_netcall_procfs_read {
};

struct __actvret_netcall_procfs_write {
};

struct __actvret_netcall_procfs_fds_read {
};

struct __actvret_netcall_procfs_get_fd_path {
};

struct __actvret_netcall_netlink_register_generic {
	unsigned short familyid;
	unsigned int mcgrps_offset;
};

struct __actvret_netcall_netlink_unregister_generic {
};

struct __actvret_netcall_netlink_unicast {
};

struct __actvret_netcall_netlink_broadcast {
};

struct __actvret_netcall_netlink_create_service {
};

struct __actvret_netcall_netlink_release_service {
};

struct __actvret_netcall_netlink_unicast_nowait {
};

struct __actvret_netcall_vsock_register_vm {
};

struct __actvret_netcall_vsock_vm_notify {
};

struct __actvret_netcall_vsock_unregister_vm {
};

struct __actvret_netcall_ldk_domain_register {
};

struct __actvret_netcall_procfs_create_ldk_entry {
};

struct __actvret_netcall_procfs_delete_ldk_entry_specific {
};

struct __actvret_netcall_procfs_create_ldk_entries {
};

struct __actvret_netcall_ldk_bpf {
};

struct __actvret_netcall_sec_mgmt {
};

struct __actvret_netcall_namespace_new_ns {
};

struct __actvret_netcall_namespace_destroy_ns {
};

#ifdef __aarch64__
union __actvret_netcall {
	struct __actvret_netcall_socket_revoke socket_revoke;
	struct __actvret_netcall_socket_fork socket_fork;
	struct __actvret_netcall_socket_dev_ioctl socket_dev_ioctl;
	struct __actvret_netcall_socket_send_fd socket_send_fd;
	struct __actvret_netcall_socket_put_send_fd socket_put_send_fd;
	struct __actvret_netcall_socket_proc_unauthoried_files socket_proc_unauthoried_files;
	struct __actvret_netcall_socket_query_info socket_query_info;
	struct __actvret_netcall_socket_sys_read socket_sys_read;
	struct __actvret_netcall_socket_sys_write socket_sys_write;
	struct __actvret_netcall_socket_sys_ioctl socket_sys_ioctl;
	struct __actvret_netcall_socket_mmap socket_mmap;
	struct __actvret_netcall_socket_attach socket_attach;
	struct __actvret_netcall_socket_get socket_get;
	struct __actvret_netcall_socket_put socket_put;
	struct __actvret_netcall_socket_socket socket_socket;
	struct __actvret_netcall_socket_socketpair socket_socketpair;
	struct __actvret_netcall_socket_close socket_close;
	struct __actvret_netcall_socket_shutdown socket_shutdown;
	struct __actvret_netcall_socket_bind socket_bind;
	struct __actvret_netcall_socket_listen socket_listen;
	struct __actvret_netcall_socket_accept socket_accept;
	struct __actvret_netcall_socket_accept4 socket_accept4;
	struct __actvret_netcall_socket_connect socket_connect;
	struct __actvret_netcall_socket_sendmmsg socket_sendmmsg;
	struct __actvret_netcall_socket_recvmmsg socket_recvmmsg;
	struct __actvret_netcall_socket_read socket_read;
	struct __actvret_netcall_socket_write socket_write;
	struct __actvret_netcall_socket_sendmsg socket_sendmsg;
	struct __actvret_netcall_socket_recvmsg socket_recvmsg;
	struct __actvret_netcall_socket_sendto socket_sendto;
	struct __actvret_netcall_socket_recvfrom socket_recvfrom;
	struct __actvret_netcall_socket_readv socket_readv;
	struct __actvret_netcall_socket_writev socket_writev;
	struct __actvret_netcall_socket_getsockopt socket_getsockopt;
	struct __actvret_netcall_socket_setsockopt socket_setsockopt;
	struct __actvret_netcall_socket_getsockname socket_getsockname;
	struct __actvret_netcall_socket_getpeername socket_getpeername;
	struct __actvret_netcall_socket_ioctl socket_ioctl;
	struct __actvret_netcall_socket_fcntl socket_fcntl;
	struct __actvret_netcall_socket_fchown socket_fchown;
	struct __actvret_netcall_socket_dup socket_dup;
	struct __actvret_netcall_socket_dup3 socket_dup3;
	struct __actvret_netcall_socket_fstat socket_fstat;
	struct __actvret_netcall_epoll_recheck_revents epoll_recheck_revents;
	struct __actvret_netcall_cfg_loglevel cfg_loglevel;
	struct __actvret_netcall_cfg_show_stats cfg_show_stats;
	struct __actvret_netcall_cfg_getifaddrs cfg_getifaddrs;
	struct __actvret_netcall_netdev_rx netdev_rx;
	struct __actvret_netcall_netdev_notify netdev_notify;
	struct __actvret_netcall_netdev_rx_ex_slowdata netdev_rx_ex_slowdata;
	struct __actvret_netcall_procfs_read procfs_read;
	struct __actvret_netcall_procfs_write procfs_write;
	struct __actvret_netcall_procfs_fds_read procfs_fds_read;
	struct __actvret_netcall_procfs_get_fd_path procfs_get_fd_path;
	struct __actvret_netcall_netlink_register_generic netlink_register_generic;
	struct __actvret_netcall_netlink_unregister_generic netlink_unregister_generic;
	struct __actvret_netcall_netlink_unicast netlink_unicast;
	struct __actvret_netcall_netlink_broadcast netlink_broadcast;
	struct __actvret_netcall_netlink_create_service netlink_create_service;
	struct __actvret_netcall_netlink_release_service netlink_release_service;
	struct __actvret_netcall_netlink_unicast_nowait netlink_unicast_nowait;
	struct __actvret_netcall_vsock_register_vm vsock_register_vm;
	struct __actvret_netcall_vsock_vm_notify vsock_vm_notify;
	struct __actvret_netcall_vsock_unregister_vm vsock_unregister_vm;
	struct __actvret_netcall_ldk_domain_register ldk_domain_register;
	struct __actvret_netcall_procfs_create_ldk_entry procfs_create_ldk_entry;
	struct __actvret_netcall_procfs_delete_ldk_entry_specific procfs_delete_ldk_entry_specific;
	struct __actvret_netcall_procfs_create_ldk_entries procfs_create_ldk_entries;
	struct __actvret_netcall_ldk_bpf ldk_bpf;
	struct __actvret_netcall_sec_mgmt sec_mgmt;
	struct __actvret_netcall_namespace_new_ns namespace_new_ns;
	struct __actvret_netcall_namespace_destroy_ns namespace_destroy_ns;
};

#elif defined(__arm__)
union __actvret_netcall {
	struct __actvret_netcall_socket_revoke socket_revoke;
	struct __actvret_netcall_socket_fork socket_fork;
	struct __actvret_netcall_socket_dev_ioctl socket_dev_ioctl;
	struct __actvret_netcall_socket_send_fd socket_send_fd;
	struct __actvret_netcall_socket_put_send_fd socket_put_send_fd;
	struct __actvret_netcall_socket_proc_unauthoried_files socket_proc_unauthoried_files;
	struct __actvret_netcall_socket_query_info socket_query_info;
	struct __actvret_netcall_socket_sys_read socket_sys_read;
	struct __actvret_netcall_socket_sys_write socket_sys_write;
	struct __actvret_netcall_socket_sys_ioctl socket_sys_ioctl;
	struct __actvret_netcall_socket_mmap socket_mmap;
	struct __actvret_netcall_socket_attach socket_attach;
	struct __actvret_netcall_socket_get socket_get;
	struct __actvret_netcall_socket_put socket_put;
	struct __actvret_netcall_socket_socket socket_socket;
	struct __actvret_netcall_socket_socketpair socket_socketpair;
	struct __actvret_netcall_socket_close socket_close;
	struct __actvret_netcall_socket_shutdown socket_shutdown;
	struct __actvret_netcall_socket_bind socket_bind;
	struct __actvret_netcall_socket_listen socket_listen;
	struct __actvret_netcall_socket_accept socket_accept;
	struct __actvret_netcall_socket_accept4 socket_accept4;
	struct __actvret_netcall_socket_connect socket_connect;
	struct __actvret_netcall_socket_sendmmsg socket_sendmmsg;
	struct __actvret_netcall_socket_recvmmsg socket_recvmmsg;
	struct __actvret_netcall_socket_read socket_read;
	struct __actvret_netcall_socket_write socket_write;
	struct __actvret_netcall_socket_sendmsg socket_sendmsg;
	struct __actvret_netcall_socket_recvmsg socket_recvmsg;
	struct __actvret_netcall_socket_sendto socket_sendto;
	struct __actvret_netcall_socket_recvfrom socket_recvfrom;
	struct __actvret_netcall_socket_readv socket_readv;
	struct __actvret_netcall_socket_writev socket_writev;
	struct __actvret_netcall_socket_getsockopt socket_getsockopt;
	struct __actvret_netcall_socket_setsockopt socket_setsockopt;
	struct __actvret_netcall_socket_getsockname socket_getsockname;
	struct __actvret_netcall_socket_getpeername socket_getpeername;
	struct __actvret_netcall_socket_ioctl socket_ioctl;
	struct __actvret_netcall_socket_fcntl socket_fcntl;
	struct __actvret_netcall_socket_fchown socket_fchown;
	struct __actvret_netcall_socket_dup socket_dup;
	struct __actvret_netcall_socket_dup3 socket_dup3;
	struct __actvret_netcall_socket_fstat socket_fstat;
	struct __actvret_netcall_epoll_recheck_revents epoll_recheck_revents;
	struct __actvret_netcall_cfg_loglevel cfg_loglevel;
	struct __actvret_netcall_cfg_show_stats cfg_show_stats;
	struct __actvret_netcall_cfg_getifaddrs cfg_getifaddrs;
	struct __actvret_netcall_netdev_rx netdev_rx;
	struct __actvret_netcall_netdev_notify netdev_notify;
	struct __actvret_netcall_netdev_rx_ex_slowdata netdev_rx_ex_slowdata;
	struct __actvret_netcall_procfs_read procfs_read;
	struct __actvret_netcall_procfs_write procfs_write;
	struct __actvret_netcall_procfs_fds_read procfs_fds_read;
	struct __actvret_netcall_procfs_get_fd_path procfs_get_fd_path;
	struct __actvret_netcall_netlink_register_generic netlink_register_generic;
	struct __actvret_netcall_netlink_unregister_generic netlink_unregister_generic;
	struct __actvret_netcall_netlink_unicast netlink_unicast;
	struct __actvret_netcall_netlink_broadcast netlink_broadcast;
	struct __actvret_netcall_netlink_create_service netlink_create_service;
	struct __actvret_netcall_netlink_release_service netlink_release_service;
	struct __actvret_netcall_netlink_unicast_nowait netlink_unicast_nowait;
	struct __actvret_netcall_vsock_register_vm vsock_register_vm;
	struct __actvret_netcall_vsock_vm_notify vsock_vm_notify;
	struct __actvret_netcall_vsock_unregister_vm vsock_unregister_vm;
	struct __actvret_netcall_ldk_domain_register ldk_domain_register;
	struct __actvret_netcall_procfs_create_ldk_entry procfs_create_ldk_entry;
	struct __actvret_netcall_procfs_delete_ldk_entry_specific procfs_delete_ldk_entry_specific;
	struct __actvret_netcall_procfs_create_ldk_entries procfs_create_ldk_entries;
	struct __actvret_netcall_ldk_bpf ldk_bpf;
	struct __actvret_netcall_sec_mgmt sec_mgmt;
	struct __actvret_netcall_namespace_new_ns namespace_new_ns;
	struct __actvret_netcall_namespace_destroy_ns namespace_destroy_ns;
	struct __actvret_netcall_socket_send socket_send;
	struct __actvret_netcall_socket_recv socket_recv;
	struct __actvret_netcall_socket_fcntl64 socket_fcntl64;
	struct __actvret_netcall_socket_fchown32 socket_fchown32;
	struct __actvret_netcall_socket_dup2 socket_dup2;
	struct __actvret_netcall_socket_fstat64 socket_fstat64;
};

#else
union __actvret_netcall {
	struct __actvret_netcall_socket_revoke socket_revoke;
	struct __actvret_netcall_socket_fork socket_fork;
	struct __actvret_netcall_socket_dev_ioctl socket_dev_ioctl;
	struct __actvret_netcall_socket_send_fd socket_send_fd;
	struct __actvret_netcall_socket_put_send_fd socket_put_send_fd;
	struct __actvret_netcall_socket_proc_unauthoried_files socket_proc_unauthoried_files;
	struct __actvret_netcall_socket_query_info socket_query_info;
	struct __actvret_netcall_socket_sys_read socket_sys_read;
	struct __actvret_netcall_socket_sys_write socket_sys_write;
	struct __actvret_netcall_socket_sys_ioctl socket_sys_ioctl;
	struct __actvret_netcall_socket_mmap socket_mmap;
	struct __actvret_netcall_socket_attach socket_attach;
	struct __actvret_netcall_socket_get socket_get;
	struct __actvret_netcall_socket_put socket_put;
	struct __actvret_netcall_socket_socket socket_socket;
	struct __actvret_netcall_socket_socketpair socket_socketpair;
	struct __actvret_netcall_socket_close socket_close;
	struct __actvret_netcall_socket_shutdown socket_shutdown;
	struct __actvret_netcall_socket_bind socket_bind;
	struct __actvret_netcall_socket_listen socket_listen;
	struct __actvret_netcall_socket_accept socket_accept;
	struct __actvret_netcall_socket_accept4 socket_accept4;
	struct __actvret_netcall_socket_connect socket_connect;
	struct __actvret_netcall_socket_sendmmsg socket_sendmmsg;
	struct __actvret_netcall_socket_recvmmsg socket_recvmmsg;
	struct __actvret_netcall_socket_read socket_read;
	struct __actvret_netcall_socket_write socket_write;
	struct __actvret_netcall_socket_sendmsg socket_sendmsg;
	struct __actvret_netcall_socket_recvmsg socket_recvmsg;
	struct __actvret_netcall_socket_sendto socket_sendto;
	struct __actvret_netcall_socket_recvfrom socket_recvfrom;
	struct __actvret_netcall_socket_readv socket_readv;
	struct __actvret_netcall_socket_writev socket_writev;
	struct __actvret_netcall_socket_getsockopt socket_getsockopt;
	struct __actvret_netcall_socket_setsockopt socket_setsockopt;
	struct __actvret_netcall_socket_getsockname socket_getsockname;
	struct __actvret_netcall_socket_getpeername socket_getpeername;
	struct __actvret_netcall_socket_ioctl socket_ioctl;
	struct __actvret_netcall_socket_fcntl socket_fcntl;
	struct __actvret_netcall_socket_fchown socket_fchown;
	struct __actvret_netcall_socket_dup socket_dup;
	struct __actvret_netcall_socket_dup3 socket_dup3;
	struct __actvret_netcall_socket_fstat socket_fstat;
	struct __actvret_netcall_epoll_recheck_revents epoll_recheck_revents;
	struct __actvret_netcall_cfg_loglevel cfg_loglevel;
	struct __actvret_netcall_cfg_show_stats cfg_show_stats;
	struct __actvret_netcall_cfg_getifaddrs cfg_getifaddrs;
	struct __actvret_netcall_netdev_rx netdev_rx;
	struct __actvret_netcall_netdev_notify netdev_notify;
	struct __actvret_netcall_netdev_rx_ex_slowdata netdev_rx_ex_slowdata;
	struct __actvret_netcall_procfs_read procfs_read;
	struct __actvret_netcall_procfs_write procfs_write;
	struct __actvret_netcall_procfs_fds_read procfs_fds_read;
	struct __actvret_netcall_procfs_get_fd_path procfs_get_fd_path;
	struct __actvret_netcall_netlink_register_generic netlink_register_generic;
	struct __actvret_netcall_netlink_unregister_generic netlink_unregister_generic;
	struct __actvret_netcall_netlink_unicast netlink_unicast;
	struct __actvret_netcall_netlink_broadcast netlink_broadcast;
	struct __actvret_netcall_netlink_create_service netlink_create_service;
	struct __actvret_netcall_netlink_release_service netlink_release_service;
	struct __actvret_netcall_netlink_unicast_nowait netlink_unicast_nowait;
	struct __actvret_netcall_vsock_register_vm vsock_register_vm;
	struct __actvret_netcall_vsock_vm_notify vsock_vm_notify;
	struct __actvret_netcall_vsock_unregister_vm vsock_unregister_vm;
	struct __actvret_netcall_ldk_domain_register ldk_domain_register;
	struct __actvret_netcall_procfs_create_ldk_entry procfs_create_ldk_entry;
	struct __actvret_netcall_procfs_delete_ldk_entry_specific procfs_delete_ldk_entry_specific;
	struct __actvret_netcall_procfs_create_ldk_entries procfs_create_ldk_entries;
	struct __actvret_netcall_ldk_bpf ldk_bpf;
	struct __actvret_netcall_sec_mgmt sec_mgmt;
	struct __actvret_netcall_namespace_new_ns namespace_new_ns;
	struct __actvret_netcall_namespace_destroy_ns namespace_destroy_ns;
};

#endif
enum __hmnet_mgr {
	__hmnet_INVAL_mgr=0,
	__hmnet_socket_mgr,
	__hmnet_epoll_mgr,
	__hmnet_cfg_mgr,
	__hmnet_netdev_mgr,
	__hmnet_procfs_mgr,
	__hmnet_netlink_mgr,
	__hmnet_vsock_mgr,
	__hmnet_ldk_mgr,
	__hmnet_sec_mgr,
	__hmnet_namespace_mgr,
	__hmnet_MAX_mgr,
};

#define __hmnet_method_MIN __ACTV_HIGHTABLE_START
#define __hmnet_method_socket_socket (__ACTV_HIGHTABLE_START + 1)
#define __hmnet_method_socket_socketpair (__ACTV_HIGHTABLE_START + 2)
#define __hmnet_method_socket_close (__ACTV_HIGHTABLE_START + 3)
#define __hmnet_method_socket_shutdown (__ACTV_HIGHTABLE_START + 4)
#define __hmnet_method_socket_bind (__ACTV_HIGHTABLE_START + 5)
#define __hmnet_method_socket_listen (__ACTV_HIGHTABLE_START + 6)
#define __hmnet_method_socket_accept (__ACTV_HIGHTABLE_START + 7)
#define __hmnet_method_socket_accept4 (__ACTV_HIGHTABLE_START + 8)
#define __hmnet_method_socket_connect (__ACTV_HIGHTABLE_START + 9)
#define __hmnet_method_socket_sendmmsg (__ACTV_HIGHTABLE_START + 10)
#define __hmnet_method_socket_recvmmsg (__ACTV_HIGHTABLE_START + 11)
#define __hmnet_method_socket_read (__ACTV_HIGHTABLE_START + 12)
#define __hmnet_method_socket_write (__ACTV_HIGHTABLE_START + 13)
#define __hmnet_method_socket_sendmsg (__ACTV_HIGHTABLE_START + 14)
#define __hmnet_method_socket_recvmsg (__ACTV_HIGHTABLE_START + 15)
#define __hmnet_method_socket_sendto (__ACTV_HIGHTABLE_START + 16)
#define __hmnet_method_socket_recvfrom (__ACTV_HIGHTABLE_START + 17)
#define __hmnet_method_socket_readv (__ACTV_HIGHTABLE_START + 18)
#define __hmnet_method_socket_writev (__ACTV_HIGHTABLE_START + 19)
#define __hmnet_method_socket_getsockopt (__ACTV_HIGHTABLE_START + 20)
#define __hmnet_method_socket_setsockopt (__ACTV_HIGHTABLE_START + 21)
#define __hmnet_method_socket_getsockname (__ACTV_HIGHTABLE_START + 22)
#define __hmnet_method_socket_getpeername (__ACTV_HIGHTABLE_START + 23)
#define __hmnet_method_socket_ioctl (__ACTV_HIGHTABLE_START + 24)
#define __hmnet_method_socket_fcntl (__ACTV_HIGHTABLE_START + 25)
#define __hmnet_method_socket_fchown (__ACTV_HIGHTABLE_START + 26)
#define __hmnet_method_socket_dup (__ACTV_HIGHTABLE_START + 27)
#define __hmnet_method_socket_dup3 (__ACTV_HIGHTABLE_START + 28)
#define __hmnet_method_socket_fstat (__ACTV_HIGHTABLE_START + 29)
#define __hmnet_method_socket_send (__ACTV_HIGHTABLE_START + 30)
#define __hmnet_method_socket_recv (__ACTV_HIGHTABLE_START + 31)
#define __hmnet_method_socket_fcntl64 (__ACTV_HIGHTABLE_START + 32)
#define __hmnet_method_socket_fchown32 (__ACTV_HIGHTABLE_START + 33)
#define __hmnet_method_socket_dup2 (__ACTV_HIGHTABLE_START + 34)
#define __hmnet_method_socket_fstat64 (__ACTV_HIGHTABLE_START + 35)
#define __hmnet_method_ldk_bpf (__ACTV_HIGHTABLE_START + 36)
#define __hmnet_method_socket_revoke (__ACTV_HIGHTABLE_START + 37)
#define __hmnet_method_socket_fork (__ACTV_HIGHTABLE_START + 38)
#define __hmnet_method_socket_dev_ioctl (__ACTV_HIGHTABLE_START + 39)
#define __hmnet_method_socket_send_fd (__ACTV_HIGHTABLE_START + 40)
#define __hmnet_method_socket_put_send_fd (__ACTV_HIGHTABLE_START + 41)
#define __hmnet_method_socket_proc_unauthoried_files (__ACTV_HIGHTABLE_START + 42)
#define __hmnet_method_socket_query_info (__ACTV_HIGHTABLE_START + 43)
#define __hmnet_method_socket_sys_read (__ACTV_HIGHTABLE_START + 44)
#define __hmnet_method_socket_sys_write (__ACTV_HIGHTABLE_START + 45)
#define __hmnet_method_socket_sys_ioctl (__ACTV_HIGHTABLE_START + 46)
#define __hmnet_method_socket_mmap (__ACTV_HIGHTABLE_START + 47)
#define __hmnet_method_socket_attach (__ACTV_HIGHTABLE_START + 48)
#define __hmnet_method_socket_get (__ACTV_HIGHTABLE_START + 49)
#define __hmnet_method_socket_put (__ACTV_HIGHTABLE_START + 50)
#define __hmnet_method_socket_rsvd_8 (__ACTV_HIGHTABLE_START + 51)
#define __hmnet_method_socket_rsvd_7 (__ACTV_HIGHTABLE_START + 52)
#define __hmnet_method_socket_rsvd_6 (__ACTV_HIGHTABLE_START + 53)
#define __hmnet_method_socket_rsvd_5 (__ACTV_HIGHTABLE_START + 54)
#define __hmnet_method_socket_rsvd_4 (__ACTV_HIGHTABLE_START + 55)
#define __hmnet_method_socket_rsvd_3 (__ACTV_HIGHTABLE_START + 56)
#define __hmnet_method_socket_rsvd_2 (__ACTV_HIGHTABLE_START + 57)
#define __hmnet_method_socket_rsvd_1 (__ACTV_HIGHTABLE_START + 58)
#define __hmnet_method_epoll_recheck_revents (__ACTV_HIGHTABLE_START + 59)
#define __hmnet_method_epoll_rsvd_1 (__ACTV_HIGHTABLE_START + 60)
#define __hmnet_method_cfg_loglevel (__ACTV_HIGHTABLE_START + 61)
#define __hmnet_method_cfg_show_stats (__ACTV_HIGHTABLE_START + 62)
#define __hmnet_method_cfg_getifaddrs (__ACTV_HIGHTABLE_START + 63)
#define __hmnet_method_cfg_rsvd_1 (__ACTV_HIGHTABLE_START + 64)
#define __hmnet_method_netdev_rx (__ACTV_HIGHTABLE_START + 65)
#define __hmnet_method_netdev_notify (__ACTV_HIGHTABLE_START + 66)
#define __hmnet_method_netdev_rx_ex_slowdata (__ACTV_HIGHTABLE_START + 67)
#define __hmnet_method_netdev_rsvd_1 (__ACTV_HIGHTABLE_START + 68)
#define __hmnet_method_procfs_read (__ACTV_HIGHTABLE_START + 69)
#define __hmnet_method_procfs_write (__ACTV_HIGHTABLE_START + 70)
#define __hmnet_method_procfs_fds_read (__ACTV_HIGHTABLE_START + 71)
#define __hmnet_method_procfs_get_fd_path (__ACTV_HIGHTABLE_START + 72)
#define __hmnet_method_netlink_register_generic (__ACTV_HIGHTABLE_START + 73)
#define __hmnet_method_netlink_unregister_generic (__ACTV_HIGHTABLE_START + 74)
#define __hmnet_method_netlink_unicast (__ACTV_HIGHTABLE_START + 75)
#define __hmnet_method_netlink_broadcast (__ACTV_HIGHTABLE_START + 76)
#define __hmnet_method_netlink_create_service (__ACTV_HIGHTABLE_START + 77)
#define __hmnet_method_netlink_release_service (__ACTV_HIGHTABLE_START + 78)
#define __hmnet_method_netlink_unicast_nowait (__ACTV_HIGHTABLE_START + 79)
#define __hmnet_method_netlink_rsvd_1 (__ACTV_HIGHTABLE_START + 80)
#define __hmnet_method_vsock_register_vm (__ACTV_HIGHTABLE_START + 81)
#define __hmnet_method_vsock_vm_notify (__ACTV_HIGHTABLE_START + 82)
#define __hmnet_method_vsock_unregister_vm (__ACTV_HIGHTABLE_START + 83)
#define __hmnet_method_vsock_rsvd_1 (__ACTV_HIGHTABLE_START + 84)
#define __hmnet_method_ldk_domain_register (__ACTV_HIGHTABLE_START + 85)
#define __hmnet_method_procfs_create_ldk_entry (__ACTV_HIGHTABLE_START + 86)
#define __hmnet_method_procfs_delete_ldk_entry_specific (__ACTV_HIGHTABLE_START + 87)
#define __hmnet_method_procfs_create_ldk_entries (__ACTV_HIGHTABLE_START + 88)
#define __hmnet_method_ldk_rsvd_1 (__ACTV_HIGHTABLE_START + 89)
#define __hmnet_method_sec_mgmt (__ACTV_HIGHTABLE_START + 90)
#define __hmnet_method_namespace_new_ns (__ACTV_HIGHTABLE_START + 91)
#define __hmnet_method_namespace_destroy_ns (__ACTV_HIGHTABLE_START + 92)
#define __hmnet_method_MAX (__ACTV_HIGHTABLE_START + 93)
raw_static_assert(sizeof(struct __actvret_netcall_socket_revoke) < 512,
            netcall_socket_revoke_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_fork) < 512,
            netcall_socket_fork_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_dev_ioctl) < 512,
            netcall_socket_dev_ioctl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_send_fd) < 512,
            netcall_socket_send_fd_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_put_send_fd) < 512,
            netcall_socket_put_send_fd_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_proc_unauthoried_files) < 512,
            netcall_socket_proc_unauthoried_files_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_query_info) < 512,
            netcall_socket_query_info_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_sys_read) < 512,
            netcall_socket_sys_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_sys_write) < 512,
            netcall_socket_sys_write_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_sys_ioctl) < 512,
            netcall_socket_sys_ioctl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_mmap) < 512,
            netcall_socket_mmap_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_attach) < 512,
            netcall_socket_attach_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_get) < 512,
            netcall_socket_get_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_put) < 512,
            netcall_socket_put_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_socket) < 512,
            netcall_socket_socket_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_socketpair) < 512,
            netcall_socket_socketpair_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_close) < 512,
            netcall_socket_close_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_shutdown) < 512,
            netcall_socket_shutdown_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_bind) < 512,
            netcall_socket_bind_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_listen) < 512,
            netcall_socket_listen_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_accept) < 512,
            netcall_socket_accept_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_accept4) < 512,
            netcall_socket_accept4_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_connect) < 512,
            netcall_socket_connect_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_sendmmsg) < 512,
            netcall_socket_sendmmsg_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_recvmmsg) < 512,
            netcall_socket_recvmmsg_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_read) < 512,
            netcall_socket_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_write) < 512,
            netcall_socket_write_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_sendmsg) < 512,
            netcall_socket_sendmsg_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_recvmsg) < 512,
            netcall_socket_recvmsg_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_sendto) < 512,
            netcall_socket_sendto_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_recvfrom) < 512,
            netcall_socket_recvfrom_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_readv) < 512,
            netcall_socket_readv_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_writev) < 512,
            netcall_socket_writev_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_getsockopt) < 512,
            netcall_socket_getsockopt_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_setsockopt) < 512,
            netcall_socket_setsockopt_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_getsockname) < 512,
            netcall_socket_getsockname_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_getpeername) < 512,
            netcall_socket_getpeername_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_ioctl) < 512,
            netcall_socket_ioctl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_fcntl) < 512,
            netcall_socket_fcntl_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_fchown) < 512,
            netcall_socket_fchown_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_dup) < 512,
            netcall_socket_dup_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_dup3) < 512,
            netcall_socket_dup3_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_fstat) < 512,
            netcall_socket_fstat_too_many_actvret_data);
#ifdef __arm__
raw_static_assert(sizeof(struct __actvret_netcall_socket_send) < 512,
            netcall_socket_send_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_recv) < 512,
            netcall_socket_recv_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_fcntl64) < 512,
            netcall_socket_fcntl64_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_fchown32) < 512,
            netcall_socket_fchown32_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_dup2) < 512,
            netcall_socket_dup2_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_socket_fstat64) < 512,
            netcall_socket_fstat64_too_many_actvret_data);
#endif
raw_static_assert(sizeof(struct __actvret_netcall_epoll_recheck_revents) < 512,
            netcall_epoll_recheck_revents_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_cfg_loglevel) < 512,
            netcall_cfg_loglevel_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_cfg_show_stats) < 512,
            netcall_cfg_show_stats_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_cfg_getifaddrs) < 512,
            netcall_cfg_getifaddrs_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_netdev_rx) < 512,
            netcall_netdev_rx_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_netdev_notify) < 512,
            netcall_netdev_notify_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_netdev_rx_ex_slowdata) < 512,
            netcall_netdev_rx_ex_slowdata_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_procfs_read) < 512,
            netcall_procfs_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_procfs_write) < 512,
            netcall_procfs_write_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_procfs_fds_read) < 512,
            netcall_procfs_fds_read_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_procfs_get_fd_path) < 512,
            netcall_procfs_get_fd_path_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_netlink_register_generic) < 512,
            netcall_netlink_register_generic_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_netlink_unregister_generic) < 512,
            netcall_netlink_unregister_generic_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_netlink_unicast) < 512,
            netcall_netlink_unicast_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_netlink_broadcast) < 512,
            netcall_netlink_broadcast_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_netlink_create_service) < 512,
            netcall_netlink_create_service_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_netlink_release_service) < 512,
            netcall_netlink_release_service_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_netlink_unicast_nowait) < 512,
            netcall_netlink_unicast_nowait_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_vsock_register_vm) < 512,
            netcall_vsock_register_vm_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_vsock_vm_notify) < 512,
            netcall_vsock_vm_notify_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_vsock_unregister_vm) < 512,
            netcall_vsock_unregister_vm_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_ldk_domain_register) < 512,
            netcall_ldk_domain_register_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_procfs_create_ldk_entry) < 512,
            netcall_procfs_create_ldk_entry_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_procfs_delete_ldk_entry_specific) < 512,
            netcall_procfs_delete_ldk_entry_specific_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_procfs_create_ldk_entries) < 512,
            netcall_procfs_create_ldk_entries_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_ldk_bpf) < 512,
            netcall_ldk_bpf_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_sec_mgmt) < 512,
            netcall_sec_mgmt_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_namespace_new_ns) < 512,
            netcall_namespace_new_ns_too_many_actvret_data);
raw_static_assert(sizeof(struct __actvret_netcall_namespace_destroy_ns) < 512,
            netcall_namespace_destroy_ns_too_many_actvret_data);
raw_static_assert(sizeof(union __actvret_netcall) < 512,
        netcall_too_many_actvret_data);
extern int __actvcall_netcall_socket_revoke(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_netcall_socket_revoke(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_netcall_socket_revoke(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_socket_revoke;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_socket_revoke(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_socket_revoke(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_socket_revoke;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_socket_revoke(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_socket_revoke(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_netcall_socket_revoke(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_socket_revoke(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_netcall_socket_revoke(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_netcall_socket_fork(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_netcall_socket_fork(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_netcall_socket_fork(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_socket_fork;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_socket_fork(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_socket_fork(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_socket_fork;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_socket_fork(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_socket_fork(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_netcall_socket_fork(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_socket_fork(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_netcall_socket_fork(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_netcall_socket_dev_ioctl(unsigned long long __rpc_info, void *__buf, int cmd, void *args);


extern int __actvcapcall_netcall_socket_dev_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, int cmd, void *args);


static inline int __attribute__((always_inline)) actvxactcall_netcall_socket_dev_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, int cmd, void *args)
{
	const unsigned long __callno = __hmnet_method_socket_dev_ioctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_socket_dev_ioctl(rpc_info, NULL, cmd, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_socket_dev_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int cmd, void *args)
{
	const unsigned long __callno = __hmnet_method_socket_dev_ioctl;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_socket_dev_ioctl(__rref, rpc_info, NULL, cmd, args);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_socket_dev_ioctl(int cmd, void *args)
{
	return actvxactcall_netcall_socket_dev_ioctl(false, false, cmd, args);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_socket_dev_ioctl(rref_t __rref, int cmd, void *args)
{
	return actvxactcapcall_netcall_socket_dev_ioctl(false, false, __rref, cmd, args);
}

extern int __actvcall_netcall_socket_send_fd(unsigned long long __rpc_info, void *__buf, unsigned long argu_info);


extern int __actvcapcall_netcall_socket_send_fd(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long argu_info);


static inline int __attribute__((always_inline)) actvxactcall_netcall_socket_send_fd(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long argu_info, struct __actvret_netcall_socket_send_fd *__ret)
{
	const unsigned long __callno = __hmnet_method_socket_send_fd;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_socket_send_fd(rpc_info, __ret, argu_info);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_socket_send_fd(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long argu_info, struct __actvret_netcall_socket_send_fd *__ret)
{
	const unsigned long __callno = __hmnet_method_socket_send_fd;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_socket_send_fd(__rref, rpc_info, __ret, argu_info);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_socket_send_fd(unsigned long argu_info, struct __actvret_netcall_socket_send_fd *__ret)
{
	return actvxactcall_netcall_socket_send_fd(false, false, argu_info, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_socket_send_fd(rref_t __rref, unsigned long argu_info, struct __actvret_netcall_socket_send_fd *__ret)
{
	return actvxactcapcall_netcall_socket_send_fd(false, false, __rref, argu_info, __ret);
}

extern int __actvcall_netcall_socket_put_send_fd(unsigned long long __rpc_info, void *__buf, uintptr_t argu_info);


extern int __actvcapcall_netcall_socket_put_send_fd(rref_t __rref, unsigned long long __rpc_info, void *__buf, uintptr_t argu_info);


static inline int __attribute__((always_inline)) actvxactcall_netcall_socket_put_send_fd(bool is_xact_fwd, bool is_xact_allow_refwd, uintptr_t argu_info)
{
	const unsigned long __callno = __hmnet_method_socket_put_send_fd;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_socket_put_send_fd(rpc_info, NULL, argu_info);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_socket_put_send_fd(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, uintptr_t argu_info)
{
	const unsigned long __callno = __hmnet_method_socket_put_send_fd;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_socket_put_send_fd(__rref, rpc_info, NULL, argu_info);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_socket_put_send_fd(uintptr_t argu_info)
{
	return actvxactcall_netcall_socket_put_send_fd(false, false, argu_info);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_socket_put_send_fd(rref_t __rref, uintptr_t argu_info)
{
	return actvxactcapcall_netcall_socket_put_send_fd(false, false, __rref, argu_info);
}

extern int __actvcall_netcall_socket_proc_unauthoried_files(unsigned long long __rpc_info, void *__buf, int sel_null_fd);


extern int __actvcapcall_netcall_socket_proc_unauthoried_files(rref_t __rref, unsigned long long __rpc_info, void *__buf, int sel_null_fd);


static inline int __attribute__((always_inline)) actvxactcall_netcall_socket_proc_unauthoried_files(bool is_xact_fwd, bool is_xact_allow_refwd, int sel_null_fd)
{
	const unsigned long __callno = __hmnet_method_socket_proc_unauthoried_files;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_socket_proc_unauthoried_files(rpc_info, NULL, sel_null_fd);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_socket_proc_unauthoried_files(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int sel_null_fd)
{
	const unsigned long __callno = __hmnet_method_socket_proc_unauthoried_files;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_socket_proc_unauthoried_files(__rref, rpc_info, NULL, sel_null_fd);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_socket_proc_unauthoried_files(int sel_null_fd)
{
	return actvxactcall_netcall_socket_proc_unauthoried_files(false, false, sel_null_fd);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_socket_proc_unauthoried_files(rref_t __rref, int sel_null_fd)
{
	return actvxactcapcall_netcall_socket_proc_unauthoried_files(false, false, __rref, sel_null_fd);
}

extern int __actvcall_netcall_socket_query_info(unsigned long long __rpc_info, void *__buf, int fd, uint32_t cnode, void *info);


extern int __actvcapcall_netcall_socket_query_info(rref_t __rref, unsigned long long __rpc_info, void *__buf, int fd, uint32_t cnode, void *info);


static inline int __attribute__((always_inline)) actvxactcall_netcall_socket_query_info(bool is_xact_fwd, bool is_xact_allow_refwd, int fd, uint32_t cnode, void *info)
{
	const unsigned long __callno = __hmnet_method_socket_query_info;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_socket_query_info(rpc_info, NULL, fd, cnode, info);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_socket_query_info(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int fd, uint32_t cnode, void *info)
{
	const unsigned long __callno = __hmnet_method_socket_query_info;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_socket_query_info(__rref, rpc_info, NULL, fd, cnode, info);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_socket_query_info(int fd, uint32_t cnode, void *info)
{
	return actvxactcall_netcall_socket_query_info(false, false, fd, cnode, info);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_socket_query_info(rref_t __rref, int fd, uint32_t cnode, void *info)
{
	return actvxactcapcall_netcall_socket_query_info(false, false, __rref, fd, cnode, info);
}

extern long __actvcall_netcall_socket_sys_read(unsigned long long __rpc_info, void *__buf, uint32_t cnode, int fd, void *buf, size_t len, int flags);


extern long __actvcapcall_netcall_socket_sys_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, uint32_t cnode, int fd, void *buf, size_t len, int flags);


static inline long __attribute__((always_inline)) actvxactcall_netcall_socket_sys_read(bool is_xact_fwd, bool is_xact_allow_refwd, uint32_t cnode, int fd, void *buf, size_t len, int flags)
{
	const unsigned long __callno = __hmnet_method_socket_sys_read;
	long __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_socket_sys_read(rpc_info, NULL, cnode, fd, buf, len, flags);
	} while (__res == (long)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline long __attribute__((always_inline)) actvxactcapcall_netcall_socket_sys_read(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, uint32_t cnode, int fd, void *buf, size_t len, int flags)
{
	const unsigned long __callno = __hmnet_method_socket_sys_read;
	long __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_socket_sys_read(__rref, rpc_info, NULL, cnode, fd, buf, len, flags);
	} while (__res == (long)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline long __attribute__((always_inline)) actvcall_netcall_socket_sys_read(uint32_t cnode, int fd, void *buf, size_t len, int flags)
{
	return actvxactcall_netcall_socket_sys_read(false, false, cnode, fd, buf, len, flags);
}

static inline long __attribute__((always_inline)) actvcapcall_netcall_socket_sys_read(rref_t __rref, uint32_t cnode, int fd, void *buf, size_t len, int flags)
{
	return actvxactcapcall_netcall_socket_sys_read(false, false, __rref, cnode, fd, buf, len, flags);
}

extern long __actvcall_netcall_socket_sys_write(unsigned long long __rpc_info, void *__buf, uint32_t cnode, int fd, const void *buf, size_t len, int flags);


extern long __actvcapcall_netcall_socket_sys_write(rref_t __rref, unsigned long long __rpc_info, void *__buf, uint32_t cnode, int fd, const void *buf, size_t len, int flags);


static inline long __attribute__((always_inline)) actvxactcall_netcall_socket_sys_write(bool is_xact_fwd, bool is_xact_allow_refwd, uint32_t cnode, int fd, const void *buf, size_t len, int flags)
{
	const unsigned long __callno = __hmnet_method_socket_sys_write;
	long __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_socket_sys_write(rpc_info, NULL, cnode, fd, buf, len, flags);
	} while (__res == (long)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline long __attribute__((always_inline)) actvxactcapcall_netcall_socket_sys_write(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, uint32_t cnode, int fd, const void *buf, size_t len, int flags)
{
	const unsigned long __callno = __hmnet_method_socket_sys_write;
	long __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const void*), __arch_actv_rpc_stksz_alignment(const void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_socket_sys_write(__rref, rpc_info, NULL, cnode, fd, buf, len, flags);
	} while (__res == (long)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline long __attribute__((always_inline)) actvcall_netcall_socket_sys_write(uint32_t cnode, int fd, const void *buf, size_t len, int flags)
{
	return actvxactcall_netcall_socket_sys_write(false, false, cnode, fd, buf, len, flags);
}

static inline long __attribute__((always_inline)) actvcapcall_netcall_socket_sys_write(rref_t __rref, uint32_t cnode, int fd, const void *buf, size_t len, int flags)
{
	return actvxactcapcall_netcall_socket_sys_write(false, false, __rref, cnode, fd, buf, len, flags);
}

extern long __actvcall_netcall_socket_sys_ioctl(unsigned long long __rpc_info, void *__buf, uint32_t cnode, int fd, int request, unsigned long arg);


extern long __actvcapcall_netcall_socket_sys_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, uint32_t cnode, int fd, int request, unsigned long arg);


static inline long __attribute__((always_inline)) actvxactcall_netcall_socket_sys_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, uint32_t cnode, int fd, int request, unsigned long arg)
{
	const unsigned long __callno = __hmnet_method_socket_sys_ioctl;
	long __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_socket_sys_ioctl(rpc_info, NULL, cnode, fd, request, arg);
	} while (__res == (long)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline long __attribute__((always_inline)) actvxactcapcall_netcall_socket_sys_ioctl(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, uint32_t cnode, int fd, int request, unsigned long arg)
{
	const unsigned long __callno = __hmnet_method_socket_sys_ioctl;
	long __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uint32_t), __arch_actv_rpc_stksz_alignment(uint32_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long), __arch_actv_rpc_stksz_alignment(unsigned long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_socket_sys_ioctl(__rref, rpc_info, NULL, cnode, fd, request, arg);
	} while (__res == (long)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline long __attribute__((always_inline)) actvcall_netcall_socket_sys_ioctl(uint32_t cnode, int fd, int request, unsigned long arg)
{
	return actvxactcall_netcall_socket_sys_ioctl(false, false, cnode, fd, request, arg);
}

static inline long __attribute__((always_inline)) actvcapcall_netcall_socket_sys_ioctl(rref_t __rref, uint32_t cnode, int fd, int request, unsigned long arg)
{
	return actvxactcapcall_netcall_socket_sys_ioctl(false, false, __rref, cnode, fd, request, arg);
}

extern int __actvcall_netcall_socket_mmap(unsigned long long __rpc_info, void *__buf, int fd, unsigned int vs_cnode_idx, uintptr_t vaddr, size_t aligned_len, unsigned int prot, unsigned int flags, long long offset);


extern int __actvcapcall_netcall_socket_mmap(rref_t __rref, unsigned long long __rpc_info, void *__buf, int fd, unsigned int vs_cnode_idx, uintptr_t vaddr, size_t aligned_len, unsigned int prot, unsigned int flags, long long offset);


static inline int __attribute__((always_inline)) actvxactcall_netcall_socket_mmap(bool is_xact_fwd, bool is_xact_allow_refwd, int fd, unsigned int vs_cnode_idx, uintptr_t vaddr, size_t aligned_len, unsigned int prot, unsigned int flags, long long offset, struct __actvret_netcall_socket_mmap *__ret)
{
	const unsigned long __callno = __hmnet_method_socket_mmap;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_socket_mmap(rpc_info, __ret, fd, vs_cnode_idx, vaddr, aligned_len, prot, flags, offset);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_socket_mmap(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int fd, unsigned int vs_cnode_idx, uintptr_t vaddr, size_t aligned_len, unsigned int prot, unsigned int flags, long long offset, struct __actvret_netcall_socket_mmap *__ret)
{
	const unsigned long __callno = __hmnet_method_socket_mmap;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(uintptr_t), __arch_actv_rpc_stksz_alignment(uintptr_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(long long), __arch_actv_rpc_stksz_alignment(long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_socket_mmap(__rref, rpc_info, __ret, fd, vs_cnode_idx, vaddr, aligned_len, prot, flags, offset);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_socket_mmap(int fd, unsigned int vs_cnode_idx, uintptr_t vaddr, size_t aligned_len, unsigned int prot, unsigned int flags, long long offset, struct __actvret_netcall_socket_mmap *__ret)
{
	return actvxactcall_netcall_socket_mmap(false, false, fd, vs_cnode_idx, vaddr, aligned_len, prot, flags, offset, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_socket_mmap(rref_t __rref, int fd, unsigned int vs_cnode_idx, uintptr_t vaddr, size_t aligned_len, unsigned int prot, unsigned int flags, long long offset, struct __actvret_netcall_socket_mmap *__ret)
{
	return actvxactcapcall_netcall_socket_mmap(false, false, __rref, fd, vs_cnode_idx, vaddr, aligned_len, prot, flags, offset, __ret);
}

extern int __actvcall_netcall_socket_attach(unsigned long long __rpc_info, void *__buf, struct ux_socket *sock, unsigned int cnode, int fd);


extern int __actvcapcall_netcall_socket_attach(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct ux_socket *sock, unsigned int cnode, int fd);


static inline int __attribute__((always_inline)) actvxactcall_netcall_socket_attach(bool is_xact_fwd, bool is_xact_allow_refwd, struct ux_socket *sock, unsigned int cnode, int fd)
{
	const unsigned long __callno = __hmnet_method_socket_attach;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct ux_socket*), __arch_actv_rpc_stksz_alignment(struct ux_socket*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_socket_attach(rpc_info, NULL, sock, cnode, fd);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_socket_attach(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct ux_socket *sock, unsigned int cnode, int fd)
{
	const unsigned long __callno = __hmnet_method_socket_attach;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct ux_socket*), __arch_actv_rpc_stksz_alignment(struct ux_socket*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_socket_attach(__rref, rpc_info, NULL, sock, cnode, fd);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_socket_attach(struct ux_socket *sock, unsigned int cnode, int fd)
{
	return actvxactcall_netcall_socket_attach(false, false, sock, cnode, fd);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_socket_attach(rref_t __rref, struct ux_socket *sock, unsigned int cnode, int fd)
{
	return actvxactcapcall_netcall_socket_attach(false, false, __rref, sock, cnode, fd);
}

extern int __actvcall_netcall_socket_get(unsigned long long __rpc_info, void *__buf, unsigned int cnode, int fd);


extern int __actvcapcall_netcall_socket_get(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode, int fd);


static inline int __attribute__((always_inline)) actvxactcall_netcall_socket_get(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cnode, int fd, struct __actvret_netcall_socket_get *__ret)
{
	const unsigned long __callno = __hmnet_method_socket_get;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_socket_get(rpc_info, __ret, cnode, fd);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_socket_get(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cnode, int fd, struct __actvret_netcall_socket_get *__ret)
{
	const unsigned long __callno = __hmnet_method_socket_get;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, sizeof(*__ret), __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_socket_get(__rref, rpc_info, __ret, cnode, fd);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_socket_get(unsigned int cnode, int fd, struct __actvret_netcall_socket_get *__ret)
{
	return actvxactcall_netcall_socket_get(false, false, cnode, fd, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_socket_get(rref_t __rref, unsigned int cnode, int fd, struct __actvret_netcall_socket_get *__ret)
{
	return actvxactcapcall_netcall_socket_get(false, false, __rref, cnode, fd, __ret);
}

extern int __actvcall_netcall_socket_put(unsigned long long __rpc_info, void *__buf, struct ux_socket *sock);


extern int __actvcapcall_netcall_socket_put(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct ux_socket *sock);


static inline int __attribute__((always_inline)) actvxactcall_netcall_socket_put(bool is_xact_fwd, bool is_xact_allow_refwd, struct ux_socket *sock)
{
	const unsigned long __callno = __hmnet_method_socket_put;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct ux_socket*), __arch_actv_rpc_stksz_alignment(struct ux_socket*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_socket_put(rpc_info, NULL, sock);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_socket_put(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct ux_socket *sock)
{
	const unsigned long __callno = __hmnet_method_socket_put;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct ux_socket*), __arch_actv_rpc_stksz_alignment(struct ux_socket*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_socket_put(__rref, rpc_info, NULL, sock);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_socket_put(struct ux_socket *sock)
{
	return actvxactcall_netcall_socket_put(false, false, sock);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_socket_put(rref_t __rref, struct ux_socket *sock)
{
	return actvxactcapcall_netcall_socket_put(false, false, __rref, sock);
}

extern int __lsyscall_socket_close(long syscall_num, long long sock, int fd);


static inline int __attribute__((always_inline)) lsyscall_socket_close(long long sock, int fd)
{
	return __lsyscall_socket_close(SYS_close, sock, fd);
}

extern int __lsyscall_socket_bind(long syscall_num, long long socket, const void *addr, socklen_t len);


static inline int __attribute__((always_inline)) lsyscall_socket_bind(long long socket, const void *addr, socklen_t len)
{
	return __lsyscall_socket_bind(SYS_bind, socket, addr, len);
}

extern int __lsyscall_socket_listen(long syscall_num, long long socket, int backlog);


static inline int __attribute__((always_inline)) lsyscall_socket_listen(long long socket, int backlog)
{
	return __lsyscall_socket_listen(SYS_listen, socket, backlog);
}

extern int __lsyscall_socket_fcntl(long syscall_num, long long socket, int cmd, unsigned long val, int fd);


static inline int __attribute__((always_inline)) lsyscall_socket_fcntl(long long socket, int cmd, unsigned long val, int fd)
{
	return __lsyscall_socket_fcntl(SYS_fcntl, socket, cmd, val, fd);
}

#ifdef __arm__
extern int __lsyscall_socket_fcntl64(long syscall_num, long long socket, int cmd, unsigned long val, int fd);


static inline int __attribute__((always_inline)) lsyscall_socket_fcntl64(long long socket, int cmd, unsigned long val, int fd)
{
	return __lsyscall_socket_fcntl64(SYS_fcntl64, socket, cmd, val, fd);
}

#endif
extern int __actvcall_netcall_epoll_recheck_revents(unsigned long long __rpc_info, void *__buf, void *data);


extern int __actvcapcall_netcall_epoll_recheck_revents(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *data);


static inline int __attribute__((always_inline)) actvxactcall_netcall_epoll_recheck_revents(bool is_xact_fwd, bool is_xact_allow_refwd, void *data)
{
	const unsigned long __callno = __hmnet_method_epoll_recheck_revents;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_epoll_recheck_revents(rpc_info, NULL, data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_epoll_recheck_revents(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *data)
{
	const unsigned long __callno = __hmnet_method_epoll_recheck_revents;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_epoll_recheck_revents(__rref, rpc_info, NULL, data);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_epoll_recheck_revents(void *data)
{
	return actvxactcall_netcall_epoll_recheck_revents(false, false, data);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_epoll_recheck_revents(rref_t __rref, void *data)
{
	return actvxactcapcall_netcall_epoll_recheck_revents(false, false, __rref, data);
}

extern int __actvcall_netcall_cfg_loglevel(unsigned long long __rpc_info, void *__buf, unsigned int loglevel);


extern int __actvcapcall_netcall_cfg_loglevel(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int loglevel);


static inline int __attribute__((always_inline)) actvxactcall_netcall_cfg_loglevel(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int loglevel)
{
	const unsigned long __callno = __hmnet_method_cfg_loglevel;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_cfg_loglevel(rpc_info, NULL, loglevel);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_cfg_loglevel(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int loglevel)
{
	const unsigned long __callno = __hmnet_method_cfg_loglevel;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_cfg_loglevel(__rref, rpc_info, NULL, loglevel);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_cfg_loglevel(unsigned int loglevel)
{
	return actvxactcall_netcall_cfg_loglevel(false, false, loglevel);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_cfg_loglevel(rref_t __rref, unsigned int loglevel)
{
	return actvxactcapcall_netcall_cfg_loglevel(false, false, __rref, loglevel);
}

extern int __actvcall_netcall_cfg_show_stats(unsigned long long __rpc_info, void *__buf, struct sysif_protocol ss_protocol, unsigned long long shm_key);


extern int __actvcapcall_netcall_cfg_show_stats(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct sysif_protocol ss_protocol, unsigned long long shm_key);


static inline int __attribute__((always_inline)) actvxactcall_netcall_cfg_show_stats(bool is_xact_fwd, bool is_xact_allow_refwd, struct sysif_protocol ss_protocol, unsigned long long shm_key)
{
	const unsigned long __callno = __hmnet_method_cfg_show_stats;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct sysif_protocol), __arch_actv_rpc_stksz_alignment(struct sysif_protocol));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_cfg_show_stats(rpc_info, NULL, ss_protocol, shm_key);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_cfg_show_stats(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, struct sysif_protocol ss_protocol, unsigned long long shm_key)
{
	const unsigned long __callno = __hmnet_method_cfg_show_stats;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct sysif_protocol), __arch_actv_rpc_stksz_alignment(struct sysif_protocol));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_cfg_show_stats(__rref, rpc_info, NULL, ss_protocol, shm_key);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_cfg_show_stats(struct sysif_protocol ss_protocol, unsigned long long shm_key)
{
	return actvxactcall_netcall_cfg_show_stats(false, false, ss_protocol, shm_key);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_cfg_show_stats(rref_t __rref, struct sysif_protocol ss_protocol, unsigned long long shm_key)
{
	return actvxactcapcall_netcall_cfg_show_stats(false, false, __rref, ss_protocol, shm_key);
}

extern int __actvcall_netcall_cfg_getifaddrs(unsigned long long __rpc_info, void *__buf, unsigned long long shm_key);


extern int __actvcapcall_netcall_cfg_getifaddrs(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long shm_key);


static inline int __attribute__((always_inline)) actvxactcall_netcall_cfg_getifaddrs(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long shm_key)
{
	const unsigned long __callno = __hmnet_method_cfg_getifaddrs;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_cfg_getifaddrs(rpc_info, NULL, shm_key);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_cfg_getifaddrs(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long shm_key)
{
	const unsigned long __callno = __hmnet_method_cfg_getifaddrs;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_cfg_getifaddrs(__rref, rpc_info, NULL, shm_key);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_cfg_getifaddrs(unsigned long long shm_key)
{
	return actvxactcall_netcall_cfg_getifaddrs(false, false, shm_key);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_cfg_getifaddrs(rref_t __rref, unsigned long long shm_key)
{
	return actvxactcapcall_netcall_cfg_getifaddrs(false, false, __rref, shm_key);
}

extern int __actvcall_netcall_netdev_rx(unsigned long long __rpc_info, void *__buf, unsigned int cell_pool_offset, int shm_len, int rx_len, int offset, int devno);


extern int __actvcapcall_netcall_netdev_rx(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cell_pool_offset, int shm_len, int rx_len, int offset, int devno);


static inline int __attribute__((always_inline)) actvxactcall_netcall_netdev_rx(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cell_pool_offset, int shm_len, int rx_len, int offset, int devno)
{
	const unsigned long __callno = __hmnet_method_netdev_rx;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_netdev_rx(rpc_info, NULL, cell_pool_offset, shm_len, rx_len, offset, devno);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_netdev_rx(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cell_pool_offset, int shm_len, int rx_len, int offset, int devno)
{
	const unsigned long __callno = __hmnet_method_netdev_rx;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_netdev_rx(__rref, rpc_info, NULL, cell_pool_offset, shm_len, rx_len, offset, devno);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_netdev_rx(unsigned int cell_pool_offset, int shm_len, int rx_len, int offset, int devno)
{
	return actvxactcall_netcall_netdev_rx(false, false, cell_pool_offset, shm_len, rx_len, offset, devno);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_netdev_rx(rref_t __rref, unsigned int cell_pool_offset, int shm_len, int rx_len, int offset, int devno)
{
	return actvxactcapcall_netcall_netdev_rx(false, false, __rref, cell_pool_offset, shm_len, rx_len, offset, devno);
}

extern int __actvcall_netcall_netdev_notify(unsigned long long __rpc_info, void *__buf, enum hm_netdev_event event, int devno, int option, struct netdev_ifname ifname, unsigned long long features);


extern int __actvcapcall_netcall_netdev_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, enum hm_netdev_event event, int devno, int option, struct netdev_ifname ifname, unsigned long long features);


static inline int __attribute__((always_inline)) actvxactcall_netcall_netdev_notify(bool is_xact_fwd, bool is_xact_allow_refwd, enum hm_netdev_event event, int devno, int option, struct netdev_ifname ifname, unsigned long long features)
{
	const unsigned long __callno = __hmnet_method_netdev_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum hm_netdev_event), __arch_actv_rpc_stksz_alignment(enum hm_netdev_event));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct netdev_ifname), __arch_actv_rpc_stksz_alignment(struct netdev_ifname));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_netdev_notify(rpc_info, NULL, event, devno, option, ifname, features);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_netdev_notify(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, enum hm_netdev_event event, int devno, int option, struct netdev_ifname ifname, unsigned long long features)
{
	const unsigned long __callno = __hmnet_method_netdev_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(enum hm_netdev_event), __arch_actv_rpc_stksz_alignment(enum hm_netdev_event));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(struct netdev_ifname), __arch_actv_rpc_stksz_alignment(struct netdev_ifname));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_netdev_notify(__rref, rpc_info, NULL, event, devno, option, ifname, features);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_netdev_notify(enum hm_netdev_event event, int devno, int option, struct netdev_ifname ifname, unsigned long long features)
{
	return actvxactcall_netcall_netdev_notify(false, false, event, devno, option, ifname, features);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_netdev_notify(rref_t __rref, enum hm_netdev_event event, int devno, int option, struct netdev_ifname ifname, unsigned long long features)
{
	return actvxactcapcall_netcall_netdev_notify(false, false, __rref, event, devno, option, ifname, features);
}

extern int __actvcall_netcall_netdev_rx_ex_slowdata(unsigned long long __rpc_info, void *__buf, int num);


extern int __actvcapcall_netcall_netdev_rx_ex_slowdata(rref_t __rref, unsigned long long __rpc_info, void *__buf, int num);


static inline int __attribute__((always_inline)) actvxactcall_netcall_netdev_rx_ex_slowdata(bool is_xact_fwd, bool is_xact_allow_refwd, int num, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_netdev_rx_ex_slowdata;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_netdev_rx_ex_slowdata(rpc_info, ___void_buf, num);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_netdev_rx_ex_slowdata(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int num, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_netdev_rx_ex_slowdata;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_netdev_rx_ex_slowdata(__rref, rpc_info, ___void_buf, num);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_netdev_rx_ex_slowdata(int num, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_netcall_netdev_rx_ex_slowdata(false, false, num, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_netdev_rx_ex_slowdata(rref_t __rref, int num, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_netcall_netdev_rx_ex_slowdata(false, false, __rref, num, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_netcall_procfs_read(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_netcall_procfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_netcall_procfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_procfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_procfs_read(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_procfs_read(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_procfs_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_procfs_read(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_procfs_read(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_netcall_procfs_read(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_procfs_read(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_netcall_procfs_read(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_netcall_procfs_write(unsigned long long __rpc_info, void *__buf);


extern int __actvcapcall_netcall_procfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf);


static inline int __attribute__((always_inline)) actvxactcall_netcall_procfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_procfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_procfs_write(rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_procfs_write(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_procfs_write;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_procfs_write(__rref, rpc_info, ___void_buf);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_procfs_write(void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_netcall_procfs_write(false, false, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_procfs_write(rref_t __rref, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_netcall_procfs_write(false, false, __rref, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_netcall_procfs_fds_read(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, unsigned long long fd_map, unsigned int max_files);


extern int __actvcapcall_netcall_procfs_fds_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, unsigned long long fd_map, unsigned int max_files);


static inline int __attribute__((always_inline)) actvxactcall_netcall_procfs_fds_read(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cnode_idx, unsigned long long fd_map, unsigned int max_files)
{
	const unsigned long __callno = __hmnet_method_procfs_fds_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_procfs_fds_read(rpc_info, NULL, cnode_idx, fd_map, max_files);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_procfs_fds_read(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cnode_idx, unsigned long long fd_map, unsigned int max_files)
{
	const unsigned long __callno = __hmnet_method_procfs_fds_read;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_procfs_fds_read(__rref, rpc_info, NULL, cnode_idx, fd_map, max_files);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_procfs_fds_read(unsigned int cnode_idx, unsigned long long fd_map, unsigned int max_files)
{
	return actvxactcall_netcall_procfs_fds_read(false, false, cnode_idx, fd_map, max_files);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_procfs_fds_read(rref_t __rref, unsigned int cnode_idx, unsigned long long fd_map, unsigned int max_files)
{
	return actvxactcapcall_netcall_procfs_fds_read(false, false, __rref, cnode_idx, fd_map, max_files);
}

extern int __actvcall_netcall_procfs_get_fd_path(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, int fd, unsigned long long path_buf, unsigned int path_buf_len);


extern int __actvcapcall_netcall_procfs_get_fd_path(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, int fd, unsigned long long path_buf, unsigned int path_buf_len);


static inline int __attribute__((always_inline)) actvxactcall_netcall_procfs_get_fd_path(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cnode_idx, int fd, unsigned long long path_buf, unsigned int path_buf_len)
{
	const unsigned long __callno = __hmnet_method_procfs_get_fd_path;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_procfs_get_fd_path(rpc_info, NULL, cnode_idx, fd, path_buf, path_buf_len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_procfs_get_fd_path(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cnode_idx, int fd, unsigned long long path_buf, unsigned int path_buf_len)
{
	const unsigned long __callno = __hmnet_method_procfs_get_fd_path;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_procfs_get_fd_path(__rref, rpc_info, NULL, cnode_idx, fd, path_buf, path_buf_len);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_procfs_get_fd_path(unsigned int cnode_idx, int fd, unsigned long long path_buf, unsigned int path_buf_len)
{
	return actvxactcall_netcall_procfs_get_fd_path(false, false, cnode_idx, fd, path_buf, path_buf_len);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_procfs_get_fd_path(rref_t __rref, unsigned int cnode_idx, int fd, unsigned long long path_buf, unsigned int path_buf_len)
{
	return actvxactcapcall_netcall_procfs_get_fd_path(false, false, __rref, cnode_idx, fd, path_buf, path_buf_len);
}

extern int __actvcall_netcall_netlink_register_generic(unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, unsigned long long dh_ap_uref);


extern int __actvcapcall_netcall_netlink_register_generic(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, unsigned long long dh_ap_uref);


static inline int __attribute__((always_inline)) actvxactcall_netcall_netlink_register_generic(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned long long dh_ap_uref, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_netcall_netlink_register_generic *__ret)
{
	const unsigned long __callno = __hmnet_method_netlink_register_generic;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 1);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_netlink_register_generic(rpc_info, ___void_buf, ptr_to_void(__ret), sizeof(*__ret), dh_ap_uref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_netlink_register_generic(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned long long dh_ap_uref, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_netcall_netlink_register_generic *__ret)
{
	const unsigned long __callno = __hmnet_method_netlink_register_generic;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 1);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_netlink_register_generic(__rref, rpc_info, ___void_buf, ptr_to_void(__ret), sizeof(*__ret), dh_ap_uref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_netlink_register_generic(unsigned long long dh_ap_uref, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_netcall_netlink_register_generic *__ret)
{
	return actvxactcall_netcall_netlink_register_generic(false, false, dh_ap_uref, ___void_buf, ___void_buf_sz, __ret);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_netlink_register_generic(rref_t __rref, unsigned long long dh_ap_uref, void *___void_buf, unsigned long ___void_buf_sz, struct __actvret_netcall_netlink_register_generic *__ret)
{
	return actvxactcapcall_netcall_netlink_register_generic(false, false, __rref, dh_ap_uref, ___void_buf, ___void_buf_sz, __ret);
}

extern int __actvcall_netcall_netlink_unregister_generic(unsigned long long __rpc_info, void *__buf, unsigned short familyid);


extern int __actvcapcall_netcall_netlink_unregister_generic(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned short familyid);


static inline int __attribute__((always_inline)) actvxactcall_netcall_netlink_unregister_generic(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned short familyid)
{
	const unsigned long __callno = __hmnet_method_netlink_unregister_generic;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned short), __arch_actv_rpc_stksz_alignment(unsigned short));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_netlink_unregister_generic(rpc_info, NULL, familyid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_netlink_unregister_generic(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned short familyid)
{
	const unsigned long __callno = __hmnet_method_netlink_unregister_generic;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned short), __arch_actv_rpc_stksz_alignment(unsigned short));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_netlink_unregister_generic(__rref, rpc_info, NULL, familyid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_netlink_unregister_generic(unsigned short familyid)
{
	return actvxactcall_netcall_netlink_unregister_generic(false, false, familyid);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_netlink_unregister_generic(rref_t __rref, unsigned short familyid)
{
	return actvxactcapcall_netcall_netlink_unregister_generic(false, false, __rref, familyid);
}

extern int __actvcall_netcall_netlink_unicast(unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned int dst_port);


extern int __actvcapcall_netcall_netlink_unicast(rref_t __rref, unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned int dst_port);


static inline int __attribute__((always_inline)) actvxactcall_netcall_netlink_unicast(bool is_xact_fwd, bool is_xact_allow_refwd, int protocol, unsigned int src_portid, unsigned int dst_port, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_netlink_unicast;
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
		__res = __actvcall_netcall_netlink_unicast(rpc_info, ___void_buf, protocol, src_portid, dst_port);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_netlink_unicast(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int protocol, unsigned int src_portid, unsigned int dst_port, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_netlink_unicast;
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
		__res = __actvcapcall_netcall_netlink_unicast(__rref, rpc_info, ___void_buf, protocol, src_portid, dst_port);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_netlink_unicast(int protocol, unsigned int src_portid, unsigned int dst_port, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_netcall_netlink_unicast(false, false, protocol, src_portid, dst_port, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_netlink_unicast(rref_t __rref, int protocol, unsigned int src_portid, unsigned int dst_port, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_netcall_netlink_unicast(false, false, __rref, protocol, src_portid, dst_port, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_netcall_netlink_broadcast(unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned int dst_group);


extern int __actvcapcall_netcall_netlink_broadcast(rref_t __rref, unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned int dst_group);


static inline int __attribute__((always_inline)) actvxactcall_netcall_netlink_broadcast(bool is_xact_fwd, bool is_xact_allow_refwd, int protocol, unsigned int src_portid, unsigned int dst_group, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_netlink_broadcast;
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
		__res = __actvcall_netcall_netlink_broadcast(rpc_info, ___void_buf, protocol, src_portid, dst_group);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_netlink_broadcast(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int protocol, unsigned int src_portid, unsigned int dst_group, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_netlink_broadcast;
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
		__res = __actvcapcall_netcall_netlink_broadcast(__rref, rpc_info, ___void_buf, protocol, src_portid, dst_group);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_netlink_broadcast(int protocol, unsigned int src_portid, unsigned int dst_group, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_netcall_netlink_broadcast(false, false, protocol, src_portid, dst_group, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_netlink_broadcast(rref_t __rref, int protocol, unsigned int src_portid, unsigned int dst_group, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_netcall_netlink_broadcast(false, false, __rref, protocol, src_portid, dst_group, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_netcall_netlink_create_service(unsigned long long __rpc_info, void *__buf, int protocol);


extern int __actvcapcall_netcall_netlink_create_service(rref_t __rref, unsigned long long __rpc_info, void *__buf, int protocol);


static inline int __attribute__((always_inline)) actvxactcall_netcall_netlink_create_service(bool is_xact_fwd, bool is_xact_allow_refwd, int protocol)
{
	const unsigned long __callno = __hmnet_method_netlink_create_service;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_netlink_create_service(rpc_info, NULL, protocol);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_netlink_create_service(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int protocol)
{
	const unsigned long __callno = __hmnet_method_netlink_create_service;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_netlink_create_service(__rref, rpc_info, NULL, protocol);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_netlink_create_service(int protocol)
{
	return actvxactcall_netcall_netlink_create_service(false, false, protocol);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_netlink_create_service(rref_t __rref, int protocol)
{
	return actvxactcapcall_netcall_netlink_create_service(false, false, __rref, protocol);
}

extern int __actvcall_netcall_netlink_release_service(unsigned long long __rpc_info, void *__buf, int protocol);


extern int __actvcapcall_netcall_netlink_release_service(rref_t __rref, unsigned long long __rpc_info, void *__buf, int protocol);


static inline int __attribute__((always_inline)) actvxactcall_netcall_netlink_release_service(bool is_xact_fwd, bool is_xact_allow_refwd, int protocol)
{
	const unsigned long __callno = __hmnet_method_netlink_release_service;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_netlink_release_service(rpc_info, NULL, protocol);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_netlink_release_service(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int protocol)
{
	const unsigned long __callno = __hmnet_method_netlink_release_service;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_netlink_release_service(__rref, rpc_info, NULL, protocol);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_netlink_release_service(int protocol)
{
	return actvxactcall_netcall_netlink_release_service(false, false, protocol);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_netlink_release_service(rref_t __rref, int protocol)
{
	return actvxactcapcall_netcall_netlink_release_service(false, false, __rref, protocol);
}

extern int __actvcall_netcall_netlink_unicast_nowait(unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned int dst_port);


extern int __actvcapcall_netcall_netlink_unicast_nowait(rref_t __rref, unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned int dst_port);


static inline int __attribute__((always_inline)) actvxactcall_netcall_netlink_unicast_nowait(bool is_xact_fwd, bool is_xact_allow_refwd, int protocol, unsigned int src_portid, unsigned int dst_port, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_netlink_unicast_nowait;
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
		__res = __actvcall_netcall_netlink_unicast_nowait(rpc_info, ___void_buf, protocol, src_portid, dst_port);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_netlink_unicast_nowait(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int protocol, unsigned int src_portid, unsigned int dst_port, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_netlink_unicast_nowait;
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
		__res = __actvcapcall_netcall_netlink_unicast_nowait(__rref, rpc_info, ___void_buf, protocol, src_portid, dst_port);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_netlink_unicast_nowait(int protocol, unsigned int src_portid, unsigned int dst_port, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_netcall_netlink_unicast_nowait(false, false, protocol, src_portid, dst_port, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_netlink_unicast_nowait(rref_t __rref, int protocol, unsigned int src_portid, unsigned int dst_port, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_netcall_netlink_unicast_nowait(false, false, __rref, protocol, src_portid, dst_port, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_netcall_vsock_register_vm(unsigned long long __rpc_info, void *__buf, unsigned int cid, unsigned int dev_group_id);


extern int __actvcapcall_netcall_vsock_register_vm(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cid, unsigned int dev_group_id);


static inline int __attribute__((always_inline)) actvxactcall_netcall_vsock_register_vm(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cid, unsigned int dev_group_id)
{
	const unsigned long __callno = __hmnet_method_vsock_register_vm;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_vsock_register_vm(rpc_info, NULL, cid, dev_group_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_vsock_register_vm(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cid, unsigned int dev_group_id)
{
	const unsigned long __callno = __hmnet_method_vsock_register_vm;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_vsock_register_vm(__rref, rpc_info, NULL, cid, dev_group_id);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_vsock_register_vm(unsigned int cid, unsigned int dev_group_id)
{
	return actvxactcall_netcall_vsock_register_vm(false, false, cid, dev_group_id);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_vsock_register_vm(rref_t __rref, unsigned int cid, unsigned int dev_group_id)
{
	return actvxactcapcall_netcall_vsock_register_vm(false, false, __rref, cid, dev_group_id);
}

extern int __actvcall_netcall_vsock_vm_notify(unsigned long long __rpc_info, void *__buf, unsigned int cid, unsigned short cmd, void *args, size_t size);


extern int __actvcapcall_netcall_vsock_vm_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cid, unsigned short cmd, void *args, size_t size);


static inline int __attribute__((always_inline)) actvxactcall_netcall_vsock_vm_notify(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cid, unsigned short cmd, void *args, size_t size)
{
	const unsigned long __callno = __hmnet_method_vsock_vm_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned short), __arch_actv_rpc_stksz_alignment(unsigned short));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_vsock_vm_notify(rpc_info, NULL, cid, cmd, args, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_vsock_vm_notify(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cid, unsigned short cmd, void *args, size_t size)
{
	const unsigned long __callno = __hmnet_method_vsock_vm_notify;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned short), __arch_actv_rpc_stksz_alignment(unsigned short));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(size_t), __arch_actv_rpc_stksz_alignment(size_t));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_vsock_vm_notify(__rref, rpc_info, NULL, cid, cmd, args, size);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_vsock_vm_notify(unsigned int cid, unsigned short cmd, void *args, size_t size)
{
	return actvxactcall_netcall_vsock_vm_notify(false, false, cid, cmd, args, size);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_vsock_vm_notify(rref_t __rref, unsigned int cid, unsigned short cmd, void *args, size_t size)
{
	return actvxactcapcall_netcall_vsock_vm_notify(false, false, __rref, cid, cmd, args, size);
}

extern int __actvcall_netcall_vsock_unregister_vm(unsigned long long __rpc_info, void *__buf, unsigned int cid);


extern int __actvcapcall_netcall_vsock_unregister_vm(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cid);


static inline int __attribute__((always_inline)) actvxactcall_netcall_vsock_unregister_vm(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cid)
{
	const unsigned long __callno = __hmnet_method_vsock_unregister_vm;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_vsock_unregister_vm(rpc_info, NULL, cid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_vsock_unregister_vm(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cid)
{
	const unsigned long __callno = __hmnet_method_vsock_unregister_vm;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_vsock_unregister_vm(__rref, rpc_info, NULL, cid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_vsock_unregister_vm(unsigned int cid)
{
	return actvxactcall_netcall_vsock_unregister_vm(false, false, cid);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_vsock_unregister_vm(rref_t __rref, unsigned int cid)
{
	return actvxactcapcall_netcall_vsock_unregister_vm(false, false, __rref, cid);
}

extern int __actvcall_netcall_ldk_domain_register(unsigned long long __rpc_info, void *__buf, int family, unsigned long long dh_ap_uref);


extern int __actvcapcall_netcall_ldk_domain_register(rref_t __rref, unsigned long long __rpc_info, void *__buf, int family, unsigned long long dh_ap_uref);


static inline int __attribute__((always_inline)) actvxactcall_netcall_ldk_domain_register(bool is_xact_fwd, bool is_xact_allow_refwd, int family, unsigned long long dh_ap_uref)
{
	const unsigned long __callno = __hmnet_method_ldk_domain_register;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_ldk_domain_register(rpc_info, NULL, family, dh_ap_uref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_ldk_domain_register(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, int family, unsigned long long dh_ap_uref)
{
	const unsigned long __callno = __hmnet_method_ldk_domain_register;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(int), __arch_actv_rpc_stksz_alignment(int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned long long), __arch_actv_rpc_stksz_alignment(unsigned long long));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_ldk_domain_register(__rref, rpc_info, NULL, family, dh_ap_uref);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_ldk_domain_register(int family, unsigned long long dh_ap_uref)
{
	return actvxactcall_netcall_ldk_domain_register(false, false, family, dh_ap_uref);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_ldk_domain_register(rref_t __rref, int family, unsigned long long dh_ap_uref)
{
	return actvxactcapcall_netcall_ldk_domain_register(false, false, __rref, family, dh_ap_uref);
}

extern int __actvcall_netcall_procfs_create_ldk_entry(unsigned long long __rpc_info, void *__buf, unsigned int nsid, const char *path, void *ops, void *ctx, unsigned int mode);


extern int __actvcapcall_netcall_procfs_create_ldk_entry(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int nsid, const char *path, void *ops, void *ctx, unsigned int mode);


static inline int __attribute__((always_inline)) actvxactcall_netcall_procfs_create_ldk_entry(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int nsid, const char *path, void *ops, void *ctx, unsigned int mode)
{
	const unsigned long __callno = __hmnet_method_procfs_create_ldk_entry;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_procfs_create_ldk_entry(rpc_info, NULL, nsid, path, ops, ctx, mode);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_procfs_create_ldk_entry(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int nsid, const char *path, void *ops, void *ctx, unsigned int mode)
{
	const unsigned long __callno = __hmnet_method_procfs_create_ldk_entry;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_procfs_create_ldk_entry(__rref, rpc_info, NULL, nsid, path, ops, ctx, mode);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_procfs_create_ldk_entry(unsigned int nsid, const char *path, void *ops, void *ctx, unsigned int mode)
{
	return actvxactcall_netcall_procfs_create_ldk_entry(false, false, nsid, path, ops, ctx, mode);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_procfs_create_ldk_entry(rref_t __rref, unsigned int nsid, const char *path, void *ops, void *ctx, unsigned int mode)
{
	return actvxactcapcall_netcall_procfs_create_ldk_entry(false, false, __rref, nsid, path, ops, ctx, mode);
}

extern int __actvcall_netcall_procfs_delete_ldk_entry_specific(unsigned long long __rpc_info, void *__buf, const char *pathname, unsigned int nsid);


extern int __actvcapcall_netcall_procfs_delete_ldk_entry_specific(rref_t __rref, unsigned long long __rpc_info, void *__buf, const char *pathname, unsigned int nsid);


static inline int __attribute__((always_inline)) actvxactcall_netcall_procfs_delete_ldk_entry_specific(bool is_xact_fwd, bool is_xact_allow_refwd, const char *pathname, unsigned int nsid)
{
	const unsigned long __callno = __hmnet_method_procfs_delete_ldk_entry_specific;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_procfs_delete_ldk_entry_specific(rpc_info, NULL, pathname, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_procfs_delete_ldk_entry_specific(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, const char *pathname, unsigned int nsid)
{
	const unsigned long __callno = __hmnet_method_procfs_delete_ldk_entry_specific;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(const char*), __arch_actv_rpc_stksz_alignment(const char*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_procfs_delete_ldk_entry_specific(__rref, rpc_info, NULL, pathname, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_procfs_delete_ldk_entry_specific(const char *pathname, unsigned int nsid)
{
	return actvxactcall_netcall_procfs_delete_ldk_entry_specific(false, false, pathname, nsid);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_procfs_delete_ldk_entry_specific(rref_t __rref, const char *pathname, unsigned int nsid)
{
	return actvxactcapcall_netcall_procfs_delete_ldk_entry_specific(false, false, __rref, pathname, nsid);
}

extern int __actvcall_netcall_procfs_create_ldk_entries(unsigned long long __rpc_info, void *__buf, void *parent, unsigned int size, void *ops);


extern int __actvcapcall_netcall_procfs_create_ldk_entries(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *parent, unsigned int size, void *ops);


static inline int __attribute__((always_inline)) actvxactcall_netcall_procfs_create_ldk_entries(bool is_xact_fwd, bool is_xact_allow_refwd, void *parent, unsigned int size, void *ops)
{
	const unsigned long __callno = __hmnet_method_procfs_create_ldk_entries;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_procfs_create_ldk_entries(rpc_info, NULL, parent, size, ops);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_procfs_create_ldk_entries(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, void *parent, unsigned int size, void *ops)
{
	const unsigned long __callno = __hmnet_method_procfs_create_ldk_entries;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(void*), __arch_actv_rpc_stksz_alignment(void*));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_procfs_create_ldk_entries(__rref, rpc_info, NULL, parent, size, ops);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_procfs_create_ldk_entries(void *parent, unsigned int size, void *ops)
{
	return actvxactcall_netcall_procfs_create_ldk_entries(false, false, parent, size, ops);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_procfs_create_ldk_entries(rref_t __rref, void *parent, unsigned int size, void *ops)
{
	return actvxactcapcall_netcall_procfs_create_ldk_entries(false, false, __rref, parent, size, ops);
}

extern int __actvcall_netcall_sec_mgmt(unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command);


extern int __actvcapcall_netcall_sec_mgmt(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command);


static inline int __attribute__((always_inline)) actvxactcall_netcall_sec_mgmt(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int module, unsigned int command, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_sec_mgmt;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_sec_mgmt(rpc_info, ___void_buf, module, command);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_sec_mgmt(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int module, unsigned int command, void *___void_buf, unsigned long ___void_buf_sz)
{
	const unsigned long __callno = __hmnet_method_sec_mgmt;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, ___void_buf_sz, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_sec_mgmt(__rref, rpc_info, ___void_buf, module, command);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_sec_mgmt(unsigned int module, unsigned int command, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcall_netcall_sec_mgmt(false, false, module, command, ___void_buf, ___void_buf_sz);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_sec_mgmt(rref_t __rref, unsigned int module, unsigned int command, void *___void_buf, unsigned long ___void_buf_sz)
{
	return actvxactcapcall_netcall_sec_mgmt(false, false, __rref, module, command, ___void_buf, ___void_buf_sz);
}

extern int __actvcall_netcall_namespace_new_ns(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, unsigned int nsid);


extern int __actvcapcall_netcall_namespace_new_ns(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, unsigned int nsid);


static inline int __attribute__((always_inline)) actvxactcall_netcall_namespace_new_ns(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int cnode_idx, unsigned int nsid)
{
	const unsigned long __callno = __hmnet_method_namespace_new_ns;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_namespace_new_ns(rpc_info, NULL, cnode_idx, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_namespace_new_ns(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int cnode_idx, unsigned int nsid)
{
	const unsigned long __callno = __hmnet_method_namespace_new_ns;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_namespace_new_ns(__rref, rpc_info, NULL, cnode_idx, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_namespace_new_ns(unsigned int cnode_idx, unsigned int nsid)
{
	return actvxactcall_netcall_namespace_new_ns(false, false, cnode_idx, nsid);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_namespace_new_ns(rref_t __rref, unsigned int cnode_idx, unsigned int nsid)
{
	return actvxactcapcall_netcall_namespace_new_ns(false, false, __rref, cnode_idx, nsid);
}

extern int __actvcall_netcall_namespace_destroy_ns(unsigned long long __rpc_info, void *__buf, unsigned int nsid);


extern int __actvcapcall_netcall_namespace_destroy_ns(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int nsid);


static inline int __attribute__((always_inline)) actvxactcall_netcall_namespace_destroy_ns(bool is_xact_fwd, bool is_xact_allow_refwd, unsigned int nsid)
{
	const unsigned long __callno = __hmnet_method_namespace_destroy_ns;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(0, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcall_netcall_namespace_destroy_ns(rpc_info, NULL, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvxactcapcall_netcall_namespace_destroy_ns(bool is_xact_fwd, bool is_xact_allow_refwd, rref_t __rref, unsigned int nsid)
{
	const unsigned long __callno = __hmnet_method_namespace_destroy_ns;
	int __res;
	unsigned long __stacksz = 0UL;
	struct __arch_actv_stksz_state __stksz_state;
	__stksz_state = __arch_actv_rpc_stksz_init(1, 0);
	__stksz_state = __arch_actv_rpc_stksz_append(__stksz_state, sizeof(unsigned int), __arch_actv_rpc_stksz_alignment(unsigned int));
	__stacksz = __arch_actv_rpc_stksz_done(__stksz_state);
	do {
		unsigned long long rpc_info = __arch_actv_xact_rpc_info_gen(is_xact_fwd, is_xact_allow_refwd, __callno, 0UL, __stacksz / sizeof(unsigned long));
		__res = __actvcapcall_netcall_namespace_destroy_ns(__rref, rpc_info, NULL, nsid);
	} while (__res == (int)E_HM_ACTIVATION_RPC_RETRY);
	return __res;
}

static inline int __attribute__((always_inline)) actvcall_netcall_namespace_destroy_ns(unsigned int nsid)
{
	return actvxactcall_netcall_namespace_destroy_ns(false, false, nsid);
}

static inline int __attribute__((always_inline)) actvcapcall_netcall_namespace_destroy_ns(rref_t __rref, unsigned int nsid)
{
	return actvxactcapcall_netcall_namespace_destroy_ns(false, false, __rref, nsid);
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

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_netcall_socket_revoke(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_netcall_socket_revoke(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_socket_revoke(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_netcall_socket_revoke(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_socket_revoke(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_netcall_socket_revoke(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_netcall_socket_fork(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_netcall_socket_fork(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_socket_fork(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_netcall_socket_fork(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_socket_fork(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_netcall_socket_fork(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_socket_dev_ioctl(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_socket_dev_ioctl(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_socket_dev_ioctl(unsigned long long __rpc_info, void *__buf, int cmd, void *args)
{
	int __ret;
	int __maybe_unused __actv_local_var_cmd = cmd;
	__maybe_unused void *__actv_local_var_args = args;
	__ret = ____actvcall_netcall_socket_dev_ioctl(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_socket_dev_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, int cmd, void *args)
{
	int __ret;
	int __maybe_unused __actv_local_var_cmd = cmd;
	__maybe_unused void *__actv_local_var_args = args;
	__ret = ____actvcapcall_netcall_socket_dev_ioctl(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_netcall_socket_send_fd(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_netcall_socket_send_fd(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_socket_send_fd(unsigned long long __rpc_info, void *__buf, unsigned long argu_info)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_argu_info = argu_info;
	__ret = ____actvcall_netcall_socket_send_fd(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(argu_info, __actv_local_var_argu_info));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_socket_send_fd(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long argu_info)
{
	int __ret;
	unsigned long __maybe_unused __actv_local_var_argu_info = argu_info;
	__ret = ____actvcapcall_netcall_socket_send_fd(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(argu_info, __actv_local_var_argu_info));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_socket_put_send_fd(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_socket_put_send_fd(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_socket_put_send_fd(unsigned long long __rpc_info, void *__buf, uintptr_t argu_info)
{
	int __ret;
	uintptr_t __maybe_unused __actv_local_var_argu_info = argu_info;
	__ret = ____actvcall_netcall_socket_put_send_fd(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(argu_info, __actv_local_var_argu_info));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_socket_put_send_fd(rref_t __rref, unsigned long long __rpc_info, void *__buf, uintptr_t argu_info)
{
	int __ret;
	uintptr_t __maybe_unused __actv_local_var_argu_info = argu_info;
	__ret = ____actvcapcall_netcall_socket_put_send_fd(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(argu_info, __actv_local_var_argu_info));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_socket_proc_unauthoried_files(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_socket_proc_unauthoried_files(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_socket_proc_unauthoried_files(unsigned long long __rpc_info, void *__buf, int sel_null_fd)
{
	int __ret;
	int __maybe_unused __actv_local_var_sel_null_fd = sel_null_fd;
	__ret = ____actvcall_netcall_socket_proc_unauthoried_files(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sel_null_fd, __actv_local_var_sel_null_fd));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_socket_proc_unauthoried_files(rref_t __rref, unsigned long long __rpc_info, void *__buf, int sel_null_fd)
{
	int __ret;
	int __maybe_unused __actv_local_var_sel_null_fd = sel_null_fd;
	__ret = ____actvcapcall_netcall_socket_proc_unauthoried_files(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sel_null_fd, __actv_local_var_sel_null_fd));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_socket_query_info(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_socket_query_info(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_socket_query_info(unsigned long long __rpc_info, void *__buf, int fd, uint32_t cnode, void *info)
{
	int __ret;
	int __maybe_unused __actv_local_var_fd = fd;
	uint32_t __maybe_unused __actv_local_var_cnode = cnode;
	__maybe_unused void *__actv_local_var_info = info;
	__ret = ____actvcall_netcall_socket_query_info(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(cnode, __actv_local_var_cnode), __ARCH_ACTV_RPC_PASS_ARG(info, __actv_local_var_info));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_socket_query_info(rref_t __rref, unsigned long long __rpc_info, void *__buf, int fd, uint32_t cnode, void *info)
{
	int __ret;
	int __maybe_unused __actv_local_var_fd = fd;
	uint32_t __maybe_unused __actv_local_var_cnode = cnode;
	__maybe_unused void *__actv_local_var_info = info;
	__ret = ____actvcapcall_netcall_socket_query_info(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(cnode, __actv_local_var_cnode), __ARCH_ACTV_RPC_PASS_ARG(info, __actv_local_var_info));
	return __ret;
}

static long __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_socket_sys_read(unsigned long long rpc_info, void *__buf, ...);


static long __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_socket_sys_read(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


long __internal_actvcall __actvcall_netcall_socket_sys_read(unsigned long long __rpc_info, void *__buf, uint32_t cnode, int fd, void *buf, size_t len, int flags)
{
	long __ret;
	uint32_t __maybe_unused __actv_local_var_cnode = cnode;
	int __maybe_unused __actv_local_var_fd = fd;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_len = len;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcall_netcall_socket_sys_read(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode, __actv_local_var_cnode), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

long __internal_actvcall __actvcapcall_netcall_socket_sys_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, uint32_t cnode, int fd, void *buf, size_t len, int flags)
{
	long __ret;
	uint32_t __maybe_unused __actv_local_var_cnode = cnode;
	int __maybe_unused __actv_local_var_fd = fd;
	__maybe_unused void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_len = len;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcapcall_netcall_socket_sys_read(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode, __actv_local_var_cnode), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

static long __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_socket_sys_write(unsigned long long rpc_info, void *__buf, ...);


static long __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_socket_sys_write(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


long __internal_actvcall __actvcall_netcall_socket_sys_write(unsigned long long __rpc_info, void *__buf, uint32_t cnode, int fd, const void *buf, size_t len, int flags)
{
	long __ret;
	uint32_t __maybe_unused __actv_local_var_cnode = cnode;
	int __maybe_unused __actv_local_var_fd = fd;
	__maybe_unused const void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_len = len;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcall_netcall_socket_sys_write(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode, __actv_local_var_cnode), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

long __internal_actvcall __actvcapcall_netcall_socket_sys_write(rref_t __rref, unsigned long long __rpc_info, void *__buf, uint32_t cnode, int fd, const void *buf, size_t len, int flags)
{
	long __ret;
	uint32_t __maybe_unused __actv_local_var_cnode = cnode;
	int __maybe_unused __actv_local_var_fd = fd;
	__maybe_unused const void *__actv_local_var_buf = buf;
	size_t __maybe_unused __actv_local_var_len = len;
	int __maybe_unused __actv_local_var_flags = flags;
	__ret = ____actvcapcall_netcall_socket_sys_write(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode, __actv_local_var_cnode), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(buf, __actv_local_var_buf), __ARCH_ACTV_RPC_PASS_ARG(len, __actv_local_var_len), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags));
	return __ret;
}

static long __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_socket_sys_ioctl(unsigned long long rpc_info, void *__buf, ...);


static long __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_socket_sys_ioctl(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


long __internal_actvcall __actvcall_netcall_socket_sys_ioctl(unsigned long long __rpc_info, void *__buf, uint32_t cnode, int fd, int request, unsigned long arg)
{
	long __ret;
	uint32_t __maybe_unused __actv_local_var_cnode = cnode;
	int __maybe_unused __actv_local_var_fd = fd;
	int __maybe_unused __actv_local_var_request = request;
	unsigned long __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcall_netcall_socket_sys_ioctl(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode, __actv_local_var_cnode), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(request, __actv_local_var_request), __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

long __internal_actvcall __actvcapcall_netcall_socket_sys_ioctl(rref_t __rref, unsigned long long __rpc_info, void *__buf, uint32_t cnode, int fd, int request, unsigned long arg)
{
	long __ret;
	uint32_t __maybe_unused __actv_local_var_cnode = cnode;
	int __maybe_unused __actv_local_var_fd = fd;
	int __maybe_unused __actv_local_var_request = request;
	unsigned long __maybe_unused __actv_local_var_arg = arg;
	__ret = ____actvcapcall_netcall_socket_sys_ioctl(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode, __actv_local_var_cnode), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(request, __actv_local_var_request), __ARCH_ACTV_RPC_PASS_ARG(arg, __actv_local_var_arg));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_netcall_socket_mmap(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_netcall_socket_mmap(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_socket_mmap(unsigned long long __rpc_info, void *__buf, int fd, unsigned int vs_cnode_idx, uintptr_t vaddr, size_t aligned_len, unsigned int prot, unsigned int flags, long long offset)
{
	int __ret;
	int __maybe_unused __actv_local_var_fd = fd;
	unsigned int __maybe_unused __actv_local_var_vs_cnode_idx = vs_cnode_idx;
	uintptr_t __maybe_unused __actv_local_var_vaddr = vaddr;
	size_t __maybe_unused __actv_local_var_aligned_len = aligned_len;
	unsigned int __maybe_unused __actv_local_var_prot = prot;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	long long __maybe_unused __actv_local_var_offset = offset;
	__ret = ____actvcall_netcall_socket_mmap(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(vs_cnode_idx, __actv_local_var_vs_cnode_idx), __ARCH_ACTV_RPC_PASS_ARG(vaddr, __actv_local_var_vaddr), __ARCH_ACTV_RPC_PASS_ARG(aligned_len, __actv_local_var_aligned_len), __ARCH_ACTV_RPC_PASS_ARG(prot, __actv_local_var_prot), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_socket_mmap(rref_t __rref, unsigned long long __rpc_info, void *__buf, int fd, unsigned int vs_cnode_idx, uintptr_t vaddr, size_t aligned_len, unsigned int prot, unsigned int flags, long long offset)
{
	int __ret;
	int __maybe_unused __actv_local_var_fd = fd;
	unsigned int __maybe_unused __actv_local_var_vs_cnode_idx = vs_cnode_idx;
	uintptr_t __maybe_unused __actv_local_var_vaddr = vaddr;
	size_t __maybe_unused __actv_local_var_aligned_len = aligned_len;
	unsigned int __maybe_unused __actv_local_var_prot = prot;
	unsigned int __maybe_unused __actv_local_var_flags = flags;
	long long __maybe_unused __actv_local_var_offset = offset;
	__ret = ____actvcapcall_netcall_socket_mmap(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(vs_cnode_idx, __actv_local_var_vs_cnode_idx), __ARCH_ACTV_RPC_PASS_ARG(vaddr, __actv_local_var_vaddr), __ARCH_ACTV_RPC_PASS_ARG(aligned_len, __actv_local_var_aligned_len), __ARCH_ACTV_RPC_PASS_ARG(prot, __actv_local_var_prot), __ARCH_ACTV_RPC_PASS_ARG(flags, __actv_local_var_flags), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_socket_attach(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_socket_attach(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_socket_attach(unsigned long long __rpc_info, void *__buf, struct ux_socket *sock, unsigned int cnode, int fd)
{
	int __ret;
	__maybe_unused struct ux_socket *__actv_local_var_sock = sock;
	unsigned int __maybe_unused __actv_local_var_cnode = cnode;
	int __maybe_unused __actv_local_var_fd = fd;
	__ret = ____actvcall_netcall_socket_attach(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sock, __actv_local_var_sock), __ARCH_ACTV_RPC_PASS_ARG(cnode, __actv_local_var_cnode), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_socket_attach(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct ux_socket *sock, unsigned int cnode, int fd)
{
	int __ret;
	__maybe_unused struct ux_socket *__actv_local_var_sock = sock;
	unsigned int __maybe_unused __actv_local_var_cnode = cnode;
	int __maybe_unused __actv_local_var_fd = fd;
	__ret = ____actvcapcall_netcall_socket_attach(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sock, __actv_local_var_sock), __ARCH_ACTV_RPC_PASS_ARG(cnode, __actv_local_var_cnode), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_retbuf"))) __internal_rpccall ____actvcall_netcall_socket_get(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_retbuf"))) __internal_rpccapcall ____actvcapcall_netcall_socket_get(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_socket_get(unsigned long long __rpc_info, void *__buf, unsigned int cnode, int fd)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode = cnode;
	int __maybe_unused __actv_local_var_fd = fd;
	__ret = ____actvcall_netcall_socket_get(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode, __actv_local_var_cnode), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_socket_get(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode, int fd)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode = cnode;
	int __maybe_unused __actv_local_var_fd = fd;
	__ret = ____actvcapcall_netcall_socket_get(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode, __actv_local_var_cnode), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_socket_put(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_socket_put(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_socket_put(unsigned long long __rpc_info, void *__buf, struct ux_socket *sock)
{
	int __ret;
	__maybe_unused struct ux_socket *__actv_local_var_sock = sock;
	__ret = ____actvcall_netcall_socket_put(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sock, __actv_local_var_sock));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_socket_put(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct ux_socket *sock)
{
	int __ret;
	__maybe_unused struct ux_socket *__actv_local_var_sock = sock;
	__ret = ____actvcapcall_netcall_socket_put(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(sock, __actv_local_var_sock));
	return __ret;
}

int __lsyscall_socket_close(long syscall_num, long long sock, int fd)
{
	long ret = lsyscall_syscall2(syscall_num, (long)sock, (long)fd);
	if (ret < 0 && ret > -4096) {
		ret = posix2hmerrno(-ret);
	}
	return (int)ret;
}

int __lsyscall_socket_bind(long syscall_num, long long socket, const void *addr, socklen_t len)
{
	long ret = lsyscall_syscall3(syscall_num, (long)socket, (long)addr, (long)len);
	if (ret < 0 && ret > -4096) {
		ret = posix2hmerrno(-ret);
	}
	return (int)ret;
}

int __lsyscall_socket_listen(long syscall_num, long long socket, int backlog)
{
	long ret = lsyscall_syscall2(syscall_num, (long)socket, (long)backlog);
	if (ret < 0 && ret > -4096) {
		ret = posix2hmerrno(-ret);
	}
	return (int)ret;
}

int __lsyscall_socket_fcntl(long syscall_num, long long socket, int cmd, unsigned long val, int fd)
{
	long ret = lsyscall_syscall4(syscall_num, (long)socket, (long)cmd, (long)val, (long)fd);
	if (ret < 0 && ret > -4096) {
		ret = posix2hmerrno(-ret);
	}
	return (int)ret;
}

#ifdef __arm__
int __lsyscall_socket_fcntl64(long syscall_num, long long socket, int cmd, unsigned long val, int fd)
{
	long ret = lsyscall_syscall4(syscall_num, (long)socket, (long)cmd, (long)val, (long)fd);
	if (ret < 0 && ret > -4096) {
		ret = posix2hmerrno(-ret);
	}
	return (int)ret;
}

#endif
static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_epoll_recheck_revents(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_epoll_recheck_revents(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_epoll_recheck_revents(unsigned long long __rpc_info, void *__buf, void *data)
{
	int __ret;
	__maybe_unused void *__actv_local_var_data = data;
	__ret = ____actvcall_netcall_epoll_recheck_revents(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(data, __actv_local_var_data));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_epoll_recheck_revents(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *data)
{
	int __ret;
	__maybe_unused void *__actv_local_var_data = data;
	__ret = ____actvcapcall_netcall_epoll_recheck_revents(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(data, __actv_local_var_data));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_cfg_loglevel(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_cfg_loglevel(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_cfg_loglevel(unsigned long long __rpc_info, void *__buf, unsigned int loglevel)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_loglevel = loglevel;
	__ret = ____actvcall_netcall_cfg_loglevel(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(loglevel, __actv_local_var_loglevel));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_cfg_loglevel(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int loglevel)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_loglevel = loglevel;
	__ret = ____actvcapcall_netcall_cfg_loglevel(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(loglevel, __actv_local_var_loglevel));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_cfg_show_stats(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_cfg_show_stats(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_cfg_show_stats(unsigned long long __rpc_info, void *__buf, struct sysif_protocol ss_protocol, unsigned long long shm_key)
{
	int __ret;
	struct sysif_protocol __maybe_unused __actv_local_var_ss_protocol = ss_protocol;
	unsigned long long __maybe_unused __actv_local_var_shm_key = shm_key;
	__ret = ____actvcall_netcall_cfg_show_stats(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ss_protocol, __actv_local_var_ss_protocol), __ARCH_ACTV_RPC_PASS_ARG(shm_key, __actv_local_var_shm_key));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_cfg_show_stats(rref_t __rref, unsigned long long __rpc_info, void *__buf, struct sysif_protocol ss_protocol, unsigned long long shm_key)
{
	int __ret;
	struct sysif_protocol __maybe_unused __actv_local_var_ss_protocol = ss_protocol;
	unsigned long long __maybe_unused __actv_local_var_shm_key = shm_key;
	__ret = ____actvcapcall_netcall_cfg_show_stats(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(ss_protocol, __actv_local_var_ss_protocol), __ARCH_ACTV_RPC_PASS_ARG(shm_key, __actv_local_var_shm_key));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_cfg_getifaddrs(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_cfg_getifaddrs(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_cfg_getifaddrs(unsigned long long __rpc_info, void *__buf, unsigned long long shm_key)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_shm_key = shm_key;
	__ret = ____actvcall_netcall_cfg_getifaddrs(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(shm_key, __actv_local_var_shm_key));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_cfg_getifaddrs(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned long long shm_key)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_shm_key = shm_key;
	__ret = ____actvcapcall_netcall_cfg_getifaddrs(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(shm_key, __actv_local_var_shm_key));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_netdev_rx(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_netdev_rx(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_netdev_rx(unsigned long long __rpc_info, void *__buf, unsigned int cell_pool_offset, int shm_len, int rx_len, int offset, int devno)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cell_pool_offset = cell_pool_offset;
	int __maybe_unused __actv_local_var_shm_len = shm_len;
	int __maybe_unused __actv_local_var_rx_len = rx_len;
	int __maybe_unused __actv_local_var_offset = offset;
	int __maybe_unused __actv_local_var_devno = devno;
	__ret = ____actvcall_netcall_netdev_rx(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cell_pool_offset, __actv_local_var_cell_pool_offset), __ARCH_ACTV_RPC_PASS_ARG(shm_len, __actv_local_var_shm_len), __ARCH_ACTV_RPC_PASS_ARG(rx_len, __actv_local_var_rx_len), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(devno, __actv_local_var_devno));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_netdev_rx(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cell_pool_offset, int shm_len, int rx_len, int offset, int devno)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cell_pool_offset = cell_pool_offset;
	int __maybe_unused __actv_local_var_shm_len = shm_len;
	int __maybe_unused __actv_local_var_rx_len = rx_len;
	int __maybe_unused __actv_local_var_offset = offset;
	int __maybe_unused __actv_local_var_devno = devno;
	__ret = ____actvcapcall_netcall_netdev_rx(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cell_pool_offset, __actv_local_var_cell_pool_offset), __ARCH_ACTV_RPC_PASS_ARG(shm_len, __actv_local_var_shm_len), __ARCH_ACTV_RPC_PASS_ARG(rx_len, __actv_local_var_rx_len), __ARCH_ACTV_RPC_PASS_ARG(offset, __actv_local_var_offset), __ARCH_ACTV_RPC_PASS_ARG(devno, __actv_local_var_devno));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_netdev_notify(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_netdev_notify(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_netdev_notify(unsigned long long __rpc_info, void *__buf, enum hm_netdev_event event, int devno, int option, struct netdev_ifname ifname, unsigned long long features)
{
	int __ret;
	enum hm_netdev_event __maybe_unused __actv_local_var_event = event;
	int __maybe_unused __actv_local_var_devno = devno;
	int __maybe_unused __actv_local_var_option = option;
	struct netdev_ifname __maybe_unused __actv_local_var_ifname = ifname;
	unsigned long long __maybe_unused __actv_local_var_features = features;
	__ret = ____actvcall_netcall_netdev_notify(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(event, __actv_local_var_event), __ARCH_ACTV_RPC_PASS_ARG(devno, __actv_local_var_devno), __ARCH_ACTV_RPC_PASS_ARG(option, __actv_local_var_option), __ARCH_ACTV_RPC_PASS_ARG(ifname, __actv_local_var_ifname), __ARCH_ACTV_RPC_PASS_ARG(features, __actv_local_var_features));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_netdev_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, enum hm_netdev_event event, int devno, int option, struct netdev_ifname ifname, unsigned long long features)
{
	int __ret;
	enum hm_netdev_event __maybe_unused __actv_local_var_event = event;
	int __maybe_unused __actv_local_var_devno = devno;
	int __maybe_unused __actv_local_var_option = option;
	struct netdev_ifname __maybe_unused __actv_local_var_ifname = ifname;
	unsigned long long __maybe_unused __actv_local_var_features = features;
	__ret = ____actvcapcall_netcall_netdev_notify(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(event, __actv_local_var_event), __ARCH_ACTV_RPC_PASS_ARG(devno, __actv_local_var_devno), __ARCH_ACTV_RPC_PASS_ARG(option, __actv_local_var_option), __ARCH_ACTV_RPC_PASS_ARG(ifname, __actv_local_var_ifname), __ARCH_ACTV_RPC_PASS_ARG(features, __actv_local_var_features));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_netcall_netdev_rx_ex_slowdata(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_netcall_netdev_rx_ex_slowdata(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_netdev_rx_ex_slowdata(unsigned long long __rpc_info, void *__buf, int num)
{
	int __ret;
	int __maybe_unused __actv_local_var_num = num;
	__ret = ____actvcall_netcall_netdev_rx_ex_slowdata(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(num, __actv_local_var_num));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_netdev_rx_ex_slowdata(rref_t __rref, unsigned long long __rpc_info, void *__buf, int num)
{
	int __ret;
	int __maybe_unused __actv_local_var_num = num;
	__ret = ____actvcapcall_netcall_netdev_rx_ex_slowdata(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(num, __actv_local_var_num));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_netcall_procfs_read(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_netcall_procfs_read(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_procfs_read(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_netcall_procfs_read(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_procfs_read(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_netcall_procfs_read(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_netcall_procfs_write(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_netcall_procfs_write(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_procfs_write(unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcall_netcall_procfs_write(__rpc_info, __buf);
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_procfs_write(rref_t __rref, unsigned long long __rpc_info, void *__buf)
{
	int __ret;
	__ret = ____actvcapcall_netcall_procfs_write(__rref, __rpc_info, __buf);
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_procfs_fds_read(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_procfs_fds_read(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_procfs_fds_read(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, unsigned long long fd_map, unsigned int max_files)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	unsigned long long __maybe_unused __actv_local_var_fd_map = fd_map;
	unsigned int __maybe_unused __actv_local_var_max_files = max_files;
	__ret = ____actvcall_netcall_procfs_fds_read(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx), __ARCH_ACTV_RPC_PASS_ARG(fd_map, __actv_local_var_fd_map), __ARCH_ACTV_RPC_PASS_ARG(max_files, __actv_local_var_max_files));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_procfs_fds_read(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, unsigned long long fd_map, unsigned int max_files)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	unsigned long long __maybe_unused __actv_local_var_fd_map = fd_map;
	unsigned int __maybe_unused __actv_local_var_max_files = max_files;
	__ret = ____actvcapcall_netcall_procfs_fds_read(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx), __ARCH_ACTV_RPC_PASS_ARG(fd_map, __actv_local_var_fd_map), __ARCH_ACTV_RPC_PASS_ARG(max_files, __actv_local_var_max_files));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_procfs_get_fd_path(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_procfs_get_fd_path(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_procfs_get_fd_path(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, int fd, unsigned long long path_buf, unsigned int path_buf_len)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	int __maybe_unused __actv_local_var_fd = fd;
	unsigned long long __maybe_unused __actv_local_var_path_buf = path_buf;
	unsigned int __maybe_unused __actv_local_var_path_buf_len = path_buf_len;
	__ret = ____actvcall_netcall_procfs_get_fd_path(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(path_buf, __actv_local_var_path_buf), __ARCH_ACTV_RPC_PASS_ARG(path_buf_len, __actv_local_var_path_buf_len));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_procfs_get_fd_path(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, int fd, unsigned long long path_buf, unsigned int path_buf_len)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	int __maybe_unused __actv_local_var_fd = fd;
	unsigned long long __maybe_unused __actv_local_var_path_buf = path_buf;
	unsigned int __maybe_unused __actv_local_var_path_buf_len = path_buf_len;
	__ret = ____actvcapcall_netcall_procfs_get_fd_path(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx), __ARCH_ACTV_RPC_PASS_ARG(fd, __actv_local_var_fd), __ARCH_ACTV_RPC_PASS_ARG(path_buf, __actv_local_var_path_buf), __ARCH_ACTV_RPC_PASS_ARG(path_buf_len, __actv_local_var_path_buf_len));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_fallback"))) __internal_rpccall ____actvcall_netcall_netlink_register_generic(unsigned long long rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, ...);


static int __attribute__((alias("__internal_rpccapcall_fallback"))) __internal_rpccapcall ____actvcapcall_netcall_netlink_register_generic(rref_t __rref, unsigned long long rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, ...);


int __internal_actvcall __actvcall_netcall_netlink_register_generic(unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, unsigned long long dh_ap_uref)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_dh_ap_uref = dh_ap_uref;
	__ret = ____actvcall_netcall_netlink_register_generic(__rpc_info, __buf, __rply_buf, rply_buf_sz, __ARCH_ACTV_RPC_PASS_ARG(dh_ap_uref, __actv_local_var_dh_ap_uref));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_netlink_register_generic(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *__rply_buf, unsigned long rply_buf_sz, unsigned long long dh_ap_uref)
{
	int __ret;
	unsigned long long __maybe_unused __actv_local_var_dh_ap_uref = dh_ap_uref;
	__ret = ____actvcapcall_netcall_netlink_register_generic(__rref, __rpc_info, __buf, __rply_buf, rply_buf_sz, __ARCH_ACTV_RPC_PASS_ARG(dh_ap_uref, __actv_local_var_dh_ap_uref));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_netlink_unregister_generic(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_netlink_unregister_generic(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_netlink_unregister_generic(unsigned long long __rpc_info, void *__buf, unsigned short familyid)
{
	int __ret;
	unsigned short __maybe_unused __actv_local_var_familyid = familyid;
	__ret = ____actvcall_netcall_netlink_unregister_generic(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(familyid, __actv_local_var_familyid));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_netlink_unregister_generic(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned short familyid)
{
	int __ret;
	unsigned short __maybe_unused __actv_local_var_familyid = familyid;
	__ret = ____actvcapcall_netcall_netlink_unregister_generic(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(familyid, __actv_local_var_familyid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_netcall_netlink_unicast(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_netcall_netlink_unicast(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_netlink_unicast(unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned int dst_port)
{
	int __ret;
	int __maybe_unused __actv_local_var_protocol = protocol;
	unsigned int __maybe_unused __actv_local_var_src_portid = src_portid;
	unsigned int __maybe_unused __actv_local_var_dst_port = dst_port;
	__ret = ____actvcall_netcall_netlink_unicast(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(protocol, __actv_local_var_protocol), __ARCH_ACTV_RPC_PASS_ARG(src_portid, __actv_local_var_src_portid), __ARCH_ACTV_RPC_PASS_ARG(dst_port, __actv_local_var_dst_port));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_netlink_unicast(rref_t __rref, unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned int dst_port)
{
	int __ret;
	int __maybe_unused __actv_local_var_protocol = protocol;
	unsigned int __maybe_unused __actv_local_var_src_portid = src_portid;
	unsigned int __maybe_unused __actv_local_var_dst_port = dst_port;
	__ret = ____actvcapcall_netcall_netlink_unicast(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(protocol, __actv_local_var_protocol), __ARCH_ACTV_RPC_PASS_ARG(src_portid, __actv_local_var_src_portid), __ARCH_ACTV_RPC_PASS_ARG(dst_port, __actv_local_var_dst_port));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_netcall_netlink_broadcast(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_netcall_netlink_broadcast(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_netlink_broadcast(unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned int dst_group)
{
	int __ret;
	int __maybe_unused __actv_local_var_protocol = protocol;
	unsigned int __maybe_unused __actv_local_var_src_portid = src_portid;
	unsigned int __maybe_unused __actv_local_var_dst_group = dst_group;
	__ret = ____actvcall_netcall_netlink_broadcast(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(protocol, __actv_local_var_protocol), __ARCH_ACTV_RPC_PASS_ARG(src_portid, __actv_local_var_src_portid), __ARCH_ACTV_RPC_PASS_ARG(dst_group, __actv_local_var_dst_group));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_netlink_broadcast(rref_t __rref, unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned int dst_group)
{
	int __ret;
	int __maybe_unused __actv_local_var_protocol = protocol;
	unsigned int __maybe_unused __actv_local_var_src_portid = src_portid;
	unsigned int __maybe_unused __actv_local_var_dst_group = dst_group;
	__ret = ____actvcapcall_netcall_netlink_broadcast(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(protocol, __actv_local_var_protocol), __ARCH_ACTV_RPC_PASS_ARG(src_portid, __actv_local_var_src_portid), __ARCH_ACTV_RPC_PASS_ARG(dst_group, __actv_local_var_dst_group));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_netlink_create_service(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_netlink_create_service(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_netlink_create_service(unsigned long long __rpc_info, void *__buf, int protocol)
{
	int __ret;
	int __maybe_unused __actv_local_var_protocol = protocol;
	__ret = ____actvcall_netcall_netlink_create_service(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(protocol, __actv_local_var_protocol));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_netlink_create_service(rref_t __rref, unsigned long long __rpc_info, void *__buf, int protocol)
{
	int __ret;
	int __maybe_unused __actv_local_var_protocol = protocol;
	__ret = ____actvcapcall_netcall_netlink_create_service(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(protocol, __actv_local_var_protocol));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_netlink_release_service(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_netlink_release_service(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_netlink_release_service(unsigned long long __rpc_info, void *__buf, int protocol)
{
	int __ret;
	int __maybe_unused __actv_local_var_protocol = protocol;
	__ret = ____actvcall_netcall_netlink_release_service(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(protocol, __actv_local_var_protocol));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_netlink_release_service(rref_t __rref, unsigned long long __rpc_info, void *__buf, int protocol)
{
	int __ret;
	int __maybe_unused __actv_local_var_protocol = protocol;
	__ret = ____actvcapcall_netcall_netlink_release_service(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(protocol, __actv_local_var_protocol));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_netcall_netlink_unicast_nowait(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_netcall_netlink_unicast_nowait(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_netlink_unicast_nowait(unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned int dst_port)
{
	int __ret;
	int __maybe_unused __actv_local_var_protocol = protocol;
	unsigned int __maybe_unused __actv_local_var_src_portid = src_portid;
	unsigned int __maybe_unused __actv_local_var_dst_port = dst_port;
	__ret = ____actvcall_netcall_netlink_unicast_nowait(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(protocol, __actv_local_var_protocol), __ARCH_ACTV_RPC_PASS_ARG(src_portid, __actv_local_var_src_portid), __ARCH_ACTV_RPC_PASS_ARG(dst_port, __actv_local_var_dst_port));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_netlink_unicast_nowait(rref_t __rref, unsigned long long __rpc_info, void *__buf, int protocol, unsigned int src_portid, unsigned int dst_port)
{
	int __ret;
	int __maybe_unused __actv_local_var_protocol = protocol;
	unsigned int __maybe_unused __actv_local_var_src_portid = src_portid;
	unsigned int __maybe_unused __actv_local_var_dst_port = dst_port;
	__ret = ____actvcapcall_netcall_netlink_unicast_nowait(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(protocol, __actv_local_var_protocol), __ARCH_ACTV_RPC_PASS_ARG(src_portid, __actv_local_var_src_portid), __ARCH_ACTV_RPC_PASS_ARG(dst_port, __actv_local_var_dst_port));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_vsock_register_vm(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_vsock_register_vm(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_vsock_register_vm(unsigned long long __rpc_info, void *__buf, unsigned int cid, unsigned int dev_group_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cid = cid;
	unsigned int __maybe_unused __actv_local_var_dev_group_id = dev_group_id;
	__ret = ____actvcall_netcall_vsock_register_vm(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cid, __actv_local_var_cid), __ARCH_ACTV_RPC_PASS_ARG(dev_group_id, __actv_local_var_dev_group_id));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_vsock_register_vm(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cid, unsigned int dev_group_id)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cid = cid;
	unsigned int __maybe_unused __actv_local_var_dev_group_id = dev_group_id;
	__ret = ____actvcapcall_netcall_vsock_register_vm(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cid, __actv_local_var_cid), __ARCH_ACTV_RPC_PASS_ARG(dev_group_id, __actv_local_var_dev_group_id));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_vsock_vm_notify(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_vsock_vm_notify(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_vsock_vm_notify(unsigned long long __rpc_info, void *__buf, unsigned int cid, unsigned short cmd, void *args, size_t size)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cid = cid;
	unsigned short __maybe_unused __actv_local_var_cmd = cmd;
	__maybe_unused void *__actv_local_var_args = args;
	size_t __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcall_netcall_vsock_vm_notify(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cid, __actv_local_var_cid), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_vsock_vm_notify(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cid, unsigned short cmd, void *args, size_t size)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cid = cid;
	unsigned short __maybe_unused __actv_local_var_cmd = cmd;
	__maybe_unused void *__actv_local_var_args = args;
	size_t __maybe_unused __actv_local_var_size = size;
	__ret = ____actvcapcall_netcall_vsock_vm_notify(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cid, __actv_local_var_cid), __ARCH_ACTV_RPC_PASS_ARG(cmd, __actv_local_var_cmd), __ARCH_ACTV_RPC_PASS_ARG(args, __actv_local_var_args), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_vsock_unregister_vm(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_vsock_unregister_vm(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_vsock_unregister_vm(unsigned long long __rpc_info, void *__buf, unsigned int cid)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cid = cid;
	__ret = ____actvcall_netcall_vsock_unregister_vm(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cid, __actv_local_var_cid));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_vsock_unregister_vm(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cid)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cid = cid;
	__ret = ____actvcapcall_netcall_vsock_unregister_vm(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cid, __actv_local_var_cid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_ldk_domain_register(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_ldk_domain_register(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_ldk_domain_register(unsigned long long __rpc_info, void *__buf, int family, unsigned long long dh_ap_uref)
{
	int __ret;
	int __maybe_unused __actv_local_var_family = family;
	unsigned long long __maybe_unused __actv_local_var_dh_ap_uref = dh_ap_uref;
	__ret = ____actvcall_netcall_ldk_domain_register(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(family, __actv_local_var_family), __ARCH_ACTV_RPC_PASS_ARG(dh_ap_uref, __actv_local_var_dh_ap_uref));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_ldk_domain_register(rref_t __rref, unsigned long long __rpc_info, void *__buf, int family, unsigned long long dh_ap_uref)
{
	int __ret;
	int __maybe_unused __actv_local_var_family = family;
	unsigned long long __maybe_unused __actv_local_var_dh_ap_uref = dh_ap_uref;
	__ret = ____actvcapcall_netcall_ldk_domain_register(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(family, __actv_local_var_family), __ARCH_ACTV_RPC_PASS_ARG(dh_ap_uref, __actv_local_var_dh_ap_uref));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_procfs_create_ldk_entry(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_procfs_create_ldk_entry(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_procfs_create_ldk_entry(unsigned long long __rpc_info, void *__buf, unsigned int nsid, const char *path, void *ops, void *ctx, unsigned int mode)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__maybe_unused const char *__actv_local_var_path = path;
	__maybe_unused void *__actv_local_var_ops = ops;
	__maybe_unused void *__actv_local_var_ctx = ctx;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	__ret = ____actvcall_netcall_procfs_create_ldk_entry(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid), __ARCH_ACTV_RPC_PASS_ARG(path, __actv_local_var_path), __ARCH_ACTV_RPC_PASS_ARG(ops, __actv_local_var_ops), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_procfs_create_ldk_entry(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int nsid, const char *path, void *ops, void *ctx, unsigned int mode)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__maybe_unused const char *__actv_local_var_path = path;
	__maybe_unused void *__actv_local_var_ops = ops;
	__maybe_unused void *__actv_local_var_ctx = ctx;
	unsigned int __maybe_unused __actv_local_var_mode = mode;
	__ret = ____actvcapcall_netcall_procfs_create_ldk_entry(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid), __ARCH_ACTV_RPC_PASS_ARG(path, __actv_local_var_path), __ARCH_ACTV_RPC_PASS_ARG(ops, __actv_local_var_ops), __ARCH_ACTV_RPC_PASS_ARG(ctx, __actv_local_var_ctx), __ARCH_ACTV_RPC_PASS_ARG(mode, __actv_local_var_mode));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_procfs_delete_ldk_entry_specific(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_procfs_delete_ldk_entry_specific(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_procfs_delete_ldk_entry_specific(unsigned long long __rpc_info, void *__buf, const char *pathname, unsigned int nsid)
{
	int __ret;
	__maybe_unused const char *__actv_local_var_pathname = pathname;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcall_netcall_procfs_delete_ldk_entry_specific(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(pathname, __actv_local_var_pathname), __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_procfs_delete_ldk_entry_specific(rref_t __rref, unsigned long long __rpc_info, void *__buf, const char *pathname, unsigned int nsid)
{
	int __ret;
	__maybe_unused const char *__actv_local_var_pathname = pathname;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcapcall_netcall_procfs_delete_ldk_entry_specific(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(pathname, __actv_local_var_pathname), __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_procfs_create_ldk_entries(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_procfs_create_ldk_entries(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_procfs_create_ldk_entries(unsigned long long __rpc_info, void *__buf, void *parent, unsigned int size, void *ops)
{
	int __ret;
	__maybe_unused void *__actv_local_var_parent = parent;
	unsigned int __maybe_unused __actv_local_var_size = size;
	__maybe_unused void *__actv_local_var_ops = ops;
	__ret = ____actvcall_netcall_procfs_create_ldk_entries(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(parent, __actv_local_var_parent), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size), __ARCH_ACTV_RPC_PASS_ARG(ops, __actv_local_var_ops));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_procfs_create_ldk_entries(rref_t __rref, unsigned long long __rpc_info, void *__buf, void *parent, unsigned int size, void *ops)
{
	int __ret;
	__maybe_unused void *__actv_local_var_parent = parent;
	unsigned int __maybe_unused __actv_local_var_size = size;
	__maybe_unused void *__actv_local_var_ops = ops;
	__ret = ____actvcapcall_netcall_procfs_create_ldk_entries(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(parent, __actv_local_var_parent), __ARCH_ACTV_RPC_PASS_ARG(size, __actv_local_var_size), __ARCH_ACTV_RPC_PASS_ARG(ops, __actv_local_var_ops));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_buf"))) __internal_rpccall ____actvcall_netcall_sec_mgmt(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_buf"))) __internal_rpccapcall ____actvcapcall_netcall_sec_mgmt(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_sec_mgmt(unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_module = module;
	unsigned int __maybe_unused __actv_local_var_command = command;
	__ret = ____actvcall_netcall_sec_mgmt(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(module, __actv_local_var_module), __ARCH_ACTV_RPC_PASS_ARG(command, __actv_local_var_command));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_sec_mgmt(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int module, unsigned int command)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_module = module;
	unsigned int __maybe_unused __actv_local_var_command = command;
	__ret = ____actvcapcall_netcall_sec_mgmt(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(module, __actv_local_var_module), __ARCH_ACTV_RPC_PASS_ARG(command, __actv_local_var_command));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_namespace_new_ns(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_namespace_new_ns(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_namespace_new_ns(unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, unsigned int nsid)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcall_netcall_namespace_new_ns(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx), __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_namespace_new_ns(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int cnode_idx, unsigned int nsid)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_cnode_idx = cnode_idx;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcapcall_netcall_namespace_new_ns(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(cnode_idx, __actv_local_var_cnode_idx), __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

static int __attribute__((alias("__internal_rpccall_nobuf"))) __internal_rpccall ____actvcall_netcall_namespace_destroy_ns(unsigned long long rpc_info, void *__buf, ...);


static int __attribute__((alias("__internal_rpccapcall_nobuf"))) __internal_rpccapcall ____actvcapcall_netcall_namespace_destroy_ns(rref_t __rref, unsigned long long rpc_info, void *__buf, ...);


int __internal_actvcall __actvcall_netcall_namespace_destroy_ns(unsigned long long __rpc_info, void *__buf, unsigned int nsid)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcall_netcall_namespace_destroy_ns(__rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

int __internal_actvcall __actvcapcall_netcall_namespace_destroy_ns(rref_t __rref, unsigned long long __rpc_info, void *__buf, unsigned int nsid)
{
	int __ret;
	unsigned int __maybe_unused __actv_local_var_nsid = nsid;
	__ret = ____actvcapcall_netcall_namespace_destroy_ns(__rref, __rpc_info, __buf, __ARCH_ACTV_RPC_PASS_ARG(nsid, __actv_local_var_nsid));
	return __ret;
}

#endif
#endif
