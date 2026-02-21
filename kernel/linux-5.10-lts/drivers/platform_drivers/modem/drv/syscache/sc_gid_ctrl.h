/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
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
#ifndef __SC_GID_CTRL_H_
#define __SC_GID_CTRL_H_

#include <linux/types.h>
#include <linux/version.h>
#include <linux/spinlock.h>
#include <bsp_syscache.h>

#define SC_GID_CTRL_MAX 32
#define SC_GID_CTRL_ALIGN 0x1000
#define SC_GID_PHONE_USED_CNT 4

struct gid_ctrl_cfg_dt {
    unsigned int arhint;
    unsigned int awhint;
    unsigned int arcache;
    unsigned int awcache;
    unsigned int domain;
};

typedef union {
    struct {
        unsigned int awcache : 4;
        unsigned int arcache : 4;
        unsigned int domain : 2;
        unsigned int awhint : 4;
        unsigned int arhint : 4;
        unsigned int gid : 6;
        unsigned int partid : 2;
        unsigned int override_en : 6;
    } bits;
    unsigned int val;
} gid_cfg;

struct gid_ctrl_cfg_om {
    unsigned long long start_addr;
    unsigned long long end_addr;
    unsigned int size;
    unsigned int cfg;
    unsigned int enable;
};

struct gid_ctrl {
    char *regs_acpu;
    char *regs_peri;
    unsigned int init;
    spinlock_t lock;
    gid_cfg common_cfg;
    unsigned int gid_only;
    unsigned int entry_max_num;
    unsigned int entry_bit_map;
    unsigned int sc_mode_idx;
    unsigned int *dump_base;
    unsigned int sc_gid_cmo[SC_GID_PHONE_USED_CNT];
    struct gid_ctrl_cfg_om cfg_om[SC_GID_CTRL_MAX];
};

void sc_gid_ctrl_hal_addr_cfg(struct gid_ctrl *gid_ctx, unsigned int start_addr, unsigned int end_addr, unsigned int cfg, int idx);
void sc_gid_ctrl_hal_enabel(struct gid_ctrl *gid_ctx, unsigned int idx, unsigned int enable);

int sc_addr_config(unsigned int start_addr, unsigned int end_addr, int rpolicy, int wpolicy, int gid, int idx);
void sc_addr_enable(unsigned int idx, unsigned int enable);
struct gid_ctrl *sc_get_gid_ctx(void);
void sc_gid_ctrl_dump_reg(struct gid_ctrl *gid_ctx);

// test
void sc_gid_ctrl_show(unsigned int idx);
void gid_ctrl_dump_hook(void);

#endif
