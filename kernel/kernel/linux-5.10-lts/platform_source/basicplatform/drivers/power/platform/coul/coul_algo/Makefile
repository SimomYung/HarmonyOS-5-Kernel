
obj-$(CONFIG_COUL_ALGORITHM_CORE)         += coul.o
coul-y                                    := coul_merge.o coul_merge_drv.o coul_merge_sysfs.o coul_soe.o
coul-y                                    += coul_interface.o coul_core.o coul_dts.o coul_fault_evt_ops.o coul_nv.o coul_ocv_ops.o coul_private_interface.o coul_temp.o coul_ocv_monitor.o
obj-$(or $(CONFIG_HUAWEI_DSM),$(CONFIG_HUAWEI_OHOS_DSM)) += coul_dsm.o
obj-$(CONFIG_COUL_MERGE_BIGDATA)          += coul_merge_bigdata.o

EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/common/power