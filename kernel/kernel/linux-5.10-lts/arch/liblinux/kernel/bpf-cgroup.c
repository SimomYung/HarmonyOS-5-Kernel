// SPDX-License-Identifier: GPL-2.0-only
#include <linux/bpf-cgroup.h>

#include <trace/hooks/liblinux.h>

DEFINE_STATIC_KEY_ARRAY_FALSE(liblinux_cgroup_bpf_enabled_key, MAX_BPF_ATTACH_TYPE);
EXPORT_SYMBOL(liblinux_cgroup_bpf_enabled_key);

int __override __cgroup_bpf_check_dev_permission(short dev_type, u32 major, u32 minor,
		short access, enum bpf_attach_type type)
{
	return 0;
}

int __override __cgroup_bpf_run_filter_sysctl(struct ctl_table_header *head,
		struct ctl_table *table, int write,
		char **buf, size_t *pcount, loff_t *ppos,
		enum bpf_attach_type type)
{
	return 0;
}

static void hook_cgroup_bpf_enabled_key_dec(void *args, unsigned int type)
{
	static_branch_dec(&liblinux_cgroup_bpf_enabled_key[type]);
}
INIT_VENDOR_HOOK(ldk_vh_cgroup_bpf_enabled_key_dec, hook_cgroup_bpf_enabled_key_dec);

static void hook_cgroup_bpf_enabled_key_inc(void *args, struct bpf_prog *old_prog, unsigned int type)
{
	if (!old_prog)
		static_branch_inc(&liblinux_cgroup_bpf_enabled_key[type]);
}
INIT_VENDOR_HOOK(ldk_vh_cgroup_bpf_enabled_key_inc, hook_cgroup_bpf_enabled_key_inc);
