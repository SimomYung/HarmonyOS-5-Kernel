EXTRA_CFLAGS += -I$(srctree)/include/chipset_common/hwpower/common_module/

ifdef CONFIG_AUDIO_BATTERY_STATE
	ccflags-y += -I$(srctree)/drivers/devkit/audiokit/
endif

obj-$(CONFIG_AUDIO_BATTERY_STATE) += battery_state.o
