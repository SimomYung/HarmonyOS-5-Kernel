
-include $(srctree)/drivers/platform_drivers/modem/drv/product_config/product_config.mk
KBUILD_CPPFLAGS += -I$(srctree)/drivers/platform_drivers/modem/drv/odt_v500/core/
KBUILD_CPPFLAGS += -I$(srctree)/drivers/platform_drivers/modem/drv/odt_v500/debug/
KBUILD_CPPFLAGS += -I$(srctree)/drivers/platform_drivers/modem/drv/odt_v500/hal/common/
KBUILD_CPPFLAGS += -I$(srctree)/drivers/platform_drivers/modem/drv/odt_v500/hal/v300/

obj-y += hal/v300/v300_hal.o
obj-y += hal/common/hal_common.o
obj-y += core/odt_enc_src.o
obj-y += core/odt_enc_dst.o
obj-y += core/odt_init.o
obj-y += core/odt_irq_manager.o
obj-y += core/odt_mode_manager.o
obj-y += debug/odt_debug.o

subdir-ccflags-y := -Werror
