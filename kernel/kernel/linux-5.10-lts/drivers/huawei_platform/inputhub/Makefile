subdir-ccflags-y += -Idrivers/hisi/tzdriver
ifeq ($(wildcard $(srctree)/drivers/huawei_platform/inputhub/$(TARGET_BOARD_PLATFORM)),)
obj-y += default/
else
obj-y += $(TARGET_BOARD_PLATFORM)/
endif

ifeq ($(CONFIG_HEADER_FILE_REPLACE_EXTRACT),y)
ccflags-y  += -I$(srctree)/include/platform_include/smart/linux/platform/$(TARGET_BOARD_PLATFORM)
subdir-ccflags-y += -I$(srctree)/include/platform_include/smart/linux/platform/$(TARGET_BOARD_PLATFORM)
endif