/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#ifndef __SPPM_DEVICE_H__
#define __SPPM_DEVICE_H__

#include <linux/workqueue.h>
#include <linux/spinlock_types.h>
#include "sppm_ipc_protocol.h"

#include "mdrv_sppm.h"

typedef struct {
    volatile unsigned int shreshold_cnt;  // 正常数据写水线中断次数
    volatile unsigned int cpu_pp_ovfw;  // CPU直写模式因总线反压导致pingpang buffer溢出告警屏蔽后中断状态实时上报
    volatile unsigned int regs_ovfl;      // Reg寄存器多，周期频繁造成回绕
    volatile unsigned int ring_buf_ovfw;  // 非循环覆盖模式下ring buffer因软件未及时读数据更新读指针
    volatile unsigned int rresp_err;      // qsp64接口读通道出错告警屏蔽后中断状态实时上报
    volatile unsigned int wresp_err;      // qsp64接口写通道出错告警屏蔽后中断状态实时上报
    volatile unsigned int regs_numerr;    // 读通道配置寄存器个数错误
    volatile unsigned int cpu_wr_reg_err;  // CPU直写不匹配
} sppm_om_info;

typedef int (*sppm_shreshold_cb)(void);

typedef struct {
    struct device *dev;
    void __iomem *regs;
    unsigned int version_id;
    unsigned int running;  // 是否在采样状态

    char platform_name[SPPM_SRC_NAME_LEN];
    int irq;

    unsigned int rchan_start_idx;  // 本代理核上可用的通道起始索引值
    unsigned int rchan_num;        // 本核上可用读通道个数

    int sample_type; /* threshold : 0, circular : 1 */
    int dest_chan;   // 确定此配置的来源，用于控制数据目的通道。

    sppm_shreshold_cb callback;
    struct delayed_work sppm_task; /* Record Data work in bottom half */
    spinlock_t synclock;           /* sync perf_interrupt and perf_del */

    unsigned long long virt_addr;    // ringbuffer 物理地址
    unsigned long long rb_phy_addr;  // ringbuffer 物理地址
    unsigned int rb_size;            // ringbuffer 物理地址
    unsigned int status;
    sppm_om_info om_info;
} sppm_dev_info;

/*
 * sppm_rchan_cfg interface.
 * 用于通道配置
 */
typedef struct {
    char *name;
    unsigned int freq;
    unsigned int type_id;
    unsigned int *event_ids;
    unsigned int event_num;
    char *private_data;
} sppm_rchan_cfg;

sppm_dev_info *sppm_get_dev_info(void);
int sppm_device_config(void);
int sppm_device_deconfig(void);
int sppm_ui_mode_config(sppm_ipc_buf_config *pconfig);
int sppm_pwr_enable(void);
int sppm_pwr_disable(void);
int sppm_rchan_enable_by_logic_id(unsigned int chan_id);
int sppm_rchan_disable_by_logic_id(unsigned int chan_id);
int bsp_sppm_rchan_disable_all(void);
int sppm_shreshold_cb_reg(sppm_shreshold_cb pcb);
// 根据用户界面配置分配一个读通道，界面提前按组分配好下发配置；每组一个PHY通道
int bsp_sppm_rchan_alloc(sppm_rchan_cfg *pui_cfg);
int bsp_sppm_rchan_free(void);
int bsp_sppm_start_sampling(void);
int bsp_sppm_stop_sampling(void);

#endif /* mdrv_sppm.h */
