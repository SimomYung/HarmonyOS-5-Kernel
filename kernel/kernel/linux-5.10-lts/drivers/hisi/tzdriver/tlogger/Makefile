KERNEL_DIR := $(srctree)

ifneq ($(TARGET_BUILD_VARIANT),user)
	ccflags-y += -DDEF_ENG
endif

EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver
EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver/core
EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver/include
EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver/src_adaptor

ifeq ($(CONFIG_TZDRIVER_INTERNAL),y)
	include $(KERNEL_DIR)/drivers/platform_drivers/tzdriver/tzdriver_internal/internal.mk
endif

obj-$(CONFIG_TEELOG) += tlogger.o

# For tee log memory type: bbox, rdr, or pages,
# If no log mechanism is available, the pages memory can be used.
obj-$(CONFIG_PAGES_MEM) += log_pages_cfg.o
obj-$(CONFIG_RDR_MEM) += log_rdr_cfg.o
