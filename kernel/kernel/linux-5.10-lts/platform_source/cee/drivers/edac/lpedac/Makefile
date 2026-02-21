SOC_PLATFORM := $(strip $(subst ", ,$(CONFIG_VENDOR_CHIP_PREFIX)))
SOC_CHIP_TYPE := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_SUFFIX)))

ifneq ($(SOC_CHIP_TYPE),)
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/$(SOC_PLATFORM)
else
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/$(SOC_PLATFORM)_$(SOC_CHIP_TYPE)
endif

EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/common

obj-$(CONFIG_CPU_EDAC_ARM64) += cpu_edac_arm64.o

ccflags-$(CONFIG_CPU_EDAC_ARM64)  += -I$(srctree)/drivers/edac

obj-$(CONFIG_RAS_EDAC) += ras_common.o
obj-$(CONFIG_RAS_EDAC) += ras_device.o
obj-$(CONFIG_RAS_EDAC_UNCORRECTED_ERROR) += ras_ecc.o
obj-$(CONFIG_RAS_EDAC_DEBUG) += ras_err_inj.o
obj-$(CONFIG_IMPROVE_YIELD) += cpu_improve_yield.o
obj-$(CONFIG_DFX_SH) += cpu_sh.o
ccflags-$(CONFIG_RAS_EDAC)  += -I$(srctree)/drivers/edac
ccflags-y += -Wextra -Wdate-time -Wfloat-equal -Wshadow
