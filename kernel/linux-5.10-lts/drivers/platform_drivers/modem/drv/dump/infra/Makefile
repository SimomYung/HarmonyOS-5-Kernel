-include $(srctree)/drivers/platform_drivers/modem/drv/product_config/product_config.mk

obj-y += dump_config.o dump_sharemem.o dump_teeos_service.o dump_works.o

ifeq ($(strip $(CFG_BSP_CONFIG_PHONE_TYPE)),YES)
obj-y += log/dump_extra_logs_stub.o
else
ifeq ($(strip $(CFG_CONFIG_MODEM_FULL_DUMP)),YES)
obj-y += log/dump_extra_logs.o
else
obj-y += log/dump_extra_logs_stub.o
endif
endif

ifeq ($(strip $(CFG_CONFIG_MODEM_DUMP_UPLOAD)),YES)
obj-y += log/dump_genl.o log/dump_log_uploader.o
endif

ifeq ($(strip $(CFG_BSP_CONFIG_PHONE_TYPE))_$(strip $(CONFIG_HISI_MODEM_OHOS)),YES_y)
obj-y += log/dump_log_parser.o
endif

ifeq ($(strip $(CFG_BSP_CONFIG_PHONE_TYPE)),YES)
obj-y += log/dump_log_compress.o
endif

obj-y += log/dump_log_strategy.o log/dump_logs.o
