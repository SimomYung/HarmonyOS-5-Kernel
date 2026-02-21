#include "vcodec_mcu_hal.h"
#include "vcodec_mcu.h"
#include "dbg.h"
#include "vdec_memory_map.h"
#include "vcodec_vdec_dbg.h"

int32_t vcodec_mcu_init_hal(struct vcodec_dev_mcu* dev, uint8_t *reg_base)
{
	dev->reg_base_vir_addr = reg_base;
	dev->subctrl_vir_addr =
		(S_VDE_SUB_CTRL_NS_REGS_TYPE *)(reg_base + OFFSET_OF_VDEC_BASE(SUB_CTRL_REG_BASE));

	return 0;
}

void vcodec_mcu_deinit_hal(struct vcodec_dev_mcu* dev)
{
	dev->reg_base_vir_addr = NULL;
	dev->subctrl_vir_addr = NULL;
}

static int32_t vdec_mcore_load_image(struct vcodec_dev_mcu* dev)
{
	int32_t ret;
	void *image = NULL;
	size_t size = 0;

	ret = kernel_read_file_from_path(VDEC_MCORE_IMG, 0,
		&(image), MCU_IMAGE_SIZE, &size, READING_FIRMWARE);
	if (ret < 0 || size == 0 || size > MCU_IMAGE_SIZE) {
		dprint(PRN_ERROR, "read vdec mcu image failed");
		return -EFAULT;
	}
	ret = memcpy_s(dev->mcu_exec_mem.virt_addr, MCU_IMAGE_SIZE, image, size);
	if (ret)
		dprint(PRN_ERROR, "load mcu image memcpy error");
	else
		dprint(PRN_ALWS, "load mcu image success, name:%s, size:%lu", VDEC_MCORE_IMG, size);
	vfree(image);
	mb();
	return ret;
}

int32_t vcodec_mcu_hal_poweron(struct vcodec_dev_mcu *dev, uint32_t load_image_flag)
{
	volatile S_VDE_SUB_CTRL_NS_REGS_TYPE *ctl = dev->subctrl_vir_addr;

	// 1、enable mcu clock
	{
		ctl->MCU_RST0.bits.mcu_por_rst = 1;
		ctl->MCU_RST0.bits.mcu_wdt_rst = 1;
	}

	{
		ctl->MCU_CRG1.bits.tim_clk_en = 1;
		ctl->MCU_CRG1.bits.ipc_clk_en = 1;
		ctl->MCU_CRG1.bits.uart_clk_en = 0;
	}

	// 2、config address
	ctl->MCU_POR_PC = MCU_ENTRY_ADDR;

	// 3 set mcu_instr_fetch_en = 1, then mcu enter wait state
	ctl->MCU_RST0.bits.mcu_core_wait = 1;
	ctl->MCU_CTRL0.bits.mcu_instr_fetch_en = 1;

	// 4 set mcu_por_rst = 0, then mcu exit reset state
	ctl->MCU_RST0.bits.mcu_por_rst = 0;
	ctl->MCU_RST0.bits.mcu_wdt_rst = 0;

	// 5 load mcu image to ddr by AP
	if (load_image_flag && vdec_mcore_load_image(dev) != 0) {
        dprint(PRN_ERROR, "load vdec mcu image to addr failed");
		return -EIO;
	}

	// 6 set mcu_instr_fetch_en = 0, then start mcu core
	ctl->MCU_RST0.bits.mcu_core_wait = 0;
	ctl->MCU_CTRL0.bits.mcu_instr_fetch_en = 0;
	return 0;
}

void vcodec_mcu_hal_poweroff(struct vcodec_dev_mcu *dev)
{
	dev->subctrl_vir_addr->MCU_RST0.bits.mcu_por_rst = 1;
	dev->subctrl_vir_addr->MCU_RST0.bits.mcu_wdt_rst = 1;
}

// for debug, print mcu current PC and status
void vcodec_mcu_hal_show_status(struct vcodec_dev_mcu *dev)
{
	S_VDE_SUB_CTRL_NS_REGS_TYPE *ctl = dev->subctrl_vir_addr;

	dprint(PRN_ALWS, "mcu current status:");
	dprint(PRN_ALWS, "MCU_DFX_CMT_PC0:0x%x", ctl->MCU_DFX_CMT_PC0);
	dprint(PRN_ALWS, "MCU_DFX_CMT_PC1:0x%x", ctl->MCU_DFX_CMT_PC1);
	dprint(PRN_ALWS, "MCU_DBG_SIGNAL0:0x%x", ctl->MCU_DBG_SIGNAL0.u32);
	dprint(PRN_ALWS, "mcu_dfx_cmt_gpr_idx1:%u", ctl->MCU_DBG_SIGNAL0.bits.mcu_dfx_cmt_gpr_idx1);
	dprint(PRN_ALWS, "mcu_dfx_cmt_gpr_idx0:%u", ctl->MCU_DBG_SIGNAL0.bits.mcu_dfx_cmt_gpr_idx0);
	dprint(PRN_ALWS, "mcu_dfx_cmt_gpr_vld1:%u", ctl->MCU_DBG_SIGNAL0.bits.mcu_dfx_cmt_gpr_vld1);
	dprint(PRN_ALWS, "mcu_dfx_cmt_gpr_vld0:%u", ctl->MCU_DBG_SIGNAL0.bits.mcu_dfx_cmt_gpr_vld0);
	dprint(PRN_ALWS, "mcu_top_dfx_cmt_vld1:%u", ctl->MCU_DBG_SIGNAL0.bits.mcu_top_dfx_cmt_vld1);
	dprint(PRN_ALWS, "mcu_top_dfx_cmt_vld0:%u", ctl->MCU_DBG_SIGNAL0.bits.mcu_top_dfx_cmt_vld0);
	dprint(PRN_ALWS, "mcu_in_double_excp_hdlr:%u", ctl->MCU_DBG_SIGNAL0.bits.mcu_in_double_excp_hdlr);
	dprint(PRN_ALWS, "mcu_dcsr_stoptime:%u", ctl->MCU_DBG_SIGNAL0.bits.mcu_dcsr_stoptime);
	dprint(PRN_ALWS, "mcu_ahbs_store_idle:%u", ctl->MCU_DBG_SIGNAL0.bits.mcu_ahbs_store_idle);
	dprint(PRN_ALWS, "mcu_in_async_excp_hdlr:%u", ctl->MCU_DBG_SIGNAL0.bits.mcu_in_async_excp_hdlr);
	dprint(PRN_ALWS, "mcu_in_nmi_hdlr:%u", ctl->MCU_DBG_SIGNAL0.bits.mcu_in_nmi_hdlr);
	dprint(PRN_ALWS, "mcu_debug_mode:%u", ctl->MCU_DBG_SIGNAL0.bits.mcu_debug_mode);
	dprint(PRN_ALWS, "mcu_sleep_mode:%u", ctl->MCU_DBG_SIGNAL0.bits.mcu_sleep_mode);
}

static int32_t vcodec_mcu_hal_is_enter_wfi(struct vcodec_dev_mcu *dev)
{
	uint32_t cnt = 0;
	volatile S_VDE_SUB_CTRL_NS_REGS_TYPE *ctl = dev->subctrl_vir_addr;

	for (cnt = 0; cnt < MCU_SUSPEND_WAIT_CNT; cnt++) {
		if (ctl->MCU_DBG_SIGNAL0.bits.mcu_sleep_mode == 1)
			break;
		msleep(1);
	}
	if (cnt == MCU_SUSPEND_WAIT_CNT) {
		dprint(PRN_ERROR, "wait mcu enter wfi timeout");
		return -EBUSY;
	}
	return 0;
}

int32_t vcodec_mcu_hal_wait_suspend_done(struct vcodec_dev_mcu *dev)
{
	return vcodec_mcu_hal_is_enter_wfi(dev);
}
