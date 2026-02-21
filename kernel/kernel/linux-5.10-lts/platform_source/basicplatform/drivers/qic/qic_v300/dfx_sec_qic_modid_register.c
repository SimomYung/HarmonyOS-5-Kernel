/*
 * QIC v300 modid register functions source file.
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
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/limits.h>
#include <linux/list.h>
#include <linux/spinlock_types.h>
#include "dfx_sec_qic_err_probe.h"
#include "dfx_sec_qic_modid_register.h"
#include "soc_qic_bus.h"
#include "platform_include/basicplatform/linux/dfx_qic_v300_modid_para.h"
#include <mntn_public_interface.h>

static LIST_HEAD(__qic_modid_list);
static DEFINE_SPINLOCK(__qic_modid_list_lock);
static volatile s32 mod_cnt = 0;

static s32 dfx_qic_cnt_check(s32 mod_cnt)
{
	if (mod_cnt > QIC_REGISTER_LIST_MAX_LENGTH)
		return -1;
	else
		return 0;
}
static u32 dfx_qic_check_para_registerd(u32 mid_start, u32 mid_end, u32 bus)
{
	struct qic_modid_trans_s *p_modid_e = NULL;
	struct list_head *cur = NULL;
	struct list_head *next = NULL;
	ulong lock_flag;

	spin_lock_irqsave(&__qic_modid_list_lock, lock_flag);

	list_for_each_safe(cur, next, &__qic_modid_list) {
		p_modid_e = list_entry(cur, struct qic_modid_trans_s, s_list);
		if (!p_modid_e) {
			pr_err("It might be better to look around here. %s:%d\n", __func__, __LINE__);
			continue;
		}

		if (((mid_end >= p_modid_e->mid_start) && (mid_start <= p_modid_e->mid_end)) &&
			(bus & p_modid_e->bus)) {
			spin_unlock_irqrestore(&__qic_modid_list_lock, lock_flag);
			return QIC_MODID_EXIST;
		}
	}

	spin_unlock_irqrestore(&__qic_modid_list_lock, lock_flag);

	return QIC_MODID_NOT_EXIST;
}

static void __qic_modid_register(struct qic_modid_trans_s *node)
{
	ulong lock_flag;

	spin_lock_irqsave(&__qic_modid_list_lock, lock_flag);
	mod_cnt++;
	list_add_tail(&(node->s_list), &__qic_modid_list);
	spin_unlock_irqrestore(&__qic_modid_list_lock, lock_flag);
}

/*
 * qic modid registe API,for use to registe own process, and before this,
 * qic err use the AP_S_NOC process, after adapt,user can define his own process.
 */
void dfx_qic_modid_register(u32 mid_start, u32 mid_end, u32 bus, u32 modid)
{
	struct qic_modid_trans_s *node = NULL;
	s32 pret;
	u32 ret;

	pret = dfx_qic_cnt_check(mod_cnt);
	if (pret) {
		pr_err("%s node is full!\n", __func__);
		return;
	}

	if (mid_start > mid_end) {
		pr_err("%s qic mid_start should be less than or equal to mid_end\n", __func__);
		return;
	}

	if (!(bus & QIC_BUS_ID_MASK)) {
		pr_err("%s qic bus[%d] is not support\n", __func__, bus);
		return;
	}

	/*
	 * before registering modid,we have to check that has modid been registered
	 * berore,double check.
	 */
	ret = dfx_qic_check_para_registerd(mid_start, mid_end, bus);
	if (ret == QIC_MODID_EXIST) {
		pr_err("%s node is exist!\n", __func__);
		return;
	}

	node = kzalloc(sizeof(struct qic_modid_trans_s), GFP_ATOMIC);
	if (node == NULL)
		return;

	node->mid_start = mid_start;
	node->mid_end   = mid_end;
	node->bus       = bus;
	node->modid     = modid;
	pr_info("[%s]: modid register master id[%d:%d] !\n", __func__, node->mid_start, node->mid_end);
	pr_info("[%s]: modid register bus = 0x%x !\n", __func__, node->bus);
	pr_info("[%s]: modid register modid = 0x%x !\n", __func__, node->modid);
	pr_info("[%s]: modid register node is ok !\n", __func__);

	/*
	 *  this func is the real func to registe the user's modid and
	 *  user's err judge
	 */
	__qic_modid_register(node);
}
EXPORT_SYMBOL(dfx_qic_modid_register);

#define QIC_ALL_BUS_MASK 0xFFFF
#ifdef CONFIG_DFX_SEC_QIC_V300_ADVANCED
#define SOC_MODEM_MID_START 0x80
#define SOC_MODEM_MID_END 0xBF

void dfx_qic_mdm_modid_register(unsigned int modid)
{
	dfx_qic_modid_register(SOC_IPF_MID, SOC_IPF_MID, 1 << QIC_MDMPERI, modid);
	dfx_qic_modid_register(SOC_SOCP_MID, SOC_SOCP_MID, 1 << QIC_MDMPERI, modid);
	dfx_qic_modid_register(SOC_SPE_MID, SOC_MAA_MID, 1 << QIC_MDMPERI, modid);
	dfx_qic_modid_register(SOC_EICC_MID, SOC_EICC_MID, 1 << QIC_MDMPERI, modid);
	/* when modem write/read to cause qic, it should call modem modid */
	dfx_qic_modid_register(SOC_MODEM_MID_START, SOC_MODEM_MID_END, QIC_ALL_BUS_MASK, modid);
}
EXPORT_SYMBOL(dfx_qic_mdm_modid_register);
#endif

#ifdef CONFIG_DFX_SEC_QIC_V300_MDM_MODID_REGISTER
#define SOC_MODEM_MID_START SOC_TSP_DCC_THREAD0_MID
#define SOC_MODEM_MID_END SOC_TSP_PCC_THREAD2_MID
void dfx_qic_mdm_modid_register(unsigned int modid)
{
	dfx_qic_modid_register(SOC_SOCP_MID, SOC_EICC_MID, 1 << QIC_MDM_PERI, modid);
	dfx_qic_modid_register(SOC_SNE_MID, SOC_MAA_MID, 1 << QIC_MDM_PERI, modid);
	/* when modem write/read to cause qic, it should call modem modid */
	dfx_qic_modid_register(SOC_MODEM_MID_START, SOC_MODEM_MID_END, QIC_ALL_BUS_MASK, modid);
}
EXPORT_SYMBOL(dfx_qic_mdm_modid_register);
#endif

static u32 __qic_find_modid(u32 qic_busid, u32 qic_mid)
{
	struct qic_modid_trans_s *p_modid_e = NULL;
	struct list_head *cur = NULL;
	struct list_head *next = NULL;
	ulong lock_flag;
	u32 ret = QIC_MODID_NEGATIVE;

	spin_lock_irqsave(&__qic_modid_list_lock, lock_flag);

	list_for_each_safe(cur, next, &__qic_modid_list) {
		p_modid_e = list_entry(cur, struct qic_modid_trans_s, s_list);
		if (((1 << qic_busid) & p_modid_e->bus) &&
				((qic_mid >= p_modid_e->mid_start) && (qic_mid <= p_modid_e->mid_end))) {
			pr_err("[%s]: modid register info, mid[%d:%d] bus[0x%x] modid[0x%x]!\n", __func__,
						p_modid_e->mid_start, p_modid_e->mid_end, p_modid_e->bus, p_modid_e->modid);
			ret = p_modid_e->modid;
		}
	}
	spin_unlock_irqrestore(&__qic_modid_list_lock, lock_flag);

	return ret;
}

u32 dfx_qic_modid_find(u32 qic_busid, u32 qic_mid)
{
	u32 ret = QIC_MODID_NEGATIVE;

	if (qic_busid >= QIC_BUS_ID_MAX)
		return ret;

	return __qic_find_modid(qic_busid, qic_mid);
}
