GCOV_PROFILE := $(CONFIG_GCOV_KERNEL)
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/adapter/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/asp_codec_codecless/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/asp_codec_ana/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/asp_codec_ana/base/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/asp_codec_ana/platform/asp_codec_ana_v7/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/drivers/da_separate/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/platforms/common_image/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/include/

obj-$(CONFIG_SND_SOC_ASP_CODEC_ANA_V7) += platform/asp_codec_ana_v7/
obj-$(CONFIG_SND_SOC_ASP_CODEC_ANA_V8) += platform/asp_codec_ana_v8/
snd-soc-asp-codec-ana-objs +=  base/asp_codec_ana_utils.o
snd-soc-asp-codec-ana-objs +=  base/asp_codec_ana.o
obj-$(CONFIG_SND_SOC_ASP_CODEC_ANA) += snd-soc-asp-codec-ana.o
