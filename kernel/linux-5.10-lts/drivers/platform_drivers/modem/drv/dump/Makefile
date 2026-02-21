#
# Makefile for the memory technology device drivers.
#

# Core functionality.
-include $(srctree)/drivers/platform_drivers/modem/drv/product_config/product_config.mk

obj-y               += infra/
obj-y               += subsys_comm/
obj-y               += subsys_cp/
obj-y               += subsys_mdmap/

ifeq ($(strip $(CFG_ENABLE_BUILD_DUMP_MDM_LPM3)),YES)
obj-y               += subsys_mdmlpm/
endif

obj-y += dump_exc_mgr.o dump_frame.o dump_subsys_mgr.o dump_rdr_wrapper.o

KBUILD_CPPFLAGS += -I$(srctree)/drivers/platform_drivers/modem/drv/dump \
                   -I$(srctree)/drivers/platform_drivers/modem/drv/dump/subsys_mdmap \
                   -I$(srctree)/drivers/platform_drivers/modem/drv/dump/infra \
                   -I$(srctree)/drivers/platform_drivers/modem/drv/dump/infra/log
