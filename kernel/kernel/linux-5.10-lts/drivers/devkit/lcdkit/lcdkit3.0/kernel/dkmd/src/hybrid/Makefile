obj-y += lcd_kit_hybrid_core.o
obj-y += lcd_kit_hybrid_control.o
obj-y += lcd_kit_hybrid_parse.o
obj-y += lcd_kit_hybrid_swctrl.o
obj-y += lcd_kit_hybrid_aod.o
obj-y += lcd_kit_hybrid_force.o
obj-y += lcd_kit_hybrid_msg_trans.o
obj-y += lcd_kit_hybrid_trace.o

EXTRA_CFLAGS += \
	-I$(srctree)/drivers/devkit/lcdkit/lcdkit3.0/kernel/dkmd/include/hybrid \
	-I$(srctree)/drivers/devkit/lcdkit/lcdkit3.0/kernel/common/include \
	-I$(srctree)/drivers/devkit/lcdkit/lcdkit3.0/kernel/dkmd/include \
	-I$(srctree)/drivers/devkit/lcdkit/lcdkit3.0/kernel/dkmd/adapt \
	-I$(srctree)/drivers/devkit/tpkit/3_0/hybrid \
	-I$(srctree)/drivers/devkit/tpkit/3_0 \
	-I$(srctree)/include/platform_include/display/dkmd \
	-I$(srctree)/drivers/video/fbdev/dkmd/dpu/begonia/composer \
	-I$(srctree)/drivers/video/fbdev/dkmd/dpu/begonia/device \
	-I$(srctree)/drivers/video/fbdev/dkmd/dksm \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm/chrdev \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm/timeline \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm/isr \
	-I$(srctree)/drivers/huawei_platform/sensor/ext_inputhub/default \
	-I$(srctree)/drivers/huawei_platform/sensor/ext_inputhub/default/inner_cmd \
	-I$(srctree)/drivers/huawei_platform/power/watch_low_power_manager

ifeq ($(CONFIG_EXT_INPUTHUB_MATTING_ALSC),y)
EXTRA_CFLAGS += -I$(srctree)/drivers/huawei_platform/sensor/ext_inputhub/matting_algo_task
endif

ifeq ($(CONFIG_DFX_ZEROHUNG),y)
EXTRA_CFLAGS += -I$(srctree)/drivers/hwzerohung/watchpoint
endif