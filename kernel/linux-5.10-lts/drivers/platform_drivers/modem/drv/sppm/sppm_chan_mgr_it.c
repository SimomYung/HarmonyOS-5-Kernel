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

#include <bsp_slice.h>
#include <securec.h>
#include <bsp_print.h>
#include <osl_list.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/dma-mapping.h>
#include "mdrv_sppm.h"
#include "sppm_device.h"
#include "sppm_reg_base.h"
#include "sppm_ipc_protocol.h"

#undef THIS_MODU
#define THIS_MODU mod_sppm

#define MS_PER_SECOND 10000
#define ONE_MS_TO_US 1000
#define NORMAL 0x00
#define CIRCULAR 0x01
#define MAX_CHAN_USE 16
#define SPPM_CASE_NAME_LEN 32

int sppm_rb_dump(void);

static unsigned int g_sppm_test_event_ids[SPPM_MAX_REGS_EACH_CHAN] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x010, 0x011, 0x012, 0x013};
static unsigned long g_sppm_test_event_regs[SPPM_MAX_REGS_EACH_CHAN] = {0xE6801EE0,
    0xE6801EE4,
    0xE6801EE8,
    0xE6801EEC,
    0xE6801EE0,
    0xE6801EE4,
    0xE6801EE8,
    0xE6801EEC,
    0xE6801EE0,
    0xE6801EE4,
    0xE6801EE8,
    0xE6801EEC,
    0xE6801EE0,
    0xE6801EE4};

// should be sec regs space
static unsigned long g_sppm_test_event_sec_regs[SPPM_MAX_REGS_EACH_CHAN] = {0xF42010F0,
    0xF42010F4,
    0xF42010F8,
    0xF42010FC,
    0xF42010F0,
    0xF42010F4,
    0xF42010F8,
    0xF42010FC,
    0xF42010F0,
    0xF42010F4,
    0xF42010F8,
    0xF42010FC,
    0xF42010F0,
    0xF42010F4};

typedef struct {
    unsigned char case_name[SPPM_CASE_NAME_LEN];  // 用例名称
    sppm_rchan_cfg *rchan[MAX_CHAN_USE];
    unsigned int rchan_num;    // 用例使用rchan_num
    unsigned int time_second;  // 用例采集时间
    unsigned int mode;         // 0:normal, 1:circular.
    unsigned int rb_size;      // 本用例默认配置ring buffer大小
} sppm_test_config;

unsigned int g_it_chan = SPPM_MAX_RCHAN_NUM;  // 默认配置为最大通道数索引

typedef enum { THRESHOLD_DEAL, THRESHOLD_UN_DEAL } threthold_deal_e;

threthold_deal_e g_threshold_mode = THRESHOLD_DEAL;

// 10ksps
static sppm_rchan_cfg g_sppm_reg_case_hs = {.name = "sppm_ap_client",
    .freq = 100000,
    .type_id = 0x5a,
    .event_num = 14,
    .event_ids = (unsigned int *)g_sppm_test_event_ids};

// 10ksps
static sppm_rchan_cfg g_sppm_reg_case = {.name = "sppm_ap_client",
    .freq = 10000,
    .type_id = 0x5a,
    .event_num = 14,
    .event_ids = (unsigned int *)g_sppm_test_event_ids};

static sppm_rchan_cfg g_sppm_reg_long_period_case = {.name = "sppm_ap_client",
    .freq = 10,
    .type_id = 0x69,
    .event_num = 14,
    .event_ids = (unsigned int *)g_sppm_test_event_ids};

static sppm_rchan_cfg g_sppm_reg_half_freq_case = {.name = "sppm_ap_client",
    .freq = 5000,
    .type_id = 0x5b,
    .event_num = 14,
    .event_ids = (unsigned int *)g_sppm_test_event_ids};

static sppm_rchan_cfg g_sppm_case_reg_err_0 = {.name = "sppm_ap_client",
    .freq = 5000,
    .type_id = 0x5e,
    .event_num = 0,
    .event_ids = (unsigned int *)g_sppm_test_event_ids};

static sppm_rchan_cfg g_sppm_case_reg_err_15 = {.name = "sppm_ap_client",
    .freq = 5000,
    .type_id = 0x5e,
    .event_num = 15,
    .event_ids = (unsigned int *)g_sppm_test_event_ids};

static sppm_rchan_cfg g_sppm_case_ddr = {.name = "sppm_ap_client",
    .freq = 5000,
    .type_id = 0x5d,
    .event_num = 14,
    .event_ids = (unsigned int *)g_sppm_test_event_ids,
    .private_data = "ddr"};

static sppm_rchan_cfg g_sppm_case_ddr_36bit = {.name = "sppm_ap_client",
    .freq = 5000,
    .type_id = 0x88,
    .event_num = 14,
    .event_ids = (unsigned int *)g_sppm_test_event_ids,
    .private_data = "ddr_36"};

static sppm_rchan_cfg g_sppm_case_sec = {.name = "sppm_ap_client",
    .freq = 5000,
    .type_id = 0x60,
    .event_num = 4,
    .event_ids = (unsigned int *)g_sppm_test_event_ids,
    .private_data = "sec"};

sppm_test_config normal_rd_cfg[] = {{.case_name = "normal",
    .rchan = {&g_sppm_reg_case},
    .rchan_num = 1,
    .time_second = 1,
    .mode = NORMAL,
    .rb_size = 0x2000}};

sppm_test_config stress_wr_cfg[] = {{.case_name = "read_cpu_write",
    .rchan = {&g_sppm_reg_case_hs},
    .rchan_num = 1,
    .time_second = 1,
    .mode = NORMAL,
    .rb_size = 0x2000}};

sppm_test_config normal_rd_long_period_cfg[] = {{.case_name = "normal_long_period",
    .rchan = {&g_sppm_reg_long_period_case},
    .rchan_num = 1,
    .time_second = 1,
    .mode = NORMAL,
    .rb_size = 256}};

sppm_test_config normal_rd_circula_cfg[] = {{.case_name = "circular",
    .rchan = {&g_sppm_reg_case},
    .rchan_num = 1,
    .time_second = 5,
    .mode = CIRCULAR,
    .rb_size = 0x1000}};

sppm_test_config hs_rd_circula_cfg[] = {{.case_name = "high_speed_rd_case",
    .rchan = {&g_sppm_reg_case},
    .rchan_num = 1,
    .time_second = 10,
    .mode = CIRCULAR,
    .rb_size = 0x2000}};

sppm_test_config normal_wr_cfg[] = {{.case_name = "normal_wr_case", .mode = CIRCULAR, .rb_size = 0x2000}};

sppm_test_config sppm_rd_chan_period_cases[] = {{.case_name = "mix_with_2:1 freq",
    .rchan = {&g_sppm_reg_case, &g_sppm_reg_half_freq_case},
    .rchan_num = 2,
    .time_second = 2,
    .mode = NORMAL,
    .rb_size = 0x2000}};

sppm_test_config sppm_rd_chan_ddr_cases[] = {
    {.case_name = "ddr_rchan",
        .rchan = {&g_sppm_case_ddr},
        .rchan_num = 1,
        .time_second = 1,
        .mode = NORMAL,
        .rb_size = 0x1000},
};

sppm_test_config sppm_rd_chan_ddr_36bit_cases[] = {
    {.case_name = "ddr_36_rchan",
        .rchan = {&g_sppm_case_ddr_36bit},
        .rchan_num = 1,
        .time_second = 1,
        .mode = NORMAL,
        .rb_size = 0x1000},
};

sppm_test_config sppm_rd_chan_sec_cases[] = {
    {.case_name = "sec_rchan",
        .rchan = {&g_sppm_case_sec},
        .rchan_num = 1,
        .time_second = 1,
        .mode = NORMAL,
        .rb_size = 0x100},
};

sppm_test_config sppm_reg_err_cfg[] = {{.case_name = "reg_err_0",
                                           .rchan = {&g_sppm_case_reg_err_0},
                                           .rchan_num = 1,
                                           .time_second = 1,
                                           .mode = NORMAL,
                                           .rb_size = 0x100},
    {.case_name = "reg_err_15",
        .rchan = {&g_sppm_case_reg_err_15},
        .rchan_num = 1,
        .time_second = 1,
        .mode = NORMAL,
        .rb_size = 0x100}};

int sppm_it_test_init(sppm_client_pd_notify pdfunc)
{
    return SPPM_OK;
}

int sppm_it_test_deinit(void)
{
    return SPPM_OK;
}

#define DDR_REG_LENGTH 0x80
#define DDR_DATA_MAGIC 0x5a
static char *ddr_rd_addr = NULL;
dma_addr_t phy_addr;
unsigned int high_ddr_base = 1;
static char *ddr_36bit_rd_addr = NULL;

int sppm_it_set_36bit_ddr_base(unsigned int high_base)
{
    high_ddr_base = high_base;
    return SPPM_OK;
}

int sppm_it_test_config(sppm_event_entry *pentry, sppm_rchan_regs *pregs)
{
    unsigned int i;
    unsigned long *puregs = g_sppm_test_event_regs;
    sppm_dev_info *dev_info = sppm_get_dev_info();
    if (strcmp(pentry->private_data, "ddr") == 0) {
        sppm_info("<%s> ddr mem space for alloc.\n", __func__);
        if (ddr_rd_addr == NULL) {
            ddr_rd_addr = dma_alloc_coherent(dev_info->dev, DDR_REG_LENGTH, &phy_addr, GFP_KERNEL);
            if (ddr_rd_addr == NULL) {
                sppm_err("<%s> cannot allocate ring buffer\n", __func__);
                return SPPM_ERROR;
            }
        }
        for (i = 0; i < DDR_REG_LENGTH; i++) {
            ddr_rd_addr[i] = i;
        }
        for (i = 0; i < pentry->nums; i++) {
            pregs->uregs[i] = (unsigned long)phy_addr + i * SPPM_CTRL_REG_SIZE;
        }
        pregs->msb_4bit = 0;
    } else if (strcmp(pentry->private_data, "ddr_36") == 0) {
        if (ddr_36bit_rd_addr == NULL) {
            ddr_36bit_rd_addr = (char *)ioremap((phys_addr_t)(uintptr_t)((unsigned long long)high_ddr_base << 0x20), 0x1000);
            if (ddr_36bit_rd_addr == NULL) {
                sppm_err("<%s> cannot ioremap 36bit addr\n", __func__);
                return SPPM_ERROR;
            }
        }
        for (i = 0; i < DDR_REG_LENGTH; i++) {
            ddr_36bit_rd_addr[i] = i + 0x05;
        }
        for (i = 0; i < pentry->nums; i++) {  // 高36位地址配置0x100000000，手动配置ddr值及安全属性；
            pregs->uregs[i] = (unsigned long)i * SPPM_CTRL_REG_SIZE;
        }
        pregs->msb_4bit = high_ddr_base;
    } else if (strcmp(pentry->private_data, "sec") == 0) {
        puregs = g_sppm_test_event_sec_regs;
        for (i = 0; i < pentry->nums; i++) {
            pregs->uregs[i] = puregs[i];
        }
        pregs->msb_4bit = 0;
    } else {
        for (i = 0; i < pentry->nums; i++) {
            pregs->uregs[i] = puregs[i];
        }
        pregs->msb_4bit = 0;
    }

    return SPPM_OK;
}

int sppm_it_test_start(void)
{
    return SPPM_OK;
}

int sppm_it_test_stop(void)
{
    return SPPM_OK;
}

static sppm_reg_interface g_sppm_test_interface = {.name = "sppm_ap_client",
    .init = sppm_it_test_init,
    .deinit = sppm_it_test_deinit,
    .config = sppm_it_test_config,
    .start = sppm_it_test_start,
    .stop = sppm_it_test_stop};

// 水线中断处理
int sppm_it_shreshold_cb(void)
{
    unsigned int rptr;
    if (g_threshold_mode != THRESHOLD_DEAL) {
        return SPPM_OK;
    }
    rptr = sppm_get_rb_wptr();
    (void)sppm_set_rb_rptr(rptr);
    sppm_clear_int_status(SPPM_INT_RING_BUFF_INT);
    sppm_set_int_enable(SPPM_INT_RING_BUFF_INT);
    sppm_info("sppm_it_shreshold_cb cb enter:%d.\n", rptr);
    return SPPM_OK;
}

// 模块主动注册，UI界面匹配
int sppm_it_test_register(void)
{
    int ret;
    ret = bsp_sppm_client_reg(&g_sppm_test_interface, &g_it_chan);
    if (ret != SPPM_OK) {
        sppm_err("bsp_sppm_client_reg err.\n");
        return SPPM_ERROR;
    }
    if (sppm_shreshold_cb_reg(sppm_it_shreshold_cb) != SPPM_OK) {
        sppm_err("sppm_shreshold_cb_reg err.\n");
        return SPPM_ERROR;
    }
    return SPPM_OK;
}

// 模块主动注册，正常使用时UI界面匹配
int sppm_it_test_chan_alloc(sppm_test_config *pconfig)
{
    unsigned int i;
    for (i = 0; i < pconfig->rchan_num; i++) {
        bsp_sppm_rchan_alloc(pconfig->rchan[i]);
    }

    return SPPM_OK;
}

// 模块主动注册，正常使用时UI界面匹配
int sppm_it_test_mode_config(unsigned int rb_size, unsigned int mode)
{
    sppm_ipc_buf_config mode_config;
    mode_config.buffer_size = rb_size;
    mode_config.sample_type = (unsigned int)mode;  // 循环模式 或者中断模式
    mode_config.dest_chan = 0x01;
    sppm_ui_mode_config(&mode_config);
    return SPPM_OK;
}

int sppm_ringbuf_size_wait_forever(int dst_size)
{
    int cur_wptr;
    while (1) {
        cur_wptr = sppm_get_rb_wptr();
        if ((dst_size - cur_wptr) > SPPM_ECAH_MSG_PKG_SIZE) {
            udelay(ONE_MS_TO_US);
        } else {
            return SPPM_OK;
        }
    }
    return SPPM_OK;
}
// it case reset unrest for IP can access.
int sppm_reset_unreset_case(void)
{
    unsigned int rptr;
    sppm_info("sppm_reset_unreset_case.\n");
    sppm_pwr_enable();
    rptr = sppm_get_rb_rptr();
    sppm_info("sppm reg of rptr:%d.\n", rptr);
    sppm_pwr_disable();
    return SPPM_OK;
}

// it case for normal read data check.
int sppm_normal_rd_case(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(normal_rd_cfg) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_normal_rd_case:%s\n", normal_rd_cfg[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(normal_rd_cfg[i].rb_size, normal_rd_cfg[i].mode);
        sppm_it_test_chan_alloc(&normal_rd_cfg[i]);
        bsp_sppm_start_sampling();
        sppm_ringbuf_size_wait_forever(normal_rd_cfg[i].rb_size);
        bsp_sppm_stop_sampling();
        sppm_rb_dump();
    }
    return SPPM_OK;
}

// it case for normal read data check.
int sppm_normal_rd_long_period_case(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(normal_rd_long_period_cfg) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_normal_long_period_rd_case:%s\n", normal_rd_long_period_cfg[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(normal_rd_long_period_cfg[i].rb_size, normal_rd_long_period_cfg[i].mode);
        sppm_it_test_chan_alloc(&normal_rd_long_period_cfg[i]);
        bsp_sppm_start_sampling();
        sppm_ringbuf_size_wait_forever(normal_rd_long_period_cfg[i].rb_size);
        bsp_sppm_stop_sampling();
        sppm_rb_dump();
    }
    return SPPM_OK;
}

// it ap read sec chan reg. no data and should abort.
int sppm_rd_chan_sec_case(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(sppm_rd_chan_sec_cases) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_rd_chan_sec_case name:%s\n", sppm_rd_chan_sec_cases[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(sppm_rd_chan_sec_cases[i].rb_size, sppm_rd_chan_sec_cases[i].mode);
        sppm_it_test_chan_alloc(&sppm_rd_chan_sec_cases[i]);
        bsp_sppm_start_sampling();
        sppm_ringbuf_size_wait_forever(sppm_rd_chan_sec_cases[i].rb_size);
        bsp_sppm_stop_sampling();
        sppm_rb_dump();
    }
    return SPPM_OK;
}

// it case unsec access sec chan.
int sppm_modify_sec_ch_para_case(void)
{
    sppm_info("sppm_modify_sec_ch_para_case.\n");
    sppm_pwr_enable();
    sppm_set_rchan_period(0x12, 0x00);  // 配置好安全属性后，应该会挂。
    return SPPM_OK;
}

// it case for timestamp.
int sppm_normal_timestamp_check_case(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(normal_rd_cfg) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_normal_timestamp_check_case name:%s\n", normal_rd_cfg[i].case_name);
        sppm_info("sppm start at time:%d\n", bsp_get_slice_value());
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(normal_rd_cfg[i].rb_size, normal_rd_cfg[i].mode);
        sppm_it_test_chan_alloc(&normal_rd_cfg[i]);
        bsp_sppm_start_sampling();
        sppm_ringbuf_size_wait_forever(normal_rd_cfg[i].rb_size);
        bsp_sppm_stop_sampling();
        sppm_rb_dump();
    }
    return SPPM_OK;
}

// it case for ringbuffer data check.
int sppm_rd_chan_ddr_case(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(sppm_rd_chan_ddr_cases) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_rd_chan_ddr_case name:%s\n", sppm_rd_chan_ddr_cases[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(sppm_rd_chan_ddr_cases[i].rb_size, sppm_rd_chan_ddr_cases[i].mode);
        sppm_it_test_chan_alloc(&sppm_rd_chan_ddr_cases[i]);
        bsp_sppm_start_sampling();
        sppm_ringbuf_size_wait_forever(sppm_rd_chan_ddr_cases[i].rb_size);
        bsp_sppm_stop_sampling();
        sppm_rb_dump();
    }
    return SPPM_OK;
}

// it case for ringbuffer data check.
int sppm_rd_chan_ddr_36bit_case(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(sppm_rd_chan_ddr_36bit_cases) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_rd_chan_ddr_case name:%s\n", sppm_rd_chan_ddr_36bit_cases[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(sppm_rd_chan_ddr_36bit_cases[i].rb_size, sppm_rd_chan_ddr_36bit_cases[i].mode);
        sppm_it_test_chan_alloc(&sppm_rd_chan_ddr_36bit_cases[i]);
        bsp_sppm_start_sampling();
        sppm_ringbuf_size_wait_forever(sppm_rd_chan_ddr_36bit_cases[i].rb_size);
        bsp_sppm_stop_sampling();
        sppm_rb_dump();
    }
    return SPPM_OK;
}

// it case for ringbuffer data check.
int sppm_normal_rd_dataformat_case(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(normal_rd_cfg) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_normal_rd_dataformat_case:%s\n", normal_rd_cfg[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(normal_rd_cfg[i].rb_size, normal_rd_cfg[i].mode);
        sppm_it_test_chan_alloc(&normal_rd_cfg[i]);
        bsp_sppm_start_sampling();
        sppm_ringbuf_size_wait_forever(normal_rd_cfg[i].rb_size);
        bsp_sppm_stop_sampling();
        sppm_rb_dump();
    }
    return SPPM_OK;
}

// it case reg err check.
int sppm_reg_err_check_case(void)
{
    unsigned int i;    
    for (i = 0; i < sizeof(sppm_reg_err_cfg) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_reg_err_check_case:%s\n", sppm_reg_err_cfg[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(sppm_reg_err_cfg[i].rb_size, sppm_reg_err_cfg[i].mode);
        sppm_it_test_chan_alloc(&sppm_reg_err_cfg[i]);
        sppm_clear_int_status(SPPM_INT_REG_NUM_ERR);
        bsp_sppm_start_sampling();
    }

    // SPPM_INT_CPU_WR_REG_ERR 中断清除
    sppm_clear_int_status(SPPM_INT_REG_NUM_ERR);
    return SPPM_OK;
}

// it case period config case.
int sppm_period_case(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(sppm_rd_chan_period_cases) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_period_case:%s\n", sppm_rd_chan_period_cases[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(sppm_rd_chan_period_cases[i].rb_size, sppm_rd_chan_period_cases[i].mode);
        sppm_it_test_chan_alloc(&sppm_rd_chan_period_cases[i]);
        bsp_sppm_start_sampling();
        sppm_ringbuf_size_wait_forever(sppm_rd_chan_period_cases[i].rb_size);
        bsp_sppm_stop_sampling();
        sppm_rb_dump();
    }
    return SPPM_OK;
}

#define ACPU_CPU_ID 0x01UL
// it case cpu write dir
int sppm_normal_cpu_write_case(unsigned int num)
{
    unsigned int i;
    sppm_info("sppm_normal_cpu_write_case.\n");
    bsp_sppm_rchan_free();
    sppm_it_test_mode_config(normal_wr_cfg[0].rb_size, normal_wr_cfg[0].mode);
    sppm_set_wchan_chanid(0x5a5a);
    bsp_sppm_start_sampling();
    for (i = 0; i < num; i++) {
        sppm_write_cpu((ACPU_CPU_ID << SPPM_CPU_ID_WRITE_OFFSET) | i, bsp_get_slice_value());
    }
    bsp_sppm_stop_sampling();
    sppm_rb_dump();
    return SPPM_OK;
}

// it case cpu write dir
int sppm_normal_cpu_write_id_mismatch_case(void)
{
    unsigned int i;
    unsigned int cur_err_flag;
    volatile unsigned int pre_err_cnt, cur_err_cnt;
    sppm_dev_info *dev_info = sppm_get_dev_info();
    sppm_info("sppm_normal_cpu_write_id_mismatch_case.\n");
    bsp_sppm_rchan_free();
    sppm_it_test_mode_config(normal_wr_cfg[0].rb_size, normal_wr_cfg[0].mode);
    sppm_set_wchan_chanid(0x5a5a);
    sppm_clear_int_status(SPPM_INT_CPU_WR_REG_ERR);
    pre_err_cnt = dev_info->om_info.cpu_wr_reg_err;
    bsp_sppm_start_sampling();
    for (i = 0; i < SPPM_MAX_REGS_EACH_CHAN; i++) {
        sppm_write_cpu(0x03 << SPPM_CPU_ID_WRITE_OFFSET | i, bsp_get_slice_value());
    }
    cur_err_cnt = dev_info->om_info.cpu_wr_reg_err;
    if (pre_err_cnt == cur_err_cnt) {
        bsp_sppm_stop_sampling();
        sppm_info("sppm_normal_cpu_write_id_mismatch_case err.\n");
        return SPPM_ERROR;
    }
    bsp_sppm_stop_sampling();
    return SPPM_OK;
}

// it case disable all chan case.
int sppm_disable_all_rchan_case(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(sppm_rd_chan_period_cases) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_disable_all_rchan_case:%s\n", sppm_rd_chan_period_cases[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(sppm_rd_chan_period_cases[i].rb_size, sppm_rd_chan_period_cases[i].mode);
        sppm_it_test_chan_alloc(&sppm_rd_chan_period_cases[i]);
        bsp_sppm_start_sampling();
        sppm_ringbuf_size_wait_forever(sppm_rd_chan_period_cases[i].rb_size);
        bsp_sppm_rchan_disable_all();
        bsp_sppm_stop_sampling();
    }
    return SPPM_OK;
}

// it case single chan stop case.
int sppm_single_chan_stop_case(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(normal_rd_cfg) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_single_chan_stop_case:%s\n", normal_rd_cfg[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(normal_rd_cfg[i].rb_size, normal_rd_cfg[i].mode);
        sppm_it_test_chan_alloc(&normal_rd_cfg[i]);
        bsp_sppm_start_sampling();
        sppm_ringbuf_size_wait_forever(normal_rd_cfg[i].rb_size >> 1);
        sppm_rchan_disable_by_logic_id(g_it_chan);
        sppm_rchan_enable_by_logic_id(g_it_chan);
        sppm_ringbuf_size_wait_forever(normal_rd_cfg[i].rb_size);
        bsp_sppm_stop_sampling();
    }
    return SPPM_OK;
}

// it case multi chans stop case.
int sppm_multi_chan_stop_case(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(sppm_rd_chan_period_cases) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_multi_chan_stop_case:%s\n", sppm_rd_chan_period_cases[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(sppm_rd_chan_period_cases[i].rb_size, sppm_rd_chan_period_cases[i].mode);
        sppm_it_test_chan_alloc(&sppm_rd_chan_period_cases[i]);
        bsp_sppm_start_sampling();
        sppm_ringbuf_size_wait_forever(sppm_rd_chan_period_cases[i].rb_size >> 1);

        sppm_rchan_disable_by_logic_id(g_it_chan);

        sppm_rchan_enable_by_logic_id(g_it_chan);

        sppm_ringbuf_size_wait_forever(sppm_rd_chan_period_cases[i].rb_size);
        bsp_sppm_stop_sampling();
    }
    return SPPM_OK;
}

#define SPPM_IT_RB_WATI_MS_INTERVAL 1000
// it case ring buffer over flow.
int sppm_ring_buf_ovfw_case(void)
{
    unsigned int i;
    sppm_dev_info *dev_info = sppm_get_dev_info();
    volatile unsigned int pre_err_cnt, cur_err_cnt;
    unsigned int delay_cnt = 40;
    for (i = 0; i < sizeof(normal_rd_cfg) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_ring_buf_ovfw_case:%s\n", normal_rd_cfg[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(normal_rd_cfg[i].rb_size, normal_rd_cfg[i].mode);
        sppm_it_test_chan_alloc(&normal_rd_cfg[i]);
        g_threshold_mode = THRESHOLD_UN_DEAL;
        bsp_sppm_start_sampling();
        pre_err_cnt = dev_info->om_info.ring_buf_ovfw;
        cur_err_cnt = pre_err_cnt;
        while (cur_err_cnt == pre_err_cnt) {
            mdelay(SPPM_IT_RB_WATI_MS_INTERVAL);
            if (delay_cnt-- == 0) {
                break;
            }
            cur_err_cnt = dev_info->om_info.ring_buf_ovfw;
        }
        bsp_sppm_stop_sampling();
        g_threshold_mode = THRESHOLD_DEAL;
    }
    return SPPM_OK;
}

// it case ring buffer over flow recover.
int sppm_ring_buf_ovfw_recover_case(void)
{
    unsigned int i;
    unsigned int rptr;

    sppm_dev_info *dev_info = sppm_get_dev_info();
    volatile unsigned int pre_err_cnt, cur_err_cnt;
    for (i = 0; i < sizeof(normal_rd_cfg) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_ring_buf_ovfw_recover_case:%s\n", normal_rd_cfg[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(normal_rd_cfg[i].rb_size, normal_rd_cfg[i].mode);
        sppm_it_test_chan_alloc(&normal_rd_cfg[i]);
        g_threshold_mode = THRESHOLD_UN_DEAL;
        bsp_sppm_start_sampling();
        pre_err_cnt = dev_info->om_info.ring_buf_ovfw;
        cur_err_cnt = pre_err_cnt;
        while (cur_err_cnt == pre_err_cnt) {
            cur_err_cnt = dev_info->om_info.ring_buf_ovfw;
        }
        rptr = sppm_get_rb_wptr();
        (void)sppm_set_rb_rptr(rptr);
        sppm_set_int_enable(SPPM_INT_RING_BUFF_OVFW);
        sppm_ringbuf_size_wait_forever(normal_rd_cfg[i].rb_size >> 1);

        bsp_sppm_stop_sampling();
    }
    g_threshold_mode = THRESHOLD_DEAL;
    return SPPM_OK;
}

// it case shreshold level interrupt.
int sppm_shreshold_cnt_case(void)
{
    unsigned int i;
    sppm_dev_info *dev_info = sppm_get_dev_info();
    volatile unsigned int pre_err_cnt, cur_err_cnt;
    for (i = 0; i < sizeof(normal_rd_cfg) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_shreshold_cnt_case:%s\n", normal_rd_cfg[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(normal_rd_cfg[i].rb_size, normal_rd_cfg[i].mode);
        sppm_it_test_chan_alloc(&normal_rd_cfg[i]);
        bsp_sppm_start_sampling();
        pre_err_cnt = dev_info->om_info.shreshold_cnt;
        sppm_ringbuf_size_wait_forever(normal_rd_cfg[i].rb_size);
        cur_err_cnt = dev_info->om_info.shreshold_cnt;
        if (cur_err_cnt == pre_err_cnt) {
            bsp_sppm_stop_sampling();
            sppm_info("sppm_shreshold_cnt_case case err.\n");
            return SPPM_ERROR;
        }
        bsp_sppm_stop_sampling();
    }
    return SPPM_OK;
}

// it case shreshold level interrupt deal procedure.
#define SPPM_IT_SHRESHOLD_TIMES 3
int sppm_shreshold_continue_case(void)
{
    unsigned int i;
    sppm_dev_info *dev_info = sppm_get_dev_info();
    volatile unsigned int pre_err_cnt, cur_err_cnt;
    for (i = 0; i < sizeof(normal_rd_cfg) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_shreshold_continue_case:%s\n", normal_rd_cfg[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(normal_rd_cfg[i].rb_size, normal_rd_cfg[i].mode);
        sppm_it_test_chan_alloc(&normal_rd_cfg[i]);
        bsp_sppm_start_sampling();
        pre_err_cnt = dev_info->om_info.shreshold_cnt;
        cur_err_cnt = dev_info->om_info.shreshold_cnt;
        while (cur_err_cnt < (pre_err_cnt + SPPM_IT_SHRESHOLD_TIMES)) {
            cur_err_cnt = dev_info->om_info.shreshold_cnt;
        }
        bsp_sppm_stop_sampling();
    }
    return SPPM_OK;
}

// it case shreshold level interrupt.
int sppm_circular_begin_case(void)
{
    unsigned int i;

    for (i = 0; i < sizeof(normal_rd_circula_cfg) / sizeof(sppm_test_config); i++) {
        sppm_info("sppm_circular_case:%s\n", normal_rd_circula_cfg[i].case_name);
        bsp_sppm_rchan_free();
        sppm_it_test_mode_config(normal_rd_circula_cfg[i].rb_size, normal_rd_circula_cfg[i].mode);
        sppm_it_test_chan_alloc(&normal_rd_circula_cfg[i]);
        bsp_sppm_start_sampling();
    }
    return SPPM_OK;
}

// it security mode can only be set in sec world.
int sppm_security_case(void)
{
    unsigned int secconfig = 0xff00;
    unsigned int secconfig_rd;
    sppm_info("sppm_security_case.\n");
    sppm_pwr_enable();
    secconfig_rd = sppm_rchan_get_secunsec();
    if (secconfig_rd != 0) {
        sppm_err("<%s> sppm has been configed:0x%x\n", __func__, secconfig_rd);
        secconfig = ~secconfig_rd;
        sppm_rchan_set_secunsec(secconfig);
        secconfig_rd = sppm_rchan_get_secunsec();
        if (secconfig_rd == secconfig) {
            sppm_err("<%s> sppm_security_case err at first config\n", __func__);
            sppm_pwr_disable();
            return SPPM_ERROR;
        }
        return SPPM_OK;
    }
    sppm_rchan_set_secunsec(secconfig);
    secconfig_rd = sppm_rchan_get_secunsec();
    if (secconfig_rd != 0xff00) {
        sppm_err("<%s> sppm_security_case err at first config\n", __func__);
        sppm_pwr_disable();
        return SPPM_ERROR;
    }
    secconfig = 0x00ff;
    sppm_rchan_set_secunsec(secconfig);
    secconfig_rd = sppm_rchan_get_secunsec();
    if (secconfig_rd != 0xff00) {
        sppm_err("<%s> sppm_security_case err at first config\n", __func__);
        sppm_pwr_disable();
        return SPPM_ERROR;
    }
    sppm_pwr_disable();
    return SPPM_OK;
}

// it case high speed rchan read stress, alloc all chan
int sppm_high_speed_stress_begin_case(void)
{
    int i;
    sppm_info("sppm_high_speed_stress_case.\n");
    bsp_sppm_rchan_free();
    sppm_it_test_mode_config(hs_rd_circula_cfg[0].rb_size, hs_rd_circula_cfg[0].mode);
    for (i = 0; i < MAX_CHAN_USE; i++) {
        sppm_it_test_chan_alloc(&hs_rd_circula_cfg[0]);
    }
    bsp_sppm_start_sampling();

    return SPPM_OK;
}

// it case cpu write stress
int sppm_cpu_write_stress_case(unsigned int times)
{
    unsigned int i, j;
    sppm_info("sppm_cpu_write_stress_case.\n");
    bsp_sppm_rchan_free();
    sppm_it_test_mode_config(normal_wr_cfg[0].rb_size, normal_wr_cfg[0].mode);
    sppm_set_wchan_chanid(0x5a5a);
    bsp_sppm_start_sampling();
    for (j = 0; j < times; j++) {
        for (i = 0; i < SPPM_MAX_REGS_EACH_CHAN; i++) {
            sppm_write_cpu((ACPU_CPU_ID << SPPM_CPU_ID_WRITE_OFFSET) | i, bsp_get_slice_value());
        }
    }
    bsp_sppm_stop_sampling();
    sppm_rb_dump();
    return SPPM_OK;
}

// it case rchan read stress
int sppm_rchan_stress_begin_case(void)
{
    unsigned int i;
    sppm_info("sppm_rchan_stress_case.\n");
    bsp_sppm_rchan_free();
    sppm_it_test_mode_config(normal_rd_circula_cfg[0].rb_size, normal_rd_circula_cfg[0].mode);
    for (i = 0; i < MAX_CHAN_USE; i++) {
        sppm_it_test_chan_alloc(&normal_rd_circula_cfg[0]);
    }
    bsp_sppm_start_sampling();
    return SPPM_OK;
}

// it case rchan read stress
int sppm_rchan_stress_cpu_wr_begin_case(unsigned int times)
{
    unsigned int i, j;
    sppm_info("sppm_rchan_stress_case.\n");
    bsp_sppm_rchan_free();
    sppm_it_test_mode_config(stress_wr_cfg[0].rb_size, stress_wr_cfg[0].mode);
    for (i = 0; i < MAX_CHAN_USE; i++) {
        sppm_it_test_chan_alloc(&stress_wr_cfg[0]);
    }
    bsp_sppm_start_sampling();
    for (j = 0; j < times; j++) {
        for (i = 0; i < SPPM_MAX_REGS_EACH_CHAN; i++) {
            sppm_write_cpu((ACPU_CPU_ID << SPPM_CPU_ID_WRITE_OFFSET) | i, bsp_get_slice_value());
        }
    }
    return SPPM_OK;
}

#define SPPM_DUMP_COL_EACH_LING 8

int sppm_rb_dump(void)
{
    sppm_dev_info *dev_info = sppm_get_dev_info();
    unsigned int i;
    char *pint = (char *)dev_info->virt_addr;
    if (pint == NULL) {
        sppm_err("rb not init.\n");
    }
    sppm_info("sppm ring buff dump with size:%d:\n", dev_info->rb_size);
    for (i = 0; i < dev_info->rb_size / sizeof(char) / SPPM_DUMP_COL_EACH_LING; i++) {
        sppm_info("0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",
            *(pint + i * SPPM_DUMP_COL_EACH_LING + 0x00),
            *(pint + i * SPPM_DUMP_COL_EACH_LING + 0x01),
            *(pint + i * SPPM_DUMP_COL_EACH_LING + 0x02),
            *(pint + i * SPPM_DUMP_COL_EACH_LING + 0x03),
            *(pint + i * SPPM_DUMP_COL_EACH_LING + 0x04),
            *(pint + i * SPPM_DUMP_COL_EACH_LING + 0x05),
            *(pint + i * SPPM_DUMP_COL_EACH_LING + 0x06),
            *(pint + i * SPPM_DUMP_COL_EACH_LING + 0x07));
    }
    return SPPM_OK;
}
