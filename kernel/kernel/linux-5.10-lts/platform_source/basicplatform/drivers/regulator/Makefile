ccflags-y += -Wdate-time -Wextra -Wfloat-equal -Wformat=2 -Wshadow
PLATFORM_PATH := $(strip $(subst ", , $(CONFIG_IP_REGULATOR_PLATFORM)))

ifdef CONFIG_IP_PLATFORM_UNIFICATION
EXTRA_CFLAGS += -I$(srctree)/platform_source/basicplatform/drivers/regulator/ip_regulator_autofs/$(PLATFORM_PATH)
endif

ifeq ($(CONFIG_NSV_IP_PLATFORM),y)
obj-$(CONFIG_DRV_IP_REGULATOR) += ip_regulator_new.o
else ifeq ($(PLATFORM_PATH),spreq)
obj-$(CONFIG_DRV_IP_REGULATOR) += ip_regulator_new.o
else
obj-$(CONFIG_DRV_IP_REGULATOR) += ip_regulator.o
endif
obj-$(CONFIG_FREQ_AUTODOWN)+= ip_regulator_autofs/
obj-$(CONFIG_REGULATOR_VM_DEV) += regulator_vm_dev.o

EXTRA_CFLAGS += -I$(srctree)/drivers/regulator/
obj-$(CONFIG_REGULATOR_PLATFORM) += regulator_regmap.o
obj-$(CONFIG_REGULATOR_SMARTPA) += regulator_smartpa.o
obj-$(CONFIG_REGULATOR_SUB_PMU_SPMI) += regulator_sub_pmu_spmi.o
obj-$(CONFIG_DRV_REGULATOR_DEBUG) += regulator_debug.o
