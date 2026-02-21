# SPDX-License-Identifier: GPL-2.0

obj-y += reboot.o
obj-$(CONFIG_PM_SLEEP) += wakelock.o
obj-y += export_symbols.o
obj-y += cpu.o

ifeq ($(CONFIG_LIBLINUX_MIPSMEM_FREQ),y)
	obj-$(CONFIG_DEVFREQ_DEVBW) += mips_mem.o mipsmem_hmdal.o
else
	obj-$(CONFIG_DEVFREQ_DEVBW) += devfreq_devbw.o governor_hmdal.o
endif

CFLAGS_governor_hmdal.o += -I $(srctree)/platform_source/cee/drivers/devfreq/ \
			   -I $(srctree)/drivers/devfreq/

ifneq ($(CONFIG_LDK_THERMAL),y)
	obj-$(CONFIG_IPA_THERMAL) += tz_adapter.o
endif

obj-$(CONFIG_LIBLINUX_HIBERNATE) += hibernate.o

obj-$(CONFIG_POWER_DUBAI) += dubai_proxy.o dubai_proxy_cpustat.o
CFLAGS_dubai_proxy.o += -I $(srctree)/drivers/dubai/

obj-y += dfc.o
obj-$(CONFIG_LPCPU_CPUFREQ_DT) += lpu_cpufreq.o

USER_OBJS := dubai_proxy_cpustat.o dfc.o hibernate.o
include arch/liblinux/scripts/Makefile.rules
