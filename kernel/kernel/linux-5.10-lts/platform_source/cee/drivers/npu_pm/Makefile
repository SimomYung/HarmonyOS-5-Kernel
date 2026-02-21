LP_CHIP_TYPE := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_SUFFIX)))
LP_SOC_CHIP_NAME := $(strip $(subst ", ,$(CONFIG_VENDOR_CHIP_PREFIX)))

ccflags-y += -Wextra -Wdate-time -Wfloat-equal -Wshadow -Wformat=2

obj-$(CONFIG_NPUFREQ)      += npu_devfreq.o
obj-$(CONFIG_THERMAL_EX_SUSTAIN_POWER) += npu_freq_cooling.o
ifeq ($(CONFIG_NPU_PM_SMC), y)
obj-$(CONFIG_NPU_PM)       += smc/npu_pm_smc.o
obj-$(CONFIG_NPU_PM)       += smc/npu_dvfs_smc.o
obj-$(CONFIG_NPU_PM_DEBUG) += smc/npu_pm_debug_smc.o
else ifeq ($(CONFIG_NPU_PM_PMC), y)
obj-$(CONFIG_NPU_PM)       += pmc/npu_pm_pmc.o
obj-$(CONFIG_NPU_PM)       += pmc/npu_dvfs_pmc.o
obj-$(CONFIG_NPU_PM_DEBUG) += pmc/npu_pm_debug_pmc.o
else
obj-$(CONFIG_NPU_PM)       += npu_pm.o
obj-$(CONFIG_NPU_PM)      += npu_dvfs.o
obj-$(CONFIG_NPU_PM_DEBUG) += npu_pm_debug.o
obj-$(CONFIG_NPU_PM_AVS_VDM)  += npu_avs.o
endif

ccflags-y  += -I$(srctree)/drivers/platform_drivers/ap/platform/common \
	-I$(srctree)/include/platform_include/basicplatform/linux

ifeq ($(LP_CHIP_TYPE), cs2)
ccflags-y += -I$(srctree)/drivers/platform_drivers/ap/platform/$(LP_SOC_CHIP_NAME)_cs2
else ifeq ($(LP_CHIP_TYPE), es)
ccflags-y += -I$(srctree)/drivers/platform_drivers/ap/platform/$(LP_SOC_CHIP_NAME)_es
else
ccflags-y += -I$(srctree)/drivers/platform_drivers/ap/platform/$(LP_SOC_CHIP_NAME)
endif

ccflags-$(CONFIG_NPU_PM)  +=
