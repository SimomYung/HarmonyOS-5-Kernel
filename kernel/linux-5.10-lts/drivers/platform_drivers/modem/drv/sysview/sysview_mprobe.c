/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
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
 */
#include <linux/cpumask.h>
#include <bsp_mprobe.h>
#include <bsp_cpufreq.h>
#include "sysview_comm.h"
#include "sysview_mprobe.h"
#include "product_config.h"
#include "bsp_diag.h"
#include "mdrv_diag.h"
#define DIAG_MXT_SYSVIEW_ID(msg_id) DIAG_DRV_GEN_CMD_ID(DIAG_FRAME_MSG_MXT, DIAG_DRV_FIELD_OM, DIAG_DRV_MODULE_SYSVIEW, msg_id)

#undef THIS_MODU
#define THIS_MODU mod_coresight
struct sysview_info g_sysmp_ctrl;

static int sysview_mprobe_send(u32 comp_id, u32 msg_id, u8 *data, u32 len)
{
    int ret;
    unsigned int i;
    unsigned int cmd_id = DIAG_MXT_SYSVIEW_ID(msg_id);
    unsigned int cnt = len / SYSVIEW_MPROBE_MAX_PACKAGE;
    unsigned int left = len % SYSVIEW_MPROBE_MAX_PACKAGE;

    for (i = 0; i < cnt; i++) {
        ret = bsp_diag_mxt_report(cmd_id, data, SYSVIEW_MPROBE_MAX_PACKAGE);
        if (ret != BSP_OK) {
            return ret;
        }
        data += SYSVIEW_MPROBE_MAX_PACKAGE;
    }
    return bsp_diag_mxt_report(cmd_id, data, left);
}

static void sysview_mprobe_send_cpuinfo(u32 mode, drv_cpuinfo_data_s *usage, u32 size)
{
    u32 i;
    if (mode & SYSVIEW_MPROBE_CPULOAD) {
        (void)memset_s(usage, size, 0, sizeof(drv_cpuinfo_data_s));
        for_each_present_cpu(i)
        {
            if (i >= SYSVIEW_MAX_CORE) {
                break;
            }
            if ((1U << i) & g_sysmp_ctrl.core_mask) {
                usage->usage[i] = sysview_cpu_usage_get(i);
            }
        }
        usage->core_num = num_possible_cpus();
        sysview_mprobe_send(MPROBE_COMPID_SYSVIEW, SYSVIEW_MPROBE_CPULOAD, (u8 *)(uintptr_t)usage,
            sizeof(drv_cpuinfo_data_s));
    }
}
static void sysview_mprobe_send_meminfo(u32 mode)
{
    sysview_meminfo_s mem_info = { 0 };
    if (mode & 0x8) { // memory id
        sysview_memory_get(&mem_info);
        // send id 0x4 is same with ccpu
        sysview_mprobe_send(MPROBE_COMPID_SYSVIEW, SYSVIEW_MPROBE_MEMORY, (u8 *)(uintptr_t)&mem_info,
            sizeof(sysview_meminfo_s));
    }
}
static void sysview_mprobe_send_curfreq(u32 mode)
{
    u32 val;
    if (mode & (1U << 4)) { // cpu freq
        val = bsp_dfs_get_curfreq(DFS_CPU_FREQ);
        sysview_mprobe_send(MPROBE_COMPID_SYSVIEW, SYSVIEW_MPROBE_CPUFREQ, (u8 *)(uintptr_t)&val,
                sizeof(u32));
    }
    if (mode & (1U << 5)) { // bus freq
        val = bsp_dfs_get_curfreq(DFS_BUS_FREQ);
        sysview_mprobe_send(MPROBE_COMPID_SYSVIEW, SYSVIEW_MPROBE_BUSFREQ, (u8 *)(uintptr_t)&val,
                sizeof(u32));
    }
}
static int sysview_mprobe_send_task(void *data)
{
    u32 mode;
    drv_cpuinfo_data_s *usage = NULL;
    usage = (drv_cpuinfo_data_s *)osl_malloc(sizeof(drv_cpuinfo_data_s));
    if (usage == NULL) {
        bsp_err("usage malloc err\n");
        return BSP_ERROR;
    }
    while (1) {
        osl_sem_downtimeout(&g_sysmp_ctrl.load_task_sem, g_sysmp_ctrl.jiffies);
        mode = g_sysmp_ctrl.mode;
        sysview_mprobe_send_cpuinfo(mode, usage, sizeof(drv_cpuinfo_data_s));
        sysview_mprobe_send_meminfo(mode);
        sysview_mprobe_send_curfreq(mode);
    }
    osl_free(usage);
    return BSP_OK;
}

static void sysview_mprobe_func_reg(unsigned char *data)
{
    diag_frame_head_s *head = (diag_frame_head_s *)data;
    int i;
    u32 val;
    u32 cfg_info[0x3] = { 0, 0, 0 }; // must > sizeof(diag_drv_cpuinfo_s)
    diag_drv_cpuinfo_s *cfg = NULL;
    if (head == NULL) {
        return;
    }
    switch (DIAG_GET_MSG_ID_FROM_CMD_ID(head->cmd_id)) {
        case SYSVIEW_USAGE:
            for (i = 0; i < head->msg_len && i < sizeof(cfg_info); i++) {
                cfg_info[i / 0x4] = cfg_info[i / 0x4] * 0x100 + head->data[i];
            }
            cfg = (diag_drv_cpuinfo_s *)(uintptr_t)&cfg_info;
            sysview_start(cfg, &g_sysmp_ctrl);
            break;
        case SYSVIEW_DDR_FREQ_LOCK_DIS:
            val = SYSVIEW_DDR_ENABLE;
            sysview_msg_sendtolpm((void *)(uintptr_t)&val, sizeof(u32));
            break;
        case SYSVIEW_DDR_FREQ_LOCK_EN:
            val = SYSVIEW_DDR_DISABLE;
            sysview_msg_sendtolpm((void *)(uintptr_t)&val, sizeof(u32));
            break;
        case SYSVIEW_DDR_FREQ_GET:
            val = SYSVIEW_DDR_FREQ;
            sysview_msg_sendtolpm((void *)(uintptr_t)&val, sizeof(u32));
            break;
        case SYSVIEW_CPU_FREQ_LOCK_DIS:
            (void)bsp_dfs_quit_fix_freq(DFS_CPU_FREQ);
            break;
        case SYSVIEW_CPU_FREQ_LOCK_EN:
            (void)bsp_dfs_req_fix_freq(DFS_CPU_FREQ, bsp_dfs_get_curfreq(DFS_CPU_FREQ));
            break;
        default:
            break;
    }
    return;
}

static void sysview_mprobe_callback(unsigned int state)
{
    if (DIAG_GET_MXT_CONN_STATE(state) == 0) {
        sysview_stop(&g_sysmp_ctrl);
    }
    return;
}
int sysview_mprobe_deal_lpm(void *buf, u32 len)
{
    struct sysview_msg_lpm_head *msg_head = (struct sysview_msg_lpm_head *)(uintptr_t)buf;
    struct sysview_msg_ddr_freq *msg_data = (struct sysview_msg_ddr_freq *)(uintptr_t)buf;
    if (buf == NULL || len < sizeof(struct sysview_msg_lpm_head) || g_sysmp_ctrl.init_flag != SYSVIEW_INIT_OK) {
        return BSP_ERROR;
    }
    switch (msg_head->type) {
        case SYSVIEW_MSG_DDR_FREQ:
            if (len != sizeof(struct sysview_msg_ddr_freq)) {
                return BSP_ERROR;
            }
            return sysview_mprobe_send(MPROBE_COMPID_SYSVIEW, SYSVIEW_MPROBE_DDR_FREQ,
                (void *)(uintptr_t)&msg_data->freq, sizeof(msg_data->freq));
        default:
            break;
    }
    return BSP_ERROR;
}

void sysview_mprobe_init(void)
{
    unsigned int cmd_id = DIAG_DRV_GEN_CMD_ID(DIAG_FRAME_MSG_MXT, DIAG_DRV_FIELD_OM, DIAG_DRV_MODULE_SYSVIEW, 0); // 0x9FA02280;
    if (sysview_task_init(&g_sysmp_ctrl, "sysview_mprobe", sysview_mprobe_send_task)) {
        return;
    }
    bsp_diag_drv_cfg_fn_register(cmd_id, sysview_mprobe_func_reg);
    mdrv_diag_conn_state_notify_fun_reg(sysview_mprobe_callback);
    g_sysmp_ctrl.init_flag = SYSVIEW_INIT_OK;
    return;
}