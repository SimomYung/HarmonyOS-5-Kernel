ifeq ($(CONFIG_SECBOOT_BUFFER_FORM_PAGE), y)
ccflags-$(CONFIG_LOAD_IMAGE) += -DCONFIG_SECBOOT_BUFFER_FORM_PAGE
endif

ccflags-y += -I$(srctree)/drivers/platform_drivers/tzdriver

obj-y += load_image.o
obj-y += load_vrl.o

ccflags-y += -Wdate-time -Wextra -Wfloat-equal -Wformat=2 -Wshadow