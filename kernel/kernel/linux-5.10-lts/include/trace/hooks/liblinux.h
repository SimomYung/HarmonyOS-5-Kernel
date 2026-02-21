/* SPDX-License-Identifier: GPL-2.0 */
/*
 * liblinux Vendor Hooks
 *
 * Copyright (c) 2022, Huawei Tech. Co., Ltd.
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM liblinux

#define TRACE_INCLUDE_PATH trace/hooks
#if !defined(_TRACE_HOOK_LIBLINUX_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_HOOK_LIBLINUX_H
/* Define struct should be ahead of vendor_hooks.h, where there may undef TRACE_HEADER_MULTI_READ */
#ifndef TRACE_HEADER_MULTI_READ
enum liblinux_dfx_mem_event {
	LIBLINUX_DFX_MEM_MIN,
	LIBLINUX_DFX_SLAB_ALLOC,
	LIBLINUX_DFX_SLAB_FREE,
	LIBLINUX_DFX_PAGE_ALLOC,
	LIBLINUX_DFX_PAGE_FREE,
	LIBLINUX_DFX_VMALLOC_ALLOC,
	LIBLINUX_DFX_VMALLOC_FREE,
	LIBLINUX_DFX_MEM_MAX,
};
#endif

#include <trace/hooks/vendor_hooks.h>

#define BATCH_FLAG 0x4000
#define ROOT_NET_NSID 8189
#define HOOK_ALWAYS_ON 1
#define HOOK_ALWAYS_OFF 0

#ifdef CONFIG_LIBLINUX
#define DECLARE_LDK_HOOK DECLARE_HOOK
#define DECLARE_LDK_RESTRICTED_HOOK DECLARE_RESTRICTED_HOOK
#else
#define DECLARE_LDK_HOOK(name, proto, args) \
	static inline void trace_##name(proto) {} \
	static inline bool trace_##name##_enabled(void) { return false; }

#define DECLARE_LDK_RESTRICTED_HOOK(name, proto, arg, cond) \
	DECLARE_LDK_HOOK(name, PARAMS(proto), PARAMS(args))
#endif

/*
 * kernel_init done hook
 */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_kernel_init_done,
	     TP_PROTO(enum system_states system_state),
	     TP_ARGS(system_state), HOOK_ALWAYS_ON);

/*
 * ldk_rvh_dfx_call_mem used in:
 * mm/{slab.h, slub.c, slab_common.c}
 * mm/pal/page_alloc.c
 */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_dfx_call_mem,
	     TP_PROTO(enum liblinux_dfx_mem_event ev_enum, void *addr,
		      unsigned long size, const char *slab_cache_name),
	     TP_ARGS(ev_enum, addr, size, slab_cache_name), HOOK_ALWAYS_ON);

#ifdef CONFIG_CMA
struct cma;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_cma_add_section,
	TP_PROTO(unsigned long pfn, unsigned long count),
	TP_ARGS(pfn, count), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_cma_alloc,
	TP_PROTO(unsigned long *bitmap_no, const struct cma *cma, size_t count,
		 unsigned int align_order, int *ret),
	TP_ARGS(bitmap_no, cma, count, align_order, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_cma_release,
	TP_PROTO(const struct cma *cma, const struct page *pages, unsigned int count),
	TP_ARGS(cma, pages, count), HOOK_ALWAYS_ON);
#endif /* CONFIG_CMA */

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_zone_pageset_init_late,
	TP_PROTO(int unuse),
	TP_ARGS(unuse), HOOK_ALWAYS_ON);

/* drivers/base/core.c */
#define LIBLINUX_VH_DEVICE_ADD	1U
#define LIBLINUX_VH_DEVICE_DEL	2U
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_device_notify,
	TP_PROTO(struct device *dev, unsigned int event, int *ret),
	TP_ARGS(dev, event, ret), HOOK_ALWAYS_ON);

/* kernel/task_work.c */
struct task_struct;
enum task_work_notify_mode;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_task_work_check,
	     TP_PROTO(struct task_struct *task, enum task_work_notify_mode notify),
	     TP_ARGS(task, notify), HOOK_ALWAYS_ON);

/* kernel/workqueue.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_set_vip_prio,
	TP_PROTO(struct task_struct *p, int prio),
	TP_ARGS(p, prio), HOOK_ALWAYS_ON);

struct worker_pool;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_unbind_workers,
			TP_PROTO(struct worker_pool *pool, int cpu, struct mutex *lock, struct list_head *workers),
			TP_ARGS(pool, cpu, lock, workers), HOOK_ALWAYS_ON);

struct worker;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_get_kworker,
	TP_PROTO(struct worker **worker, int node, int prep_flag),
	TP_ARGS(worker, node, prep_flag), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_kworker_exit,
	TP_PROTO(struct worker *worker, int *cached, struct worker_pool **pool, void (set_pf_hndlr)(bool)),
	TP_ARGS(worker, cached, pool, set_pf_hndlr), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_free_kworker,
	TP_PROTO(struct worker *worker), TP_ARGS(worker), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_wake_cached_kworker,
	TP_PROTO(struct worker *worker), TP_ARGS(worker), HOOK_ALWAYS_ON);

/* fs/{char,block}_dev.c */
#define VH_REG_REGION	0
#define VH_UNREG_REGION	1
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_chrdev_region,
	     TP_PROTO(int action, unsigned int *major, unsigned int baseminor,
		      int minorct, const char *name, int *ret),
	     TP_ARGS(action, major, baseminor, minorct, name, ret), HOOK_ALWAYS_ON);
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_blkdev_region,
	     TP_PROTO(int action, unsigned int *major,
		      const char *name, int *ret),
	     TP_ARGS(action, major, name, ret), HOOK_ALWAYS_ON);

/* fs/configfs */
struct path;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_gdentry,
	     TP_PROTO(const char *pathname, struct path *path, int *ret),
	     TP_ARGS(pathname, path, ret), HOOK_ALWAYS_ON);
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_pdentry,
	     TP_PROTO(void *sb, void *dentry),
	     TP_ARGS(sb, dentry), HOOK_ALWAYS_ON);

/* fs/procfs/generic.c */
#define VH_REG_PROCFS_PREPARE 0
#define VH_REG_PROCFS 1
struct proc_dir_entry;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_proc_register,
	     TP_PROTO(int action, struct proc_dir_entry *dp),
	     TP_ARGS(action, dp), HOOK_ALWAYS_ON);
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_remove_proc_entry,
	     TP_PROTO(struct proc_dir_entry *de),
	     TP_ARGS(de), HOOK_ALWAYS_ON);
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_remove_proc_subtree,
	     TP_PROTO(struct proc_dir_entry *de),
	     TP_ARGS(de), HOOK_ALWAYS_ON);

struct file_system_type;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_proc_root_init,
	     TP_PROTO(struct file_system_type *type),
	     TP_ARGS(type), HOOK_ALWAYS_ON);

struct net;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_devinet_sysctl_unregister,
	     TP_PROTO(struct net *net, char *dev_name),
	     TP_ARGS(net, dev_name), HOOK_ALWAYS_ON);
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_addrconf_sysctl_unregister,
	     TP_PROTO(struct net *net, char *dev_name),
	     TP_ARGS(net, dev_name), HOOK_ALWAYS_ON);
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_remove_proc_entry_with_ns,
	     TP_PROTO(struct net *net, char *pre_path, const char *dev_name),
	     TP_ARGS(net, pre_path, dev_name), HOOK_ALWAYS_ON);

#ifdef CONFIG_NET_NS
#define QUOTA_LOCK	1
#define QUOTA_UNLOCK	0
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_proc_quota,
			    TP_PROTO(int quota_mutex),
			    TP_ARGS(quota_mutex), HOOK_ALWAYS_ON);
#endif

/* fs/proc/proc_sysctl.c */
struct ctl_table;
struct ctl_table_header;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_register_sysctl_table,
			TP_PROTO(const char *dir, struct ctl_table_header *head, struct ctl_table *table),
			TP_ARGS(dir, head, table), HOOK_ALWAYS_ON);

/* fs/kernfs */
struct kernfs_node;
struct kobject;
struct device;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_kernfs_add_one,
			TP_PROTO(struct kernfs_node *kn,
			struct kernfs_node *(*next)(struct kernfs_node *pos, struct kernfs_node *root),
			void (*activate)(struct kernfs_node *kn),
			void (*remove)(struct kernfs_node *), int *ret),
			TP_ARGS(kn, next, activate, remove, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_kernfs_put,
			TP_PROTO(struct kernfs_node *kn, int *ret),
			TP_ARGS(kn, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_kernfs_notify,
			TP_PROTO(struct kernfs_node *kn),
			TP_ARGS(kn), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_kernfs_rename_ns,
			TP_PROTO(struct kernfs_node *kn, struct kernfs_node *new_parent,
				 const char *new_name, const void *new_ns, int *ret),
			TP_ARGS(kn, new_parent, new_name, new_ns, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_setup_net,
			TP_PROTO(void *ns, int *ret),
			TP_ARGS(ns, ret), HOOK_ALWAYS_ON);
struct net_device;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_post_register_netdev,
			TP_PROTO(struct net_device *dev, int *ret),
			TP_ARGS(dev, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_pre_register_netdev,
			TP_PROTO(struct net_device *dev),
			TP_ARGS(dev), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_kobject_add_uevent,
			TP_PROTO(struct device *dev, struct kobject *kobj),
			TP_ARGS(dev, kobj), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_pre_init_module,
			TP_PROTO(char *driver),
			TP_ARGS(driver), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_post_init_module,
			TP_PROTO(char *driver),
			TP_ARGS(driver), HOOK_ALWAYS_ON);

/* fs/sysfs */
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_sysfs_init,
			TP_PROTO(struct kernfs_root *root, int *ret),
			TP_ARGS(root, ret), HOOK_ALWAYS_ON);

/* kernel/power */
struct pm_qos_constraints;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_pm_qos_notify,
			TP_PROTO(struct pm_qos_constraints *c, int value),
			TP_ARGS(c, value), HOOK_ALWAYS_ON);
/* uapps/tppmgr/dal/ldk/charge_notifier.c */
struct power_supply;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_power_supply_charge_notify,
			    TP_PROTO(struct power_supply *psy),
			    TP_ARGS(psy), HOOK_ALWAYS_ON);

/* net/network.c */
struct pernet_operations;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_proc_net_init,
			TP_PROTO(struct pernet_operations *ops),
			TP_ARGS(ops), HOOK_ALWAYS_ON);

#ifdef CONFIG_NET_NS
struct liblinux_procfs_create_ops;
struct liblinux_pal_kernfs_ops;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_liblinux_proc_create_net_data,
			TP_PROTO(struct proc_dir_entry *head_dp, struct liblinux_procfs_create_ops **create_ops,
			struct liblinux_pal_kernfs_ops **ops, bool *need_create),
			TP_ARGS(head_dp, create_ops, ops, need_create), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_register_net_sysctl_table,
			TP_PROTO(struct ctl_table_header *head, struct liblinux_procfs_create_ops **create_ops,
			struct liblinux_pal_kernfs_ops **ops, bool *need_create),
			TP_ARGS(head, create_ops, ops, need_create), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_set_net_nsid,
			TP_PROTO(struct net *net, unsigned int nsid),
			TP_ARGS(net, nsid), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_proc_net_register,
			TP_PROTO(struct net *net, struct proc_dir_entry *netd, int *err),
			TP_ARGS(net, netd, err), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_proc_net_unregister,
			TP_PROTO(struct net *net),
			TP_ARGS(net), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_devhost_net_socket_get_ns_by_fd,
			TP_PROTO(int fd, struct net **net),
			TP_ARGS(fd, net), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_devhost_net_socket_get_ns_by_pid,
			TP_PROTO(int pid, struct net **net),
			TP_ARGS(pid, net), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_devhost_net_socket_get_netns,
			TP_PROTO(struct net **net),
			TP_ARGS(net), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_devhost_net_socket_put_netns,
			TP_PROTO(struct net *net),
			TP_ARGS(net), HOOK_ALWAYS_ON);
#endif

/* net/core net/ipv4 net/ipv6 net/packet */
struct sock;
struct socket;
struct page_frag;
struct sk_buff;
struct flowi4;
struct tcphdr;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_sk_page_frag,
	     TP_PROTO(struct sock *sk, struct page_frag **pfrag),
	     TP_ARGS(sk, pfrag), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_sk_vm_insert_page,
	     TP_PROTO(struct vm_area_struct *vma, unsigned long addr, unsigned long kaddr, struct page *page, int *ret),
	     TP_ARGS(vma, addr, kaddr, page, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_tcp_init_transfer,
	     TP_PROTO(struct sock *sk, int op, struct sk_buff *skb),
	     TP_ARGS(sk, op, skb), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_udp_send_skb,
	     TP_PROTO(struct sk_buff *skb, struct flowi4 *fl4, int *ret),
	     TP_ARGS(skb, fl4, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_inet_stream_connect,
	     TP_PROTO(struct socket *sock, int flags),
	     TP_ARGS(sock, flags), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_inet_listen,
	     TP_PROTO(struct socket *sock),
	     TP_ARGS(sock), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_inet_accept,
	     TP_PROTO(struct socket *sock),
	     TP_ARGS(sock), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_tcp_sendmsg,
	     TP_PROTO(struct sock *sk),
	     TP_ARGS(sk), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_tcp_recvmsg,
	     TP_PROTO(struct sock *sk),
	     TP_ARGS(sk), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_udp_sendmsg,
	     TP_PROTO(struct sock *sk),
	     TP_ARGS(sk), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_udp_recvmsg,
	     TP_PROTO(struct sock *sk),
	     TP_ARGS(sk), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_retransmit_fail,
	     TP_PROTO(struct sock *sk, struct sk_buff *skb, char *extra_info, int err),
	     TP_ARGS(sk, skb, extra_info, err), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_tcp_connect_rcv,
	     TP_PROTO(struct sock *sk, const struct tcphdr *th),
	     TP_ARGS(sk, th), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_skb_drop_reason,
	     TP_PROTO(const struct sock *sk, struct sk_buff *skb, int reason),
	     TP_ARGS(sk, skb, reason), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_route_drop_reason,
	     TP_PROTO(const struct sock *sk, void *flow, int err),
	     TP_ARGS(sk, flow, err), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_bpf_drop_reason,
            TP_PROTO(void *ctx, int reason),
            TP_ARGS(ctx, reason), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_netfilter_drop_reason,
	     TP_PROTO(const struct sock *sk, struct sk_buff *skb, void *hookfn),
	     TP_ARGS(sk, skb, hookfn), HOOK_ALWAYS_ON);

struct netdev_queue;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_netdev_queue_debug,
	     TP_PROTO(void *ctx, struct netdev_queue *q, int reason),
	     TP_ARGS(ctx, q, reason), HOOK_ALWAYS_ON);

/* sk_event */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_sk_event_set,
	    TP_PROTO(struct sock *sk, int val, int *ret),
	    TP_ARGS(sk, val, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_sk_event_free,
	    TP_PROTO(struct sock *sk),
	    TP_ARGS(sk), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_tcp_event_update,
	    TP_PROTO(struct sock *sk, struct sk_buff *skb, int flag, int val),
	    TP_ARGS(sk, skb, flag, val), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_sk_event_read,
	    TP_PROTO(struct sock *sk, char __user *optval,
		     int __user *optlen, int *ret),
	    TP_ARGS(sk, optval, optlen, ret), HOOK_ALWAYS_ON);

struct net;
struct compat_ifreq;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_compat_ifreq_ioctl,
	     TP_PROTO(struct net *net, struct socket *sock, unsigned int cmd,
			struct compat_ifreq __user *uifr32, int *ret),
	     TP_ARGS(net, sock, cmd, uifr32, ret), HOOK_ALWAYS_ON);

struct xt_table_info;
struct nf_hook_state;
struct ipt_entry;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_trace_iptable_drop,
	     TP_PROTO(const struct nf_hook_state *state, const char *tablename,
		      const struct xt_table_info *private,
		      const struct ipt_entry *e, struct sk_buff *skb),
	     TP_ARGS(state, tablename, private, e, skb), HOOK_ALWAYS_ON);

struct ip6t_entry;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_trace_iptable6_drop,
	     TP_PROTO(const struct nf_hook_state *state, const char *tablename,
		      const struct xt_table_info *private,
		      const struct ip6t_entry *e, struct sk_buff *skb),
	     TP_ARGS(state, tablename, private, e, skb), HOOK_ALWAYS_ON);

#ifdef CONFIG_NETLINK_ROUTE_NLMSG_READPRIV
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_sysctl_core_net_init,
	     TP_PROTO(struct net *net, struct ctl_table *tbl, struct user_namespace *init_user_ns),
	     TP_ARGS(net, tbl, init_user_ns), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_net_defaults_init_net,
	     TP_PROTO(struct net *net),
	     TP_ARGS(net), HOOK_ALWAYS_ON);
#endif

/* net/tls */
#include <linux/sockptr.h>
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_do_tls_setsockopt,
	     TP_PROTO(struct sock *sk, int optname, sockptr_t optval,
		      unsigned int optlen, int *ret),
	     TP_ARGS(sk, optname, optval, optlen, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_tls_sw_cache_init,
	     TP_PROTO(int unuse),
	     TP_ARGS(unuse), HOOK_ALWAYS_ON);

struct tls_sw_context_rx;
struct strp_msg;
struct tls_msg;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_tls_skb_alloc,
	     TP_PROTO(struct tls_sw_context_rx *ctx, struct sk_buff **skb,
		      struct strp_msg **rxm, int *n_sgin),
	     TP_ARGS(ctx, skb, rxm, n_sgin), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_tls_consume_skb,
	     TP_PROTO(struct tls_sw_context_rx *ctx, struct sk_buff *skb, bool drop),
	     TP_ARGS(ctx, skb, drop), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_tls_ctx_update,
	     TP_PROTO(struct tls_sw_context_rx *ctx, struct sk_buff **skb,
		      struct strp_msg **rxm, struct tls_msg **tlm),
	     TP_ARGS(ctx, skb, rxm, tlm), HOOK_ALWAYS_ON);

/* drivers/base/power/wakeup.c */
struct wakeup_source;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_ws_debugfs_init,
			TP_PROTO(struct wakeup_source *ws),
			TP_ARGS(ws), HOOK_ALWAYS_ON);

/* drivers/base/power/wakeup.c */
typedef void (*split_counters_t)(unsigned int *cnt, unsigned int *inpr);
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_wakeup_source_activate,
			TP_PROTO(bool is_activate, split_counters_t split_counters_f),
			TP_ARGS(is_activate, split_counters_f),
			TP_CONDITION(HOOK_ALWAYS_ON)
);

/* platform_source/cee/drivers/devfreq/mem_lat/devfreq_devbw.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_devfreq_add_devbw,
			TP_PROTO(const char **name),
			TP_ARGS(name), HOOK_ALWAYS_ON);

/* platform_source/cee/drivers/mips_mem.c */
struct platform_device;
struct mipsmem_cluster_policy;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_mips_mem_info,
			TP_PROTO(struct platform_device *pdev,
			struct mipsmem_cluster_policy *cluster_policy),
			TP_ARGS(pdev, cluster_policy),
			HOOK_ALWAYS_ON);

/* drivers/base/firmware_loader/main.c */
struct fw_priv;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_fw_get_filesystem_firmware,
			TP_PROTO(struct device *device, struct fw_priv *fw_priv,
			const char *suffix,
			int (*decompress)(struct device *dev,
					  struct fw_priv *fw_priv,
					  size_t in_size,
					  const void *in_buffer),
			const char * const *fw_path,
			size_t fw_path_size, int *ret),
			TP_ARGS(device, fw_priv, suffix, decompress, fw_path, fw_path_size, ret),
			HOOK_ALWAYS_ON);

typedef void (*free_fw_priv_func_t)(struct fw_priv *fw_priv);
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_request_firmware,
			TP_PROTO(struct fw_priv *fw_priv,
				 free_fw_priv_func_t free_fw_priv_func),
			TP_ARGS(fw_priv, free_fw_priv_func),
			HOOK_ALWAYS_ON);

/* arch/liblinux/fs/file_utils.c */
struct file;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_filp_open_put,
			TP_PROTO(struct file *file),
			TP_ARGS(file),
			HOOK_ALWAYS_ON);

/* arch/liblinux/mm/page_alloc.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_free_pages,
			TP_PROTO(void *page, unsigned int order, int *ret),
			TP_ARGS(page, order, ret),
			TP_CONDITION(HOOK_ALWAYS_ON));

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_hisp_alloc_largest_pages,
			TP_PROTO(unsigned int order, gfp_t *gfp_mask),
			TP_ARGS(order, gfp_mask),
			TP_CONDITION(HOOK_ALWAYS_ON));

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_alloc_pages_time_stat,
			TP_PROTO(unsigned int order, gfp_t gfp_mask,
				 s64 used_us, struct page *page),
			TP_ARGS(order, gfp_mask, used_us, page),
			TP_CONDITION(HOOK_ALWAYS_ON));

/* drivers/scsi/sd.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_sd_ida_alloc,
			TP_PROTO(int *sd_index),
			TP_ARGS(sd_index), HOOK_ALWAYS_ON);
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_sd_ida_free,
			TP_PROTO(int sd_index),
			TP_ARGS(sd_index), HOOK_ALWAYS_ON);

/* drivers/usb/gadget/function/f_fs.c */
struct sg_table;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_ffs_build_sg_list,
			TP_PROTO(struct sg_table *sgt, size_t sz, void **raddr),
			TP_ARGS(sgt, sz, raddr), HOOK_ALWAYS_ON);
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_ffs_free_buffer,
			TP_PROTO(void *vaddr, struct sg_table *sgt),
			TP_ARGS(vaddr, sgt), HOOK_ALWAYS_ON);

/* platform_source/basicplatform/drivers/rpmb/rpmb_ufs.c &
 * platform_source/basicplatform/block/blk_scsi_kern.c */
struct bsg_class_device;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_get_rpmb_bcd,
                        TP_PROTO(const char *devname, struct bsg_class_device *bcd),
                        TP_ARGS(devname, bcd), HOOK_ALWAYS_ON);

struct ufs_blk_ioc_rpmb_data;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_ufs_bsg_ioctl_rpmb,
			TP_PROTO(struct ufs_blk_ioc_rpmb_data *rdata, bool is_write,
				 int id, int source, int *ret),
			TP_ARGS(rdata, is_write, id, source, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_sel_load_policy,
			TP_PROTO(const char *buffer, unsigned long size, ssize_t *ret),
			TP_ARGS(buffer, size, ret), HOOK_ALWAYS_ON);

#ifdef CONFIG_HM_SEPOL_POLICYDB
struct policydb;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_sel_policydb_init,
			TP_PROTO(struct policydb *p, int *ret),
			TP_ARGS(p, ret), HOOK_ALWAYS_ON);

struct policy_file;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_sel_role_trans_read,
			TP_PROTO(struct policydb *p, struct policy_file *fp, int *ret),
			TP_ARGS(p, fp, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_sel_role_trans_write,
			TP_PROTO(struct policydb *p, void *fp, int *ret),
			TP_ARGS(p, fp, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_sel_role_trans_destroy,
			TP_PROTO(struct policydb *p),
			TP_ARGS(p), HOOK_ALWAYS_ON);

struct context;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_sel_role_trans_search,
			TP_PROTO(struct policydb *p, struct context *newcon, uint32_t srole, uint32_t ttype, uint32_t tclass),
			TP_ARGS(p, newcon, srole, ttype, tclass), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_sel_policydb_write,
			TP_PROTO(struct policydb *p, struct policy_file *fp, void **data, size_t *len, int *ret),
			TP_ARGS(p, fp, data, len, ret), HOOK_ALWAYS_ON);
#endif /* CONFIG_HM_SEPOL_POLICYDB */

#ifdef CONFIG_SECURITY_SELINUX_DEVELOP
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_sel_update_enforce,
			TP_PROTO(int value, ssize_t *ret),
			TP_ARGS(value, ret), HOOK_ALWAYS_ON);

#endif /* CONFIG_SECURITY_SELINUX_DEVELOP */
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_sel_update_cache_threshold,
			TP_PROTO(int value, ssize_t *ret),
			TP_ARGS(value, ret), HOOK_ALWAYS_ON);

struct timekeeper;
struct timespec64;
/* timekeeping_update function pointer */
typedef void (*tk_update_func_t)(struct timekeeper *tk, unsigned int actions);
/* tk_set_wall_to_mono function pointer */
typedef void (*tk_set_wtm_func_t)(struct timekeeper *tk, struct timespec64 wtm);
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_tk_init,
			TP_PROTO(struct timekeeper *tk, void *seq,
				 void *tk_lock, tk_update_func_t tk_update,
				 tk_set_wtm_func_t tk_set_wtm),
			TP_ARGS(tk, seq, tk_lock, tk_update, tk_set_wtm), HOOK_ALWAYS_ON);

/* drivers/dma-buf/heaps/page_pool.c */
struct dmabuf_page_pool;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_dmabuf_page_pool_do_shrink,
			TP_PROTO(struct dmabuf_page_pool *pool, bool high, int nr_to_scan, int *freed_out),
			TP_ARGS(pool, high, nr_to_scan, freed_out), HOOK_ALWAYS_ON);
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_dmabuf_page_pool_free_pages,
			TP_PROTO(struct dmabuf_page_pool *pool, struct page *page),
			TP_ARGS(pool, page), HOOK_ALWAYS_ON);
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_track_dmabuf_free_pages_mod,
			TP_PROTO(struct dmabuf_page_pool *pool, int index, int cnt),
			TP_ARGS(pool, index, cnt), HOOK_ALWAYS_ON);

/* mm/vmstat.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_buddy_dma_free_pages_set,
			TP_PROTO(int item),
			TP_ARGS(item), HOOK_ALWAYS_ON);

/* drivers/connectivity/hi11xx/hi1105/wifi/hmac/hmac_auto_adjust_freq.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_hmac_calc_throughput_aspm,
			TP_PROTO(unsigned int rx_throughput_mbps),
			TP_ARGS(rx_throughput_mbps), HOOK_ALWAYS_ON);

/* arch/liblinux/kernel/kthread.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_init_task,
			TP_PROTO(struct task_struct *tsk),
			TP_ARGS(tsk), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_exit_task,
			TP_PROTO(struct task_struct *tsk),
			TP_ARGS(tsk), HOOK_ALWAYS_ON);

struct kthread_worker;
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_get_cached_worker,
			TP_PROTO(struct kthread_worker **worker, int cpu, unsigned int flags, char *name),
			TP_ARGS(worker, cpu, flags, name), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_destroy_worker,
			TP_PROTO(struct kthread_worker *worker),
			TP_ARGS(worker), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_free_kthread_worker,
			TP_PROTO(struct kthread_worker *worker),
			TP_ARGS(worker), HOOK_ALWAYS_ON);

/* arch/liblinux/kernel/fs/namei.c */
struct filename;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_user_path_create,
			TP_PROTO(const char __user *pathname, struct path *path,
				 unsigned int lookup_flags, struct dentry **dentry),
			TP_ARGS(pathname, path, lookup_flags, dentry), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_user_path_at,
			TP_PROTO(const char __user *pathname, struct path *path, int *ret),
			TP_ARGS(pathname, path, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_touch_atime,
			TP_PROTO(struct path *path),
			TP_ARGS(path), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_done_path_create,
			TP_PROTO(struct path *path, struct dentry *dentry),
			TP_ARGS(path, dentry), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_inode_permission,
			TP_PROTO(const char *pathname, bool user, int mask, int *ret),
			TP_ARGS(pathname, user, mask, ret), HOOK_ALWAYS_ON);

/* arch/liblinux/kernel/bpf.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_bpf_cmd_check,
			TP_PROTO(int cmd, int *ret),
			TP_ARGS(cmd, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_charge_memlock,
			TP_PROTO(u32 pages, int errno, int *ret),
			TP_ARGS(pages, errno, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_uncharge_memlock,
			TP_PROTO(u32 pages),
			TP_ARGS(pages), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_cgroup_bpf_enabled_key_dec,
			TP_PROTO(unsigned int type),
			TP_ARGS(type), HOOK_ALWAYS_ON);

struct bpf_prog;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_cgroup_bpf_enabled_key_inc,
			TP_PROTO(struct bpf_prog *old_prog, unsigned int type),
			TP_ARGS(old_prog, type), HOOK_ALWAYS_ON);

/* arch/liblinux/lib/uaccess.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_copy_to_user,
			TP_PROTO(void __user *to, const void *from,
				unsigned long n, int *ret),
			TP_ARGS(to, from, n, ret), HOOK_ALWAYS_ON);

/* arch/liblinux/net/net_comm.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_devhost_current_mm_set,
			TP_PROTO(struct task_struct *cur),
			TP_ARGS(cur), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_devhost_current_mm_clear,
			TP_PROTO(struct task_struct *cur),
			TP_ARGS(cur), HOOK_ALWAYS_ON);

/* samples/ldk/cred.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_syscall_exit,
			TP_PROTO(struct task_struct *tsk),
			TP_ARGS(tsk), HOOK_ALWAYS_ON);

/* drivers/tty/pty.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_devpts_get_priv,
			    TP_PROTO(int index, void **ret),
			    TP_ARGS(index, ret), HOOK_ALWAYS_ON);

#ifdef CONFIG_LIBLINUX_TTY_FLUSH_SYNC
struct tty_port;
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_tty_port_cfg_sync_flush,
			    TP_PROTO(struct tty_port *port),
			    TP_ARGS(port), HOOK_ALWAYS_ON);
#endif

/* net/xdp/xdp_umem.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_xdp_pin_user_pages,
	     TP_PROTO(unsigned long start, unsigned long nr_pages, unsigned int gup_flags,
                        struct page **pages, struct vm_area_struct **vmas, long *ret),
	     TP_ARGS(start, nr_pages, gup_flags, pages, vmas, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_xdp_unpin_user_pages,
             TP_PROTO(struct page **pages, unsigned long npages, bool make_dirty),
             TP_ARGS(pages, npages, make_dirty), HOOK_ALWAYS_ON);

/* net/xdp/alg_umem.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_alg_copy_to_user_pages,
			TP_PROTO(void *msg, void *areq, size_t len, int *err),
			TP_ARGS(msg, areq, len, err), HOOK_ALWAYS_ON);

/* arch/arm64/kernel/insn.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_patch_map,
			    TP_PROTO(void *addr, int *ret),
			    TP_ARGS(addr, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_patch_unmap,
			    TP_PROTO(void *addr, int *ret),
			    TP_ARGS(addr, ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_initcall_blacklist,
			    TP_PROTO(initcall_t fn, int *ret),
			    TP_ARGS(fn, ret), HOOK_ALWAYS_ON);

/* platform_source/basicplatform/block/mas_blk_hm.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_sd_proxy_block_event_cmd,
			    TP_PROTO(int event_id),
			    TP_ARGS(event_id), HOOK_ALWAYS_ON);

/* arch/liblinux/mm/cache.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_vh_dev_set_user_inval_cache,
			TP_PROTO(struct device *dev),
			TP_ARGS(dev), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_kthread_create_on_node,
                        TP_PROTO(struct task_struct *p),
                        TP_ARGS(p), HOOK_ALWAYS_ON);

/* kernel/softirq.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_tick_irq_exit,
			TP_PROTO(int cpu), TP_ARGS(cpu), HOOK_ALWAYS_ON);
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_wakeup_softirqd,
			TP_PROTO(int unused), TP_ARGS(unused), HOOK_ALWAYS_ON);

/* kernel/rcu/tree.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_rcu_restart_tick,
			TP_PROTO(int wake_cpu), TP_ARGS(wake_cpu), HOOK_ALWAYS_ON);
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_rcu_notify_qs,
			TP_PROTO(int cpu, bool need_qs), TP_ARGS(cpu, need_qs), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_thread_set_freezable,
			TP_PROTO(int *ret), TP_ARGS(ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_thread_freezing,
			TP_PROTO(bool *ret), TP_ARGS(ret), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_thread_try_to_freeze,
			TP_PROTO(int *ret), TP_ARGS(ret), HOOK_ALWAYS_ON);

/* kernel/irq/manage.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_set_affinity_hint,
			TP_PROTO(struct irq_desc *desc, const struct cpumask *m, int *ret),
			TP_ARGS(desc, m, ret), HOOK_ALWAYS_ON);
/* vendor/huawei/extra/kernel/drivers/power/hw_kstate/hw_procnetstat.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_get_thread_name,
			TP_PROTO(pid_t tid, char *comm, int len),
			TP_ARGS(tid, comm, len), HOOK_ALWAYS_ON);

/* drivers/base/devtmpfs.c */
DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_devtmpfs_setup,
			    TP_PROTO(struct task_struct **task, int *err),
			    TP_ARGS(task, err), HOOK_ALWAYS_ON);

DECLARE_LDK_RESTRICTED_HOOK(ldk_rvh_smp_call_cpu_dump,
            TP_PROTO(int unuse),
            TP_ARGS(unuse), HOOK_ALWAYS_ON);
/* NOTE: ALL LDK hooks must be places above this line */
#endif /* !defined(_TRACE_HOOK_LIBLINUX_H) || defined(TRACE_HEADER_MULTI_READ) */

/* This part must be outside protection */
#include <trace/define_trace.h>
