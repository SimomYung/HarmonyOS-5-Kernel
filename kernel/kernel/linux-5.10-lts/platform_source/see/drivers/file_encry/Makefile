# special for charlotte_csv2 phone:
FBE_SOC_CHIP_NAME := $(strip $(subst ", ,$(CONFIG_VENDOR_CHIP_PREFIX)))
FBE_CHIP_TYPE := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_SUFFIX)))

ifeq ($(FBE_SOC_CHIP_NAME), charlotte)
ifneq ($(product_type), armpc)
ifeq ($(FBE_CHIP_TYPE), csv2)
ccflags-$(CONFIG_FILE_BASED_ENCRYPTO) += -DCONFIG_FBE3_1
CONFIG_FBE3_1 := y
ifeq ($(CONFIG_HIBERNATION),y)
ccflags-$(CONFIG_FILE_BASED_ENCRYPTO) += -DCONFIG_FBE_S4
endif
else
ccflags-$(CONFIG_FILE_BASED_ENCRYPTO) += -DCONFIG_FBE_MSPC_ABSENT
endif
endif
endif
# other product def should defined in defconfig

ifeq ($(CONFIG_FILE_BASED_ENCRYPTO_S4),y)
ccflags-$(CONFIG_FILE_BASED_ENCRYPTO) += -DCONFIG_FBE_S4
endif

ifeq ($(CONFIG_FBE3_1), y)
ccflags-y += -I$(srctree)/drivers/platform_drivers/tzdriver \
	     -I$(srctree)/platform_source/basicplatform/drivers/rpmb
else
ccflags-y += -I$(srctree)/drivers/platform_drivers/tzdriver \
	     -I$(srctree)/platform_source/see/drivers/crypto_core
endif

ifeq ($(CONFIG_FBE_ARMPC), y)
ccflags-$(CONFIG_FILE_BASED_ENCRYPTO) += -DCONFIG_PRODUCT_ARMPC
endif

ccflags-y += -Wdate-time -Wextra -Wfloat-equal -Wformat=2 -Wshadow

obj-$(CONFIG_FILE_BASED_ENCRYPTO) += fbex_dev.o \
	fbex_driver.o fbex_screen_handle.o fbex_preload.o
obj-$(CONFIG_FBE_ARMPC) += fbex_armpc.o

ccflags-$(CONFIG_FBE4_UECE_FOR_HM_FSCRYPTO) += -DCONFIG_FBE_UECE
obj-$(CONFIG_FBE4_UECE_FOR_HM_FSCRYPTO) += fbex_uece.o

obj-$(CONFIG_FILE_BASED_ENCRYPTO_FOR_HM_FSCRYPTO) += fbex_fscrypto.o
obj-$(CONFIG_FILE_BASED_ENCRYPTO_DBG) += fbex_debug.o
