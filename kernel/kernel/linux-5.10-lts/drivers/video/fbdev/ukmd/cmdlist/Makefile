obj-$(CONFIG_UKMD_CMDLIST_ENABLE) += ukmd_cmdlist.o

GCOV_PROFILE := $(CONFIG_GCOV_KERNEL)

ukmd_cmdlist-objs := \
	cmdlist_node.o \
	cmdlist_client_mgr.o \
	cmdlist_mem_dma.o \
	cmdlist_mem_ion.o \
	cmdlist_mem_mgr.o \
	cmdlist_client.o \
	cmdlist_dev.o \
	cmdlist_drv.o \
	cmdlist_interface.o

EXTRA_CFLAGS += \
	-I$(srctree)/drivers/video/fbdev/ukmd/cmdlist \
	-I$(srctree)/drivers/video/fbdev/ukmd/cmdlist/include \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm/chrdev \
	-I$(srctree)/drivers/video/fbdev/ukmd/uksm/fence \
	-I$(srctree)/include/platform_include/display/ukmd \
	-I$(srctree)/arch/arm64/include/ \
	-I$(srctree)/drivers/staging/android/ion \
	-I$(srctree)/include/

ifeq ($(CONFIG_DKMD_DPU_RES_MGR),y)
EXTRA_CFLAGS += -I$(srctree)/drivers/video/fbdev/dkmd/dpu/begonia/res_mgr
EXTRA_CFLAGS += -I$(srctree)/drivers/video/fbdev/dkmd/dpu/begonia/res_mgr/config
EXTRA_CFLAGS += -DDPU_RES_MGR
endif

ifeq ($(CONFIG_AKMD_AAE_RES_MGR),y)
EXTRA_CFLAGS += -I$(srctree)/drivers/video/fbdev/akmd/res_mgr
EXTRA_CFLAGS += -I$(srctree)/drivers/video/fbdev/akmd/res_mgr/config
EXTRA_CFLAGS += -DAAE_RES_MGR
endif

CFLAGS_MODULE += -Wdate-time -Wextra -Wfloat-equal -Wformat=2 -Wshadow
# If you want to compile into modular drive, will need to decide
# according to the following configuration to compile
ifeq ($(CONFIG_UKMD_CMDLIST_ENABLE),m)

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
