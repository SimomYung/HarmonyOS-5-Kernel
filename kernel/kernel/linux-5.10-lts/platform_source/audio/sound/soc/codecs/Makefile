subdir-ccflags-y += -Wdate-time -Wfloat-equal -Wformat=2 -Wextra -Wshadow

EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/drivers/da_combine_dsp/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/drivers/codec_bus/slimbus/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/drivers/codec_bus/soundwire/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/drivers/da_separate/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/platforms/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/asp_codec_ana/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/adapter/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/platforms/common_image/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/include/

ifeq ($(CONFIG_AUDIO_DEBUG),y)
endif

snd-soc-hdmi-audio-objs := vendor/hdmi-audio.o
snd-soc-dp-audio-objs := vendor/dp_audio.o

ccflags-y  += -I$(srctree)/drivers/devkit/audiokit/
ifneq ($(TARGET_BUILD_VARIANT),user)
	EXTRA_CFLAGS += -I$(srctree)/drivers/devkit/audiokit/headset_debug
	EXTRA_CFLAGS += -DCONFIG_HUAWEI_HEADSET_DEBUG
endif

ifneq ($(strip $(CONFIG_SWITCH)),)
ccflags-y += -DCONFIG_HUAWEI_HEADSET_DEBUG_SWITCH
endif

obj-$(CONFIG_HDMI_K3) += snd-soc-hdmi-audio.o
obj-$(CONFIG_SND_SOC_DP_AUDIO)    += snd-soc-dp-audio.o


obj-y += vendor/adapter/asoc_adapter.o
obj-$(CONFIG_SND_SOC_CODEC_DEBUG) += vendor/codec_debug.o

obj-$(CONFIG_SND_SOC_ASP_CODEC_ANA) += vendor/asp_codec_ana/
obj-$(CONFIG_SND_SOC_ASP_CODEC_CODECLESS) += vendor/asp_codec_codecless/

ifeq ($(product_type),cdc_ace)
obj-$(CONFIG_CHARPRO_IP_PLATFORM) += vendor/proxy_codec/
endif

obj-$(CONFIG_SND_GT_PROXY_CODEC) += vendor/proxy_codec/
