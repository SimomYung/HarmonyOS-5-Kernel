GCOV_PROFILE := $(CONFIG_GCOV_KERNEL)
subdir-ccflags-y += -Werror -Wdate-time -Wfloat-equal -Wformat=2

EXTRA_CFLAGS += -I$(srctree)/platform_source/basicplatform/drivers/mntn/blackbox/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/include/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/drivers/comm/
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/sound/soc/codecs/vendor/adapter/
EXTRA_CFLAGS += -I$(srctree)//platform_source/audio/sound/soc/platforms/common_image/

ifneq ($(CONFIG_SND_SOC_ARMPC_CUSTOM),y)
obj-y += audio_file.o
obj-y += comm_file.o
endif