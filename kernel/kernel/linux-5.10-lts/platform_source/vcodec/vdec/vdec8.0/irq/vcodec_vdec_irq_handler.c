/*
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include "dbg.h"
#include "vcodec_vdec.h"
#include "vcodec_vdec_plat.h"
#include "vcodec_vdec_irq_handler.h"
#include "vcodec_osal.h"

#define EM_ERR_LOG_NUM   8
#define CFG_QIC_START_ADDR      0x004C0000
#define CORE_CFG_QIC_START_ADDR 0x004A0000

typedef enum {
	QIC_CFG_TYPE,            /* qic_cfg err */
	QIC_CORE_CFG_TYPE,       /* qic_core_cfg err */
	QIC_CORE_DATA_TYPE,      /* qic_core_data err */
} others_isr_type;

irqreturn_t vdec_bsp_isr(int irq, void *id)
{
	vdec_entry *vdec = vdec_get_entry();	
	return vdec->bsp_dev->irq_proc(vdec->bsp_dev);
}

irqreturn_t vdec_pxp_isr(int irq, void *id)
{
	vdec_entry *vdec = vdec_get_entry();
	return vdec->pxp_dev->irq_proc(vdec->pxp_dev);
}

irqreturn_t vdec_scd_isr(int irq, void *id)
{
	vdec_entry *vdec = vdec_get_entry();
	return vdec->scd_dev->irq_proc(vdec->scd_dev);
}

irqreturn_t vdec_watch_dog_isr(int irq, void *id)
{
	vdec_entry *vdec = vdec_get_entry();

	dprint(PRN_ALWS, "enter WatchDogClearInt");
	wr_vreg(vdec->reg_base_vir_addr, 0x00409000, 0x1AAEE533);
	wr_vreg(vdec->reg_base_vir_addr, 0x0040901C, 0x1);
	// close watchdog
	wr_vreg(vdec->reg_base_vir_addr, 0x00409014, 0x0);

	wr_vreg(vdec->reg_base_vir_addr, 0x00409004, 0x1);
	// set reset state for each process
	vdec_set_reset_state();
	vdec_sh_record_mcu_panic();
	dprint(PRN_ALWS, "exit WatchDogClearInt");
	return IRQ_HANDLED;
}

static const char *others_isr_name(others_isr_type isr_type)
{
	const char *name[] = {
		"QIC_CFG_TYPE",            /* qic_cfg err */
		"QIC_CORE_CFG_TYPE",       /* qic_core_cfg err */
		"QIC_CORE_DATA_TYPE",      /* qic_core_data err */
	};
	if (isr_type >= sizeof(name) / sizeof(name[0])) {
		return "OTHERS_ISR_UNKNOWN";
	}
	return name[isr_type];
}

static void vdec_subctrl_task_irq_clear(uint32_t status_reg_value)
{
	vdec_entry *vdec = vdec_get_entry();

	// INTR_CLR_NS_VDEC: write intr statu to clear intr_subctrl_xxx_task_wovfl
	wr_vreg(vdec->reg_base_vir_addr, 0x4080E0, status_reg_value);
}

static void vdec_qic_irq_info(others_isr_type isr_type, uint32_t start_addr)
{
	uint32_t mask_reg_value;
	uint32_t status_reg_value;
	uint32_t log_reg_value[EM_ERR_LOG_NUM];
	vdec_entry *vdec = vdec_get_entry();

	// intr mask reg: bit0: 1 is mask, 0 is not mask
	rd_vreg(vdec->reg_base_vir_addr, (start_addr + 0x0004), mask_reg_value);
	// intr status reg: bit0: intr original status
	rd_vreg(vdec->reg_base_vir_addr, (start_addr + 0x0008), status_reg_value);
	dprint(PRN_ALWS, "%s irq status:0x%x, mask:0x%x",
		others_isr_name(isr_type), status_reg_value, mask_reg_value);

	/* EM_ERR_LOG0-7: exception latch info reg
	  + EM_ERR_LOG0: 31-20:emp_cmd_tsize, 15-12:emp_cmd_status, 8:emp_cmd_excl, 7:emp_cmd_burst, 3-0:emp_cmd_opc
	  + EM_ERR_LOG1: 23-16:emp_cmd_ibid, 15-8:emp_cmd_tbid, 7-0:emp_cmd_mmidx, mmidx corresponds to base address
	  + EM_ERR_LOG3: lower 32bit offset address
	  + EM_ERR_LOG4: upper 32bit offset address
	  + EM_ERR_LOG5: 13-6:mid, 5-0:swid
	*/
	rd_vreg(vdec->reg_base_vir_addr, (start_addr + 0x0020), log_reg_value[0]);
	rd_vreg(vdec->reg_base_vir_addr, (start_addr + 0x0028), log_reg_value[1]);
	rd_vreg(vdec->reg_base_vir_addr, (start_addr + 0x002C), log_reg_value[2]);
	rd_vreg(vdec->reg_base_vir_addr, (start_addr + 0x0040), log_reg_value[5]);
	rd_vreg(vdec->reg_base_vir_addr, (start_addr + 0x0044), log_reg_value[6]);
	rd_vreg(vdec->reg_base_vir_addr, (start_addr + 0x0048), log_reg_value[7]);
	dprint(PRN_ALWS, "%s irq info log0-2:0x%x 0x%x 0x%x", others_isr_name(isr_type),
		log_reg_value[0], log_reg_value[1], log_reg_value[2]);
	dprint(PRN_ALWS, "%s irq info log5-7:0x%x 0x%x 0x%x", others_isr_name(isr_type),
		log_reg_value[5], log_reg_value[6], log_reg_value[7]);
}

void vdec_m1_ctrl_irq_info(void)
{
	return;
}

static void vdec_qic_irq_clear(uint32_t start_addr)
{
	vdec_entry *vdec = vdec_get_entry();

	// write 1 to clear intr_qic_xxx
	wr_vreg(vdec->reg_base_vir_addr, (start_addr + 0x000C), 0x1);
}

static void vdec_qic_cfg_irq_process(void)
{
	vdec_qic_irq_info(QIC_CFG_TYPE, CFG_QIC_START_ADDR);
	vdec_qic_irq_clear(CFG_QIC_START_ADDR);
}

static void vdec_qic_core_cfg_irq_process(void)
{
	vdec_plat_ops *ops = get_vdec_plat_ops();
	if (ops->is_open_qic_core_cfg_intr()) {
		vdec_qic_irq_info(QIC_CORE_CFG_TYPE, CORE_CFG_QIC_START_ADDR);
		vdec_qic_irq_clear(CORE_CFG_QIC_START_ADDR);
	}
}

static void vdec_qic_core_data_irq_process(void)
{
	vdec_plat_ops *ops = get_vdec_plat_ops();
	uint32_t start_addr = ops->get_qic_core_data_start_addr();
	vdec_qic_irq_info(QIC_CORE_DATA_TYPE, start_addr);
	vdec_qic_irq_clear(start_addr);
}

irqreturn_t vdec_others_isr(int irq, void *id)
{
	uint32_t mask_reg_value;
	uint32_t status_reg_value;
	vdec_entry *vdec = vdec_get_entry();
	vdec_plat_ops *ops = get_vdec_plat_ops();

	// INTR_MASK_NS_VDEC1: intr mask reg
	rd_vreg(vdec->reg_base_vir_addr, 0x4080B4, mask_reg_value);
	// INTR_NS_VDEC1: intr read reg
	rd_vreg(vdec->reg_base_vir_addr, 0x408088, status_reg_value);
	dprint(PRN_ALWS, "others irq status:0x%x, mask:0x%x", status_reg_value, mask_reg_value);

	vdec_subctrl_task_irq_clear(status_reg_value);

	vdec_qic_cfg_irq_process();
	vdec_qic_core_cfg_irq_process();
	vdec_qic_core_data_irq_process();
	vdec_m1_ctrl_irq_info();
	// media1_crg
	ops->get_media1_crg(vdec->media1_crg_vir_addr);

	vdec->qic_irq_cnt++;
	if (vdec->qic_irq_cnt >= 10) {
		ops->set_qic_irq_mask(vdec->reg_base_vir_addr);
		dprint(PRN_ALWS, "close qic irq");
	}
	return IRQ_HANDLED;
}