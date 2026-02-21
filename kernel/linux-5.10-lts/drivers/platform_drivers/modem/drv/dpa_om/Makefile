-include $(srctree)/drivers/platform_drivers/modem/drv/product_config/product_config.mk
ifeq ($(strip $(CFG_CONFIG_DPA_OM)),YES)
ifeq ($(strip $(CFG_CONFIG_MODEM_CORESIGHT)),YES)
obj-y                   := om_msg.o dump.o ocd.o
else
obj-y                   := om_msg.o dump.o ocd_stub.o
endif
else
obj-y                   := om_stub.o
endif