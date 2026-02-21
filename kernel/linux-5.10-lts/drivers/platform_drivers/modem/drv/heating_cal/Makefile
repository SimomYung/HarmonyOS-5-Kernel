KBUILD_CPPFLAGS += -Idrivers/platform_drivers/modem/drv/nvim
-include $(srctree)/drivers/platform_drivers/modem/drv/product_config/product_config.mk

ifeq ($(strip $(CFG_CONFIG_HEATING_CAL_PHONE)),YES)
obj-y += heating_cal_phone.o
else
obj-y += heating_cal_iot.o
endif

obj-y += heating_cal_common.o
