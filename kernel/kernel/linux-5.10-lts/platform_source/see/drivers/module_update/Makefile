
ccflags-$(CONFIG_MODULE_UPDATE)+= -I$(srctree)/drivers/platform_drivers/tzdriver
ccflags-$(CONFIG_MODULE_UPDATE)+= -I$(srctree)/platform_source/see/drivers/module_update/include

obj-$(CONFIG_MODULE_UPDATE)    += src/ioctl/module_update_ioctl.o
obj-$(CONFIG_MODULE_UPDATE)    += src/module_update_by_tee.o

ccflags-y += -Wdate-time -Wextra -Wfloat-equal -Wformat=2 -Wshadow