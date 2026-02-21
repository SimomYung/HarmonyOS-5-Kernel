EXTRA_CFLAGS += $(NPU_GLOBAL_CFLAGS)
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/tzdriver/
EXTRA_CFLAGS += -I$(NKD_DIR)/inc
EXTRA_CFLAGS += -I$(NKD_DIR)/communication
EXTRA_CFLAGS += -I$(NPD_DIR)/ldk
EXTRA_CFLAGS += -I$(NPD_DIR)/udk
EXTRA_CFLAGS += -DCONFIG_NPU_LOAD_TS
obj-$(CONFIG_NPU_LOAD_TS) += npu_load_ts_img.o
