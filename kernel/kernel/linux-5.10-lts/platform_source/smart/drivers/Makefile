ifeq ($(CONFIG_HEADER_FILE_REPLACE_EXTRACT),y)
ccflags-y  += -I$(srctree)/include/platform_include/smart/linux/platform/$(TARGET_BOARD_PLATFORM)
subdir-ccflags-y += -I$(srctree)/include/platform_include/smart/linux/platform/$(TARGET_BOARD_PLATFORM)
endif

ccflags-y  += -I$(srctree)/platform_source/smart/drivers
subdir-ccflags-y += -I$(srctree)/platform_source/smart/drivers
obj-$(CONFIG_CONTEXTHUB)   += bsp/
obj-y   += channels/
obj-$(CONFIG_BSP_SYSCOUNTER) += syscounter/