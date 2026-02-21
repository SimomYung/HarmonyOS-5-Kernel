KERNEL_DIR := $(srctree)

ifneq ($(TARGET_BUILD_VARIANT),user)
	ccflags-y += -DDEF_ENG
endif
ifeq ($(CONFIG_TZDRIVER_OHOS),y)
	ccflags-y += -DTA_PATH_LOADED_BY_TZDRIVER=\"/vendor/bin\"
endif

ifeq ($(product_type), armpc)
	ccflags-y += -DTZ_S4_SMC
	ccflags-y += -DCONFIG_PRODUCT_ARMPC
endif

EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver
EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver/agent_rpmb/core
EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver/auth
EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver/tlogger
EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver/tui
EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver/ion
EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver/core
EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver/src_adaptor
EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver/include

ifeq ($(CONFIG_TZDRIVER_INTERNAL), y)
	include $(KERNEL_DIR)/drivers/platform_drivers/tzdriver/tzdriver_internal/internal.mk
endif

obj-y += teek_client_api.o
obj-y += smc_smp.o tc_client_driver.o session_manager.o mailbox_mempool.o teek_app_load.o teek_app_read.o
obj-y += agent.o gp_ops.o mem.o cmdmonitor.o tzdebug.o tz_spi_notify.o tz_pm.o tee_compat_check.o
obj-y += reserved_mempool.o
obj-y += teek_client_ext.o
obj-y += tee_info.o
obj-y += shared_mem.o
obj-$(CONFIG_TZ_S4_SUPPORT) += tz_pm.o

ifdef CONFIG_ENABLE_DSTB
obj-y += dstb_msg.o
endif

ifdef CONFIG_FFA_SUPPORT
obj-y += ffa_abi.o
else
obj-y += smc_abi.o
endif
