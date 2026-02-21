/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2023. All rights reserved.
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
#ifndef __OCD_H__
#define __OCD_H__

#include <product_config.h>

#include <osl_types.h>
#include <osl_cache.h>
#include <osl_malloc.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/slab.h>
#include <mdrv_print.h>
#include <bsp_dt.h>
#include <bsp_om_enum.h>
#include <bsp_coresight.h>
#include <bsp_dump.h>
#include <bsp_nvim.h>
#include <bsp_version.h>
#include <securec.h>

#define DPA_CORE_NUMS 1
#define DPA_OCD_NUMS 2
#define DPA_THREAD_MAX 4

#define LTRACE_ENABLED 1
#define LTRACE_DISABLED 0

#define LTRACE_SAVE 1
#define LTRACE_NOSAVE 0

#define LTRACE_DATA_WIDTH 4
#define FIELD_NAMELEN_MAX 12

#define DSS_REGS_OFFSET(dss) (0x40000 + (dss) * 0x40000)
#define THREAD_REGS_OFFSET(thread) (0x20000 + (thread) * 0x8000)
#define OCD_REGS_OFFSET 0x1000

#define DSS_BASEADDR(base, dss) (void *)((uintptr_t)(base) + DSS_REGS_OFFSET(dss))

#define THREAD_BASEADDR(base, dss, thread) (void *)(DSS_BASEADDR(base, dss) + THREAD_REGS_OFFSET(thread))

#define OCD_BASEADDR(base, dss, thread) (void *)(THREAD_BASEADDR(base, dss, thread) + OCD_REGS_OFFSET)

#define OCD_PTRACE_START(N) (0x60C + ((unsigned long)(N) * 0x14))
#define OCD_PTRACE_END(N) (0x610 + ((unsigned long)(N) * 0x14))

#define CS_CLK_ENABLE 1
#define CS_CLK_DISABLE 0

#define SCU_CLK_DIS 0x80
#define OCD_CLK_BIT (1U << 1)
#define OCD_DSS0_SCU_REG_SHARE_BASE 0x4D000

enum ocd_version_e {
    OCD_VERSION_LTRACE = 0,
    OCD_VERSION_ETRACE,
    OCD_VERSION_LTRACE_V300,
    OCD_VERSION_MAX,
};
struct trace_data_head_info {
    unsigned int magic;
    unsigned int length;
};

enum ocd_trace_status_e {
    DPA_OCD_SUSPEND_PROCESS = 0,
    DPA_OCD_NORMAL_PROCESS = 1,
    DPA_OCD_DUMP_PROCESS = DPA_OCD_NORMAL_PROCESS,
};
// 0x10 >= struct trace_data_head_info
#define GET_TRACE_DUMP_LEN(len) ((len) + 0x10)

struct ocd_ltrace_info_s {
    void *base;
    int size;
    spinlock_t spinlock;
    u32 cpu;
    u32 thread;
    bool enable;
    bool os_unlock;
    char *buf;
    u32 process;
    u32 is_wrapped;
    u32 wrptr;
};
struct ocd_ltrace_cfg_s {
    u32 disable_cfg;
    u32 enable_cfg;
    u32 version;
    u32 buf_len;
    void *tsp_base;
};
void ocd_ltrace_save_all(void);
void ocd_ltrace_stop_all(void);
void ocd_ltrace_resume(void);
#endif