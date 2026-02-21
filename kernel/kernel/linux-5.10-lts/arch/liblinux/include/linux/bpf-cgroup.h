/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBLINUX_BPF_CGROUP_H__
#define __LIBLINUX_BPF_CGROUP_H__

#include_next <linux/bpf-cgroup.h>

#ifdef CONFIG_CGROUPS

extern struct static_key_false liblinux_cgroup_bpf_enabled_key[MAX_BPF_ATTACH_TYPE];
#define liblinux_cgroup_bpf_enabled(type) static_branch_unlikely(&liblinux_cgroup_bpf_enabled_key[type])

#include_next <linux/bpf-cgroup.h>

#undef BPF_CGROUP_RUN_PROG_UDP4_SENDMSG_LOCK
#undef BPF_CGROUP_RUN_PROG_UDP4_RECVMSG_LOCK

#define __BPF_CGROUP_RUN_SA_PROG_LOCK(sk, uaddr, type, t_ctx)		       \
({									       \
	int __ret = 0;							       \
	if (liblinux_cgroup_bpf_enabled(type))	{					       \
		lock_sock(sk);						       \
		__ret = __cgroup_bpf_run_filter_sock_addr(sk, uaddr, type,     \
							  t_ctx);	       \
		release_sock(sk);					       \
	}								       \
	__ret;								       \
})

#define BPF_CGROUP_RUN_PROG_UDP4_SENDMSG_LOCK(sk, uaddr, t_ctx)		       \
	__BPF_CGROUP_RUN_SA_PROG_LOCK(sk, uaddr, BPF_CGROUP_UDP4_SENDMSG, t_ctx)

#define BPF_CGROUP_RUN_PROG_UDP4_RECVMSG_LOCK(sk, uaddr)			\
	__BPF_CGROUP_RUN_SA_PROG_LOCK(sk, uaddr, BPF_CGROUP_UDP4_RECVMSG, NULL)

#endif

#endif /* __LIBLINUX_BPF_CGROUP_H__ */
