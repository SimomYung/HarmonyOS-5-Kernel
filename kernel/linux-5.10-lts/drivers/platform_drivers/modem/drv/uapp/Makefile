-include $(srctree)/drivers/platform_drivers/modem/drv/product_config/product_config.mk

subdir-ccflags-y := -Werror
subdir-ccflags-y += -I$(srctree)/drivers/platform_drivers/tzdriver
subdir-ccflags-y += -I$(srctree)/drivers/platform_drivers/tzdriver_hm

ifeq ($(strip $(CFG_CONFIG_SECBOOT_UAPP)),YES)
obj-y += uapp.o
endif
