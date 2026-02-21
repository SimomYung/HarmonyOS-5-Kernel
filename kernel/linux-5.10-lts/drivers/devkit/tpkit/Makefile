# Makefile for the irda drivers.
#

# Each configuration option enables a list of files.

obj-$(CONFIG_HUAWEI_HOST_TS_KIT) += hostprocessing/
obj-$(CONFIG_HUAWEI_ROTARY_CROWN)  += panel_driver/
obj-$(CONFIG_HUAWEI_TS_KIT_3_0)    += 3_0/

ifeq ($(CONFIG_MODULE_KO_TP),y)
obj-$(CONFIG_HUAWEI_TP_KIT_COLOR_INFO_3_0) += 3_0/
endif
