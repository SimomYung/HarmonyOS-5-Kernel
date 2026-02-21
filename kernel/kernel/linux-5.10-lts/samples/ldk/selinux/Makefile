# SPDX-License-Identifier: GPL-2.0

obj-y += security.o

obj-y += selinuxfs.o
CFLAGS_selinuxfs.o += -I$(srctree)/security/selinux/include/ -I$(objtree)/security/selinux/

obj-y += net.o
CFLAGS_net.o += -I$(srctree)/security/selinux/include/ -I$(objtree)/security/selinux/
CFLAGS_net.o += ${LINUXINCLUDE} ${KBUILD_CPPFLAGS}

obj-$(CONFIG_BPF_SYSCALL) += bpf.o
CFLAGS_bpf.o += -I$(srctree)/security/selinux/include/ -I$(objtree)/security/selinux/
CFLAGS_bpf.o += ${KBUILD_CPPFLAGS}

USER_OBJS := security.o
include arch/liblinux/scripts/Makefile.rules
