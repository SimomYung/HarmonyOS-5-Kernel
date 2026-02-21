obj-$(CONFIG_UKMD_UKSM) += ukmd_uksm.o
GCOV_PROFILE := $(CONFIG_GCOV_KERNEL)

ukmd_uksm-objs = \
	uksm_debug.o \
	ukmd_utils.o \
	chrdev/ukmd_sysfs.o \
	chrdev/ukmd_chrdev.o \
	fence/ukmd_acquire_fence.o \
	fence/ukmd_fence_utils.o \
	fence/ukmd_release_fence.o \
	isr/ukmd_isr.o \
	timeline/ukmd_timeline.o

EXTRA_CFLAGS += \
	-I$(srctree)/include/platform_include/display/ukmd/ \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm/chrdev \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm/fence \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm/timeline \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm/isr

CFLAGS_MODULE += -Wdate-time -Wextra -Wfloat-equal -Wformat=2 -Wshadow
# If you want to compile into modular drive, will need to decide
# according to the following configuration to compile
ifeq ($(CONFIG_UKMD_UKSM),m)

KERNEL_DIR = $(ANDROID_BUILD_TOP)/kernel/linux-5.10
KERNEL_OUT = $(ANDROID_PRODUCT_OUT)/obj/KERNEL_OBJ
PWD := $(shell pwd)
ARCH := arm64
CROSS_COMPILE := aarch64-linux-android-
CFLAGS_MODULE += -fno-pic

default:
	$(MAKE) -C $(KERNEL_OUT) ARCH=$(ARCH) CFLAGS=$(EXTRA_CFLAGS) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules
	$(MAKE) -C $(KERNEL_OUT) M=$(PWD) INSTALL_MOD_PATH=$(PWD)/signed INSTALL_MOD_STRIP=1 modules_install

endif

clean:
	rm -rf *.o .*.cmd *.ko *.mod *.mod.c .tmp_versions *.symvers *.order *.symversions signed .*.d
	find . -name "*.o" | xargs rm -f
	find . -name ".*.cmd" | xargs rm -f
	find . -name ".*.d" | xargs rm -f
	find . -name "*.gcno" | xargs rm -f
