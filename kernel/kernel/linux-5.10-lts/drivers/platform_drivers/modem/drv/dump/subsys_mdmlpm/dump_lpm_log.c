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
#include <linux/delay.h>
#include <mdrv_memory_layout.h>
#include <bsp_dump.h>
#include <bsp_dump_mem.h>
#include <bsp_slice.h>
#include <dump_rdr_wrapper.h>
#include <dump_subsys_mgr.h>
#include <dump_frame.h>
#include <dump_infra.h>
#include <dump_log_strategy.h>
#include <dump_logs.h>
#include "dump_lpm_log.h"

struct dump_area_s *dump_get_lpmcu_area(void)
{
    int ret;
    struct dump_area_mntn_addr_info_s area_info = { NULL, NULL, 0 };

    ret = bsp_dump_get_area_info(DUMP_AREA_LPM3, &area_info);
    if (ret != BSP_OK) {
        dump_error("fail to find lpmcu area head\n");
        return NULL;
    }
    return (struct dump_area_s *)area_info.vaddr;
}

int dump_get_lpmcu_save_done(void)
{
    int flag = 0;
    struct dump_area_s *lpmcu_area = dump_get_lpmcu_area();

    if (lpmcu_area == NULL) {
        return DUMP_ERRNO_GET_DUMP_MEM_AREA;
    }
    flag = lpmcu_area->area_head.share_info.done_flag;
    if (flag == DUMP_SAVE_SUCCESS) {
        return DUMP_OK;
    }
    return DUMP_ERROR;
}

void dump_clear_lpmcu_save_done(void)
{
    struct dump_area_s *lpmcu_area = dump_get_lpmcu_area();

    if (lpmcu_area == NULL) {
        return;
    }
    lpmcu_area->area_head.share_info.done_flag = 0;
    return;
}

/******************************* lpmcu_dump_works:lpmcu dump真正要做的事情 ***************************/
int dump_wait_lpmcu_save_done(u32 ms)
{
    u32 time_start = bsp_get_elapse_ms();

    do {
        if (dump_get_lpmcu_save_done() == DUMP_OK) {
            dump_print("lpmcu save done\n");
            return BSP_OK;
        }

        if (ms <= (bsp_get_elapse_ms() - time_start)) {
            dump_error("wait lpmcu done timeout!\n");
            return BSP_ERROR;
        }
        msleep(DUMP_SLEEP_5MS_NUM);
    } while (1);
    return BSP_ERROR;
}

void dump_lpmcu_wait_done(const dump_exc_log_desc_s *exc_info, void *priv_data)
{
    UNUSED(exc_info);
    UNUSED(priv_data);
    dump_print("begin to wait lpmcu save done\n");
    if (dump_is_minidump()) {
        (void)dump_wait_lpmcu_save_done(DUMP_WAIT_1S_NUM);
    }
    dump_print("lpmcu_callback finish\n");
    return;
}

void dump_lpmcu_done(const dump_exc_log_desc_s *exc_info, void *priv_data)
{
    UNUSED(exc_info);
    UNUSED(priv_data);
    dump_set_save_done(DUMP_SUBSYS_MDMLPM);
    dump_print("lpmcu_callback finish\n");
}

__init void dump_lpmcu_log_init(void)
{
    dump_work_s work;

    work.work_type = DUMP_WORK_ASYNC;
    work.work_fn = dump_lpmcu_done;
    work.prior = DUMP_WORK_PRIO_ASYNC_HOOK_SET_SUBSYS_DONE;
    work.priv_data = NULL;
    dump_subsys_add_work(DUMP_SUBSYS_MDMLPM, &work);

    if (!dump_is_minidump()) {
        return;
    }

    work.work_fn = dump_lpmcu_wait_done;
    work.prior = DUMP_WORK_PRIO_ASYNC_HOOK_WAIT_DONE;
    dump_subsys_add_work(DUMP_SUBSYS_MDMLPM, &work);
}
