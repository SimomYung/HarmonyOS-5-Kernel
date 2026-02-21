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

#ifndef __SYSBOOT_DPART_POLICY_H__
#define __SYSBOOT_DPART_POLICY_H__

#define OEMINFO_DPART_MAGIC 0x44504152 // "DPAR"
#define SYSBOOT_IMG_NUMS_MAX 32

struct dpart_oeminfo { // 在oeminfo中定义的结构体
    u32 dpart_magic;
    u32 total_boot_times; // 系统从出产以来的总启动次数，更新粒度以curr_boot_times只最大值为准
    u32 boot_part_switched; // 分区切换状态：0：未切换，1：发生切换
};

/* 启动分区优先级：寄存器中启动分区 > onchiprom传递的启动分区 > oeminfo分区中的启动分区
   寄存器中的启动分区信息：xloader阶段启动失败后会切换分区，由于xloader阶段不支持写文件，切换后的启动分区保存在非掉电寄存器；
   onchiprom传递的启动分区：fmc硬件双boot启动，不传递启动分区；fmc软件双boot以及emmc启动，传递启动分区；
   oeminfo分区中保存的启动分区：OTA升级完成时在kernel阶段切换启动分区、全系统启动运行失败时在hiboot阶段切换启动分区；
 */
union sysboot_dpart_reg_info {
    u32 value;
    struct {
        u32 boot_part : 2; // 寄存器中记录的启动分区，A分区或B分区
        u32 curr_boot_retry_times : 3; // 当前启动重试次数，启动成功后清零；
        u32 curr_boot_times : 4; // 当前启动次数，每次启动时+1，达到最大值后清零，不区分正常启动次数与异常启动
        u32 xloader_part_switch_state : 1; // xloader阶段分区切换状态：0：启动分区未切换，1：启动分区发生切换
        u32 escape_state : 1; // 系统逃生状态：0：未执行逃生，1：已执行逃生
        u32 xloader_load_failed : 1; // 0: 镜像加载成功，1：镜像加载失败
        u32 ptable_load_failed : 1; // 0: 镜像加载成功，1：镜像加载失败
        u32 uce_load_failed : 1; // 0: 镜像加载成功，1：镜像加载失败
        u32 hiboot_load_failed : 1; // 0: 镜像加载成功，1：镜像加载失败
        u32 boot_download_mode : 1; // 0:正常启动模式，1：download模式； 内存OTA场景使用
    } member;
};

struct sysboot_dpart_runtime_info {
    u32 dpart_enable; // 双备份是否使能，来源于xloader
    u32 dpart_policy; // 双备份策略：AA或AB启动，来源于xloader
    u32 boot_retry_times; // 启动次数尝试阈值
    u32 default_boot_part; // 默认启动分区，来源于xloader
    u32 boot_part_switched; // 当前启动分区切换状态：0：未切换，1：已切换
    u32 boot_part_swiched_state_cleared; // 分区切换状态被清除：0：未清除；1：已清除
    u32 image_load_failed; // 1个bit代表一个镜像，最大支持32个镜像，0: 镜像加载成功，1：镜像加载失败; 写入oeminfo分区, 来源于xloader
    u32 image_resotre_state1; // 2个bit代表一个镜像，对应编号为0-15镜像：不需要恢复，1：镜像恢复失败，2：镜像恢复成功;写入oeminfo分区
    u32 image_resotre_state2; // 2个bit代表一个镜像，对应编号为16-31镜像：0：不需要恢复，1：镜像恢复失败，2：镜像恢复成功;写入oeminfo分区
    u32 oeminfo_total_boot_times; // 系统从出厂以来的总启动次数，更新粒度以curr_boot_times只最大值为准
    union sysboot_dpart_reg_info reg_info; // 非掉电寄存器结构信息，来源于xloader与hiboot
};

struct sysboot_dpart_info {
    struct sysboot_dpart_runtime_info runtime_info; // 系统运行时信息
};

void sysboot_set_boot_download_mode(bool flag);
int sysboot_get_boot_download_mode(void);
struct sysboot_dpart_info *sysboot_get_dpart_info(void);
const char *sysboot_dpart_get_part_name_by_img_type(unsigned int img_type);

#endif
