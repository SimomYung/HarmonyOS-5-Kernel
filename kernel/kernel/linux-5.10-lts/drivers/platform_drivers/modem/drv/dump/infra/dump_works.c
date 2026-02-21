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
#include <osl_types.h>
#include <osl_malloc.h>
#include <dump_frame.h>
#include <dump_subsys.h>
#include "dump_errno.h"
#include "dump_config.h"
#include "dump_infra.h"
#include "dump_works.h"

__ro_after_init struct workqueue_struct *g_dump_default_wq;

int dump_work_check_param(dump_work_s *work)
{
    if (work == NULL || work->work_fn == NULL) {
        return DUMP_ERRNO_INVA_PARAM;
    }

    if (work->prior >= DUMP_WORK_PRIO_INVALID || work->work_type >= DUMP_WORK_INVALID) {
        return DUMP_ERRNO_INVA_PARAM;
    }

    return DUMP_OK;
}

void dump_work_clear_state(dump_work_s *work)
{
    work->exec_state = DUMP_WORK_STATE_PENDING;
}

int dump_work_check_duplicate(dump_work_s *work_item, dump_work_s *work_node)
{
    if ((work_item->work_type != work_node->work_type) || (work_item->work_fn != work_node->work_fn) ||
        (work_item->prior != work_node->prior) || (work_item->priv_data != work_node->priv_data)) {
        return 0;
    }
    return 1;
}

void dump_show_work(dump_work_ctrl_s *work_ctrl)
{
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    dump_work_s *work_node = NULL;

    if (work_ctrl == NULL) {
        dump_error("work param null!\n");
        return;
    }

    list_for_each_safe(p, n, &work_ctrl->work_list)
    {
        work_node = list_entry(p, dump_work_s, work_list);
        if (work_node && (work_node->work_fn)) {
            dump_print("callback=%pS priv_data=%pS\n", work_node->work_fn, work_node->priv_data);
        }
    }
}

int dump_add_work(dump_work_ctrl_s *work_ctrl, dump_work_s *work)
{
    unsigned long flags;
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    dump_work_s *work_item = NULL;
    dump_work_s *work_node = NULL;
    int ret;

    if (work_ctrl == NULL || work == NULL) {
        dump_error("work param null!\n");
        return DUMP_ERRNO_INVA_PARAM;
    }

    ret = dump_work_check_param(work);
    if (ret != DUMP_OK) {
        dump_error("invalid parameter!\n");
        return DUMP_ERRNO_INVA_PARAM;
    }

    work_item = (dump_work_s *)osl_malloc(sizeof(dump_work_s));
    if (work_item == NULL) {
        dump_error("malloc fail!\n");
        return DUMP_ERRNO_MEM_OPS;
    }

    ret = memcpy_s(work_item, sizeof(dump_work_s), work, sizeof(dump_work_s));
    if (ret != EOK) {
        osl_free(work_item);
        dump_error("memcpy_error:0x%x\n", ret);
        return DUMP_ERRNO_MEM_OPS;
    }

    dump_work_clear_state(work_item);
    spin_lock_irqsave(&work_ctrl->spinlock, flags);
    list_for_each_safe(p, n, &work_ctrl->work_list) {
        work_node = list_entry(p, dump_work_s, work_list);
        if (dump_work_check_duplicate(work_item, work_node)) {
            osl_free(work_item);
            work_item = work_node;
            dump_warning("already register\n");
            break;
        }
        if (work_item->prior < work_node->prior) {
            list_add_tail(&work_item->work_list, p);
            break;
        }
    }
    if (p == &work_ctrl->work_list) {
        list_add_tail(&work_item->work_list, &work_ctrl->work_list);
    }
    spin_unlock_irqrestore(&work_ctrl->spinlock, flags);
    return (int)(uintptr_t)work_item; // 截断后的指针取值范围也和errno不同，返回值可以判断出来
}

int dump_del_work(dump_work_ctrl_s *work_ctrl, int ticket)
{
    unsigned long flags;
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    dump_work_s *work_item = NULL;
    dump_work_s *work_node = NULL;

    if (work_ctrl == NULL || ticket == 0) {
        dump_error("work param null!\n");
        return DUMP_ERRNO_INVA_PARAM;
    }

    spin_lock_irqsave(&work_ctrl->spinlock, flags);
    list_for_each_safe(p, n, &work_ctrl->work_list) {
        work_node = list_entry(p, dump_work_s, work_list);
        if ((int)(uintptr_t)work_node == ticket) {
            work_item = work_node;
            break;
        }
    }
    if (work_item == NULL) {
        spin_unlock_irqrestore(&work_ctrl->spinlock, flags);
        return DUMP_ERRNO_INVA_PARAM;
    }
    list_del(&work_item->work_list);
    osl_free(work_item);
    spin_unlock_irqrestore(&work_ctrl->spinlock, flags);
    return DUMP_OK;
}

void dump_do_work_core(struct list_head *work_list_head, dump_work_cb_e current_type)
{
    const dump_exc_log_desc_s *exc_desc = dump_get_current_exc_desc();
    struct list_head *p = NULL;
    struct list_head *n = NULL;
    dump_work_s *work_item = NULL;

    list_for_each_safe(p, n, work_list_head) {
        work_item = list_entry(p, dump_work_s, work_list);
        /* 这里额外判断下当前work是否在working状态，只有1种情况下会存在work_item=working时，又要执行该work：
         * 1. work中异常，此时该work不能执行
         * 如果work中阻塞，等下次又来了个需要modem存log的事件(不一定是modem异常，可以是别的子系统异常)，queue work会失败，因此不会重入
         */
        if ((work_item->work_type == current_type) && (work_item->exec_state != DUMP_WORK_STATE_WORKING)) {
            work_item->exec_state = DUMP_WORK_STATE_WORKING;
            work_item->work_fn(exc_desc, work_item->priv_data);
            work_item->exec_state = DUMP_WORK_STATE_DONE;
        }
    }
}

void dump_work_func_default(struct work_struct *work)
{
    dump_work_ctrl_s *work_ctrl = container_of(work, dump_work_ctrl_s, kwork);

    if (!work_ctrl->init_state) {
        return;
    }
    dump_do_work_core(&work_ctrl->work_list, DUMP_WORK_ASYNC);
}

static inline int dump_do_work_sync(dump_work_ctrl_s *work_ctrl)
{
    dump_do_work_core(&work_ctrl->work_list, DUMP_WORK_SYNC);
    return DUMP_OK;
}

static inline  int dump_do_work_async(dump_work_ctrl_s *work_ctrl)
{
    bool ret;
    if (work_ctrl->dump_wq == NULL) {
        return DUMP_ERRNO_INVA_PARAM;
    }
    ret = queue_work(work_ctrl->dump_wq, &work_ctrl->kwork);
    if (ret == false) {
        return DUMP_ERRNO_QUEUE_WORK;
    }
    return DUMP_OK;
}

int dump_do_works(dump_work_cb_e work_type, dump_work_ctrl_s *work_ctrl)
{
    int ret;
    if (work_ctrl == NULL || !work_ctrl->init_state) {
        return DUMP_ERRNO_INVA_PARAM;
    }
    if (work_type >= DUMP_WORK_INVALID) {
        return DUMP_ERRNO_INVA_PARAM;
    }
    if (work_type == DUMP_WORK_ASYNC) {
        ret = dump_do_work_async(work_ctrl);
    } else {
        ret = dump_do_work_sync(work_ctrl);
    }
    return ret;
}

int dump_work_ctrl_factory(dump_work_ctrl_s *ctrl, work_func_t worker_fn, const char *wq_name)
{
    if (ctrl == NULL) {
        return DUMP_ERRNO_INVA_PARAM;
    }
    if (ctrl->init_state != 0) {
        return DUMP_OK;
    }
    INIT_LIST_HEAD(&ctrl->work_list);
    spin_lock_init(&ctrl->spinlock);
    if (worker_fn != NULL) {
        INIT_WORK(&ctrl->kwork, worker_fn);
    } else {
        INIT_WORK(&ctrl->kwork, dump_work_func_default);
    }

    if (wq_name != NULL && wq_name[0] == '\0') {
        ctrl->dump_wq = create_singlethread_workqueue(wq_name);
    } else {
        if (g_dump_default_wq == NULL) {
            g_dump_default_wq = alloc_workqueue("mdmdump_wq", WQ_UNBOUND | WQ_HIGHPRI, DUMP_SUBSYS_MDMBUTT);
        }
        ctrl->dump_wq = g_dump_default_wq;
    }
    ctrl->init_state = 1;
    if (ctrl->dump_wq == NULL) {
        dump_error("create wq[%s] fail\n", wq_name);
        return DUMP_ERRNO_CREATE_WQ;
    }

    return DUMP_OK;
}