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

#include <dump_subsys_mgr.h>
#include <dump_infra.h>
#include <dump_modid.h>
#include "dump_rdr_wrapper.h"

/* 这里配置外部上报的异常，需要modem存log时，如何处理。最后一个是default处理方式 */
#ifdef BSP_CONFIG_PHONE_TYPE
const dump_exc_cfg_s g_dump_default_exc_cfg[] = {
    {
        .reset_subsys_mask = DUMP_SUBSYS_MASK_AP,
        .notify_subsys_mask = DUMP_SUBSYS_VALID_MASK,
        .exc_type = CP_S_MODEMAP,
        .src_modid_start = 0,
        .src_modid_end = 0xFFFFFFFF,
        .exc_modid = RDR_PHONE_MDMAP_PANIC_MOD_ID,
        .exc_desc = "mdmap panic",
    },
    {
        .reset_subsys_mask = 0,
        .notify_subsys_mask = 0,
        .exc_type = 0,
        .src_modid_start = 0,
        .src_modid_end = 0xFFFFFFFF,
        .exc_modid = 0xFFFFFFFE,
        .exc_desc = "other sys dump/rst mdm",
    }
};
#else
const dump_exc_cfg_s g_dump_default_exc_cfg[] = {
    {
        .reset_subsys_mask = DUMP_SUBSYS_MASK_AP,
        .notify_subsys_mask = DUMP_SUBSYS_VALID_MASK,
        .exc_type = 0,
        .src_modid_start = 0,
        .src_modid_end = 0xFFFFFFFF,
        .exc_modid = 0xFFFFFFFE,
        .exc_desc = "other sys dump/rst mdm",
    }
};
#endif

const dump_exc_cfg_s *dump_get_defult_exc_cfg(u32 rdr_modid)
{
    u32 i;
    const u32 size = sizeof(g_dump_default_exc_cfg) / sizeof(g_dump_default_exc_cfg[0]);
    for (i = 0; i < size; i++) {
        if (rdr_modid == g_dump_default_exc_cfg[i].exc_modid) {
            return &g_dump_default_exc_cfg[i];
        }
    }
    return &g_dump_default_exc_cfg[size - 1];
}