#net om module
-include $(srctree)/drivers/platform_drivers/modem/drv/product_config/product_config.mk
subdir-ccflags-y := -Werror

obj-y += net_om_pkt.o net_om_cnt.o

ifeq ($(strip $(CFG_CONFIG_DPA)),YES)
obj-y += net_om_dpa.o
endif
