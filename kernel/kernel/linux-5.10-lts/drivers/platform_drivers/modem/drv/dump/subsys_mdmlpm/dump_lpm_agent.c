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
#include <securec.h>
#include <osl_types.h>
#include <bsp_slice.h>
#include <bsp_ipc_fusion.h>
#include <bsp_om_enum.h>
#include <bsp_dump.h>
#include <dump_area.h>
#include <dump_modid.h>
#include <dump_rdr_wrapper.h>
#include <dump_subsys_mgr.h>
#include <dump_frame.h>
#include <dump_infra.h>
#include "dump_lpm_exc_cfg.h"
#include "dump_lpm_log.h"
#include "dump_lpm_agent.h"

int dump_lpmcu_prepare_handler(const dump_exc_log_desc_s *exc_desc);
int dump_lpmcu_event_handler(int event, const dump_exc_log_desc_s *exc_desc);

dump_lpmcu_agent_ctrl_s g_dump_lpmcu_agent;
__ro_after_init dump_subsys_drv_s g_dump_lpm_subsys = {
    .dump_sys = DUMP_SUBSYS_MDMLPM,
    .name = "MDMLPMCU",
    /* g_dump_lpm_exc_cfg 在另一个文件中声名；也方便后续版本有差异时，拆分到hal层
    .exc_cfg = {
        .num = ARRAY_SIZE(g_dump_lpm_exc_cfg),
        .cfg_list = &g_dump_lpm_exc_cfg,
    },
    */
    .work_ctrl = &g_dump_lpmcu_agent.work_ctrl,
    .dump_prepare_handler = dump_lpmcu_prepare_handler,
    .reset_ops = NULL, /* 不支持单独复位 */
    .event_handler = dump_lpmcu_event_handler,
};

int dump_fill_lpmcu_exc_info(dump_base_info_s *base_info, dump_subsys_exc_info_s *subsys_exc)
{
    int ret = DUMP_OK;
    
    subsys_exc->e_from_subsys = DUMP_SUBSYS_MDMLPM;
    subsys_exc->data = NULL;
    subsys_exc->length = 0;
    subsys_exc->reason = DUMP_REASON_NORMAL;
    (void)memset_s(subsys_exc->task_name, DUMP_EXCINFO_TASKNAME_LEN, 0, DUMP_EXCINFO_TASKNAME_LEN);

    if (base_info != NULL) {
        subsys_exc->modid = base_info->mod_id;
        subsys_exc->arg1 = base_info->arg1;
        subsys_exc->arg2 = base_info->arg2;
        subsys_exc->reboot_context = base_info->reboot_context;
        if (subsys_exc->reboot_context == DUMP_CTX_INT) {
            subsys_exc->int_no = base_info->reboot_int;
        } else {
            subsys_exc->task_id = base_info->reboot_task;
        }
    } else {
        subsys_exc->modid = 0;
        subsys_exc->arg1 = 0;
        subsys_exc->arg2 = 0;
        subsys_exc->reboot_context = DUMP_CTX_INT;
        subsys_exc->int_no = 0xFFFFFFFF;
        ret = DUMP_ERRNO_GET_BASEINFO;
    }
    return ret;
}

/******************************* lpmcu异常检测 ***************************/
void dump_lpmcu_ipc_handle(void *arg)
{
    int ret, rpt_ret;
    dump_subsys_exc_info_s subsys_exc;
    dump_base_info_s *base_info = (dump_base_info_s *)bsp_dump_get_field_addr(DUMP_MDMLPM_BASE_INFO);
    u32 timestamp = bsp_get_slice_value();

    /* 不论本次异常是否被rdr接纳，都将exc_flag置true,原因参见mdmcp的ipc_handler
     */
    g_dump_lpmcu_agent.lpmcu_exc_flag = true; 

    ret = dump_fill_lpmcu_exc_info(base_info, &subsys_exc); 

    rpt_ret = (int)dump_report_exception(&subsys_exc);

    /* 注意：当前只对reject进行了单独处理，但实际上accept later与accept now也可能是有差异的；只是accept now没有特殊的后处理流程
     *（例如在当前上下文保存一些数据，或者更新baseinfo等）
     */
    if (!dump_is_exc_rpt_accept(rpt_ret)) {
        dump_error("reject handle lpmcu exc by %d\n", rpt_ret);
        return;
    }


    if (ret != DUMP_OK) {
        dump_print("get lpmcu exc info fail ret %d\n", ret);
    }
    dump_print("modem lpmcu enter system error! timestamp:0x%x\n", timestamp);

    return;
}

__init int dump_lpmcu_ipc_init(void)
{
    int ret;

    ipc_handle_t phandle = NULL;

    ret = bsp_ipc_open(IPC_ACPU_INT_SRC_MCU_DUMP, &phandle);
    if (ret != BSP_OK) {
        dump_error("ipc_int_open fail!lpmcu_agent fail!\n");
        return BSP_OK;
    }

    ret = bsp_ipc_connect(phandle, (ipc_cbk)dump_lpmcu_ipc_handle, NULL);
    if (ret != BSP_OK) {
        dump_error("ipc_int_connect fail!lpmcu_agent fail!\n");
        return BSP_OK;
    }

    return BSP_OK;
}

/******************************* pre-dump：通知LPMCU ***************************/
void dump_notify_lpmcu(u32 mod_id)
{
    int ret;
    ipc_handle_t phandle = NULL;

    ret = bsp_ipc_open(IPC_MCU_INT_SRC_ACPU_DUMP, &phandle);
    if ((ret != BSP_OK) || (phandle == NULL)) {
        dump_error("notify lpmcu fail,please let ipc check!\n");
        return;
    }

    ret = bsp_ipc_send(phandle);
    if (ret != BSP_OK) {
        dump_error("notify lpmcu fail,please let ipc check!\n");
        return;
    }
    dump_print("notify lpmcu success\n");
}

int dump_lpmcu_prepare_handler(const dump_exc_log_desc_s *exc_desc)
{
    u32 modid = 0xFFFFFFFE;
    /* 不应该走到 */
    if (!dump_valid_exc_desc(exc_desc)) {
        dump_debug("exc_desc inv\n");
    } else {
        modid = exc_desc->exc_info->subsys_info.modid;
    }

    if (g_dump_lpmcu_agent.lpmcu_exc_flag) {
        dump_print("LPMCU exception,no need to notify LPMCU core,modid=0x%x\n", modid);
        return DUMP_OK;
    }
    dump_notify_lpmcu(modid);
    return DUMP_OK;
}

int dump_lpmcu_reset_before_cb(const dump_exc_log_desc_s *exc_desc)
{
    if (dump_valid_exc_desc(exc_desc)) {
        /* 单独复位前检查lpmcu相关标记, 如果上报了多次异常，怎么处理？ 比如异常A需要MDM单独复位，异常B需要DPA单独复位
         * 如果提前清理了exc_flag，那下次通知lpmcu就会有问题了；当前无法处理这种情况；后面考虑如何通过异常计数方案来处理这种情况
         */
        if ((exc_desc->exc_info->subsys_info.e_from_subsys == DUMP_SUBSYS_LPMCU)) {
            g_dump_lpmcu_agent.lpmcu_exc_flag = false;
            dump_print("lpm_exc_flag clear!\n");
        }
    } else {
        /* 不应该走到这，但如果走到了，不知道当前是什么异常的情况下，都把exc_flag清掉，这样后面再有lpm异常的时候，也能通知到lpm */
        if (g_dump_lpmcu_agent.lpmcu_exc_flag) {
            g_dump_lpmcu_agent.lpmcu_exc_flag = false;
            dump_print("lpm_exc_flag clear!\n");
        }
    }
    return BSP_OK;
}

/******************************* dump_event：dump事件处理 ***************************/
int dump_lpmcu_event_handler(int event, const dump_exc_log_desc_s *exc_desc)
{
    int ret = DUMP_OK;
    switch (event) {
        /* lpmcu触发的异常，如果是某个子系统单独复位，则在reset阶段清除信息；如果是整机复位，不需要清除
         * 因此，只需要在RESET_START阶段清除lpmcu_save done标记；因为单独复位才会走到reset_event；
         * 而handle_done event则任何需要存log的异常处理完后都会走到
         */
        case DUMP_EVENT_RESET_START:
            dump_clear_lpmcu_save_done();
            ret = dump_lpmcu_reset_before_cb(exc_desc);
            break;
        case DUMP_EVENT_EXC_HANDLE_DONE:
        default:
            break;
    }
    return ret;
}

/* 允许多次调用 */
void dump_lpmcu_subsys_drv_prepare(void)
{
    int ret;
    /* 使用默认workqueue和async work fn；work ctrl需要先初始化，后面注册work时会用到 */
    ret = dump_work_ctrl_factory(&g_dump_lpmcu_agent.work_ctrl, NULL, NULL);
    if (ret != DUMP_OK) {
        dump_print("lpmcu work ctrl init fail\n");
    }

    dump_get_lpmcu_exc_cfg(&g_dump_lpm_subsys.exc_cfg);
    dump_register_subsys(&g_dump_lpm_subsys);
}

__init void dump_lpmcu_subsys_init(void)
{
    int ret;

    ret = dump_lpmcu_ipc_init();
    if (ret != BSP_OK) {
        dump_print("reg lpmcu ipc fail\n");
    }

    dump_lpmcu_log_init();

    g_dump_lpmcu_agent.init_state = true;

    dump_print("dump mdm lpmcu init ok\n");
}
