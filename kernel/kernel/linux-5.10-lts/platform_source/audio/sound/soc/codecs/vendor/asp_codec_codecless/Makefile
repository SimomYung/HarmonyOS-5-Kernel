GCOV_PROFILE := $(CONFIG_GCOV_KERNEL)
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/
EXTRA_CFLAGS += -I$(srctree)/include/linux/platform_drivers/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/platforms/common_image/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/include/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/drivers/soc_mad/
EXTRA_CFLAGS +=	-I$(srctree)/platform_source/audio/drivers/hifi_dsp/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/asp_codec_codecless/base/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/asp_codec_codecless/audio_pinctrl/
EXTRA_CFLAGS += -I$(srctree)/include/platform_include/audio/ipc/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/drivers/hifi_mailbox/mailbox/
EXTRA_CFLAGS +=	-I$(srctree)/platform_source/audio/drivers/hifi_dsp/om/
ifeq ($(CONFIG_BALT_IP_PLATFORM),y)
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/asp_codec_codecless/platform/balt/
obj-y += platform/balt/
endif

ifeq ($(CONFIG_CHAR_IP_PLATFORM),y)
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/asp_codec_codecless/platform/clt/
obj-y += platform/clt/
endif

ifeq ($(CONFIG_CHARPRO_IP_PLATFORM),y)
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/asp_codec_codecless/platform/cpro/
obj-y += platform/cpro/
endif

ifeq ($(CONFIG_NA_IP_PLATFORM),y)
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/asp_codec_codecless/platform/nap/
obj-y += platform/nap/
endif

LP_SOC_CHIP_NAME := $(strip $(subst ", ,$(CONFIG_VENDOR_CHIP_PREFIX)))
ifeq ($(LP_SOC_CHIP_NAME),nashville)
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/asp_codec_codecless/platform/nsv/
obj-y += platform/nsv/
endif

LP_SOC_CHIP_NAME := $(findstring xian, $(CONFIG_VENDOR_CHIP_PREFIX))
ifneq ($(LP_SOC_CHIP_NAME),)
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/asp_codec_codecless/platform/xian/
obj-y += platform/xian/
endif

EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/asp_codec_codecless/store/

snd-soc-asp-codec-base-objs := base/asp_codec_utils.o base/asp_codec_control.o store/asp_codec_store.o base/codec_pm.o

ifeq ($(CONFIG_SND_SOC_PC_PCM), y)
snd-soc-asp-codec-base-objs += base/asp_codec_pc_sr.o
endif

ifeq ($(CONFIG_DRV_IODIE),y)
snd-soc-asp-codec-base-objs += base/dsp_iodie_drv.o
endif

obj-y += snd-soc-asp-codec-base.o

obj-y += audio_pinctrl/
