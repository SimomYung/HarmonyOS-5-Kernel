/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2025. All rights reserved.
 * foss@huawei.com
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
 *
 */

#include <product_config.h>
#include <osl_types.h>
#include <dump_subsys.h>
#include <dump_modid.h>
#include <dump_rdr_wrapper.h>
#include <dump_errno.h>
#include "dump_cp_exc_cfg.h"

/* e_exce_type 在EXCH_SOURCE枚举中申明，需要AP和MODEM同时修改
 * AP侧modem exc后面的枚举是其他组件，不易变更，因此这里对于目前不存在的exc_type
 * 都使用相似的枚举来表示异常原因，差异大的，直接使用CP_S_EXCEPTION
 */
#define DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT \
            (DUMP_SUBSYS_MDM_VALID_MASK | DUMP_SUBSYS_MASK_AP | DUMP_SUBSYS_MASK_LPMCU | DUMP_SUBSYS_MASK_HIFI)
__ro_after_init dump_exc_cfg_s g_dump_cp_exc_cfg[] = {
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_EXCEPTION, RDR_MODEM_CP_CPU_EXC_MOD_ID,
        RDR_MODEM_CP_CPU_EXC_MOD_ID, RDR_MODEM_CP_COMMON_MOD_ID, "CP ABORT"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_EXCEPTION, RDR_MODEM_CP_WDT_EXC_MOD_ID,
        RDR_MODEM_CP_WDT_EXC_MOD_ID, RDR_MODEM_CP_WDT_MOD_ID, "CP WDT"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_DRV_EXC, RDR_MODEM_CP_DRV_MOD_ID_START,
        RDR_MODEM_CP_DRV_MOD_ID_END, RDR_MODEM_CP_DRV_MOD_ID, "CP DRV EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_PAM_EXC, RDR_MODEM_CP_PAM_MOD_ID_START,
        RDR_MODEM_CP_PAM_MOD_ID_END, RDR_MODEM_CP_PAM_MOD_ID, "CP PAM EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_CTTF_EXC, RDR_MODEM_CP_GUTTF_MOD_ID_START,
        RDR_MODEM_CP_GUTTF_MOD_ID_END, RDR_MODEM_CP_GUTTF_MOD_ID, "CP GUTTF EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_GUAS_EXC, RDR_MODEM_CP_GUWAS_MOD_ID_START,
        RDR_MODEM_CP_GUWAS_MOD_ID_END, RDR_MODEM_CP_GUWAS_MOD_ID, "CP GUWAS EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_GUAS_EXC, RDR_MODEM_CP_GUGAS_MOD_ID_START,
        RDR_MODEM_CP_GUGAS_MOD_ID_END, RDR_MODEM_CP_GUGAS_MOD_ID, "CP GUGAS EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_GUCNAS_EXC, RDR_MODEM_CP_GUNRNAS_MOD_ID_START,
        RDR_MODEM_CP_GUNRNAS_MOD_ID_END, RDR_MODEM_CP_GUNRNAS_MOD_ID, "CP GUNRNAS EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_CPHY_EXC, RDR_MODEM_CP_GUCPHY_MOD_ID_START,
        RDR_MODEM_CP_GUCPHY_MOD_ID_END, RDR_MODEM_CP_GUCPHY_MOD_ID, "CP GUCPHY EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_EXCEPTION, RDR_MODEM_CP_EASYRF_MOD_ID_START,
        RDR_MODEM_CP_EASYRF_MOD_ID_END, RDR_MODEM_CP_EASYRF_MOD_ID, "CP EASYRF EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_TLPS_EXC, RDR_MODEM_CP_LNAS_MOD_ID_START,
        RDR_MODEM_CP_LNAS_MOD_ID_END, RDR_MODEM_CP_LNAS_MOD_ID, "CP LNAS EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_TLPS_EXC, RDR_MODEM_CP_LRRC_MOD_ID_START,
        RDR_MODEM_CP_LRRC_MOD_ID_END, RDR_MODEM_CP_LRRC_MOD_ID, "CP LRRC EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_TLPS_EXC, RDR_MODEM_CP_NLL2_MOD_ID_START,
        RDR_MODEM_CP_NLL2_MOD_ID_END, RDR_MODEM_CP_NLL2_MOD_ID, "CP NLL2 EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_EXCEPTION, RDR_MODEM_CP_VOICE_MOD_ID_START,
        RDR_MODEM_CP_VOICE_MOD_ID_END, RDR_MODEM_CP_VOICE_MOD_ID, "CP VOICE EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_EXCEPTION, RDR_MODEM_CP_UPHY_MOD_ID_START,
        RDR_MODEM_CP_UPHY_MOD_ID_END, RDR_MODEM_CP_UPHY_MOD_ID, "CP UPHY EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_EXCEPTION, RDR_MODEM_CP_FESSBB_MOD_ID_START,
        RDR_MODEM_CP_FESSBB_MOD_ID_END, RDR_MODEM_CP_FESSBB_MOD_ID, "CP FESS BB EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_EXCEPTION, RDR_MODEM_CP_FESSRF_MOD_ID_START,
        RDR_MODEM_CP_FESSRF_MOD_ID_END, RDR_MODEM_CP_FESSRF_MOD_ID, "CP FESS RF EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_EXCEPTION, RDR_MODEM_CP_GPHY_MOD_ID_START,
        RDR_MODEM_CP_GPHY_MOD_ID_END, RDR_MODEM_CP_GPHY_MOD_ID, "CP GPHY EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_EXCEPTION, RDR_MODEM_CP_WPHY_MOD_ID_START,
        RDR_MODEM_CP_WPHY_MOD_ID_END, RDR_MODEM_CP_WPHY_MOD_ID, "CP WPHY EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_EXCEPTION, RDR_MODEM_CP_LPHY_MOD_ID_START,
        RDR_MODEM_CP_LPHY_MOD_ID_END, RDR_MODEM_CP_LPHY_MOD_ID, "CP LPHY EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_EXCEPTION, RDR_MODEM_CP_NPHY_MOD_ID_START,
        RDR_MODEM_CP_NPHY_MOD_ID_END, RDR_MODEM_CP_NPHY_MOD_ID, "CP NPHY EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_EXCEPTION, RDR_MODEM_CP_IMS_MOD_ID_START,
        RDR_MODEM_CP_IMS_MOD_ID_END, RDR_MODEM_CP_IMS_MOD_ID, "CP IMS EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_EXCEPTION, RDR_MODEM_CP_5GRRC_MOD_ID_START,
        RDR_MODEM_CP_5GRRC_MOD_ID_END, RDR_MODEM_CP_5GRRC_MOD_ID, "CP 5GRRC EXC"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMCP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_EXCEPTION, RDR_MODEM_MOD_ID_START,
        RDR_MODEM_MOD_ID_END, RDR_MODEM_CP_MOD_ID, "CP UNKNOW EXC"
    }
};

int dump_get_cp_exc_cfg(dump_subsys_exc_cfg_s *cfg)
{
    if (cfg == NULL) {
        return DUMP_ERRNO_INVA_PARAM;
    }
    cfg->num = ARRAY_SIZE(g_dump_cp_exc_cfg);
    cfg->cfg_list = (const dump_exc_cfg_s *)g_dump_cp_exc_cfg;
    return DUMP_OK;
}

/* 若某个异常需要子系统A复位，但是子系统A又不支持单独复位，那么就配置成整机复位 */
__init void dump_cp_subsys_update_exc_cfg(u64 reset_support_subsysmask)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(g_dump_cp_exc_cfg); i++) {
        if (g_dump_cp_exc_cfg[i].reset_subsys_mask & (~reset_support_subsysmask)) {
            g_dump_cp_exc_cfg[i].reset_subsys_mask = DUMP_SUBSYS_MASK_AP;
        }
    }
}