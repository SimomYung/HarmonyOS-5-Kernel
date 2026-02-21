GCOV_PROFILE := $(CONFIG_GCOV_KERNEL)
ccflags-y += -Wdate-time -Wfloat-equal -Wformat=2

EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/include/
ifeq ($(CONFIG_MAD_EXTENSION),y)
EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/drivers/soc_mad/mad_extension/
endif

soc-mad-objs := soc_mad.o

obj-$(CONFIG_SOC_MAD) += soc-mad.o

obj-$(CONFIG_MAD_EXTENSION) += mad_extension/