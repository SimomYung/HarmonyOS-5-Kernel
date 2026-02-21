/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
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

#ifndef __SYSBOOT_DPART_DUMP_H__
#define __SYSBOOT_DPART_DUMP_H__

#include <bsp_partition.h>
#include "sysboot_dpart_policy.h"

#define DPART_DUMP_HEAD_MAGIC 0x44444844 // "DDHD"->DPART DUMP首字母，HEAD首尾字母
#define DPART_DUMP_DATA_MAGIC 0x44444441 // "DDDA"->DPART DUMP首字母，DATA首尾字母
#define DPART_DUMP_MAIN_IDX_MAX 3
#define DPART_DUMP_SUB_IDX_MAX 4
#define DPART_DUMP_CNT_MAX ((DPART_DUMP_MAIN_IDX_MAX) * (DPART_DUMP_SUB_IDX_MAX))

enum SYSBOOT_STATE {
    SYSBOOT_BOOT_FAILED = 0,
    SYSBOOT_BOOT_SUCCESS,
};

enum SYSBOOT_EXCEPTION_TYPE {
    SYSBOOT_NONE_EXCEPTION = 0,
    SYSBOOT_BOOT_EXCEPTION,
    SYSBOOT_RUNTIME_EXCEPTION,
    SYSBOOT_MAX_EXCEPTION,
};

enum SYSBOOT_EXCEPTION_STAGE {
    SYSBOOT_NONE_STAGE_EXCEPTION = 0,
    SYSBOOT_XLOADER_EXCEPTION,
    SYSBOOT_HIBOOT_EXCEPTION,
    SYSBOOT_AP_EXCEPTION,
    SYSBOOT_LPMCU_EXCEPTION,
    SYSBOOT_MODEM_EXCEPTION,
    SYSBOOT_MAX_STAGE_EXCEPTION
};

struct sysboot_dpart_dump_info_head {
    u32 dpart_dump_head_magic;
    u8 dpart_dump_cnt;
    u8 dpart_dump_main_idx; // struct sysboot_dpart_dump_info中data数组索引
    u8 dpart_dump_sub_idx; // struct sysboot_dpart_dump_info_data中node数组索引
    u8 rsv;
};

struct sysboot_dpart_dump_info_boot_data {
    u32 total_boot_times; // 系统总启动次数
    u8 sysboot_state; // 系统启动状态：0：全系统启动失败，1：全系统启动成功
    u8 exception_type; // 系统异常类型：0：无异常； 1：启动时异常，2：运行时运行
    u8 exception_stage; // 系统异常阶段：0：无异常 1：xloader，2：hiboot，3：ap，4： lpmcu，5：modem
    u8 dpart_enable; // 双备份是否使能
    u8 dpart_policy; //  双备份策略：AA或AB启动
    u8 boot_retry_times; // 启动尝试次数
    u8 curr_boot_retry_times; // 当前启动尝试次数
    u8 boot_part_switched; // 分区切换状态：0：分区未切换，1：分区发生了切换；
    u8 default_boot_part; // 默认启动分区
    u8 escape_state; // 系统逃生状态：0：未执行逃生，1：已执行逃生
    u8 rsv[2];
    u32 image_load_failed; // 1个bit代表一个镜像，最大支持32个镜像，0: 镜像加载成功，1：镜像加载失败;
    u32 image_resotre_state1; // 2个bit代表一个镜像，对应编号为0-15镜像：不需要恢复，1：镜像恢复失败，2：镜像恢复成功
    u32 image_resotre_state2; // 2个bit代表一个镜像，对应编号为16-31镜像：0：不需要恢复，1：镜像恢复失败，2：镜像恢复成功
};

struct sysboot_dpart_dump_info_update_data {
    u32 update_version;
    u32 update_times;
    u8 update_state;
    u8 update_result;
    u8 rsc[2];
    u32 img_back_result;
    u32 part_update_complete;
};

struct sysboot_dpart_dump_info_data_node {
    u32 dpart_dump_data_magic;
    struct sysboot_dpart_dump_info_boot_data boot_data;
    struct sysboot_dpart_dump_info_update_data update_data;
};

struct sysboot_dpart_dump_info_data {
    struct sysboot_dpart_dump_info_data_node node[DPART_DUMP_SUB_IDX_MAX]; // 对应一个oeminfo结构，最多存放4次log；
};

// 将oeminfo结构汇总到内存中；
struct sysboot_dpart_dump_info {
    struct task_struct *taskid;
    osl_sem_id task_trigger_sem;
    osl_sem_id sysboot_dump_sem;
    char part_name_arr[SYSBOOT_IMG_NUMS_MAX * PART_NAMELEN + 1];
    char img_restore_arr[SYSBOOT_IMG_NUMS_MAX * 5U + 1]; //存储每个镜像的恢复状态：“SUCC”或“FAIL” 字符串
    char dump_history[2048U];
    struct sysboot_dpart_dump_info_head head; // head是单独的一个oeminfo结构；
    struct sysboot_dpart_dump_info_data data[DPART_DUMP_MAIN_IDX_MAX]; // 需要使用3个oeminfo结构存放12次log
    u32 inited;
};

#endif
