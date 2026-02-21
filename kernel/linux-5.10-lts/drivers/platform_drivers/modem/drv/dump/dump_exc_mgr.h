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

#ifndef _DUMP_EXC_MGR_H_
#define _DUMP_EXC_MGR_H_

#include <osl_types.h>
#include "dump_exception.h"
#include "dump_subsys.h"

static inline bool dump_is_exc_rpt_accept(enum dump_exc_rpt_ret ret)
{
    if (ret == DUMP_EXC_ACCEPT_NOW || ret == DUMP_EXC_ACCEPT_LATER) {
        return true;
    }
    return false;
}

static inline bool dump_valid_exc_desc(const dump_exc_log_desc_s *exc_desc)
{
    if (unlikely(exc_desc == NULL)) {
        return false;
    }
    if (unlikely(exc_desc->exc_info == NULL)) {
        return false;
    }
    if (unlikely(exc_desc->exc_info->exc_cfg == NULL)) {
        return false;
    }
    return true;
}

static inline bool dump_valid_exc_log_path(const dump_exc_log_desc_s *exc_desc)
{
    unsigned long ret;
    if (unlikely(exc_desc == NULL)) {
        return false;
    }
    ret = strnlen(exc_desc->log_path, RDR_DUMP_FILE_PATH_LEN);
    if (ret == 0 || ret == RDR_DUMP_FILE_PATH_LEN) {
        return false;
    }
    return true;
}

/* exception interface */
int dump_register_exception(const dump_subsys_drv_s *subsys);
/* 异常上报接口1：直接上报异常 */
enum dump_exc_rpt_ret dump_report_exception(dump_subsys_exc_info_s *subsys_exc);
/* 异常上报接口2：先lock，后更新信息并report */
enum dump_exc_rpt_ret dump_lock_exception(dump_subsys_exc_info_s *subsys_exc);
int dump_update_and_report_exception(dump_subsys_exc_info_s *subsys_exc);
/* 获取异常配置信息，没找到modid时，使用最后该子系统最后一个配置作为默认值；注意，若该子系统没有exc_cfg，则会返回NULL */
const dump_exc_cfg_s *dump_get_subsys_exc_cfg(const dump_subsys_drv_s *subsys, u32 modid);

/* 给dump frame的接口 */
dump_exc_info_s *dump_get_current_exception_by_rdrid(u32 modid);
/* 如果通过rdr id get不到异常信息，则说明是ap检测到的异常，需要modem dump/reset */
dump_exc_info_s *dump_report_exception_from_rdr(dump_rdr_exc_info_s *rdr_exc);
/* 异常处理完成后，从dump exc list中删除该异常 */
int dump_remove_exception(const dump_exc_info_s *this_exc);
/* 打印本次异常信息 */
void dump_show_exc_info(const dump_exc_info_s *exc);
int dump_exc_manager_init(void);

/* dump_frame为了前向兼容而中使用 */
void dump_fill_rdr_exc_info(const dump_subsys_drv_s *subsys, const dump_exc_cfg_s *exc_info,
                            struct rdr_exception_info_s *rdr_exc);
/* hal中实现的函数,有些外部上报的异常，不是modem配置的，但是需要modem存log，例如RDR_PHONE_MDMAP_PANIC_MOD_ID，需要判断是否允许 */
const dump_exc_cfg_s *dump_get_defult_exc_cfg(u32 rdr_modid);

#endif