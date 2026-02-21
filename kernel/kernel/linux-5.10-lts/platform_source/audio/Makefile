ifeq ($(CONFIG_RDA_AUDIO_DRIVERS),y)
obj-y += rda/ldk/
else
obj-y += drivers/
obj-y += sound/
endif

menuconfig:
	cd ../..;make ARCH=arm64 menuconfig KBUILD_KCONFIG=platform_source/audio/Kconfig KCONFIG_CONFIG=platform_source/audio/.config
