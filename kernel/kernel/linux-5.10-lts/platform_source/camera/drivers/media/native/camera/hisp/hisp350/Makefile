SOC_PLATFORM := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_PREFIX)))
SOC_CHIP_TYPE := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_SUFFIX)))

ccflags-y += -Wextra -Wdate-time -Wfloat-equal -Wshadow
ccflags-y += -I$(srctree)/platform_source/camera/drivers/media/native/camera/hisp
ccflags-y += -I$(srctree)/platform_source/camera/drivers/media/native/camera
ccflags-y += -I$(srctree)/drivers/platform_drivers/ap/platform/common
ccflags-y += -I$(srctree)/drivers/platform_drivers/ap/platform/$(SOC_PLATFORM)

ifneq ($(SOC_CHIP_TYPE),)
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/$(SOC_PLATFORM)
else
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/$(SOC_PLATFORM)_$(SOC_CHIP_TYPE)
endif


ifeq ($(strip $(subst ", , $(CONFIG_VENDOR_CHIP_SUFFIX))),es)
EXTRA_CFLAGS += -DCAM_CHIP_TYPE_ES
endif

ifeq ($(CONFIG_KERNEL_CAMERA_ISPV370), y)
EXTRA_CFLAGS += -DHISP370_CAMERA
endif
EXTRA_CFLAGS += -DHISP350_CAMERA
obj-y += hisp350.o
