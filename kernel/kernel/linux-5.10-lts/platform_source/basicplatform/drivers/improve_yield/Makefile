ccflags-y += -Wdate-time -Wextra -Wfloat-equal -Wformat=2 -Wshadow

SOC_PLATFORM := $(strip $(subst ", ,$(CONFIG_VENDOR_CHIP_PREFIX)))
SOC_CHIP_TYPE := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_SUFFIX)))

ifneq ($(SOC_CHIP_TYPE),)
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/$(SOC_PLATFORM)
else
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/$(SOC_PLATFORM)_$(SOC_CHIP_TYPE)
endif

EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/common

obj-$(CONFIG_IMPROVE_YIELD)  += improve_yield_enable.o improve_yield_module.o improve_yield_nve_ops.o improve_yield_register_info.o
