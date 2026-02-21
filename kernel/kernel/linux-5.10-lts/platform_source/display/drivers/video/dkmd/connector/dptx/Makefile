obj-$(CONFIG_DKMD_DPU_DP) += dp_ctrl/
obj-$(CONFIG_DKMD_DPU_DP) += dpu_dp.o

GCOV_PROFILE := $(CONFIG_GCOV_KERNEL)

dpu_dp-objs := \
	dp_drv.o \
	dkmd_dp.o

EXTRA_CFLAGS += \
	-I$(srctree)/include/ \
	-I$(srctree)/arch/arm64/include/ \
	-I$(srctree)/include/platform_include/display/dkmd/ \
	-I$(srctree)/include/platform_include/display/linux/ \
	-I$(srctree)/drivers/video/fbdev/dkmd/dksm \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm/timeline \
	-I$(srctree)/drivers/video/fbdev/dkmd/dksm/peri \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm/chrdev \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm/isr \
	-I$(srctree)/drivers/video/fbdev/dkmd/dpu/begonia/res_mgr \
	-I$(srctree)/drivers/video/fbdev/dkmd/dpu/begonia/res_mgr/config \
	-I$(srctree)/drivers/video/fbdev/dkmd/dpu/begonia/res_mgr/dvfs \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/mipi \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/dsc \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/spr \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/offline \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/dptx \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/dptx/dp_ctrl \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/dptx/dp_ctrl/controller \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/dptx/dp_ctrl/link \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/dptx/dp_ctrl/phy \
	-I$(srctree)/drivers/video/fbdev/dkmd/dpu/begonia/composer

CFLAGS_MODULE += -Wdate-time -Wextra -Wfloat-equal -Wformat=2 -Wshadow

DS_CHIP_TYPE := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_SUFFIX)))
DS_SOC_CHIP_NAME := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_PREFIX)))
ifeq ($(DS_CHIP_TYPE),)
EXTRA_CFLAGS += \
	-I$(srctree)/drivers/platform_drivers/ap/platform/$(DS_SOC_CHIP_NAME)
else
EXTRA_CFLAGS += \
	-I$(srctree)/drivers/platform_drivers/ap/platform/$(DS_SOC_CHIP_NAME)_$(DS_CHIP_TYPE)
endif

ifeq ($(CONFIG_DKMD_DPU_VERSION),$(filter $(CONFIG_DKMD_DPU_VERSION), 720))
EXTRA_CFLAGS += \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/dptx/dp_ctrl/config/v720
else ifeq ($(CONFIG_DKMD_DPU_VERSION),$(filter $(CONFIG_DKMD_DPU_VERSION), 740))
EXTRA_CFLAGS += \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/dptx/dp_ctrl/config/v740
else ifeq ($(CONFIG_DKMD_DPU_VERSION),$(filter $(CONFIG_DKMD_DPU_VERSION), 800 900 9200))
EXTRA_CFLAGS += \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/dptx/dp_ctrl/config/v800
else
EXTRA_CFLAGS += \
	-I$(srctree)/drivers/video/fbdev/dkmd/connector/dptx/dp_ctrl/config/default
endif

clean:
	rm -rf *.o .*.cmd *.ko *.mod.c .tmp_versions *.symvers *.order *.symversions signed .*.d
	find . -name "*.o" | xargs rm -f
	find . -name ".*.cmd" | xargs rm -f
	find . -name ".*.d" | xargs rm -f
	find . -name "*.gcno" | xargs rm -f
