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

#include <linux/kernel.h>
#include <securec.h>
#include "pfa_mac_entry.h"
#include "pfa_dbg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifdef CONFIG_PFA_FW

void tab_clr(unsigned int is_write)
{
    struct pfa *pfa = &g_pfa;
    pfa_tab_clt_t tab_clr;

    tab_clr.u32 = pfa_readl(pfa->regs, PFA_TAB_CLR);
    if (is_write) {
        tab_clr.bits.pfa_tab_clr |= BIT(1);
    } else {
        tab_clr.bits.pfa_tab_clr &= ~(BIT(1));
    }
    pfa_writel(pfa->regs, PFA_TAB_CLR, tab_clr.u32);
}

int tab_act_result(void)
{
    u32 timeout = 500;
    struct pfa *pfa = &g_pfa;
    pfa_tab_act_result_t act_result;

    do {
        act_result.u32 = pfa_readl(pfa->regs, PFA_TAB_ACT_RESULT);
        if (act_result.bits.pfa_tab_done) {
            /* pfa_tab_done bit is w1c,so we must clear it */
            pfa_writel(pfa->regs, PFA_TAB_ACT_RESULT, act_result.u32);
            return act_result.bits.pfa_tab_success ? 0 : -EBUSY;
        }

        timeout--;
        if (!timeout) {
            PFA_ERR("pfa mac entry add/delete timeout!\n");
            return -ETIMEDOUT;
        }

        udelay(1);
    } while (1);

    return 0;
}

int pfa_mac_entry_config(enum pfa_tab_ctrl_type type, const unsigned int *buf, unsigned int size)
{
    unsigned int i;
    struct pfa *pfa = &g_pfa;
    struct pfa_mac_fw_entry *ent = (struct pfa_mac_fw_entry*)buf;

    for (i = 0; i < size; i++) {
        pfa_writel(pfa->regs, PFA_TAB_CONTENT(i), buf[i]);
    }
    pfa_writel(pfa->regs, PFA_TAB_CTRL, type);

    if (type == PFA_TAB_CTRL_DEL_MAC_FW) {
        pfa->hal->del_dm_mac_entry(pfa, ent);
    }
    if (type == PFA_TAB_CTRL_ADD_MAC_FW) {
        pfa->hal->add_dm_mac_entry(pfa, ent);
    }

    return tab_act_result();
}

void pfa_macfw_entry_clear(void)
{
    struct pfa *pfa = &g_pfa;

    if (pfa->macfw.slab) {
        kmem_cache_destroy(pfa->macfw.slab);
        pfa->macfw.slab = NULL;
    }
}

void pfa_print_macfw_backup(void)
{
    struct pfa_mac_fw_entry_ext *pos = NULL;
    struct pfa_mac_fw_entry_ext *n = NULL;
    struct pfa *pfa = &g_pfa;
    list_for_each_entry_safe(pos, n, &pfa->macfw.backups, list)
    {
        PFA_ERR("MAC: %x%x\n", pos->ent.mac_hi, pos->ent.mac_lo);
    }
}

int pfa_mac_filt_add(struct pfa_mac_filt_entry *ent)
{
    if (ent == NULL) {
        return -EINVAL;
    }
    return pfa_mac_entry_config(PFA_TAB_CTRL_ADD_MAC_FILT, (unsigned int *)ent, sizeof(*ent) / sizeof(unsigned int));
}

int pfa_mac_filt_del(struct pfa_mac_filt_entry *ent)
{
    if (ent == NULL) {
        return -EINVAL;
    }
    return pfa_mac_entry_config(PFA_TAB_CTRL_DEL_MAC_FILT, (unsigned int *)ent, sizeof(*ent) / sizeof(unsigned int));
}

static void pfa_mac_list_del(struct pfa *pfa, struct pfa_mac_fw_entry_ext *ent_ext, struct list_head *d_list)
{
    struct pfa_mac_fw_entry_ext *pos = NULL;
    struct pfa_mac_fw_entry_ext *n = NULL;

    list_for_each_entry_safe(pos, n, d_list, list)
    {
        if (ent_ext->ent.mac_hi == pos->ent.mac_hi && ent_ext->ent.mac_lo == pos->ent.mac_lo) {
            list_del_init((struct list_head *)&pos->list);
            kmem_cache_free(pfa->macfw.slab, (void *)pos);
        }
    }
}

/* add an entry to PFA HW */
static int pfa_mac_entry_add_sync(struct pfa *pfa, struct pfa_mac_fw_entry_ext *new_entry)
{
    int ret = 0;

    if (!pfa->mask_flags) {
        /* to avoid duplication, delete first */
        (void)pfa_mac_entry_config(PFA_TAB_CTRL_DEL_MAC_FW, (unsigned int *)&new_entry->ent,
            sizeof(new_entry->ent) / sizeof(unsigned int));

        /* if no duplication, add it */
        ret = pfa_mac_entry_config(PFA_TAB_CTRL_ADD_MAC_FW, (unsigned int *)&new_entry->ent,
            sizeof(new_entry->ent) / sizeof(unsigned int));
    }

    if (!ret) {
        pfa_mac_list_del(pfa, new_entry, &pfa->macfw.backups);
        list_add(&new_entry->list, &pfa->macfw.backups);
    }

    return ret;
}

static int pfa_mac_entry_del_sync(struct pfa *pfa, struct pfa_mac_fw_entry_ext *ent_ext)
{
    int ret;

    ret = pfa_mac_entry_config(PFA_TAB_CTRL_DEL_MAC_FW, (unsigned int *)&ent_ext->ent,
        sizeof(ent_ext->ent) / sizeof(unsigned int));
    pfa_mac_list_del(pfa, ent_ext, &pfa->macfw.backups);

    return ret;
}

int __pfa_mac_fw_add(const unsigned char *mac, unsigned short vid, unsigned int br_pfano, unsigned int src_pfano,
    unsigned int is_static, unsigned int vlan_valid)
{
    struct pfa *pfa = &g_pfa;
    struct pfa_mac_fw_entry_ext *ent_ext = NULL;
    unsigned long flags;
    int ret;

    if (mac == NULL) {
        return -EINVAL;
    }

    ent_ext = (struct pfa_mac_fw_entry_ext *)kmem_cache_alloc(pfa->macfw.slab, GFP_ATOMIC);
    if (ent_ext == NULL) {
        return -ENOMEM;
    }

    ret = memset_s((void *)ent_ext, sizeof(*ent_ext), 0, sizeof(struct pfa_mac_fw_entry_ext));
    if (unlikely(ret)) {
        WARN_ON_ONCE(1);
    }

    /* mac field in big endian */
    ent_ext->ent.mac_hi = (mac[0]) | (mac[1] << 8) | (mac[2] << 16) | (mac[3] << 24); /* 2,3,8,16,24 mac addr shift */
    ent_ext->ent.mac_lo = (mac[4]) | (mac[5] << 8); /* 4,5,8 mac addr shift */
    ent_ext->ent.port_br_id = br_pfano;
    ent_ext->ent.port_no = src_pfano;
    ent_ext->ent.vid = vid;
    ent_ext->ent.is_static = is_static;
    ent_ext->ent.vlan_valid = vlan_valid;

    spin_lock_irqsave(&pfa->macfw.lock, flags);

    ret = pfa_mac_entry_add_sync(pfa, ent_ext);
    if (ret) {
        /* if entry op fail, add it to pending queue */
        pfa_mac_list_del(pfa, ent_ext, &pfa->macfw.pending);
        kmem_cache_free(pfa->macfw.slab, (void *)ent_ext);
    }

    spin_unlock_irqrestore(&pfa->macfw.lock, flags);

    return ret;
}

void pfa_mac_fw_add(const unsigned char *mac, unsigned short vid, unsigned int br_pfano, unsigned int src_pfano,
    unsigned int is_static, unsigned int vlan_valid)
{
    int ret;
    struct pfa *pfa = &g_pfa;

    if (pfa->ops.pfa_macfw_add_filter && pfa->ops.pfa_macfw_add_filter(mac)) {
        pfa->macfw.macfw_add_filter++;
        return;
    }

    ret = __pfa_mac_fw_add(mac, vid, br_pfano, src_pfano, is_static, vlan_valid);
    if (ret) {
        pfa->macfw.macfw_add_fail++;
    }
}

// interface
int __pfa_mac_fw_del(const unsigned char *mac, unsigned short vid, unsigned int br_pfano, unsigned int src_pfano,
    unsigned int vlan_valid)
{
    struct pfa *pfa = &g_pfa;
    struct pfa_mac_fw_entry_ext ent_ext = {{0}};
    struct pfa_mac_fw_entry_ext *pos = NULL;
    struct pfa_mac_fw_entry_ext *n = NULL;
    int ret;
    unsigned long flags;

    if (mac == NULL) {
        return -EINVAL;
    }

    /* mac field in big endian */
    ent_ext.ent.mac_hi = (mac[0]) | (mac[1] << 8) | (mac[2] << 16) | (mac[3] << 24); /* 2,3,8,16,24 mac addr shift */
    ent_ext.ent.mac_lo = (mac[4]) | (mac[5] << 8); /* 4,5,8 mac addr shift */
    ent_ext.ent.port_br_id = br_pfano & 0xf;
    ent_ext.ent.port_no = src_pfano & 0xf;
    ent_ext.ent.vid = vid & 0xfff;
    ent_ext.ent.vlan_valid = vlan_valid;

    spin_lock_irqsave(&pfa->macfw.lock, flags);

    /* delete the entry both in HW and pending queue */
    ret = pfa_mac_entry_del_sync(pfa, &ent_ext);
    if (!ret) {
        list_for_each_entry_safe(pos, n, &pfa->macfw.pending, list)
        {
            list_del_init((struct list_head *)&pos->list);
            if (!pfa_mac_entry_add_sync(pfa, pos)) {
                break;
            } else {
                list_add(&pos->list, &pfa->macfw.pending);
            }
        }
    } else {
        pfa_mac_list_del(pfa, &ent_ext, &pfa->macfw.pending);
    }

    spin_unlock_irqrestore(&pfa->macfw.lock, flags);
    return ret;
}

void pfa_mac_fw_del(const unsigned char *mac, unsigned short vid, unsigned int br_pfano,
    unsigned int src_pfano, unsigned int vlan_valid)
{
    int ret;
    struct pfa *pfa = &g_pfa;

    ret = __pfa_mac_fw_del(mac, vid, br_pfano, src_pfano, vlan_valid);
    if (ret) {
        pfa->macfw.macfw_del_fail++;
    }
}

void pfa_macfw_entry_dump(void)
{
    struct pfa *pfa = &g_pfa;

    pfa->hal->dbgen_en();

    pfa->hal->show_mac_entry(pfa, IS_MAC_FW_ENTRY);
    pfa->hal->dbgen_dis();
}

void pfa_macfl_entry_dump(void)
{
    struct pfa *pfa = &g_pfa;

    pfa->hal->dbgen_en();

    pfa->hal->show_mac_entry(pfa, IS_MAC_FL_ENTRY);

    pfa->hal->dbgen_dis();
}

int pfa_qos_mac_lmt_add(struct pfa_qos_mac_entry *ent)
{
    if (ent == NULL) {
        return -EINVAL;
    }
    PFA_ERR("ent->mac_hi:0x%x, ent->mac_low:0x%x \n", ent->mac_hi, ent->mac_lo);
    PFA_ERR("ent->limit_byte_lo:%d, ent->limit_byte_hi:%d, ent->limit_en:%d \n", ent->limit_byte_lo, ent->limit_byte_hi,
            ent->limit_en);
    return pfa_mac_entry_config(PFA_TAB_CTRL_ADD_QOS_MAC_LMT, (unsigned int *)ent,
        sizeof(struct pfa_qos_mac_entry) / sizeof(unsigned int));
}

int pfa_qos_mac_lmt_del(struct pfa_qos_mac_entry *ent)
{
    if (ent == NULL) {
        return -EINVAL;
    }
    return pfa_mac_entry_config(PFA_TAB_CTRL_DEL_QOS_MAC_LMT, (unsigned int *)ent,
        sizeof(struct pfa_qos_mac_entry) / sizeof(unsigned int));
}
void pfa_qos_mac_entry_dump(void)
{
    unsigned int pfa_tab_entry[PFA_QOS_MAC_TAB_WORD_NO] = {0};
    struct pfa *pfa = &g_pfa;
    unsigned int i, j;
    unsigned long long lmt;
    unsigned long long mac_addr;

    pfa->hal->dbgen_en();

    for (i = 0; i < PFA_QOS_MAC_TAB_NUMBER; i++) {
        for (j = 0; j < PFA_QOS_MAC_TAB_WORD_NO; j++) {
            pfa_tab_entry[j] = pfa_readl(pfa->regs, PFA_QOS_MAC_TAB(i * PFA_QOS_MAC_TAB_WORD_NO + j));
            if (pfa_tab_entry[j] != 0) {
                PFA_ERR("[qos mac entry (%d)] pfa_tab_entry[%d] :0x%x \n", i, j, pfa_tab_entry[j]);
            }
        }

        if (pfa_tab_entry[0] == 0 && pfa_tab_entry[1] == 0 && pfa_tab_entry[2] == 0) { // 2 is qos entry
            continue;
        }

        lmt = ((((unsigned long long)pfa_tab_entry[1]) & 0x3ff) << 30) | (pfa_tab_entry[0] >> 2); /* 2,30 bit shift */
        mac_addr = ((((unsigned long long)pfa_tab_entry[2]) & 0x3ffffff) << 22) | (pfa_tab_entry[1] >> 10); /* 22,10 */

        PFA_ERR("mac_qos_tab_limit_en :%d \n", pfa_tab_entry[0] & 3); /* 3, bit calc */
        PFA_ERR("mac_qos_tab_limit_byte :%llx \n", lmt);
        PFA_ERR("mac_addr :%llx \n", mac_addr);
        PFA_ERR("mac_addr_vid :%d \n", pfa_tab_entry[2] >> 26); /* 26 bit shift */
    }

    pfa->hal->dbgen_dis();
}
#endif

int pfa_mac_entry_table_init(struct pfa *pfa)
{
    /* alloc pool for mac fw entry */
    pfa->macfw.slab = (struct kmem_cache *)kmem_cache_create(dev_name(pfa->dev), sizeof(struct pfa_mac_fw_entry_ext),
                                                             0, SLAB_HWCACHE_ALIGN, NULL);

    if (pfa->macfw.slab == NULL) {
        PFA_ERR("alloc mac fw slab failed\n");
        return -ENOMEM;
    }

    INIT_LIST_HEAD(&pfa->macfw.pending);
    INIT_LIST_HEAD(&pfa->macfw.backups);
    spin_lock_init(&pfa->macfw.lock);
    PFA_INFO("[init] pfa mac entry table init success\n");
    return 0;
}

void pfa_mac_entry_table_exit(struct pfa *pfa)
{
    unsigned long flags;
    struct pfa_mac_fw_entry_ext *pos = NULL;
    struct pfa_mac_fw_entry_ext *n = NULL;

    /* free node in pending queue */
    spin_lock_irqsave(&pfa->macfw.lock, flags);
    list_for_each_entry_safe(pos, n, &pfa->macfw.pending, list)
    {
        list_del_init(&pos->list);
        kmem_cache_free(pfa->macfw.slab, (void *)pos);
    }

    list_for_each_entry_safe(pos, n, &pfa->macfw.backups, list)
    {
        list_del_init(&pos->list);
        kmem_cache_free(pfa->macfw.slab, (void *)pos);
    }
    spin_unlock_irqrestore(&pfa->macfw.lock, flags);

    if (pfa->macfw.slab != NULL) {
        kmem_cache_destroy(pfa->macfw.slab);
        pfa->macfw.slab = NULL;
    }
}

void pfa_config_eth_vlan_tag_sel(unsigned int flag)
{
    struct pfa *pfa = &g_pfa;

    pfa->hal->config_eth_vlan_tag_sel(pfa, flag);
}

bool mdrv_pfa_macfw_find_mac(const unsigned char *mac)
{
    struct pfa *pfa = &g_pfa;
    struct pfa_mac_fw_entry_ext *pos = NULL;
    struct pfa_mac_fw_entry_ext *n = NULL;
    unsigned int mac_hi, mac_lo;

    if (mac == NULL) {
        return false;
    }
    mac_hi = (mac[0]) | (mac[1] << 8) | (mac[2] << 16) | (mac[3] << 24); /* 2,3,8,16,24 mac addr shift */
    mac_lo = (mac[4]) | (mac[5] << 8); /* 4,5,8 mac addr shift */

    list_for_each_entry_safe(pos, n, &pfa->macfw.backups, list)
    {
        if (pos->ent.mac_hi == mac_hi && pos->ent.mac_lo == mac_lo) {
            return true;
        }
    }

    return false;
}

MODULE_LICENSE("GPL");

#ifdef CONFIG_PFA_FW
EXPORT_SYMBOL(tab_clr);
EXPORT_SYMBOL(pfa_print_macfw_backup);
EXPORT_SYMBOL(pfa_macfw_entry_clear);
EXPORT_SYMBOL(pfa_mac_entry_config);
EXPORT_SYMBOL(pfa_qos_mac_lmt_add);
EXPORT_SYMBOL(pfa_qos_mac_lmt_del);
EXPORT_SYMBOL(mdrv_pfa_macfw_find_mac);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
