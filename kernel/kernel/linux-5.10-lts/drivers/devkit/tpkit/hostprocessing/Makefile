EXTRA_CFLAGS += -I$(srctree)/drivers/hwspinlock
EXTRA_CFLAGS += -I$(srctree)/drivers/devkit/lcdkit/lcdkit3.0/kernel/common/include
EXTRA_CFLAGS += -I$(srctree)/include/dfx

obj-$(CONFIG_HUAWEI_THP) += thp.o
thp-objs += huawei_thp_api.o huawei_thp_attr.o huawei_thp_core.o huawei_thp_mt_wrapper.o huawei_thp_thread.o
thp-objs += huawei_thp_mt_wrapper_ioctl_common.o huawei_thp_mt_wrapper_ioctl_input.o huawei_thp_mt_wrapper_ioctl_pen.o
thp-objs += power_adapter/huawei_power_platform_resource.o power_adapter/huawei_power_adapter.o
thp-objs += power_adapter/huawei_power_config_parse.o power_adapter/huawei_independent_power_supply.o
thp-objs += power_adapter/huawei_multi_panel_common_power_supply.o power_adapter/huawei_tddi_power_supply.o
thp-objs += bus_adapter/huawei_bus_adapter.o bus_adapter/huawei_spi_bus_platform.o bus_adapter/huawei_spi_bus_config_parse.o
thp-$(CONFIG_HUAWEI_THP_PEN_CLK) +=huawei_pen_clk.o
obj-$(CONFIG_HUAWEI_THP_NOVATECH) += novatek_thp.o
obj-$(CONFIG_HUAWEI_THP_SYNAPTICS) +=synaptics_thp.o
obj-$(CONFIG_JDI_HOST_TS_KIT) += bu21150_thp.o
obj-$(CONFIG_HUAWEI_THP_HIMAX) += himax_thp.o
obj-$(CONFIG_HUAWEI_THP_GOODIX) += goodix_thp.o
obj-$(CONFIG_HUAWEI_THP_CHIPONE) += chipone_thp.o
obj-$(CONFIG_HUAWEI_THP_SOLOMON) += ssl_thp.o
obj-$(CONFIG_HUAWEI_SHB_THP) += huawei_thp_log.o
obj-$(CONFIG_HUAWEI_THP_ILITEK) += ilitek_thp.o
obj-$(CONFIG_HUAWEI_THP_ST) += stmicro_thp.o
obj-$(CONFIG_HUAWEI_THP_RAYDIUM) += raydium_thp.o
obj-$(CONFIG_HUAWEI_THP_FOCALTECH) += focaltech_thp.o
obj-$(CONFIG_HUAWEI_THP_HISILICON) += hisilicon_thp.o
obj-n += huawei_thp_test.o

ifeq ($(TARGET_BOARD_PLATFORM),taro)
EXTRA_CFLAGS += -DCONFIG_LCD_KIT_DRIVER
endif

ifeq ($(CONFIG_HUAWEI_THP_QCOM),y)
EXTRA_CFLAGS += -I$(srctree)/drivers/hwsensor/apsensor_channel
EXTRA_CFLAGS += -Idrivers/video/fbdev/hisi/dss
EXTRA_CFLAGS += -Idrivers/hisi/tzdriver
EXTRA_CFLAGS += -Idrivers/tzdriver
EXTRA_CFLAGS += -Idrivers/devkit/lcdkit/lcdkit1.0/core/common

ifeq ($(wildcard $(srctree)/drivers/huawei_platform/inputhub/$(TARGET_BOARD_PLATFORM)),)
EXTRA_CFLAGS += -Idrivers/huawei_platform/inputhub/default
else
EXTRA_CFLAGS += -Idrivers/huawei_platform/inputhub/$(TARGET_BOARD_PLATFORM)
endif

ifeq ($(TARGET_BOARD_PLATFORM), $(TRINKET))
EXTRA_CFLAGS += -Idrivers/gpu/drm/msm
EXTRA_CFLAGS += -Idrivers/gpu/drm/msm/dsi-staging
EXTRA_CFLAGS += -Idrivers/hwsensor/pssensor
endif

EXTRA_CFLAGS += -Idrivers/huawei_platform/sensor2_0/
else
EXTRA_CFLAGS += -I$(srctree)/drivers/video/fbdev/dkmd/dpu/azalea
EXTRA_CFLAGS += -I$(srctree)/drivers/hisi/tzdriver
EXTRA_CFLAGS += -I$(srctree)/drivers/tzdriver
EXTRA_CFLAGS += -I$(srctree)/drivers/devkit/lcdkit/lcdkit1.0/core/common

ifeq ($(wildcard $(srctree)/drivers/huawei_platform/inputhub/$(TARGET_BOARD_PLATFORM)),)
EXTRA_CFLAGS += -I$(srctree)/drivers/huawei_platform/inputhub/default
else
EXTRA_CFLAGS += -I$(srctree)/drivers/huawei_platform/inputhub/$(TARGET_BOARD_PLATFORM)
endif

ifeq ($(TARGET_BOARD_PLATFORM), $(TRINKET))
EXTRA_CFLAGS += -I$(srctree)/drivers/gpu/drm/msm
EXTRA_CFLAGS += -I$(srctree)/drivers/gpu/drm/msm/dsi-staging
EXTRA_CFLAGS += -I$(srctree)/drivers/hwsensor/pssensor
endif

EXTRA_CFLAGS += -I$(srctree)/drivers/huawei_platform/sensor2_0/
endif
EXTRA_CFLAGS += -I$(srctree)/drivers/misc/mediatek/sensor/2.0/core
EXTRA_CFLAGS += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/alsps/alspshub
EXTRA_CFLAGS += -I$(srctree)/platform_source
EXTRA_CFLAGS += -I$(srctree)/platform_include

ifeq ($(CONFIG_HW_EDP_DRIVER),y)
EXTRA_CFLAGS += -I$(srctree)/drivers/huawei_platform/edp/edp_ts_kit
endif
