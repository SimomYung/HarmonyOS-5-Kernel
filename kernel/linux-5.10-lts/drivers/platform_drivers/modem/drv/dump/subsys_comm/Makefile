-include $(srctree)/drivers/platform_drivers/modem/drv/product_config/product_config.mk

obj-y += dump_comm_agent.o dump_comm_log.o dump_sec_log.o

ifeq ($(strip $(CFG_BSP_CONFIG_PHONE_TYPE)),YES)
obj-y += dump_avs_escape.o dump_reset_log.o
endif
