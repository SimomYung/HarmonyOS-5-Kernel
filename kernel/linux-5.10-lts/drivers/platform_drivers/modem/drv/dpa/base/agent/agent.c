/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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
#include <linux/io.h>
#include <linux/pm_runtime.h>
#include <linux/of_device.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include "bsp_dpa.h"
#include <securec.h>
#include <msg_id.h>
#include <bsp_print.h>
#include <bsp_version.h>
#include <bsp_reset.h>
#include <bsp_msg.h>
#include "pm.h"
#include "rpm.h"
#include "bsp_print.h"
#include "bsp_version.h"
#include <bsp_diag.h>
#include <bsp_slice.h>
#include <bsp_ipcmsg.h>
#include <bsp_ipc_fusion.h>
#include <bsp_dump.h>
#include "agent.h"

static ipc_handle_t g_lockup_phandle = NULL;
static ipc_handle_t g_lockup_rls_phandle = NULL;
static unsigned int g_dpa_agent_sem;

int bsp_dpapm_get_dpa_access_state(dpa_regs_access_state_e dpa_regs_region)
{
    dpa_pm_ctrl_s *ctrl = (dpa_pm_ctrl_s *)bsp_dpa_pm_get_ctrl();
    if (ctrl == NULL || ctrl->cb_ops.get_dpa_access_state == NULL) {
        return BSP_ERROR;
    }

    return ctrl->cb_ops.get_dpa_access_state(dpa_regs_region);
}


void bsp_dpa_reg_info_get(reg_ctrl_e id, struct dpa_reg_info *info)
{
    dpa_pm_ctrl_s *ctrl = (dpa_pm_ctrl_s *)bsp_dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        return;
    }

    if (info == NULL || id >= REG_BASE_MAX) {
        return;
    }

    info->virt_addr = ctrl->reg_info[id].virt_addr;
    info->phy_addr = ctrl->reg_info[id].phy_addr;
    info->size = ctrl->reg_info[id].size;
}

void bsp_dpa_tcm_bak_info_get(struct dpa_reg_info *dtcm_info, struct dpa_reg_info *itcm_info)
{
    dpa_pm_ctrl_s *ctrl = (dpa_pm_ctrl_s *)bsp_dpa_pm_get_ctrl();
    if (ctrl == NULL) {
        return;
    }
    if (dtcm_info != NULL) {
        dtcm_info->virt_addr = ctrl->dtcm_bak;
        dtcm_info->phy_addr = 0;
        dtcm_info->size = ctrl->reg_info[DPA_DTCM_GLOCAL].size;
    }
    if (itcm_info != NULL) {
        itcm_info->virt_addr = ctrl->itcm_bak;
        itcm_info->phy_addr = 0;
        itcm_info->size = ctrl->image_itcm_size;
    }
}

struct mutex g_dpa_msg_mutex_lock;

unsigned int g_int_ts = 0;
unsigned int g_dpa_agent_timeout_ts = 0;

static void fore_lockup_dpa_ack_ipc_cb(void *arg)
{
    g_dpa_agent_sem = 1;

    g_int_ts = bsp_get_slice_value();
}

int dpa_wait_access_status(void)
{
    int cnt = 100;

    while (g_dpa_agent_sem == 0 && cnt) {
        cnt--;
        msleep(1);
    }

    if (cnt <= 0) {
        g_dpa_agent_timeout_ts =  bsp_get_slice_value();
        bsp_err("wait dpa running timeout, g_dpa_agent_sem is %u!\n", g_dpa_agent_sem);
        return -1;
    }

    return 0;
}

int dpa_msg_send_prepare(void)
{
    int ret;

    g_dpa_agent_sem = 0;

    ret = bsp_ipc_send(g_lockup_phandle);
    if (ret) {
        bsp_err("dpa wakeup ipc send fail. \n");
        return -1;
    }

    ret = dpa_wait_access_status();
    if (ret) {
        ret = bsp_ipc_send(g_lockup_rls_phandle);
        if (ret) {
            bsp_err("dpa wakeup fail, rls ipc send fail. \n");
            return -1;
        }
        return -1;
    }

    return 0;
}
unsigned int g_dpa_msg_send_ts = 0;
int bsp_msg_sendto_dpa(struct msg_chn_hdl *chn_hdl, const struct msg_addr *dst, void *msg, unsigned len)
{
    int ret;
    version_chip_type_e chip_type = (version_chip_type_e)bsp_get_version_info()->chip_type;

    if (chip_type == CHIP_MAXWELL) {
        mutex_lock(&g_dpa_msg_mutex_lock);

        g_dpa_msg_send_ts =  bsp_get_slice_value();

        // dpa 状态需保证在running状态，并持锁，不进低功耗
        ret = dpa_msg_send_prepare();
        if (ret) {
            bsp_err("dpa status prepare fai, ret = %d \n", ret);
            mutex_unlock(&g_dpa_msg_mutex_lock);
            return ret;
        }

        // 需要保证dpa是running时，eicc可用，才能发msg消息;必须在锁里，否则消息并发，第二个消息prepare都判断过后，第一个消息会将dpa锁释放，导致下面接口失败；
        ret = bsp_msg_lite_sendto(chn_hdl, dst, msg, len);
        if (ret) {
            bsp_err("msg send to dpa err, ret = %d \n", ret);
        }

        mutex_unlock(&g_dpa_msg_mutex_lock);
    } else {
        ret = bsp_msg_lite_sendto(chn_hdl, dst, msg, len);
        if (ret) {
            bsp_err("msg send to dpa err, ret = %d \n", ret);
        }
    }

    return ret;
}

static msg_chn_t g_dpa_agent_msg_hdl = NULL;
unsigned int g_dpa_mdm_rst_stat;

int dpa_agent_send_msg_to_dpa(unsigned int status)
{
    int ret;
    struct msg_addr dst;

    dst.core = MSG_CORE_DPA;
    dst.chnid = MSG_CHN_DPA_COMM;

    ret = bsp_msg_sendto_dpa(g_dpa_agent_msg_hdl, &dst, (void *)&status, sizeof(unsigned int));
    if (ret) {
        bsp_err("send msg err, ret = %d \n", ret);
    }
    return ret;
}

int bsp_dpa_agent_send_msg_to_dpa(unsigned int status)
{
    return dpa_agent_send_msg_to_dpa(status);
}

int dpa_agent_msg_rcv_data(void *msg, unsigned int len)
{
    if (len != sizeof(unsigned int)) {
        bsp_err("msg len(0x%x) err, need len:0x%x\n", len, (unsigned int)sizeof(unsigned int));
        return -1;
    }

    g_dpa_mdm_rst_stat = *(unsigned int *)msg;

    return 0;

}

int dpa_agent_msg_proc(const struct msg_addr *src, void *msg, unsigned int len)
{
    if (src == NULL || msg == NULL) {
        bsp_err("para err\n");
        return -1;
    }

    if (src->chnid == MSG_CHN_DPA_COMM) {
        return dpa_agent_msg_rcv_data(msg, len);
    } else {
       bsp_err("msg chnid err\n");
       return -1;
    }

    return 0;
}


// dpa 单独复位
int dpa_agent_reset_cb_after(void)
{
    int ret, timeout = 500;

    // 发msg给dpa，告知dpa，mdm要进行单独复位
    g_dpa_mdm_rst_stat = MODEM_REST_AFTER_REQ;
    ret = dpa_agent_send_msg_to_dpa(MODEM_REST_AFTER_REQ);
    if (ret) {
        return ret;
    }

    // 等待dpa返回结果
    while(g_dpa_mdm_rst_stat != MODEM_REST_AFTER_DONE && timeout >= 0) {
        if (g_dpa_mdm_rst_stat == MODEM_REST_AFTER_FAIL) {
            // mdm rst，dpa 处理失败，则返回错误让整机复位
            bsp_err("dpa-agent mdm-rst before fail.\n");
            return -1;
        }
        msleep(1);
        timeout--;
    }

    if (timeout < 0) {
        bsp_err("dpa-agent mdm-rst before timeout fail.\n");
        return -1;
    }

    return 0;
}

int dpa_agent_reset_cb_before(void)
{
    int ret, timeout = 500;

    // 发msg给dpa，告知dpa，mdm要进行单独复位
    g_dpa_mdm_rst_stat = MODEM_REST_BEFORE_REQ;
    ret = dpa_agent_send_msg_to_dpa(MODEM_REST_BEFORE_REQ);
    if (ret) {
        return ret;
    }

    // 等待dpa返回结果
    while(g_dpa_mdm_rst_stat != MODEM_REST_BEFORE_DONE && timeout >= 0) {
        if (g_dpa_mdm_rst_stat == MODEM_REST_BEFORE_FAIL) {
            // mdm rst，dpa 处理失败，则返回错误让整机复位
            bsp_err("dpa-agent mdm-rst after fail.\n");
            return -1;
        }
        msleep(1);
        timeout--;
    }

    if (timeout < 0) {
        bsp_err("dpa-agent mdm-rst after timeout fail.\n");
        return -1;
    }

    return 0;
}

int dpa_agent_reset_cb_func(drv_reset_cb_moment_e eparam, int mac_id)
{
    int ret = 0;

    if (eparam == MDRV_RESET_CB_BEFORE) {
        ret = dpa_agent_reset_cb_before();
        if (ret) {
            bsp_err("dpa agent cb before fail,ret=%d\n", ret);
        }
    } else if (eparam == MDRV_RESET_CB_AFTER) {
        ret = dpa_agent_reset_cb_after();
        if (ret) {
            bsp_err("dpa agent cb after fail,ret=%d\n", ret);
        }
    }

    return ret;
}

int dpa_agent_msg_init(void)
{
    int ret = 0;
    struct msgchn_attr lite_attr = { 0 };

    if (g_dpa_agent_msg_hdl == NULL) {
        bsp_msgchn_attr_init(&lite_attr);
        lite_attr.chnid = MSG_CHN_DPA_COMM;
        lite_attr.coremask = MSG_CORE_MASK(MSG_CORE_DPA);
        lite_attr.lite_notify = dpa_agent_msg_proc;
        ret = bsp_msg_lite_open(&g_dpa_agent_msg_hdl, &lite_attr);
        if (ret) {
            bsp_err("msg open err,ret=%d\n", ret);
            return ret;
        }
    }

    bsp_err("msg init ok\n");

    return ret;
}

int dpa_agent_init(void)
{
    int ret;
    ipc_handle_t phandle = NULL;

    mutex_init(&g_dpa_msg_mutex_lock);

    ret = bsp_ipc_open(IPC_DPA_INT_SRC_ACPU_FORCE_LOCKUP, &g_lockup_phandle);
    if (ret != BSP_OK) {
        bsp_err("dpa agent open ipc fail.\n");
        return BSP_ERROR;
    }
    ret = dpa_agent_msg_init();
    if (ret) {
        bsp_err("dpa agent msg init failed\n");
        return ret;
    }

    ret = bsp_reset_cb_func_register("dpa-agent", dpa_agent_reset_cb_func, 0, DRV_RESET_CB_PIOR_DPA);
    if (ret) {
        bsp_err("mdm reset dpa call back func failed\n");
        return ret;
    }

    ret = bsp_ipc_open(IPC_DPA_INT_SRC_ACPU_FORCE_LOCKUP_RLS, &g_lockup_rls_phandle);
    if (ret != BSP_OK) {
        bsp_err("dpa agent open ipc fail.\n");
        return BSP_ERROR;
    }

    ret = bsp_ipc_open(IPC_ACPU_INT_SRC_DPA_FORCE_LOCKUP_ACK, &phandle);
    if (ret) {
        bsp_err("dpa agent ipc open fail \n");
        return ret;
    }

    ret = bsp_ipc_connect(phandle, fore_lockup_dpa_ack_ipc_cb, NULL);
    if(ret) {
        bsp_err("dpa agent ipc connect fail \n");
        return ret;
    }

    return 0;
}

subsys_initcall_sync(dpa_agent_init);

EXPORT_SYMBOL(bsp_msg_sendto_dpa);
EXPORT_SYMBOL(bsp_dpa_reg_info_get);
EXPORT_SYMBOL(bsp_dpapm_get_dpa_access_state);
