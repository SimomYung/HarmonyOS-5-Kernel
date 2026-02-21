ccflags-y += -I$(srctree)/drivers/thermal
obj-$(CONFIG_THERMAL_TSENSOR)	+= tsens.o
obj-$(CONFIG_THERMAL_PERIPHERAL)	+= peripheral_tm.o ntc_tm_table.o
obj-$(CONFIG_IPA_THERMAL)	+= ipa_thermal.o
ifneq ($(CONFIG_LIBLINUX),y)
obj-$(CONFIG_IPA_THERMAL)	+= ipa_kernel_thermal.o
obj-$(CONFIG_IPA_THERMAL)	+= lpm_thermal.o
obj-$(CONFIG_PERF_CTRL)		+= thermal_perf_ctrl.o
endif
ifeq ($(CONFIG_LIBLINUX),y)
ifeq ($(CONFIG_LDK_THERMAL),y)
obj-$(CONFIG_IPA_THERMAL)	+= ipa_kernel_thermal.o
obj-$(CONFIG_IPA_THERMAL)	+= lpm_thermal.o
obj-$(CONFIG_LDK_THERMAL)	+= ipafreq_set.o
obj-$(CONFIG_PERF_CTRL)		+= thermal_perf_ctrl.o
endif
endif
obj-$(CONFIG_THERMAL_HOTPLUG)   += hotplug/hotplug_thermal_gov.o
obj-$(CONFIG_THERMAL_HOTPLUG)   += hotplug/hotplug_thermal_cpu.o
obj-$(CONFIG_THERMAL_GPU_HOTPLUG)   += hotplug/hotplug_thermal_gpu.o
obj-$(CONFIG_THERMAL_NPU_HOTPLUG)   += hotplug/hotplug_thermal_npu.o
obj-$(CONFIG_THERMAL_ISP_HOTPLUG)   += hotplug/hotplug_thermal_isp.o
obj-$(CONFIG_GPU_IPA_THERMAL)	+= gpu_ipa_thermal.o
obj-$(CONFIG_THERMAL_SOC_SHELL)	+= soc_shell_thermal.o soc_shell_thermal_sysfs.o
obj-$(CONFIG_OPT_IPA_THERMAL)	+= opt_ipa_thermal.o opt_ipa_thermal_sysfs.o
obj-$(CONFIG_THERMAL_SHELL) += shell_temp.o
obj-$(CONFIG_THERMAL_AMBIENT) += ambient_temp.o
obj-$(CONFIG_HKADC_MODEM) += modem_hkadc_temp.o
obj-$(CONFIG_ITS) += its.o
obj-$(CONFIG_THERMAL_NPU) += npu_thermal.o
ifeq ($(CONFIG_THERMAL_NPU_V2),y)
obj-y += npu_thermal_v2.o
obj-$(CONFIG_THERMAL_GOV_DYNAMIC_INTERVAL_CTRL) += npu_thermal_sysfs_v2.o
endif
obj-$(CONFIG_THERMAL_GOV_INTERVAL_CTRL) += gov_interval_ctrl.o
obj-$(CONFIG_IPA_THERMAL) += thermal_ctrl.o
obj-$(CONFIG_THERMAL_MNTN_DMD) += thermal_dmd.o
obj-$(CONFIG_THERMAL_INTELLIGENT_FREQ_CTRL) += shell_thermal.o
obj-$(CONFIG_THERMAL_INTELLIGENT_FREQ_CTRL) += cpufreq_intelligent_cooling.o
obj-$(CONFIG_LDK_THERMAL_INTELLIGENT)	+= shell_thermal_ldk.o
obj-y += hw_dpm/
