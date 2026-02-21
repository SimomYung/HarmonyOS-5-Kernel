
ccflags-y += -Wextra -Wdate-time -Wfloat-equal -Wformat=2 -Wshadow
ccflags-y += -I$(srctree)/platform_source/camera/drivers/media/native/camera
ccflags-y += -I$(srctree)/platform_source/smart/drivers/channels

NPU_CHIP_VERSION :=$(strip $(subst ", ,$(CONFIG_VENDOR_CHIP_PREFIX)))
ccflags-y += -I$(srctree)/drivers/platform_drivers/ap/platform/$(NPU_CHIP_VERSION)/
ifeq ($(wildcard $(srctree)/drivers/huawei_platform/inputhub/$(NPU_CHIP_VERSION)),)
ccflags-y  += -I$(srctree)/drivers/huawei_platform/inputhub/default/
else
ccflags-y  += -I$(srctree)/drivers/huawei_platform/inputhub/$(NPU_CHIP_VERSION)/
endif
EXTRA_CFLAGS += -Idrivers/platform_drivers/tzdriver

obj-$(CONFIG_CONTEXTHUB_IGS_CAM) += igs_cam.o

ifeq ($(CONFIG_CONTEXTHUB_IGS_CAM_20),y)
igs_cam-objs := igs_cam_dev_20.o
else
igs_cam-objs := igs_cam_dev.o
endif
