// SPDX-License-Identifier: GPL-2.0-only
#include <linux/bpf.h>
#include <liblinux/pal.h>

#include <trace/hooks/liblinux.h>

static void hook_bpf_cmd_check(void *args, int cmd, int *ret)
{
	switch (cmd) {
	case BPF_MAP_FREEZE:
	case BPF_PROG_TEST_RUN:
	case BPF_MAP_GET_NEXT_ID:
	case BPF_BTF_GET_NEXT_ID:
	case BPF_MAP_GET_FD_BY_ID:
	case BPF_RAW_TRACEPOINT_OPEN:
	case BPF_BTF_LOAD:
	case BPF_BTF_GET_FD_BY_ID:
	case BPF_TASK_FD_QUERY:
	case BPF_MAP_LOOKUP_AND_DELETE_ELEM:
	case BPF_MAP_LOOKUP_BATCH:
	case BPF_MAP_LOOKUP_AND_DELETE_BATCH:
	case BPF_MAP_UPDATE_BATCH:
	case BPF_MAP_DELETE_BATCH:
	case BPF_LINK_CREATE:
	case BPF_LINK_UPDATE:
	case BPF_LINK_GET_FD_BY_ID:
	case BPF_LINK_GET_NEXT_ID:
	case BPF_ENABLE_STATS:
	case BPF_ITER_CREATE:
	case BPF_LINK_DETACH:
	case BPF_PROG_BIND_MAP:
		*ret = -EINVAL;
		break;
	default:
		*ret = 0;
		break;
	}
}
INIT_VENDOR_HOOK(ldk_vh_bpf_cmd_check, hook_bpf_cmd_check);

static void hook_bpf_charge_memlock(void *args, u32 pages, int *ret)
{
    *ret = liblinux_pal_charge_mlock(pages << PAGE_SHIFT);
    if (*ret)
        *ret = -EPERM;
}
INIT_VENDOR_HOOK(ldk_vh_bpf_charge_memlock, hook_bpf_charge_memlock);

static void hook_bpf_uncharge_memlock(void *args, u32 pages)
{
    (void)liblinux_pal_uncharge_mlock(pages << PAGE_SHIFT);
}
INIT_VENDOR_HOOK(ldk_vh_bpf_uncharge_memlock, hook_bpf_uncharge_memlock);
