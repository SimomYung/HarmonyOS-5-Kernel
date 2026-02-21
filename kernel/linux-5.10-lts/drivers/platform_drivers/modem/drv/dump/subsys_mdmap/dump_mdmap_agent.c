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
#include <securec.h>
#include <osl_types.h>
#include <linux/kernel.h>
#include <bsp_dump.h>
#include <bsp_adump.h>
#include <bsp_slice.h>
#include <dump_rdr_wrapper.h>
#include <dump_frame.h>
#include <dump_subsys_mgr.h>
#include <dump_infra.h>
#include <dump_config.h>
#include <dump_modid.h>
#include "dump_baseinfo.h"
#include "dump_mdmap_log.h"
#include "dump_mdmap_exc_cfg.h"
#include "dump_mdmap_agent.h"

int dump_mdmap_prepare_handler(const dump_exc_log_desc_s *exc_desc);
int dump_mdmap_event_handler(int event, const dump_exc_log_desc_s *exc_desc);

dump_mdmap_agent_ctrl_s g_dump_mdmap_agent;
__ro_after_init dump_subsys_drv_s g_dump_mdmap_subsys = {
    .dump_sys = DUMP_SUBSYS_MDMAP,
    .name = "MDMAP",
    /* g_dump_mdmap_exc_cfg 在另一个文件中声名；也方便后续版本有差异时，拆分到hal层
    .exc_cfg = {
        .num = ARRAY_SIZE(g_dump_mdmap_exc_cfg),
        .cfg_list = &g_dump_mdmap_exc_cfg,
    },
    */
    .work_ctrl = &g_dump_mdmap_agent.work_ctrl,
    .dump_prepare_handler = dump_mdmap_prepare_handler,
    .reset_ops = NULL, /* 不支持单独复位 */
    .event_handler = dump_mdmap_event_handler,
};

void dump_fill_mdmap_exc_ctx_info(dump_subsys_exc_info_s *subsys_exc)
{
    int len;
    (void)memset_s(subsys_exc->task_name, DUMP_EXCINFO_TASKNAME_LEN, 0, DUMP_EXCINFO_TASKNAME_LEN);
    if (in_interrupt()) {
        subsys_exc->reboot_context = DUMP_CTX_INT;
        subsys_exc->int_no = 0xAAAABBBB;
    } else {
        subsys_exc->reboot_context = DUMP_CTX_TASK;
        subsys_exc->task_id = current->pid;
        len = strlen(current->comm);
        len = (len >= DUMP_EXCINFO_TASKNAME_LEN) ? DUMP_EXCINFO_TASKNAME_LEN : len;
        if (memcpy_s(subsys_exc->task_name, sizeof(subsys_exc->task_name), current->comm, len) != EOK) {
            dump_debug("err\n");
        }
        subsys_exc->task_name[DUMP_EXCINFO_TASKNAME_LEN - 1] = '\0';
    }
}

/******************************* mdmap异常检测 ***************************/
void system_error(u32 mod_id, u32 arg1, u32 arg2, char *data, u32 length)
{
    int rpt_ret;
    dump_subsys_exc_info_s subsys_exc;
    u32 timestamp = bsp_get_slice_value();
    const dump_exc_cfg_s *curr_exc_cfg = NULL;

#ifndef BSP_CONFIG_PHONE_TYPE
    if (!dump_is_init_done()) {
        panic("call system_error when dump not init, modid=0x%x, arg1=0x%x, arg2=0x%x", mod_id, arg1, arg2);
    }
#endif

    subsys_exc.e_from_subsys = DUMP_SUBSYS_MDMAP;
    subsys_exc.reason = DUMP_REASON_NORMAL;
    subsys_exc.modid = mod_id;
    subsys_exc.arg1 = arg1;
    subsys_exc.arg2 = arg2;
    subsys_exc.data = data;
    subsys_exc.length = length;
    dump_fill_mdmap_exc_ctx_info(&subsys_exc); 
    
    rpt_ret = (int)dump_report_exception(&subsys_exc);
    if (!dump_is_exc_rpt_accept(rpt_ret)) {
        dump_error("reject handle mdmap exc 0x%x by %d\n", mod_id, rpt_ret);
        return;
    }

    g_dump_mdmap_agent.mdmap_exc_flag = true;

    dump_print("modem acore enter system error! timestamp:0x%x\n", timestamp);
    dump_print("mod_id=0x%x arg1=0x%x arg2=0x%x  len=0x%x\n", mod_id, arg1, arg2, length);

    /* 如果当前不是accept_now，就不保存user-data等信息了 */
    if (rpt_ret != DUMP_EXC_ACCEPT_NOW) {
        return;
    }

    curr_exc_cfg = dump_get_subsys_exc_cfg((const dump_subsys_drv_s *)&g_dump_mdmap_subsys, mod_id);
    /* 当前异常不需要保存log */
    if (curr_exc_cfg->notify_subsys_mask == 0) {
        return;
    }

    bsp_adump_stop_sche_record();
    dump_save_usr_data(data, length);
    bsp_adump_save_exc_scene(mod_id, 0, 0);
    dump_save_base_info(mod_id, arg1, arg2, data, length, NULL);

    if (bsp_dump_get_product_type() == DUMP_PHONE) {
        dump_show_stack(mod_id, arg1);
    }
    return;
}
EXPORT_SYMBOL(system_error);

int dump_mdmap_prepare_handler(const dump_exc_log_desc_s *exc_desc)
{
    const dump_subsys_exc_info_s *subsys_info = NULL;
    if (dump_valid_exc_desc(exc_desc)) {
        dump_save_rdr_baseinfo(exc_desc);
        subsys_info = &exc_desc->exc_info->subsys_info;
        if ((subsys_info->e_from_subsys == DUMP_SUBSYS_MDMAP) &&
            (exc_desc->exc_info->rpt_status == DUMP_EXC_ACCEPT_LATER)) {
            dump_save_base_info(subsys_info->modid, subsys_info->arg1, subsys_info->arg2, subsys_info->data,
                subsys_info->length, (subsys_info->reboot_context == DUMP_CTX_TASK) ? subsys_info->task_name : NULL);
            bsp_adump_save_exc_scene(subsys_info->modid, subsys_info->arg1, subsys_info->arg2);
        } else if (subsys_info->e_from_subsys != DUMP_SUBSYS_MDMAP) {
            dump_save_base_info(BSP_MODU_OTHER_CORE, 0, 0, NULL, 0, NULL);
        }
    } else {
        dump_debug("exc_desc inv\n");
        dump_save_base_info(BSP_MODU_OTHER_CORE, 0, 0, NULL, 0, NULL);
    }

    return BSP_OK;
}

/******************************* dump_event：dump事件处理 ***************************/
int dump_mdmap_event_handler(int event, const dump_exc_log_desc_s *exc_desc)
{
    int ret = DUMP_OK;
    switch (event) {
        /* 整机复位时，不需要清除baseinfo等标记，因此只要在reset start阶段处理就行 */
        case DUMP_EVENT_RESET_START:
            if (dump_valid_exc_desc(exc_desc) && !(exc_desc->exc_info->subsys_info.e_from_subsys == DUMP_SUBSYS_MDMAP) &&
                g_dump_mdmap_agent.mdmap_exc_flag) {
                g_dump_mdmap_agent.mdmap_exc_flag = false;
                dump_print("mdmap_exc_flag clear!\n");
            }
            dump_base_info_reset();
            break;
        case DUMP_EVENT_EXC_HANDLE_DONE:
            break;
        default:
            break;
    }
    return ret;
}

/* 允许多次调用 */
void dump_mdmap_subsys_drv_prepare(void)
{
    int ret;
    /* 使用默认workqueue和async work fn；work ctrl需要先初始化，后面注册work时会用到 */
    ret = dump_work_ctrl_factory(&g_dump_mdmap_agent.work_ctrl, NULL, NULL);
    if (ret != DUMP_OK) {
        dump_print("mdmap work ctrl init fail\n");
    }

    dump_get_mdmap_exc_cfg(&g_dump_mdmap_subsys.exc_cfg);
    dump_register_subsys(&g_dump_mdmap_subsys);
}

__init void dump_mdmap_subsys_init(void)
{
    void *addr = NULL;
    int ret;
    u32 usr_data_size = dump_get_u32_cfg(DUMP_CFG_USERDATA_SIZE);

    addr = bsp_dump_register_field(DUMP_MODEMAP_USER_DATA, "USER_DATA", usr_data_size, 0);
    if (addr == NULL) {
        dump_error("fail to get mdmap user data field addr\n");
    }


    ret = dump_base_info_init();
    if (ret) {
        dump_error("mdmap base info is null\n");
    }

    dump_mdmap_log_init();

    g_dump_mdmap_agent.init_state = true;
    dump_print("dump mdmap init ok\n");
}
