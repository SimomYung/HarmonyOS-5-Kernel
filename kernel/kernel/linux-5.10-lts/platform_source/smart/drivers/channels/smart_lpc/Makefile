SENSORHUB_SOC_CHIP_NAME := $(strip $(subst ", ,$(CONFIG_VENDOR_CHIP_PREFIX)))
 
ifeq ($(wildcard $(srctree)/drivers/huawei_platform/inputhub/$(SENSORHUB_SOC_CHIP_NAME)),)
ccflags-y  += -I$(srctree)/drivers/huawei_platform/inputhub/default/
else
ccflags-y  += -I$(srctree)/drivers/huawei_platform/inputhub/$(SENSORHUB_SOC_CHIP_NAME)/
endif
EXTRA_CFLAGS += -Idrivers/platform_drivers/tzdriver
 
ccflags-y  += -I$(srctree)/platform_source/smart/drivers/channels/
 
obj-$(CONFIG_CONTEXTHUB_LPC) += smart_lpc_dev.o