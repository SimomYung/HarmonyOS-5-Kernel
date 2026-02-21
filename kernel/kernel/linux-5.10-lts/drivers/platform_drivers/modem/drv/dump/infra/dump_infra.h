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
#ifndef _DUMP_INFRA_H_
#define _DUMP_INFRA_H_

#include <linux/sched.h>
#include <bsp_print.h>
#include "dump_errno.h"

#undef THIS_MODU
#define THIS_MODU mod_dump

#define dump_info(fmt, ...) (bsp_info("[L%d] " fmt, __LINE__, ##__VA_ARGS__))
#define dump_debug(fmt, ...) (bsp_debug("<%s> " fmt, __FUNCTION__, ##__VA_ARGS__))
#define dump_warning(fmt, ...) (bsp_wrn("[L%d] " fmt, __LINE__, ##__VA_ARGS__))
#define dump_error(fmt, ...) (bsp_err("<%s> " fmt, __FUNCTION__, ##__VA_ARGS__))
#define dump_print(fmt, ...) (bsp_err(fmt, ##__VA_ARGS__))

#define DUMP_BIT(nr) (1UL << (nr))

#define DUMP_MSLEEP_60 600
#define DUMP_MSLEEP_35 350
#define DUMP_SLEEP_5MS_NUM 5
#define DUMP_SLEEP_30MS_NUM 30
#define DUMP_WAIT_30S_NUM 30000 /* 30000 ms */
#define DUMP_WAIT_15S_NUM 15000 /* 15000 ms */
#define DUMP_WAIT_5S_NUM 5000   /* 5000 ms */
#define DUMP_WAIT_3S_NUM 3000   /* 3000 ms */
#define DUMP_WAIT_2S_NUM 2000
#define DUMP_WAIT_1S_NUM 1000

#define RDR_DUMP_FILE_PATH_LEN 128
#define RDR_DUMP_FILE_CP_PATH "cp_log/"
#define DUMP_SAVE_FLAG "CP_DONE"
#define RDR_DUMP_DIR_PTH_LEN (RDR_DUMP_FILE_PATH_LEN - strlen(RDR_DUMP_FILE_CP_PATH))

#define DUMP_LOG_YEAR_START 1900
#define DUMP_MAX_MODULE_LEN 16

static inline void dump_wait_done(volatile const u64 *flag, u64 expect_flag, int *time)
{
    while (((*flag) & expect_flag) != expect_flag) {
        set_current_state(TASK_INTERRUPTIBLE);
        (void)schedule_timeout(HZ / 10); /* 10 means wait 100ms */
        if (((*time)--) <= 0) {
            break;
        }
    }
}

#ifdef DRV_BUILD_LLT
#include <linux/export.h>
#define DUMP_LLT_EXPORT_SYMBOL(x) EXPORT_SYMBOL(x)
#else
#define DUMP_LLT_EXPORT_SYMBOL(x)
#endif

#endif