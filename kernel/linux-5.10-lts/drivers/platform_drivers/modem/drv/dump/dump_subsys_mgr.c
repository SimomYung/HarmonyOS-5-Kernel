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
#include <product_config.h>
#include <linux/kernel.h>
#include <linux/cache.h>
#include <linux/bitops.h>
#include <osl_types.h>
#include <dump_rdr_wrapper.h>
#include "dump_subsys_mgr.h"
#include "dump_exc_mgr.h"
#include "dump_infra.h"

// llt版本通过打桩subsys来实现测试
#ifdef DRV_BUILD_GCOV
__ro_after_init const dump_subsys_drv_s *g_dump_subsys[DUMP_SUBSYS_MDMBUTT];
#else
const dump_subsys_drv_s *g_dump_subsys[DUMP_SUBSYS_MDMBUTT];
#endif

bool g_dump_subsys_drv_prepare_done = false;

u64 dump_get_rdr_core_mask(u64 dump_subsys_mask)
{
    u64 rdr_core = 0;
    if (dump_subsys_mask & (~DUMP_SUBSYS_VALID_MASK)) {
        dump_error("invalid param\n");
        return 0;
    }
    if (dump_subsys_mask & DUMP_SUBSYS_MDM_VALID_MASK) {
        rdr_core |= RDR_CP;
    }
    if (dump_subsys_mask & DUMP_SUBSYS_MASK_AP) {
        rdr_core |= RDR_AP;
    }
    if (dump_subsys_mask & DUMP_SUBSYS_MASK_LPMCU) {
        rdr_core |= RDR_LPM3;
    }
    if (dump_subsys_mask & DUMP_SUBSYS_MASK_HIFI) {
        rdr_core |= RDR_HIFI;
    }
    return rdr_core;
}

/* 将subsys转换为modem dump.bin中global_base中e_core的值，不使用dump_subsys的原始枚举是为了和之前的版本兼容,由于是modem dump内部使用，
 * 因此与RDR dump core有一定差异，对于RDR来说，modem子系统都是RDR_CP，但是我们这里将MDMAP、MDMLPM配置为RDR_AP、RDR_LPM3
 */
u32 dump_subsys_to_inner_rdr_core(u32 dump_subsys)
{
    u32 rdr_core;
    switch (dump_subsys) {
        case DUMP_SUBSYS_COMM:
        case DUMP_SUBSYS_MDMAP:
        case DUMP_SUBSYS_AP:
            rdr_core = RDR_AP;
            break;
        case DUMP_SUBSYS_MDMLPM:
        case DUMP_SUBSYS_LPMCU:
            rdr_core = RDR_LPM3;
            break;
        default:
            rdr_core = RDR_CP;
            break;
    }
    return rdr_core;
}

/* 内部接口，节省空间的话可以不检查subsys内容是否合法 */
bool dump_check_subsys_param(const dump_subsys_drv_s *subsys)
{
    if (subsys == NULL || subsys->dump_sys >= DUMP_SUBSYS_MDMBUTT) {
        dump_error("ops null or id err\n");
        return false;
    }
    if ((subsys->exc_cfg.num != 0) && (subsys->exc_cfg.cfg_list == NULL)) {
        dump_error("exc_cfg err\n");
        return false;
    }
    if (subsys->work_ctrl == NULL) {
        dump_error("workctl err\n");
        return false;
    }
    if (subsys->name == NULL || subsys->name[0] == '\0' ||
        strnlen(subsys->name, DUMP_SUBSYS_NAME_LEN_MAX) >= DUMP_SUBSYS_NAME_LEN_MAX) {
        dump_error("name err\n");
        return false;
    }
    if ((subsys->dump_prepare_handler == NULL) || (subsys->event_handler == NULL)) {
        dump_error("hook null err\n");
        return false;
    }
    return true;
}

int dump_register_subsys(const dump_subsys_drv_s *subsys)
{
    if (!dump_check_subsys_param(subsys)) {
        dump_error("param err\n");
        return DUMP_ERRNO_INVA_PARAM;
    }
    g_dump_subsys[subsys->dump_sys] = subsys;
    return DUMP_OK;
}
DUMP_LLT_EXPORT_SYMBOL(dump_register_subsys);

const dump_subsys_drv_s *dump_get_subsys(u32 subsys)
{
    if (subsys < DUMP_SUBSYS_MDMBUTT) {
        return g_dump_subsys[subsys];
    }
    return NULL;
}
DUMP_LLT_EXPORT_SYMBOL(dump_get_subsys);

u64 dump_get_online_subsys(void)
{
    u64 online_subsys = 0;
    int i;
    for (i = 0; i < DUMP_SUBSYS_MDMBUTT; i++) {
        if (g_dump_subsys[i] != NULL) {
            online_subsys |= DUMP_SUBSYS_MASK(i);
        }
    }
    return online_subsys;
}

/* 通知对应子系统准备dump */
int dump_notify_subsys_dump_prepare(const dump_exc_log_desc_s *exc_desc)
{
    int i;
    if (!dump_valid_exc_desc(exc_desc)) {
        dump_error("exc_desc inv\n");
        return DUMP_ERRNO_INVA_PARAM;
    }
    for (i = 0; i < DUMP_SUBSYS_MDMBUTT; i++) {
        if ((exc_desc->exc_info->exc_cfg->notify_subsys_mask & DUMP_SUBSYS_MASK(i)) && (g_dump_subsys[i] != NULL)) {
            g_dump_subsys[i]->dump_prepare_handler(exc_desc);
        }
    }
    return DUMP_OK;
}

int dump_subsys_do_works(dump_work_cb_e work_type, const dump_exc_log_desc_s *exc_desc)
{
    int i, work_ret;
    int final_ret = DUMP_OK;
    if ((work_type >= DUMP_WORK_INVALID) || !dump_valid_exc_desc(exc_desc)) {
        dump_error("exc_desc inv\n");
        return DUMP_ERRNO_INVA_PARAM;
    }
    for (i = 0; i < DUMP_SUBSYS_MDMBUTT; i++) {
        /* 若当前子系统存在，并且需要被通知，并且该子系统的dump work还没有执行完
         * 最后这个条件是为了防止某些场景下，子系统不想执行work定制的；这时，子系统会在自己的dump_prepare回调中，将done_mask置位
         * （比如子系统正在进行单独复位，此时不能do work；或者他认为该异常和自己没关系，不愿意响应其他子系统异常时对自己的dump请求）
         */
        if ((g_dump_subsys[i] != NULL) && (exc_desc->exc_info->exc_cfg->notify_subsys_mask & DUMP_SUBSYS_MASK(i)) &&
            ((exc_desc->dump_done_subsys & DUMP_SUBSYS_MASK(i)) == 0)) {
            work_ret = dump_do_works(work_type, g_dump_subsys[i]->work_ctrl);
            if (work_ret != DUMP_OK) {
                final_ret = work_ret;
            }
        }
    }
    return final_ret;
}

int dump_subsys_add_work(enum dump_subsys subsys_id, dump_work_s *work)
{
    if (subsys_id >= DUMP_SUBSYS_MDMBUTT) {
        return DUMP_ERRNO_INVA_PARAM;
    }
    if (g_dump_subsys[subsys_id] == NULL) {
        return DUMP_ERRNO_SUBSYS_NOT_EXISIT;
    }
    return dump_add_work(g_dump_subsys[subsys_id]->work_ctrl, work);
}

int dump_subsys_del_work(enum dump_subsys subsys_id, int ticket)
{
    if (subsys_id >= DUMP_SUBSYS_MDMBUTT) {
        return DUMP_ERRNO_INVA_PARAM;
    }
    if (g_dump_subsys[subsys_id] == NULL) {
        return DUMP_ERRNO_SUBSYS_NOT_EXISIT;
    }
    return dump_del_work(g_dump_subsys[subsys_id]->work_ctrl, ticket);
}

// 调试使用
void dump_subsys_show_work_all(void)
{
    int i;
    for (i = 0; i < DUMP_SUBSYS_MDMBUTT; i++) {
        dump_print("works of subsys%d:\n", i);
        if (g_dump_subsys[i] != NULL) {
            dump_show_work(g_dump_subsys[i]->work_ctrl);
        }
    }
}

u64 dump_get_reset_subsys_support(void)
{
    int i;
    u64 result = DUMP_SUBSYS_MASK_AP; // 一定支持整机复位
    for (i = 0; i < DUMP_SUBSYS_MDMBUTT; i++) {
        if ((g_dump_subsys[i] != NULL) && (g_dump_subsys[i]->reset_ops != NULL)) {
            result |= DUMP_SUBSYS_MASK(i);
        }
    }
    return result;
}

/* 通知对应子系统开始reset */
dump_reset_result_e dump_subsys_reset(const dump_exc_log_desc_s *exc_desc)
{
    dump_reset_result_e ret = DUMP_RESET_SUCCESS;
    int i;
    if (!dump_valid_exc_desc(exc_desc)) {
        dump_error("exc_desc inv\n");
        return DUMP_RESET_NOT_SUPPORT;
    }
    if (unlikely(hweight64(exc_desc->exc_info->exc_cfg->reset_subsys_mask) > 1)) {
        dump_error("multi sys rst 0x%llx\n", exc_desc->exc_info->exc_cfg->reset_subsys_mask);
        return DUMP_RESET_NOT_SUPPORT;
    }
    for (i = 0; i < DUMP_SUBSYS_MDMBUTT; i++) {
        if ((exc_desc->exc_info->exc_cfg->reset_subsys_mask & DUMP_SUBSYS_MASK(i)) && (g_dump_subsys[i] != NULL)) {
            if (g_dump_subsys[i]->reset_ops == NULL) {
                ret = DUMP_RESET_NOT_SUPPORT;
                /* 某个子系统不支持单独复位，但是有人配置了单独复位，这个是非预期的，因为dump在注册rdr_exc的时候，会进行检查，
                 * 因此这种情况直接退出，进行整机复位
                 */
                break;
            } else {
                ret = g_dump_subsys[i]->reset_ops(exc_desc);
                /* 当前只支持单个子系统复位，后面如果支持多个子系统再修改 */
                break;
            }
        }
    }
    return ret;
}

/* 通知相关子系统,有modem dump相关发生 */
int dump_subsys_broadcast_event_to_related(int event, const dump_exc_log_desc_s *exc_desc)
{
    int i;
    if (!dump_valid_exc_desc(exc_desc)) {
        dump_error("exc_desc inv\n");
        return DUMP_ERRNO_INVA_PARAM;
    }
    for (i = 0; i < DUMP_SUBSYS_MDMBUTT; i++) {
        if (((exc_desc->exc_info->exc_cfg->notify_subsys_mask | exc_desc->exc_info->exc_cfg->reset_subsys_mask) &
              DUMP_SUBSYS_MASK(i)) && (g_dump_subsys[i] != NULL)) {
            g_dump_subsys[i]->event_handler(event, exc_desc);
        }
    }
    return DUMP_OK;
}

/* 通知所有子系统,有modem子系统相关事件发生 */
int dump_subsys_broadcast_event_to_all(int event, const dump_exc_log_desc_s *exc_desc)
{
    int i;
    if (!dump_valid_exc_desc(exc_desc)) {
        dump_error("exc_desc inv\n");
        return DUMP_ERRNO_INVA_PARAM;
    }
    for (i = 0; i < DUMP_SUBSYS_MDMBUTT; i++) {
        if ((exc_desc->exc_info->exc_cfg->notify_subsys_mask & DUMP_SUBSYS_MASK(i)) && (g_dump_subsys[i] != NULL)) {
            g_dump_subsys[i]->event_handler(event, exc_desc);
        }
    }
    return DUMP_OK;
}

/* 各个子系统的初始化,除了这里，子系统外的文件不允许使用各子系统内部的头文件(dump_area接口除外，维测内存是mdrv一定存在的功能) */
/* dump_subsys_drv_prepare_all之后才能向子系统注册work */
#include "subsys_comm/dump_comm_agent.h"
#include "subsys_cp/dump_cp_agent.h"
#include "subsys_mdmap/dump_mdmap_agent.h"
#include "subsys_mdmlpm/dump_lpm_agent.h"

bool dump_is_subsys_drv_prepare_done(void)
{
    return g_dump_subsys_drv_prepare_done;
}

void dump_subsys_drv_prepare_all(void)
{
    if (g_dump_subsys_drv_prepare_done) {
        return;
    }
    g_dump_subsys_drv_prepare_done = true;
    dump_comm_subsys_drv_prepare();
    dump_mdmap_subsys_drv_prepare();
    dump_lpmcu_subsys_drv_prepare();
    dump_cp_subsys_drv_prepare();
}

/* 等所有子系统完全初始化后，才注册异常，因为有些异常需要子系统单独复位，但当前相应子系统是否支持单独复位，需要初始化后才决定 */
__init void dump_subsys_register_exc_all(void)
{
    int i;
    u64 reset_support_subsysmask = dump_get_reset_subsys_support();
    dump_print("reset support subsys is 0x%llx\n", reset_support_subsysmask);
    /* 通知子系统更新异常的reset core mask */
    dump_comm_subsys_update_exc_cfg(reset_support_subsysmask);
    dump_mdmap_subsys_update_exc_cfg(reset_support_subsysmask);
    dump_lpmcu_subsys_update_exc_cfg(reset_support_subsysmask);
    dump_cp_subsys_update_exc_cfg(reset_support_subsysmask);

    for (i = 0; i < DUMP_SUBSYS_MDMBUTT; i++) {
        if (g_dump_subsys[i] != NULL) {
            dump_register_exception(g_dump_subsys[i]);
        }
    }
}

__init void dump_subsys_init_all(void)
{
    if (!g_dump_subsys_drv_prepare_done) {
        dump_subsys_drv_prepare_all();
    }
    dump_comm_subsys_init();
    dump_mdmap_subsys_init();
    dump_lpmcu_subsys_init();
    dump_cp_subsys_init();
    dump_subsys_register_exc_all();
}