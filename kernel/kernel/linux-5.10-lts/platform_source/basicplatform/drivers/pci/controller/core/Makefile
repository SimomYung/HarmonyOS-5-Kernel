#
# Makefile for kernel vendor pcie.
#

obj-y = hi_pcie_core.o hi_pcie_common.o hi_pcie_msi.o hi_pcie_dsm.o

ifeq (${CONFIG_PCIE_HCD_LDK}, y)
EXTRA_CFLAGS += -I$(srctree)/samples/ldk/vendor
endif
obj-$(CONFIG_PCIE_HCD_LDK) += hi_pcie_udk_comm.o

