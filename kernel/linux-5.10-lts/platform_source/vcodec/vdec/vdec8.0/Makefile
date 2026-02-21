################# feature configuration ################
VCODEC_SOC_CHIP_NAME := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_PREFIX)))
VCODEC_SOC_CHIP_TYPE := $(strip $(subst ", , $(CONFIG_VENDOR_CHIP_SUFFIX)))

ifeq ($(CONFIG_VCODEC_PLATFORM_VCODECV800),y)
PRODUCT_VER := VCodecV800
EXTRA_CFLAGS += -DENABLE_VDEC_SELF_HEALING
ifeq ($(TARGET_VERSION_MODE),factory)
EXTRA_CFLAGS += -DVDEC_SELF_HEALING_ONLY_RECORD
endif
endif
ifeq ($(CONFIG_VCODEC_PLATFORM_VCODECV900),y)
PRODUCT_VER := VCodecV900
endif
VCODEC_PLATFORM_NAME := linux
include platform_source/vcodec/vdec/vdec8.0/product/$(PRODUCT_VER)/linux_cfg/vfmw_make.cfg

################# src files #######################
VFMW_SRC += vdec_self_healing.c
VFMW_SRC += vcodec_vdec.c
VFMW_SRC += vcodec_vdec_dbg.c
VFMW_SRC += vcodec_vdec_plat.c
VFMW_SRC += smmu/smmu.c
VFMW_SRC += smmu/smmu_osal.c
VFMW_SRC += smmu/smmu_regs.c
VFMW_SRC += smmu/smmu_tbu.c
VFMW_SRC += regulator/vcodec_vdec_regulator.c
VFMW_SRC += regulator/vdec_sc_regulator.c
VFMW_SRC += osal/$(VCODEC_PLATFORM_NAME)/vcodec_event_queue.c
VFMW_SRC += osal/$(VCODEC_PLATFORM_NAME)/vcodec_platform_osal.c
VFMW_SRC += osal/$(VCODEC_PLATFORM_NAME)/vcodec_platform_proc.c
VFMW_SRC += osal/$(VCODEC_PLATFORM_NAME)/vcodec_async_work.c
VFMW_SRC += osal/$(VCODEC_PLATFORM_NAME)/video_log.c
VFMW_SRC += product/$(PRODUCT_VER)/vcodec_vdec_plat_var.c

ifeq ($(TARGET_VERSION_MODE),factory)
EXTRA_CFLAGS += -DENABLE_VDEC_DUMP_REG
VFMW_SRC += osal/$(VCODEC_PLATFORM_NAME)/vdec_dump_reg.c
endif

ifeq ($(VFMW_PROC_SUPPORT),y)
EXTRA_CFLAGS += -DVFMW_PROC_SUPPORT
VFMW_SRC += proc/vcodec_proc.c
VFMW_SRC += proc/vcodec_proc_common.c
VFMW_SRC += proc/mcu/vcodec_proc_mcu.c
VFMW_SRC += proc/scd/vcodec_proc_scd.c
VFMW_SRC += proc/bsp/vcodec_proc_bsp.c
VFMW_SRC += proc/pxp/vcodec_proc_pxp.c
VFMW_SRC += proc/vdec/vcodec_proc_vdec.c
endif

VFMW_SRC += tvp/tvp_adapter.c

# when compile module, do not compile pcie
ifeq ($(CONFIG_VCODEC_VDEC),y)
ifeq ($(CONFIG_PCIE_KPORT_EP_FPGA_VERIFY),y)
EXTRA_CFLAGS += -DPCIE_FPGA_VERIFY
VFMW_SRC += vcodec_vdec_pcie.c
endif
endif

VFMW_SRC += dev_mgr/dev_mgr.c
VFMW_SRC += dev_mgr/scd/vcodec_scd_intf.c
VFMW_SRC += dev_mgr/bsp/vcodec_bsp_intf.c
VFMW_SRC += dev_mgr/pxp/vcodec_pxp_intf.c
VFMW_SRC += dev_mgr/util/vcodec_dev_mgr_util.c
VFMW_SRC += irq/vcodec_vdec_irq_handler.c
VFMW_SRC += memory/vcodec_vdec_memory_common.c
VFMW_SRC += memory/vcodec_dyn_ion.c
VFMW_SRC += tzsp_client/vdec_tzsp_client.c


# Identify the formal version or the FPGA version.
# CONFIG_VCODEC_VDEC_FPGA := y
ifeq ($(CONFIG_VCODEC_VDEC_FPGA),y)
	# Identify mcu version or non-mcu version.
	CONFIG_VCODEC_VDEC_MCU := y
	ifeq ($(CONFIG_VCODEC_VDEC_MCU), y)
		EXTRA_CFLAGS += -DVDEC_MCORE_ENABLE
		VFMW_SRC += vcodec_vdec_ioctl_mcu.c
		VFMW_SRC += dev_mgr/mcu/vcodec_mcu_dev.c
		VFMW_SRC += dev_mgr/mcu/vcodec_mcu_hal.c
		VFMW_SRC += dev_mgr/mcu/vcodec_mcu_intf.c
		VFMW_SRC += dev_mgr/mcu/vcodec_mcu_ipc.c
		VFMW_SRC += dev_mgr/mcu/vcodec_mcu_queue.c
	else
		VFMW_SRC += vcodec_vdec_ioctl.c
		VFMW_SRC += dev_mgr/mcu/vcodec_mcu_bypass.c
	endif

	# Identify normal irq version or poll irq version.
	# CONFIG_VCODEC_IRQ_POLL := y
	ifeq ($(CONFIG_VCODEC_IRQ_POLL), y)
		VFMW_SRC += irq/poll/vcodec_vdec_irq.c
	else
		VFMW_SRC += irq/normal/vcodec_vdec_irq.c
	endif

	# Identify normal version or smmu bypass version.
	# CONFIG_VCODEC_SMMU_BYPASS := y
	ifeq ($(CONFIG_VCODEC_SMMU_BYPASS), y)
		VFMW_SRC += memory/smmu_bypass/vcodec_vdec_memory.c
	else
		VFMW_SRC += memory/normal/vcodec_vdec_memory.c
	endif

	ifeq ($(VFMW_PROC_SUPPORT),y)
		VFMW_SRC += proc/vcodec_proc_fpga.c
	endif
	VFMW_SRC += dev_mgr/scd/vcodec_scd_fpga.c
	VFMW_SRC += dev_mgr/bsp/vcodec_bsp_fpga.c
	VFMW_SRC += dev_mgr/pxp/vcodec_pxp_fpga.c
	VFMW_SRC += dev_mgr/util/vcodec_dev_mgr_fpga.c
else
	EXTRA_CFLAGS += -DVDEC_MCORE_ENABLE
	VFMW_SRC += vcodec_vdec_ioctl_mcu.c
	VFMW_SRC += dev_mgr/mcu/vcodec_mcu_dev.c
	VFMW_SRC += dev_mgr/mcu/vcodec_mcu_hal.c
	VFMW_SRC += dev_mgr/mcu/vcodec_mcu_intf.c
	VFMW_SRC += dev_mgr/mcu/vcodec_mcu_ipc.c
	VFMW_SRC += dev_mgr/mcu/vcodec_mcu_queue.c

	VFMW_SRC += irq/normal/vcodec_vdec_irq.c
	VFMW_SRC += memory/normal/vcodec_vdec_memory.c

	ifeq ($(VFMW_PROC_SUPPORT),y)
		VFMW_SRC += proc/vcodec_proc_formal.c
	endif
	VFMW_SRC += dev_mgr/scd/vcodec_scd_dev.c
	VFMW_SRC += dev_mgr/scd/vcodec_scd_hal.c
	VFMW_SRC += dev_mgr/bsp/vcodec_bsp_dev.c
	VFMW_SRC += dev_mgr/bsp/vcodec_bsp_hal.c
	VFMW_SRC += dev_mgr/bsp/vcodec_bsp_sw_data.c
	VFMW_SRC += dev_mgr/pxp/vcodec_pxp_dev.c
	VFMW_SRC += dev_mgr/pxp/vcodec_pxp_hal.c
	VFMW_SRC += dev_mgr/util/vcodec_dev_mgr_formal.c
endif

################################## compile flags ##############################
EXTRA_CFLAGS += -fno-pic

ifeq ($(CONFIG_VCODEC_VDEC),m)
EXTRA_CFLAGS += -DENABLE_VDEC_MODULE
endif

ifeq ($(CONFIG_VCODEC_DEBUG),y)
EXTRA_CFLAGS += -DENABLE_VDEC_PROC
endif

EXTRA_CFLAGS += -DVCODEC_TVP_SUPPORT

EXTRA_CFLAGS += -DSCD_TIME_OUT=800
EXTRA_CFLAGS += -DSCD_FPGA_TIME_OUT=20000
EXTRA_CFLAGS += -DVDH_TIME_OUT=$(VFMW_VDH_TIME_OUT)
EXTRA_CFLAGS += -DVDH_FPGA_TIME_OUT=200000
EXTRA_CFLAGS += -DVDH_ONE_FRM_PERF=$(VFMW_VDH_ONE_FRM_PERF)
EXTRA_CFLAGS += -DVDH_FPGA_ONE_FRM_PERF=$(VFMW_VDH_FPGA_ONE_FRM_PERF)
EXTRA_CFLAGS += -DVDEC_CLK_RATE_LEVEL_MAX=$(VDEC_CLK_RATE_LEVEL_MAX)

################### include path ###########################
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/common/lowpm
EXTRA_CFLAGS += -I$(srctree)/platform_source/vcodec/vdec/vdec8.0
EXTRA_CFLAGS += -I$(srctree)/platform_source/vcodec/vdec/vdec8.0/include
EXTRA_CFLAGS += -I$(srctree)/platform_source/vcodec/vdec/vdec8.0/smmu
EXTRA_CFLAGS += -I$(srctree)/platform_source/vcodec/vdec/vdec8.0/osal
EXTRA_CFLAGS += -I$(srctree)/platform_source/vcodec/vdec/vdec8.0/osal/$(VCODEC_PLATFORM_NAME)
EXTRA_CFLAGS += -I$(srctree)/platform_source/vcodec/vdec/vdec8.0/product
EXTRA_CFLAGS += -I$(srctree)/platform_source/vcodec/vdec/vdec8.0/product/$(PRODUCT_VER)
EXTRA_CFLAGS += -I$(srctree)/platform_source/vcodec/vdec/vdec8.0/dev_mgr
EXTRA_CFLAGS += -I$(srctree)/platform_source/vcodec/vdec/vdec8.0/dev_mgr/util
EXTRA_CFLAGS += -I$(srctree)/platform_source/vcodec/vdec/vdec8.0/irq
EXTRA_CFLAGS += -I$(srctree)/platform_source/vcodec/vdec/vdec8.0/memory
EXTRA_CFLAGS += -I$(srctree)/platform_source/vcodec/vdec/vdec8.0/regulator
EXTRA_CFLAGS += -I$(srctree)/platform_source/vcodec/vdec/vdec8.0/tvp
EXTRA_CFLAGS += -I$(srctree)/platform_source/vcodec/vdec/vdec8.0/tzsp_client
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/tzdriver

ifeq ($(VCODEC_SOC_CHIP_TYPE), )
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/$(VCODEC_SOC_CHIP_NAME)
else
EXTRA_CFLAGS += -I$(srctree)/drivers/platform_drivers/ap/platform/$(VCODEC_SOC_CHIP_NAME)_$(VCODEC_SOC_CHIP_TYPE)
endif

ifeq ($(VFMW_PROC_SUPPORT),y)
EXTRA_CFLAGS += -I$(srctree)/platform_source/vcodec/vdec/vdec8.0/proc
endif

VFMW_OBJ := $(VFMW_SRC:%.c=%.o)
obj-$(CONFIG_VCODEC_VDEC) += $(VFMW_OBJ)
