ccflags-y += -Wall -Werror
EXTRA_CFLAGS += -Idrivers/platform_drivers/tzdriver

ifeq ($(CONFIG_LIBLINUX_CDC_RDA), y)
obj-$(CONFIG_MNTN_RDA_DIAGINFO) += bbox_diaginfo.o
obj-$(CONFIG_MNTN_RDA_DIAGINFO_DEBUG) += diaginfo_test.o
else
obj-$(CONFIG_DFX_BB_DIAGINFO) += bbox_lpmcu_diaginfo.o
obj-$(CONFIG_DFX_BB_DIAGINFO) += bbox_ap_ipc.o
obj-$(CONFIG_DFX_BB_DIAGINFO) += bbox_diaginfo.o mntn_diaginfo_record.o bbox_user_diaginfo.o
obj-$(CONFIG_DFX_HW_DIAG) += dfx_hw_diag.o
obj-$(CONFIG_MNTN_DIAGINFO_SERVER) += diaginfo_server.o
obj-$(CONFIG_DFX_DIAGINFO_BASEINFO) += dfx_diaginfo_baseinfo.o
endif
