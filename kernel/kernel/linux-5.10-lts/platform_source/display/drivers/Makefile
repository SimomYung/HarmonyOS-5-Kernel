
obj-$(CONFIG_DRMDRIVER)                   += drmdriver/
obj-$(CONFIG_JPU_SUPPORT) += jpu/

obj-$(CONFIG_VFB_BE) += cdc_rda/vfb/
obj-$(CONFIG_DPU_INDEP_RST_VM_CTRL) += cdc_rda/indep_rst/

ifeq ($(CONFIG_RDA_DISPLAY_DRIVERS),y)
obj-y += cdc_rda/drm/ # rda drm driver
else
obj-$(CONFIG_DPU_DRM) += dpu_drm/ # 信创PC
endif