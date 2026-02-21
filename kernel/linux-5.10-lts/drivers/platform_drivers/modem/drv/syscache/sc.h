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
#ifndef __SC_H_
#define __SC_H_

#include <linux/platform_device.h>
#include <of.h>
#include "sc_gid_ctrl.h"
#include <bsp_syscache.h>
#include <product_config.h>

#define SC_INT_CNT 2
#define SC_GID_MAX 16
#define SC_DFX_INT_BIT_CNT 32

typedef union {
    unsigned int value;
    struct {
        unsigned int quota_l : 15;
        unsigned int reserved0 : 1;
        unsigned int quota_h : 15;
        unsigned int reserved1 : 1;
    } gid_quota;
    struct {
        unsigned int gid_way_en : 16;
        unsigned int gid_way_srch : 16;
    } gid_way_allc;
    struct {
        unsigned int normal_rd_allc_plc : 2;
        unsigned int normal_wr_allc_plc : 2;
        unsigned int midflt_rd_allc_plc : 2;
        unsigned int midflt_wr_allc_plc : 2;
        unsigned int replace_same_gid_en  : 1;
        unsigned int gid_priority : 2;
        unsigned int gid_en : 1;
        unsigned int sot_ignore : 1;
        unsigned int gid_hint_en : 1;
        unsigned int res : 18;
    } gid_allc_plc;
    union {
        struct {
            unsigned int mid_flt0 : 8;
            unsigned int res0 : 8;
            unsigned int mid_flt0_msk : 8;
            unsigned int res1 : 7;
            unsigned int mid_flt0_en : 1;
        } gid_mid_flt0;

        struct {
            unsigned int mid_flt1 : 8;
            unsigned int res0 : 8;
            unsigned int mid_flt1_msk : 8;
            unsigned int res1 : 7;
            unsigned int mid_flt1_en : 1;
        } gid_mid_flt1;
    } reg;
} sc_gid_cfg;

typedef union {
    unsigned int val;
    struct {
        unsigned int res0 : 12;
        unsigned int cmo_cmd_acess_err : 1;
        unsigned int cmo_overflow_err : 1;
        unsigned int cmo_pa_err : 1;
        unsigned int cmo_byway_err : 1;
        unsigned int cmo_bygid_err : 1;
        unsigned int res1 : 15;
    } bits;
} cmo_irq_stat;

typedef union {
    unsigned int val;
    struct {
        unsigned int res0 : 11;
        unsigned int dfx_cmogid_illegal : 1;
        unsigned int dfx_cmo_sec : 1;
        unsigned int dfx_cmo_lpway : 1;
        unsigned int dfx_cmd_lpway : 1;
        unsigned int dfx_multihit : 1;
        unsigned int dfx_pri_noalloc : 1;
        unsigned int dfx_gid_mismatch : 1;
        unsigned int dfx_gid_illegal : 1;
        unsigned int eccerr : 1;
        unsigned int pmu_cnt11 : 1;
        unsigned int pmu_cnt10 : 1;
        unsigned int pmu_cnt9 : 1;
        unsigned int pmu_cnt8 : 1;
        unsigned int pmu_cnt7 : 1;
        unsigned int pmu_cnt6 : 1;
        unsigned int pmu_cnt5 : 1;
        unsigned int pmu_cnt4 : 1;
        unsigned int pmu_cnt3 : 1;
        unsigned int pmu_cnt2 : 1;
        unsigned int pmu_cnt1 : 1;
        unsigned int pmu_cnt0 : 1;
    } bits;
} dfx_irq_stat;

struct sc_dfx_err {
    unsigned int cnt[SC_DFX_INT_BIT_CNT];
    unsigned int dfx_cmo_gid_err;
    unsigned int dfx_cmo_lpway;
    unsigned int dfx_cmd_lpway;
    unsigned int dfx_multihit;

    unsigned int dfx_pri_noalloc;
    unsigned int dfx_gid_mismatch;
    unsigned int dfx_gid_err;
    unsigned int dfx_eccerr;
};

struct sc {
    char *regs;
    struct device *dev;
    int irq_fc;
    int irq_dfx;
    unsigned int dbg;
    unsigned int irq_fc_mask;
    unsigned int irq_dfx_mask;
    unsigned long irq_flag;
    unsigned int cmo_err_irq;
    unsigned int dfx_err_irq[SC_INT_CNT];
    unsigned int ecc_err[SC_INT_CNT];

    unsigned int init;
    spinlock_t lock;
    unsigned int slice_cnt;
    unsigned int max_quota;
    unsigned int gid_quota_ringbuf;
    unsigned int ramaining_quota;
    unsigned int timeout;
    unsigned int cmo_clr_fail;

    // cmo异常中断计数
    unsigned int cmo_cmd_acess_err;
    unsigned int cmo_overflow_err;
    unsigned int cmo_pa_err;
    unsigned int cmo_byway_err;
    unsigned int cmo_bygid_err;

    // dfx异常中断计数
    struct sc_dfx_err *dfx_cnt;

    unsigned int cmo_cache_err;
};

typedef enum {
    SC_CMO_EVENT = 0,
    SC_CMO_INTR = 1,
} sc_sync_type;

typedef enum {
    SC_INV = 0,
    SC_CLEAN = 2,
    SC_CLEAN_INV = 4,
} sc_ops_type;

typedef enum {
    CMO_BY_WAY = 0,
    CMO_BY_GID,
    CMO_BY_64PA,
    CMO_BY_128PA,
    CMO_BY_2KPA,
    CMO_BY_WAY_AND_GID,
} sc_cmo_type;

enum sc_gid_priority {
    SC_GID_PRIORITY0 = 0,
    SC_GID_PRIORITY1 = 1,
    SC_GID_PRIORITY2 = 2,
    SC_GID_PRIORITY3 = 3
};

typedef union {
    unsigned long long value;
    union {
        struct {
            unsigned long long opt : 4;
            unsigned long long cmdt : 4;
            unsigned long long way_bitmap : 16;
            unsigned long long res0 : 20;
            unsigned long long res1 : 20;
        } by_way;

        struct {
            unsigned long long opt : 4;
            unsigned long long cmdt : 4;
            unsigned long long gid_bitmap : 16;
            unsigned long long res0 : 20;
            unsigned long long res1 : 20;
        } by_gid;

        struct {
            unsigned long long opt : 4;
            unsigned long long cmdt : 4;
            unsigned long long res0 : 2;
            unsigned long long phy_addr : 30;
            unsigned long long res1 : 24;
        } by_64pa;

        struct {
            unsigned long long opt : 4;
            unsigned long long cmdt : 4;
            unsigned long long res0 : 3;
            unsigned long long phy_addr : 29;
            unsigned long long res1 : 23;
        } by_128pa;
        struct {
            unsigned long long opt : 4;
            unsigned long long cmdt : 4;
            unsigned long long start_addr : 24;
            unsigned long long end_addr : 24;
            unsigned long long res : 9;
        } by_2kpa;
        struct {
            unsigned long long opt : 4;
            unsigned long long cmdt : 4;
            unsigned long long gid_bitmap : 16;
            unsigned long long way_bitmap : 16;
            unsigned long long res : 24;
        } by_way_and_gid;

        struct {
            unsigned long long opt : 4;
            unsigned long long cmdt : 4;
            unsigned long long rslt : 4;
            unsigned long long thread_id : 16;
            unsigned long long seq : 10;
            unsigned long long res : 26;
        } sync;
    } param;
} sc_cmo_cmd;

struct sc_cmo_cfg {
    sc_sync_type sync_type;
    sc_ops_type ops;
    sc_cmo_type cmo_type;
    unsigned int gid_bitmap;
    unsigned int way_bitmap;
    unsigned long long phy_addr;
    size_t size;
};
#define CMO_OPERATE_MASK  0xf
#define CMO_BITMAP_MASK   0xffff
#define CMO_64B_PA_SHIFT  6
#define CMO_128B_PA_SHIFT 7
#define CMO_2K_PA_SHIFT   11
#define CMO_4K_PA_SHIFT   12
#define CMO_FINISH_MASK 0x1
#define CMO_EVENT_INT_CLR_FLAG      BIT(19)
int sc_hal_alloc_quota(struct sc *sc_ctx, int gid, unsigned int quota);
unsigned int sc_hal_get_cache_size(struct sc *sc_ctx, unsigned int gid);
void sc_hal_cfg_cmo(struct sc *sc_ctx, unsigned long long value);
void sc_hal_clear_cmo_intr(struct sc *sc_ctx, unsigned int idx, unsigned int value);
unsigned int sc_hal_get_cmo_stat(struct sc *sc_ctx, unsigned int idx);
unsigned int sc_get_cache_size(unsigned int gid);
int sc_ops_cache(sc_sync_type sync_type, sc_ops_type ops, sc_cmo_type cmo_type,
    unsigned int gid_bitmap, unsigned int way_bitmap, unsigned long long phy_addr, int size);
int sc_cmo_sync_cmd_by_event(struct sc *sc_ctx);
void sc_hal_irq_enable_fc(struct sc *sc_ctx);
void sc_hal_irq_enable_dfx(struct sc *sc_ctx);
void sc_hal_irq_disable_fc(struct sc *sc_ctx);
void sc_hal_irq_disable_dfx(struct sc *sc_ctx);
unsigned int sc_hal_get_dfx_cmo_irq(struct sc *sc_ctx);
void sc_hal_clr_dfx_cmo_irq(struct sc *sc_ctx);
unsigned int sc_hal_get_dfx_irq(struct sc *sc_ctx, unsigned int slice);
void sc_hal_clr_dfx_irq(struct sc *sc_ctx, unsigned int slice);
void sc_hal_set_bypass(struct sc *sc_ctx, int gid, unsigned int bypass);
unsigned int sc_hal_get_bypass(struct sc *sc_ctx, int slice);
unsigned long long sc_hal_get_flow(struct sc *sc_ctx, unsigned int dir);
unsigned int sc_hal_get_ecc_err(struct sc *sc_ctx, unsigned int slice);
void sc_dump_reg_hook(struct sc *sc_ctx);
void sc_cmo_by_gid(unsigned int gid);
void sc_show_om(void);
void sc_set_bypass(unsigned int gid, unsigned int bypass);
unsigned int sc_show_quota(unsigned int gid);
void sc_set_dbg(unsigned int bit);

#ifdef CONFIG_SYSCACHE
int sc_init(struct platform_device *pdev);
void sc_dump_hook(void);
#else
static inline int sc_init(struct platform_device *pdev)
{
    return 0;
}

static inline void sc_dump_hook(void)
{
    return;
}
#endif

#endif
