ccflags-y  += -I$(srctree)/include/linux/platform_drivers/contexthub/channels

include $(srctree)/platform_source/smart/drivers/channels/base_channels.mk

ifneq ($(CONFIG_INPUTHUB_AS),y)
include $(srctree)/platform_source/smart/drivers/channels/extern_channels.mk
endif

ifeq ($(CONFIG_CONTEXTHUB_IO_DIE_PCIE), y)
EXTRA_CFLAGS += -I$(srctree)/drivers/hwspinlock
endif
