
-include $(srctree)/drivers/platform_drivers/modem/drv/product_config/product_config.mk

obj-y += dump_mdmap_agent.o dump_mdmap_exc_cfg.o dump_mdmap_log.o dump_area.o dump_field_strategy.o

ifeq ($(strip $(CFG_BSP_CONFIG_PHONE_TYPE)),YES)
obj-y += dump_baseinfo.o dump_boot_check.o dump_ko.o dump_lastkmsg.o mdmap_subsys_exc_stub.o dump_area_phone.o
endif
