obj-$(CONFIG_DKMD_DPU_DP) += hdcptx.o
GCOV_PROFILE := $(CONFIG_GCOV_KERNEL)

hdcptx-objs := \
	hdcp_common.o

# dksm for log, connect for dp_ctrl
EXTRA_CFLAGS += \
	-I$(srctree)/include/ \
	-I$(srctree)/arch/arm64/include/ \
	-I$(srctree)/include/platform_include/display/dkmd/ \
	-I$(srctree)/include/platform_include/display/linux/ \
	-I$(srctree)/drivers/video/fbdev/ukmd/cmdlist \
	-I$(srctree)/drivers/video/fbdev/dkmd/backlight \
	-I$(srctree)/drivers/video/fbdev/dkmd/dksm \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm \
	-I$(srctree)/drivers/video/fbdev/dkmd/uksm/timeline \
	-I$(srctree)/drivers/video/fbdev/dkmd/dksm/buf_sync \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm/fence \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm/chrdev \
	-I$(srctree)/drivers/video/fbdev/dkmd/dksm/peri \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm/isr \
	-I$(srctree)/drivers/video/fbdev/dkmd/dksm/wlt \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/mipi \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/dsc \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/spr \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/offline \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/panel \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/dptx/dp_ctrl \
	-I$(srctree)/drivers/video/fbdev/dkmd/dpu/begonia/res_mgr \
	-I$(srctree)/drivers/video/fbdev/dkmd/dpu/begonia/res_mgr/config

CFLAGS_MODULE += -Wdate-time -Wextra -Wfloat-equal -Wformat=2 -Wshadow
ifeq ($(CONFIG_DKMD_DPU_DEVICE), m)
ifeq ($(chip_type),)
EXTRA_CFLAGS += \
	-I$(srctree)/drivers/platform_drivers/ap/platform/$(TARGET_PRODUCT)
else
EXTRA_CFLAGS += \
	-I$(srctree)/drivers/platform_drivers/ap/platform/$(TARGET_PRODUCT)_$(chip_type)
endif
endif

clean:
	rm -rf *.o .*.cmd *.ko *.mod *.mod.c .tmp_versions *.symvers *.order *.symversions signed .*.d
	find . -name "*.o" | xargs rm -f
	find . -name ".*.cmd" | xargs rm -f
	find . -name ".*.d" | xargs rm -f
	find . -name "*.gcno" | xargs rm -f
