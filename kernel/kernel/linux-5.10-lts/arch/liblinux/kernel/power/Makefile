# SPDX-License-Identifier: GPL-2.0-only
obj-y += suspend.o
obj-$(CONFIG_LIBLINUX_HIBERNATE) += hibernate.o
obj-$(CONFIG_SUSPEND_FAIL_NOTIFIER) += suspend_fail_notifier.o
obj-$(CONFIG_SUSPEND_FAIL_ERROR_INJECT) += suspend_fail_error_inject.o
CFLAGS_suspend_fail_error_inject.o := -I$(srctree)/lib

obj-$(CONFIG_LIBLINUX_HAVE_ULSR) += ulsr.o
CFLAGS_ulsr.o += -I $(srctree)/platform_source/cee/drivers/pm \
		 -I $(srctree)/drivers/platform_drivers/ap/platform/common/lowpm
