# SPDX-License-Identifier: GPL-2.0

# libdh-linux sources
obj-y += libdh_linux.o libdh_extralib.o external_symbols.o
obj-y += liblinux_symbols.o page.o

obj-y += default_affinity.o

obj-$(CONFIG_LIBLINUX_BATCH_UACCESS) += batch_uaccess.o

obj-y += misc.o task_work.o
obj-y += iofast.o
CFLAGS_misc.o += ${LINUXINCLUDE} ${KBUILD_CPPFLAGS}
CFLAGS_iofast.o += ${LINUXINCLUDE} ${KBUILD_CPPFLAGS}
CFLAGS_iofast.o += -I$(srctree)/mm

obj-y += time.o
obj-y += time_sync.o

obj-$(CONFIG_PROC_FS) += procfs.o
obj-$(CONFIG_SYSFS) += kernfs.o sysfs_batch.o
obj-$(CONFIG_LIBLINUX_ASYNC_SYSFS) += async_sysfs.o
obj-$(CONFIG_CONFIGFS_FS) += configfs.o
obj-$(CONFIG_PROC_SYSCTL) += sysctl.o
obj-$(CONFIG_DFX_MEMCHECK_STACK) += memcheck.o

obj-y += file.o

obj-y += mem_info.o
CFLAGS_mem_info.o += -I$(srctree)/drivers/hwmemcheck/

obj-y += drivers/
obj-$(CONFIG_PM) += power/
obj-$(CONFIG_SECURITY_SELINUX) += selinux/
obj-$(CONFIG_NET_NS) += netns/ procfs_net.o netlog.o
obj-$(CONFIG_LIBLINUX_VB2_VMALLOC_WORKAROUND)	+= videobuf2/
obj-y += tests/

obj-y += vendor/
obj-$(CONFIG_LIBLINUX_HMTRACEFS) += hmtracefs/
obj-$(CONFIG_LIBLINUX_DEBUGFS) += debugfs/
obj-$(CONFIG_LIBLINUX_CONFIGFS_EXT) += configfs_ext/
obj-y += dfx/

ccflags-y += -DCONFIG_LIBLINUX=1
ccflags-y += -DCONFIG_DEVHOST_FS_TRANSFS=1
ccflags-$(CONFIG_NO_HZ_IDLE) += -DLNXBASE_HELPER_ENABLE_STOP_TICK=1
ccflags-$(CONFIG_NET) += -DCONFIG_DEVHOST_NETLINK=1
ccflags-$(CONFIG_STACKTRACE) += -DLNXBASE_HELPER_ENABLE_BACKTRACE=1
ccflags-$(CONFIG_CC_OPTIMIZE_FOR_PERFORMANCE) += -O2
ccflags-$(CONFIG_LIBLINUX_SKIP_TIMESYNC) += -DCONFIG_DEVHOST_SKIP_TIMESYNC=1

CFLAGS_libdh_linux.o += -I$(srctree)/arch/liblinux/include/
CFLAGS_libdh_linux.o += -DLNXBASE_HELPER_WITH_MODULE_NS -DLNXBASE_HELPER_ENABLE_TIMESYNC \
		        $(if $(CONFIG_NET),,-DLNXBASE_HELPER_SKIP_NETLINK)

CFLAGS_external_symbols.o += -include $(objtree)/include/generated/autoconf.h \
			     -include $(srctree)/include/linux/export.h
CFLAGS_external_symbols.o += -D__override='__attribute__((__section__(".text.override")))'

CFLAGS_liblinux_symbols.o += -I $(srctree)/drivers/of/
CFLAGS_memcheck.o += $(if $(CONFIG_DFX_MEMCHECK_STACK),-DCONFIG_DFX_MEMCHECK_STACK,)

obj-$(CONFIG_DFX_TIME) += common_printk.o
CFLAGS_common_printk.o += -I$(srctree)/platform_source/basicplatform/drivers/ \
			  -I$(srctree)/kernel/printk/

obj-$(CONFIG_HUAWEI_KSTATE) += binderfs.o
CFLAGS_binderfs.o += -include $(objtree)/include/generated/autoconf.h \
			   -include $(srctree)/include/linux/export.h \
			   -include $(srctree)/include/huawei_platform/power/hw_kcollect.h

obj-y += sysevent.o
CFLAGS_sysevent.o += -include $(objtree)/include/generated/autoconf.h

obj-$(CONFIG_LIBLINUX) += fscrypto_ctrl.o

obj-$(CONFIG_LIBLINUX_MODULE_BINDER) += binder.o

obj-y += ldk_stat.o
CFLAGS_ldk_stat.o += ${LINUXINCLUDE} ${KBUILD_CPPFLAGS}

# mark all non-linux objects as USER_OBJS to remove kernel CFLAGS
USER_OBJS := libdh_linux.o libdh_extralib.o external_symbols.o misc.o binderfs.o iofast.o \
	     time.o memcheck.o sysevent.o fscrypto_ctrl.o ldk_stat.o
include arch/liblinux/scripts/Makefile.rules
