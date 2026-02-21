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
#include <linux/kernel.h>
#include <osl_types.h>
#include <bsp_slice.h>
#include <bsp_wdt.h>
#include <bsp_reset.h>
#include <bsp_ipc_fusion.h>
#include <bsp_om_enum.h>
#include <bsp_dump_mem.h>
#include <bsp_dump.h>
#include <bsp_adump.h>
#include <bsp_coresight.h>
#include <dump_rdr_wrapper.h>
#include <dump_infra.h>
#include <dump_frame.h>
#include <dump_errno.h>
#include <dump_subsys_mgr.h>
#include "dump_cp_exc_cfg.h"
#include "dump_cp_log.h"
#include "dump_cp_agent.h"

int dump_cp_prepare_handler(const dump_exc_log_desc_s *exc_desc);
int dump_cp_event_handler(int event, const dump_exc_log_desc_s *exc_desc);

dump_cp_agent_ctrl_s g_dump_cp_agent;
__ro_after_init dump_subsys_drv_s g_dump_cp_subsys = {
    .dump_sys = DUMP_SUBSYS_CP,
    .name = "MDMCP",
    /* g_dump_cp_exc_cfg 数组较大，在另一个文件中声名；也方便后续版本有差异时，拆分到hal层
    .exc_cfg = {
        .num = ARRAY_SIZE(g_dump_cp_exc_cfg),
        .cfg_list = &g_dump_cp_exc_cfg,
    },
    */
    .work_ctrl = &g_dump_cp_agent.work_ctrl,
    .dump_prepare_handler = dump_cp_prepare_handler,
    .reset_ops = NULL, /* 支持单独复位时才挂接 */
    .event_handler = dump_cp_event_handler,
};

int dump_fill_cp_exc_info(dump_base_info_s *base_info, dump_subsys_exc_info_s *subsys_exc)
{
    int ret = DUMP_OK;
    int len;
    
    subsys_exc->e_from_subsys = DUMP_SUBSYS_CP;
    subsys_exc->data = NULL;
    subsys_exc->length = 0;

    if (base_info != NULL) {
        subsys_exc->modid = base_info->mod_id;
        subsys_exc->arg1 = base_info->arg1;
        subsys_exc->arg2 = base_info->arg2;
        subsys_exc->reboot_context = base_info->reboot_context;
        subsys_exc->reason = (base_info->mod_id != DRV_ERRNO_DUMP_ARM_EXC) ? DUMP_REASON_NORMAL : DUMP_REASON_ARM;
        if (subsys_exc->reboot_context == DUMP_CTX_INT) {
            subsys_exc->int_no = base_info->reboot_int;
            (void)memset_s(subsys_exc->task_name, DUMP_EXCINFO_TASKNAME_LEN, 0, DUMP_EXCINFO_TASKNAME_LEN);
        } else {
            subsys_exc->task_id = base_info->reboot_task;
            len = sizeof(base_info->task_name);
            len = (len >= DUMP_EXCINFO_TASKNAME_LEN) ? DUMP_EXCINFO_TASKNAME_LEN : len;
            if (memcpy_s(subsys_exc->task_name, DUMP_EXCINFO_TASKNAME_LEN, base_info->task_name, len) != EOK) {
                ret = DUMP_ERRNO_MEM_OPS;
            }
            subsys_exc->task_name[DUMP_EXCINFO_TASKNAME_LEN - 1] = '\0';
        }
    } else {
        subsys_exc->modid = 0;
        subsys_exc->arg1 = 0;
        subsys_exc->arg2 = 0;
        subsys_exc->reboot_context = DUMP_CTX_INT;
        subsys_exc->reason = DUMP_REASON_NORMAL;
        subsys_exc->int_no = 0xFFFFFFFF;
        (void)memset_s(subsys_exc->task_name, DUMP_EXCINFO_TASKNAME_LEN, 0, DUMP_EXCINFO_TASKNAME_LEN);
        ret = DUMP_ERRNO_GET_BASEINFO;
    }
    return ret;
}
DUMP_LLT_EXPORT_SYMBOL(dump_fill_cp_exc_info);

/******************************* CP狗超时复位事件接收&上报 ***************************/
void dump_cp_wdt_hook(unsigned int mod_id)
{
    int rpt_ret;
    u32 timestamp = bsp_get_slice_value();
    dump_base_info_s *baseinfo = NULL;
    dump_subsys_exc_info_s subsys_exc;
    bsp_wdt_irq_disable(WDT_CCORE_ID);
    baseinfo = dump_save_cp_base_info_wdt(mod_id);
    /* 应该在baseinfo准备好之后，立刻通知CP(因为baseinfo中保存了一些核间交互的信息，所以一定要等他完成) */
    dump_cp_async_exc_proc(mod_id);
    dump_fill_cp_exc_info(baseinfo, &subsys_exc);
    rpt_ret = (int)dump_report_exception(&subsys_exc);
    if (!dump_is_exc_rpt_accept(rpt_ret)) {
        dump_error("reject handle cp exc by %d\n", rpt_ret);
        return;
    }
    dump_print("[0x%x]modem cp wdt enter system error! \n", timestamp);

    bsp_coresight_stop_cp();
    /* 如果当前不是第一现场，不需要执行后面的步骤 */
    if (rpt_ret != DUMP_EXC_ACCEPT_NOW) {
        return;
    }
    /* bsp_adump_save_exc_scene应该在确定cp_wdt是第一次异常之后再调用，否则容易覆盖第一次异常的信息 */
    bsp_adump_stop_sche_record();
    bsp_adump_save_exc_scene(BSP_MODU_OTHER_CORE, 0, 0);
}

__init void dump_cp_wdt_init(void)
{
    int ret;
    ret = bsp_wdt_register_hook(WDT_CCORE_ID, dump_cp_wdt_hook);
    if (ret != BSP_OK) {
        dump_error("fail to register wdt hook\n");
    }
}

/******************************* CP主动复位事件接收&上报 ***************************/
void dump_cp_ipc_handler(void *arg)
{
    int ret, rpt_ret;
    dump_subsys_exc_info_s subsys_exc;
    dump_base_info_s *base_info = (dump_base_info_s *)bsp_dump_get_field_addr(DUMP_LRCCPU_BASE_INFO_SMP);
    u32 timestamp = bsp_get_slice_value();

    /* 不论本次异常是否被rdr接纳，都将cp_exc_flag置true，并关狗
     * 上报异常被拒绝的情况：1、当前正在cp 单独复位，2、或者已经有整机复位的异常了
     * 如果是case 1，
     *   1.1 发生在单独复位前，此时异常不需要处理，这里置位的标记，关闭的wdt；会在单独复位时重新打开
     *   1.2 发生在单独复位中，此时异常需要处理，但是当前机制是交由单独复位失败处理，而不是此时上报；关闭的wdt不影响功能
     * 如果是case 2，该异常不需要处理，关闭wdt也不影响功能
     */
    g_dump_cp_agent.cp_exc_flag = true; 
    bsp_wdt_irq_disable(WDT_CCORE_ID);

    ret = dump_fill_cp_exc_info(base_info, &subsys_exc); 

    rpt_ret = (int)dump_report_exception(&subsys_exc);
    if (!dump_is_exc_rpt_accept(rpt_ret)) {
        dump_error("reject handle cp exc by %d\n", rpt_ret);
        return;
    }

    if (ret != DUMP_OK) {
        dump_print("get cp exc info fail ret %d\n", ret);
    }
    dump_print("modem ccore enter system error! timestamp:0x%x\n", timestamp);
    /* 如果当前不是第一现场，不需要执行后面的步骤 */
    if (ret != DUMP_EXC_ACCEPT_NOW) {
        return;
    }
    bsp_adump_stop_sche_record();
    bsp_adump_save_exc_scene(BSP_MODU_OTHER_CORE, 0, 0);

    return;
}

__init int dump_cp_ipc_init(void)
{
    int ret;
    ipc_handle_t phandle = NULL;

    ret = bsp_ipc_open(IPC_ACPU_INT_SRC_TSP_DUMP, &phandle);
    if (ret != BSP_OK) {
        dump_print("open cp ipc fail\n");
        return DUMP_ERRNO_IPC_OPEN;
    }
    ret = bsp_ipc_connect(phandle, (ipc_cbk)dump_cp_ipc_handler, NULL);
    if (ret != BSP_OK) {
        dump_print("conn cp ipc fail\n");
        return DUMP_ERRNO_IPC_CPNNECT;
    }
    return DUMP_OK;
}

/******************************* pre-dump：通知CP ***************************/
void dump_notify_cp(u32 mod_id)
{
    int ret;
    ipc_handle_t phandle = NULL;

    ret = bsp_ipc_open(IPC_TSP_INT_SRC_ACPU_DUMP, &phandle);
    if (ret == BSP_OK) {
        if (phandle != NULL) {
            bsp_ipc_send(phandle);
        }
    }
    if (ret == BSP_OK) {
        dump_print("notify mdm ccore success\n");
    } else {
        dump_error("notify modem ccore fail,please let ipc check\n");
    }
}

void dump_cp_async_exc_proc(u32 mod_id)
{
    /* mark cp_exc_flag, so will not notify cp(proc in func dump_cp_notify_callback) later in rdr callback */
    if (!g_dump_cp_agent.cp_exc_flag) {
        g_dump_cp_agent.cp_exc_flag = true;
        dump_notify_cp(mod_id);
        dump_error("modem exc catch by ap!\n");
    }
}

int dump_cp_prepare_handler(const dump_exc_log_desc_s *exc_desc)
{
    u32 modid = 0;
    /* 不应该走到 */
    if (!dump_valid_exc_desc(exc_desc)) {
        dump_debug("exc_desc inv\n");
    } else {
        modid = exc_desc->exc_info->subsys_info.modid;
    }

    if (g_dump_cp_agent.cp_exc_flag == true) {
        dump_print("CP exception,no need to notify C core,modid=0x%x\n", modid);
        return DUMP_OK;
    }
    if (bsp_reset_ccore_is_reboot() != 0) {
        dump_print("modem is reseting now,do not notify\n");
        dump_set_save_done(DUMP_SUBSYS_CP);
        return DUMP_OK;
    }
    dump_notify_cp(modid);
    return DUMP_OK;
}

/******************************* 复位CP ***************************/
void dump_cp_area_reset(dump_area_id_e areaid)
{
    int ret;
    struct dump_area_mntn_addr_info_s area_info = { 0 };
    struct dump_area_s *area;
    ret = bsp_dump_get_area_info(areaid, &area_info);
    if (ret == BSP_OK && area_info.vaddr != NULL && area_info.paddr != NULL && area_info.len != 0) {
         area = (struct dump_area_s *)area_info.vaddr;
         area->area_head.magic_num = DUMP_LEVEL1_AREA_EXPIRED_MAGICNUM;
         area->area_head.share_info.done_flag = 0;
         dump_print("area[%d] reset done\n", areaid);
    }
}

int dump_check_reset_freq(const dump_exc_log_desc_s *exc_desc)
{
    u64 diff = 0;
    dump_cp_reset_s *reset_info = &g_dump_cp_agent.reset_info;

    if (reset_info->count % DUMP_CP_REST_TIME_COUNT == 0 && reset_info->count != 0) {
        diff = (reset_info->reset_time[DUMP_CP_REST_TIME_COUNT - 1] - reset_info->reset_time[0]);
        if (diff < DUMP_CP_REST_TIME_COUNT * DUMP_CP_REST_TIME_SLICE) {
            dump_error("stop modem single reset\n ");
            return BSP_ERROR;
        }

        if (memset_s(reset_info, sizeof(g_dump_cp_agent.reset_info), 0, sizeof(g_dump_cp_agent.reset_info)) != EOK) {
            dump_debug("err\n");
        }
    }
    if (exc_desc->exc_info->exc_cfg->exc_type != CP_S_NORMALRESET) {
        /* 该接口不会失败，不需要判断返回值 */
        (void)bsp_slice_getcurtime(&reset_info->reset_time[reset_info->count % DUMP_CP_REST_TIME_COUNT]);
        reset_info->count++;
    }
    return BSP_OK;
}

dump_reset_result_e dump_mdm_reset_proc(u32 modid)
{
    int ret;
    dump_print("enter dump reset, mod id:0x%x\n", modid);
    ret = bsp_cp_reset();
    if (ret != 0) {
        dump_error("mdm reset not support!\n");
        return DUMP_RESET_NOT_SUPPORT;
    }

    if (!bsp_reset_is_successful(RDR_MODEM_CP_RESET_TIMEOUT)) {
        /* 类似看门狗超时, 首先保存下C核当前trace和debug寄存器信息 */
        bsp_coresight_stop_cp();
        dump_error("mdm reset fail!\n");
        return DUMP_RESET_NOT_SUCCESS;
    }
    return DUMP_RESET_SUCCESS;
}

dump_reset_result_e dump_cp_reset_handler(const dump_exc_log_desc_s *exc_desc)
{
    int ret;
    u32 modid = 0xFFFFFFFF;

    if ((exc_desc != NULL) && (exc_desc->exc_info != NULL)) {
        modid = exc_desc->exc_info->rdr_info.modid;
    }
    /* 入参不用检查，调用者检查了 */
    if (g_dump_cp_agent.modem_off == DUMP_MODEM_OFF) {
        dump_error("modem already power off, should not do single reset.\n");
        return DUMP_RESET_SUCCESS;
    }

    ret = dump_check_reset_freq(exc_desc);
    if (ret != DUMP_OK) {
        /* 当前modem单独复位过多，直接关闭modem */
        if (g_dump_cp_agent.modem_off != DUMP_MODEM_OFF) {
            bsp_modem_power_off();
            g_dump_cp_agent.modem_off = DUMP_MODEM_OFF;
            dump_error("modem reset too many times,shut down\n");
        }
        /* 关modem之后，也需要标记当前异常处理完毕，这样之后调用system_error触发的整机复位，才能真正走下去；
         * 至于之后调用的单独复位，会因为g_dump_cp_agent.modem_off=DUMP_MODEM_OFF，而直接退出，不会重新打开modem
         */
        return DUMP_RESET_SUCCESS;
    }

    ret = dump_mdm_reset_proc(modid);
    return (dump_reset_result_e)ret;
}

int dump_cp_reset_before_cb(const dump_exc_log_desc_s *exc_desc)
{
    if (dump_valid_exc_desc(exc_desc)) {
        /* 单独复位前重置CP area，防止后续单独复位失败且CP在启动早期异常，dump未初始化时，AP获取到残留的CP信息，
        * 例如CPlog是否保存完成，一些CP_FIELD里用作交互的信息 */
        if (exc_desc->exc_info->exc_cfg->reset_subsys_mask & DUMP_SUBSYS_MASK_CP) {
            dump_cp_area_reset(DUMP_AREA_LR);
            dump_cp_area_reset(DUMP_AREA_MTEE);
        }
        if ((exc_desc->exc_info->subsys_info.e_from_subsys == DUMP_SUBSYS_CP)) {
            g_dump_cp_agent.cp_exc_flag = false;
            dump_print("cp_exc_flag clear!\n");
        }
    } else {
        /* 不应该走到这，但如果走到了，不知道当前是什么异常的情况下，都把CP的exc_flag清掉，这样后面再有cp异常的时候，也能通知到CP
         * 但是这里不能重置CP_AREA的valid标记，因为这里如果不是CP异常了，清理掉了的话，后续的CP异常，无法保存很多日志
         */
        if (g_dump_cp_agent.cp_exc_flag) {
            g_dump_cp_agent.cp_exc_flag = false;
            dump_print("cp_exc_flag clear!\n");
        }
    }
    return BSP_OK;
}

/******************************* dump_event：dump事件处理 ***************************/
int dump_cp_event_handler(int event, const dump_exc_log_desc_s *exc_desc)
{
    int ret = DUMP_OK;
    switch (event) {
        case DUMP_EVENT_RESET_START:
            ret = dump_cp_reset_before_cb(exc_desc);
            break;
        default:
            break;
    }
    return ret;
}

/* 允许多次调用 */
void dump_cp_subsys_drv_prepare(void)
{
    int ret;
    /* 使用默认workqueue和async work fn；work ctrl需要先初始化，后面注册work时会用到 */
    ret = dump_work_ctrl_factory(&g_dump_cp_agent.work_ctrl, NULL, NULL);
    if (ret != DUMP_OK) {
        dump_print("cp work ctrl init fail\n");
    }

    dump_get_cp_exc_cfg(&g_dump_cp_subsys.exc_cfg);
    dump_register_subsys(&g_dump_cp_subsys);
}

__init void dump_cp_subsys_init(void)
{
    int ret;

    if (bsp_reset_is_feature_on() == MDM_RESET_SUPPORT) {
        g_dump_cp_subsys.reset_ops = dump_cp_reset_handler;
    }

    ret = dump_cp_ipc_init();
    if (ret != BSP_OK) {
        dump_print("reg cp ipc fail\n");
    }
    dump_cp_wdt_init();

    dump_cp_log_init();

    g_dump_cp_agent.init_done = true;
    dump_print("dump cp init ok\n");
}