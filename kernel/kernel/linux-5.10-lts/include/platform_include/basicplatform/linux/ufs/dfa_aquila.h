/*
 *
 * dfa aquaila platform header file
 *
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _DFA_AQUILA_H
#define _DFA_AQUILA_H

/* dfa sys ctrl */
#define acore_dfa_int_dfx(base)           ((base) + 0x0)
#define acore_dfa_int_mask(base)          ((base) + 0x4)
#define acore_dfa_int_set(base)           ((base) + 0x8)
#define acore_dfa_int_status(base)        ((base) + 0xC)
#define acore_dfa_req(base)               ((base) + 0x10)
#define acore_cfg_clk_en(base)            ((base) + 0x14)
#define acore_cfg_rst_req(base)           ((base) + 0x18)
#define dfa_cpu_halt(base)                ((base) + 0x24)
#define dfa_mem_ctrl_sp(base)             ((base) + 0x28)
#define dfa_acore_int_en(base)            ((base) + 0x40)
#define acore_int_status(base)            ((base) + 0x44)
#define dfa_qic_pd(base)                  ((base) + 0x64)
#define dfa_qic_pd_ack(base)              ((base) + 0x68)
#define acore_dfa_enabled(base)           ((base) + 0x100)
#define acore_dfa_commu_info2(base, bank) ((base) + 0x100 + 0x4 * (bank))
#define dfa_acore_commu_info2(base, bank) ((base) + 0x200 + 0x4 * (bank))

/* dfa que proc */
#define que_proc_msi_device_id(base)      (base)
#define que_proc_que_cr(base)             ((base) + 0x004UL)
#define que_proc_esilba(base)             ((base) + 0x010UL)
#define que_proc_esiuba(base)             ((base) + 0x014UL)
#define que_proc_ram_ctrl(base)           ((base) + 0x018UL)
#define que_proc_ram_status(base)         ((base) + 0x01CUL)
#define que_proc_sq_i_attr0(base, i)      ((base) + 0x020UL + (i) * 0x1CUL)
#define que_proc_sq_i_sqlba0(base, i)     ((base) + 0x024UL + (i) * 0x1CUL)
#define que_proc_sq_i_squba0(base, i)     ((base) + 0x028UL + (i) * 0x1CUL)
#define que_proc_cq_i_attr0(base, i)      ((base) + 0x030UL + (i) * 0x1CUL)
#define que_proc_cq_i_cqlba0(base, s)     ((base) + 0x034UL + (s) * 0x1CUL)
#define que_proc_cq_i_cquba0(base, s)     ((base) + 0x038UL + (s) * 0x1CUL)
#define que_proc_sq_i_sqh(base, i)        ((base) + 0x8E0UL + (i) * 0x8UL)
#define que_proc_sq_i_sqt(base, i)        ((base) + 0x8E4UL + (i) * 0x8UL)
#define que_proc_sq_i_sqis(base, i)       ((base) + 0xB60UL + (i) * 0x8UL)
#define que_proc_cq_i_cqh(base, i)        ((base) + 0xDE0UL + (i) * 0x8UL)
#define que_proc_cq_i_cqt(base, i)        ((base) + 0xDE4UL + (i) * 0x8UL)
#define que_proc_cq_i_cqis(base, i)       ((base) + 0x1060UL + (i) * 0xCUL)
#define que_proc_cq_i_cqie(base, i)       ((base) + 0x1064UL + (i) * 0xCUL)
#define que_proc_dfa_sq_i_rtc(base, j)    ((base) + 0x1420UL + (j) * 0x14UL)
#define que_proc_dfa_sq_i_rts(base, j)    ((base) + 0x1424UL + (j) * 0x14UL)
#define que_proc_dfa_sq_i_sqis(base, j)   ((base) + 0x1428UL + (j) * 0x14UL)
#define que_proc_dfa_sq_cfig(base, j)     ((base) + 0x1430UL + (j) * 0x14UL)

/* soc actrl */
#define actrl_iso_en_group0_peri(base)        (base)
#define actrl_ao_mem_ctrl2(base)              ((base) + 0x814UL)
#define actrl_hsdt_mem_status(base)           ((base) + 0x9D0UL)

/* sctrl */
#define sctrl_scefusedata2(base)              ((base) + (0x00CUL))
#define sctrl_scefusedata8(base)              ((base) + (0x060UL))
#define sctrl_scbakdata1_msk(base)            ((base) + (0x4C4UL))

/* peri crg */
#define crgperiph_peren0(base)                ((base) + (0x000UL))
#define crgperiph_perstat0(base)              ((base) + (0x00CUL))
#define crgperiph_perrsten0(base)             ((base) + (0x060UL))
#define crgperiph_perrstdis0(base)            ((base) + (0x064UL))
#define crgperiph_autodiv_inuse_stat1(base)   ((base) + (0x404UL))
#define crgperiph_clkdiv26(base)              ((base) + (0x700UL))
#define crgperiph_pll_fsm_ns_vote0(base)      ((base) + (0x950UL))

/* ao crg */
#define ao_crg_scperen6(base)                 ((base) + 0x1D0UL)
#define ao_crg_scperdis6(base)                ((base) + 0x1D4UL)
#define ao_crg_scperrsten1(base)              ((base) + 0x20CUL)
#define ao_crg_scperrstdis1(base)             ((base) + 0x210UL)
#define ao_crg_scclkdiv5(base)                ((base) + 0x264UL)

/* hsdt0 ctrl */
#define hsdt0_xctrl_qic_rd_nopending(base)    ((base) + 0x018UL)
#define hsdt0_xctrl_qic_wr_nopending(base)    ((base) + 0x01CUL)
#define hsdt0_xctrl_qic_syspdc_pwrd_req(base) ((base) + 0x424UL)
#define hsdt0_xctrl_qic_syspdc_pwrd_ack(base) ((base) + 0x428UL)

/* hsdt0 crg */
#define hsdt0_crg_perrsten0(base)             ((base) + 0x060UL)
#define hsdt0_crg_perrstdis0(base)            ((base) + 0x064UL)

/* ufs sysctrl */
#define ufs_crg_rst_cfg0(base)                ((base) + (0x008UL))

/* hsdt0 pctrl */
#define hsdt0_xptrl_qic_error(base)           ((base) + 0x010EC)
#endif
