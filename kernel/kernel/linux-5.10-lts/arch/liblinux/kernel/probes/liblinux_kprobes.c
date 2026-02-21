// SPDX-License-Identifier: GPL-2.0-only
#include <linux/types.h>
#include <asm/ptrace.h>
#include <linux/kprobes.h>
#include <linux/spinlock.h>
#include <linux/set_memory.h>
#include <liblinux/unimpl.h>

#include <trace/hooks/liblinux.h>

extern int kprobe_breakpoint_ss_handler(struct pt_regs *regs, unsigned int esr);
extern int kprobe_breakpoint_handler(struct pt_regs *regs, unsigned int esr);
EXPORT_SYMBOL_GPL(kprobe_breakpoint_handler);
EXPORT_SYMBOL_GPL(kprobe_breakpoint_ss_handler);

/* arch/xxx/kernel/probes/kprobes.c */
LIBLINUX_UNIMPL_RET(arch_populate_kprobe_blacklist, int, 0)

struct running_kprobe {
	pid_t pid;
	struct kprobe *kp;
};

struct running_kprobe_ctlblk {
	pid_t pid;
	struct kprobe_ctlblk blk;
};

DEFINE_SPINLOCK(current_kprobe_lock);
struct running_kprobe current_kprobes[CONFIG_NR_CPUS];

DEFINE_SPINLOCK(current_ctlblk_lock);
struct running_kprobe_ctlblk current_ctlblks[CONFIG_NR_CPUS];
static void clear_current_kprobe_ctlblk(void);

int liblinux_set_current_kprobe(struct kprobe *p)
{
	int free_idx = -1;
	pid_t pid = current->pid;
	spin_lock(&current_kprobe_lock);
	for (int i = 0; i < CONFIG_NR_CPUS; i++) {
		if (current_kprobes[i].pid == pid) {
			pr_err("current kprobe already exist. pid:%d, addr:%p\n", pid, current_kprobes[i].kp->addr);
			free_idx = i;
			break;
		} else if (free_idx < 0 && current_kprobes[i].pid == 0) {
			free_idx = i;
		}
	}
	if (free_idx >= 0) {
		current_kprobes[free_idx].pid = pid;
		current_kprobes[free_idx].kp = p;
	}
	spin_unlock(&current_kprobe_lock);

	if (free_idx < 0) {
		pr_err("Oops. no free slot for current kprobe.\n");
		return -1;
	}

	return 0;
}

void liblinux_reset_current_kprobe(void)
{
	spin_lock(&current_kprobe_lock);
	pid_t pid = current->pid;
	for (int i = 0; i < CONFIG_NR_CPUS; i++) {
		if (current_kprobes[i].pid == pid) {
			current_kprobes[i].pid = 0;
			current_kprobes[i].kp = NULL;
			break;
		}
	}
	spin_unlock(&current_kprobe_lock);
	clear_current_kprobe_ctlblk();
}

struct kprobe *liblinux_kprobe_running(void)
{
	struct kprobe *kp = NULL;
	pid_t pid = current->pid;
	spin_lock(&current_kprobe_lock);
	for (int i = 0; i < CONFIG_NR_CPUS; i++) {
		if (current_kprobes[i].pid == pid) {
			kp = current_kprobes[i].kp;
			break;
		}
	}
	spin_unlock(&current_kprobe_lock);

	return kp;
}

struct kprobe_ctlblk *liblinux_get_kprobe_ctlblk(void)
{
	struct kprobe_ctlblk *blk = NULL;
	pid_t pid = current->pid;
	int free_idx = -1;
	spin_lock(&current_ctlblk_lock);
	for (int i = 0; i < CONFIG_NR_CPUS; i++) {
		if (current_ctlblks[i].pid == pid) {
			free_idx = i;
			break;
		} else if(free_idx < 0 && current_ctlblks[i].pid == 0) {
			free_idx = i;
		}

	}
	if (free_idx >= 0) {
		blk = &(current_ctlblks[free_idx].blk);
		current_ctlblks[free_idx].pid = pid;
	}
	spin_unlock(&current_ctlblk_lock);
	if (free_idx < 0) {
		pr_err("Oops. no free slot for current kprobe ctlblk.\n");
	}

	return blk;
}

static void clear_current_kprobe_ctlblk(void)
{
	spin_lock(&current_ctlblk_lock);
	pid_t pid = current->pid;
	for (int i = 0; i < CONFIG_NR_CPUS; i++) {
		if (current_ctlblks[i].pid == pid) {
			current_ctlblks[i].pid = 0;
			break;
		}
	}
	spin_unlock(&current_ctlblk_lock);
}

static
void hook_patch_map(void *data, void *addr, int *ret)
{
	unsigned long addr_align = (uintptr_t)addr;

	addr_align = ALIGN_DOWN(addr_align, PAGE_SIZE);

	*ret = set_memory_rw(addr_align, 1);
	if (unlikely(*ret))
		pr_err("failed set memory rw ret=%d\n", *ret);
}
INIT_VENDOR_HOOK(ldk_rvh_patch_map, hook_patch_map);

static
void hook_patch_unmap(void *data, void *addr, int *ret)
{
	unsigned long addr_align = (uintptr_t)addr;

	addr_align = ALIGN_DOWN(addr_align, PAGE_SIZE);

	*ret = set_memory_rx(addr_align, 1);
	if (unlikely(*ret))
		pr_err("failed set memory rx ret=%d\n", *ret);
}
INIT_VENDOR_HOOK(ldk_rvh_patch_unmap, hook_patch_unmap);
