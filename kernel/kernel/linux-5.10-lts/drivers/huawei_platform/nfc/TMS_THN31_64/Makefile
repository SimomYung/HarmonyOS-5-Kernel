#
# Makefile for nfc devices
#
#EXTRA_CFLAGS += -Idrivers/huawei/device
ifdef CONFIG_NFC_WATCH_COMMU
obj-$(CONFIG_THN31_NFC_64)      += watch_thn31.o
else
obj-$(CONFIG_THN31_NFC_64)      += thn31.o
endif

ifdef CONFIG_EXT_INPUTHUB
ifeq ($(wildcard $(srctree)/drivers/huawei_platform/sensor/ext_inputhub/$(TARGET_BOARD_PLATFORM)),)
ccflags-y  += -I$(srctree)/drivers/huawei_platform/sensor/ext_inputhub/default/
else
ccflags-y  += -I$(srctree)/drivers/huawei_platform/sensor/ext_inputhub/$(TARGET_BOARD_PLATFORM)/
endif
endif