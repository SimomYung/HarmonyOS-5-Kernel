/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/skbuff.h>
#include "mdrv_timer.h"
#include "mdrv_msg.h"
#include "msg_id.h"
#include "rnic_log.h"
#include "rnic_private.h"
#include "rnic_nll2_msg_pif.h"
#include "rnic_pkterr.h"

STATIC struct rnic_pkterr_rel_ctx_s *rnic_pkterr_rel_ctx_ptr;

STATIC struct msg_addr rnic_nll2_chan_dst = {
	.core = MSG_CORE_TSP,
	.chnid = MSG_CHN_NLL2,
};

STATIC void rnic_pkterr_rel_rpt_err_ind(struct rnic_pkterr_rel_info_s *err_info,
					struct rx_cb_map_s *map, pfa_version_type ver,
					unsigned short rslt)
{
	struct rnic_nll2_msg_s msg = {0};

	msg.modem_id = (u32)err_info->modem_id;
	msg.pdusession_id = (u32)err_info->pdusession_id;

	msg.result_type = rslt;
	msg.msg_length = sizeof(msg);
	msg.msg_type = ID_RNIC_NLL2_PKTERR_IND;
	msg.rb_id = (map->userfield1 & RNIC_MAP_RB_ID_MASK) >> RNIC_MAP_RB_ID_OFFSET;
	msg.pfa_result = map->pfa_tft_result.u32;
	msg.sne_result = map->pfa_result.bits.acpu_pkt_rsn;
	msg.pdcp_count = (ver == PFA_VERSION_V200) ? map->userfield2 :
			  (map->userfield2 >> RNIC_MAP_PDCP_CNT_OFFSET);

	if(rnic_wwan_msg_snd(&rnic_nll2_chan_dst, &msg, sizeof(msg)))
		RNIC_LOGE("pkterr rel rpt failed");
}

void rnic_pkterr_rel_clear_err_data(unsigned int pif_id)
{
	struct rnic_pkterr_rel_ctx_s *pctx = rnic_pkterr_rel_ctx_ptr;
	struct rnic_pkterr_rel_info_s *err_info;

	if (pctx == NULL) {
		RNIC_LOGE("pkterr rel init is not ready");
		return;
	}

	if (pif_id >= RNIC_MAX_PIF_NUM) {
		RNIC_LOGE("clear_err_data: invalid pif_id");
		return;
	}

	err_info = &pctx->pkterr_info[pif_id];
	err_info->period_idx = 0;
	err_info->period_pkterr_num = 0;
	err_info->period_total_num = 0;
	err_info->total_pkterr_num = 0;

	__clear_bit(pif_id, (unsigned long *)&pctx->pif_bitmask);
}

void rnic_pkterr_rel_timeout(struct timer_list *t)
{
	struct rnic_pkterr_rel_ctx_s *pctx = rnic_pkterr_rel_ctx_ptr;
	struct rnic_pkterr_rel_info_s *err_info = NULL;
	struct rnic_pkterr_fea_cfg_s *fea_cfg = &pctx->fea_cfg;
	unsigned long *bitmasks = NULL;
	struct rx_cb_map_s map = { 0 };
	unsigned int bit;

	bitmasks = (unsigned long *)&pctx->pif_bitmask;
	for_each_set_bit(bit, bitmasks, (unsigned long)RNIC_MAX_PIF_NUM) {
		err_info = &pctx->pkterr_info[bit];
		if (err_info->total_pkterr_num >= fea_cfg->pkterr_num_threshhold) {
			/* rpt pkterr */
			rnic_pkterr_rel_rpt_err_ind(err_info, &map, mdrv_pfa_get_version(),
						    RNIC_PKTERR_RPT_ERR);
			rnic_pkterr_rel_clear_err_data(bit);
			continue;
		}
		/* reset the period (include period without packets) */
		if (err_info->period_total_num >=
		    (err_info->period_pkterr_num * RNIC_PKTERR_COEFFICIENT)) {
			err_info->period_idx = 0;
			err_info->period_pkterr_num = 0;
			err_info->period_total_num = 0;

			__clear_bit((bit), (unsigned long *)&pctx->pif_bitmask);
			continue;
		}
		/* goto the next period */
		err_info->period_idx++;
		err_info->period_pkterr_num = 0;
		err_info->period_total_num = 0;
		if (err_info->period_idx >= fea_cfg->pkterr_cycle) {
			/* rpt pkterr */
			rnic_pkterr_rel_rpt_err_ind(err_info, &map, mdrv_pfa_get_version(),
						    RNIC_PKTERR_RPT_ERR);
			rnic_pkterr_rel_clear_err_data(bit);
		}
	}

	if (pctx->pif_bitmask)
		mod_timer(&pctx->pkterr_timer,
			RNIC_PKTERR_TIME_TO_JIFFIES(pctx->fea_cfg.pkterr_period));
}

STATIC unsigned int rnic_pkterr_rel_judge_err(struct rx_cb_map_s *map,
					      unsigned int ipf_err_mask)
{
	int result;

	result = map->pfa_tft_result.u32 & ipf_err_mask;
	result |= map->pfa_result.bits.acpu_pkt_rsn == RNIC_SPE_L3_PORTOCOL_ERR;
	result |= map->pfa_result.bits.acpu_pkt_rsn == RNIC_SPE_L3_IPHDR_CHECK_ERR;
	result |= map->pfa_result.bits.acpu_pkt_rsn == RNIC_SPE_IP_IPHDR_LEN_ERR;

	return result;
}

STATIC void rnic_pkterr_rel_start_cur_period(struct rnic_pkterr_rel_info_s *err_info,
	struct rx_cb_map_s *map, pfa_version_type pfa_ver, int pifid)
{
	struct rnic_pkterr_rel_ctx_s *pctx = rnic_pkterr_rel_ctx_ptr;

	switch (pfa_ver) {
	case PFA_VERSION_V0:
		err_info->modem_id = map->pfa_tft_result.bits.modem_id;
		err_info->pdusession_id = map->pfa_tft_result.bits.pdu_session_id;
		break;
	case PFA_VERSION_V100:
		err_info->modem_id = map->pfa_tft_result.bits_v100.modem_id;
		err_info->pdusession_id = map->pfa_tft_result.bits_v100.pdu_session_id;
		break;
	case PFA_VERSION_V200:
		err_info->modem_id = map->pfa_tft_result.bits_v200.modem_id;
		err_info->pdusession_id = map->pfa_tft_result.bits_v200.pdu_session_id;
		break;
	default:
		return;
	}

	err_info->period_total_num = 1;

	__set_bit(pifid, (unsigned long *)&pctx->pif_bitmask);
}

void rnic_pkterr_rel_statistic_err(struct sk_buff *skb, pfa_version_type pfa_ver)
{
	struct rnic_pkterr_rel_ctx_s *pctx = rnic_pkterr_rel_ctx_ptr;
	struct rx_cb_map_s *map = NULL;
	struct rnic_pkterr_rel_info_s *err_info = NULL;
	unsigned int ipf_err_mask;
	int pifid = rnic_map_pifid(skb);

	if (pctx == NULL) {
		RNIC_LOGE("rnic_pkterr_rel_ctx_s is NUll");
		return;
	}

	if (!(BIT_ULL(pifid) & RNIC_PKTERR_PIFMASK) ||
	    rnic_map_pkttype(skb) != RNIC_MAP_PKTTYPE_IP)
		return;

	map = rnic_map_skb_cb(skb);
	switch (pfa_ver) {
	case PFA_VERSION_V0:
		ipf_err_mask = RNIC_IPF_ERR_MASK_V0;
		break;
	case PFA_VERSION_V100:
		ipf_err_mask = RNIC_IPF_ERR_MASK_V100;
		break;
	case PFA_VERSION_V200:
		ipf_err_mask = RNIC_IPF_ERR_MASK_V200;
		break;
	default:
		return;
	}

	err_info = &pctx->pkterr_info[pifid];
	err_info->period_total_num++;

	if (rnic_pkterr_rel_judge_err(map, ipf_err_mask)) {
		err_info->total_pkterr_num++;
		err_info->period_pkterr_num++;

		if (!pctx->pif_bitmask)
			mod_timer(&pctx->pkterr_timer,
				RNIC_PKTERR_TIME_TO_JIFFIES(pctx->fea_cfg.pkterr_period));

		if (err_info->period_idx == 0 && err_info->period_pkterr_num == 1) {
			rnic_pkterr_rel_start_cur_period(err_info, map, pfa_ver, pifid);
			rnic_pkterr_rel_rpt_err_ind(err_info, map, pfa_ver, RNIC_PKTERR_RPT_START);
		    }
	}
}

void rnic_pkterr_rel_fea_cfg(unsigned short period, unsigned short cycle,
			     unsigned int num_threshhold)
{
	struct rnic_pkterr_rel_ctx_s *pctx = rnic_pkterr_rel_ctx_ptr;

	if (rnic_pkterr_rel_ctx_ptr == NULL) {
		RNIC_LOGE("pkterr init is not ready");
		return;
	}

	pctx->fea_cfg.pkterr_cycle = (unsigned int)cycle;
	pctx->fea_cfg.pkterr_num_threshhold = num_threshhold;
	pctx->fea_cfg.pkterr_period = period;
}

int rnic_pkterr_rel_init(void)
{
	struct rnic_pkterr_rel_ctx_s *pctx= NULL;

	pctx= kzalloc(sizeof(*pctx), GFP_KERNEL);
	if (pctx== NULL) {
		RNIC_LOGE("alloc pkterr resource failed");
		return -ENOMEM;
	}

	rnic_pkterr_rel_ctx_ptr = pctx;

	pctx->fea_cfg.pkterr_cycle = RNIC_PKTERR_CYCLE;
	pctx->fea_cfg.pkterr_num_threshhold = RNIC_PKTERR_NUM_THRESHHOLD;
	pctx->fea_cfg.pkterr_period = RNIC_PKTERR_PERIOD;

	timer_setup(&pctx->pkterr_timer, rnic_pkterr_rel_timeout, 0);

	return 0;
}

void rnic_pkterr_rel_deinit(void)
{
	if (rnic_pkterr_rel_ctx_ptr == NULL)
		return;

	kfree(rnic_pkterr_rel_ctx_ptr);
	rnic_pkterr_rel_ctx_ptr = NULL;
}

#ifndef LLT_OS_VER
u32 rnic_pkterr_rel_show_status(unsigned int pif_id)
{
	struct rnic_pkterr_rel_ctx_s *pctx = rnic_pkterr_rel_ctx_ptr;
	struct rnic_pkterr_rel_info_s *info = NULL;

	if (pctx == NULL) {
		RNIC_LOGE("pkterr rel init is not ready");
		return -1;
	}

	if (pif_id >= RNIC_MAX_PIF_NUM) {
		RNIC_LOGE("clear_err_data: invalid pif_id");
		return -1;
	}

	info = &pctx->pkterr_info[pif_id];
	pr_err("[rnic] pkterr_info period_idx %u \n", info->period_idx);
	pr_err("[rnic] pkterr_info period_pkterr_num %u \n", info->period_pkterr_num);
	pr_err("[rnic] pkterr_info total_pkterr_num %u \n", info->total_pkterr_num);
	pr_err("[rnic] pkterr_info period_total_num %u \n", info->period_total_num);

	pr_err("[rnic] pkterr_fea pkterr_cycle %u \n", pctx->fea_cfg.pkterr_cycle);
	pr_err("[rnic] pkterr_fea pkterr_period %u \n",  pctx->fea_cfg.pkterr_period);
	pr_err("[rnic] pkterr_fea pkterr_num_threshhold %u \n",  pctx->fea_cfg.pkterr_num_threshhold);

	return 0;
}
#endif