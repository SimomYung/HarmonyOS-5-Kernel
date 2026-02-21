FBE_SOC_CHIP_NAME := $(strip $(subst ", ,$(CONFIG_VENDOR_CHIP_PREFIX)))
FBE_CHIP_TYPE := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_SUFFIX)))
FBE_CHIP_VERSION := $(FBE_SOC_CHIP_NAME)_$(FBE_CHIP_TYPE)

ifeq ($(FBE_SOC_CHIP_NAME), charlotte)
ifeq ($(FBE_CHIP_TYPE), csv2)
ccflags-$(CONFIG_FILE_BASED_ENCRYPTO) += -DCONFIG_FBE3_1
ifeq ($(CONFIG_HIBERNATION),y)
ccflags-$(CONFIG_FILE_BASED_ENCRYPTO) += -DCONFIG_FBE_S4
endif
else
ccflags-$(CONFIG_FILE_BASED_ENCRYPTO) += -DCONFIG_FBE_MSPC_ABSENT
endif
endif

ifeq ($(FBE_SOC_CHIP_NAME)_$(product_type), charlotte_armpc)
ccflags-$(CONFIG_FILE_BASED_ENCRYPTO) += -DCONFIG_PRODUCT_ARMPC
endif

ifeq ($(CONFIG_FBE3_1), y)
ccflags-y += -I$(srctree)/drivers/platform_drivers/tzdriver \
	     -I$(srctree)/platform_source/basicplatform/drivers/rpmb
else ifeq ($(FBE_CHIP_VERSION), charlotte_csv2)
ccflags-y += -I$(srctree)/drivers/platform_drivers/tzdriver \
	     -I$(srctree)/platform_source/basicplatform/drivers/rpmb
else
ccflags-y += -I$(srctree)/drivers/platform_drivers/tzdriver \
	     -I$(srctree)/platform_source/see/drivers/crypto_core
endif

ccflags-y += -Wdate-time -Wextra -Wfloat-equal -Wformat=2 -Wshadow

obj-$(CONFIG_FILE_BASED_ENCRYPTO) += fbex_dev.o \
	fbex_driver.o fbex_screen_handle.o fbex_preload.o
ifeq ($(FBE_SOC_CHIP_NAME)_$(product_type), charlotte_armpc)
obj-$(CONFIG_FILE_BASED_ENCRYPTO) += fbex_armpc.o
endif

ccflags-$(CONFIG_FBE4_UECE_FOR_HM_FSCRYPTO) += -DCONFIG_FBE_UECE
obj-$(CONFIG_FBE4_UECE_FOR_HM_FSCRYPTO) += fbex_uece.o

obj-$(CONFIG_FILE_BASED_ENCRYPTO_FOR_HM_FSCRYPTO) += fbex_fscrypto.o
obj-$(CONFIG_FILE_BASED_ENCRYPTO_DBG) += fbex_debug.o
