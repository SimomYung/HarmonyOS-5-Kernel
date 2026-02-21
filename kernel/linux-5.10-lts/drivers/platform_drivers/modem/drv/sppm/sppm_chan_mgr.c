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
#include <securec.h>
#include <osl_list.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include "sppm_reg_base.h"
#include "sppm_device.h"
#include "mdrv_sppm.h"

#undef THIS_MODU
#define THIS_MODU mod_sppm

#define SPPM_RUNNING 0x01
#define SPPM_STOP    0x0
/*
 * sppm_rchan_unit interface.
 * 用于本逻辑通道
 */
typedef struct {
    char name[SPPM_SRC_NAME_LEN];
    unsigned int logic_id;
    unsigned int phy_id;
    unsigned int event_ids[SPPM_MAX_REGS_EACH_CHAN];
    unsigned int event_num;
    unsigned int event_reg_addr[SPPM_MAX_REGS_EACH_CHAN];
    unsigned char msb_4bit;
    char private_data[SPPM_SRC_PRIV_LEN];
    unsigned int freq;
    unsigned int type_id;
} sppm_rchan_unit;

typedef struct {
    unsigned int client_reg_idx;                      // 已注册的模块数目
    unsigned int rchan_idx;                           // 读通道索引
    sppm_reg_interface *pclient[SPPM_MAX_RCHAN_NUM];  // 回调函数结构体列表
    sppm_rchan_unit *prchan[SPPM_MAX_RCHAN_NUM];      // 读通道控制单元 ,每个client可能使用多个读通道
} sppm_rchan_mgr;

sppm_rchan_mgr g_sppm_rchan_mgr = {0};  // 默认初始化为0

// 可能在系统初始化阶段由模块提前注册，需要保证链表随时可挂接,理论上每个核可以注册client不受限制，不能重复注册
int bsp_sppm_client_reg(sppm_reg_interface *preg, unsigned int *chan)
{
    unsigned int i;
    if (preg == NULL) {
        sppm_err("client preg NULL.\n");
        return SPPM_ERROR;
    }

    if (g_sppm_rchan_mgr.client_reg_idx >= SPPM_MAX_RCHAN_NUM) {
        sppm_err("client preg exceed max num.\n");
        return SPPM_ERROR;
    }
    for (i = 0; i < g_sppm_rchan_mgr.client_reg_idx; i++) {
        if (strcmp(preg->name, g_sppm_rchan_mgr.pclient[i]->name) == 0) {
            sppm_err("client %s has registered.\n", preg->name);
            return SPPM_ERROR;
        }
    }

    g_sppm_rchan_mgr.pclient[g_sppm_rchan_mgr.client_reg_idx] = preg;

    // 返回当前索引，可以作为逻辑通道使用
    *chan = g_sppm_rchan_mgr.client_reg_idx++;
    return SPPM_OK;
}

// 读通道对应Reg寄存器获取接口，根据logic_id，找到对应的config注册函数，获取event_reg_addr
static int sppm_get_rchan_regs(sppm_rchan_unit *prchan_unit)
{
    sppm_event_entry event_entry;
    sppm_rchan_regs rchan_regs;
    unsigned int i;
    event_entry.events_list = prchan_unit->event_ids;
    event_entry.nums = prchan_unit->event_num;
    event_entry.private_data = prchan_unit->private_data;

    if (g_sppm_rchan_mgr.pclient[prchan_unit->logic_id]->config(&event_entry, &rchan_regs) == SPPM_OK) {
        for (i = 0; i < prchan_unit->event_num; i++) {
            prchan_unit->event_reg_addr[i] = rchan_regs.uregs[i];
        }
        prchan_unit->msb_4bit = rchan_regs.msb_4bit;
        return SPPM_OK;
    }
    sppm_err("get client config regs failed.\n");
    return SPPM_ERROR;
}

int sppm_rchan_alloc_cfg_check(sppm_rchan_cfg *pui_cfg, unsigned int *match_client_id)
{
    sppm_dev_info *dev_info = sppm_get_dev_info();
    unsigned int i;
    if (dev_info == NULL) {
        sppm_err("dev not inited.\n");
        return SPPM_ERROR;
    }
    for (i = 0; i < g_sppm_rchan_mgr.client_reg_idx; i++) {
        if (strcmp(pui_cfg->name, g_sppm_rchan_mgr.pclient[i]->name) == 0) {
            break;
        }
    }
    if (i == g_sppm_rchan_mgr.client_reg_idx) {
        sppm_err("client not found: %s\n", pui_cfg->name);
        return SPPM_ERROR;
    }

    if (g_sppm_rchan_mgr.rchan_idx >= dev_info->rchan_num || pui_cfg->event_num > SPPM_MAX_REGS_EACH_CHAN) {
        sppm_err("all read chans have been allocated\n");
        return SPPM_ERROR;
    }

    if (pui_cfg->event_num > SPPM_MAX_REGS_EACH_CHAN) {
        sppm_err("pui_cfg para fail with event_num:%d.\n", pui_cfg->event_num);
        return SPPM_ERROR;
    }
    *match_client_id = i;
    return SPPM_OK;
}

// 根据界面配置，进行物理读通道分配，需要界面下传名字匹配
int bsp_sppm_rchan_alloc(sppm_rchan_cfg *pui_cfg)
{
    sppm_rchan_unit *pcfg;
    sppm_dev_info *dev_info = sppm_get_dev_info();
    unsigned int match_client_id;
    if (sppm_rchan_alloc_cfg_check(pui_cfg, &match_client_id) != SPPM_OK) {
        return SPPM_ERROR;
    }

    pcfg = kmalloc(sizeof(sppm_rchan_unit), GFP_KERNEL);
    if (pcfg == NULL) {
        sppm_err("rchan unit ctrl allocated fail.\n");
        return SPPM_ERROR;
    }
    (void)memset_s(pcfg, sizeof(sppm_rchan_unit), 0, sizeof(sppm_rchan_unit));
    pcfg->logic_id = match_client_id;
    pcfg->event_num = pui_cfg->event_num;
    pcfg->type_id = pui_cfg->type_id;
    pcfg->freq = pui_cfg->freq;

    if (strcpy_s(pcfg->name, SPPM_SRC_NAME_LEN, pui_cfg->name)) {
        kfree(pcfg);
        sppm_err("pcfg name copy fail.\n");
    }
    if (memcpy_s(pcfg->event_ids,
        sizeof(int) * SPPM_MAX_REGS_EACH_CHAN,
        pui_cfg->event_ids,
        sizeof(int) * pui_cfg->event_num)) {
        kfree(pcfg);
        sppm_err("private data copy fail.\n");
    }

    if (pui_cfg->private_data != NULL) {
        if (memcpy_s(pcfg->private_data, SPPM_SRC_PRIV_LEN, pui_cfg->private_data, strlen(pui_cfg->private_data))) {
            kfree(pcfg);
            sppm_err("private data copy fail.\n");
        }
    }

    if (sppm_get_rchan_regs(pcfg) == SPPM_OK) {
        pcfg->phy_id = dev_info->rchan_start_idx + g_sppm_rchan_mgr.rchan_idx;
        g_sppm_rchan_mgr.prchan[g_sppm_rchan_mgr.rchan_idx++] = pcfg;
        return SPPM_OK;
    }
    kfree(pcfg);
    sppm_err("rchan unit init fail.\n");
    return SPPM_ERROR;
}

// 物理读通道释放
int bsp_sppm_rchan_free(void)
{
    unsigned int i;
    for (i = 0; i < g_sppm_rchan_mgr.rchan_idx; i++) {
        kfree(g_sppm_rchan_mgr.prchan[i]);
        g_sppm_rchan_mgr.prchan[i] = NULL;
    }

    g_sppm_rchan_mgr.rchan_idx = 0;
    return SPPM_OK;
}

// 根据频率转换为采样周期计数器接口
int sppm_freq_to_interval(unsigned int freq)
{
    if (freq == 0) {
        return 0;
    }
    return SPPM_SMP_BASE_FREQ / freq;
}

// 读通道配置接口，配置周期，regs，nums, chan_id;
int sppm_rchan_para_config(void)
{
    unsigned int i, j;
    unsigned int freq2interval;

    for (i = 0; i < g_sppm_rchan_mgr.rchan_idx; i++) {
        sppm_info("<%s> phy chan %d\n", __func__, g_sppm_rchan_mgr.prchan[i]->phy_id);
        freq2interval = sppm_freq_to_interval(g_sppm_rchan_mgr.prchan[i]->freq);
        sppm_set_rchan_period(g_sppm_rchan_mgr.prchan[i]->phy_id, freq2interval);
        sppm_set_rchan_chanid(g_sppm_rchan_mgr.prchan[i]->phy_id, g_sppm_rchan_mgr.prchan[i]->type_id);
        sppm_set_rchan_reg_nums(g_sppm_rchan_mgr.prchan[i]->phy_id, g_sppm_rchan_mgr.prchan[i]->event_num);
        for (j = 0; j < g_sppm_rchan_mgr.prchan[i]->event_num; j++) {
            sppm_set_rchan_reg_addr(
                g_sppm_rchan_mgr.prchan[i]->phy_id, j, g_sppm_rchan_mgr.prchan[i]->event_reg_addr[j]);
        }
        sppm_set_rchan_hibit_addr(g_sppm_rchan_mgr.prchan[i]->phy_id, g_sppm_rchan_mgr.prchan[i]->msb_4bit);
    }
    return SPPM_OK;
}

int bsp_sppm_rchan_enable_all(void)
{
    unsigned int i;
    for (i = 0; i < g_sppm_rchan_mgr.rchan_idx; i++) {
        sppm_rchan_enable(g_sppm_rchan_mgr.prchan[i]->phy_id);
    }
    return SPPM_OK;
}

int bsp_sppm_rchan_disable_all(void)
{
    unsigned int i;
    for (i = 0; i < g_sppm_rchan_mgr.rchan_idx; i++) {
        sppm_rchan_disable(g_sppm_rchan_mgr.prchan[i]->phy_id);
    }
    return SPPM_OK;
}

int sppm_rchan_enable_by_logic_id(unsigned int chan_id)
{
    unsigned int i;
    for (i = 0; i < g_sppm_rchan_mgr.rchan_idx; i++) {
        if (g_sppm_rchan_mgr.prchan[i]->logic_id == chan_id) {
            sppm_rchan_enable(g_sppm_rchan_mgr.prchan[i]->phy_id);
        }
    }
    return SPPM_OK;
}

int sppm_rchan_disable_by_logic_id(unsigned int chan_id)
{
    unsigned int i;
    for (i = 0; i < g_sppm_rchan_mgr.rchan_idx; i++) {
        if (g_sppm_rchan_mgr.prchan[i]->logic_id == chan_id) {
            sppm_rchan_disable(g_sppm_rchan_mgr.prchan[i]->phy_id);
        }
    }

    return SPPM_OK;
}

int bsp_sppm_client_pd_notify(unsigned int chan_id, client_pm_state_e pw_state)
{
    sppm_dev_info *dev_info = sppm_get_dev_info();
    if (chan_id >= g_sppm_rchan_mgr.client_reg_idx) {
        return SPPM_ERROR;
    }

    if (pw_state == SPPM_CLIENT_ON) {  // power on client can be sampled.
        if (dev_info->running == SPPM_STOP) {
            return SPPM_OK;
        }
        return sppm_rchan_enable_by_logic_id(chan_id);
    } else { // power off client can not be sampled.
        if (dev_info->running == SPPM_STOP) {
            return SPPM_OK;
        }
        return sppm_rchan_disable_by_logic_id(chan_id);
    }
    return SPPM_OK;
}

static int sppm_cpu_write_en(void)
{
    return sppm_wchan_enable();
}

static int sppm_cpu_write_disable(void)
{
    return sppm_wchan_disable();
}

int bsp_sppm_client_init(void)
{
    unsigned int i;
    for (i = 0; i < g_sppm_rchan_mgr.client_reg_idx; i++) {
        if (g_sppm_rchan_mgr.pclient[i]->init(bsp_sppm_client_pd_notify) != SPPM_OK) {
            return SPPM_ERROR;
        }
    }
    return SPPM_OK;
}

// 整体控制SPPM启动采样
int bsp_sppm_start_sampling(void)
{
    unsigned int i;
    sppm_dev_info *dev_info = sppm_get_dev_info();
    sppm_pwr_enable();
    sppm_cpu_write_en();
    // 回调各个IP启动采样通知
    for (i = 0; i < g_sppm_rchan_mgr.client_reg_idx; i++) {
        if (g_sppm_rchan_mgr.pclient[i]->start() != SPPM_OK) {
            sppm_err("start fail: %s\n", g_sppm_rchan_mgr.pclient[i]->name);
            return SPPM_ERROR;
        }
    }
    sppm_rchan_para_config();
    bsp_sppm_rchan_enable_all();
    sppm_device_config();
    sppm_start_en();
    sppm_clear_int_status(SPPM_INT_RING_BUFF_INT);   // 芯片约束，参考使用说明
    sppm_set_int_enable(SPPM_INT_ALL);
    dev_info->running = SPPM_RUNNING;
    return SPPM_OK;
}

// 整体控制SPPM停止采样
int bsp_sppm_stop_sampling(void)
{
    unsigned int i;
    sppm_dev_info *dev_info = sppm_get_dev_info();
    unsigned int delay_cnt = SPPM_RCHAN_BUSY_DELAY_US_NUM;
    unsigned int idle_state;

    dev_info->running = SPPM_STOP;

    // 回调各个IP停止采样通知
    for (i = 0; i < g_sppm_rchan_mgr.client_reg_idx; i++) {
        if (g_sppm_rchan_mgr.pclient[i]->stop() != SPPM_OK) {
            sppm_err("stop fail: %s\n", g_sppm_rchan_mgr.pclient[i]->name);
        }
    }
    sppm_cpu_write_disable();
    bsp_sppm_rchan_disable_all();
    sppm_start_dis();
    sppm_set_int_disable(SPPM_INT_ALL);
    idle_state = sppm_get_all_idle_state();
    while ((idle_state & 0x01) != 0) {
        udelay(0x01);
        delay_cnt--;
        if (delay_cnt == 0) {
            sppm_err("sppm stop wait idle overtime.\n");
            return SPPM_ERROR;
        }
        idle_state = sppm_get_all_idle_state();
    }
    sppm_info("sppm stop with delay us:%d\n", SPPM_RCHAN_BUSY_DELAY_US_NUM - delay_cnt);
    return SPPM_OK;
}

int sppm_config_data_show(void)
{
    unsigned int i, j;
    sppm_info("reg client numbers: %d\n", g_sppm_rchan_mgr.client_reg_idx);
    for (i = 0; i < g_sppm_rchan_mgr.client_reg_idx; i++) {
        sppm_info("client name: %s\n", g_sppm_rchan_mgr.pclient[i]->name);
    }
    sppm_info("\n");
    for (i = 0; i < g_sppm_rchan_mgr.rchan_idx; i++) {
        sppm_info("rchan for client: %s\n", g_sppm_rchan_mgr.prchan[i]->name);
        sppm_info("rchan logic_id: %d\n", g_sppm_rchan_mgr.prchan[i]->logic_id);
        sppm_info("rchan phy_id: %d\n", g_sppm_rchan_mgr.prchan[i]->phy_id);
        sppm_info("rchan event_num: %d\n", g_sppm_rchan_mgr.prchan[i]->event_num);
        sppm_info("rchan freq: %d\n", g_sppm_rchan_mgr.prchan[i]->freq);
        sppm_info("rchan type_id: 0x%x\n", g_sppm_rchan_mgr.prchan[i]->type_id);
        sppm_info("rchan event_id vs event_reg:\n");
        for (j = 0; j < g_sppm_rchan_mgr.prchan[i]->event_num; j++) {
            sppm_info("id: %d, reg:0x%x\n",
                g_sppm_rchan_mgr.prchan[i]->event_ids[j],
                g_sppm_rchan_mgr.prchan[i]->event_reg_addr[j]);
        }
    }
    return SPPM_OK;
}
