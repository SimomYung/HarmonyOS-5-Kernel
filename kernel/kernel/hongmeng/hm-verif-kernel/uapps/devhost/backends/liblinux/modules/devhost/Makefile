obj-m += devhost.o

devhost-y := core.o builtin.o dma.o \
	     vtimer.o virqchip.o kvic.o pmu.o \
	     module.o devfs.o poll.o vitschip_platform_msi.o
devhost-$(CONFIG_BLOCK) += block.o
devhost-$(CONFIG_NET) += net.o net_stack.o

ifeq ($(findstring 5.10, $(KERNELVERSION)), 5.10)
devhost-$(CONFIG_POSIX_TIMERS) += posix_clock.o
else
devhost-y += posix_clock.o
endif

ifeq (${CONFIG_DEVHOST_PM_PLUGIN}, y)
devhost-y += power/power.o
devhost-y += power/pmqos.o
devhost-$(CONFIG_PM_SLEEP) += power/wakeup.o
endif

devhost-${CONFIG_KEYS} += key.o

ifneq (${CONFIG_OF_EARLY_FLATTREE},y)
$(error CONFIG_OF_EARLY_FLATTREE is not defined)
endif

ifeq (${CONFIG_MDC_GODEL_HACK}, y)
devhost-$(CONFIG_PCI_MSI_IRQ_DOMAIN) += vitschip_pci_msi.o
endif

ifeq (${CONFIG_DEVHOST_NETLINK}, y)
devhost-$(CONFIG_NET) += netlink.o
endif
devhost-y += kevtd.o

ccflags-y += ${DEVHOST_API_CFLAGS}
ccflags-y += -nostdlib

ifeq ($(findstring 5.10, $(KERNELVERSION)), 5.10)
ldk5-out := posix_clock.o
devhost-y := $(filter-out $(ldk5-out),$(devhost-y))
endif
