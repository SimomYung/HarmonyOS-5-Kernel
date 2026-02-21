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
#include <linux/sched.h>
#include <linux/timekeeping.h>
#include <linux/rtc.h>
#include <bsp_dump.h>
#include <bsp_slice.h>
#include <bsp_uuid.h>
#include <bsp_modem_patch.h>
#include <dump_subsys_mgr.h>
#include <dump_exc_mgr.h>
#include <dump_infra.h>
#include "dump_area.h"
#include "dump_baseinfo.h"

dump_base_info_s *g_mdm_dump_base_info = NULL;

void dump_save_int_contex_info(void)
{
    if (g_mdm_dump_base_info == NULL) {
        return;
    }
    g_mdm_dump_base_info->reboot_task = (u32)(-1);
    if (memset_s(g_mdm_dump_base_info->task_name, sizeof(g_mdm_dump_base_info->task_name), 0,
        sizeof(g_mdm_dump_base_info->task_name)) != EOK) {
        dump_debug("err\n");
    }

    if (bsp_dump_get_product_type() == DUMP_MBB) {
    } else {
        g_mdm_dump_base_info->reboot_int = 0xAAAABBBB;
    }
    g_mdm_dump_base_info->reboot_context = DUMP_CTX_INT;
}

void dump_save_task_contex_info(void)
{
    struct task_struct *task = NULL;

    if (g_mdm_dump_base_info == NULL) {
        return;
    }

    g_mdm_dump_base_info->reboot_task_tcb = (u32)(uintptr_t)current;
    if (g_mdm_dump_base_info->mod_id == 0x11000025 || g_mdm_dump_base_info->mod_id == 0x1100002A) {
        /* A核VOS只记录的任务的pid */
        g_mdm_dump_base_info->reboot_task_tcb = g_mdm_dump_base_info->arg1;
        task = find_task_by_vpid(g_mdm_dump_base_info->arg1);
    } else {
        g_mdm_dump_base_info->reboot_task = (u32)(((struct task_struct *)(current))->pid);
        task = (struct task_struct *)(current);
    }
    if (task != NULL) {
        if (memset_s(g_mdm_dump_base_info->task_name, sizeof(g_mdm_dump_base_info->task_name), 0,
            sizeof(g_mdm_dump_base_info->task_name)) != EOK) {
            dump_debug("err\n");
        }
        if (memcpy_s(g_mdm_dump_base_info->task_name, sizeof(g_mdm_dump_base_info->task_name),
            ((struct task_struct *)(task))->comm, strlen(((struct task_struct *)(task))->comm)) != EOK) {
            dump_debug("err\n");
        }
        dump_print("exception task name is %s\n", g_mdm_dump_base_info->task_name);
    }
    g_mdm_dump_base_info->reboot_int = (u32)(-1);
    g_mdm_dump_base_info->reboot_context = DUMP_CTX_TASK;
}

void dump_save_exc_contex(void)
{
    if (g_mdm_dump_base_info == NULL) {
        return;
    }

    if (g_mdm_dump_base_info->mod_id == BSP_MODU_OTHER_CORE) {
        g_mdm_dump_base_info->reboot_task = (u32)(-1);
        g_mdm_dump_base_info->reboot_int = (u32)(-1);
    } else {
        if (in_interrupt()) {
            dump_save_int_contex_info();
        } else {
            dump_save_task_contex_info();
        }
    }

    return;
}

void dump_save_base_info(u32 mod_id, u32 arg1, u32 arg2, const char *data, u32 length, const char *sence)
{
    u32 len = 0;
    if (unlikely(g_mdm_dump_base_info == NULL)) {
        return;
    }

    g_mdm_dump_base_info->mod_id = mod_id;
    g_mdm_dump_base_info->arg1 = arg1;
    g_mdm_dump_base_info->arg2 = arg2;
    g_mdm_dump_base_info->arg3 = (u32)(uintptr_t)data;
    g_mdm_dump_base_info->arg3_length = length;
    g_mdm_dump_base_info->reboot_time = bsp_get_slice_value();

    if (unlikely(sence != NULL)) {
        len = strlen(sence);
        if (memcpy_s(g_mdm_dump_base_info->task_name, sizeof(g_mdm_dump_base_info->task_name), sence,
            len > DUMP_BASEINFO_SENCE_LEN ? DUMP_BASEINFO_SENCE_LEN : len) != EOK) {
            dump_debug("err\n");
        }
        g_mdm_dump_base_info->reboot_context = DUMP_CTX_TASK;
        g_mdm_dump_base_info->reboot_int = 0xFFFFFFFF;
    } else {
        dump_save_exc_contex();
    }
    dump_print("input modid = 0x%x\n", mod_id);

    return;
}

void dump_save_rdr_baseinfo(const dump_exc_log_desc_s *exc_desc)
{
    struct dump_global_base_info_s *global_base_info = (struct dump_global_base_info_s *)dump_get_global_baseinfo();
    struct dump_global_top_head_s *top_head = (struct dump_global_top_head_s *)dump_get_rdr_top_head();
    const dump_rdr_exc_info_s *rdr_exc_info = &exc_desc->exc_info->rdr_info;
    const dump_subsys_drv_s *subsys = dump_get_subsys(exc_desc->exc_info->subsys_info.e_from_subsys);
    const char *subsys_name = "unknown";
    time64_t txc = 0;
    struct rtc_time tm = { 0 };
    char temp[DUMP_DATETIME_LEN] = {0};

    if (global_base_info == NULL) {
        dump_error("fail to get global_base_info\n");
        return;
    }
    if (subsys != NULL) {
        subsys_name = (subsys->name == NULL) ? "unknown" : subsys->name;
    }

    txc = ktime_get_real_seconds();
    rtc_time64_to_tm(txc, &tm);
    if (snprintf_s(temp, sizeof(temp), (sizeof(temp) - 1), "%04d-%02d-%02d %02d:%02d:%02d",
        tm.tm_year + DUMP_LOG_YEAR_START, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec) < 0) {
        dump_debug("er\n");
    }

    (void)memset_s(global_base_info, sizeof(*global_base_info), 0, sizeof(*global_base_info));
    global_base_info->modid = rdr_exc_info->modid;
    global_base_info->arg1 = rdr_exc_info->arg1;
    global_base_info->arg2 = rdr_exc_info->arg2;
    if (memcpy_s(global_base_info->e_module, sizeof(global_base_info->e_module), subsys_name,
        strnlen(subsys_name, DUMP_NAME_LEN)) != EOK) {
        dump_debug("err\n");
    }
    global_base_info->e_module[DUMP_NAME_LEN - 1] = '\0';
    global_base_info->e_type = rdr_exc_info->etype;

    global_base_info->e_core = dump_subsys_to_inner_rdr_core(exc_desc->exc_info->subsys_info.e_from_subsys);
    global_base_info->start_flag = DUMP_START_EXCH;
    global_base_info->savefile_flag = DUMP_SAVE_FILE_END;
    (void)memset_s(global_base_info->e_desc, sizeof(global_base_info->e_desc), 0, sizeof(global_base_info->e_desc));
    if (memcpy_s(global_base_info->e_desc, sizeof(global_base_info->e_desc), exc_desc->exc_info->exc_cfg->exc_desc,
        DUMP_EXC_DESC_SIZE < DUMP_DESC_LEN ? DUMP_EXC_DESC_SIZE : DUMP_DESC_LEN - 1) != EOK) {
        dump_debug("err\n");
    }
    if (memcpy_s(global_base_info->datetime, sizeof(global_base_info->datetime), temp,
        (unsigned long)DUMP_DATETIME_LEN) != EOK) {
        dump_debug("err\n");
    }
    global_base_info->timestamp = bsp_get_slice_value();
    if (top_head != NULL) {
        top_head->codepatch = bsp_modem_cold_patch_is_exist();
    }

    dump_print("save rdr global info ok\n");
}

int dump_base_info_reset(void)
{
    if (g_mdm_dump_base_info == NULL) {
        return BSP_ERROR;
    }
    g_mdm_dump_base_info->mod_id = (u32)(-1);
    g_mdm_dump_base_info->arg1 = 0;
    g_mdm_dump_base_info->arg2 = 0;
    g_mdm_dump_base_info->arg3 = 0;
    g_mdm_dump_base_info->arg3_length = 0;
    g_mdm_dump_base_info->reboot_time = 0;
    g_mdm_dump_base_info->reboot_task = (u32)(-1);
    g_mdm_dump_base_info->reboot_int = (u32)(-1);
    g_mdm_dump_base_info->reboot_context = 0;
    (void)memset_s(g_mdm_dump_base_info->task_name, sizeof(g_mdm_dump_base_info->task_name), 0,
                   sizeof(g_mdm_dump_base_info->task_name));
    g_mdm_dump_base_info->vec = 0xff;
    return DUMP_OK;
}

__init int dump_base_info_init(void)
{
    g_mdm_dump_base_info = (dump_base_info_s *)bsp_dump_register_field(DUMP_MODEMAP_BASE_INFO_SMP, "BASE_INFO",
        sizeof(dump_base_info_s), 0);
    if (memset_s(g_mdm_dump_base_info, sizeof(dump_base_info_s), 0, sizeof(dump_base_info_s)) != EOK) {
        dump_debug("err\n");
    }
    if (strcpy_s((char *)g_mdm_dump_base_info->version_uuid, sizeof(g_mdm_dump_base_info->version_uuid),
                 get_modem_uuid())) {
        dump_debug("err\n");
    }
    g_mdm_dump_base_info->mod_id = (u32)(-1);
    g_mdm_dump_base_info->vec = 0xff;
    dump_print("base info init ok\n");
    return BSP_OK;
}
