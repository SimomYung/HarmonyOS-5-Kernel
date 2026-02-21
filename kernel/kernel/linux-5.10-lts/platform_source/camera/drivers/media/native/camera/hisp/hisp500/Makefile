
SOC_PLATFORM := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_PREFIX)))
SOC_CHIP_TYPE := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_SUFFIX)))

ccflags-y += -Wextra -Wdate-time -Wfloat-equal -Wshadow
ccflags-y += -I$(srctree)/platform_source/camera/drivers/media/native/camera/hisp
ccflags-y += -I$(srctree)/platform_source/camera/drivers/media/native/camera
ccflags-y += -I$(srctree)/drivers/platform_drivers/ap/platform/common
ccflags-y += -I$(srctree)/drivers/platform_drivers/ap/platform/$(SOC_PLATFORM)
ccflags-y += -I$(srctree)/platform_source/ffrt/hts/hpd/v100
ccflags-y += -I$(srctree)/platform_source/ffrt/hts/hkd/interfaces

ifeq ($(SOC_CHIP_TYPE), scout)
EXTRA_CFLAGS += -DSCOUT
endif

ifeq ($(strip $(subst ", , $(CONFIG_VENDOR_CHIP_SUFFIX))),es)
EXTRA_CFLAGS += -DCAM_CHIP_TYPE_ES
endif
EXTRA_CFLAGS += -DHISP500_CAMERA
EXTRA_CFLAGS += -DISP_LINX_CPU_SUPPORT
obj-y += hisp500.o
