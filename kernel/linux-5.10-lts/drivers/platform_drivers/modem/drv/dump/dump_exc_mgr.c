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
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/bitops.h>
#include <osl_types.h>
#include <bsp_slice.h>
#include <dump_rdr_wrapper.h>
#include <dump_infra.h>
#include <dump_area.h>
#include "dump_subsys_mgr.h"
#include "dump_exc_mgr.h"
#include "dump_modid.h"

enum dump_exc_proc_type {
    DUMP_EXC_PROC_TYPE_RST_AP = 0,
    DUMP_EXC_PROC_TYPE_RST_MDMCP,
    // DUMP_EXC_PROC_TYPE_RST_DPA, // DPA单独复位，当前不支持
    DUMP_EXC_PORC_TYPE_MDMSYS_BUTT,
    DUMP_EXC_PROC_TYPE_OTHER_SYS_REQ_MDMDUMP = DUMP_EXC_PORC_TYPE_MDMSYS_BUTT,
    DUMP_EXC_PROC_TYPE_BUTT,
};

typedef struct {
    spinlock_t lock;
    dump_exc_info_s exc_list[DUMP_EXC_PROC_TYPE_BUTT];
} dump_exc_info_list_s;

dump_exc_info_list_s g_dump_exc_info_list;

bool dump_check_valid_exception(const dump_exc_cfg_s *exc_info)
{
    if (exc_info == NULL) {
        return false;
    }
    if (exc_info->exc_modid < DUMP_RDR_CP_MODID_START || exc_info->exc_modid > DUMP_RDR_CP_MODID_END) {
        dump_error("invalid modid\n");
        return false;
    }
    if (exc_info->src_modid_start > exc_info->src_modid_end) {
        dump_error("invalid modid range\n");
        return false;
    }
    if ((exc_info->reset_subsys_mask | exc_info->notify_subsys_mask) & (~DUMP_SUBSYS_VALID_MASK)) {
        dump_error("invalid mask\n");
        return false;
    }
    if (hweight64(exc_info->reset_subsys_mask) > 1) {
        dump_error("only support reset 1 dump subsys now\n");
        return false;
    }
    /* 对于非modem子系统，当前只支持复位AP */
    if ((exc_info->reset_subsys_mask & DUMP_SUBSYS_OTHER_VALID_MASK) &&
        (exc_info->reset_subsys_mask != DUMP_SUBSYS_MASK_AP)) {
        dump_error("other subsys only support ap rst\n");
    }
    return true;
}

void dump_fill_rdr_exc_info(const dump_subsys_drv_s *subsys, const dump_exc_cfg_s *exc_info,
                            struct rdr_exception_info_s *rdr_exc)
{
    rdr_exc->e_modid = exc_info->exc_modid;
    rdr_exc->e_modid_end = exc_info->exc_modid;
    rdr_exc->e_process_priority = RDR_ERR;
    rdr_exc->e_reboot_priority = RDR_REBOOT_WAIT;
    rdr_exc->e_notify_core_mask = dump_get_rdr_core_mask(exc_info->notify_subsys_mask);
    rdr_exc->e_reset_core_mask = dump_get_rdr_core_mask(exc_info->reset_subsys_mask);
    rdr_exc->e_from_core = RDR_CP;
    /* MBB上，exc_subsys=MDMAP时，这里之前是RDR_AP(除了定位问题外无实际影响)，因此需要特殊处理 */
    if ((bsp_dump_get_product_type() == DUMP_MBB) && (subsys->dump_sys == DUMP_SUBSYS_MDMAP)) {
        rdr_exc->e_from_core = RDR_AP;
    }
    rdr_exc->e_reentrant = (unsigned int)RDR_REENTRANT_ALLOW;
    rdr_exc->e_exce_type = exc_info->exc_type;
    rdr_exc->e_upload_flag = (unsigned int)RDR_UPLOAD_YES;
    rdr_exc->e_save_log_flags = RDR_SAVE_LOGBUF;

    if (rdr_exc->e_notify_core_mask != 0) {
        // rdr的约束，如果存log，不能只通知其他子系统dump，不通知自己
        rdr_exc->e_notify_core_mask |= RDR_CP;
    } else {
        // ap rdr机制，即使不通知其他子系统存log，也会判断e_save_log_flags并尝试保存，并且此时会保存到错误的路径下(上次有log的异常路径)
        rdr_exc->e_save_log_flags = 0;
    }

    memset_s(rdr_exc->e_from_module, sizeof(rdr_exc->e_from_module), 0, sizeof(rdr_exc->e_from_module));
    if (strcpy_s((char *)rdr_exc->e_from_module, sizeof(rdr_exc->e_from_module), subsys->name) != EOK) {
        dump_debug("cp err\n");
    }

    /* 如果不支持单独复位，那么应该进行整机复位；这里理论上不应该走到，因为在subsys_mgr初始化时，
     * 会根据检查支持单独复位的子系统，并对exc_cfg重新配置，除非实现错误；
     * 相应实现的检查，要加到it测试用例里(当前新老dump同时在主线，后面再添加这个用例)
     */
    if ((rdr_exc->e_reset_core_mask & RDR_CP) && !(dump_get_reset_subsys_support() & exc_info->reset_subsys_mask)) {
        rdr_exc->e_reset_core_mask &= ~RDR_CP;
        rdr_exc->e_reset_core_mask |= RDR_AP;
    }

    if (rdr_exc->e_reset_core_mask & RDR_AP) {
        rdr_exc->e_reboot_priority = RDR_REBOOT_NOW;
        rdr_exc->e_reentrant = (unsigned int)RDR_REENTRANT_DISALLOW;
    }

    memset_s(rdr_exc->e_desc, sizeof(rdr_exc->e_desc), 0, sizeof(rdr_exc->e_desc));
    if (strcpy_s((char *)rdr_exc->e_desc, sizeof(rdr_exc->e_desc), exc_info->exc_desc) != EOK) {
        dump_debug("cp err\n");
    }
}

int dump_register_exception(const dump_subsys_drv_s *subsys)
{
    struct rdr_exception_info_s rdr_exc = {0};
    const dump_subsys_exc_cfg_s *exc_cfg = NULL;
    const dump_exc_cfg_s *exc_info = NULL;
    int i;
    int ret = 0;
    if (subsys == NULL) {
        dump_error("exc param err\n");
        return DUMP_ERRNO_INVA_PARAM;
    }
    exc_cfg = &subsys->exc_cfg;
    if (exc_cfg->num != 0 && exc_cfg->cfg_list == NULL) {
        dump_error("exc param err\n");
        return DUMP_ERRNO_INVA_PARAM;
    }

    for (i = 0; i < exc_cfg->num; i++){
        exc_info = &exc_cfg->cfg_list[i];
        if (!dump_check_valid_exception(exc_info)) {
            dump_error("subsys %d exc_cfg %d err\n", subsys->dump_sys, i);
            return DUMP_ERRNO_INVA_PARAM;
        }

        dump_fill_rdr_exc_info(subsys, exc_info, &rdr_exc);
        if ((rdr_exc.e_reset_core_mask != RDR_AP) && (rdr_exc.e_reset_core_mask != RDR_CP)) {
            dump_error("only support reset RDR_AP/CP now\n");
            return DUMP_ERRNO_INVA_PARAM;
        }

        if (!rdr_register_exception(&rdr_exc)) {
            dump_error("reg exc fail modid=0x%x\n", rdr_exc.e_modid);
            ret = DUMP_ERROR;
        }
    }

    return ret;
}
DUMP_LLT_EXPORT_SYMBOL(dump_register_exception);

void dump_fill_current_exc_info(const dump_exc_cfg_s *exc_cfg, dump_subsys_exc_info_s *subsys_exc,
                                dump_exc_info_s *curr_exc)
{
    curr_exc->timestamp = bsp_get_slice_value();
    curr_exc->subsys_info = *subsys_exc;
    curr_exc->rdr_info.modid = exc_cfg->exc_modid;
    curr_exc->rdr_info.arg1 = subsys_exc->e_from_subsys;
    curr_exc->rdr_info.arg2 = subsys_exc->modid;
    curr_exc->rdr_info.etype = exc_cfg->exc_type;
    curr_exc->exc_cfg = exc_cfg;
    curr_exc->voice = dump_get_mdm_voice_status();
}

/* 没找到modid时，使用最后该子系统最后一个配置作为默认值；注意，若该子系统没有exc_cfg，则会返回NULL */
const dump_exc_cfg_s *dump_get_subsys_exc_cfg(const dump_subsys_drv_s *subsys, u32 modid)
{
    const dump_subsys_exc_cfg_s *sys_exc_cfg = &subsys->exc_cfg;
    const dump_exc_cfg_s *ret_cfg = NULL;
    int i;
    if ((sys_exc_cfg->num == 0) && (sys_exc_cfg->cfg_list == NULL)) {
        return NULL;
    }
    /* 没找到时，使用最后1个作为默认值 */
    for (i = 0; i < (int)sys_exc_cfg->num - 1; i++) {
        if ((modid >= sys_exc_cfg->cfg_list[i].src_modid_start) && (modid <= sys_exc_cfg->cfg_list[i].src_modid_end)) {
            break;
        }
    }
    ret_cfg = &sys_exc_cfg->cfg_list[i];
    return ret_cfg;
}

enum dump_exc_proc_type dump_get_exc_proc_type(const dump_exc_cfg_s *exc_cfg)
{
    if (exc_cfg == NULL) {
        return DUMP_EXC_PROC_TYPE_OTHER_SYS_REQ_MDMDUMP;
    }
    /* modem上报的异常，都应该在下面的if判断中覆盖 */
    if (dump_get_rdr_core_mask(exc_cfg->reset_subsys_mask) & RDR_AP) {
        return DUMP_EXC_PROC_TYPE_RST_AP;
    } else if (exc_cfg->reset_subsys_mask & DUMP_SUBSYS_MASK_CP) {
        return DUMP_EXC_PROC_TYPE_RST_MDMCP;
    }
    /* 如果不是modem dump上报的异常，归为这种场景 */
    return DUMP_EXC_PROC_TYPE_OTHER_SYS_REQ_MDMDUMP;
}

enum dump_exc_rpt_ret dump_check_exc_prt_strategy(enum dump_exc_proc_type curr_exc_type)
{
    int i;
    enum dump_exc_rpt_ret ret = DUMP_EXC_ACCEPT_NOW;
    if (curr_exc_type < 0 || curr_exc_type >= DUMP_EXC_PROC_TYPE_BUTT) {
        return DUMP_EXC_REJECT_INVALID;
    }

    /* 这里也要与 其他子系统检测到的modem异常 进行比较，如果处理策略相同，就拒绝重入；
     *  若其他子系统只是想让modem存log，那exc_type会是DUMP_EXC_PROC_TYPE_OTHER_SYS_REQ_MDMDUMP，不会阻止modem异常上报
     */
    if (g_dump_exc_info_list.exc_list[DUMP_EXC_PROC_TYPE_OTHER_SYS_REQ_MDMDUMP].rdr_info.modid) {
        if (curr_exc_type == 
            dump_get_exc_proc_type(g_dump_exc_info_list.exc_list[DUMP_EXC_PROC_TYPE_OTHER_SYS_REQ_MDMDUMP].exc_cfg)) {
            return DUMP_EXC_REJECT_REENTRY;
        }
    }

    for (i = 0; i < DUMP_EXC_PORC_TYPE_MDMSYS_BUTT; i++) {
        if (g_dump_exc_info_list.exc_list[i].rdr_info.modid != 0) {
            if ((i == curr_exc_type) || (i == DUMP_EXC_PROC_TYPE_RST_AP)) {
                ret = DUMP_EXC_REJECT_REENTRY;
                break;
            } else {
                ret = DUMP_EXC_ACCEPT_LATER;
            }
        }
    }
    return ret;
}

void dump_show_exc_info(const dump_exc_info_s *exc)
{
    dump_print("timestamp = 0x%x\n", exc->timestamp);
    dump_print("mod_id = 0x%x\n", exc->subsys_info.modid);
    dump_print("rdr_mod_id = 0x%x\n", exc->rdr_info.modid);
    dump_print("arg1 = 0x%x\n", exc->subsys_info.arg1);
    dump_print("arg2 = 0x%x\n", exc->subsys_info.arg2);
    dump_print("reason = 0x%x\n", exc->subsys_info.reason);
    dump_print("voice = 0x%x\n", exc->voice);
    dump_print("reboot_context = 0x%x\n", exc->subsys_info.reboot_context);
    if (exc->subsys_info.reboot_context == DUMP_CTX_TASK){
        dump_print("task_id = 0x%x\n", exc->subsys_info.task_id);
        if (strnlen(exc->subsys_info.task_name, DUMP_EXCINFO_TASKNAME_LEN) != 0) {
            dump_print("task_name = %s\n", exc->subsys_info.task_name);
        }
    } else {
        dump_print("int_no = 0x%x\n", exc->subsys_info.int_no);
    }
    if (strnlen(exc->exc_cfg->exc_desc, DUMP_EXC_DESC_SIZE) != 0) {
        dump_print("exc_desc = %s\n", exc->exc_cfg->exc_desc);
    }
}

enum dump_exc_rpt_ret dump_lock_exception_core(dump_subsys_exc_info_s *subsys_exc, dump_exc_info_s **exc_info)
{
    unsigned long flags;
    enum dump_exc_rpt_ret ret;
    const dump_subsys_drv_s *subsys = NULL;
    const dump_exc_cfg_s *curr_exc_cfg = NULL;
    enum dump_exc_proc_type curr_exc_type;

    if (subsys_exc == NULL || subsys_exc->e_from_subsys >= DUMP_SUBSYS_MDMBUTT) {
        return DUMP_EXC_REJECT_INVALID;
    }

    subsys = dump_get_subsys(subsys_exc->e_from_subsys);
    if (subsys == NULL) {
        dump_error("no subsys %d\n", subsys_exc->e_from_subsys);
        return DUMP_EXC_REJECT_INVALID;
    }

    curr_exc_cfg = dump_get_subsys_exc_cfg(subsys, subsys_exc->modid);
    curr_exc_type = dump_get_exc_proc_type(curr_exc_cfg);

    spin_lock_irqsave(&g_dump_exc_info_list.lock, flags);
    ret = dump_check_exc_prt_strategy(curr_exc_type);
    if (!dump_is_exc_rpt_accept(ret)) {
        spin_unlock_irqrestore(&g_dump_exc_info_list.lock, flags);
        dump_error("exc from sys 0x%x modid 0x%x will reject[%d]\n", subsys_exc->e_from_subsys, subsys_exc->modid, ret);
        return ret;
    }
    *exc_info = &g_dump_exc_info_list.exc_list[curr_exc_type];
    g_dump_exc_info_list.exc_list[curr_exc_type].rpt_status = ret;
    dump_fill_current_exc_info(curr_exc_cfg, subsys_exc, *exc_info);
    spin_unlock_irqrestore(&g_dump_exc_info_list.lock, flags);
    return ret;
}

enum dump_exc_rpt_ret dump_lock_exception(dump_subsys_exc_info_s *subsys_exc)
{
    dump_exc_info_s *exc_info = NULL;
    return dump_lock_exception_core(subsys_exc, &exc_info);
}

int dump_update_and_report_exception(dump_subsys_exc_info_s *subsys_exc)
{
    int i;
    dump_exc_info_s *locked_exc = NULL;
    for (i = 0; i < DUMP_EXC_PORC_TYPE_MDMSYS_BUTT; i++) {
        locked_exc = &g_dump_exc_info_list.exc_list[i];
        if ((locked_exc->subsys_info.e_from_subsys == subsys_exc->e_from_subsys) && 
            (locked_exc->subsys_info.modid == subsys_exc->modid)) {
            locked_exc->subsys_info = *subsys_exc;
            rdr_system_error(locked_exc->rdr_info.modid, locked_exc->rdr_info.arg1, locked_exc->rdr_info.arg2);
            dump_show_exc_info(locked_exc);
            return DUMP_OK;
        }
    }

    return DUMP_ERRNO_INVA_PARAM;
}

enum dump_exc_rpt_ret dump_report_exception(dump_subsys_exc_info_s *subsys_exc)
{
    dump_exc_info_s *exc_info_node = NULL;
    enum dump_exc_rpt_ret ret = dump_lock_exception_core(subsys_exc, &exc_info_node);

    if (!dump_is_exc_rpt_accept(ret)) {
        return ret;
    }

    /* 先上报异常，再打印 */
    rdr_system_error(exc_info_node->rdr_info.modid, exc_info_node->rdr_info.arg1, exc_info_node->rdr_info.arg2);
    if (ret == DUMP_EXC_ACCEPT_NOW) {
        dump_show_exc_info(exc_info_node);
    } else if (ret == DUMP_EXC_ACCEPT_LATER) {
        dump_error("exist exc handling, new exc from sys 0x%x modid 0x%x proc later\n", subsys_exc->e_from_subsys,
                   subsys_exc->modid);
    }
    return ret;
}
DUMP_LLT_EXPORT_SYMBOL(dump_report_exception);

/* 从列表中删除，并禁用内存 */
int dump_remove_exception(const dump_exc_info_s *this_exc)
{
    int i;
    for (i = 0; i < DUMP_EXC_PROC_TYPE_BUTT; i++) {
        if (this_exc == &g_dump_exc_info_list.exc_list[i]) {
            (void)memset_s(&g_dump_exc_info_list.exc_list[i], sizeof(dump_exc_info_s), 0, sizeof(dump_exc_info_s));
            return DUMP_OK;
        }
    }
    return DUMP_ERRNO_INVA_PARAM;
}

/* 当外部检测到modem异常，或者单纯需要modem存log时，需要创建一个subsys_exc，这里尽可能获取准确的reason信息 */
u32 dump_convert_rdr_etype_to_reboot_reason(u32 etype)
{
    static const u32 dump_reason_map[][2] = {
        {CP_S_MODEMDMSS, DUMP_REASON_DMSS},
        {CP_S_MODEMNOC, DUMP_REASON_NOC},
    };
    int i;
    for (i = 0; i < ARRAY_SIZE(dump_reason_map); i++) {
        if (dump_reason_map[i][0] == etype) {
            return dump_reason_map[i][1];
        }
    }
    return DUMP_REASON_UNDEF;
}

/* 其他子系统检测到的异常，需要modem dump/reset，构造curr_exc_info */
dump_exc_info_s *dump_report_exception_from_rdr(dump_rdr_exc_info_s *rdr_exc)
{
    unsigned long flags;
    int subsys_id, i;
    const dump_subsys_drv_s *subsys = NULL;
    const dump_exc_cfg_s *curr_exc_cfg = NULL;
    dump_exc_info_s *curr_exc = &g_dump_exc_info_list.exc_list[DUMP_EXC_PROC_TYPE_OTHER_SYS_REQ_MDMDUMP];
    if (rdr_exc == NULL) {
        return NULL;
    }

    curr_exc_cfg = dump_get_defult_exc_cfg(rdr_exc->modid);

    if (curr_exc->rdr_info.modid != 0) {
        dump_error("last exc rpt by rdr not clr, overwrite\n");
    }

    /* 遍历所有子系统的exc_cfg */
    for (subsys_id = 0; subsys_id < DUMP_SUBSYS_MDMBUTT; subsys_id++) {
        subsys = dump_get_subsys(subsys_id);
        for (i = 0; subsys && (i < subsys->exc_cfg.num) && subsys->exc_cfg.cfg_list; i++) {
            if (rdr_exc->modid == subsys->exc_cfg.cfg_list[i].exc_modid) {
                curr_exc_cfg = &subsys->exc_cfg.cfg_list[i];
                goto found;
            }
        }
    }

found:
    spin_lock_irqsave(&g_dump_exc_info_list.lock, flags);
    curr_exc->timestamp = bsp_get_slice_value();
    curr_exc->exc_cfg = curr_exc_cfg;
    curr_exc->rdr_info = *rdr_exc;

    (void)memset_s(&curr_exc->subsys_info, sizeof(dump_subsys_exc_info_s), 0, sizeof(dump_subsys_exc_info_s));
    curr_exc->subsys_info.e_from_subsys = subsys_id;
    curr_exc->subsys_info.modid = curr_exc_cfg->src_modid_start;
    curr_exc->subsys_info.reboot_context = DUMP_CTX_INT;
    curr_exc->subsys_info.int_no = (u32)-1;
    curr_exc->subsys_info.reason = dump_convert_rdr_etype_to_reboot_reason(curr_exc_cfg->exc_type);
    curr_exc->voice = dump_get_mdm_voice_status();
    curr_exc->rpt_status = DUMP_EXC_ACCEPT_NOW;

    spin_unlock_irqrestore(&g_dump_exc_info_list.lock, flags);
    if (subsys_id == DUMP_SUBSYS_MDMBUTT) {
        dump_error("rdr modid 0x%x not mdm_cfg, proc default\n", rdr_exc->modid);
    }

    return curr_exc;
}

/* 该接口用于rdr回调中，获取modem子系统已上报的异常，因此不检查EXC_PROC_TYPE_OTHER_SYS的数据 */
dump_exc_info_s *dump_get_current_exception_by_rdrid(u32 modid)
{
    int i;
    for (i = 0; i < DUMP_EXC_PORC_TYPE_MDMSYS_BUTT; i++) {
        if (g_dump_exc_info_list.exc_list[i].rdr_info.modid == modid) {
            return &g_dump_exc_info_list.exc_list[i];
        }
    }
    return NULL;
}

__init int dump_exc_manager_init(void)
{
    spin_lock_init(&g_dump_exc_info_list.lock);
    return DUMP_OK;
}