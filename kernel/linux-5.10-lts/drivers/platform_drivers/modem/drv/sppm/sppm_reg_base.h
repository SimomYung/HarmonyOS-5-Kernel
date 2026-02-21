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
#ifndef SPPM_REG_BASE_H
#define SPPM_REG_BASE_H

#include <osl_types.h>

#define SPPM_READL(base, off) readl((void *)((uintptr_t)(base) + (off)))
#define SPPM_WRITEL(base, val, off) writel((val), (void *)((uintptr_t)(base) + (off)))

void sppm_base_addr_set(void *sppm_base, void *pericrg_base);

// sppm crg reset en
int sppm_reset_en(void);

// sppm crg reset disable
int sppm_reset_dis(void);

// sppm crg clk en
int sppm_crg_clk_en(void);

// sppm crg clk disable
int sppm_crg_clk_dis(void);

int sppm_start_en(void);

int sppm_start_dis(void);

int sppm_rchan_enable(unsigned int chan_idx);

int sppm_rchan_disable(unsigned int chan_idx);

unsigned int sppm_get_rchan_en_state(void);

int sppm_rchan_set_secunsec(unsigned int secunsec);

int sppm_rchan_get_secunsec(void);

int sppm_set_wchan_chanid(unsigned int wchan_id);

int sppm_set_rchan_period(unsigned int chan_id, unsigned int prd_cnt);

int sppm_set_rchan_chanid(unsigned int chan_id, unsigned int chanid);

int sppm_set_rchan_reg_nums(unsigned int chan_id, unsigned int nums);

int sppm_set_rchan_reg_addr(unsigned int chan_id, unsigned int reg_idx, unsigned int reg_val);

int sppm_set_ringbuff_base_addr(unsigned int msb_addr);  // ring buf初始地址，16字节对齐，只需要配置高32位

int sppm_set_ringbuff_size(unsigned int msb_size);  // 外部ring buf大小配置寄存器，以64字节为粒度,大小>>6

int sppm_set_ringbuff_mode(unsigned int mode);

int sppm_set_ringbuff_int_threshold(unsigned int thrd_size);

unsigned int sppm_get_rb_rptr(void);
unsigned int sppm_get_rb_wptr(void);

unsigned int sppm_set_rb_rptr(unsigned int value);
unsigned int sppm_set_rb_wptr(unsigned int value);

int sppm_wchan_enable(void);
int sppm_wchan_disable(void);

int sppm_set_wchan_mst_id(unsigned int msg_id);

int sppm_write_cpu(unsigned int cpudata, unsigned int ts);

int sppm_set_rchan_hibit_addr(unsigned int chan_id, unsigned int msb_addr);

unsigned int sppm_get_32k_slice(void);

unsigned int sppm_get_32k_ov_slice(void);

unsigned int sppm_set_int_enable(unsigned int bitmask);

unsigned int sppm_set_int_disable(unsigned int bitmask);

unsigned int sppm_get_raw_int_status(void);

unsigned int sppm_clear_int_status(unsigned int clr_data);

unsigned int sppm_get_int_status_masked(void);

unsigned int sppm_get_rchan_busy_state(void);
unsigned int sppm_get_all_idle_state(void);

#endif
