
-include $(srctree)/drivers/platform_drivers/modem/drv/product_config/product_config.mk
ifeq ($(strip $(CFG_CONFIG_AVS_TEST)),YES)
obj-y                   := avs_test.o
endif
ifeq ($(strip $(BSP_CONFIG_PHONE_TYPE)), YES)
obj-y                   += avs_cfg.o
endif