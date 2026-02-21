SOC_PLATFORM := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_PREFIX)))
SOC_CHIP_TYPE := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_SUFFIX)))

ccflags-y += -Wextra -Wdate-time -Wfloat-equal -Wshadow -Wno-implicit-int
ifeq ($(es_low_freq),true)
EXTRA_CFLAGS += -DCONFIG_ES_LOW_FREQ
endif

ccflags-y += -I$(srctree)/platform_source/camera/drivers/media/native/camera/hjpeg
ccflags-y += -I$(srctree)/platform_source/camera/drivers/media/native/camera
ccflags-y += -I$(srctree)/drivers/platform_drivers/ap/platform/common
ccflags-y += -I$(srctree)/drivers/platform_drivers/ap/platform/$(SOC_PLATFORM)
obj-y += hjpeg_process.o smmu_cfg.o jpegenc_cfg.o cvdr_cfg.o default_config.o
ifeq ($(pcie_fpga_verify),true)
ccflags-$(CONFIG_KERNEL_CAMERA_ISPV350) += -DJPEG_USE_PCIE_VERIFY=1
endif
ifneq ($(CONFIG_KERNEL_CAMERA_ISPV300), y)
obj-y += hjpeg_debug.o
endif

ifeq ($(SOC_CHIP_TYPE), scout)
EXTRA_CFLAGS += -DSCOUT
endif

ifeq ($(CONFIG_KERNEL_CAMERA_ISPV350), y)
obj-y += platform_v350/
else ifeq ($(CONFIG_KERNEL_CAMERA_ISPV370), y)
obj-y += platform_v350/
else ifeq ($(CONFIG_KERNEL_CAMERA_ISPV500), y)
obj-y += platform_v500/
EXTRA_CFLAGS += -DJPEGENC_BYPASS
EXTRA_CFLAGS += -DJPEGENC_SMMU_TBU_DUMP_BYPASS
endif

ifeq ($(CONFIG_CAMERA_ISPV230), y)
EXTRA_CFLAGS += -DHISP230_CAMERA
endif
