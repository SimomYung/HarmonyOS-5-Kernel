GCOV_PROFILE := $(CONFIG_GCOV_KERNEL)
ccflags-y += -Wdate-time -Wfloat-equal -Wformat=2

EXTRA_CFLAGS += -I$(srctree)/platform_source/audio/include/

mad-extension-objs := mad_extension.o

obj-$(CONFIG_MAD_EXTENSION) += mad-extension.o
