
obj-$(CONFIG_OISP) += hoisp.o

ifeq ($(CONFIG_OISP), y)
subdir-ccflags-y += -DISP_LINX_CPU_SUPPORT
subdir-ccflags-y += -Wdate-time -Wextra -Wfloat-equal -Wformat=2 -Wshadow
subdir-ccflags-y += -I$(srctree)/../../vendor/huawei/chipset_common/plugins/kernel/include
endif
