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

#ifndef __DUMP_LPM_AGENT_H__
#define __DUMP_LPM_AGENT_H__

#include <product_config.h>
#include <osl_types.h>
#include <dump_works.h>

#define DUMP_MDMLPM_BASE_INFO 0x0d000000
#define DUMP_AREA_MAGICNUM 0x4e656464

typedef struct {
    bool lpmcu_exc_flag;
    bool init_state;
    dump_work_ctrl_s work_ctrl;
} dump_lpmcu_agent_ctrl_s;

#ifdef ENABLE_BUILD_DUMP_MDM_LPM3
void dump_lpmcu_subsys_drv_prepare(void);
void dump_lpmcu_subsys_init(void);
/* 若某个异常需要子系统A复位，但是子系统A又不支持单独复位，那么就配置成整机复位 */
void dump_lpmcu_subsys_update_exc_cfg(u64 reset_support_subsysmask);
#else
static inline void dump_lpmcu_subsys_drv_prepare(void)
{
    return;
}
static inline void dump_lpmcu_subsys_init(void)
{
    return;
}
/* 若某个异常需要子系统A复位，但是子系统A又不支持单独复位，那么就配置成整机复位 */
static inline void dump_lpmcu_subsys_update_exc_cfg(u64 reset_support_subsysmask)
{
    return;
}
#endif

#endif
