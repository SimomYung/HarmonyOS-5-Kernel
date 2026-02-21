# SPDX-License-Identifier: GPL-2.0-only
obj-y += misc.o devno.o file_utils.o unimpl.o

obj-$(CONFIG_BLOCK) += block.o
obj-$(CONFIG_BPF_SYSCALL) += namei.o
CFLAGS_block.o += -I$(srctree)/block/partitions

obj-y += devpts/

obj-y += fs-writeback.o
