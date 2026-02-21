ifneq ($(TARGET_BUILD_VARIANT),user)
ifeq ($(CONFIG_LCD_KIT_DEBUG),y)
EXTRA_CFLAGS += -DLCD_KIT_DEBUG_ENABLE
endif
endif

ifeq ($(TARGET_VERSION_MODE),factory)
EXTRA_CFLAGS += -DLCD_FACTORY_MODE
endif

ifneq ($(TARGET_BUILD_VARIANT),user)
ifeq ($(CONFIG_LCD_KIT_DEBUG),y)
obj-y += lcd_kit_debug_dkmd.o
endif
endif

ifeq ($(CONFIG_INPUTHUB_PLAT_C),y)
EXTRA_CFLAGS += -DLCD_ALSC_ENABLE
endif

obj-y	+= lcd_kit_disp.o
obj-y	+= lcd_kit_panel.o
obj-y	+= lcd_kit_utils.o
obj-y	+= lcd_kit_part_utils.o
obj-y	+= lcd_kit_power.o
obj-y	+= lcd_kit_ext_power.o
obj-y	+= lcd_kit_parse_spr.o
obj-y	+= lcd_kit_scp.o
obj-y	+= lcd_kit_power_key.o
obj-y	+= lcd_kit_dynamic_panel.o
obj-y	+= panel/c12_6p88_fw_partial_refresh.o
obj-$(CONFIG_LCD_KIT_HYBRID)	+= hybrid/

EXTRA_CFLAGS += -I$(srctree)/include/platform_include/display/dkmd \
		-I$(srctree)/drivers/devkit/lcdkit/lcdkit3.0 \
		-I$(srctree)/drivers/devkit/lcdkit/lcdkit3.0/kernel/common/include \
		-I$(srctree)/drivers/devkit/lcdkit/lcdkit3.0/kernel/dkmd/include \
		-I$(srctree)/drivers/devkit/lcdkit/lcdkit3.0/kernel/dkmd/adapt \
		-I$(srctree)/drivers/devkit/lcdkit/lcdkit3.0/kernel/dkmd/displayengine \
		-I$(srctree)/drivers/devkit/lcdkit/lcdkit3.0/kernel/dkmd/displayengine/include \
		-I$(srctree)/drivers/video/fbdev/dkmd/aod \
		-I$(srctree)/drivers/huawei_platform/inputhub/default \
		-I$(srctree)/platform_source/camera/drivers/media/native/camera \
		-I$(srctree)/drivers/regulator/

ifeq ($(CONFIG_EXT_INPUTHUB_MATTING_ALSC),y)
EXTRA_CFLAGS += -I$(srctree)/drivers/huawei_platform/sensor/ext_inputhub/matting_algo_task
endif
