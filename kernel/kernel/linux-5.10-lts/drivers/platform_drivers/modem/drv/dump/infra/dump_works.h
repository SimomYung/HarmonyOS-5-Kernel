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
#ifndef _DUMP_WORKS_H_
#define _DUMP_WORKS_H_

#include <osl_types.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <dump_exception.h>

typedef enum {
    DUMP_WORK_PRIO_BEGIN = 0,
    DUMP_WORK_PRIO_SYNC_HOOK_START = DUMP_WORK_PRIO_BEGIN,
    DUMP_WORK_PRIO_SYNC_HOOK_DUMP_PLAT_START = DUMP_WORK_PRIO_SYNC_HOOK_START,
    /* 0~50 dump 模块自身的sync works*/
    /* 51~100 给外组件提供的同步回调优先级 */
    DUMP_WORK_PRIO_SYNC_HOOK_OTHER_MODULE_HIGH = 51,
    DUMP_WORK_PRIO_SYNC_HOOK_OTHER_MODULE_LOW = 100,
    /* 100~150 dump自身用于在外组件sync work回调执行完成之后执行的事物 */
    DUMP_WORK_PRIO_SYNC_HOOK_DUMP_PLAT_END = 150,
    DUMP_WORK_PRIO_ASYNC_HOOK_START = 151,
    DUMP_WORK_PRIO_ASYNC_HOOK_DUMP_PLAT_START = DUMP_WORK_PRIO_SYNC_HOOK_START,
    /* 151~200 dump 模块自身的async works*/
    /* 151~159 在waitdone之前执行的任务 */
    DUMP_WORK_PRIO_ASYNC_HOOK_WAIT_DONE = 160,
    DUMP_WORK_PRIO_ASYNC_HOOK_PLAT_DUMP_AFTER_DONE = 161,

    DUMP_WORK_PRIO_ASYNC_HOOK_OTHER_MODULE_HIGH = 201,
    /* 201~250 给外组件提供的异步回调优先级 */
    DUMP_WORK_PRIO_ASYNC_HOOK_OTHER_MODULE_LOW = 250,
    /* 251~300 dump自身用于在外组件sync work回调执行完成之后执行的事物 */
    DUMP_WORK_PRIO_ASYNC_HOOK_SET_SUBSYS_DONE = 299,
    DUMP_WORK_PRIO_ASYNC_HOOK_DUMP_PLAT_END = 300,

    /* 以上是整体区域划分，剩下的子系统内部work的优先级，挪到子系统内部自行定义 */
    /* mdmap */
    DUMP_WORK_PRIO_SYNC_HOOK_DUMP_KMSG = 1,
    DUMP_WORK_PRIO_ASYNC_HOOK_DUMP_MDM_SECOS = 151,
    /* comm */
    DUMP_WORK_PRIO_ASYNC_HOOK_COMMON_UPDATE_AVS_ESCAPE = 151,
    DUMP_WORK_PRIO_ASYNC_HOOK_COMMON_SAVE_RSTLOG_BIN = 152,
    DUMP_WORK_PRIO_ASYNC_HOOK_COMMON_SAVE_MDMDUMP_BIN = 161,
    DUMP_WORK_PRIO_ASYNC_HOOK_COMMON_PARSE_MDMDUMP_BIN = 162,
    DUMP_WORK_PRIO_ASYNC_HOOK_COMMON_SAVE_LOGMEM_BIN = 163,
    DUMP_WORK_PRIO_ASYNC_HOOK_COMMON_PARSE_LOGMEM_BIN = 164,
    DUMP_WORK_PRIO_ASYNC_HOOK_COMMON_SAVE_OTHER_BIN = 165,
    DUMP_WORK_PRIO_ASYNC_HOOK_COMMON_SAVE_SECDUMP_BIN = 166,
    DUMP_WORK_PRIO_ASYNC_HOOK_COMMON_SAVE_MDMDUMP_BACKUP_BIN = 166,
    DUMP_WORK_PRIO_ASYNC_HOOK_COMMON_PARSE_OTHER_LOG = 168,

    DUMP_WORK_PRIO_ASYNC_HOOK_COMMON_COMPRESS_LOG = 290, /* 最后在执行日志压缩 */
    DUMP_WORK_PRIO_ASYNC_HOOK_COMMON_UPLOADER_WAITDONE = 291,

    DUMP_WORK_PRIO_INVALID = DUMP_WORK_PRIO_ASYNC_HOOK_DUMP_PLAT_END + 1,
} dump_work_proir_e;

typedef enum {
    DUMP_WORK_SYNC = 0,
    DUMP_WORK_ASYNC = 1,
    DUMP_WORK_INVALID,
} dump_work_cb_e;

typedef void (*dump_work_func)(const dump_exc_log_desc_s *exc_info, void *priv_data);

typedef struct {
    u32 init_state;
    struct list_head work_list;
    spinlock_t spinlock;
    /* 执行异步回调使用的kworker work_struct，各模块使用自己的，防止某一子系统阻塞，影响其他子系统 */
    struct work_struct kwork;
    /* 执行子系统dump async回调使用的workqueue，若不指定，则使用dump公共work_queue，可能会受到其他dump work阻塞 */
    struct workqueue_struct *dump_wq;
} dump_work_ctrl_s;

#define DUMP_WORK_STATE_PENDING 0
#define DUMP_WORK_STATE_WORKING 0xA5A55A5A
#define DUMP_WORK_STATE_DONE 0x5A5A5A5A

typedef struct {
    struct list_head work_list;
    dump_work_func work_fn;
    void *priv_data;
    unsigned int work_type;
    u32 prior; /* 优先级 */
    u32 exec_state;
} dump_work_s;

/* add work公共函数，添加子系统work，添加exc_manager的work这些接口，由对应模块独立封装 */
int dump_add_work(dump_work_ctrl_s *work_ctrl, dump_work_s *work);
int dump_del_work(dump_work_ctrl_s *work_ctrl, int ticket);

/* worker_fn如果没有指定，就用默认的，即将work_list中的所有回调执行一遍；
 * wq_name如果没指定，就用默认workqueue；当前只有
 */
int dump_work_ctrl_factory(dump_work_ctrl_s *ctrl, work_func_t worker_fn, const char *wq_name);

/* @note： async work返回OK，不代表执行完了，只是表示work已经加入队列等待调度 */
int dump_do_works(dump_work_cb_e wprk_type, dump_work_ctrl_s *work_ctrl);

/* 调试使用 */
void dump_show_work(dump_work_ctrl_s *work_ctrl);
#endif