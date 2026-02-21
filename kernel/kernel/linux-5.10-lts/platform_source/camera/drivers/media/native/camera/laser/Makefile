#
# Makefile for camera laser driver
#

ccflags-y += -I$(srctree)/platform_source/camera/drivers/media/native/camera
EXTRA_CFLAGS += -fno-pic

ifeq ($(TARGET_VERSION_MODE),factory)
EXTRA_CFLAGS += -DCAM_LASER_FACTORY_MODE
endif

obj-y += laser_common.o
obj-y += hw_laser_dmd.o
obj-$(CONFIG_KERNEL_CAMERA_VL53L0_970) += hw_laser_subdev.o
obj-$(CONFIG_KERNEL_CAMERA_VL53L0) += vl53l0.o
obj-$(CONFIG_KERNEL_CAMERA_VL53L0_970) += vl53Lx/
obj-$(CONFIG_KERNEL_CAMERA_VL53L0_970) += vi53xx/
obj-$(CONFIG_KERNEL_CAMERA_VL53L0_970) += tmf880x/
obj-$(CONFIG_KERNEL_CAMERA_VL53L0_970) += mt380xx/
