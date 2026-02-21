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

#include <osl_types.h>
#include <mdrv_memory_layout.h>
#include <bsp_dump.h>
#include <dump_exc_mgr.h>
#include <dump_subsys_mgr.h>
#include <dump_infra.h>
#include <dump_area.h>
#include <dump_logs.h>
#include "dump_reset_log.h"
#include "dump_avs_escape.h"
#include "dump_sec_log.h"
#include "dump_comm_log.h"

/******************************* common_dump_works ***************************/
void dump_save_mntn_bin(const char *dir_name)
{
    struct dump_global_area_ctrl_s global_area = { 0 };
    int ret;
    ret = dump_get_global_info(&global_area);
    if (ret == BSP_OK && global_area.virt_addr != NULL) {
        dump_log_save(dir_name, "modem_dump.bin", (u8 *)global_area.virt_addr, (u8 *)(uintptr_t)global_area.phy_addr,
            global_area.length);
        dump_print("save modem_dump.bin finished\n");
    }
}

void dump_save_mntn_backup(const char *dir_name)
{
    int ret;
    struct dump_global_area_ctrl_s global_area = { 0 };
    dump_save_last_kmsg();
    ret = dump_get_global_info(&global_area);
    if (ret == BSP_OK && global_area.virt_addr != NULL) {
        dump_log_save(dir_name, "modem_dump_backup.bin", (u8 *)global_area.virt_addr,
            (u8 *)(uintptr_t)global_area.phy_addr, global_area.length);
        dump_print("save modem_dump_backup.bin finished\n");
    }
}

void dump_common_done(const dump_exc_log_desc_s *exc_info, void *priv_data)
{
    UNUSED(exc_info);
    UNUSED(priv_data);
    dump_set_save_done(DUMP_SUBSYS_COMM);
    dump_print("comm_callback finish\n");
}

void dump_common_wait_other_subsys_done(const dump_exc_log_desc_s *exc_desc, void *priv_data)
{
    u64 online_subsys = dump_get_online_subsys();
    u64 notify_subsys_mask = 0;
    int time = DUMP_MSLEEP_35;

    UNUSED(priv_data);
    if (dump_valid_exc_desc(exc_desc)) {
        notify_subsys_mask = (exc_desc->exc_info->exc_cfg->notify_subsys_mask & online_subsys) & (~DUMP_SUBSYS_MASK_COMM);
        dump_wait_done(&exc_desc->dump_done_subsys, notify_subsys_mask, &time);
    } else {
        dump_debug("exc_desc inv\n");
    }
    dump_print("commsubsys wait other subsys finish\n");
}

__init void dump_comm_log_init(void)
{
    dump_work_s work;

    work.work_type = DUMP_WORK_ASYNC;
    work.work_fn = dump_common_wait_other_subsys_done;
    work.prior = DUMP_WORK_PRIO_ASYNC_HOOK_WAIT_DONE;
    work.priv_data = NULL;
    dump_subsys_add_work(DUMP_SUBSYS_COMM, &work);

    work.work_type = DUMP_WORK_ASYNC;
    work.work_fn = dump_common_done;
    work.prior = DUMP_WORK_PRIO_ASYNC_HOOK_SET_SUBSYS_DONE;
    work.priv_data = NULL;
    dump_subsys_add_work(DUMP_SUBSYS_COMM, &work);

    dump_reset_log_init();
    dump_avs_escape_init();
    dump_sec_log_init();
    bsp_dump_register_log_notifier(DUMP_SAVE_MDM_DUMP, dump_save_mntn_bin);
    bsp_dump_register_log_notifier(DUMP_SAVE_MDM_DUMP_BACKUP, dump_save_mntn_backup);
}
