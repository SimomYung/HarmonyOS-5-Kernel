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

#ifndef _DUMP_SUBSYS_MGR_H_
#define _DUMP_SUBSYS_MGR_H_

#include <osl_types.h>
#include <dump_works.h>
#include "dump_exception.h"
#include "dump_subsys.h"

/* subsys interface */
int dump_register_subsys(const dump_subsys_drv_s *ops);
/* 获取一个dump子系统的driver data */
const dump_subsys_drv_s *dump_get_subsys(u32 subsys);
/* 获取当前系统已注册的modem subsys mask */
u64 dump_get_online_subsys(void);
/* 获取支持单独复位的子系统，注意，需要在dump_is_subsys_drv_prepare_done之后才能获取到正确的结果 */
u64 dump_get_reset_subsys_support(void);

/* 通知对应子系统准备dump */
int dump_notify_subsys_dump_prepare(const dump_exc_log_desc_s *exc_desc);
/* 通知对应子系统dump */
int dump_subsys_do_works(dump_work_cb_e work_type, const dump_exc_log_desc_s *exc_desc);
/* 通知所有子系统,发生了某事件 */
int dump_subsys_broadcast_event_to_all(int event, const dump_exc_log_desc_s *exc_desc);
/* 通知相关子系统,发生了某事件 */
int dump_subsys_broadcast_event_to_related(int event, const dump_exc_log_desc_s *exc_desc);
/* 通知相关子系统复位 */
dump_reset_result_e dump_subsys_reset(const dump_exc_log_desc_s *exc_desc);

/* 输入dump子系统的mask，返回rdr子系统配置notify_core/reset_core的mask */
u64 dump_get_rdr_core_mask(u64 dump_subsys_mask);
/* 将subsys转换为modem dump.bin中global_base中e_core的值，不使用dump_subsys的原始枚举是为了和之前的版本兼容,由于是modem dump内部使用，
 * 因此与RDR dump core有一定差异，对于RDR来说，modem子系统都是RDR_CP，但是我们这里将MDMAP、MDMLPM配置为RDR_AP、RDR_LPM3
 */
u32 dump_subsys_to_inner_rdr_core(u32 dump_subsys);
/* ticket or ERRNO */
int dump_subsys_add_work(enum dump_subsys subsys_id, dump_work_s *work);
int dump_subsys_del_work(enum dump_subsys subsys_id, int ticket);
/* 初始化相关 */
bool dump_is_subsys_drv_prepare_done(void);
/* step1. 各个子系统subsys driver结构体初始化;dump对外的注册回调的接口中，需要调用该接口，保证subsys_mgr初始化ok */
void dump_subsys_drv_prepare_all(void);
/* step2. 各个子系统内部逻辑初始化，需要在dump_subsys_drv_prepare_all之后执行  */
void dump_subsys_init_all(void);
#endif