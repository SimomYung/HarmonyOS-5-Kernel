# Hisilicon IPP v500 Makefile
obj-$(CONFIG_HISPIPP_V500) += modules/ segment/ drivers/ dataflow/
obj-$(CONFIG_HISPIPP_V500) += memory.o

SOC_PLATFORM := $(strip $(subst ", ,$(CONFIG_VENDOR_CHIP_PREFIX)))
SOC_CHIP_TYPE := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_SUFFIX)))

ifneq ($(SOC_CHIP_TYPE),)
subdir-ccflags-$(CONFIG_HISPIPP_V500) += -I$(srctree)/drivers/platform_drivers/ap/platform/common/$(SOC_PLATFORM)
else
subdir-ccflags-$(CONFIG_HISPIPP_V500) += -I$(srctree)/drivers/platform_drivers/ap/platform/common/$(SOC_PLATFORM)_$(SOC_CHIP_TYPE)
endif

ifeq ($(SOC_CHIP_TYPE), scout)
subdir-ccflags-$(CONFIG_HISPIPP_V500) += -DNSV_IPP_SCOUT
endif

subdir-ccflags-$(CONFIG_HISPIPP_V500) += -Wdate-time -Wextra -Wfloat-equal -Wformat=2 -Wshadow
ifeq ($(USE_BISHENG_CLANG),true)
subdir-ccflags-$(CONFIG_HISPIPP_V500) += -Wno-error=unused-but-set-variable
endif
subdir-ccflags-$(CONFIG_HISPIPP_V500) += -I$(srctree)/platform_source/isp/drivers/ipp/v500 \
                                    -I$(srctree)/platform_source/isp/drivers/ipp/v500/dataflow \
                                    -I$(srctree)/platform_source/isp/drivers/ipp/v500/segment \
                                    -I$(srctree)/platform_source/isp/drivers/ipp/v500/modules \
                                    -I$(srctree)/platform_source/isp/drivers/ipp/v500/platform \
                                    -I$(srctree)/platform_source/isp/drivers/ipp/v500/platform/reg_offset_field \
                                    -I$(srctree)/platform_source/isp/drivers/ipp/v500/include \
                                    -I$(srctree)/platform_source/isp/drivers/ipp/v500/include/config_table \
                                    -I$(srctree)/platform_source/isp/drivers/ipp/v500/include/dataflow \
                                    -I$(srctree)/platform_source/isp/drivers/ipp/v500/include/drivers \
                                    -I$(srctree)/platform_source/isp/drivers/ipp/v500/include/modules \
                                    -I$(srctree)/platform_source/isp/drivers/ipp/v500/include/segment \
                                    -I$(srctree)/platform_source/isp/drivers/ipp/v500/include/ippmessage_inc \
                                    -I$(srctree)/../../third_party/bounds_checking_function/include \
                                    -I$(srctree)/drivers/platform_drivers/ap/platform/common
ifneq ($(TARGET_BUILD_VARIANT),user)
subdir-ccflags-$(CONFIG_HISPIPP_V500) += -DCONFIG_IPP_DEBUG
endif

