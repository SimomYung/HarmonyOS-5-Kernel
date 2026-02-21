/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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

#include <mdrv_memory_layout.h>
#include "bsp_rfile.h"
#include "bsp_sysctrl.h"
#include "bsp_version.h"
#include "pfa.h"
#include "pfa_dbg.h"
#include "pfa_img.h"

#define PFA_IMG_SECT_NUM_MAX 10000

void *get_img_from_ddr(void)
{
    unsigned long phy_addr;
    unsigned int size;
    struct pfa *pfa = &g_pfa;

    phy_addr = mdrv_mem_region_get("pfa_ddr", &size);
    if (phy_addr == 0 || size == 0) {
        PFA_ERR("img addr get fail\n");
        return NULL;
    }
    if (pfa->plat_type == PLAT_EMU) {
        pfa->core_mem_info.img_addr_vir = phys_to_virt(phy_addr);
    } else {
        pfa->core_mem_info.img_addr_vir = ioremap(phy_addr, size);
    }
    if (pfa->core_mem_info.img_addr_vir == NULL) {
        PFA_ERR("img addr remap fail\n");
        return NULL;
    }
    pfa->core_mem_info.img_size = size;
    pfa->core_mem_info.img_addr = (void*)(uintptr_t)phy_addr;
    PFA_ERR("img addr phy:0x%llx, vir:0x%llx\n", phy_addr, pfa->core_mem_info.img_addr_vir);
    return (void *)(uintptr_t)pfa->core_mem_info.img_addr_vir; // emu后台加载 size=0x20000
}

struct pfa_core_memory *img_analyze(void *img_addr, u32 *memory_num)
{
    struct pfa_img_header *header = (struct pfa_img_header *)img_addr;
    struct pfa_img_header_section *sections = img_addr + PFA_SEG_HDR_OFFSET;
    unsigned int sect_num = header->sect_num;
    struct pfa_core_memory *memorys = NULL;
    unsigned int i;
    struct pfa *pfa = &g_pfa;

    if (sect_num > PFA_IMG_SECT_NUM_MAX || sect_num == 0) {
        PFA_ERR("sect_num:%d err\n", sect_num);
        return NULL;
    }

    memorys = (struct pfa_core_memory*)kzalloc(sizeof(*memorys) * sect_num, GFP_KERNEL);
    if (memorys == NULL) {
        PFA_ERR("alloc core mem fail\n");
        return NULL;
    }
    pfa->core_mem_info.memorys = memorys;
    pfa->core_mem_info.memory_num = sect_num;

    for (i = 0; i < sect_num; i++) {
        memorys[i].mem_type = sections[i].mem_type;
        memorys[i].load_type = sections[i].load_type;
        memorys[i].img_addr = img_addr + header->data_offset + sections[i].sect_data_offset;
        memorys[i].tcm_addr = sections[i].dest_addr + PFA_DM_BASE;
        memorys[i].size = sections[i].size;
        if (memorys[i].tcm_addr > PFA_IMG_BASE_END || memorys[i].size > PFA_IMG_BASE_END ||
            memorys[i].tcm_addr + memorys[i].size > PFA_IMG_BASE_END) {
            PFA_ERR("size:0x%x, tcm_addr:0x%x err\n", memorys[i].size, memorys[i].tcm_addr);
            return NULL;
        }
        if (header->data_offset + sections[i].sect_data_offset > pfa->core_mem_info.img_size) {
            PFA_ERR("size:0x%x, head offset:0x%x, section offset:0x%x err\n",
            pfa->core_mem_info.img_size, header->data_offset, sections[i].sect_data_offset);
            return NULL;
        }
    }
    *memory_num = sect_num;
    return memorys;
}

void tcm_sw_load(unsigned int dest, void *src, unsigned int size) // 考虑用eicc
{
    unsigned int offset;
    unsigned int val;
    struct pfa *pfa = &g_pfa;

    for (offset = 0; offset < size;) {
        val = readl(src + offset);
        pfa_writel(pfa->regs, dest + offset, val);
        offset = offset + sizeof(unsigned int);
    }
}

void img_load_to_core(struct pfa_core_memory *memorys, unsigned int memory_num)
{
    unsigned int i;

    for (i = 0; i < memory_num; i++) {
        if (LOAD_TYPE_POWERON_NEED_LOAD(memorys[i].load_type)) {
            tcm_sw_load(memorys[i].tcm_addr, memorys[i].img_addr, memorys[i].size);
        }
    }
}

int pfa_load_img(struct pfa *pfa, char *data, unsigned int size)
{
    unsigned int memory_num;
    struct pfa_core_memory *memorys = NULL;

    if ((pfa->plat_type == PLAT_ESL && pfa->plat_info == PLAT_INFO_HYBRID_VDK_ZEBU) ||
        (pfa->plat_type == PLAT_EMU)) {
        get_img_from_ddr();
    } else {
        pfa->core_mem_info.img_size = size;
        pfa->core_mem_info.img_addr_vir = (void*)data;
    }
    if (pfa->core_mem_info.img_addr_vir == NULL) {
        PFA_ERR("get img fail \n");
        return -1;
    }

    memorys = img_analyze(pfa->core_mem_info.img_addr_vir, &memory_num);
    img_load_to_core(memorys, memory_num);

    return 0;
}

void pfa_set_core_halt(struct pfa *pfa, unsigned int en)
{
    pfa_writel(pfa->regs, PFA_DM_CFG_MAC_FW_REG, PFA_MAC_FW_DM_AGING_TIMEOUT | (PFA_5GLAN_PORT << 16));
    pfa_writel(pfa->regs, PFA_CORE_HALT, en);
}

bool pfa_core_boot_done(struct pfa *pfa)
{
    unsigned int val =  pfa_readl(pfa->regs, PFA_SOFT_INT_RAW);
    return val & 0x1;
}

int pfa_enable_dm_power(struct pfa *pfa)
{
    int i = 0;
    int cnt = 10; // 10us
    pfa_poff_stat_t stat;

    if (pfa->aoctrl.dm_power_by_actrl) {
        pfa->aoctrl.sysctrl_ao_addr = bsp_sysctrl_addr_byindex(ACTRL);
    } else {
        pfa->aoctrl.sysctrl_ao_addr = bsp_sysctrl_addr_byindex(SYSCTRL_AO);
    }
    if (pfa->aoctrl.sysctrl_ao_addr == NULL) {
        PFA_ERR("no get sysctrl ao base\n");
        return -1;
    }
    pfa->aoctrl.poff_ctrl_offset = PFA_POFF_CTRL_OFFSET;
    pfa->aoctrl.poff_stat_offset = PFA_POFF_STAT_OFFSET;
    pfa->aoctrl.sd_value = PFA_POFF_CTRL_SD_VALUE;
    pfa->aoctrl.ds_value = PFA_POFF_CTRL_DS_VALUE;
    writel(PFA_POFF_CTRL_SD_VALUE, pfa->aoctrl.sysctrl_ao_addr + pfa->aoctrl.poff_ctrl_offset);
    writel(PFA_POFF_CTRL_DS_VALUE, pfa->aoctrl.sysctrl_ao_addr + pfa->aoctrl.poff_ctrl_offset);
    while (i < cnt) {
        if (pfa->aoctrl.dm_power_by_actrl) {
            stat.u32 = readl(bsp_sysctrl_addr_byindex(SYSCTRL_AP_PERI) + PFA_POFF_STAT_OFFSET);
        } else {
            stat.u32 = readl(pfa->aoctrl.sysctrl_ao_addr + PFA_POFF_STAT_OFFSET);
        }
        if (stat.bits.pfa_poff_stat == 0) {
            break;
        }
        i++;
        udelay(1);
        if (i == cnt) {
            PFA_ERR("no check pfa poff stat\n");
            return -1;
        }
    }
    udelay(1);

    return 0;
}

int pfa_start_core(char *data, unsigned int size)
{
    unsigned int cnt = 1000; // 1000us
    const bsp_version_info_s *ver;
    struct pfa *pfa = &g_pfa;

    ver = bsp_get_version_info();
    if (ver == NULL) {
        PFA_ERR("pfa get version fail\n");
        return -1;
    }
    if (ver->plat_type == PLAT_ESL && pfa->plat_info != PLAT_INFO_HYBRID_VDK_ZEBU) {
        return 0;
    }

    if (pfa_enable_dm_power(pfa) != 0) {
        pfa->init_stat.enable_dm_fail++;
        return -1;
    }

    if (pfa_load_img(pfa, data, size) != 0) {
        pfa->init_stat.load_img_fail++;
        return -1;
    }
    pfa_set_core_halt(pfa, 0);
    while (cnt > 0) {
        if (pfa_core_boot_done(pfa)) {
            return 0;
        }
        udelay(1);
        cnt--;
    }
    pfa->init_stat.core_run_timeout++;
    PFA_ERR("core run timeout\n");
    return -1;
}