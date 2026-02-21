# SPDX-License-Identifier: GPL-2.0

obj-y += security.o

obj-y += selinuxfs.o
CFLAGS_selinuxfs.o += -I$(objtree)/security/selinux/ -I$(srctree)/security/selinux/ss/ -I$(srctree)/security/selinux/include/

obj-y += net.o
CFLAGS_net.o += -I$(srctree)/security/selinux/include/ -I$(objtree)/security/selinux/
CFLAGS_net.o += ${LINUXINCLUDE} ${KBUILD_CPPFLAGS}

obj-$(CONFIG_BPF_SYSCALL) += bpf.o
CFLAGS_bpf.o += -I$(srctree)/security/selinux/include/ -I$(objtree)/security/selinux/
CFLAGS_bpf.o += ${KBUILD_CPPFLAGS}

CFLAGS_security.o += ${CFLAGS_PAC}
obj-y += dmaheap.o
CFLAGS_dmaheap.o += -I$(srctree)/security/selinux/include/ -I$(objtree)/security/selinux/
CFLAGS_dmaheap.o += ${LINUXINCLUDE} ${KBUILD_CPPFLAGS}

USER_OBJS := security.o
include arch/liblinux/scripts/Makefile.rules
