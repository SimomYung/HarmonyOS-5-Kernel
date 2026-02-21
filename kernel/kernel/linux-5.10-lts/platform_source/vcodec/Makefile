
obj-$(CONFIG_VCODEC_VDEC) += vdec/
obj-$(CONFIG_VCODEC_VENC) += venc/
obj-$(CONFIG_VDEC_INDEP_RST_VM_CTRL) += vdec/vdec_indep_rst/

menuconfig:
	cd ../..;make ARCH=arm64 menuconfig KBUILD_KCONFIG=platform_source/vcodec/Kconfig KCONFIG_CONFIG=platform_source/vcodec/.config
