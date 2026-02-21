/*
 * QIC V300 err probe functions source file.
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include <linux/io.h>
#include <linux/smp.h>
#include <linux/slab.h>
#include <linux/arm-smccc.h>
#include <linux/limits.h>
#include <linux/kernel.h>
#include "dfx_sec_qic_err_probe.h"
#include "dfx_sec_qic_modid_register.h"
#include "dfx_sec_qic_masterid_info.h"
#include "platform_include/see/bl31_smc.h"
#include "platform_include/basicplatform/linux/pr_log.h"
#include "platform_include/basicplatform/linux/util.h"
#include "platform_include/basicplatform/linux/rdr_platform.h"
#include "platform_include/basicplatform/linux/rdr_pub.h"
#include "soc_pctrl_interface.h"
#ifdef CONFIG_DFX_HW_DIAG
#include <platform_include/basicplatform/linux/dfx_hw_diag.h>
#endif
#include <mntn_public_interface.h>
#include "soc_qic_bus.h"
#ifdef CONFIG_PLATDRV_SUPPORT_FFA
#include "platform_include/see/ffa/ffa_plat_drv.h"
#endif
#include "npu_soc_defines.h"
#include "platform_include/basicplatform/linux/iommu/mm_svm.h"
#include "platform_include/basicplatform/linux/npu_rdr_error_type.h"

#define PR_LOG_TAG QIC_TAG

#ifdef CONFIG_DFX_SEC_QIC_V300_MODID_REGISTER
static u32 g_qic_modid[QIC_MODID_NUM_MAX] =  { [0 ... (QIC_MODID_NUM_MAX - 1)] = QIC_MODID_NEGATIVE };
static u32 g_qic_modid_idx;
#endif

const char *g_qic_opc_type[] = {
	"Read Normal Access",
	"Read Single Access",
	"Reserved",
	"Reserved",
	"Write Normal Access",
	"Write Single Access",
	"Reserved"
};

const char *g_qic_err_type[] = {
	"OKAY: normal access success or Exclusive fail",
	"EXOKAY: Exclusive access success",
	"SLVERR: Slave receive Error response or Decoder Error",
	"DECERR",
	"TMOERR: accessed slave timeout",
	"SECERR: Firewall intercpet and return SLVERR",
	"HIDEERR: Firewall intercpet and not return SLVERR",
	"DISCERR: access powerdown area",
	"UNSERR: received unsupported Opcode",
	"SGLERR",
	"PERITY ERR",
	"ECC ERR",
	"Invalid value"
};

#ifdef CONFIG_DFX_SEC_QIC_V300_ADVANCED
const char *g_interrupt_info[] = {
	"reserved",
	"qic_m1_comb_intr_lpmcu_acpu",
	"qic_m2_comb_intr_lpmcu_acpu",
	"qic_npu_comb_intr_lpmcu_acpu",
	"intr_qice_qic_ns_to_iomcu_lpmcu_mdm_gic",
	"intr_qic_probe_hsdt0_qic",
	"intr_qic_probe_hsdt1_qic",
	"intr_qic_probe_gpu_qic",
	"intr_qic_probe_cpu_qic",
	"intr_qic_probe_cfg_dma",
	"intr_qic_probe_crossbar",
	"intr_qic_probe_modemperi",
	"qic_ao_comb_intr_lpmcu_acpu",
	"intr_qic_probe_gpu_data_qic",
	"qic_nputs_comb_intr_lpmcu_acpu",

};
#else
const char *g_interrupt_info[] = {
	"intr_qic_probe_gpudatabus_subchip",
	"intr_qic_probe_gpucfgbus_subchip",
	"qic_m2_comb_intr_lpmcu_acpu",
	"qic_npu_comb_intr_lpmcu_acpu",
	"reserved",
	"intr_qic_probe_hsdt0_qic",
	"intr_qic_probe_hsdt1_qic",
	"intr_qic_probe_center_crossbar",
	"intr_qic_safety_err_sys",
	"intr_qic_safety_err_cfg_dma",
	"intr_qic_probe_cpu_qic",
	"intr_qic_probe_cfg_dma",
	"intr_qic_probe_sysbus",
	"intr_qic_probe_m1",
	"qic_ao_comb_intr_lpmcu_acpu",
	"reserved",
};
#endif

const char *g_safe_flag[] = {
	"Sec",
	"Nonsec",
};

static u32 g_master_id = 0;
bool g_ignore_npu_smmu = false;

#ifdef CONFIG_DFX_HW_DIAG
static bool g_qic_diaginfo_saved = false;
union dfx_hw_diag_info g_hwdiag_qic_info = {0};
#endif

/* opc_type value 0,1,4,5 is valid, other is reserved */
static const char *get_opc_info(u32 opc_type)
{
	if (opc_type < OPC_TYPE_MAX)
		return g_qic_opc_type[opc_type];
	return g_qic_opc_type[OPC_TYPE_MAX];
}
static const char *get_err_type_info(u32 err_type)
{
	if (err_type < ERR_TYPE_MAX)
		return g_qic_err_type[err_type];
	return g_qic_err_type[ERR_TYPE_MAX];
}

static u64 get_start_addr(struct qic_access_info *access_info, struct sec_qic_dfx_info *dfx_info)
{
	u64 start_addr;
	u32 start_addr_high;
	u32 start_addr_low;
	__be32 *start_addr_head = (access_info->tb_info)[dfx_info->tbid].start_addr_head;

	if (dfx_info->tbid >= access_info->tb_num ||
			dfx_info->mmidx >= (access_info->tb_info)[dfx_info->tbid].mmidx_num)
		return ADDR_ERR;

	start_addr_high = be32_to_cpup(start_addr_head + dfx_info->mmidx * 2);
	start_addr_low  = be32_to_cpup(start_addr_head + dfx_info->mmidx * 2 + 1);

	start_addr = (((u64)start_addr_high << 32) | (u64)start_addr_low);

	return start_addr;
}

static char *get_tb_name(struct qic_access_info *access_info, struct sec_qic_dfx_info *dfx_info)
{
	u32 tb_id = dfx_info->tbid;
	u32 tb_num = access_info->tb_num;
	char *tb_invalid = "invalid value";

	if (tb_id < tb_num)
		return (access_info->tb_info)[tb_id].tb_name;
	return tb_invalid;
}

static char *get_ib_name(struct qic_access_info *access_info, struct sec_qic_dfx_info *dfx_info)
{
	u32 ib_id = dfx_info->ibid;
	u32 ib_num = access_info->ib_num;
	char *ib_invalid = "invalid value";

	if (ib_id < ib_num)
		return (access_info->ib_name)[dfx_info->ibid];
	return ib_invalid;
}

static u64 get_err_addr(struct qic_access_info *access_info, struct sec_qic_dfx_info *dfx_info)
{
	u64 start_addr;
	u64 addr_offset;
	u64 err_addr;

	start_addr = get_start_addr(access_info, dfx_info);
	addr_offset = ((u64)dfx_info->addr_low32 | ((u64)dfx_info->addr_high32 << 32));
	if (start_addr == ADDR_ERR) {
		err_addr = addr_offset;
	} else {
		err_addr = start_addr + addr_offset;
	}

	return err_addr;
}

static char *get_mid_name(const struct dfx_sec_qic_device *qic_dev, struct sec_qic_dfx_info *dfx_info)
{
	u32 mid = dfx_info->master_id;
	u32 i;

	for (i = 0; i < qic_dev->mid_info_num; i++) {
		if ((mid >= (qic_dev->mid_info)[i].start) && (mid <= (qic_dev->mid_info)[i].end))
			return (qic_dev->mid_info)[i].mid_name;
	}

	return NULL;
}

static void dfx_sec_qic_print_dfx_info(const struct dfx_sec_qic_device *qic_dev,
								u32 position, u32 index, struct sec_qic_dfx_info *dfx_info)
{
	u32 bus_id;
	struct qic_access_info *access_info;

	bus_id = ((qic_dev->sub_intr_info)[position].em_info)[index].qic_bus_id;
	access_info = (qic_dev->bus_info)[bus_id].access_info;

	pr_err("******qic_err occurred on %s******\n", (qic_dev->bus_info)[bus_id].bus_name);
	pr_err("bit %d: %s\n", position, g_interrupt_info[position]);
	pr_err("name        |value        |description\n");
	pr_err("opc_type    |0x%-10x |%s\n", dfx_info->opc_type, get_opc_info(dfx_info->opc_type));
	pr_err("err_type    |0x%-10x |%s\n", dfx_info->err_type, get_err_type_info(dfx_info->err_type));
	pr_err("tbid        |0x%-10x |%s\n", dfx_info->tbid, get_tb_name(access_info, dfx_info));
	pr_err("ibid        |0x%-10x |%s\n", dfx_info->ibid, get_ib_name(access_info, dfx_info));
	pr_err("mmidx       |0x%-10x |Start addr is 0x%llx\n", dfx_info->mmidx, get_start_addr(access_info, dfx_info));
	pr_err("master_id   |0x%-10x |%s\n", dfx_info->master_id, get_mid_name(qic_dev, dfx_info));
	pr_err("safe_flag   |0x%-10x |%s\n", dfx_info->safe_flag, g_safe_flag[dfx_info->safe_flag]);
	pr_err("tid         |0x%-10x \n", dfx_info->tid);
	pr_err("addr_low32  |0x%-10x \n", dfx_info->addr_low32);
	pr_err("addr_high32 |0x%-10x \n", dfx_info->addr_high32);

	pr_err("err_addr    |0x%llx \n", get_err_addr(access_info, dfx_info));

#ifdef CONFIG_DFX_HW_DIAG
	if (!g_qic_diaginfo_saved) {
		g_hwdiag_qic_info.qic_info.master = get_mid_name(qic_dev, dfx_info);
		g_hwdiag_qic_info.qic_info.err_address = get_err_addr(access_info, dfx_info);
		g_qic_diaginfo_saved = true;
	}
#endif
}

#define EM_IRQ_STATE 0x0008
#define EM_IRQ_CLEAR 0x000C
#define EM_ER_LOG0   0x0020
#define EM_ER_LOG1   0x0028
#define EM_ER_LOG2   0x002C
#define EM_ER_LOG3   0x0030
#define EM_ER_LOG4   0x0034
#define EM_ER_LOG5   0x0040
#define EM_ER_LOG6   0x0044
#define EM_ER_LOG7   0x0048

static void sec_qic_em_irq_clear(struct qic_em_info *em_info)
{
	u32 irq_state;
	u32 count = 100;

	while (count--) {
		writel_relaxed(0x1, em_info->em_intr_addr + EM_IRQ_CLEAR);
		irq_state = readl_relaxed(em_info->em_intr_addr + EM_IRQ_STATE);
		if (irq_state == 0) {
			pr_info("%s: clear irq %d times succ\n", em_info->em_name, 100 - count);
			return;
		}
	}

	pr_err("%s: clear irq fail\n", em_info->em_name);
}

static void sec_qic_em_handler(struct qic_em_info *em_info, u32 data[], u32 len)
{
	u32 irq_state;

	irq_state = readl_relaxed(em_info->em_intr_addr + EM_IRQ_STATE);
	if (irq_state == 0) {
		data[0] = EM_NO_IRQ;
		return;
	}

	data[0] = readl_relaxed(em_info->em_intr_addr + EM_ER_LOG0);
	data[1] = readl_relaxed(em_info->em_intr_addr + EM_ER_LOG1);
	data[2] = readl_relaxed(em_info->em_intr_addr + EM_ER_LOG2);
	data[3] = readl_relaxed(em_info->em_intr_addr + EM_ER_LOG3);
	data[4] = readl_relaxed(em_info->em_intr_addr + EM_ER_LOG4);
	data[5] = readl_relaxed(em_info->em_intr_addr + EM_ER_LOG5);
	data[6] = readl_relaxed(em_info->em_intr_addr + EM_ER_LOG6);
	data[7] = readl_relaxed(em_info->em_intr_addr + EM_ER_LOG7);

	sec_qic_em_irq_clear(em_info);
}

static int dfx_qic_ath_get_errlog_info(const struct dfx_sec_qic_device *qic_dev,
				u32 em_id, u32 err_log[], u32 len)
{
	struct qic_em_info *em_info = (qic_dev->sub_intr_info)[em_id].em_info;

	sec_qic_em_handler(em_info, err_log, len);
	if (err_log[0] == EM_NO_IRQ) {
		pr_err("%s: info none for no irq\n", em_info->em_name);
		return -1;
	}

	pr_info("%s: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", em_info->em_name,
		err_log[0], err_log[1], err_log[2], err_log[3],
		err_log[4], err_log[5], err_log[6], err_log[7]);

	return 0;
}

#define SH_CTRL_ID 2
static int dfx_qic_xctrl_chk(const struct dfx_sec_qic_device *qic_dev, u32 id)
{
	u32 clk_state, rst_state;

	if (!qic_dev->irq_chk)
		return 0;

	if (qic_dev->qic_ath && id == SH_CTRL_ID)
		return 0;

	clk_state = readl_relaxed(qic_dev->irq_chk[id].clk_reg);
	if ((clk_state & qic_dev->irq_chk[id].clk_mask) == 0) {
		pr_err("xctrl[%u] clock not open\n", id);
		return -1;
	}

	rst_state = readl_relaxed(qic_dev->irq_chk[id].rst_reg);
	if (rst_state & qic_dev->irq_chk[id].rst_mask) {
		pr_err("xctrl[%u] not disreset\n", id);
		return -1;
	}

	return 0;
}

#ifdef CONFIG_PLATDRV_SUPPORT_FFA
static int dfx_sec_qic_ffa(u32 bus_id, u32 index, u32 data[], u32 len, u32 flag)
{
	int ret;
	struct ffa_send_direct_data args = {0};

	if (len != ERRLOG_NUM && len != POWER_STATUS_LEN)
		return -1;
	args.data0 = MNTN_QIC_FID_VALUE;
	args.data1 = bus_id;
	args.data2 = index;
	args.data3 = flag;
	ret = ffa_platdrv_send_msg_with_shmem(&args, (u8*)data, SHMEM_OUT);
	if (ret != 0) {
		pr_err("%s: ffa send fail:%d\n", __func__, ret);
		return ret;
	}
	return 0;
}
#else
static int dfx_sec_qic_smc(u32 bus_id, u32 index, u32 data[], u32 len, u32 flag)
{
	struct arm_smccc_1_2_regs args;
	struct arm_smccc_1_2_regs res;

	if (len != ERRLOG_NUM && len != POWER_STATUS_LEN)
		return -1;
	args.a0 = MNTN_QIC_FID_VALUE;
	args.a1 = bus_id;
	args.a2 = index;
	args.a3 = flag;
	arm_smccc_1_2_smc(&args, &res);
	if (len == ERRLOG_NUM) {
		data[0] = (res.a0 & 0xFFFFFFFF);
		data[1] = (res.a1 & 0xFFFFFFFF);
		data[2] = (res.a2 & 0xFFFFFFFF);
		data[3] = (res.a3 & 0xFFFFFFFF);
		data[4] = (res.a4 & 0xFFFFFFFF);
		data[5] = (res.a5 & 0xFFFFFFFF);
		data[6] = (res.a6 & 0xFFFFFFFF);
		data[7] = (res.a7 & 0xFFFFFFFF);
	} else {
		data[0] = (res.a0 & 0xFFFFFFFF);
	}
	return 0;
}
#endif
static u32 get_em_power_status(const struct dfx_sec_qic_device *qic_dev,
				u32 position, u32 index)
{
	u32 power;
	u32 bus_id;
	int ret;

	bus_id = ((qic_dev->sub_intr_info)[position].em_info)[index].qic_bus_id;
#ifdef CONFIG_PLATDRV_SUPPORT_FFA
	ret = dfx_sec_qic_ffa(bus_id, index, &power, POWER_STATUS_LEN, GET_POWER_STATUS);
#else
	ret = dfx_sec_qic_smc(bus_id, index, &power, POWER_STATUS_LEN, GET_POWER_STATUS);
#endif
	if (ret != 0) {
		pr_err("%s: qic ffa or smc fail:%d\n", __func__, ret);
		return EM_POWER_OFF;
	}
	return power;
}

static int dfx_sec_qic_em_irq_check(const struct dfx_sec_qic_device *qic_dev,
				u32 position, u32 index)
{
	u32 value;
	u32 irq_exit;
	void __iomem * em_intr_addr;
	u32 em_power;
	u32 bus_id = ((qic_dev->sub_intr_info)[position].em_info)[index].qic_bus_id;
	/*
		NONSEC_STATUS52: It can be used to determine which bus of the MSPC, ASP, IOMCU, or AO has an interrupt.
		bit0：intr_ao_qic_probe_iomcu_lpmcu_gic
		bit1:intr_qic_probe_ao
		bit2:intr_qic_probe_asp
		bit3:intr_qic_probe_iomcu
		bit4: intr_qic_probe_mspc
		other:reserved
		NPU,hsdt0,MEDAI1,MEDAI2 bus has similar registers。
	*/
	// m1 m2 npu qice report to pctrl, theree is no need to check irq, ao bus need to check
#ifdef CONFIG_DFX_SEC_QIC_V300_ADVANCED
	if (position != SOC_PCTRL_PERI_STAT69_qic_ao_comb_intr_lpmcu_acpu_START && position != SOC_PCTRL_PERI_STAT69_intr_qic_probe_hsdt0_qic_START) 
		return EM_IRQ_EXIT;
#endif
	em_power = get_em_power_status(qic_dev, position, index);
	if (em_power == EM_POWER_OFF) {
		pr_err("%s em[%d]: info none for power off.\n", (qic_dev->bus_info)[bus_id].bus_name, index);
		return EM_IRQ_NONE;
	}
	em_intr_addr = ((qic_dev->sub_intr_info)[position].em_info)[index].em_intr_addr;
	irq_exit = ((qic_dev->sub_intr_info)[position].em_info)[index].em_intr_mask;
	value = readl_relaxed(em_intr_addr);
	if ((value & irq_exit) != irq_exit)
		return EM_IRQ_NONE;
	pr_info("%s em[%d] irq value is 0x%x mask is 0x%x\n",
		(qic_dev->bus_info)[bus_id].bus_name, index, value, irq_exit);

	return EM_IRQ_EXIT;
}

static int dfx_sec_qic_get_errlog_info(const struct dfx_sec_qic_device *qic_dev,
								u32 position, u32 index, u32 err_log[], u32 len)
{
	u32 bus_id;
	u32 em_base;
	u32 em_num;
	int ret;

	if (len < ERRLOG_NUM)
		return -1;
	bus_id = ((qic_dev->sub_intr_info)[position].em_info)[index].qic_bus_id;
	em_base = ((qic_dev->sub_intr_info)[position].em_info)[index].em_base_addr;
	em_num = (qic_dev->sub_intr_info)[position].qic_em_num;
	if (em_num > 1) {
		ret = dfx_sec_qic_em_irq_check(qic_dev, position, index);
		if (ret == EM_IRQ_NONE)
			return -1;
	}
#ifdef CONFIG_PLATDRV_SUPPORT_FFA
	ret = dfx_sec_qic_ffa(bus_id, index, err_log, len, GET_ERRLOG_INFO);
#else
	ret = dfx_sec_qic_smc(bus_id, index, err_log, len, GET_ERRLOG_INFO);
#endif
	if (ret != 0) {
		pr_err("%s: ffa or smc fail:%d\n", __func__, ret);
		return ret;
	}

	if (err_log[0] == EM_NO_IRQ) {
		pr_err("%s em[%d]: info none for no irq\n", (qic_dev->bus_info)[bus_id].bus_name, index);
		return -1;
	}

	if (err_log[0] == INVALID_PARAM) {
		pr_err("%s: invalid args bus_id %d index %d", (qic_dev->bus_info)[bus_id].bus_name, bus_id, index);
		return -1;
	}
	pr_info("%s em[%d] info: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
		(qic_dev->bus_info)[bus_id].bus_name, index,
		err_log[0], err_log[1], err_log[2], err_log[3],
		err_log[4], err_log[5], err_log[6], err_log[7]);

	return 0;
}

static void dfx_sec_qic_parse_dfx_info(struct sec_qic_dfx_info *dfx_info, u32 err_log[], u32 len)
{
	if (len < ERRLOG_NUM)
		return;

	dfx_info->opc_type    = (err_log[0] & OPC_MASK);
	dfx_info->err_type    = ((err_log[0] >> ERR_TYPE_OFFSET) & ERR_TYPE_MASK);
	dfx_info->tbid        = ((err_log[1] >> TBID_OFFSET) & TBID_MASK);
	dfx_info->ibid        = ((err_log[1] >> IBID_OFFSET) & IBID_MASK);
	dfx_info->mmidx       = ((err_log[1] >> MMIDX_OFFSET) & MMIDX_MASK);
	dfx_info->tid         = err_log[2];
	dfx_info->addr_low32  = err_log[3];
	dfx_info->addr_high32 = err_log[4];
	dfx_info->master_id   = ((err_log[5] >> MASTER_ID_OFFSET) & MASTER_ID_MASK);
	dfx_info->safe_flag   = ((err_log[7] >> SAFE_OFFSET) & SAFE_MASK);
}

#ifdef CONFIG_DFX_SEC_QIC_V300_MODID_REGISTER
static void dfx_sec_register_reset_ops(const struct dfx_sec_qic_device *qic_dev,
								u32 position, u32 index, struct sec_qic_dfx_info *dfx_info)
{
	u32 bus_id;
	u32 mid;
	u32 modid;

	bus_id = ((qic_dev->sub_intr_info)[position].em_info)[index].qic_bus_id;
	mid = dfx_info->master_id;

	modid = dfx_qic_modid_find(bus_id, mid);
	if ((modid != QIC_MODID_NEGATIVE) && (g_qic_modid_idx < QIC_MODID_NUM_MAX))
		g_qic_modid[g_qic_modid_idx++] = modid;
	if (modid == RDR_EXC_BYPASS_TYPE_NOC_NPU7 &&
		mid == SOC_MID_NPU_SMMU_TCU &&
		!is_npu_svm_eventb_received()) {
		g_ignore_npu_smmu = true;
		pr_err("[%s] ignore error of npu smmu due to svm_eventb not received!\n", __func__);
	}
}
#endif

static void dfx_sec_qic_parse_process(const struct dfx_sec_qic_device *qic_dev, u32 position)
{
	u32 dfx_info_num = (qic_dev->sub_intr_info)[position].qic_em_num;
	struct sec_qic_dfx_info dfx_info;
	u32 err_log[ERRLOG_NUM] = {0};
	u32 index;
	int ret;

	if (!dfx_info_num || dfx_info_num > MAX_QIC_DFX_INFO) {
		pr_err("[%s] error monitor number error!\n", __func__);
		return;
	}

	pr_err("start to show qic error monitor info!\n");
	for (index = 0; index < dfx_info_num; index++) {
		ret = qic_dev->qic_ath ?
			dfx_qic_ath_get_errlog_info(qic_dev, position, err_log, ERRLOG_NUM) :
			dfx_sec_qic_get_errlog_info(qic_dev, position, index, err_log, ERRLOG_NUM);
		if (ret)
			continue;
		dfx_sec_qic_parse_dfx_info(&dfx_info, err_log, ERRLOG_NUM);
		dfx_sec_qic_print_dfx_info(qic_dev, position, index, &dfx_info);
#ifdef CONFIG_DFX_SEC_QIC_V300_MODID_REGISTER
		dfx_sec_register_reset_ops(qic_dev, position, index, &dfx_info);
#endif
	}
	g_master_id = dfx_info.master_id;
	pr_err("end to show qic error monitor info!\n");
}

void dfx_sec_qic_reset_handler(const struct dfx_sec_qic_device *qic_dev)
{
#ifdef CONFIG_DFX_BB
	u32 modid_match_flag = 0;
#ifdef CONFIG_DFX_SEC_QIC_V300_MODID_REGISTER
	u32 i;
#endif
	u32 sub_reason;

	if (g_ignore_npu_smmu) {
		g_ignore_npu_smmu = false;
		return;
	}
	if (check_mntn_switch(MNTN_NOC_ERROR_REBOOT)) {
#ifdef CONFIG_DFX_SEC_QIC_V300_MODID_REGISTER
		for (i = 0; i < g_qic_modid_idx; i++) {
			if (g_qic_modid[i] != QIC_MODID_NEGATIVE) {
				modid_match_flag = 1;
				pr_err("[%s] qic modid is matched, modid = 0x%x!\n", __func__, g_qic_modid[i]);
				rdr_system_error(g_qic_modid[i], 0, 0);
			}
		}
#endif
		if (modid_match_flag == 0) {
			pr_err("%s qic reset begin\n", __func__);
			sub_reason = get_mid_info_sub_reason(qic_dev, g_master_id);
			rdr_syserr_process_for_ap(sub_reason, 0, 0);
		}
	}
#endif
}

#define QIC_BUS_KEY_ALL 0xFF
static void dfx_qic_show_xctrl(u32 bus_id)
{
#ifdef CONFIG_PLATDRV_SUPPORT_FFA
	int        ret;
	struct ffa_send_direct_data args = {0};

	args.data0 = MNTN_QIC_FID_VALUE;
	args.data1 = bus_id;

	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0)
		pr_err("%s:qic:ffa send fail:%d\n", __func__, ret);
#endif
}

int dfx_sec_qic_get_errinfo(const struct dfx_sec_qic_device *qic_dev, bool from_irq)
{
	ulong position;
	ulong value = 0;
	u32 i;
	u32 j;
	u32 emid = 0;

	if (!qic_dev) {
		pr_err("[%s] qic_init fail!\n", __func__);
		return -1;
	}

#ifdef CONFIG_DFX_SEC_QIC_V300_MODID_REGISTER
	g_qic_modid_idx = 0;
#endif

	for (i = 0; i < qic_dev->irq_reg_num; i++) {
		for (j = 0; j < qic_dev->irq_reg[i].len; j++, emid++) {
			if (dfx_qic_xctrl_chk(qic_dev, i))
				continue;

			if (qic_dev->qic_ath && i == SH_CTRL_ID) {
				value = 1;
			} else {
				value = readl_relaxed(qic_dev->irq_reg[i].reg_base +
					qic_dev->irq_reg[i].offsets[j]);
			}
			pr_err("qic irq status:0x%lx\n", value);
			if (from_irq && value == 0)
				dfx_qic_show_xctrl(QIC_BUS_KEY_ALL);
			value = value & qic_dev->irq_reg[i].reg_mask[j];
			if (qic_dev->qic_ath) {
				if (value == 0)
					continue;
				pr_err("irq emid:%u\n", emid);
				dfx_sec_qic_parse_process(qic_dev, emid);
				continue;
			}
			for_each_set_bit(position, &value, BITS_PER_LONG) {
				pr_err("irq bit:%lu\n", position);
				dfx_sec_qic_parse_process(qic_dev, position);
			}
		}
	}

#ifdef CONFIG_DFX_HW_DIAG
	if (g_qic_diaginfo_saved) {
		g_hwdiag_qic_info.qic_info.buskey = value;
		dfx_hw_diaginfo_trace(QIC_FAULT_INFO, &g_hwdiag_qic_info);
		g_qic_diaginfo_saved = false;
	}
#endif

	return 0;
}

