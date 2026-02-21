ccflags-$(CONFIG_DPU_FB_V410) += -I$(srctree)/platform_source/display/drivers/jpu/include/jpgdecv400
ccflags-$(CONFIG_DPU_FB_V501) += -I$(srctree)/platform_source/display/drivers/jpu/incldue/jpgdecv501
ccflags-$(CONFIG_DPU_FB_V510) += -I$(srctree)/platform_source/display/drivers/jpu/include/jpgdecv511
ccflags-$(CONFIG_DPU_FB_V600) += -I$(srctree)/platform_source/display/drivers/jpu/include/jpgdecv600
ifeq ($(CONFIG_DKMD_DPU_VERSION), 700)
ccflags-y += -I$(srctree)/platform_source/display/drivers/jpu/include/jpgdecv700
endif

ifeq ($(CONFIG_DKMD_DPU_VERSION), 800)
ccflags-y += -I$(srctree)/platform_source/display/drivers/jpu/include/jpgdecv700
endif

ifeq ($(CONFIG_DKMD_DPU_VERSION), 820)
ccflags-y += -I$(srctree)/platform_source/display/drivers/jpu/include/jpgdecv700
endif

ifeq ($(es_low_freq),true)
	EXTRA_CFLAGS += -DCONFIG_ES_LOW_FREQ
endif

ifeq ($(CONFIG_JPEGDEC_DEBUG),y)
	EXTRA_CFLAGS += -DJPEGDEC_DEBUG_ENALBE
endif

JPU_SOC_CHIP_NAME := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_PREFIX)))
JPU_SOC_CHIP_TYPE := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_SUFFIX)))

ifeq ($(JPU_SOC_CHIP_TYPE), )
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/$(JPU_SOC_CHIP_NAME)
else
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/$(JPU_SOC_CHIP_NAME)_$(JPU_SOC_CHIP_TYPE)
endif

ifeq ($(JPU_SOC_CHIP_TYPE), scout)
EXTRA_CFLAGS += -DJPU_SCOUT
endif

ifeq ($(CONFIG_SOC_PG_INFO), y)
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/common
endif

LOCAL_CFLAGS += -DJPEGD_ACORE_ENALBLE
EXTRA_CFLAGS += -I$(srctree)/lib/libc_sec/securec_v2/include/
EXTRA_CFLAGS += -I$(srctree)/platform_source/display/drivers/jpu/include
EXTRA_CFLAGS += -I$(srctree)/platform_source/display/drivers/jpu/osal/linux
EXTRA_CFLAGS += -I$(srctree)/platform_source/display/drivers/jpu/acore

obj-$(CONFIG_JPU_SUPPORT) := devjpu.o
devjpu-objs := \
	acore/jpu.o \
	acore/jpu_power_manager.o\
	acore/jpu_iommu.o \
	osal/linux/linux_osal.o \
	common/jpu_utils.o \
	common/jpu_decode.o \
	common/jpgdec_irq.o

clean:
	rm *.o .*cmd
