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
#include <linux/dma-mapping.h>
#include <osl_types.h>
#include <osl_sem.h>
#include <mdrv_diag.h>
#include <bsp_version.h>
#include <bsp_odt.h>
#include <bsp_dump.h>
#include "diag_debug.h"
#include "scm_src.h"
#include "scm_dst.h"
#include <osl_malloc.h>
#include "product_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

// 定义在product_config中，支持平台maxwell、charllotte、prome，未防止遗漏增加默认值定义
#ifndef SCM_SRC_CNF_BUFFER_SIZE
#define SCM_SRC_CNF_BUFFER_SIZE       0x4000  // 16K
#endif
#ifndef SCM_SRC_IND_BUFFER_SIZE
#define SCM_SRC_IND_BUFFER_SIZE       0x8000  // 32K
#endif
#define SCM_SRC_MXT_BUFFER_SIZE       0x8000  // 32K
#define SCM_SRC_NO_CODER_BUFFER_SIZE  0x8000  // 32K

// 一级头 + 二级头最大长度，二级头只有cnf和ind，cnf头长度大于ind，这里去cnf头长度
#define OAM_MSG_HDR_LEN_MAX (sizeof(oam_hdr_s) + DIAG_CNF_INFO_HEAD_LEN)

char *cnf_get_data(char *msg_hdr);
char *ind_get_data(char *msg_hdr);

scm_src_cfg_s g_src_chan_info[SCM_SRC_CHAN_BUTT] = {
    {
        .init_state = SCM_CHANNEL_UNINIT,
        .chan_id = ODT_CODER_SRC_ACPU_CNF,
        .dst_chan_id = ODT_CODER_DST_OM_CNF,
        .data_type_en = ODT_DATA_TYPE_EN,
        .data_type = ODT_DATA_TYPE_0,
        .chan_mode = ODT_ENCSRC_CHNMODE_CTSPACKET,
        .chan_prio = ODT_CHAN_PRIORITY_3,
        .bypass_en = ODT_HDLC_ENABLE,
        .buff_len = SCM_SRC_CNF_BUFFER_SIZE,

        .msg_mt = DIAG_FRAME_MT_CNF,
        .msg_hdr_len = DIAG_CNF_INFO_HEAD_LEN,
        .get_data = cnf_get_data,
        .scm_src_chan_id = SCM_CNF_SRC_CHAN,
    },
    {
        .init_state = SCM_CHANNEL_UNINIT,
        .chan_id = ODT_CODER_SRC_ACPU_IND,
        .dst_chan_id = ODT_CODER_DST_OM_IND,
        .data_type_en = ODT_DATA_TYPE_EN,
        .data_type = ODT_DATA_TYPE_0,
        .chan_mode = ODT_ENCSRC_CHNMODE_CTSPACKET,
        .chan_prio = ODT_CHAN_PRIORITY_1,
        .bypass_en = ODT_HDLC_ENABLE,
        .buff_len = SCM_SRC_IND_BUFFER_SIZE,

        .msg_mt = DIAG_FRAME_MT_IND,
        .msg_hdr_len = DIAG_IND_INFO_HEAD_LEN,
        .get_data = ind_get_data,
        .scm_src_chan_id = SCM_IND_SRC_CHAN,
    },
    {
        .init_state = SCM_CHANNEL_UNINIT,
        .chan_id = ODT_CODER_SRC_ACPU_NO_CODER_IND,
        .dst_chan_id = ODT_CODER_DST_OM_NO_CODER_IND,
        .data_type_en = ODT_DATA_TYPE_DIS,
        .data_type = ODT_DATA_TYPE_0,
        .chan_mode = ODT_ENCSRC_CHNMODE_CTSPACKET,
        .chan_prio = ODT_CHAN_PRIORITY_2,
        .bypass_en = ODT_HDLC_DISABLE,
        .buff_len = SCM_SRC_NO_CODER_BUFFER_SIZE,

        .msg_mt = DIAG_FRAME_MT_IND,
        .msg_hdr_len = DIAG_IND_INFO_HEAD_LEN,
        .get_data = ind_get_data,
        .scm_src_chan_id = SCM_NO_CODER_SRC_CHAN,
    },
    {
        .init_state = SCM_CHANNEL_UNINIT,
        .chan_id = ODT_CODER_SRC_AP_MPROBE,
        .dst_chan_id = ODT_CODER_DST_LOGSERVER,
        .data_type_en = ODT_DATA_TYPE_EN,
        .data_type = ODT_DATA_TYPE_0,
        .chan_mode = ODT_ENCSRC_CHNMODE_CTSPACKET,
        .chan_prio = ODT_CHAN_PRIORITY_1,
        .bypass_en = ODT_HDLC_ENABLE,
        .buff_len = SCM_SRC_MXT_BUFFER_SIZE,

        .msg_mt = DIAG_FRAME_MT_IND,
        .msg_hdr_len = DIAG_IND_INFO_HEAD_LEN,
        .get_data = ind_get_data,
        .scm_src_chan_id = SCM_MXT_SRC_CHAN,
    },
};

void scm_clear_diag_tool_src_buff(void)
{
    bsp_clear_odt_src_buffer(g_src_chan_info[SCM_IND_SRC_CHAN].chan_id);
}

int scm_src_chan_items_init(struct platform_device *pdev)
{
    int ret, i;
    scm_src_cfg_s *chan_attr;
    odt_src_chan_cfg_s odt_chan_info;

    for (i = 0; i < SCM_SRC_CHAN_BUTT; i++) {
        chan_attr = &g_src_chan_info[i];

        spin_lock_init(&chan_attr->src_lock);
        chan_attr->oam_msg_hdr = osl_malloc(OAM_MSG_HDR_LEN_MAX);
        if (chan_attr->oam_msg_hdr == NULL) {
            diag_err("scm_src_chan[%u] init oam_msg_hdr fail\n", i);
            return DIAG_ERRNO_MALLOC_FAIL;
        }

        chan_attr->virt_buff = (unsigned char *)dma_alloc_coherent(&pdev->dev, chan_attr->buff_len,
            (dma_addr_t *)&chan_attr->phy_buff, GFP_KERNEL);
        if (chan_attr->virt_buff == NULL) {
            diag_err("src chan buff alloc fail, i=0x%x\n", i);
            chan_attr->init_state = SCM_CHANNEL_MEM_FAIL;
            return DIAG_ERRNO_MALLOC_FAIL;
        }

        odt_chan_info.dest_chan_id = chan_attr->dst_chan_id;
        odt_chan_info.data_type = chan_attr->data_type;
        odt_chan_info.mode = chan_attr->chan_mode;
        odt_chan_info.priority = chan_attr->chan_prio;
        odt_chan_info.bypass_en = chan_attr->bypass_en;
        odt_chan_info.data_type_en = chan_attr->data_type_en;
        odt_chan_info.trans_id_en = ODT_TRANS_ID_DIS;
        odt_chan_info.rptr_img_en = ODT_PTR_IMG_DIS;
        odt_chan_info.debug_en = ODT_ENC_DEBUG_DIS;
        odt_chan_info.rptr_img_en = ODT_PTR_IMG_DIS,

        odt_chan_info.coder_src_setbuf.input_start = chan_attr->phy_buff;
        odt_chan_info.coder_src_setbuf.input_end = (chan_attr->phy_buff + chan_attr->buff_len) - 1;

        ret = bsp_odt_coder_set_src_chan(chan_attr->chan_id, &odt_chan_info);
        if (ret) {
            diag_err("src chan cfg fail, chan_id=0x%x, ret=0x%x\n", chan_attr->chan_id, ret);
            chan_attr->init_state = SCM_CHANNEL_CFG_FAIL;
            return ret;
        }
        
        ret = bsp_odt_start(chan_attr->chan_id);
        if (ret) {
            diag_err("src chan start fail, chan_id=0x%x, ret=0x%x\n", chan_attr->chan_id, ret);
            return ret;
        }
        chan_attr->init_state = SCM_CHANNEL_INIT_SUCC; /* 记录通道初始化配置成功 */
    }
    return 0;
}

int scm_src_chan_init(void)
{
    struct platform_device *pdev = NULL;

    pdev = bsp_get_odt_pdev();
    if (pdev == NULL) {
        diag_err("get odt pdev fail\n");
        return DIAG_ERRNO_GET_PDEV_FAIL;
    }
    return scm_src_chan_items_init(pdev);
}

int scm_init(void)
{
    int ret;

    ret = scm_dst_chan_init();
    if (ret) {
        diag_err("scm dst chan init fail:0x%x\n", ret);
        return ret;
    }

    ret = scm_src_chan_init();
    if (ret) {
        diag_err("scm src chan init fail:0x%x\n", ret);
        return ret;
    }

    return BSP_OK;
}

char *cnf_get_data(char *msg_hdr)
{
    diag_cnf_info_s *cnf_msg = (diag_cnf_info_s *)msg_hdr;
    return cnf_msg->data;
}

char *ind_get_data(char *msg_hdr)
{
    diag_ind_info_s *ind_msg = (diag_ind_info_s *)msg_hdr;
    return ind_msg->data;
}

#define ENCODER_MAX_TRANS_SIZE 4096 // bytes
#define ENCODER_MAX_TRANS_PAYLOAD (ENCODER_MAX_TRANS_SIZE - sizeof(oam_hdr_s))

int scm_unreel_process(odt_buffer_rw_s *odt_buffer, scm_frame_s *scm_frame)
{
    if (odt_buffer->size == scm_frame->head_size) {
        /* 恰好消息头长度跟剩余buffer长度相等，只需要head和data分别拷贝到buffer和rb_buffer */
        if (memcpy_s(odt_buffer->buffer, odt_buffer->size, scm_frame->head, scm_frame->head_size)) {
        return DIAG_ERRNO_MEMCPY_FAIL;
        }
        if (memcpy_s(odt_buffer->rb_buffer, odt_buffer->rb_size, scm_frame->data, scm_frame->data_size)) {
        return DIAG_ERRNO_MEMCPY_FAIL;
        }
    } else if (odt_buffer->size > scm_frame->head_size) { 
        /* 消息头长度比剩余buffer空间小，需要将消息头先拷贝到剩余buffer，再将data中部分数据拷贝到剩余buffer，最后data剩余部分数据拷贝到rb_buffer */
        if (memcpy_s(odt_buffer->buffer, odt_buffer->size, scm_frame->head, scm_frame->head_size)) {
            return DIAG_ERRNO_MEMCPY_FAIL;
        }
        odt_buffer->buffer += scm_frame->head_size;
        odt_buffer->size -= scm_frame->head_size;
        if (memcpy_s(odt_buffer->buffer, odt_buffer->size, scm_frame->data, odt_buffer->size)) {
            return DIAG_ERRNO_MEMCPY_FAIL;
        }
        scm_frame->data += odt_buffer->size;
        scm_frame->data_size -= odt_buffer->size;
        if (memcpy_s(odt_buffer->rb_buffer, odt_buffer->rb_size, scm_frame->data, scm_frame->data_size)) {
            return DIAG_ERRNO_MEMCPY_FAIL;
        }
    } else {
        /* 消息头长度比剩余buffer空间大，需要将head部分数据拷贝到剩余buffer，再将head剩余数据先拷贝到rb_buffer，最后将data数据拷贝到rb_buffer剩余空间 */
        if (memcpy_s(odt_buffer->buffer, odt_buffer->size, scm_frame->head, odt_buffer->size)) {
            return DIAG_ERRNO_MEMCPY_FAIL;
        }
        scm_frame->head += odt_buffer->size;
        scm_frame->head_size -= odt_buffer->size;
        if (memcpy_s(odt_buffer->rb_buffer, odt_buffer->rb_size, scm_frame->head, scm_frame->head_size)) {
            return DIAG_ERRNO_MEMCPY_FAIL;
        }
        odt_buffer->rb_buffer += scm_frame->head_size;
        odt_buffer->rb_size -= scm_frame->head_size;
        if (memcpy_s(odt_buffer->rb_buffer, odt_buffer->rb_size, scm_frame->data, scm_frame->data_size)) {
            return DIAG_ERRNO_MEMCPY_FAIL;
        }
    }
    return 0;
}

int scm_direct_process(odt_buffer_rw_s *odt_buffer, scm_frame_s *scm_frame)
{
    if (memcpy_s(odt_buffer->buffer, odt_buffer->size, scm_frame->head, scm_frame->head_size)) {
        return DIAG_ERRNO_MEMCPY_FAIL;
    }
    if (memcpy_s((void *)((uintptr_t)odt_buffer->buffer + (uintptr_t)scm_frame->head_size), odt_buffer->size - scm_frame->head_size,
        scm_frame->data, scm_frame->data_size)) {
        return DIAG_ERRNO_MEMCPY_FAIL;
    }
    return 0;
}

int scm_write_to_odt(scm_src_cfg_s *src_cfg, scm_frame_s *scm_frame)
{
    int ret;
    odt_buffer_rw_s odt_buffer = {0};
    // 只有尾部长度可能不对齐，但包头长度指示目的端取走对应有效部分，舍弃padding
    unsigned int len = SCM_ALIGN_8_BYTES(scm_frame->data_size + scm_frame->head_size);

    ret = bsp_odt_get_write_buff(src_cfg->chan_id, &odt_buffer);
    if (ret) {
        return ret;
    }
    if ((odt_buffer.size + odt_buffer.rb_size) < len) {
        return DIAG_ERRNOR_NO_FREE_SPACE;
    }

    odt_buffer.buffer = (char *)(src_cfg->virt_buff + ((unsigned char *)odt_buffer.buffer - src_cfg->phy_buff));
    odt_buffer.rb_buffer = (char *)(src_cfg->virt_buff + ((unsigned char *)odt_buffer.rb_buffer - src_cfg->phy_buff));

    if (odt_buffer.size >= scm_frame->head_size + scm_frame->data_size) {  /* 多数场景，消息头+数据包的拷贝不需要回卷 */
        ret = scm_direct_process(&odt_buffer, scm_frame);
        if (ret) {
            return ret;
        }
    } else { /* 消息头+数据包的拷贝出现回卷的场景 */
        ret = scm_unreel_process(&odt_buffer, scm_frame);
        if (ret) {
            return ret;
        }
    }

    return bsp_odt_write_done(src_cfg->chan_id, len);
}

int scm_full_pkt_send(scm_src_cfg_s *src_cfg, char *hdr, unsigned int hdr_len,
    char *data, unsigned int data_len)
{
    scm_frame_s scm_frame;
    oam_hdr_s *oam_hdr = (oam_hdr_s *)hdr;
    int ret;

    oam_hdr->odt_len = hdr_len + data_len - DIAG_ODT_HEAD_SIZE; // 不带odt的整个payload长度
    oam_hdr->fragment = 0;
    oam_hdr->ff_flag = 0; // 0-代表不分片; 1-代表分片;
    oam_hdr->eof_flag = 0; // 是否尾分片
    oam_hdr->trans_id = src_cfg->trans_id++;

    scm_frame.head = hdr;
    scm_frame.head_size = hdr_len;
    scm_frame.data = data;
    scm_frame.data_size = data_len;
    ret = scm_write_to_odt(src_cfg, &scm_frame);
    if (ret) {
        g_diag_dbg.mntn_data.scm_src_dbg[src_cfg->scm_src_chan_id].scm_send_full_pkt_fail++;
        g_diag_dbg.mntn_data.scm_src_dbg[src_cfg->scm_src_chan_id].scm_no_space += (ret == DIAG_ERRNOR_NO_FREE_SPACE);
    } else {
        g_diag_dbg.mntn_data.scm_src_dbg[src_cfg->scm_src_chan_id].scm_send_full_pkt_succ++;
        g_diag_dbg.mntn_data.scm_src_dbg[src_cfg->scm_src_chan_id].scm_send_len += hdr_len + data_len;
    }
    return ret;
}

int scm_first_seg_send(scm_src_cfg_s *src_cfg, char *hdr, unsigned int hdr_len,
    char *data, unsigned int data_len)
{
    scm_frame_s scm_frame;
    oam_hdr_s *oam_hdr = (oam_hdr_s *)hdr;
    int ret;

    oam_hdr->odt_len = hdr_len + data_len - DIAG_ODT_HEAD_SIZE;
    oam_hdr->fragment = 0;
    oam_hdr->ff_flag = 1; // 0-代表不分片; 1-代表分片;
    oam_hdr->eof_flag = 0; // 是否尾分片
    oam_hdr->trans_id = src_cfg->trans_id++;

    scm_frame.head = hdr;
    scm_frame.head_size = hdr_len;
    scm_frame.data = data;
    scm_frame.data_size = data_len;
    ret = scm_write_to_odt(src_cfg, &scm_frame);
    if (ret) {
        g_diag_dbg.mntn_data.scm_src_dbg[src_cfg->scm_src_chan_id].scm_send_first_seg_fail++;
        g_diag_dbg.mntn_data.scm_src_dbg[src_cfg->scm_src_chan_id].scm_no_space += (ret == DIAG_ERRNOR_NO_FREE_SPACE);
    } else {
        g_diag_dbg.mntn_data.scm_src_dbg[src_cfg->scm_src_chan_id].scm_send_first_seg_succ++;
        g_diag_dbg.mntn_data.scm_src_dbg[src_cfg->scm_src_chan_id].scm_send_len += hdr_len + data_len;
    }
    return ret;
}

int scm_other_seg_send(scm_src_cfg_s *src_cfg, char *hdr, unsigned int hdr_len,
    char *data, unsigned int data_len, bool is_tail)
{
    scm_frame_s scm_frame;
    oam_hdr_s *oam_hdr = (oam_hdr_s *)hdr;
    int ret;

    oam_hdr->odt_len = hdr_len + data_len - DIAG_ODT_HEAD_SIZE;
    oam_hdr->fragment++;
    oam_hdr->ff_flag = 1; // 0-代表不分片; 1-代表分片;
    oam_hdr->eof_flag = is_tail; // 是否尾分片

    scm_frame.head = hdr;
    scm_frame.head_size = hdr_len;
    scm_frame.data = data;
    scm_frame.data_size = data_len;
    ret = scm_write_to_odt(src_cfg, &scm_frame);
    if (ret) {
        g_diag_dbg.mntn_data.scm_src_dbg[src_cfg->scm_src_chan_id].scm_send_other_seg_fail++;
        g_diag_dbg.mntn_data.scm_src_dbg[src_cfg->scm_src_chan_id].scm_no_space += (ret == DIAG_ERRNOR_NO_FREE_SPACE);
    } else {
        g_diag_dbg.mntn_data.scm_src_dbg[src_cfg->scm_src_chan_id].scm_send_other_seg_succ++;
        g_diag_dbg.mntn_data.scm_src_dbg[src_cfg->scm_src_chan_id].scm_send_len += hdr_len + data_len;
    }
    return ret;
}

// 返回生成好的oam_hdr + msg_hdr， 出参hdr_len指示oam_hdr + msg_hdr总长度，使用后需osl_free释放hdr占用内存
char* scm_oam_hdr_new(scm_src_cfg_s *src_cfg, unsigned int *hdr_len, unsigned char *msg)
{
    unsigned char *hdr = NULL;
    unsigned char *msg_hdr = NULL;
    oam_hdr_s *oam_hdr;

    *hdr_len = sizeof(oam_hdr_s) + src_cfg->msg_hdr_len;
    hdr = src_cfg->oam_msg_hdr;
    (void)memset_s(hdr, *hdr_len, 0, *hdr_len);

    oam_hdr = (oam_hdr_s *)hdr;
    oam_hdr->odt_magic = 0x48495349; /* 特定magic num, 不允许修改 */

    if (src_cfg->dst_chan_id == ODT_CODER_DST_OM_NO_CODER_IND) {
        oam_hdr->sid = DIAG_SID_HMI_SRV;
    } else {
        oam_hdr->sid = DIAG_SID_DIAG_TOOL;
    }
    oam_hdr->ver = DIAG_FRAME_VER_2;
    oam_hdr->ssid = DIAG_FRAME_SSID_APP_CPU;
    oam_hdr->mt = src_cfg->msg_mt;
    oam_hdr->timestamp = bsp_get_slice_value();

    msg_hdr = hdr + sizeof(oam_hdr_s);
    if (memcpy_s(msg_hdr, src_cfg->msg_hdr_len, msg, src_cfg->msg_hdr_len)) {
        return NULL;
    }
    return hdr;
}

// 入参data_len指msg->data段数据长度，不包括二级头和三级头（如果有三级头）
// 拆分MTU=4k，加oam_hdr,进行传输；如果ring_buf空间不足时，尽可能多的传输数据块
// 1.首分片特殊处理，组合oam_hdr + msg_hdr + data_seg1
// 2.其他分片直接oam_hdr+data_segx
//             +---------+-----------------------------------+
//             | msg_hdr |          data                     |
//             +---------+-----------------------------------+
//                |            |                         |        
//   +-------+-------+------------------+     +-------+-----------------+
//   |oam_hdr|msg_hdr|    data_seg_1    |     |oam_hdr|   data_seg_x    |
//   +-------+-------+------------------+     +------ +-----------------+
//                   <MAX_TRANS_PAYLOAD >             <MAX_TRANS_PAYLOAD>
int scm_write_data(scm_src_chan_e chan, unsigned char *msg, unsigned int data_len)
{
    int ret = -1;
    unsigned long flags;
    unsigned char *data = NULL;
    unsigned int data_seg_len;
    unsigned char *hdr = NULL;
    unsigned int hdr_len = 0;
    bool is_tail = false;
    scm_src_cfg_s *src_cfg = &g_src_chan_info[chan];

    g_diag_dbg.mntn_data.scm_src_dbg[chan].scm_cnt++;
    data = src_cfg->get_data(msg);

    spin_lock_irqsave(&src_cfg->src_lock, flags);
    hdr = scm_oam_hdr_new(src_cfg, &hdr_len, msg);
    if (hdr == NULL) {
        g_diag_dbg.mntn_data.scm_src_dbg[chan].scm_new_oam_hdr_fail++;
        goto out;
    }

    if ((sizeof(oam_hdr_s) + data_len) <= ENCODER_MAX_TRANS_SIZE) { // 无需分片，一次即可完成发送
        ret = scm_full_pkt_send(src_cfg, hdr, hdr_len, data, data_len - src_cfg->msg_hdr_len);
        goto out;
    }
    // 首分片发送
    data_len -= src_cfg->msg_hdr_len;
    data_seg_len = ENCODER_MAX_TRANS_PAYLOAD - src_cfg->msg_hdr_len;
    ret = scm_first_seg_send(src_cfg, hdr, hdr_len, data, data_seg_len);
    if (ret) {
        goto out;
    }
    data += data_seg_len;
    data_len -= data_seg_len;
    // 后续分片发送
    while (data_len > 0) {
        data_seg_len = ENCODER_MAX_TRANS_PAYLOAD;
        if (data_len <= ENCODER_MAX_TRANS_PAYLOAD) {
            data_seg_len = data_len;
            is_tail = true;
        }
        ret = scm_other_seg_send(src_cfg, hdr, sizeof(oam_hdr_s), data, data_seg_len, is_tail);
        if (ret) {
            goto out;
        }
        data += data_seg_len;
        data_len -= data_seg_len;
    }
out:
    spin_unlock_irqrestore(&src_cfg->src_lock, flags);
    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
