SENSORHUB_SOC_CHIP_NAME := $(strip $(subst ", ,$(CONFIG_VENDOR_CHIP_PREFIX)))
ccflags-y += -I$(srctree)/drivers/platform_drivers/ap/platform/$(SENSORHUB_SOC_CHIP_NAME)/
ccflags-y += -I$(srctree)/include/platform_include/smart/linux/platform/$(SENSORHUB_SOC_CHIP_NAME)/
ifeq ($(wildcard $(srctree)/drivers/huawei_platform/inputhub/$(SENSORHUB_SOC_CHIP_NAME)),)
ccflags-y  += -I$(srctree)/drivers/huawei_platform/inputhub/default/
else
ccflags-y  += -I$(srctree)/drivers/huawei_platform/inputhub/$(SENSORHUB_SOC_CHIP_NAME)/
endif
EXTRA_CFLAGS += -Idrivers/platform_drivers/tzdriver

ifeq ($(CONFIG_HEADER_FILE_REPLACE_EXTRACT),y)
ccflags-y  += -I$(srctree)/include/platform_include/smart/linux/platform/$(TARGET_BOARD_PLATFORM)
subdir-ccflags-y += -I$(srctree)/include/platform_include/smart/linux/platform/$(TARGET_BOARD_PLATFORM)
endif

ccflags-y  += -I$(srctree)/platform_source/smart/drivers/channels/

obj-$(CONFIG_CONTEXTHUB_IGS) += igs.o
igs-objs := igs_ion.o igs_dev.o

obj-$(CONFIG_CONTEXTHUB_IGS_DBG) += igs_dbg.o
igs_dbg-objs := igs_dbg_dev.o

obj-$(CONFIG_CONTEXTHUB_IGS_TOF) += igs_tof.o
igs_tof-objs := tof/igs_tof_dev.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
