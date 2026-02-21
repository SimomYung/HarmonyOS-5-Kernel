subdir-ccflags-y              += -Wextra -Wdate-time -Wfloat-equal -Wshadow -Wformat=2 
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/tzdriver

obj-$(CONFIG_SEPLAT) += seplat.o seplat_mntn.o
obj-$(CONFIG_SEPLAT_TPM) += seplat_tpm.o

ifneq ($(TARGET_BUILD_VARIANT),user)
	obj-$(CONFIG_SEPLAT) += seplat_factory.o
	ifeq ($(CONFIG_SEPLAT_FMD_REPORT), y)
		obj-$(CONFIG_SEPLAT) += seplat_fmd.o
		EXTRA_CFLAGS += -DCONFIG_SEPLAT_FMD_REPORT
	endif
	EXTRA_CFLAGS += -DCONFIG_SEPLAT_FACTORY
endif

ifneq ($(CONFIG_SEPLAT_MNTN), y)
	EXTRA_CFLAGS += -DCONFIG_SEPLAT_MNTN_ABSENT
endif
