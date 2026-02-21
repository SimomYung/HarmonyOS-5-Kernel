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
#include "fipc_platform.h"
#include "fipc_dts.h"
#include "fipc_device.h"
#include "fipc_driver.h"
#include "fipc_core.h"
#include "fipc_pmsr.h"
#include "fipc_dump.h"
#include "fipc_reset.h"

#if FIPC_FEATURE_DUMP_SUPPORT
#define FIPC_DUMP_MAGIC 0x0000F1CC
#define FIPC_DUMP_VERSION 0x2
struct fipc_dump_head {
    u32 dump_start_time;
    u32 version;
    u32 dump_end_time;
    u32 len;
};

#define FIPC_DUMP_PMSR 0x0001
#define FIPC_DUMP_PMSR_VER 0x0002
#define FIPC_DUMP_HWREG 0x0002
#define FIPC_DUMP_HWREG_VER 0x0002
#define FIPC_DUMP_CHNS 0x0003
#define FIPC_DUMP_CHNS_VER 0x0001
#define FIPC_DUMP_HIFI_DBG 0x0004
#define FIPC_DUMP_HIFI_DBG_VER 0x0001

struct fipc_dump_tlv {
    u32 modid; /* this is type[tlv:t] */
    u32 version;
    u32 reserved;
    u32 len; /* this is len[tlv:l] */
};           /* [tlv:v] is after this struct */

struct fipc_dump_chnsinfo {
    u32 ldrvchn_id;
    u32 type;
    u32 wptr; /* 对于ipipe，这个dump vptr指向的位置 */
    u32 rptr; /* 对于opipe，这个dump vptr指向的位置 */
    u32 dym_flags;
    u32 const_flags;
};

struct fipc_ringbuf_dump_head {
    u32 head_tag;
    u32 chnl_idx;
    u32 chnl_type;
    u32 ring_buf_len;
};

struct fipc_ringbuf_dump_addr {
    void *ringbuf_va;
    fipcsoc_ptr_t ringbuf_pa;
};

struct fipc_dump_devinfo {
    u32 devid;
    u32 cpu_cnt;
    u32 pipepair_cnt;
};

struct fipc_dump_cpuregsinfo {
    u32 magic;
    u32 cpuid;
    u32 omask0;
    u32 omask1;
    u32 imask0;
    u32 imask1;
    u32 ostat0;
    u32 ostat1;
    u32 istat0;
    u32 istat1;
};

static u32 fipc_device_dump(struct fipc_device *pdev, u8 *buf, u32 len)
{
    struct fipc_dump_devinfo devinfo;
    struct fipc_dump_devinfo *pdevinfo = &devinfo;

    u32 used_len = sizeof(devinfo);
    if (len < used_len) {
        return 0;
    }
    pdevinfo->devid = pdev->id;
    pdevinfo->cpu_cnt = DPA_FIPC_CPU_ID_MAX;
    pdevinfo->pipepair_cnt = pdev->pipepair_cnt;
    if (memcpy_s(buf, len, &devinfo, sizeof(devinfo))) {
        return 0;
    }
    return used_len;
}

static u32 fipc_cpu_regs_dump(struct fipc_device *pdev, u32 cpuid, u8 *buf, u32 len)
{
    struct fipc_dump_cpuregsinfo cpu_regs;
    struct fipc_dump_cpuregsinfo *pcpu_regs = &cpu_regs;
    char *base_addr = (char *)pdev->base_va;
    u32 reg_offset = pdev->config_diff->core_pipe_irq_off;
    u32 used_len = sizeof(cpu_regs);
    if (len < used_len) {
        return 0;
    }
    (void)memset_s((void*)pcpu_regs, sizeof(cpu_regs), 0, sizeof(cpu_regs));
    pcpu_regs->magic = CPU_REG_MAGIC;
    pcpu_regs->cpuid = cpuid;
    pcpu_regs->omask0 = readl((void *)(base_addr + FIPC_CORE_OPIPE_INT_MASK0(cpuid, reg_offset)));
    pcpu_regs->omask1 = readl((void *)(base_addr + FIPC_CORE_OPIPE_INT_MASK1(cpuid, reg_offset)));
    pcpu_regs->imask0 = readl((void *)(base_addr + FIPC_CORE_IPIPE_INT_MASK0(cpuid, reg_offset)));
    pcpu_regs->imask1 = readl((void *)(base_addr + FIPC_CORE_IPIPE_INT_MASK1(cpuid, reg_offset)));

    pcpu_regs->ostat0 = readl((void *)(base_addr + FIPC_CORE_OPIPE_INT_STAT0(cpuid, reg_offset)));
    pcpu_regs->ostat1 = readl((void *)(base_addr + FIPC_CORE_OPIPE_INT_STAT1(cpuid, reg_offset)));
    pcpu_regs->istat0 = readl((void *)(base_addr + FIPC_CORE_IPIPE_INT_STAT0(cpuid, reg_offset)));
    pcpu_regs->istat1 = readl((void *)(base_addr + FIPC_CORE_IPIPE_INT_STAT1(cpuid, reg_offset)));

    if (memcpy_s(buf, len, &cpu_regs, sizeof(cpu_regs))) {
        return 0;
    }
    return used_len;
}

static u32 fipc_cpus_dump(struct fipc_device *pdev, u32 cpu_num, u8 *buf, u32 len)
{
    u32 cpuid;
    u32 ret_len;
    u32 used_len = 0;

    if (pdev == NULL || buf == NULL) {
        return 0;
    }

    for (cpuid = 0; cpuid < cpu_num; cpuid++) {
        ret_len = fipc_cpu_regs_dump(pdev, cpuid, buf + used_len, len - used_len);
        if (ret_len == 0) {
            /* dump空间不够 没记录完 */
            fipc_print_error("fipc no enough dump space: cpus\n");
            return used_len;
        }
        used_len += ret_len;
    }
    return used_len;
}

struct fipc_dump_piperegsinfo {
    u32 magic;
    u32 pipeid;
    u32 owptr; /* 'OPIPE_WPTR */
    u32 orptr; /* 'OPIPE_RPTR */
    u32 omask; /* 'OPIPE_INT_MASK */
    u32 oraw;  /* 'OPIPE_INT_RAW */
    u32 octrl; /* 'OPIPE_CTRL */
    u32 ostat; /* 'OPIPE_STAT */
    u32 odbg0; /* 'OPIPE_DBG0' */
    u32 odbg1; /* 'OPIPE_DBG1 */
    u32 obase_addr_h; /* 'OPIPE_BASE_ADDR_H */
    u32 obase_addr_l; /* 'OPIPE_BASE_ADDR_L */
    u32 iwptr; /* 'IPIPE_WPTR */
    u32 irptr; /* 'IPIPE_RPTR */
    u32 irmte; /* 'IPIPE_REMOTE_CTRL */
    u32 imask; /* 'IPIPE_INT_MASK */
    u32 iraw;  /* 'IPIPE_INT_RAW */
    u32 ictrl; /* 'IPIPE_CTRL */
    u32 istat; /* 'IPIPE_STAT */
    u32 idbg0; /* 'IPIPE_DBG0 */
    u32 ibase_addr_h; /* 'IPIPE_BASE_ADDR_H */
    u32 ibase_addr_l; /* 'IPIPE_BASE_ADDR_L */
    u32 icfg_h; /* 'IPIPE_CFG_ADDR_H */
    u32 icfg_l; /* 'IPIPE_CFG_ADDR_L */
};

u32 fipc_hw_dump_size(void)
{
    u32 cpu_reg_size = sizeof(struct fipc_dump_cpuregsinfo) * DPA_FIPC_CPU_ID_MAX;
    u32 dev_size = sizeof(struct fipc_dump_devinfo);
    u32 pipe_reg_size = sizeof(struct fipc_dump_piperegsinfo) * DPA_FIPC_PIPE_MAX;
    u32 head_size = sizeof(struct fipc_dump_tlv);
    u32 dump_head = sizeof(struct fipc_dump_head);

    return (cpu_reg_size + dev_size + pipe_reg_size + head_size + dump_head);
}

static u32 fipc_pipe_regs_dump(struct fipc_device *pdev, u32 pipeid, u8 *buf, u32 len)
{
    struct fipc_dump_piperegsinfo pipe_regs;
    struct fipc_dump_piperegsinfo *ppipe_regs = &pipe_regs;
    char *base_addr = (char *)pdev->base_va;

    u32 used_len = sizeof(pipe_regs);
    if (len < used_len) {
        return 0;
    }
    (void)memset_s((void*)ppipe_regs, sizeof(pipe_regs), 0, sizeof(pipe_regs));
    ppipe_regs->magic = PIPE_REG_MAGIC;
    ppipe_regs->pipeid = pipeid;
    ppipe_regs->owptr = readl((void *)(base_addr + FIPC_OPIPE_WPTR(pipeid)));
    ppipe_regs->orptr = readl((void *)(base_addr + FIPC_OPIPE_RPTR(pipeid)));
    ppipe_regs->omask = readl((void *)(base_addr + FIPC_OPIPE_INT_MASK(pipeid)));
    ppipe_regs->oraw = readl((void *)(base_addr + FIPC_OPIPE_INT_RAW(pipeid)));
    ppipe_regs->octrl = readl((void *)(base_addr + FIPC_OPIPE_CTRL(pipeid)));
    ppipe_regs->ostat = readl((void *)(base_addr + FIPC_OPIPE_STAT(pipeid)));
    ppipe_regs->odbg0 = readl((void *)(base_addr + FIPC_OPIPE_DBG0(pipeid)));
    ppipe_regs->odbg1 = readl((void *)(base_addr + FIPC_OPIPE_DBG1(pipeid)));
    ppipe_regs->obase_addr_h = readl((void *)(base_addr + FIPC_OPIPE_BASE_ADDR_H(pipeid)));
    ppipe_regs->obase_addr_l = readl((void *)(base_addr + FIPC_OPIPE_BASE_ADDR_L(pipeid)));

    ppipe_regs->iwptr = readl((void *)(base_addr + FIPC_IPIPE_WPTR(pipeid)));
    ppipe_regs->irptr = readl((void *)(base_addr + FIPC_IPIPE_RPTR(pipeid)));
    ppipe_regs->irmte = readl((void *)(base_addr + FIPC_IPIPE_REMOTE_CTRL(pipeid)));
    ppipe_regs->imask = readl((void *)(base_addr + FIPC_IPIPE_INT_MASK(pipeid)));
    ppipe_regs->iraw = readl((void *)(base_addr + FIPC_IPIPE_INT_RAW(pipeid)));
    ppipe_regs->ictrl = readl((void *)(base_addr + FIPC_IPIPE_CTRL(pipeid)));
    ppipe_regs->istat = readl((void *)(base_addr + FIPC_IPIPE_STAT(pipeid)));
    ppipe_regs->idbg0 = readl((void *)(base_addr + FIPC_IPIPE_DBG0(pipeid)));
    ppipe_regs->ibase_addr_h = readl((void *)(base_addr + FIPC_IPIPE_BASE_ADDR_H(pipeid)));
    ppipe_regs->ibase_addr_l = readl((void *)(base_addr + FIPC_IPIPE_BASE_ADDR_L(pipeid)));
    ppipe_regs->icfg_h = readl((void *)(base_addr + FIPC_IPIPE_CFG_ADDR_H(pipeid)));
    ppipe_regs->icfg_l = readl((void *)(base_addr + FIPC_IPIPE_CFG_ADDR_L(pipeid)));

    if (memcpy_s(buf, len, &pipe_regs, sizeof(pipe_regs))) {
        return 0;
    }
    return used_len;
}

static u32 fipc_pipes_dump(struct fipc_device *pdev, u8 *buf, u32 len)
{
    u32 pipeid;
    u32 ret_len;
    u32 used_len = 0;

    if (pdev == NULL || buf == NULL) {
        return 0;
    }

    for (pipeid = 0; pipeid < pdev->pipepair_cnt; pipeid++) {
        ret_len = fipc_pipe_regs_dump(pdev, pipeid, buf + used_len, len - used_len);
        if (ret_len == 0) {
            /* dump空间不够 没记录完 */
            fipc_print_error("fipc no enough dump space: pipes\n");
            return used_len;
        }
        used_len += ret_len;
    }
    return used_len;
}


u32 fipc_hwregs_dump_save(u8 *buf, u32 len)
{
    u32 devid;
    u32 ret_len = 0;
    struct fipc_device *pdev = NULL;
    struct fipc_dump_tlv *tlv = (struct fipc_dump_tlv *)buf;
    u32 used_len = sizeof(struct fipc_dump_tlv);

    if (len < used_len) {
        return 0;
    }
    tlv->modid = FIPC_DUMP_HWREG;
    tlv->version = FIPC_DUMP_HWREG_VER;

    for (devid = 0; devid < FIPC_DEVICE_NUM_MAX; devid++) {
        pdev = fipc_device_get_fast(devid);
        if (pdev == NULL || pdev->ctrl_level != FIPC_DEV_CONTROL_LEVEL_LATE) {
            continue;
        }

        if (len < used_len) {
            break;
        }
        ret_len = fipc_device_dump(pdev, buf + used_len, len - used_len);
        if (ret_len == 0) {
            /* dump空间不够 没记录完 */
            fipc_print_error("fipc no enough dump space @%d\n", __LINE__);
            break;
        }
        used_len += ret_len;

        ret_len = fipc_cpus_dump(pdev, DPA_FIPC_CPU_ID_MAX,  buf + used_len, len - used_len);
        if (ret_len == 0) {
            /* dump空间不够 没记录完 */
            break;
        }
        used_len += ret_len;

        ret_len = fipc_pipes_dump(pdev, buf + used_len, len - used_len);
        if (ret_len == 0) {
            /* dump空间不够 没记录完 */
            break;
        }
        used_len += ret_len;
    }
    tlv->len = used_len - sizeof(struct fipc_dump_tlv);

    return used_len;
}

/* fipc dump使用tlv结构，便于脚本解析 */
void dpa_fipc_do_dump(u8 *buf, u32 len)
{
    u32 used_len;
    struct fipc_dump_head *dump_hdr = (struct fipc_dump_head *)buf;
    /* 写入magic num 和 版本号 长度， 占用16字节，方便对齐分析 */
    if (len < sizeof(struct fipc_dump_head)) {
        return;
    }

    dump_hdr->dump_start_time = fipc_get_slice_value();
    dump_hdr->version = FIPC_DUMP_VERSION;
    used_len = 0;
    dump_hdr->len = used_len;
    len -= sizeof(struct fipc_dump_head);
    buf += sizeof(struct fipc_dump_head);

    if (len - used_len > 0) {
        used_len += fipc_hwregs_dump_save(buf + used_len, len - used_len);
        dump_hdr->len = used_len;
    }

    dump_hdr->dump_end_time = fipc_get_slice_value();
    return;
}


void dpa_fipc_dump_init(u8 *buf, u32 len)
{
    struct fipc_dump_head *dump_hdr = (struct fipc_dump_head *)buf;
    /* 写入magic num 和 版本号 长度， 占用16字节，方便对齐分析 */
    if (len < sizeof(struct fipc_dump_head) || buf == NULL) {
        return;
    }

    dump_hdr->version = FIPC_DUMP_VERSION;
    dump_hdr->len = fipc_hw_dump_size() - sizeof(struct fipc_dump_head);
    return;
}


static u32 fipc_chn_dump(struct fipc_device *pdev, struct fipc_channel *pchannel, u8 *buf, u32 len)
{
    struct fipc_dump_chnsinfo chninfo;
    struct fipc_dump_chnsinfo *pchninfo = &chninfo;

    u32 used_len = sizeof(chninfo);
    if (len < used_len) {
        return 0;
    }
    pchninfo->ldrvchn_id = pchannel->ldrvchn_id;
    pchninfo->type = pchannel->type;
    if (pchannel->state == (u32)FIPC_CHN_INIT) {
        pchninfo->wptr = 0xDEADBEEF;
        pchninfo->rptr = 0xDEADBEEF;
    } else {
        if (isMsgSendChn(pchannel) || pchannel->type == FIPC_CHN_TYPE_DMA) {
            pchninfo->wptr = pchannel->opipe_cfg->wptr;
            pchninfo->rptr = *(pchannel->opipe_cfg->ptr_vaddr);
        } else {
            pchninfo->wptr = *(pchannel->ipipe_cfg->ptr_vaddr);
            pchninfo->rptr = pchannel->ipipe_cfg->rptr;
        }
    }

    pchninfo->dym_flags = pchannel->dym_flags;
    pchninfo->const_flags = pchannel->const_flags;
    if (memcpy_s(buf, len, &chninfo, sizeof(chninfo))) {
        return 0;
    }
    return used_len;
}

u32 fipc_chns_dump_save(u8 *buf, u32 len)
{
    u32 idx;
    u32 ret_len, used_len;
    struct fipc_device *pdev = NULL;
    struct fipc_channel *pchannel = NULL;

    idx = 0;
    used_len = 0;
    while (fipc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        if (len < used_len) {
            return used_len;
        }
        /* 没有打开的通道没必要记录 */
        if (pchannel->state == (u32)FIPC_CHN_INIT) {
            continue;
        }
        ret_len = fipc_chn_dump(pdev, pchannel, buf + used_len, len - used_len);
        if (ret_len == 0) {
            /* dump空间不够 没记录完 */
            fipc_print_error("fipc no enough dump space @%d\n", __LINE__);
            return used_len;
        }
        used_len += ret_len;
    };

    return used_len;
}

static u32 fipc_do_pmsr_dump(u8 *buf, u32 len)
{
    u32 ret_len;
    struct fipc_dump_tlv *tlv = (struct fipc_dump_tlv *)buf;
    u32 used_len = sizeof(struct fipc_dump_tlv);

    if (len < used_len) {
        return 0;
    }
    tlv->modid = FIPC_DUMP_PMSR;
    tlv->version = FIPC_DUMP_PMSR_VER;
    ret_len = fipc_pmsr_dump_save(buf + used_len, len - used_len);
    if (ret_len == 0) {
        /* 如果没有可save到内容，那该tlv头也不需要了 */
        fipc_print_error("fipc no enough dump space @%d\n", __LINE__);
        return 0;
    }
    tlv->len = ret_len;
    used_len += ret_len;
    return used_len;
}

u32 fipc_do_hwregs_dump(u8 *buf, u32 len)
{
    u32 ret_len;
    struct fipc_dump_tlv *tlv = (struct fipc_dump_tlv *)buf;
    u32 used_len = sizeof(struct fipc_dump_tlv);

    if (len < used_len) {
        return 0;
    }
    tlv->modid = FIPC_DUMP_HWREG;
    tlv->version = FIPC_DUMP_HWREG_VER;
    ret_len = fipc_hwregs_dump_save(buf + used_len, len - used_len);
    if (ret_len == 0) {
        /* 如果没有可save到内容，那该tlv头也不需要了 */
        fipc_print_error("fipc no enough dump space @%d\n", __LINE__);
        return 0;
    }
    tlv->len = ret_len;
    used_len += ret_len;
    return used_len;
}

static u32 fipc_do_chns_dump(u8 *buf, u32 len)
{
    u32 ret_len;
    struct fipc_dump_tlv *tlv = (struct fipc_dump_tlv *)buf;
    u32 used_len = sizeof(struct fipc_dump_tlv);

    if (len < used_len) {
        return 0;
    }
    tlv->modid = FIPC_DUMP_CHNS;
    tlv->version = FIPC_DUMP_CHNS_VER;
    ret_len = fipc_chns_dump_save(buf + used_len, len - used_len);
    if (ret_len == 0) {
        /* 如果没有可save到内容，那该tlv头也不需要了 */
        fipc_print_error("fipc no enough dump space @%d\n", __LINE__);
        return 0;
    }
    tlv->len = ret_len;
    used_len += ret_len;
    return used_len;
}


static u32 fipc_do_hifi_dbg_dump(u8 *buf, u32 len)
{
    u32 ret_len;
    struct fipc_dump_tlv *tlv = (struct fipc_dump_tlv *)buf;
    u32 used_len = sizeof(struct fipc_dump_tlv);

    if (len < used_len) {
        return 0;
    }
    tlv->modid = FIPC_DUMP_HIFI_DBG;
    tlv->version = FIPC_DUMP_HIFI_DBG_VER;
    ret_len = fipc_hifi_dbg_dump_save(buf + used_len, len - used_len);
    if (ret_len == 0) {
        /* 如果没有可save到内容，那该tlv头也不需要了 */
        fipc_print_warn("fipc no enough dump space or no content to dump @%d\n", __LINE__);
        return 0;
    }
    tlv->len = ret_len;
    used_len += ret_len;
    return used_len;
}

/* fipc dump使用tlv结构，便于脚本解析 */
void fipc_do_dump(u8 *buf, u32 len)
{
    u32 used_len;
    struct fipc_dump_head *dump_hdr = (struct fipc_dump_head *)buf;
    /* 写入magic num 和 版本号 长度， 占用16字节，方便对齐分析 */
    if (len < sizeof(struct fipc_dump_head)) {
        return;
    }

    dump_hdr->dump_start_time = fipc_get_slice_value();
    dump_hdr->version = FIPC_DUMP_VERSION;
    used_len = 0;
    dump_hdr->len = used_len;
    len -= sizeof(struct fipc_dump_head);
    buf += sizeof(struct fipc_dump_head);

    if (len - used_len > 0) {
        used_len += fipc_do_pmsr_dump(buf + used_len, len - used_len);
        dump_hdr->len = used_len;
    }

    if (len - used_len > 0) {
        used_len += fipc_do_chns_dump(buf + used_len, len - used_len);
        dump_hdr->len = used_len;
    }

    if (len - used_len > 0) {
        used_len += fipc_do_hifi_dbg_dump(buf + used_len, len - used_len);
        dump_hdr->len = used_len;
    }
    dump_hdr->dump_end_time = fipc_get_slice_value();
    return;
}

int fipc_ringbuf_dump_head_update(struct fipc_ringbuf_dump_head *pfipc_dump_head,
    struct fipc_ringbuf_dump_addr *pfipc_dump_addr, struct fipc_channel *pchannel)
{
    pfipc_dump_head->chnl_idx = pchannel->user_id;
    pfipc_dump_head->chnl_type = pchannel->type;
    if (isMsgRecvChn(pchannel)) {
        if (pchannel->ipipe_cfg == NULL) {
            pfipc_dump_head->ring_buf_len = 0;
            return -1;
        }
        pfipc_dump_head->ring_buf_len = pchannel->ipipe_cfg->depth;
        pfipc_dump_addr->ringbuf_va = pchannel->ipipe_cfg->ringbuf_va;
        pfipc_dump_addr->ringbuf_pa = pchannel->ipipe_cfg->ringbuf_pa;
    } else {
        if (pchannel->opipe_cfg == NULL) {
            pfipc_dump_head->ring_buf_len = 0;
            return -1;
        }
        pfipc_dump_head->ring_buf_len = pchannel->opipe_cfg->depth;
        pfipc_dump_addr->ringbuf_va = pchannel->opipe_cfg->ringbuf_va;
        pfipc_dump_addr->ringbuf_pa = pchannel->opipe_cfg->ringbuf_pa;
    }
    if (pfipc_dump_addr->ringbuf_va == NULL) {
        pfipc_dump_head->ring_buf_len = 0;
    }
    return 0;
}

void fipc_ringbuf_dump_save(const char *dir_name)
{
    int ret;
    u32 idx = 0;
    struct fipc_device *pdev = NULL;
    struct fipc_channel *pchannel = NULL;
    struct fipc_ringbuf_dump_head fipc_dump_head;
    struct fipc_ringbuf_dump_addr fipc_dump_addr;

    if (dir_name == NULL) {
        fipc_print_always("fipc dump failed!");
        return;
    }
    fipc_dump_head.head_tag = 0xffffffff;
    while (fipc_dev_chn_get_byidx(idx, &pdev, &pchannel) == 0) {
        idx++;
        /* 没有打开的通道没必要记录 */
        if (pchannel->state == (u32)FIPC_CHN_INIT || (pchannel->const_flags & FIPC_CHN_SF_DUMP_RINGBUF_CARE) == 0) {
            continue;
        }
        ret = fipc_ringbuf_dump_head_update(&fipc_dump_head, &fipc_dump_addr, pchannel);
        if (ret != 0) {
            continue;
        }
        bsp_dump_log_append(dir_name, "fipc_dump.bin", (void *)(uintptr_t)&fipc_dump_head, NULL,
            sizeof(fipc_dump_head));
        if (fipc_dump_head.ring_buf_len == 0) {
            continue;
        }
        bsp_dump_log_append(dir_name, "fipc_dump.bin", fipc_dump_addr.ringbuf_va,
            (void *)(uintptr_t)(fipc_dump_addr.ringbuf_pa), fipc_dump_head.ring_buf_len);
    }
    fipc_print_always("fipc dump done!");
}

void fipc_ringbuf_dump_init(void)
{
    if (0 != bsp_dump_register_log_notifier(DUMP_SAVE_FIPC_INFO, fipc_ringbuf_dump_save)) {
        fipc_print_error("fipc_ringbuf_dump init failed");
    }
}

#else
void fipc_do_dump(u8 *buf, u32 len)
{
    return;
}
void fipc_ringbuf_dump_init(void)
{
    return;
}
#endif
