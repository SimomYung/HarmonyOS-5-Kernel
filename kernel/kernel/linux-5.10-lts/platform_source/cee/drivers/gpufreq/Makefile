LP_CHIP_TYPE := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_SUFFIX)))
LP_SOC_CHIP_NAME := $(strip $(subst ", ,$(CONFIG_VENDOR_CHIP_PREFIX)))

ccflags-y += -Wextra -Wdate-time -Wfloat-equal -Wshadow -Wformat=2
obj-$(CONFIG_GPUTOP_FREQ) += gpu_top.o
obj-$(CONFIG_GPU_FHSS) += gpu_fhss.o
obj-$(CONFIG_GPUFREQ_INDEPDENT) += gpufreq.o
obj-$(CONFIG_GPUFREQ_USER_VOTE) += gpufreq_fs.o
obj-$(CONFIG_GPUFLINK) += link/

ccflags-y  += -I$(srctree)/drivers/platform_drivers/ap/platform/common \
	-I$(srctree)/include/platform_include/basicplatform/linux

ifeq ($(LP_CHIP_TYPE), cs2)
ccflags-y += -I$(srctree)/drivers/platform_drivers/ap/platform/$(LP_SOC_CHIP_NAME)_cs2
else ifeq ($(LP_CHIP_TYPE), es)
ccflags-y += -I$(srctree)/drivers/platform_drivers/ap/platform/$(LP_SOC_CHIP_NAME)_es
else
ccflags-y += -I$(srctree)/drivers/platform_drivers/ap/platform/$(LP_SOC_CHIP_NAME)
endif

ccflags-$(CONFIG_GPUFREQ) +=
