obj-$(CONFIG_HIPCIE_IDLE) += hi_pcie_idle.o
obj-$(CONFIG_HIPCIE_OCT) += hi_pcie_oct.o
obj-$(CONFIG_HIPCIE_OCT) += hi_pcie_oct_common.o
obj-$(CONFIG_HIPCIE_NSV) += hi_pcie_nsv.o
obj-$(CONFIG_HIPCIE_XN) += hi_pcie_xn.o
obj-$(CONFIG_HIPCIE_CHS) += hi_pcie_chs.o

ifeq ($(CONFIG_HIPCIE_NSV), y)
EXTRA_CFLAGS += -I$(srctree)/drivers/hwspinlock
endif

ifeq ($(CONFIG_HIPCIE_IDLE), y)
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/common/lowpm/
endif
