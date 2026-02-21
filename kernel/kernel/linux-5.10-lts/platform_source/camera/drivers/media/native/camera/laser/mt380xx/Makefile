#
# Makefile for the MT380X drivers.
#

# Each configuration option enables a list of files.

ccflags-y += -I$(src)/mt380x
ccflags-y += -I$(srctree)/platform_source/camera/drivers/media/native/camera/laser
obj-y += mt380x.o

# module
mt380x-objs := laser_mt380x_module.o

# mt380x
mt380x-objs += mt380x/mt380x_module.o
mt380x-objs += mt380x/mt380x_platform.o
mt380x-objs += mt380x/mt380x_api.o
mt380x-objs += mt380x/mt380x_firmware.o

