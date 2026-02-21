# SPDX-License-Identifier: GPL-2.0-only

obj-y := setup.o

extra-y := liblinux.lds

obj-y += panic.o misc.o printk.o module.o cpu.o dumpstack.o
obj-y += cpufeature.o neon.o
obj-y += smp.o bootparam.o irq.o
obj-y += liblinux_device.o
obj-y += workqueue.o
CFLAGS_workqueue.o += -I$(srctree)/kernel/

obj-y += locking/
obj-y += power/

obj-$(CONFIG_OF_EARLY_FLATTREE) += of.o
CFLAGS_of.o += -I$(srctree)/drivers/of

obj-y += sched/ exit.o
obj-y += kthread.o
obj-y += softirq.o

obj-$(CONFIG_TRACING) += trace.o

obj-y += unimpl.o
obj-y += rtunimpl.o

obj-$(CONFIG_ARM64) += smccc-call-arm64.o
obj-$(CONFIG_ARM64) += smccc-hvc-call.o

obj-$(CONFIG_CGROUPS) += cgroup.o
obj-$(CONFIG_CGROUP_BPF) += bpf-cgroup.o
obj-$(CONFIG_BPF_SYSCALL) += bpf.o

obj-y += extable.o

obj-y += capability.o
obj-y += reboot.o
obj-y += probes/
obj-y += signal.o pid.o

obj-$(CONFIG_LIBLINUX_UNIMPL_DFX_COUNT) += unimpl_dfx.o
obj-$(CONFIG_NO_HZ_IDLE) += nohz.o
