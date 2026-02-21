# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_KPROBES)		+= liblinux_kprobes.o
obj-$(CONFIG_KPROBES)		+= kprobes_trampoline.o
obj-$(CONFIG_KPROBES)		+= kprobes.o
CFLAGS_kprobes.o += -I$(srctree)/arch/arm64/kernel/probes/
