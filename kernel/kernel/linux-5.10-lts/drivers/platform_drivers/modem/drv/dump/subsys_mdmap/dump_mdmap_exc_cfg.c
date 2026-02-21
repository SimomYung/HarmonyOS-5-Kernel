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
#include <dump_rdr_wrapper.h>
#include <dump_subsys_mgr.h>
#include <dump_modid.h>
#include "dump_mdmap_exc_cfg.h"

/* e_exce_type 在EXCH_SOURCE枚举中申明，需要AP和MODEM同时修改
 * AP侧modem exc后面的枚举是其他组件，不易变更，因此这里对于目前不存在的exc_type
 * 都使用相似的枚举来表示异常原因，差异大的，直接使用CP_S_EXCEPTION
 */
#define DUMP_MDMAP_EXC_NOTIFY_SUBSYS_DEFAULT \
            (DUMP_SUBSYS_MDM_VALID_MASK | DUMP_SUBSYS_MASK_AP | DUMP_SUBSYS_MASK_LPMCU)
#define DUMP_MDMAP_EXC_NOTIFY_SUBSYS_NO_CP (DUMP_MDMAP_EXC_NOTIFY_SUBSYS_DEFAULT & (~DUMP_SUBSYS_MASK_CP))
__ro_after_init dump_exc_cfg_s g_dump_mdmap_exc_cfg[] = {
    {
        DUMP_SUBSYS_MASK_CP, 0, CP_S_NORMALRESET, DRV_ERRNO_RESET_SIM_SWITCH,
        DRV_ERRNO_RESET_SIM_SWITCH, RDR_MODEM_CP_RESET_SIM_SWITCH_MOD_ID, "CP SIM SWITCH"
    },
    {
        DUMP_SUBSYS_MASK_AP, DUMP_MDMAP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_RESETFAIL, DRV_ERRNO_RST_FAIL,
        DRV_ERRNO_RST_FAIL, RDR_MODEM_CP_RESET_FAIL_MOD_ID, "CP RESET FAIL"
    },
    // 等PSMC超时时，不需要通知CP，因为这是在CP单独复位前触发的异常，CP的信息在上次异常时已经刷入DDR了；
    // 并且此时已经重置了DUMP CP AREA，因此无法检测到CP保存完成标记位
    {
        DUMP_SUBSYS_MASK_AP, DUMP_MDMAP_EXC_NOTIFY_SUBSYS_NO_CP, CP_S_RESETFAIL, DRV_ERRNO_MDMRST_WAIT_PSMC_TIMEOUT,
        DRV_ERRNO_MDMRST_WAIT_PSMC_TIMEOUT, RDR_MODEM_CP_RESET_STR_PSMC_BLOCK_MOD_ID, "PSMC BLOCK CP RST"
    },
    // 当前不存在, 保留做注释
    // DUMP_SUBSYS_MASK_AP, DUMP_MDMAP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_RESETFAIL, RDR_MODEM_CP_RESET_FREQUENTLY_MOD_ID,
    // RDR_MODEM_CP_RESET_FREQUENTLY_MOD_ID, RDR_MODEM_CP_RESET_FAIL_MOD_ID, "CP RESET FREQUENTLY"
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMAP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_RILD_EXCEPTION, DRV_ERRNO_RESET_RILD,
        DRV_ERRNO_RESET_RILD, RDR_MODEM_CP_RESET_RILD_MOD_ID, "CP RESET BY RILD"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMAP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_3RD_EXCEPTION, DRV_ERRNO_RESET_3RD_MODEM,
        DRV_ERRNO_RESET_3RD_MODEM, RDR_MODEM_CP_RESET_3RD_MOD_ID, "CP RESET BY 3RD"
    },
    {
        DUMP_SUBSYS_MASK_CP, DUMP_MDMAP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_MODEMAP, DRV_ERRNO_DRA_ADDR_CHECK_FAIL,
        DRV_ERRNO_DRA_ADDR_CHECK_FAIL, RDR_MODEM_CP_RESET_DRA_MOD_ID, "MDMAP DRA CHECKFAIL"
    },
    {
        DUMP_SUBSYS_MASK_AP, DUMP_MDMAP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_NORMALRESET, DRV_ERRNO_RESET_REBOOT_REQ,
        DRV_ERRNO_RESET_REBOOT_REQ, RDR_MODEM_CP_RESET_REBOOT_REQ_MOD_ID, "RILD RST CP NOT SUPPORT"
    },
    {
        DUMP_SUBSYS_MASK_AP, DUMP_MDMAP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_MODEMNOC, DRV_ERRNO_NOC_PHONE,
        DRV_ERRNO_NOC_PHONE, RDR_MODEM_NOC_MOD_ID, "MODEM NOC"
    },
    {
        DUMP_SUBSYS_MASK_CP, 0, CP_S_NORMALRESET, DRV_ERROR_USER_RESET,
        DRV_ERROR_USER_RESET, RDR_MODEM_CP_RESET_USER_RESET_MOD_ID, "MDMAP USER REQ RST SYS"
    },
    {
        DUMP_SUBSYS_MASK_AP, DUMP_MDMAP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_MODEMDMSS, DRV_ERRNO_DMSS_PHONE,
        DRV_ERRNO_DMSS_PHONE, RDR_MODEM_DMSS_MOD_ID, "MODEM DMSS"
    },
    // 当前不存在, 保留做注释
    // DUMP_SUBSYS_MASK_AP, 0, CP_S_MODEMAP, RDR_MODEM_CODE_PATCH_REVERT_MOD_ID,
    // RDR_MODEM_CODE_PATCH_REVERT_MOD_ID, RDR_MODEM_CODE_PATCH_REVERT_MOD_ID, "MODEM PATCH REVERT"
    {
        DUMP_SUBSYS_MASK_AP, DUMP_MDMAP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_MODEMAP, RDR_MODEM_AP_DRV_MOD_ID_START,
        RDR_MODEM_AP_DRV_MOD_ID_END, RDR_MODEM_AP_DRV_MOD_ID, "MDMAP DRV EXCEPTION"
    },
    {
        DUMP_SUBSYS_MASK_AP, DUMP_MDMAP_EXC_NOTIFY_SUBSYS_DEFAULT, CP_S_MODEMAP, RDR_MODEM_MOD_ID_START,
        RDR_MODEM_MOD_ID_END, RDR_MODEM_AP_MOD_ID, "MDMAP EXCEPTION"
    },
};

int dump_get_mdmap_exc_cfg(dump_subsys_exc_cfg_s *cfg)
{
    if (cfg == NULL) {
        return DUMP_ERRNO_INVA_PARAM;
    }
    cfg->num = ARRAY_SIZE(g_dump_mdmap_exc_cfg);
    cfg->cfg_list = (const dump_exc_cfg_s *)g_dump_mdmap_exc_cfg;
    return DUMP_OK;
}

/* 若某个异常需要子系统A复位，但是子系统A又不支持单独复位，那么就配置成整机复位 */
__init void dump_mdmap_subsys_update_exc_cfg(u64 reset_support_subsysmask)
{
    int i;
    for (i = 0; i < ARRAY_SIZE(g_dump_mdmap_exc_cfg); i++) {
        if (g_dump_mdmap_exc_cfg[i].reset_subsys_mask & (~reset_support_subsysmask)) {
            g_dump_mdmap_exc_cfg[i].reset_subsys_mask = DUMP_SUBSYS_MASK_AP;
        }
    }
}