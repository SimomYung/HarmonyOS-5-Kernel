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
#include <securec.h>
#include <linux/kernel.h>
#include <osl_types.h>
#include <osl_sem.h>
#include <osl_thread.h>
#include <dump_modid.h>
#include <dump_rdr_wrapper.h>
#include <dump_infra.h>
#include <dump_logs.h>
#include <dump_sharemem.h>
#include <dump_area.h>
#include "dump_subsys_mgr.h"
#include "dump_exc_mgr.h"
#include "dump_frame.h"

typedef struct {
    spinlock_t lock;
    struct semaphore sem_dump_task;
    int wait_max_time;
    dump_exc_log_desc_s curr_log_desc;
    pfn_cb_dump_done rdr_done_cb;
} dump_exc_mgr_ctrl_s;

dump_exc_mgr_ctrl_s g_dump_exc_mgr_ctrl;

__ro_after_init bool g_dump_init_done = false;

bool dump_is_init_done(void)
{
    return g_dump_init_done;
}

int dump_is_in_exception(void)
{
    return g_dump_exc_mgr_ctrl.curr_log_desc.exc_info != NULL;
}

const dump_exc_log_desc_s *dump_get_current_exc_desc(void)
{
    return &g_dump_exc_mgr_ctrl.curr_log_desc;
}

const dump_exc_info_s *bsp_dump_get_current_exc_info(void)
{
    return g_dump_exc_mgr_ctrl.curr_log_desc.exc_info;
}

/* 兼容性接口 */
const struct dump_global_base_info_s *bsp_dump_get_cur_exc_baseinfo(void)
{
    return (const struct dump_global_base_info_s *)dump_get_global_baseinfo();
}

void dump_set_current_exc_desc(unsigned int modid, unsigned int etype, unsigned long long coreid, char *logpath,
    pfn_cb_dump_done fndone)
{
    dump_exc_info_s *curr_exc = dump_get_current_exception_by_rdrid(modid);
    dump_rdr_exc_info_s rdr_exc;
    g_dump_exc_mgr_ctrl.rdr_done_cb = fndone;

    if (curr_exc == NULL) {
        dump_print("sys 0x%llx request mdm dump use modid 0x%x\n", coreid, modid);
        rdr_exc.modid = modid;
        rdr_exc.arg1 = (u32)coreid;
        rdr_exc.arg2 = 0;
        rdr_exc.etype = etype;
        curr_exc = dump_report_exception_from_rdr(&rdr_exc); /* 要保证该函数不会返回NULL */
        curr_exc->rdr_info.etype = etype;
    }

    g_dump_exc_mgr_ctrl.curr_log_desc.exc_info = curr_exc;
    g_dump_exc_mgr_ctrl.curr_log_desc.dump_done_subsys = 0;

    if (logpath == NULL || logpath[0] == '\0') {
        g_dump_exc_mgr_ctrl.curr_log_desc.log_path[0] = '\0';
    } else if (snprintf_s(g_dump_exc_mgr_ctrl.curr_log_desc.log_path, RDR_DUMP_FILE_PATH_LEN, RDR_DUMP_FILE_PATH_LEN - 1,
               "%s%s", logpath, RDR_DUMP_FILE_CP_PATH) <= 0) {
        dump_error("log path may too long %s\n", logpath);
        g_dump_exc_mgr_ctrl.curr_log_desc.log_path[0] = '\0';
    }
}

void dump_exc_handle_done(void)
{
    unsigned long flags;
    const dump_exc_info_s *curr_exc = g_dump_exc_mgr_ctrl.curr_log_desc.exc_info;
    dump_subsys_broadcast_event_to_related(DUMP_EVENT_EXC_HANDLE_DONE, &g_dump_exc_mgr_ctrl.curr_log_desc);

    /* 不应该出现exc_desc为临界区的情况，但考虑到异常场景下各种情况都可能出现，例如异常嵌套等，这里需要保护curr_exc指针清空时，其他地方不能同时使用 */
    spin_lock_irqsave(&g_dump_exc_mgr_ctrl.lock, flags);
    (void)memset_s(&g_dump_exc_mgr_ctrl.curr_log_desc, sizeof(dump_exc_log_desc_s),0, sizeof(dump_exc_log_desc_s));
    spin_unlock_irqrestore(&g_dump_exc_mgr_ctrl.lock, flags);

    if (curr_exc != NULL) {
        dump_remove_exception(curr_exc);
    }
}

void dump_create_save_done_file(const char *dir)
{
    char filename[MODEM_DUMP_FILE_NAME_LENGTH] = { 0 };
    if (snprintf_s(filename, sizeof(filename), sizeof(filename) - 1, "%s%s", dir, DUMP_SAVE_FLAG) < 0) {
        dump_info("snp err\n");
    }
    (void)dump_create_file(filename);
}

void dump_exc_async_process_setup(const dump_exc_log_desc_s *exc_desc)
{
    dump_logs_prepare_proc(exc_desc);
}

void dump_exc_async_process_teardown(const dump_exc_log_desc_s *exc_desc)
{
    /* 创建CP_DONE标记，表示文件存完（不论是存完了，还是超时了，都创建，因为可能存在保存了部分日志的情况，该标记用于通知终端APP打包modem日志）
     * 清理工作目录(当前不需要)：禁用dump fs inst(后续按需实现，此后禁止文件写入);
     */
    dump_logs_teardown_proc(exc_desc);
    if (dump_valid_exc_log_path(exc_desc)) {
        dump_create_save_done_file(exc_desc->log_path);
    }
}

int dump_set_save_done(enum dump_subsys subsys_id)
{
    if (subsys_id >= DUMP_SUBSYS_MDMBUTT) {
        dump_error("inv param\n");
        return DUMP_ERRNO_INVA_PARAM;
    }
    /* 不考虑上次异常后，某个子系统回调一直没处理完，直到后来又来了一次异常后，上次回调才处理完，然后调用了该接口的情况
     * 虽然这种情况会让本次异常处理时，误以为该子系统已经完成了处理，从而没有等待本次真正处理完成；
     * 但实际上，该子系统本次异常处理的work，大概率不会执行，因为queue_work时，上次的work还在队列里，不会再次调度进去
     * 因此如果这种情况下，不标记done_mask的话，等这个子系统存log完成标记，大概率是要等超时的，这样反而不合适
     */
    g_dump_exc_mgr_ctrl.curr_log_desc.dump_done_subsys |= DUMP_SUBSYS_MASK(subsys_id);
    return DUMP_OK;
}

int dump_wait_save_done(dump_exc_log_desc_s *exc_desc)
{
    unsigned long flags;
    u64 online_subsys = dump_get_online_subsys();
    u64 notify_subsys_mask = 0;
    int time = g_dump_exc_mgr_ctrl.wait_max_time;
    dump_print("dump wait save done\n");

    /* 该分支不应存在，这里增加判断防止异常场景访问空指针 */
    spin_lock_irqsave(&g_dump_exc_mgr_ctrl.lock, flags);
    if (exc_desc->exc_info == NULL || exc_desc->exc_info->exc_cfg == NULL) {
        spin_unlock_irqrestore(&g_dump_exc_mgr_ctrl.lock, flags);
        dump_print("dump wait done param invalid\n");
        return DUMP_ERRNO_INVA_PARAM;
    }

    notify_subsys_mask = exc_desc->exc_info->exc_cfg->notify_subsys_mask & online_subsys;
    spin_unlock_irqrestore(&g_dump_exc_mgr_ctrl.lock, flags);

    dump_wait_done(&exc_desc->dump_done_subsys, notify_subsys_mask, &time);

    if (time <= 0) {
        dump_error("wait for module dump done time out(%d s)\n", g_dump_exc_mgr_ctrl.wait_max_time);
        return DUMP_ERRNO_TIMEOUT;
    }
    dump_print("dump save done\n");
    return DUMP_OK;
}

int dump_wait_done_task(void *data)
{
    dump_exc_mgr_ctrl_s *dump_ctrl = &g_dump_exc_mgr_ctrl;
    dump_exc_log_desc_s *exc_desc = &dump_ctrl->curr_log_desc;
    u32 modid;
    u64 reset_subsys_mask;

    UNUSED(data);

    while (1) {
        osl_sem_down(&dump_ctrl->sem_dump_task);

        dump_print("enter save log task\n");
    
        // 执行 dump framework works，frame work中的works需要尽可能的安全，因此这里不调用外部注册进来的回调，包括：
        // step1： setup流程
        dump_exc_async_process_setup(exc_desc);
    
        /* step2：do async work流程
         * 2.1 然后触发其他子系统的works_queue，当前按子系统枚举顺序依次触发，后续定制顺序时，去修改subsys接口即可
         */
        dump_subsys_do_works(DUMP_WORK_ASYNC, exc_desc);
        /* 2.2 然后等超时 */
        dump_wait_save_done(exc_desc);
        /* step3 teardown流程，不论是否超时，都要执行 */
        dump_exc_async_process_teardown(exc_desc);
        
        modid = 0xFFFFFFFF;
        reset_subsys_mask = DUMP_SUBSYS_MASK_AP;
        if (exc_desc->exc_info != NULL) {
            modid = exc_desc->exc_info->rdr_info.modid;
            if (exc_desc->exc_info->exc_cfg != NULL) {
                reset_subsys_mask = exc_desc->exc_info->exc_cfg->reset_subsys_mask;
            }
        }

        if (g_dump_exc_mgr_ctrl.rdr_done_cb != NULL) {
            g_dump_exc_mgr_ctrl.rdr_done_cb(modid, RDR_CP);
            dump_print("notify rdr dump handle finished\n");
            /* 如果不需要RDR_CP复位，那么直接exc_handle_done */
            if (dump_get_rdr_core_mask(reset_subsys_mask) != RDR_CP) {
                dump_exc_handle_done();
            }
        }
    }
    return BSP_OK;
}

void dump_mdm_callback(unsigned int modid, unsigned int etype, unsigned long long coreid, char *logpath,
    pfn_cb_dump_done fndone)
{
    dump_print("enter modem dump callback\n");

    dump_set_current_exc_desc(modid, etype, coreid, logpath, fndone);
    // 如果上报前没有打印异常信息，这里重新打印一遍
    if ((g_dump_exc_mgr_ctrl.curr_log_desc.exc_info != NULL) &&
        (g_dump_exc_mgr_ctrl.curr_log_desc.exc_info->rpt_status == DUMP_EXC_ACCEPT_LATER)) {
        dump_print("now handle accept later exc\n");
        dump_show_exc_info(g_dump_exc_mgr_ctrl.curr_log_desc.exc_info);
    }

    dump_notify_subsys_dump_prepare(&g_dump_exc_mgr_ctrl.curr_log_desc);

    /* 执行各子系统的sync work，当前都是按照子系统枚举顺序来执行的，如果后面需要定制子系统间work顺序，修改subsys的接口即可 */
    dump_subsys_do_works(DUMP_WORK_SYNC, &g_dump_exc_mgr_ctrl.curr_log_desc);

    /* 如果是panic,直接通知rdr保存完成 */
    if (modid == RDR_PHONE_MDMAP_PANIC_MOD_ID) {
        if (fndone != BSP_NULL) {
            fndone(modid, RDR_CP);
            dump_print("mamap panic, notify rdr dump handle finished!\n");
        }
        dump_exc_handle_done();
    }
    osl_sem_up(&g_dump_exc_mgr_ctrl.sem_dump_task);
}

/* 后续DPA单独复位实现时如果有差异，这里再下沉到各自的子系统 */
void dump_reset_fail_handle(u32 ori_exc_rdr_modid, u32 ori_exc_subsys_modid)
{
    enum dump_exc_rpt_ret ret;
    /*mdmap subsys 一定存在，且其exc_cfg中配置了子系统失败的rdr_modid */
    dump_subsys_exc_info_s subsys_exc = {
        .e_from_subsys = DUMP_SUBSYS_MDMAP,
        .modid = DRV_ERRNO_RST_FAIL,
        .arg1 = ori_exc_rdr_modid,
        .arg2 = ori_exc_subsys_modid,
        .data = NULL,
        .length = 0,
        .reboot_context = DUMP_CTX_INT,
        .int_no = 0xFFFFFFFF,
        .reason = DUMP_REASON_RST_FAIL,
        .task_name = { 0 },
    };
    ret = dump_report_exception(&subsys_exc);
    /* 注意：当前只有CP一个子系统，单独复位失败都是整机复位，这里预期不应该失败，如果失败了，就直接向rdr上报一个modem异常 */
    if (!dump_is_exc_rpt_accept(ret) && (ret != DUMP_EXC_REJECT_REENTRY)) {
        dump_error("trigger mdm rst fail proc fail\n");
        rdr_system_error(RDR_MODEM_CP_RESET_FAIL_MOD_ID, ori_exc_rdr_modid, ori_exc_subsys_modid);
    }
}

void dump_mdm_reset(unsigned int modid, unsigned int etype, unsigned long long coreid)
{
    int ret;
    u32 arg1, arg2;
    /* 当其他子系统请求modem复位但不要求modem存log时，会走到这里；cp的exc_cfg不存在这种情况，但不排除其他子系统modid这么配置了
     * 此时使用默认modid配置，不支持复位modem
     */
    if (g_dump_exc_mgr_ctrl.curr_log_desc.exc_info == NULL) {
        dump_set_current_exc_desc(modid, etype, coreid, NULL, NULL);
    }
    if (g_dump_exc_mgr_ctrl.curr_log_desc.exc_info != NULL) {
        arg1 = g_dump_exc_mgr_ctrl.curr_log_desc.exc_info->rdr_info.modid;
        arg2 = g_dump_exc_mgr_ctrl.curr_log_desc.exc_info->subsys_info.modid;
    } else {
        arg1 = modid;
        arg2 = modid;
    }

    dump_subsys_broadcast_event_to_related(DUMP_EVENT_RESET_START, &g_dump_exc_mgr_ctrl.curr_log_desc);
    ret = dump_subsys_reset(&g_dump_exc_mgr_ctrl.curr_log_desc);
    dump_exc_handle_done();
    /* 当前处理策略都相同，失败后触发整机复位；因此这一步在exc_mgr中实现；
     * 如果后面不同子系统单独复位失败策略有差异；或者支持多个子系统同时单独复位时
     * mdm_reset流程就要修改为 notify_reset->wait_reset_done->subsys_reset_fail_callback 3步进行
     */
    if (ret != DUMP_RESET_SUCCESS) {
        dump_reset_fail_handle(arg1, arg2);
    }
}

int dump_register_mdm_exc_handler(void)
{
    struct rdr_module_ops_pub soc_ops = {
        .ops_dump = (pfn_dump)dump_mdm_callback,
        .ops_reset = (pfn_reset)dump_mdm_reset,
    };
    struct rdr_register_module_result soc_rst = { 0 };

    if (rdr_register_module_ops(RDR_CP, &soc_ops, &soc_rst) != BSP_OK) {
        dump_error("fail to register rdr ops\n");
        return DUMP_ERRNO_REG_RDR_MODULE_OPS;
    }
    return DUMP_OK;
}

__init int bsp_dump_init(void)
{
    struct task_struct *pid = NULL;
    int ret;

    g_dump_exc_mgr_ctrl.wait_max_time = DUMP_MSLEEP_60;
    spin_lock_init(&g_dump_exc_mgr_ctrl.lock);

    ret = dump_exc_manager_init();
    if (ret != DUMP_OK) {
        return ret;
    }

    osl_sem_init(0, &g_dump_exc_mgr_ctrl.sem_dump_task);
    if (osl_task_init("dump_wait_done", DUMP_WAIT_TASK_PRI, 0x2000, (OSL_TASK_FUNC)dump_wait_done_task,
        &g_dump_exc_mgr_ctrl, &pid)) {
        dump_error("creat save_modem_bootlog task error!\n");
        return DUMP_ERRNO_CREATE_TASK;
    }

    ret = dump_register_mdm_exc_handler();
    if (ret != DUMP_OK) {
        return ret;
    }

    dump_subsys_init_all();
    dump_sharemem_init();
    dump_logs_init();

    g_dump_init_done = true;
    dump_print("bsp_dump_init ok");
    return BSP_OK;
}
#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
module_init(bsp_dump_init);
#endif